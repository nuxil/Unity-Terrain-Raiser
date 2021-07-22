/*
	This program is designed to read a unity terrain heightmap file (terrain.raw) directly.
	The program will raises the terrain based on the user input..
	Normally terrains are created near the lowest level possible, which in effect prevents you from digging valleys or trenches below that height
	To overcome this problem, the program will modify the colors to reflect the desired height change.
	
	This program supports both 8bit and 16bit heightmaps exported from unity.
	When exporting a heightmap in unity, the following settings are required.

	Depth       : 16 bit or 8 bit , Your choice
	Byte Order  : Windows (Little-endian)
	Flip        : Your choice
	
	Notice. Changeing the height of the map will cause loss in resolution, how much depends on your settings!.
		
	Using -b 8 on a 16bit heightmap will work fine.
	Using -b 16 on a 8bit heightmap will not work.
	
	
	Compile code with:
	gcc unity_terrain_raiser.c -o utr
	
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>


typedef struct sHeightMap {
	char *filename_in;
	char *filename_out;
	int bitmode;
	int alt;
	int depth;
	unsigned char *data8;
	unsigned short *data16;
	int size;
} sHeightMap;


/*
	print out help, info.
*/
void ShowHelp(char *prog) {
	printf("\n\
  Example usage:\n\
  %s -i terrain.raw -o new_terrain.raw -b 16 -a 350 -d 35\n\n\
  -i input file,  The name of the unity heightmap.\n\
  -o output file, The name of the new generated heightmap.\n\
  -b bit mode,    8bit or 16bit heightmap.\n\
  -a altitude,    Height of the terrain.\n\
  -d depth,       The depth to add to the heightmap.\n", prog);
}


/*
	Parse the command line arguments
*/
bool ParseCmd(int argc, char**argv, sHeightMap *heightmap) {
	
	if (argc != 11) {
		ShowHelp(argv[0]);
		return 0;
	}

	static struct option long_options[] =  {
		{"alt",      required_argument, 0, 'a'},
		{"bitmode",  required_argument, 0, 'b'},
		{"depth",    required_argument, 0, 'd'},
		{"fileIn",   required_argument, 0, 'i'},
		{"fileOut",  required_argument, 0, 'o'},
		{NULL, 0, NULL, 0}
	};
	
	int c;
	int option_index = 0;
	// loop over all of the options
	while ((c = getopt_long(argc, argv, "a:b:d:i:o:", long_options, &option_index)) != -1) {

		switch (c)
		{
			// ---------------------------------------
			// altitude
			case 'a': {
				heightmap->alt = strtol(optarg, NULL, 10);
				if (heightmap->alt <= 0) {
					printf("Error: The -a argument needs to be greater than 0\n");
					return 0;
				}
			} break;

			// ---------------------------------------
			// bitmode, 8 bit or 16 bit.
			case 'b': {
				heightmap->bitmode = strtol(optarg, NULL, 10);   
				if (!((heightmap->bitmode == 8) || (heightmap->bitmode == 16))) {
					printf("Error: The -b Flag needs to be: -b 8 or -b 16\n");
					return 0;
				}
			} break;

			// ---------------------------------------
			// depth.
			case 'd': {
				heightmap->depth = strtol(optarg, NULL, 10);
				if (heightmap->depth < 0) {
					printf("Error: The -d argument needs to be greater than 0\n");
					return 0;
				}
			} break;

			// ---------------------------------------
			// input file
			case 'i': {
				int len = strlen(optarg)+1;
				
				heightmap->filename_in = calloc(len, sizeof(char));
				if (!(memcpy(heightmap->filename_in,  optarg,  len * sizeof(char)))) {
					printf("Error: Failed to set the -i argument\n");
					return 0;
				};
			} break;
			
			// ---------------------------------------
			// output file.
			case 'o': {
				int len = strlen(optarg)+1;
				heightmap->filename_out = calloc(len, sizeof(char));
				if (!(memcpy(heightmap->filename_out,  optarg,  len * sizeof(char)))) {
					printf("Error: Failed to set the -o argument\n");
					return 0;
				}
			} break;			

			// ---------------------------------------
			case '?': {} break;
			default: {
				abort();
			}
		}
	} 
	return 1;
}

/*
	Read a unity terrain heightmap file
*/
bool ReadUnityMap(sHeightMap *heightmap) {

	// -------------------------------------------------------
    // Open the image file
	size_t res;
	FILE *fp;
    if ((fp = fopen(heightmap->filename_in, "rb")) == NULL) {
        printf("Error: fopen source-file!\n");
        return 0;
    }
	
	// -------------------------------------------------------
	// Get the size of the file
	fseek(fp, 0, SEEK_END);			// seek to end of file
	heightmap->size = ftell(fp);	// get current file pointer
	fseek(fp, 0, SEEK_SET);			// seek back to beginning of file


	// -------------------------------------------------------
	// Allocate memory and read it to the memory so changes can be done later on.
	
	// 8 bit mode
	if (heightmap->bitmode == 8) {
		heightmap->data8 = malloc(heightmap->size * sizeof(unsigned char));
		res = fread(heightmap->data8, heightmap->size, 1, fp);

	// 16 bit mode
	} else {
		heightmap->data16 = malloc((heightmap->size/2) * sizeof(unsigned short));
		res = fread(heightmap->data16, sizeof(unsigned short), heightmap->size/2, fp); 	
	}

	// Check if fread errored.
	if(!res) {
		if (feof(fp)) {
			printf("Error: Failed at reading source file: unexpected end of file!\n");
		} else if (ferror(fp)) {
           printf("Error: Failed at reading source file!\n");
       }
	   return 0;
	}
	
	fclose(fp);
	return 1;
}


/*
	Modify the color "height" of the map
*/
bool RaiseUnityMap(sHeightMap *heightmap) {	

	int seafloor_color; 
	
	// 8 bit mode
	if (heightmap->bitmode == 8) {
		seafloor_color = (((float)255/(heightmap->alt+heightmap->depth)) * heightmap->depth);

		// Modify the colors
		for (int i = 0; i < heightmap->size; i++)  {
			heightmap->data8[i] =  heightmap->data8[i] * (255-seafloor_color)/255 + seafloor_color;
		}			
		return 1;
	
	// 16 bit mode
	} else {
		seafloor_color = (((float)65535/(heightmap->alt+heightmap->depth)) * heightmap->depth);

		// Modify the colors
		for (int i = 0; i < heightmap->size/2; i++) {
			heightmap->data16[i] =  heightmap->data16[i] * (65535-seafloor_color)/65535 + seafloor_color;
		}
		return 1;
	}	
	
	// should not be here.
	return 0;
}


/*
	Write a new terrain file.
*/
bool WriteUnityMap(sHeightMap *heightmap) {
	
	size_t res;
	FILE *fp;
	if ((fp = fopen(heightmap->filename_out, "wb")) == NULL) {
        printf("Error: fopen destination-file!\n");
        return 0;
    }	

	// 8 bit mode
	if (heightmap->bitmode == 8) {
		res = fwrite(heightmap->data8, heightmap->size, 1, fp);
	
	// 16 bit mode
	} else{ 
		res = fwrite(heightmap->data16, heightmap->size, 1, fp);
		//res = fwrite(heightmap->data16, sizeof(heightmap->data16[0]), heightmap->size/2, fp);
	}
	
	// Check if fread errored.
	if (!res) {
		printf("Error: Failed at writeing to the destination file!\n");
		fclose(fp);
		return 0;
	}
	
	fclose(fp);
	return 1;
}


/*
	House cleaning...
	Free up the Allocated memory 
*/
void CleanUp(sHeightMap *heightmap) {
	free(heightmap->filename_in);
	free(heightmap->filename_out);
	
	if (heightmap->bitmode == 8) {
		free(heightmap->data8);
	}else{
		free(heightmap->data16);
	}
}


int main(int argc, char**argv) {
	
	sHeightMap heightmap;

	if (! ParseCmd(argc, argv, &heightmap)) return 1;
		
	printf("\nNew height setting for Unity:%i\n\n", heightmap.alt+heightmap.depth);
	
	printf("Reading heightmap.\n");
	if (!ReadUnityMap(&heightmap))  { return 1;}
	
	printf("Generating new map.\n");
	if (!RaiseUnityMap(&heightmap)) { return 1;}
	
	printf("Saving the new map.\n");
	if (!WriteUnityMap(&heightmap)) { return 1;}
	
	CleanUp(&heightmap);
	printf("Done, map is ready for import in unity!.\n");
	
	return 0;
}

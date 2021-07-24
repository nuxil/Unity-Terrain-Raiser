# Unity Terrain Raiser
	
This is a standalone program, it does not interact with the unity editor in any way.<br/>
The program is designed to read a unity terrain heightmap file (terrain.raw) directly.<br/>
It will read the terrain file and create a new terrain file with a desired raised terrain, based on the user input.<br/><br/>

Normally terrains are created near the lowest level possible, which in effect prevents you from lowering the terrain.<br/>
To overcome this problem, the program will modify the colors to reflect the desired height change.<br/>
	
This program supports both 8bit and 16bit heightmaps exported from unity.<br/>
When exporting a heightmap in unity, the following settings are required.<br/>

* Depth       : 16 bit or 8 bit , Your choice
* **Byte Order  : Windows (Little-endian)**
* Flip        : Your choice
	
Notice. Changeing the height of the map will cause loss in resolution, how much depends on your settings!.
		
* Using -b 8 on a 16bit heightmap will work fine.
* Using -b 16 on a 8bit heightmap will not work.
	
	
Example usage.<br/>
Assume you have a terrain that is 100 meters in height and you want to add a sea depth of 50 meter.<br/>
This means you need to use -a 100 and -d 50<br/>
	
**utr.exe -i example_16bit.raw -o new_example_16bit.raw -b 16 -a 100 -d 50**

Next when importing the modified heightmap. you need to set the new height to 150 meters.<br/>
Also, you should move the terrain down -50 on the Y axis in the Trasform, if you want the sea level to be at 0 meter.<br/>

* -i input file,  The name of the unity heightmap.
* -o output file, The name of the new generated heightmap.
* -b bit mode,    8bit or 16bit heightmap.
* -a altitude,    Height of the terrain.
* -d depth,       The depth to add to the heightmap.

Compile code with:<br/>
**gcc unity_terrain_raiser.c -o utr**<br/>

![Image description](https://github.com/nuxil/Unity-Terrain-Raiser/blob/main/unity.gif)

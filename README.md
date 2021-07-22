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
	
	
	Example usage.
	Assume you have a terrain that is 100 meters in height and you want to add a sea depth of 50 meter.
	This means you need to use -a 100 and -d 50
	
	utr.exe -i example_16bit.raw -o new_example_16bit.raw -b 16 -a 100 -d 50
	
	Next when importing the modified heightmap. you need to set the new height to 150 meters.
	Also. you should move the terrain down -50 on the Y axis in the Trasform, if you want the sea level to be at 0 meter.
	
	
	Compile code with:
	gcc unity_terrain_raiser.c -o utr

![Image description](https://github.com/nuxil/Unity-Terrain-Raiser/blob/main/unity.gif)

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


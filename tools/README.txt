This directory contains the tools for editing RoboPanic.


RPSceneEditor
------------------

RPSceneEditor.jar is the java executable for the scene editor. To run type
	java -jar RPSceneEditor.jar

RPSceneEditor\ contains the source code for the scene editor

Notes about RPSceneEditor:
	1. the tiles file must be in the same directory as the .jar file
	   and it must be named tiles.jpg(a .gif file may work as well but
	   hasn't been tested), 
	2. the tiles.jpg file should be the same as the tiles.bmp file
	   which is read by the RoboPanic engine but should be saved
	   as a jpeg
	3. tiles must be 32 x 32
	4. can currently only create scenes the size of the screen, though
	   this will probably change
	5. this program only creates the layout of the tiles and cannot
	   be used to place objects/entities in the scenes
	6. RPSceneEditor will change and older scene files created with it
	   may not be compatible with the editor or the RoboPanic engine
	   

Changes:
11.24 - tiles are now 32 x 32 and you can't edit the old data files
	now has a "New" button to clear the table
11.22 - add some space between the tiles in the tile list so you can 
	distinguish between them
11.19 - now allows block selection
	removed the "Move" attribute and added the "Draw over character" attrib
11.17 - fixed a bug in the check box selection which didn't let you
	uncheck something properly
11.08 - an annoying bug when you double clicked in the table


------------- end RPSceneEditor --------------


other tools go here
-------------------

-- brett, adams@cs.unt.edu

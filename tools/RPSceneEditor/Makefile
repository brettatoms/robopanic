#
# Makefile for RPSceneEditor
#

VM = java 
JAR = jar
BROWSER = appletviewer
JAVAC = jikes

# manifest file for .jar
MANIFEST = MainClass.mf

# name of the jar file
JARFILE = RPSceneEditor.jar

# .html containing applet code
HTMLFILE = 

# .class file containing main()
MAINCLASS = RPSceneEditor.class

COPTIONS = +E  #jikes only options

# what to compile
CLASSES = RPSceneEditor.class TileListCellRenderer.class TileTableCellRenderer.class TileTableModel.class TileListModel.class
 
DIR := $(shell pwd)

# CLASSPATH plus specialized class path
CP = $(CLASSPATH):$(JAVAPATH)/jre/lib/ext/j3dcore.jar:$(JAVAPATH)/jre/lib/ext/j3dutils.jar:$(JAVAPATH)/jre/lib/ext/vecmath.jar:$(HOME)/code/java:./

.SUFFIXES: .java .class 

all:   $(CLASSES)
	chmod 755 $(MAINCLASS)

run-applet: $(CLASSES) jar
	$(BROWSER) $(HTMLFILE)

run-app: $(CLASSES)
	$(VM) -cp . $(basename $(MAINCLASS))

# TODO: set up a function to handle wildcard so we can include the 
#	inner classes
jar:
ifeq ($(MANIFEST),) # no manifest file defined		
	$(JAR)  cvf $(JARFILE) $(foreach VAR, $(wildcard *.class), '$(VAR)')
else
	$(JAR)  cvmf $(MANIFEST) $(JARFILE) $(foreach VAR, $(wildcard *.class), '$(VAR)')	
endif

jar-src:		
	$(JAR)  cvf $(basename $(JARFILE))-src.jar $(wildcard *)
clean:
	rm -fr `find ./ -name '*.class' -print`
	rm -fr `find ./ -name '*[~]' -print`

# Implicit rule for making Java class files from Java 
# source files. 
#.java.class: 
#	$(JAVAC) $(COPTIONS) -classpath $(CP)  $? 
%.class: %.java
	$(JAVAC) $(COPTIONS) -classpath $(CP)  $? 
















NAME = application
OBJECTS = objects/lightsettings.o objects/stringtable.o
COMPILE_EXTRA = 
LINK_EXTRA = -lren-script -lren-general

include ../library-tools/Makefile.include

# Compiling objects
objects/lightsettings.o objects/lightsettings.debug.o: lightsettings.cxx lightsettings.h
	$(COMPILE) $@ lightsettings.cxx

objects/stringtable.o objects/stringtable.debug.o: stringtable.cxx stringtable.h
	$(COMPILE) $@ stringtable.cxx

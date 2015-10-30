# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = xml

# change these to set the proper directories where each files shoould be
SRCDIR   = ./src
OBJDIR   = ./obj
BINDIR   = ./bin
INCDIR   = ./include

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm       = rm -f
mkdir    = mkdir -pv
CC       = gcc
# compiling flags here
CFLAGS   = -Wall -I. -I$(INCDIR)

LINKER   = gcc -o
# linking flags here
LFLAGS   = -Wall -I. -lm 

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(mkdir) $(OBJDIR)
	@$(mkdir) $(BINDIR)	
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(BINDIR)/$(TARGET)	
	@echo "Cleanup complete!"

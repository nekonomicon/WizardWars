#
# Half-Life Standard SDK 2.3 mp_i386.so Makefile for i386 Linux
#
# October 2002 by Leon Hartwig (hartwig@valvesoftware.com)
#

DLLNAME=wizardwars

ARCH=i486

#make sure this is the correct compiler for your system
#CC=gcc
CC=gcc

DLL_SRCDIR=./dlls
ENGINE_SRCDIR=./engine
COMMON_SRCDIR=./common
PM_SHARED_SRCDIR=./pm_shared
GAME_SHARED_SRCDIR=./game_shared
WW_SHARED_SRCDIR=./ww_shared

DLL_OBJDIR=$(DLL_SRCDIR)/obj
PM_SHARED_OBJDIR=$(PM_SHARED_SRCDIR)/obj
GAME_SHARED_OBJDIR=$(GAME_SHARED_SRCDIR)/obj
WW_SHARED_OBJDIR=$(WW_SHARED_SRCDIR)/obj

BASE_CFLAGS= -Dstricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp \
             -D_snprintf=snprintf -DCLIENT_WEAPONS

#safe optimizations
CFLAGS=$(BASE_CFLAGS) -w -march=$(ARCH) -O1 #-ggdb3

#full optimization
#CFLAGS=$(BASE_CFLAGS) -w -O1 -march=$(ARCH) -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations \
	-malign-loops=2 -malign-jumps=2 -malign-functions=2

#use these when debugging 
#CFLAGS=$(BASE_CFLAGS) -ggdb3

INCLUDEDIRS=-I. -I.. -I$(DLL_SRCDIR) -I$(ENGINE_SRCDIR) -I$(COMMON_SRCDIR) -I$(PM_SHARED_SRCDIR) -I$(GAME_SHARED_SRCDIR) -I$(WW_SHARED_SRCDIR)

#LDFLAGS=
LDFLAGS=-lm -lstdc++

SHLIBEXT=so
SHLIBCFLAGS=-fPIC
SHLIBLDFLAGS=-shared

DO_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) $(INCLUDEDIRS) -o $@ -c $<

#############################################################################
# SETUP AND BUILD
# GAME
#############################################################################

$(DLL_OBJDIR)/%.o: $(DLL_SRCDIR)/%.cpp
	$(DO_CC)

$(WW_SHARED_OBJDIR)/%.o: $(WW_SHARED_SRCDIR)/%.cpp
	$(DO_CC)

$(GAME_SHARED_OBJDIR)/%.o: $(GAME_SHARED_SRCDIR)/%.cpp
	$(DO_CC)

$(PM_SHARED_OBJDIR)/%.o: $(PM_SHARED_SRCDIR)/%.c
	$(DO_CC)
	
OBJ := $(patsubst $(DLL_SRCDIR)/%.cpp,$(DLL_OBJDIR)/%.o,$(wildcard $(DLL_SRCDIR)/*.cpp))
OBJ += $(patsubst $(WW_SHARED_SRCDIR)/%.cpp,$(WW_SHARED_OBJDIR)/%.o,$(wildcard  $(WW_SHARED_SRCDIR)/*.cpp))
OBJ += $(GAME_SHARED_OBJDIR)/voice_gamemgr.o
OBJ += $(patsubst $(PM_SHARED_SRCDIR)/%.c,$(PM_SHARED_OBJDIR)/%.o,$(wildcard $(PM_SHARED_SRCDIR)/*.c))

$(DLLNAME)_$(ARCH).$(SHLIBEXT) : neat $(OBJ)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) $(LDFLAGS) -o $@ $(OBJ)

neat:
	-mkdir $(DLL_OBJDIR)
	-mkdir $(GAME_SHARED_OBJDIR)
	-mkdir $(PM_SHARED_OBJDIR)
	-mkdir $(WW_SHARED_OBJDIR)
clean:
	-rm -f $(OBJ)
	-rm -f $(DLLNAME)_$(ARCH).$(SHLIBEXT)
spotless: clean
	-rm -r $(DLL_OBJDIR)
	-rm -r $(GAME_SHARED_OBJDIR)
	-rm -r $(PM_SHARED_OBJDIR)
	-rm -r $(WW_SHARED_OBJDIR)

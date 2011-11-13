#
# Basic KallistiOS skeleton / test program
# Copyright (C)2001-2004 Dan Potter
#   

#KOS_BASE = /usr/local/dc/kos/

CFLAGS= -O2 -fexceptions -Wno-unused -Wno-format -fomit-frame-pointer -ffast-math -frename-registers -ftracer -funit-at-a-time -funswitch-loops -DDC_ROMDISK #-DCHUISDL -DDC_ROMDISK -DDC_CDROM

# Put the filename of the output binary here
TARGET = jurtris.elf

# List all of your C files here, but change the extension to ".o"
# Include "romdisk.o" if you want a rom disk.
OBJS = main.o romdisk.o

# If you define this, the Makefile.rules will create a romdisk.o for you
# from the named dir.
KOS_ROMDISK_DIR = ./ #romdisk

# The rm-elf step is to remove the target before building, to force the
# re-creation of the rom disk.
all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(OBJS) romdisk.*

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS)
	kos-c++ -o $(TARGET) $(OBJS) -lSDL_mixer_126 -lSDL -lm -lkallisti -lz #-lSDL_129

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)

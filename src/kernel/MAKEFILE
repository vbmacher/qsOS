# makefile for QSS v1.00
#
# (c) Copyright 2005, vbmacher
#
#

LIBPATH=entry;rtl;cpu;mau;serial;keyb;console;disk;process
LIB_FILES=start.lib rtl.lib cpu.lib mau.lib serial.lib \
          keyb.lib console.lib disk.lib process.lib

LINK_OPTIONS=/t /s /L$(LIBPATH)

OBJ_FILES=entry\start.obj
EXE_FILE=qss.sys
MAP_FILE=qss.map

LINK=$(MAKEDIR)\tlink
MAKE=$(MAKEDIR)\make $(MAKE_OPTIONS)

ENTER=@cd
LEAVE=$(ENTER) ..

all: start rtl mau cpu serial keyb console disk process
   $(LINK) @&&!
         $(LINK_OPTIONS) $(OBJ_FILES), $(EXE_FILE), $(MAP_FILE), $(LIB_FILES)
!

link:
   $(LINK) @&&!
         $(LINK_OPTIONS) $(OBJ_FILES), $(EXE_FILE), $(MAP_FILE), $(LIB_FILES)
!

start:
   $(ENTER) entry
   $(MAKE)
   $(LEAVE)

rtl:
   $(ENTER) rtl
   $(MAKE)
   $(LEAVE)

mau:
   $(ENTER) mau
   $(MAKE)
   $(LEAVE)

cpu:
   $(ENTER) cpu
   $(MAKE)
   $(LEAVE)

serial:
   $(ENTER) serial
   $(MAKE)
   $(LEAVE)

keyb:
   $(ENTER) keyb
   $(MAKE)
   $(LEAVE)

console:
   $(ENTER) console
   $(MAKE)
   $(LEAVE)

disk:
   $(ENTER) disk
   $(MAKE)
   $(LEAVE)

process:
   $(ENTER) process
   $(MAKE)
   $(LEAVE)

clean:
   $(ENTER) entry
   $(MAKEDIR)\make clean
   $(ENTER) ..\rtl
   $(MAKEDIR)\make clean
   $(ENTER) ..\mau
   $(MAKEDIR)\make clean
   $(ENTER) ..\cpu
   $(MAKEDIR)\make clean
   $(ENTER) ..\serial
   $(MAKEDIR)\make clean
   $(ENTER) ..\keyb
   $(MAKEDIR)\make clean
   $(ENTER) ..\console
   $(MAKEDIR)\make clean
   $(ENTER) ..\disk
   $(MAKEDIR)\make clean
   $(ENTER) ..\process
   $(MAKEDIR)\make clean
   $(LEAVE)

   @del qss.sys
   @del qss.map
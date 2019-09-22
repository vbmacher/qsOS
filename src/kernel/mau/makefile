#
# makefile for MAU (Memory Allocation Unit)
#
# (c) Copyright 2005, vbmacher
#
#

COMPILE_OBJ=mau.obj a20.obj
LIB_NAME=mau.lib
LIST_FILE=mau.lst
LIB_OBJ=-+mau.obj -+a20.obj

INCPATH=..\include;.

CPP_OPTIONS=-O2 -mc -3 -c -d -j6 -I$(INCPATH)
ASM_OPTIONS=/mx /m3
LIB_OPTIONS=

CPP=$(MAKEDIR)\bcc $(CPP_OPTIONS)
ASM=$(MAKEDIR)\tasm $(ASM_OPTIONS)
LIB=$(MAKEDIR)\tlib

$(LIB_NAME): $(COMPILE_OBJ)
   $(LIB) @&&!
         $(LIB_NAME) $(LIB_OPTIONS) $(LIB_OBJ), $(LIST_FILE)
!

clean:
   @del *.obj
   @del *.bak
   @del *.lib
   @del *.lst

.cpp.obj:
   $(CPP) $<

.c.obj:
   $(CPP) $<

.asm.obj:
   $(ASM) $<
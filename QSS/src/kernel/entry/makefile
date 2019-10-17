#
# makefile for lib 'start.lib'
#
# (c) Copyright 2005, vbmacher
#
# Entry point for kernel
#
#

COMPILE_OBJ=start.obj
LIB_NAME=start.lib
LIST_FILE=startl.lst
LIB_OBJ=-+start.obj

INCPATH=..\include;.

CPP_OPTIONS=-O2 -ml -3 -c -d -j6 -I$(INCPATH)
#ASM_OPTIONS=/m /mx /m5 /q /t
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


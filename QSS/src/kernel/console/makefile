#
# makefile for CONSOLE
#
# (c) Copyright 2005, vbmacher
#
#

COMPILE_OBJ=chatt.obj system.obj misc.obj halt.obj \
            status.obj line.obj reboot.obj nick.obj \
            help.obj dir.obj mkdir.obj read.obj delfile.obj \
            deldir.obj cd.obj format.obj run.obj dumpkern.obj
LIB_NAME=console.lib
LIST_FILE=console.lst
LIB_OBJ=-+chatt.obj -+system.obj -+misc.obj -+halt.obj \
        -+status.obj -+line.obj -+reboot.obj -+nick.obj \
        -+help.obj -+dir.obj -+mkdir.obj -+read.obj -+delfile.obj \
        -+deldir.obj -+cd.obj -+format.obj -+run.obj -+dumpkern.obj

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
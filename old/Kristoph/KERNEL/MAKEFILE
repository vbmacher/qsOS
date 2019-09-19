# Kristoph Operating System
#
# (c) Copyright 2000-2002, P. Jakubco ml.
#
# MAKEFILE

# Compilers and linker
ASM = @tasm
CPP = @bcc
LNK = @tlink
LIB = @tlib

# Parameters
ASM_OPTIONS = /mx /m3
CPP_OPTIONS = -O2 -mc -3 -c -d -j6 -I..\INCLUDE
C_OPTIONS   = -O2 -mc -3 -c -d -j6 -I..\INCLUDE
LIB_OPTIONS = 
LNK_OPTIONS = /s /3 /t

TARGET = kristoph.sy0
SRC = kernel.lib video.lib console.lib disk.lib

KERNEL_OBJ  = -+entry.obj -+k_main.obj
CONSOLE_OBJ = -+console.obj -+string.obj -+mem.obj
DISK_OBJ    = -+disk.obj
VIDEO_OBJ   = -+video.obj -+nvideo.obj

# Syntaxes:
all: $(TARGET)

kernel.lib:
	@cd MAIN
	@$(ASM) $(ASM_OPTIONS) *.asm
	@$(CPP) $(CPP_OPTIONS) *.cpp
	@$(LIB) ..\$< $(LIB_OPTIONS) $(KERNEL_OBJ)
	@cd ..

console.lib:
	@cd CONSOLE
	@$(ASM) $(ASM_OPTIONS) *.asm
	@$(CPP) $(CPP_OPTIONS) *.cpp
	@$(LIB) ..\$< $(LIB_OPTIONS) $(CONSOLE_OBJ)
	@cd ..

disk.lib:
        @cd FS
	@$(CPP) $(CPP_OPTIONS) *.cpp
	@$(LIB) ..\$< $(LIB_OPTIONS) $(DISK_OBJ)
	@cd ..

video.lib:
	@cd VIDEO
	@$(ASM) $(ASM_OPTIONS) *.asm
	@$(CPP) $(CPP_OPTIONS) *.cpp
	@$(LIB) ..\$< $(LIB_OPTIONS) $(VIDEO_OBJ)
	@cd ..

$(TARGET): $(SRC)
#@$(LNK) $(LNK_OPTIONS) $(SRC), $(TARGET)
	@$(LNK) @LINK.TXT
	@rem split.exe

	@del *.map
	@del *.lib
	@del *.bak
	@rem del kristoph.sy0

@echo off

rem Vysvetlivky k options:

rem CPP Options - /O2 (Optimize, fastest code)
rem		- /mt (memory model - tiny)
rem	   x    - /3 (80386 protected-mode compatible instructions)
rem		- /c (Compile to .OBJ but do not link)
rem		- /d (Merge duplicate strings on)
rem		- /j6 (Errors: stop after 6 messages)
rem
rem ASM Options - /mx (Case sensitivity on globals)
rem		- /m3 (Allow 3 multiple passes to resolve forward references)
rem
rem LINK Options- /t (Create COM file)
rem	x       - /3 (Enable 32-bit processing)
rem		- /s (Detailed map of segments)

rem -------- BOOT LOADER ---------

echo Assembling/Compiling BootLoader...

rem Boot entry
tasm /mx /m3 BOOTCODE\boot.asm | more

rem Text functions
tasm /mx /m3 TEXT\text.asm | more

rem Main file
bcc /O2 /mc /c /d /3 /j6 /IINCLUDE MAIN\b_main.cpp | more

echo Linking BootLoader...
tlink /t /s boot.obj b_main.obj text.obj, kfs.img

echo Deleting unused files...
   del *.obj
   del *.map

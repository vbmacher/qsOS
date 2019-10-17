tasm /m5 boot.asm
tlink /t boot.obj
del boot.h > nul
del *.obj > nul
del *.map > nul
bin2h boot.com
del boot.com
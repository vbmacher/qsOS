#include <stdio.h>

main()
{
  int xes, xdi,i;
  asm {
    mov ax, 0x200
    mov es, ax
    xor di,di
  }
  for (i = 0; i < 20640; i++) {
    xes = _ES;
    xdi = _DI;
    printf("\nES: %d, DI: %d", xes, xdi);
    xdi++;
    _DI = xdi;
  }
  getchar();
}
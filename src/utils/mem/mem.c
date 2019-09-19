#include <stdio.h>

main()
{
 long kb;
asm {
  mov ah, 0x88
  int 0x15
  jc error
  mov dword ptr [kb], eax
  jmp ok
}
error:
  printf("\nError: Function unsupported");
  goto exit;
ok:
  printf("\nOK");
  printf("\nKB: %d", kb);
exit:
  getchar();
}
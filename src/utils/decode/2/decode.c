#include <stdio.h>

void decode_lp(int LS)
{
  unsigned char head, track, sector;

  _AX = LS;
asm {
  push bx
  push ax
  xor dh, dh      // dh = hlava zatial 0
  mov bl, 2 * 18
  div bl
  mov ch, al      // ch = stopa
  cmp ah, 18
  jl h0
  mov dh, 1       // dh = hlava 1
}
h0:
asm {
  pop ax          // v ax znova link
  mov bl, 18
  div bl
  mov cl, ah
  inc cl          // cl = sektor
  xor dl, dl
  pop bx
}
  track = _CH;
  sector = _CL;
  head = _DH;
  printf("\nT: %d, S: %d, H: %d", track, sector, head);
}

main()
{
  int i;

  for (i = 0; i < 41; i++) {
    decode_lp(i);
  }
  getchar();
}
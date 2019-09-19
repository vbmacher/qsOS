#include <stdio.h>

int head = 0;
int track = 0;
int sector = 0;

void decode_lp(int link)
{
  asm {
    mov ax, word ptr [link]
    push bx
    push ax
    xor dh, dh      // dh = hlava zatial 0
    mov byte ptr [head], 0
    mov bl, 2 * 18
    div bl
    mov ch, al      // ch = stopa
    mov byte ptr [track], ch
    cmp ah, 18
    jl qh0
    mov dh, 1       // dh = hlava 1
    mov byte ptr [head], 1
  }
qh0:
  asm {
    pop ax          // v ax znova link
    mov bl, 18
    div bl
    mov cl, ah
    inc cl          // cl = sektor
    mov byte ptr [sector], cl
    xor dl, dl
    pop bx
  }
}

void main()
{
  int i;
  printf("\nDecoding...");

  for (i = 0; i < (2880); i++) {
    decode_lp(i);
    printf("\n%d. head: %d, track: %d, sector: %d", i, head, track, sector);
  }
  getchar();
}



/*
 * INT.C
 *
 * (c) Copyrioght 2005, vbmacher
 *
 */

void interrupt (*GetVect(int Vect))(void)
{
  asm {
    xor     ax,ax
    mov     es,ax
    mov     bx, word ptr [Vect]
    shl     bx,2
    mov     ax,es:[bx]
    mov     dx,es:[bx + 2]
  }
}


void SetVect(int Vect, void interrupt (*Handler)(void))
{
  asm {
    xor     ax,ax
    mov     es,ax
    mov     bx, word ptr [Vect]
    shl     bx,2
    mov     ax, word ptr [Handler]
    cli
    mov     es:[bx],ax
    sti
  }
}

main()
{

}

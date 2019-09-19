#include <stdio.h>

void main(void)
{
  unsigned long i;
  printf("\nDecimal to Hexadecimal\n(c) Copyright 2002,P. Jakubco, ver:1.00");
  printf("\nVlozte cele dekadicke cislo v rozsahu 0 az 4,294,967,295:");
  scanf("%lu",&i);
  printf("\nJeho hexadecimalna hodnota je: %10Xh",i);
  fflush(stdin);
  getchar();
}
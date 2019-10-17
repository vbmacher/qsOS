/*
 * CONTROL.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <cpu.h>
#include <rtl.h>
#include <colors.h>

int cpu_id;
int brand;
char vendor_str[13];
char brand_str[48];

// tato funkcia sa moze volat az ked je ista podpora cpuid
void cpuTEST()
{
  cpuPVENDOR();
  textattr(atrBORDER);
  cprintf("\n\tVendor string: ");
  textattr(atrIMPTEXT);
  cprintf("%s", vendor_str);

  brand = checkBRAND();
  if (brand) {
    cpuPBRAND();
    textattr(atrBORDER);
    cprintf("\n\tBrand string: ");
    textattr(atrIMPTEXT);
    cprintf("%s", brand_str);
  }
  return 0;
}

/*
 * STRING.CPP
 *
 * Funkcie na konzolu
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 19.12.2001 - Initial file
 * 21.12.2001 - Added some formats to printf function
 * 29.1.2001  - class CString added and all her functions
 */
#include <console.h>

//     ...(const void far *s1, const void far *s2, ...
int memcmp(const void *s1, const void *s2, size_t count)
{
 asm {
	push    si
	push    di
	push    ds
	push    es

	xor     ax,ax
	les     di, [s1]
	lds     si, [s2]
	mov     cx, [count]
	cld
	rep     cmpsb
	je x
	mov al, 1
  }
  x:
  asm {
	pop     es
	pop     ds
	pop     di
	pop     si
   }
 return _AL;
}

char *itoa(long Value, char *Str, int Base)
{
	char InvString[32];
	char *pStr;
	char *pOrig = Str;

	if (Base == 10 && Value < 0) {
		*Str++ = '-';
		Value = -Value;
	}

	pStr = InvString;
	do {
		*pStr++ = GetDigit(Value % Base);
		Value /= Base;
	} while (Value);

	while (pStr != InvString) {
		*Str++ = *--pStr;
	}
	*Str = '\0';
	return pOrig;
}

void printf(const char *format, ...)
{
 unsigned short *argl;
 char buf[32];

 for (argl = &((unsigned short *)&format)[2]; *format; ++format)
  switch (*format) {
    case '\n':
     puts("\r\n");
     break;
    case '%':
     switch (*++format) {
       case 'c':
        putch(*argl++);
        break;
       case 'd':
        itoa(*argl++,buf,DEC);
        puts(buf);
        break;
       case 'u':
        itoa(*(unsigned int *)argl++,buf,DEC);
        puts(buf);
        break;
       case 'x':
        itoa(*argl++,buf,HEX);
        puts(buf);
        break;
       case 's':
        puts((const char *)*(long *)argl);
        argl += 2;
        break;
       case 'l':
        if (format[1] == 'd') {
           ++format;
           itoa(*(signed long *)argl++,buf,DEC);
	   puts(buf);
           break;
        }
        if (format[1] == 'x') {
           ++format;
           itoa(*(unsigned long *)argl++,buf,HEX);
           puts(buf);
           break;
        }
        if (format[1] == 'u') {
           ++format;
           itoa(*(unsigned long*)argl++,buf,DEC);
           puts(buf);
           break;
        }
       default:
        putch('%');
        --format;
        break;
     }
     break;
    default:
     putch(*format);
     break;
  }
}
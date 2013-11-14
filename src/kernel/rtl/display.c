/*
 * DISPLAY.CPP
 *
 * (c) Copyright 2001-2005, vbmacher
 *
 * Part of RTL lib
 *
 */

#include <rtl.h>
#include <console.h>
#include <serial.h>

void printf(const char *format, ...)
{
 unsigned short *argl;
 char buf[32];

 for (argl = &((unsigned short *)&format)[2]; *format; ++format)
  switch (*format) {
    case '\t':
     puts("    ");
     break;
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

// console printf
void cprintf(const char *format, ...)
{
 unsigned short *argl;
 char buf[32];

 for (argl = &((unsigned short *)&format)[2]; *format; ++format)
  switch (*format) {
    case '\t':
     cputs("    ");
     break;
    case '\n':
     cputs("\r\n");
     break;
    case '%':
     switch (*++format) {
       case 'c':
        cputch(*argl++);
        break;
       case 'd':
        itoa(*argl++,buf,DEC);
        cputs(buf);
        break;
       case 'u':
        itoa(*(unsigned int *)argl++,buf,DEC);
        cputs(buf);
        break;
       case 'x':
        itoa(*argl++,buf,HEX);
        cputs(buf);
        break;
       case 's':
        cputs((const char *)*(long *)argl);
        argl += 2;
        break;
       case 'l':
        if (format[1] == 'd') {
           ++format;
           itoa(*(signed long *)argl++,buf,DEC);
	   cputs(buf);
           break;
        }
        if (format[1] == 'x') {
           ++format;
           itoa(*(unsigned long *)argl++,buf,HEX);
           cputs(buf);
           break;
        }
        if (format[1] == 'u') {
           ++format;
           itoa(*(unsigned long*)argl++,buf,DEC);
           cputs(buf);
           break;
        }
       default:
        cputch('%');
        --format;
        break;
     }
     break;
    default:
     cputch(*format);
     break;
  }
}

void gets(char *s)
{
  int c = 0;
  char *P;

  P = s;
  while ((c = getchar()), (char)c != '\r') {
    switch ((char)c) {
    case 0x08: // Backspace
      if (P > s) {
        *P = 0;
	P--;
	*P = 0;
	printf("%c %c",c,c);
      }
      break;
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x09:
    case 0x0A: // '\n', Line Feed
    case 0x0B:
    case 0x0C:
    case 0x0D: // '\r', Carriage return
    case 0x0E:
    case 0x0F:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B: // ESC
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
      break;
    default:
      *P = c;
      P++;
      printf("%c",c);
      break;
    }
  }
  *P = 0;
}

// console gets
void cgets(char *s)
{
  int c = 0;
  char *P;
  char actcon;

  P = s;
  actcon = conACTIVE;
  while ((c = getchar()), (char)c != '\r') {
    if (actcon != conACTIVE) return;  // ak nastala zmena konzoly
    switch (c) {
    case 0x08: // Backspace
      if (P > s) {
        *P = 0;
	P--;
	*P = 0;
	cprintf("%c %c",c,c);
      }
      break;
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x09:
    case 0x0A: // '\n', Line Feed
    case 0x0B:
    case 0x0C:
    case 0x0D: // '\r', Carriage return
    case 0x0E:
    case 0x0F:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B: // ESC
    case 0x1C:
    case 0x1E:
    case 0x1F:
      break;
    default:
      *P = c;
      P++;
      cprintf("%c",c);
      break;
    }
  }
  *P = 0; // ASCII-Z string
}


int toupper(int ch)
{
 if (ch == -1)
   return -1;

 if (((unsigned char)ch >= 'a') && ((unsigned char)ch <= 'z')) {
   return(_toupper((unsigned char)ch));
 }
 else {
   return((unsigned char)ch);
 }
}

int tolower(int ch)
{
 if (ch == -1)
   return -1;

 if (((unsigned char)ch >= 'A') && ((unsigned char)ch <= 'Z')) {
   return(_tolower((unsigned char)ch));
 }
 else {
   return((unsigned char)ch);
 }
}

/* Return true if c is a delimiter. */
// delimiter je cudny znak
int isdelim(char c)
{
  if (strchr(" `~!@#$%^&*()_-=+\|[]{};:',.<>?/", c) || c == 9 || c == '\r' || c == '\n' || c == 0) 
    return 1;  
  return 0;
}

/* Return 1 if c is space or tab. */
int iswhite(char c)
{
  if (c == ' ' || c == '\t') return 1;
  else return 0;
}


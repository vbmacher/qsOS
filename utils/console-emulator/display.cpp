/*
 * DISPLAY.CPP
 *
 * Funkcie na zobrazenie textu
 *
 * (c) Copyright 2001-2002, P. Jakubco ml.
 *
 */

#include <display.h>

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

void con_gets(char *s)
{
  int c = 0;
  char *P;

  P = s;

  while ((c = getch()), (char)c != '\r') {
    switch ((char)c) {
    case 0x08: // Backspace
      if (P > s) {
	P--;
	*P = 0;
	printf("%c %c",c,c);
      }
      break;
    case 0x01:
    case 0x02:
    case 0x03: // NUL
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x09:
    case 0x0a: // '\n', Line Feed
    case 0x0b:
    case 0x0c:
      break;
    case 0x0d: // '\r', Carriage return
      c = '\n';
      break;
    case 0x0e:
    case 0x0f: // Shift+Tab
    case 0x10: // Alt+Q
    case 0x11: // Alt+W
    case 0x12: // Alt+E
    case 0x13: // Alt+R
    case 0x14: // Alt+T
    case 0x15: // Alt+Y
    case 0x16: // Alt+U
    case 0x17: // Alt+I
    case 0x18: // Alt+O
    case 0x19: // Alt+P
    case 0x1a:
    case 0x1b: // ESC
    case 0x1c:
    case 0x1d:
    case 0x1e: // Alt+A
    case 0x1f: // Alt+S
      break;
    default:
      *P = c;
      P++;
      printf("%c",c);
      break;
    }
  }
}

void gets(char *s)
{
  int c = 0;
  char *P;

  P = s;
  while ((c = getch()), (char)c != '\r') {
    switch ((char)c) {
    case 0x08: // Backspace
      if (P > s) {
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
    case 0x10: // '\n', Line Feed
    case 0x11:
    case 0x12:
      break;
    case 0x13: // '\r', Carriage return
      c = '\n';
      break;
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27: // ESC
    case 0x28:
    case 0x29:
    case 0x30:
    case 0x31:
      break;
    default:
      *P = c;
      P++;
      printf("%c",c);
      break;
    }
  }
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
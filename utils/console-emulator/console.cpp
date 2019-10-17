/*
 * CONSOLE.CPP
 *
 * (c) Copyright 2002, P. Jakubco ml.
 *
 *
 */

/*
  Relase notes
  ^^^^^^^^^^^^
  5.12.2002 - v0.01a - Initial relase
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define DELIMITER 1  // medzery, '\n', '\r', dalej "!@#$%^&*()<>:;"
#define VARIABLE  2  // premenna
#define NUMBER    3  // cislo
#define COMMAND   4  // prikaz
#define STRING    5  // retazec (bude dalej spracovany bud ako prikaz, alebo VAR)
#define QUOTE     6  // retazec s ""
#define UNKNOWN   7

// Prikazy
#define PRINT     1
#define CLS       2
#define COPY      3
#define DATE      4
#define TIME      5
#define DEL       6
#define DIR       7
#define MEM       8
#define MKDIR     9
#define FINISHED 10
#define EOL      17
#define CD       11
#define VER      12
#define HALT     13
#define RESET    14
#define TREE     15
#define PATH     16

struct commands { /* keyword lookup table */
  char command[20];
  char tok;
} table[] = { /* Commands must be entered lowercase */
  "print", PRINT, /* in this table. */
  "cls", CLS,
  "copy", COPY,
  "date", DATE,
  "time", TIME,
  "del", DEL,
  "dir", DIR,
  "mem", MEM,
  "mkdir", MKDIR,
  "cd", CD,
  "ver", VER,
  "halt", HALT,
  "reset", RESET,
  "tree", TREE,
  "path", PATH,
  "", HALT  /* mark end of table */
};

char *nCommand;
char Arg_Type, Arg;
static char Argument[80];

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



// Look up a a token's internal representation in the token table.
look_up(char *s)
{
  register int i,j;
  char *p;

  /* convert to lowercase */
  p = s;
  while(*p){ *p = tolower(*p); p++; }

  /* see if token is in table */
  for(i=0; *table[i].command; i++)
      if(!strcmp(table[i].command, s)) return table[i].tok;
  return 0; /* unknown command */
}

/* Return true if c is a delimiter. */
isdelim(char c)
{
  if (strchr(" ;,+-<>/*%^=()", c) || c==9 || c=='\r' || c==0) 
    return 1;  
  return 0;
}

/* Return 1 if c is space or tab. */
iswhite(char c)
{
  if (c==' ' || c=='\t') return 1;
  else return 0;
}

char GetArg()
{
  register char *temp;

  Arg_Type = 0; Arg = 0;
  temp = Argument;

  if (*nCommand == '\0') { /* koniec prikazov */
    *Argument=0;
    Arg = FINISHED;
    return (Arg_Type = DELIMITER);
  }

  while(iswhite(*nCommand)) ++nCommand;  /* vynechame biele znaky */

  if (*nCommand == '\r') { /* crlf */
    ++nCommand; ++nCommand;
    Arg = EOL; *Argument = '\r';
    Argument[1] = '\n'; Argument[2] = 0;
    return (Arg_Type = DELIMITER);
  }

  if (strchr("+-*^/%=;(),><", *nCommand)){ /* delimiter */
    *temp = *nCommand;
    nCommand++; /* advance to next position */
    temp++;
    *temp=0; 
    return (Arg_Type = DELIMITER);
  }
    
  if (*nCommand == '"') { /* quoted string */
    nCommand++;
    while (*nCommand != '"' && *nCommand != '\r')
      *temp++ = *nCommand++;
    if (*nCommand == '\r') {
      printf("\nEnd quote missing");
      return (Arg_Type = UNKNOWN);
    }
    nCommand++; *temp=0;
    return (Arg_Type = QUOTE);
  }
  
  if (isdigit(*nCommand)) { /* number */
    while (!isdelim(*nCommand))
      *temp++ = *nCommand++;
    *temp = '\0';
    return (Arg_Type = NUMBER);
  }

  if (isalpha(*nCommand)) { /* var or command */
    while (!isdelim(*nCommand))
      *temp++ = *nCommand++;
    Arg_Type = STRING;
  }

  *temp = '\0';

  /* see if a string is a command or a variable */
  if (Arg_Type == STRING) {
    Arg = look_up(Argument); /* convert to internal rep */
    if (!Arg)
      Arg_Type = VARIABLE;
    else
      Arg_Type = COMMAND; 
  }
  return Arg_Type;
}

// Funkcia spracuje aktualny prikaz
int DoCommand()
{
do {
 GetArg();
 switch (Arg_Type) {
 case COMMAND:
   goto OperateCommand;
   break;
 case QUOTE:
 case STRING:
 case UNKNOWN:
   printf("\nInvalid command");
   return -1;
 }

OperateCommand:
   switch (Arg) {
     case PRINT:
       GetArg();
       if (Arg_Type != QUOTE)
         printf("\nQuoted string missing");
       else
	 printf("\n%s",Argument);
       break;
     case VER:
       printf("\nQSOS Interpreter v 1.0b\n(c) Copyright 2002, P. Jakubco ml.");
       break;
     case HALT:
       printf("\nSystem halted.");
       return 2;
       break;
     case RESET:
       return 2;
       break;
     default:
       printf("\nInvalid command");
       break;
   }
} while (Arg != FINISHED);
 return 0;
}

void ConsoleLoop()
{
 int n;

cLoop:
 printf("\nCON1: ");

 for (n = 0; n < 64; n++) {
   nCommand[n] = 0;
 }
 // Specialne consolove gets
 con_gets(nCommand);
// for (n = 0; n < 64; n++) {
//     nCommand[n] = toupper(nCommand[n]);
// }

 if (nCommand[0] == 0)
   goto cLoop;

 if ((n = DoCommand()) == -1)
   printf("\nInvalid command.");
 else if (n == 2)
   return;

goto cLoop;
}

void main()
{
 // Alokujeme si pamat na 64 znakov
 nCommand = (char *)malloc(64);

 // Teraz vypiseme hlasku o konzole
 printf("\nQSOS console v0.01a\n\n");
 printf("\nIf you want a help, type 'help'.");

 // A ideme do smycky
 ConsoleLoop();
 free(nCommand);
}

# Kristoph Operating System

Very first learning attempt to do at least something related to OS (kernel) development.

## Part I - CONSOLE

Konzola ako tak� neexistuje, je to len zbierka funkci� na pr�cu
s nou. Konzolou som nazval monitor v textovom re�ime. Ale okrem
funkci�  na  pr�cu s n�m  (sk�r na  pr�cu s textom) s� v nej aj
in� funkcie.

Konkr�tne s� to tieto:
	void gotoxy(int x, int y);
	void wherexy(int *x, int *y);
	void puts(const char *str);
	void putch(int ch);
	void memcpy (void far *dest, void far *str, size_t size);
	void memset(void far *dest, size_c value, size_t count);
	int memcmp(const void far *s1, const void far *s2, size_t count);
	char GetDigit(int Digit);
	void printf(const char *format,...);
	int strlen(const char far *str);
	char *strcpy(char far *dest, const char far *src);
	char *strcat(char far *dest, const char far *src);
	char *strchr(const char far *str, int c);
	int strcmp(const char far *s1, const char far *s2);
	int stricmp(const char far *s1, const char far *s2);
	long atol(const char far *str);

Z niektor�ch funkci� boli z�merne odstr�nen� k�u�ov� slov� extern
a _extern, preto�e chcem op�sa� iba funkcie z h�adiska ich ��elu a
nie aj z h�adiska ich implement�cie. Okrem nich je tu aj class CString,
ktor� obsahuje tieto funkcie, oper�tory a premenn�:

	class CString {
	   public:
	      CString();
	      CString(CString &Str);
	      CString(const char *Str);
			CString(long Value, int Radix = RADIX_DEC);
			~CString();
			void operator = (CString Str);
	      void operator = (long Value);
	      void operator = (const char *Str);
	
			void operator += (CString Str);
	      void operator += (long Value);
	      void operator += (const char *Str);
	
			int operator < (CString Str);
	      int operator < (long Value);
	      int operator < (const char *Str);
	
			int operator > (CString Str);
	      int operator > (long Value);
	      int operator > (const char *Str);
	
			int operator <= (CString Str);
	      int operator <= (long Value);
	      int operator <= (const char *Str);
	
	      int operator >= (CString Str);
	      int operator >= (long Value);
	      int operator >= (const char *Str);
	
	      int operator == (CString Str);
	      int operator == (long Value);
	      int operator == (const char *Str);
	
	      int operator != (CString Str);
	      int operator != (long Value);
	      int operator != (const char *Str);
	
	      operator const char *();
			operator long();
			long Length();
			void SetCaseSense(int CaseSense);
	
			void CharFill(int Ch, int Count);
	
			void Split(int Index, CString &Str1, CString &Str2);
			void DeleteChar(int Index);
	      void InsertChar(int Index, int Char);
	
			void Clear();
		private:
			int StringCompare(const char *Str);
			int CaseSense;
	      char *String;
	      int StrLen;
	};
	
K tejto triede patria aj kon�tanty:
	#define RADIX_BIN 2
	#define RADIX_OCT 8
	#define RADIX_DEC 10
	#define RADIX_HEX 16

### Chapter I - opis jednotliv�ch funkci�

	void gotoxy(int x, int y);
	~~~~~~~~~~~~~~~~~~~~~~~~~~
	T�to funkcia nastav� kurzor v textovom re�ime na poz�ciu
	dan� premenn�mi x(st�pec) a y(riadok).


	void wherexy(int *x, int *y);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia zist� aktu�lnu poz�ciu kurzoru v textovom re�ime
	do premenn�ch x(st�pec) a y(riadok).


	void puts(const char *str);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia zobraz� text dan� pointerom str na obrazovku v text-
	ovom re�ime a kurzor posunie na koniec re�azca.


	void putch(int ch);
	~~~~~~~~~~~~~~~~~~~
	Funkcia zobraz� znak ch na obrazovku v textovom re�ime, kur-
	zor sa posunie.


	void memcpy (void far *dest, void far *str, size_t size);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia kop�ruje oblas� pam�te ve�kosti size z pointera
	str, ktor� ukazuje na zdrojov� oblas� na miesto v pam�ti,
	na ktor� ukazuje pointer dest.


	void memset(void far *dest, size_c value, size_t count);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Funkcia "nastav�" oblas� pam�te, na ktor� ukazuje pointer
	dest, ve�kosti count * sizeof(size_c) (v podstate unsigned
	char) na hodnotu premennej value.


 	int memcmp(const void far *s1, const void far *s2, size_t count);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia porovn� dve oblasti pam�te - s1 a s2, ve�kos� je dan�
	premennou count. Ak je s1 == s2, funkcia vracia 0, ak
	s1 < s2, funkcia vracia hodnotu > 0, a ak
	s1 > s2, funkcia vracia hodnotu < 0. (!!! OVERIT !!!)


	char GetDigit(int Digit);
	void printf(const char *format,...);
	int strlen(const char far *str);
	char *strcpy(char far *dest, const char far *src);
	char *strcat(char far *dest, const char far *src);
	char *strchr(const char far *str, int c);
	int strcmp(const char far *s1, const char far *s2);
	int stricmp(const char far *s1, const char far *s2);
	long atol(const char far *str);



### Chapter II - opis class CString

Je to trieda, ktor� sa tie� pou��va na pr�cu s textom, ale
sk�r "vo vn�tri", tj. text sa nevypisuje na obrazovku.

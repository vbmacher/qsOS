# Kristoph Operating System

Very first learning attempt to do at least something related to OS (kernel) development.

## Part I - CONSOLE

Konzola ako taká neexistuje, je to len zbierka funkcií na prácu
s nou. Konzolou som nazval monitor v textovom reime. Ale okrem
funkcií  na  prácu s ním  (skôr na  prácu s textom) sú v nej aj
iné funkcie.

Konkrétne sú to tieto:
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

Z niektorıch funkcií boli zámerne odstránené k¾uèové slová extern
a _extern, pretoe chcem opísa iba funkcie z h¾adiska ich úèelu a
nie aj z h¾adiska ich implementácie. Okrem nich je tu aj class CString,
ktorá obsahuje tieto funkcie, operátory a premenné:

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
	
K tejto triede patria aj konštanty:
	#define RADIX_BIN 2
	#define RADIX_OCT 8
	#define RADIX_DEC 10
	#define RADIX_HEX 16

### Chapter I - opis jednotlivıch funkcií

	void gotoxy(int x, int y);
	~~~~~~~~~~~~~~~~~~~~~~~~~~
	Táto funkcia nastaví kurzor v textovom reime na pozíciu
	danú premennımi x(ståpec) a y(riadok).


	void wherexy(int *x, int *y);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia zistí aktuálnu pozíciu kurzoru v textovom reime
	do premennıch x(ståpec) a y(riadok).


	void puts(const char *str);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia zobrazí text danı pointerom str na obrazovku v text-
	ovom reime a kurzor posunie na koniec reazca.


	void putch(int ch);
	~~~~~~~~~~~~~~~~~~~
	Funkcia zobrazí znak ch na obrazovku v textovom reime, kur-
	zor sa posunie.


	void memcpy (void far *dest, void far *str, size_t size);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia kopíruje oblas pamäte ve¾kosti size z pointera
	str, ktorı ukazuje na zdrojovú oblas na miesto v pamäti,
	na ktoré ukazuje pointer dest.


	void memset(void far *dest, size_c value, size_t count);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Funkcia "nastaví" oblas pamäte, na ktorú ukazuje pointer
	dest, ve¾kosti count * sizeof(size_c) (v podstate unsigned
	char) na hodnotu premennej value.


 	int memcmp(const void far *s1, const void far *s2, size_t count);
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Funkcia porovná dve oblasti pamäte - s1 a s2, ve¾kos je daná
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

Je to trieda, ktorá sa tie pouíva na prácu s textom, ale
skôr "vo vnútri", tj. text sa nevypisuje na obrazovku.

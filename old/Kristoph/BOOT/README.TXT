FAQ by Peter Jakubco ml.
^^^

1. Ako zavediem bootsektor na disketu ?

  Najprv ho treba skompilovat spustenim suboru
  create.bat a nasledne pomocou formatovacej utilitky
  naformatovat disketu s pouzitim skompilovaneho suboru
  KFS.IMG

2. Kde najdem formatovaci program ?

  V adresari \UTILS\FORMAT - nazov je k_format.exe
  alebo v adresari \UTILS\FORMAT\BOOT - nazov je k_boot.exe

3. Aky je medzi nimi rozdiel ?

  Program k_format.exe naformatuje celu disketu a vsetko
  aj na nej vymaze a program k_boot.exe iba prepise bootsektor.
  Ak mate k dispozicii disketu naformatovanu pod dosom, musite 
  ju celkom naformatovat; ak uz disketu naformatovanu mate a
  chcete na nej zmenit iba bootsektor, staci pouzit program
  k_boot.exe, ktory na diskete ponecha vsetky data.
  


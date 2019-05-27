/**
 * @file	TFT.h
 * @brief	Biblioteka za TFT displej 1.44" formata 128 x 128
 */

#ifndef UTFT_h
#define UTFT_h

///Makro za poravnavanje teksta uz levu ivicu displeja
#define LEFT 0
///Makro za poravnavanje teksta uz desnu ivicu displeja
#define RIGHT 9999
///Makro za poravnavanje teksta po sredini displeja
#define CENTER 9998

///Velicina displeja u pikselima po X osi
#define DISP_X_SIZE 128
///Velicina displeja u pikselima po y osi
#define DISP_Y_SIZE 128

/**
 * Funkcija inicijalizuje TFT displej
 */
void TFT_Init();

/**
 * Funkcija brise sadrzaj displeja, odnosno oboji sve piksele u odabranu boju pozadine, odredjenu funkcijom TFT_setBackColor.
 */
void TFT_clrScr();

/**
 * Funkcija iscrtava piksel na zadatoj poziciji. Boja iscrtanog piksela odredjena je funkcijom TFT_setColor.
 * @param x	x koordinata piksela koji se iscrtava (0..127)
 * @param y	y koordinata piksela koji se iscrtava (0..127)
 */
void TFT_drawPixel(int x, int y);

/**
 * Funkcija iscrtava pravu liniju izmedju zadatih krajnjih tacaka.
 * @param x1	x koordinata prve krajnje tacke (0..127)
 * @param y1	y koordinata prve krajnje tacke (0..127)
 * @param x2	x koordinata druge krajnje tacke (0..127)
 * @param y2	y koordinata druge krajnje tacke (0..127)
 */
void TFT_drawLine(int x1, int y1, int x2, int y2);

/**
 * Funkcija boji ceo displej u zadatu boju.
 * @param r	crvena komponenta boje (0..255)
 * @param g	zelena komponenta boje (0..255)
 * @param b	plava komponenta boje (0..255)
 */
void TFT_fillScr(unsigned char r, unsigned char g, unsigned char b);

/**
 * Funkcija iscrtava pravougaonik odredjen temenima jedne od njegovih dijagonala.
 * @param x1	x koordinata prvog temena (0..127)
 * @param y1	y koordinata prvog temena (0..127)
 * @param x2	x koordinata drugog temena (0..127)
 * @param y2	y koordinata drugog temena (0..127)
 */
void TFT_drawRect(int x1, int y1, int x2, int y2);

/**
 * Funkcija iscrtava pravougaonik sa zaobljenim ivicama, odredjen temenima jedne od njegovih dijagonala.
 * @param x1	x koordinata prvog temena (0..127)
 * @param y1	y koordinata prvog temena (0..127)
 * @param x2	x koordinata drugog temena (0..127)
 * @param y2	y koordinata drugog temena (0..127)
 */
void TFT_drawRoundRect(int x1, int y1, int x2, int y2);

/**
 * Funkcija iscrtava popunjen pravougaonik (unutrasnjost je obojena istom bojom kao ivice), odredjen temenima jedne od njegovih dijagonala.
 * @param x1	x koordinata prvog temena (0..127)
 * @param y1	y koordinata prvog temena (0..127)
 * @param x2	x koordinata drugog temena (0..127)
 * @param y2	y koordinata drugog temena (0..127)
 */
void TFT_fillRect(int x1, int y1, int x2, int y2);

/**
 * Funkcija iscrtava popunjen pravougaonik (unutrasnjost je obojena istom bojom kao ivice) sa zaobljenim ivicama, odredjen temenima jedne od njegovih dijagonala.
 * @param x1	x koordinata prvog temena (0..127)
 * @param y1	y koordinata prvog temena (0..127)
 * @param x2	x koordinata drugog temena (0..127)
 * @param y2	y koordinata drugog temena (0..127)
 */
void TFT_fillRoundRect(int x1, int y1, int x2, int y2);

/**
 * Funkcija iscrtava krug odredjen koordinatama centra i poluprecnikom.
 * @param x	x koordinata centra (0..127)
 * @param y	y koordinata centra (0..127)
 * @param radius	poluprecnik kruga (u pikselima)
 */
void TFT_drawCircle(int x, int y, int radius);

/**
 * Funkcija iscrtava popunjen krug (unutrasnjost je obojena istom bojom kao ivica) odredjen koordinatama centra i poluprecnikom.
 * @param x	x koordinata centra (0..127)
 * @param y	y koordinata centra (0..127)
 * @param radius	poluprecnik kruga (u pikselima)
 */
void TFT_fillCircle(int x, int y, int radius);

/**
 * Funkcija ispisuje string na zadatoj poziciji.
 * @param st	pokazivac na string
 * @param x	x koordinata pocetka ispisa  (0..127)
 * @param y	y koordinata pocetka ispisa (0..127)
 * @param deg	rotacija stringa oko pocetne pozicije (u stepenima, 0..360)
 */
void TFT_printString(char *st, int x, int y, int deg);

/**
 * Funkcija prima pokazivac na font koji je zadat u vidu look-up tabele u programskoj memoriji.
 * @param font	pokazivac na font
 */
void TFT_setFont(const unsigned char* font);

/**
 * Pozivom ove funkcije zadaje se boja kojom ce se nadalje iscrtavati elementi, sve do ponovnog poziva ove funkcije sa izmenjenim parametrima.
 * @param r	crvena komponenta boje (0..255)
 * @param g	zelena komponenta boje (0..255)
 * @param b	plava komponenta boje (0..255)
 */
void TFT_setColor(unsigned char r, unsigned char g, unsigned char b);

/**
 * Pozivom ove funkcije zadaje se boja pozadine. Prilikom poziva funkcije TFT_clrScr, ceo ekran ce biti obojen ovom bojom.
 * @param r	crvena komponenta boje (0..255)
 * @param g	zelena komponenta boje (0..255)
 * @param b	plava komponenta boje (0..255)
 */
void TFT_setBackColor(unsigned char r, unsigned char g, unsigned char b);

/**
 * Funkcija ispisuje celobrojnu numericku vrednost na zadatoj poziciji.
 * @param num	numericka vrednost koja se ispisuje
 * @param x	x koordinata pocetka ispisa  (0..127)
 * @param y	y koordinata pocetka ispisa (0..127)
 * @param len	ukupan broj karaktera koji se ispisuje
 * @param filler	karakter koji popunjava prazne pozicije do prve znacajne cifre broja koji se ispisuje
 */
void TFT_printNumI(long num, int x, int y, int len, char filler);

/**
 * Funkcija TFT_printNumF ispisuje numericku vrednost u pokretnom zarezu na zadatoj poziciji.
 * @param num	numericka vrednost koja se ispisuje
 * @param dec	broj decimala na koji se zaokruzuje vrednost broja
 * @param x	x koordinata pocetka ispisa (0..127)
 * @param y	y koordinata pocetka ispisa (0..127)
 * @param divider	decimalna tacka ('.'), ili zarez (',')
 * @param len	ukupan broj karaktera koji se ispisuje
 * @param filler	karakter koji popunjava prazne pozicije do prve znacajne cifre broja koji se ispisuje
 */
void TFT_printNumF(double num, unsigned char dec, int x, int y, char divider, int len, char filler);

/**
 * Funkcija iscrtava sliku (bitmapu) na zadatim koordinatama.
 * @param x	x koordinata gornjeg levog ugla slike
 * @param y	y koordinata gornjeg levog ugla slike
 * @param sx	broj piksela po x osi
 * @param sy	broj piksela po y osi
 * @param dat	pokazivac na look-up tabelu koja sadrzi informacije o slici
 * @param scale	faktor skaliranja
 */
void TFT_drawBitmap(int x, int y, int sx, int sy, unsigned int* dat, int scale);
void TFT_drawBitmap2(int x, int y, int sx, int sy, unsigned int* dat, int deg, int rox, int roy);
#endif

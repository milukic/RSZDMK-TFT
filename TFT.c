#include "TFT.h"

#include <math.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define LED_ON	PORTC |= (1 << 0)
#define LED_OFF	PORTC &= ~(1 << 0)
#define SCL_HI	PORTC |= (1 << 1)
#define SCL_LO	PORTC &= ~(1 << 1)
#define SDA_HI	PORTC |= (1 << 2)
#define SDA_LO	PORTC &= ~(1 << 2)
#define RS_HI	PORTC |= (1 << 3)
#define RS_LO	PORTC &= ~(1 << 3)
#define RST_HI	PORTC |= (1 << 4)
#define RST_LO	PORTC &= ~(1 << 4)
#define CS_HI	PORTC |= (1 << 5)
#define CS_LO	PORTC &= ~(1 << 5)

typedef struct
{
  const unsigned char* font;
  unsigned char x_size;
  unsigned char y_size;
  unsigned char offset;
  unsigned char numchars;
}_current_font;

//global variables:
unsigned char fcolorr, fcolorg, fcolorb;
unsigned char bcolorr, bcolorg, bcolorb;
_current_font cfont;

const unsigned char SmallFont[1144] PROGMEM = {
0x08, 0x0C, 0x20, 0x5F, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // <Space>
0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x20, 0x00, 0x00, // !
0x00, 0x28, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // "
0x00, 0x00, 0x28, 0x28, 0xFC, 0x28, 0x50, 0xFC, 0x50, 0x50, 0x00, 0x00, // #
0x00, 0x20, 0x78, 0xA8, 0xA0, 0x60, 0x30, 0x28, 0xA8, 0xF0, 0x20, 0x00, // $
0x00, 0x00, 0x48, 0xA8, 0xB0, 0x50, 0x28, 0x34, 0x54, 0x48, 0x00, 0x00, // %
0x00, 0x00, 0x20, 0x50, 0x50, 0x78, 0xA8, 0xA8, 0x90, 0x6C, 0x00, 0x00, // &
0x00, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // '
0x00, 0x04, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x04, 0x00, // (
0x00, 0x40, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x40, 0x00, // )
0x00, 0x00, 0x00, 0x20, 0xA8, 0x70, 0x70, 0xA8, 0x20, 0x00, 0x00, 0x00, // *
0x00, 0x00, 0x20, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, // +
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x80, // ,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // -
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, // .
0x00, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x80, 0x00, // /
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, // 0
0x00, 0x00, 0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00, // 1
0x00, 0x00, 0x70, 0x88, 0x88, 0x10, 0x20, 0x40, 0x80, 0xF8, 0x00, 0x00, // 2
0x00, 0x00, 0x70, 0x88, 0x08, 0x30, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, // 3
0x00, 0x00, 0x10, 0x30, 0x50, 0x50, 0x90, 0x78, 0x10, 0x18, 0x00, 0x00, // 4
0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, // 5
0x00, 0x00, 0x70, 0x90, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, // 6
0x00, 0x00, 0xF8, 0x90, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, // 7
0x00, 0x00, 0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, // 8
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x78, 0x08, 0x48, 0x70, 0x00, 0x00, // 9
0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, // :
0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x20, 0x00, // ;
0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, // <
0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, // =
0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, // >
0x00, 0x00, 0x70, 0x88, 0x88, 0x10, 0x20, 0x20, 0x00, 0x20, 0x00, 0x00, // ?
0x00, 0x00, 0x70, 0x88, 0x98, 0xA8, 0xA8, 0xB8, 0x80, 0x78, 0x00, 0x00, // @
0x00, 0x00, 0x20, 0x20, 0x30, 0x50, 0x50, 0x78, 0x48, 0xCC, 0x00, 0x00, // A
0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x48, 0x48, 0x48, 0xF0, 0x00, 0x00, // B
0x00, 0x00, 0x78, 0x88, 0x80, 0x80, 0x80, 0x80, 0x88, 0x70, 0x00, 0x00, // C
0x00, 0x00, 0xF0, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0xF0, 0x00, 0x00, // D
0x00, 0x00, 0xF8, 0x48, 0x50, 0x70, 0x50, 0x40, 0x48, 0xF8, 0x00, 0x00, // E
0x00, 0x00, 0xF8, 0x48, 0x50, 0x70, 0x50, 0x40, 0x40, 0xE0, 0x00, 0x00, // F
0x00, 0x00, 0x38, 0x48, 0x80, 0x80, 0x9C, 0x88, 0x48, 0x30, 0x00, 0x00, // G
0x00, 0x00, 0xCC, 0x48, 0x48, 0x78, 0x48, 0x48, 0x48, 0xCC, 0x00, 0x00, // H
0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00, // I
0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x90, 0xE0, 0x00, // J
0x00, 0x00, 0xEC, 0x48, 0x50, 0x60, 0x50, 0x50, 0x48, 0xEC, 0x00, 0x00, // K
0x00, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x44, 0xFC, 0x00, 0x00, // L
0x00, 0x00, 0xD8, 0xD8, 0xD8, 0xD8, 0xA8, 0xA8, 0xA8, 0xA8, 0x00, 0x00, // M
0x00, 0x00, 0xDC, 0x48, 0x68, 0x68, 0x58, 0x58, 0x48, 0xE8, 0x00, 0x00, // N
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, // O
0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00, // P
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0xE8, 0x98, 0x70, 0x18, 0x00, // Q
0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x50, 0x48, 0x48, 0xEC, 0x00, 0x00, // R
0x00, 0x00, 0x78, 0x88, 0x80, 0x60, 0x10, 0x08, 0x88, 0xF0, 0x00, 0x00, // S
0x00, 0x00, 0xF8, 0xA8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00, // T
0x00, 0x00, 0xCC, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x30, 0x00, 0x00, // U
0x00, 0x00, 0xCC, 0x48, 0x48, 0x50, 0x50, 0x30, 0x20, 0x20, 0x00, 0x00, // V
0x00, 0x00, 0xA8, 0xA8, 0xA8, 0x70, 0x50, 0x50, 0x50, 0x50, 0x00, 0x00, // W
0x00, 0x00, 0xD8, 0x50, 0x50, 0x20, 0x20, 0x50, 0x50, 0xD8, 0x00, 0x00, // X
0x00, 0x00, 0xD8, 0x50, 0x50, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00, // Y
0x00, 0x00, 0xF8, 0x90, 0x10, 0x20, 0x20, 0x40, 0x48, 0xF8, 0x00, 0x00, // Z
0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38, 0x00, // [
0x00, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, // <Backslash>
0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00, // ]
0x00, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ^
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, // _
0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // '
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x38, 0x48, 0x3C, 0x00, 0x00, // a
0x00, 0x00, 0xC0, 0x40, 0x40, 0x70, 0x48, 0x48, 0x48, 0x70, 0x00, 0x00, // b
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x48, 0x40, 0x40, 0x38, 0x00, 0x00, // c
0x00, 0x00, 0x18, 0x08, 0x08, 0x38, 0x48, 0x48, 0x48, 0x3C, 0x00, 0x00, // d
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x78, 0x40, 0x38, 0x00, 0x00, // e
0x00, 0x00, 0x1C, 0x20, 0x20, 0x78, 0x20, 0x20, 0x20, 0x78, 0x00, 0x00, // f
0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x48, 0x30, 0x40, 0x78, 0x44, 0x38, // g
0x00, 0x00, 0xC0, 0x40, 0x40, 0x70, 0x48, 0x48, 0x48, 0xEC, 0x00, 0x00, // h
0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00, // i
0x00, 0x00, 0x10, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0xE0, // j
0x00, 0x00, 0xC0, 0x40, 0x40, 0x5C, 0x50, 0x70, 0x48, 0xEC, 0x00, 0x00, // k
0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00, // l
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xA8, 0xA8, 0xA8, 0xA8, 0x00, 0x00, // m
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x48, 0x48, 0x48, 0xEC, 0x00, 0x00, // n
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x48, 0x48, 0x30, 0x00, 0x00, // o
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x48, 0x48, 0x48, 0x70, 0x40, 0xE0, // p
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x48, 0x48, 0x48, 0x38, 0x08, 0x1C, // q
0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x60, 0x40, 0x40, 0xE0, 0x00, 0x00, // r
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x40, 0x30, 0x08, 0x78, 0x00, 0x00, // s
0x00, 0x00, 0x00, 0x20, 0x20, 0x70, 0x20, 0x20, 0x20, 0x18, 0x00, 0x00, // t
0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x48, 0x48, 0x48, 0x3C, 0x00, 0x00, // u
0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0x48, 0x50, 0x30, 0x20, 0x00, 0x00, // v
0x00, 0x00, 0x00, 0x00, 0x00, 0xA8, 0xA8, 0x70, 0x50, 0x50, 0x00, 0x00, // w
0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x50, 0x20, 0x50, 0xD8, 0x00, 0x00, // x
0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0x48, 0x50, 0x30, 0x20, 0x20, 0xC0, // y
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x10, 0x20, 0x20, 0x78, 0x00, 0x00, // z
0x00, 0x18, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x18, 0x00, // {
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // |
0x00, 0x60, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x60, 0x00, // }
0x40, 0xA4, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ~
};

//protected function prototypes:
void TFT_Write_Bus(char VH, char VL);
void TFT_Write_COM(char VL);
void TFT_Write_DATA_2B(char VH, char VL);
void TFT_Write_DATA(char VL);
void setPixel(unsigned char r, unsigned char g, unsigned char b);
void drawHLine(int x, int y, int l);
void drawVLine(int x, int y, int l);
void printChar(unsigned char c, int x, int y);
void setXY(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void clrXY();
void rotateChar(unsigned char c, int x, int y, int pos, int deg);

// *** Hardware specific functions ***
void TFT_Write_Bus(char VH, char VL)
{
	unsigned char i;

	if (VH == 1)
		RS_HI;
	else
		RS_LO;

	for (i = 0; i < 8; i++) {
		if (VL & 0x80)
			SDA_HI;
		else
			SDA_LO;
		VL <<= 1;

		SCL_LO;
		SCL_HI;
	}
}

void TFT_Write_COM(char VL)
{
	TFT_Write_Bus(0x00, VL);
}

void TFT_Write_DATA_2B(char VH, char VL)
{
	TFT_Write_Bus(0x01, VH);
	TFT_Write_Bus(0x01, VL);
}

void TFT_Write_DATA(char VL)
{
	TFT_Write_Bus(0x01, VL);
}

int abs(int num)
{
	if (num >= 0)
		return num;
	else
		return -num;
}

void TFT_Init()
{
	DDRC = 0x3f;

	//LED Control
	LED_ON;

	//reset sequence
	RST_HI;
	_delay_ms(5);
	RST_LO;
	_delay_ms(15);
	RST_HI;
	_delay_ms(15);

	CS_LO;

	TFT_Write_COM(0x11); //Sleep exit
	_delay_ms(12);

	//ST7735R Frame Rate
	TFT_Write_COM(0xB1);
	TFT_Write_DATA(0x01);
	TFT_Write_DATA(0x2C);
	TFT_Write_DATA(0x2D);
	TFT_Write_COM(0xB2);
	TFT_Write_DATA(0x01);
	TFT_Write_DATA(0x2C);
	TFT_Write_DATA(0x2D);
	TFT_Write_COM(0xB3);
	TFT_Write_DATA(0x01);
	TFT_Write_DATA(0x2C);
	TFT_Write_DATA(0x2D);
	TFT_Write_DATA(0x01);
	TFT_Write_DATA(0x2C);
	TFT_Write_DATA(0x2D);

	TFT_Write_COM(0xB4); //Column inversion
	TFT_Write_DATA(0x07);

	//ST7735R Power Sequence
	TFT_Write_COM(0xC0);
	TFT_Write_DATA(0xA2);
	TFT_Write_DATA(0x02);
	TFT_Write_DATA(0x84);
	TFT_Write_COM(0xC1);
	TFT_Write_DATA(0xC5);
	TFT_Write_COM(0xC2);
	TFT_Write_DATA(0x0A);
	TFT_Write_DATA(0x00);
	TFT_Write_COM(0xC3);
	TFT_Write_DATA(0x8A);
	TFT_Write_DATA(0x2A);
	TFT_Write_COM(0xC4);
	TFT_Write_DATA(0x8A);
	TFT_Write_DATA(0xEE);

	TFT_Write_COM(0xC5); //VCOM
	TFT_Write_DATA(0x0E);

	TFT_Write_COM(0x36); //MX, MY, RGB mode
	TFT_Write_DATA(0xC8);

	//ST7735R Gamma Sequence
	TFT_Write_COM(0xe0);
	TFT_Write_DATA(0x0f);
	TFT_Write_DATA(0x1a);
	TFT_Write_DATA(0x0f);
	TFT_Write_DATA(0x18);
	TFT_Write_DATA(0x2f);
	TFT_Write_DATA(0x28);
	TFT_Write_DATA(0x20);
	TFT_Write_DATA(0x22);
	TFT_Write_DATA(0x1f);
	TFT_Write_DATA(0x1b);
	TFT_Write_DATA(0x23);
	TFT_Write_DATA(0x37);
	TFT_Write_DATA(0x00);

	TFT_Write_DATA(0x07);
	TFT_Write_DATA(0x02);
	TFT_Write_DATA(0x10);
	TFT_Write_COM(0xe1);
	TFT_Write_DATA(0x0f);
	TFT_Write_DATA(0x1b);
	TFT_Write_DATA(0x0f);
	TFT_Write_DATA(0x17);
	TFT_Write_DATA(0x33);
	TFT_Write_DATA(0x2c);
	TFT_Write_DATA(0x29);
	TFT_Write_DATA(0x2e);
	TFT_Write_DATA(0x30);
	TFT_Write_DATA(0x30);
	TFT_Write_DATA(0x39);
	TFT_Write_DATA(0x3f);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x07);
	TFT_Write_DATA(0x03);
	TFT_Write_DATA(0x10);

	TFT_Write_COM(0x2a);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x7f);
	TFT_Write_COM(0x2b);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x00);
	TFT_Write_DATA(0x9f);

	TFT_Write_COM(0xF0); //Enable test command
	TFT_Write_DATA(0x01);
	TFT_Write_COM(0xF6); //Disable ram power save mode
	TFT_Write_DATA(0x00);

	TFT_Write_COM(0x3A); //65k mode
	TFT_Write_DATA(0x05);
	TFT_Write_COM(0x29); //Display on

	CS_HI;

	fcolorr = fcolorg = fcolorb = 255;
	bcolorr = bcolorg = bcolorb = 0;
	cfont.font = 0;
	TFT_setFont(SmallFont);
}

void setXY(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	TFT_Write_COM(0x2a);
	TFT_Write_DATA(x1 >> 8);
	TFT_Write_DATA(x1);
	TFT_Write_DATA(x2 >> 8);
	TFT_Write_DATA(x2);
	TFT_Write_COM(0x2b);
	TFT_Write_DATA(y1 >> 8);
	TFT_Write_DATA(y1 + 32);
	TFT_Write_DATA(y2 >> 8);
	TFT_Write_DATA(y2 + 32);
	TFT_Write_COM(0x2c);
}

void clrXY()
{
	setXY(0, 0, DISP_X_SIZE - 1, DISP_Y_SIZE - 1);
}

void TFT_drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	drawHLine(x1, y1, x2 - x1);
	drawHLine(x1, y2, x2 - x1);
	drawVLine(x1, y1, y2 - y1);
	drawVLine(x2, y1, y2 - y1);
}

void TFT_drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	if ((x2 - x1) > 4 && (y2 - y1) > 4)
	{
		TFT_drawPixel(x1 + 1, y1 + 1);
		TFT_drawPixel(x2 - 1, y1 + 1);
		TFT_drawPixel(x1 + 1, y2 - 1);
		TFT_drawPixel(x2 - 1, y2 - 1);
		drawHLine(x1 + 2, y1, x2 - x1 - 4);
		drawHLine(x1 + 2, y2, x2 - x1 - 4);
		drawVLine(x1, y1 + 2, y2 - y1 - 4);
		drawVLine(x2, y1 + 2, y2 - y1 - 4);
	}
}

void TFT_fillRect(int x1, int y1, int x2, int y2)
{
	int tmp, i;

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	for (i = 0; i < ((y2 - y1) / 2) + 1; i++)
	{
		drawHLine(x1, y1 + i, x2 - x1);
		drawHLine(x1, y2 - i, x2 - x1);
	}
}

void TFT_fillRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp, i;

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	if ((x2 - x1) > 4 && (y2 - y1) > 4)
	{
		for (i = 0; i < ((y2 - y1) / 2) + 1; i++)
		{
			switch (i)
			{
				case 0:
					drawHLine(x1 + 2, y1 + i, x2 - x1 - 4);
					drawHLine(x1 + 2, y2 - i, x2 - x1 - 4);
					break;
				case 1:
					drawHLine(x1 + 1, y1 + i, x2 - x1 - 2);
					drawHLine(x1 + 1, y2 - i, x2 - x1 - 2);
					break;
				default:
					drawHLine(x1, y1 + i, x2 - x1);
					drawHLine(x1, y2 - i, x2 - x1);
			}
		}
	}
}

void TFT_drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	char ch, cl;

	ch = ((fcolorr & 248) | fcolorg >> 5);
	cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

	CS_LO;
	setXY(x, y + radius, x, y + radius);
	TFT_Write_DATA_2B(ch, cl);
	setXY(x, y - radius, x, y - radius);
	TFT_Write_DATA_2B(ch, cl);
	setXY(x + radius, y, x + radius, y);
	TFT_Write_DATA_2B(ch, cl);
	setXY(x - radius, y, x - radius, y);
	TFT_Write_DATA_2B(ch, cl);

	while (x1 < y1)
	{
		if (f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		setXY(x + x1, y + y1, x + x1, y + y1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		TFT_Write_DATA_2B(ch, cl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		TFT_Write_DATA_2B(ch, cl);
	}
	CS_HI;
	clrXY();
}

void TFT_fillCircle(int x, int y, int radius)
{
	int x1, y1;

	CS_LO;
	for (y1 = -radius; y1 <= radius; y1++)
		for (x1 = -radius; x1 <= radius; x1++)
			if (x1 * x1 + y1 * y1 <= radius * radius)
			{
				setXY(x + x1, y + y1, x + x1, y + y1);
				TFT_Write_DATA_2B(((fcolorr & 248) | fcolorg >> 5),
						((fcolorg & 28) << 3 | fcolorb >> 3));
			}
	CS_HI;
	clrXY();
}

void TFT_clrScr()
{
	long i;

	CS_LO;
	clrXY();

	for (i = 0; i < ((DISP_X_SIZE) * (DISP_Y_SIZE)); i++)
	{
		TFT_Write_Bus(1, 0);
		TFT_Write_Bus(1, 0);
	}
	CS_HI;
}

void TFT_fillScr(unsigned char r, unsigned char g, unsigned char b)
{
	long i;
	char ch, cl;

	ch = ((r & 248) | g >> 5);
	cl = ((g & 28) << 3 | b >> 3);

	CS_LO;
	clrXY();

	for (i = 0; i < ((DISP_X_SIZE + 1) * (DISP_Y_SIZE + 1)); i++)
	{
		TFT_Write_Bus(1, ch);
		TFT_Write_Bus(1, cl);
	}
	CS_HI;
}

void TFT_setColor(unsigned char r, unsigned char g, unsigned char b)
{
	fcolorr = r;
	fcolorg = g;
	fcolorb = b;
}

void TFT_setBackColor(unsigned char r, unsigned char g, unsigned char b)
{
	bcolorr = r;
	bcolorg = g;
	bcolorb = b;
}

void setPixel(unsigned char r, unsigned char g, unsigned char b)
{
	TFT_Write_DATA_2B(((r & 248) | g >> 5), ((g & 28) << 3 | b >> 3)); // rrrrrggggggbbbbb
}

void TFT_drawPixel(int x, int y)
{
	CS_LO;
	setXY(x, y, x, y);
	setPixel(fcolorr, fcolorg, fcolorb);
	CS_HI;
	clrXY();
}

void TFT_drawLine(int x1, int y1, int x2, int y2)
{
	int tmp, i;
	float delta, tx, ty;
	//float m, b, dx, dy;
	char ch, cl;

	ch = ((fcolorr & 248) | fcolorg >> 5);
	cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

	if (((x2 - x1) < 0))
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	if (((y2 - y1) < 0))
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	if (y1 == y2)
	{
		if (x1 > x2)
		{
			tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		drawHLine(x1, y1, x2 - x1);
	}
	else if (x1 == x2)
	{
		if (y1 > y2)
		{
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		drawVLine(x1, y1, y2 - y1);
	}
	else if (abs(x2 - x1) > abs(y2 - y1))
	{
		CS_LO;
		delta = ((float) (y2 - y1) / (float) (x2 - x1));
		ty = (float) (y1);
		if (x1 > x2)
		{
			for (i = x1; i >= x2; i--)
			{
				setXY(i, (int) (ty + 0.5), i, (int) (ty + 0.5));
				TFT_Write_DATA_2B(ch, cl);
				ty = ty - delta;
			}
		}
		else
		{
			for (i = x1; i <= x2; i++)
			{
				setXY(i, (int) (ty + 0.5), i, (int) (ty + 0.5));
				TFT_Write_DATA_2B(ch, cl);
				ty = ty + delta;
			}
		}
		CS_HI;
	}
	else
	{
		CS_LO;
		delta = ((float) (x2 - x1) / (float) (y2 - y1));
		tx = (float) (x1);
		if (y1 > y2)
		{
			for (i = y2 + 1; i > y1; i--)
			{
				setXY((int) (tx + 0.5), i, (int) (tx + 0.5), i);
				TFT_Write_DATA_2B(ch, cl);
				tx = tx + delta;
			}
		}
		else
		{
			for (i = y1; i < y2 + 1; i++)
			{
				setXY((int) (tx + 0.5), i, (int) (tx + 0.5), i);
				TFT_Write_DATA_2B(ch, cl);
				tx = tx + delta;
			}
		}
		CS_HI;
	}

	clrXY();
}

void drawHLine(int x, int y, int l) {
	int i;
	char ch, cl;

	ch = ((fcolorr & 248) | fcolorg >> 5);
	cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

	CS_LO;
	setXY(x, y, x + l, y);
	for (i = 0; i < l + 1; i++)
		TFT_Write_DATA_2B(ch, cl);
	CS_HI;
	clrXY();
}

void drawVLine(int x, int y, int l) {
	int i;
	char ch, cl;

	ch = ((fcolorr & 248) | fcolorg >> 5);
	cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

	CS_LO;
	setXY(x, y, x, y + l);
	for (i = 0; i < l; i++)
		TFT_Write_DATA_2B(ch, cl);
	CS_HI;
	clrXY();
}

void printChar(unsigned char c, int x, int y) {
	unsigned char i, ch;
	unsigned int j;
	unsigned int temp;

	CS_LO;

	setXY(x, y, x + cfont.x_size - 1, y + cfont.y_size - 1);

	temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
	for (j = 0; j < ((cfont.x_size / 8) * cfont.y_size); j++)
	{
		ch = pgm_read_byte(&cfont.font[temp]);
		for (i = 0; i < 8; i++) {
			if ((ch & (1 << (7 - i))) != 0)
			{
				setPixel(fcolorr, fcolorg, fcolorb);
			}
			else
			{
				setPixel(bcolorr, bcolorg, bcolorb);
			}
		}
		temp++;
	}

	CS_HI;
	clrXY();
}

void rotateChar(unsigned char c, int x, int y, int pos, int deg)
{
	unsigned char i, j, ch;
	unsigned int temp;
	int zz, newx, newy;
	float radian;

	radian = deg * 0.0175;

	CS_LO;

	temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
	for (j = 0; j < cfont.y_size; j++)
	{
		for (zz = 0; zz < (cfont.x_size / 8); zz++)
		{
			ch = pgm_read_byte(&cfont.font[temp + zz]);
			for (i = 0; i < 8; i++)
			{
				newx = x + (((i + (zz * 8) + (pos * cfont.x_size)) * cos(radian)) - ((j) * sin(radian)));
				newy = y + (((j) * cos(radian)) + ((i + (zz * 8) + (pos * cfont.x_size)) * sin(radian)));

				setXY(newx, newy, newx + 1, newy + 1);

				if ((ch & (1 << (7 - i))) != 0)
				{
					setPixel(fcolorr, fcolorg, fcolorb);
				}
				else
				{
					setPixel(bcolorr, bcolorg, bcolorb);
				}
			}
		}
		temp += (cfont.x_size / 8);
	}
	CS_HI;
	clrXY();
}

void TFT_printString(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (x == RIGHT)
		x = (DISP_X_SIZE) - (stl * cfont.x_size);
	if (x == CENTER)
		x = ((DISP_X_SIZE) - (stl * cfont.x_size)) / 2;

	for (i = 0; i < stl; i++)
		if (deg == 0)
			printChar(*st++, x + (i * (cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void TFT_printNumI(long num, int x, int y, int len, char filler)
{
	char buf[25];
	char st[27];
	char neg = 0;
	int i, c = 0, f = 0;

	if (num == 0)
	{
		if (len != 0)
		{
			for (c = 0; c < (len - 1); c++)
				st[c] = filler;
			st[c] = 48;
			st[c + 1] = 0;
		}
		else
		{
			st[0] = 48;
			st[1] = 0;
		}
	}
	else
	{
		if (num < 0)
		{
			neg = 1;
			num = -num;
		}

		while (num > 0)
		{
			buf[c] = 48 + (num % 10);
			c++;
			num = (num - (num % 10)) / 10;
		}
		buf[c] = 0;

		if (neg)
		{
			st[0] = 45;
		}

		if (len > (c + neg))
		{
			for (i = 0; i < (len - c - neg); i++)
			{
				st[i + neg] = filler;
				f++;
			}
		}

		for (i = 0; i < c; i++)
		{
			st[i + neg + f] = buf[c - i - 1];
		}
		st[c + neg + f] = 0;

	}

	TFT_printString(st, x, y, 0);
}

void TFT_printNumF(double num, unsigned char dec, int x, int y, char divider, int len, char filler)
{
	char buf[25];
	char st[27];
	char neg = 0;
	int i, j, c = 0, f = 0;
	int c2, mult;
	unsigned long inum;

	if (dec < 1)
		dec = 1;
	if (dec > 5)
		dec = 5;

	if (num == 0)
	{
		if (len != 0)
		{
			for (c = 0; c < (len - 2 - dec); c++)
				st[c] = filler;
			st[c] = 48;
			st[c + 1] = divider;
			for (i = 0; i < dec; i++)
				st[c + 2 + i] = 48;
			st[c + 2 + dec] = 0;
		}
		else
		{
			st[0] = 48;
			st[1] = divider;
			for (i = 0; i < dec; i++)
				st[2 + i] = 48;
			st[2 + dec] = 0;
		}
	}
	else
	{
		if (num < 0)
		{
			neg = 1;
			num = -num;
		}

		mult = 1;
		for (j = 0; j < dec; j++)
			mult = mult * 10;
		inum = (long) (num * mult + 0.5);

		while (inum > 0)
		{
			buf[c] = 48 + (inum % 10);
			c++;
			inum = (inum - (inum % 10)) / 10;
		}
		if ((num < 1) && (num > 0))
		{
			buf[c] = 48;
			c++;
		}
		while (c < (dec + 1))
		{
			buf[c] = 48;
			c++;
		}
		buf[c] = 0;

		if (neg)
		{
			st[0] = 45;
		}

		if (len > (c + neg - 1))
		{
			for (i = 0; i < (len - c - neg - 1); i++)
			{
				st[i + neg] = filler;
				f++;
			}
		}

		c2 = neg;
		for (i = 0; i < c; i++)
		{
			st[c2 + f] = buf[c - i - 1];
			c2++;
			if ((c - (c2 - neg)) == dec)
			{
				st[c2 + f] = divider;
				c2++;
			}
		}
		st[c2 + f] = 0;
	}

	TFT_printString(st, x, y, 0);
}

void TFT_setFont(const unsigned char* font)
{
	cfont.font = font;
	cfont.x_size = pgm_read_byte(&font[0]);
	cfont.y_size = pgm_read_byte(&font[1]);
	cfont.offset = pgm_read_byte(&font[2]);
	cfont.numchars = pgm_read_byte(&font[3]);
}

void TFT_drawBitmap(int x, int y, int sx, int sy, prog_uint16_t* dat, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale == 1)
	{
		CS_LO;
		setXY(x, y, x + sx - 1, y + sy - 1);
		for (tc = 0; tc < (sx * sy); tc++)
		{
			col = pgm_read_word(&dat[tc]);
			TFT_Write_DATA_2B(col >> 8, col & 0xff);
		}
		CS_HI;
	}
	else
	{
		CS_LO;
		for (ty = 0; ty < sy; ty++)
		{
			setXY(x, y + (ty * scale), x + ((sx * scale) - 1),
					y + (ty * scale) + scale);
			for (tsy = 0; tsy < scale; tsy++)
				for (tx = 0; tx < sx; tx++)
				{
					col = pgm_read_word(&dat[(ty * sx) + tx]);
					for (tsx = 0; tsx < scale; tsx++)
						TFT_Write_DATA_2B(col >> 8, col & 0xff);
				}
		}
		CS_HI;
	}
	clrXY();
}

void TFT_drawBitmap2(int x, int y, int sx, int sy, prog_uint16_t* dat, int deg)
{
	unsigned int col;
	int tx, ty, newx, newy;

	float radian;
	radian = deg * 0.0175;

	if (deg == 0)
		TFT_drawBitmap(x, y, sx, sy, dat, 1);
	else
	{
		CS_LO;
		for (ty = 0; ty < sy; ty++)
			for (tx = 0; tx < sx; tx++)
			{
				col = pgm_read_word(&dat[(ty * sx) + tx]);

				newx = x + ((tx * cos(radian)) - (ty * sin(radian)));
				newy = y + ((ty * cos(radian)) + (tx * sin(radian)));

				setXY(newx, newy, newx, newy);
				TFT_Write_DATA_2B(col >> 8, col & 0xff);
			}
		CS_HI;
	}
	clrXY();
}

/*
 * bmp.h
 *
 *  Created on: 2016年4月20日
 *      Author: gentalk
 */

#ifndef BMP_H_
#define BMP_H_

unsigned char BMP1[] =
{
	0x00,0x03,0x05,0x09,0x11,0xFF,0x11,0x89,0x05,0xC3,0x00,0xE0,0x00,0xF0,0x00,0xF8,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x28,0xFF,0x11,0xAA,0x44,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,0x01,0x38,0x44,0x82,0x92,
	0x92,0x74,0x01,0x83,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x44,0xC7,0x01,0x7D,
	0x7D,0x7D,0x7D,0x01,0x7D,0x7D,0x7D,0x7D,0x01,0x7D,0x7D,0x7D,0x7D,0x01,0xFF,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x00,0x00,
	0x6D,0x6D,0x6D,0x6D,0x6D,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x40,0x40,
	0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDB,0xDB,0xDB,0xDB,0xDB,0x00,0x00,
	0xDB,0xDB,0xDB,0xDB,0xDB,0x00,0x00,0xDB,0xDB,0xDB,0xDB,0xDB,0x00,0x00,0xDB,0xDB,
	0xDB,0xDB,0xDB,0x00,0x00,0xDA,0xDA,0xDA,0xDA,0xDA,0x00,0x00,0xD8,0xD8,0xD8,0xD8,
	0xD8,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
	0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x80,
	0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x00,0x00,
	0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x06,0x06,
	0x06,0x06,0x06,0x00,0x00,0x06,0x06,0x06,0xE6,0x66,0x20,0x00,0x06,0x06,0x86,0x06,
	0x06,0x00,0x00,0x06,0x06,0x06,0x06,0x86,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x00,
	0x00,0x86,0x86,0x86,0x86,0x86,0x80,0x80,0x86,0x86,0x06,0x86,0x86,0xC0,0xC0,0x86,
	0x86,0x86,0x06,0x06,0xD0,0x30,0x76,0x06,0x06,0x06,0x06,0x00,0x00,0x06,0x06,0x06,
	0x06,0x06,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x06,0x06,0x06,0x06,0x06,
	0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x1C,0x00,0xFE,0x00,0x01,
	0x02,0x00,0xC4,0x18,0x20,0x02,0x9E,0x63,0xB2,0x0E,0x00,0xFF,0x81,0x81,0xFF,0x00,
	0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0xFF,0x00,0x23,0xEA,0xAA,0xBF,0xAA,
	0xEA,0x03,0x3F,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0C,0x08,0x00,0x00,0x01,0x01,0x01,
	0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x81,0x80,0x80,0x81,0x80,
	0x81,0x80,0x80,0x80,0x80,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
	0x01,0x00,0x01,0x01,0x09,0x0C,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
	0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
	0x00,0x1E,0x21,0x40,0x40,0x50,0x21,0x5E,0x00,0x1E,0x21,0x40,0x40,0x50,0x21,0x5E,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xC1,0xC1,0xFF,
	0xFF,0xC1,0xC1,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xFC,0xF3,0xEF,0xF3,0xFC,
	0x80,0xFF,0x80,0xEE,0xEE,0xEE,0xF5,0xFB,0xFF,0x9C,0xBE,0xB6,0xB6,0x88,0xFF,0x00,
};

// 128 64   页1个字节8个位
unsigned char LOGO_BMP[128][8] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
	{0x60,0x00,0x00,0x00,0x00,0x10,0x00,0x6C},
	{0x70,0x00,0x00,0x00,0x00,0x30,0x00,0x00},
	{0x78,0x00,0x00,0x00,0x00,0xF0,0x00,0x7C},
	{0x7C,0x00,0x00,0x00,0x07,0xF0,0x00,0xFE},
	{0x7E,0x00,0x00,0x00,0x3F,0xF0,0x00,0xC6},
	{0x7F,0x00,0x00,0x01,0xFF,0xF0,0x00,0xC6},
	{0x7F,0x80,0x00,0x0F,0xFF,0xF0,0x00,0xC6},
	{0x7F,0xC0,0x00,0x7F,0xFF,0xF0,0x00,0xFE},
	{0x7F,0xE0,0x03,0xFF,0xFF,0xF0,0x00,0x7C},
	{0x7F,0xF0,0x3F,0xFF,0xFF,0xF0,0x00,0x02},
	{0x7F,0xF8,0x3F,0xFF,0xFF,0xF0,0x00,0x06},
	{0x7F,0xFC,0x3F,0xFF,0xFF,0xF0,0x00,0x1E},
	{0x7F,0xFE,0x3F,0xFF,0xFF,0xF0,0x00,0xBC},
	{0x7F,0xFF,0x3F,0xFF,0xFF,0xF0,0x00,0xE0},
	{0x7F,0xFF,0xBF,0xFF,0xFF,0x80,0x00,0xF8},
	{0x7F,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x3E},
	{0x7F,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x0E},
	{0x7F,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xB8},
	{0x7F,0xFF,0xFF,0xF8,0x00,0x00,0x00,0xE0},
	{0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0xFE},
	{0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x1E},
	{0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x02},
	{0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00},
	{0x7F,0xEF,0xFF,0xF0,0x02,0x00,0x00,0x06},
	{0x7F,0xE7,0xFF,0xF0,0x02,0x00,0x00,0x0E},
	{0x7F,0xE3,0xFF,0xF0,0x02,0x00,0x00,0x1C},
	{0x7F,0xE1,0xFF,0xF8,0x03,0x00,0x00,0xBA},
	{0x7F,0xE0,0xFF,0xFC,0x03,0x00,0x00,0xF6},
	{0x7F,0xE0,0x7F,0xFE,0x03,0x80,0x00,0xE6},
	{0x7F,0xE0,0x3F,0xFF,0x03,0x80,0x00,0xF6},
	{0x7F,0xE0,0x3F,0xFF,0x83,0xC0,0x00,0x3E},
	{0x7F,0xE0,0x3F,0xFF,0xC3,0xC0,0x00,0x0E},
	{0x7F,0xE0,0x3F,0xFF,0xE3,0xE0,0x00,0x02},
	{0x7F,0xE0,0x3F,0xFF,0xF3,0xE0,0x00,0x00},
	{0x7F,0xE0,0x3F,0xFF,0xFB,0xF0,0x00,0x02},
	{0x7F,0xE0,0x3F,0xFF,0xFF,0xF0,0x00,0x66},
	{0x7F,0xE0,0x3F,0xFF,0xFF,0xF8,0x00,0xF6},
	{0x7F,0xE0,0x3F,0xFF,0xFF,0xF8,0x00,0xD6},
	{0x7F,0xE0,0x3F,0xFF,0xFF,0xFC,0x00,0xD6},
	{0x7F,0xF0,0x7F,0xFF,0xFF,0xFC,0x00,0xD6},
	{0x7F,0xF8,0xFF,0xF7,0xFF,0xFE,0x00,0xD6},
	{0x7F,0xFF,0xFF,0xF3,0xFF,0xFE,0x00,0xDE},
	{0x3F,0xFF,0xFF,0xE1,0xFF,0xFF,0x00,0x8C},
	{0x3F,0xFF,0xFF,0xE0,0xFF,0xCF,0x00,0x40},
	{0x1F,0xFF,0xFF,0xC0,0x7F,0xC7,0x80,0xC0},
	{0x1F,0xFF,0xFF,0xC0,0x3F,0xC3,0x80,0xC0},
	{0x0F,0xFF,0xFF,0x80,0x1F,0xC1,0xC0,0xFE},
	{0x07,0xFF,0xFF,0x00,0x0F,0xC0,0xC0,0xFE},
	{0x03,0xFF,0xFE,0x00,0x07,0xC0,0x60,0xC0},
	{0x01,0xFF,0xFC,0x00,0x03,0xC0,0x20,0xC0},
	{0x00,0x7F,0xF0,0x00,0x01,0xC0,0x00,0x86},
	{0x00,0x0F,0x80,0x00,0x00,0xC0,0x00,0x16},
	{0x00,0x00,0x00,0x00,0x00,0x40,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x62},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8C},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

#endif /* BMP_H_ */

/*
 * Copyright (C) 2012 by denis@denis.com.br
 *
 * All rights reserved. No part of this software may be reproduced,transmitted,
 * transcribed, stored in a retrieval system, or translated into any language
 * or computer language, in any form or by any means, electronic, mechanical,
 * magnetic, optical, chemical, manual or otherwise, without the prior written
 * permission of DENIS DOS SANTOS SILVA.
 *
*/



/*

================================================================
this demo run on verifone devices (vx/evo) and you must sign it!
================================================================

$15 usd per font
$1 usd per device


Magic        0xA3 (program)
Flags        0x06 (Thumb, 4KB Aligned)
Version      1.0
Code Addr    0x70420040
Code Size    320508 (0x4E3FC)
Data Addr    0x7046F000
Data Size    156 (0x9C)
Heap Size    0 (0x0)
Stack Addr   0x70220000
Stack Size   2097152 (0x200000)
Entry        0x70420089 (Thumb code)
Library      .SYS.LIB
Library      N:/ELOG.LIB

library features:
	- c89/99 compatible
	- os/hardware agnostic
	- no external deps (libs)
	- no extra files
	- no memory leaks (malloc/free)
	- static or dynamic linking
	- callback + context-based
	- metrics
	- usage of same font on screen/printer =)

Impact:
	- https://docs.microsoft.com/en-us/typography/font-list/impact
	- https://www.dafontfree.io/impact-font/
Nokia Cellphone FC:
	- https://www.dafont.com/pt/nokia-cellphone.font
	- https://www.dafont.com/pt/zeh-jose-fernando-baldo-caneiro.d467
	- https://portfolio.zehfernando.com/
DS-Digital:
	- https://www.dafont.com/ds-digital.font
	- https://www.dafont.com/dusit-supasawat.d411
*/



#include <stdio.h>
#include <time.h>
#include <svc.h>

#include "tf.h"

#define HEXRGB_TO_RGB16(color)		((color & 0xF80000) >> 8) | ((color & 0xFC00) >> 5) | ((color & 0xF8) >> 2)
#define	KEY_0						0x30
#define	KEY_1						0x31
#define	KEY_2						0x32
#define	KEY_3						0x33
#define	KEY_4						0x34
#define	KEY_5						0x35
#define	KEY_6						0x36
#define	KEY_7						0x37
#define	KEY_8						0x38
#define	KEY_9						0x39
#define KEY_ENTER					0x0d
#define KEY_ESC						0x1b
#define KEY_BS						0x08
#define KEY_STAR					0x2a
#define KEY_HASH					0x23

#define draw						tf_draw_string
#define draw_ex						tf_draw_string_ex
#define font_height					tf_get_font_height

#define sleep(x)					SVC_WAIT((unsigned int) ((unsigned int) x) * 1000)
#define sleep_ms(x)					SVC_WAIT((unsigned int) ((unsigned int) x))

// TODO: buffered/printer/bitmap/fb

void drawer(void *ctx, uint32_t x0, uint32_t y0, uint32_t color) {
	(void) ctx;
    write_pixels(x0, y0, x0, y0, HEXRGB_TO_RGB16(color));
}

void drawer_blue(void *ctx, uint32_t x0, uint32_t y0, uint32_t color) {
	(void) ctx;
	write_pixels(x0, y0, x0, y0, (color) ? HEXRGB_TO_RGB16(0xffffff) : HEXRGB_TO_RGB16(0x0000ff));
}

void drawer_invert(void *ctx, uint32_t x0, uint32_t y0, uint32_t color) {
	(void) ctx;
	write_pixels(x0, y0, x0, y0, (color) ? HEXRGB_TO_RGB16(0x000000) : HEXRGB_TO_RGB16(0xffffff));
}

//
// entry point
//

const char *COPYRIGHT =
"/*"
" * Copyright (C) 2012 by denis@denis.com.br"
" *"
" * All rights reserved. No part of this software may be reproduced,transmitted,"
" * transcribed, stored in a retrieval system, or translated into any language"
" * or computer language, in any form or by any means, electronic, mechanical,"
" * magnetic, optical, chemical, manual or otherwise, without the prior written"
" * permission of DENIS DOS SANTOS SILVA."
" *"
"*/"
"";
	
const char BUILD[32] = __DATE__  " " __TIME__;
	
int main(const int argc, const char **argv) {
	callback_t *cb;
	void *ctx = NULL;
	const void *font;
	int console, timer;
	int offset = 0;
	long evt = 0L;
	char buffer[128];
	displayInfo_t di;

	console = open(DEV_CONSOLE, 0);
	if (console < 0) SVC_RESTART("");

	if ((_SYS_VERSION>=0x301) && (_syslib_version() >= 0x301)) {
		setFree(FREE_TYPE_NO_VALIDATE);
		SVC_WAIT(50);
	}

	tf_init(NULL);
	
	set_cursor(0);
	set_backlight(1);
	set_backlight_level(100);
	set_kb_backlight(100);
	
	#if 0
	set_display_color(FOREGROUND_COLOR, HEXRGB_TO_RGB16(0x0000ff));
	set_display_color(BACKGROUND_COLOR, HEXRGB_TO_RGB16(0xffffff));
	#endif

	window(-1, -1, -1, -1);
	set_font("");
	
	clrscr();
	gotoxy(3, 1);
	fprintf(stdout, "any key to continue");

	get_console(1);
	read_event();

	for(;;) {
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}
	
	set_display_coordinate_mode(PIXEL_MODE);
	SVC_INFO_DISPLAY_EXT(&di);
	SVC_WAIT(1500);
	
	///
	
	clrscr();

	for(;;) {
		cb = &drawer;
		font = (const void *) &ncfc_normal_8px;		

		draw(font, "0123456789", strlen("0123456789"), 10, 50*1, cb, ctx);
		draw(font, "abcdef", strlen("abcdef"), 10, 50*2, cb, ctx);
		draw(font, "ABCDEF", strlen("ABCDEF"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}

	///
	
	get_console(1);
	clrscr();

	for(;;) {
		cb = &drawer;
		font = (const void *) &ncfc_normal_12px;

		draw(font, "0123456789", strlen("0123456789"), 10, 50*1, cb, ctx);
		draw(font, "abcdef", strlen("abcdef"), 10, 50*2, cb, ctx);
		draw(font, "ABCDEF", strlen("ABCDEF"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}

	///
	
	get_console(1);
	clrscr();
	
	for(;;) {
		cb = &drawer_invert;
		font = (const void *) &ncfc_normal_16px;
		
		draw(font, "0123456789", strlen("0123456789"), 10, 50*1, cb, ctx);
		draw(font, "abcdef", strlen("abcdef"), 10, 50*2, cb, ctx);
		draw(font, "ABCDEF", strlen("ABCDEF"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}
	
	///
	
	get_console(1);
	clrscr();
	
	for(;;) {
		cb = &drawer;
		font = (const void *) &ncfc_normal_16px;
	
		draw(font, "0123456789", strlen("0123456789"), 10, 50*1, cb, ctx);
		draw(font, "abcdef", strlen("abcdef"), 10, 50*2, cb, ctx);
		draw(font, "ABCDEF", strlen("ABCDEF"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}

	///
	
	get_console(1);
	clrscr();

	for (;;) {
		cb = &drawer;
		font = (const void *) &ncfc_normal_18px;

		// nokia_cellphone0.png
		// &eacute ='é' = 0xe9 = 233
		
		draw(font, "c\xe9u laranja", strlen("ceu laranja"), 10, 50*1, cb, ctx);
		draw(font, "012345", strlen("012345"), 10, 50*2, cb, ctx);
		draw(font, "abcdef", strlen("abcdef"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}
	
	///
	
	get_console(1);
	clrscr();
	
	for (;;) {
		cb = &drawer;
		font = (const void *) &ncfc_normal_36px;

		draw(font, "c" "\xe9" "u", strlen("ceu"), 10, 50*1, cb, ctx);
		draw(font, "azul", strlen("azul"), 10, 50*2, cb, ctx);
		draw(font, "verde", strlen("verde"), 10, 50*3, cb, ctx);
		
		evt = wait_evt( EVT_KBD );
		if (evt & EVT_KBD) break;
	}
	
	// TODO: more examples
	
	///
	
	int x = 2;
	int y = 5;
	int isDone = 0;

	get_console(1);
	clrscr();

	cb = &drawer;
	font = (const void *) &ncfc_normal_14px;
	
	for(;;) {
		unsigned char buf;
		time_t now = time(NULL);
		struct tm *t = gmtime(&now);

		/*
		####### ## ######## ####
		012345678901234567890123
		WWW MMM dd hh:mm:ss yyyy
		*/

		snprintf(buffer, sizeof(buffer), "%s", asctime(t));
		// TODO: if time is not changed... continue...
		
		
		offset = font_height(font);
		draw(font, &buffer[0x00], 7, x,          y, cb, ctx);
		draw(font, &buffer[0x08], 2, x, y+offset*1, cb, ctx);
		draw(font, &buffer[0x0b], 9, x, y+offset*2, cb, ctx);
		draw(font, &buffer[0x14], 4, x, y+offset*3, cb, ctx);
		
		if (read(console, &buf, 1) == 1) {
			buf &= 0x7f;
			switch(buf) {
				case KEY_0:		font	= &ncfc_normal_14px;		clrscr(); continue;
				case KEY_1:		font	= &ncfc_normal_16px;		clrscr(); continue;
				case KEY_2:		font	= &ncfc_normal_18px;		clrscr(); continue;
				case KEY_3:		font	= &ncfc_normal_20px;		clrscr(); continue;
				case KEY_4:		font	= &ncfc_normal_22px;		clrscr(); continue;
				case KEY_5:		font	= &ncfc_normal_24px;		clrscr(); continue;
				case KEY_6:		font	= &ncfc_normal_26px;		clrscr(); continue;
				case KEY_7:		font	= &ncfc_normal_28px;		clrscr(); continue;
				case KEY_8:		font	= &ds_digital_negrito_36px;	clrscr(); continue;
				case KEY_9:		font	= &impact_normal_20px;		clrscr(); continue;
				case KEY_STAR:	cb		= &drawer;					clrscr(); continue;
				case KEY_HASH:	cb		= &drawer_invert;			clrscr(); continue;
				case KEY_BS:	cb		= &drawer_blue;				clrscr(); continue;
				case KEY_ESC:	isDone=1;
			}
		}

		if (isDone) break;
		sleep_ms( 500 );
	}

	/// done
	
	_remove("CONFIG.SYS");
	SVC_WAIT(1000);
	SVC_RESTART("");

	return EXIT_SUCCESS;
}



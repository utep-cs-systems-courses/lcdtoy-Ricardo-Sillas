/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include <p2switches.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
/** Initializes everything, clears the screen, draws "hello" and a square */

// Screen size: 128x160

// Starting position of box
u_char x_pos = 64;
u_char y_pos = 80;

u_int color = COLOR_GOLDENROD;		// Main color of box
int l = 0;				// Position for sound arrays
int count = 0;				// timer for sleep
int timer = 0;				// timer for sound

// For sound
int tempo[] = {7,2,7,2,7,2,7,2,14,2,7,2,7,2,14,2,14,2,14,2,7,2,7,2,14,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,14,2,14,2,14,2,7,2,7,2,14,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2};
int as[] = {2025,2273,2408,3034,2703,2703,1804,2025,2273,2408,2408,2408,2025,2273,2408,2703,2703,1136,1204,1136,1204,1136,2703,2703,1136,1204,1136,1204,1136,2703,2703,1804,2025,2273,2408,2408,2408,2025,2273,2408,2703,2703,1136,1204,1136,1204,1136,2703,2703,1136,1204,1136,1204,1136};

// Creates box
void perPos(u_char x_pos, u_char y_pos, u_int color) {
  u_char i;
  u_char j;
  for(j = 0; j <= 10; j++) {
    for(i = 0; i <= 20; i++) {
      drawPixel(j + x_pos, i + y_pos, color);
    }
  }
  for(j = 0; j <= 20; j++) {
    for(i = 0; i <= 2; i++) {
      drawPixel(x_pos - 1 - i, j + y_pos, COLOR_BLUE);
      drawPixel(x_pos + 11 + i, j + y_pos, COLOR_BLUE);
      drawPixel(j + x_pos - 2, y_pos - 1 - i, COLOR_BLUE);
      drawPixel(j + x_pos - 2, y_pos + 21 + i, COLOR_BLUE);
    }
  }
}

void
main()
{
  configureClocks();
  lcd_init();
  buzzer_init();
  p2sw_init(15);
  or_sr(0x8);
  u_char j;
  u_char i;
  clearScreen(COLOR_BLUE);

  //  For switches, sleep, and song 
  while(1) {
    //	Switches
    u_int switches = p2sw_read(), k;
    for(k = 0; k < 4; k++) {
      if(!(switches & (1<<k))) {
	color = COLOR_GOLDENROD;
	and_sr(0xffef);
	if(k == 0) {
	  x_pos-=2;
	}
	if(k == 1) {
	  y_pos-=2;
	}
	if(k == 2) {
	  y_pos+=2;
	}
	if(k == 3) {
	  x_pos+=2;
	}
	count = 0;
      }
    }
    // Sleep
    if(count == 250) {
      color = COLOR_BLUE;
      perPos(x_pos,y_pos,color);
      or_sr(0x10);
    }
    count += 1;
    perPos(x_pos, y_pos, color);
    // Song
    timer++;
    if(timer == tempo[l]) {
      if(l%2 == 1) {
	buzzer_set_period(as[l/2 + 1]);
      }
      else {
	buzzer_set_period(0);
      }
      l++;
      if(l == 108) {
	// Goes back to the middle of the song to make it loop
	l = 57;
      }
      timer = 0;
    }
  }
}

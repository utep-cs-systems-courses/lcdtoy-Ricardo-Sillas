#include <libTimer.h>
#include <p2switches.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

// Screen size: 128x160

// Starting position of box
u_char x_pos = 45;
u_char y_pos = 70;

int redrawScreen = 1;

// For sound
int tempo[] = {7,2,7,2,7,2,7,2,14,2,7,2,7,2,14,2,14,2,14,2,7,2,7,2,14,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,14,2,14,2,14,2,7,2,7,2,14,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2,14,2,7,2,7,2,7,2,7,2,7,2,7,2};
int as[] = {2025,2273,2408,3034,2703,2703,1804,2025,2273,2408,2408,2408,2025,2273,2408,2703,2703,1136,1204,1136,1204,1136,2703,2703,1136,1204,1136,1204,1136,2703,2703,1804,2025,2273,2408,2408,2408,2025,2273,2408,2703,2703,1136,1204,1136,1204,1136,2703,2703,1136,1204,1136,1204,1136};

// Creates object
void perPos(u_char x_pos, u_char y_pos) {
  u_char i;
  u_char j;
  for(i = 0; i <= 10; i++) {
    for(j = 0; j <= 10 + i; j++) {
      drawPixel(20-j+x_pos, i+y_pos, COLOR_GOLDENROD);
      drawPixel(20+j+x_pos, i+y_pos, COLOR_GOLDENROD);
    }
  }
  for(i = 0; i <= 44; i++) {
    for(j = 0; j <= 2; j++) {
      drawPixel(x_pos+i-2, y_pos+11+j, COLOR_BLUE);
    }
  }
  for(i = 0; i <= 24; i++) {
    for(j = 0; j <= 2; j++) {
      drawPixel(x_pos+8+i, y_pos-3+j, COLOR_BLUE);
    } 
  }
  for(i = 0; i <= 14; i++) {
    for(j = 0; j <= 2; j++) {
      drawPixel(x_pos+9-i, y_pos-2+j+i, COLOR_BLUE);
      drawPixel(x_pos+30+i, y_pos+i+j-2, COLOR_BLUE);
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
  enableWDTInterrupts();
  or_sr(0x8);
  clearScreen(COLOR_BLUE);
  drawString5x7(50, 20, "Move!", COLOR_BLACK, COLOR_BLUE);  // Writing string "Move!"

  int timer = 0;
  int note = 0;

  for(;;) {
    while(!redrawScreen) {
      buzzer_set_period(0);  // Stop song
      or_sr(0x10);  // Sleep
    }
    perPos(x_pos, y_pos);
    // Song
    timer++;
    if(timer == tempo[note]) {
      if(note%2 == 1) {
	buzzer_set_period(as[note/2 + 1]);
      }
      else {
	buzzer_set_period(0);
      }
      l++;
      if(note == 108) {
	// Goes back to the middle of the song to make it loop
	note = 57;
      }
      timer = 0;
    }
  }
}

void wdt_c_handler() {
  static short counter = 0;
  static short count = 0;
  count ++;
  if(count == 15) {
    redrawScreen = 1;
    u_int switches = p2sw_read(), k;
    for(k = 0; k < 4; k++) {          // to wake up
      if(!(switches & (1<<k))) {
        if(k == 0) {             // button one
          redrawScreen = 1;
	  counter = 0;
	  x_pos -= 2;
	}
	if(k == 1) {            // button two
          redrawScreen = 1;
	  counter = 0;
	  y_pos -= 2;
	}
	if(k == 2) {            // button three
          redrawScreen = 1;
	  counter = 0;
	  y_pos += 2;
	}
	if(k == 3) {            // button four
          redrawScreen = 1;
	  counter = 0;
	  x_pos += 2;
	}
      }
    }
    if(counter > 250) {         // timer for about 10 seconds to turn off
      clearScreen(COLOR_BLUE);
      redrawScreen = 0;
    }
    counter ++;
    count = 0;
  }
}
	  

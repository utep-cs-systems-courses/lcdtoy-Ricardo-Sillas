/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;
  u_char j;
  clearScreen(COLOR_BLUE);

  for(j = 0; j < 15; j++) {
    drawPixel(0, j, COLOR_BLACK);
    drawPixel(j, 15, COLOR_BLACK);
    drawPixel(j, j, COLOR_BLACK);
  }
  //  for(j = 30; j < 60; j++) {
  //    drawPixel(j, j, COLOR_BLACK);
  //    drawPixel(j, 30, COLOR_BLACK);
  //  }
}

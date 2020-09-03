#include <GxEPD.h>
#include <GxGDEW027W3/GxGDEW027W3.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=D0*/ 16, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/2, /*BUSY=D6*/12); // default selection of (9), 7

void setup()
{
  display.init();
  display.eraseDisplay();
  display.setRotation(3);
}

void loop()
{

  display.drawPaged(showWeight);
  delay(10000);
  display.drawPaged(showInfo);
}


void showWeight()
{
  const GFXfont* f = &FreeSans24pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(50, 100);
  //display.println();
  display.println("69 kg");
}
void showInfo()
{
  const GFXfont* f = &FreeSans18pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(50, 135);
  //display.println();
  display.println("Updated!");

}
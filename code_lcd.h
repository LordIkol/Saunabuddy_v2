#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET LED_BUILTIN
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display_s(SCREEN_WIDTH, 32, &Wire, OLED_RESET);
int deg = 0xF7;

void lcd_showTemp(int sensor) {
  tcaselect(0);
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (sensor == 0) {
    display.println( F("Temp 1:") );
    display.println(String(temperature_outside) + (char)247);
    display.display();
  } else {
    display.println( F("Temp 2:") );
    display.println(String(temperature_inside) + (char)247);
    display.display();
  }
}

void lcd_showHumidity() {
  tcaselect(0);
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println( F("Humidity:") );
  display.println(String(humidity) + "%" );
  display.display();
}

void lcd_showSensorInfo(int sensor, bool showhumidity) {
  //display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  Tempinfo ti = Sensorinfo.get(sensor);
  //if (showhumidity) {
    tcaselect(1);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println( "Humidity " + String(sensor) );
    display.println(String(ti.humidity) + "%" );
    display.setRotation(0);
    display.display();
  //} else {
    tcaselect(0);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println( "Temp " + String(sensor) );
    display.println(String(ti.temperature) + (char)247 );
    display.setRotation(2);
    display.display();
  //}
}

void lcd_showSensorInfo_Big(int sensor) {
    //display.clearDisplay();
    String Sensorname = "Default";
    if (sensor == 0) {
        Sensorname = "One";
    }
    else if (sensor == 1) {
        Sensorname = "Two";
    }
    else if (sensor == 2) {
        Sensorname = "Three";
    }
    else if (sensor == 3) {
        Sensorname = "Four";
    }
    else {
        Sensorname = "Default";
    }

    int offset = (10 - Sensorname.length()) / 2 * 13;
    display.clearDisplay();
    display.setFont();
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    Tempinfo ti = Sensorinfo.get(sensor);
    tcaselect(0);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Temp   Hum");
    display.setTextSize(3); // Draw 2X-scale text
    display.setCursor(0, 22);
    display.print((int)(ti.temperature + 0.5f));
    display.drawChar(display.getCursorX(),display.getCursorY(),deg,1,0,2);
    display.print("  ");
    display.print((int)(ti.humidity + 0.5f));
    display.print("%");
    display.setTextSize(2); // Draw 2X-scale text
    display.setCursor(offset, 50);
    display.println(Sensorname);
    display.display();
    //}
}

void initialize_lcd() {
  tcaselect(0);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.display();

  tcaselect(1);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.setRotation(0);
  display.display();
}

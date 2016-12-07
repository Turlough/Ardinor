#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUMLEDS 45

#define BRIGHTNESS 50

#define WAIT 2000

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_RGB + NEO_KHZ800);


uint32_t foreground = strip.Color(200,200,200);
uint32_t background = strip.Color(100,100,20);
uint32_t off = strip.Color(0,0,0);

Adafruit_NeoPixel strips[] = {
  
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_RBG + NEO_KHZ800),
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GBR + NEO_KHZ800),
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_BRG + NEO_KHZ800),
  Adafruit_NeoPixel(NUMLEDS, PIN, NEO_BGR + NEO_KHZ800)
  
};

void setup() {
  Serial.begin(115200);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setBackground(off);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  setBackground(off);
  
  for(uint16_t i=0; i<6; i++) {
    
    strip = strips[i];
    strip.begin();

    setBackground(strip.Color(255,255,255));
    strip.show();
    delay(500);
    
    setBackground(strip.Color(255,0,0));
    strip.show();
    delay(500); 
    
    setBackground(strip.Color(0,255,0));
    strip.show();
    delay(500); 

    setBackground(strip.Color(0,0,255));
    strip.show();
    delay(500); 

    setBackground(strip.Color(0,0,0));
    strip.show();
    delay(500); 
    
  }
  delay(WAIT);

}

void setBackground(uint32_t color){
    for(uint16_t i=0; i<strip.numPixels(); i++) { 
      strip.setPixelColor(i, color);
    }
}




#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <NewPing.h>
#include <math.h>

//LED vars
#define LED_CONTROL_PIN 10 //blue
#define NUMLEDS 60
#define WAIT 100                                                                                                                                                                                                                                                      
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, LED_CONTROL_PIN, NEO_GRB + NEO_KHZ800);


uint32_t off = strip.Color(0,0,0);
uint32_t foreground = strip.Color(150,255,30);


void setup() {
  Serial.begin(115200);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setBackground(off);
  strip.show(); // Initialize all pixels to 'off'

  
}

  uint8_t maxcolor[] {150, 255, 30};
  uint8_t colors[3];
  uint32_t color;
  int t = 0;
void loop() {
    
    setBackground(off);
    t %= NUMLEDS;
    t++;
    setLed(t, foreground);
    delay(WAIT);

    for (int i =0; i< NUMLEDS; i++){
      
      int phase = t + i;
      phase %= 6;
      double amplitude = cos(phase * 5)/PI;
      amplitude++;
      amplitude /= 2;
      
      for(int c = 0; c <3; c++){
        colors[c] = maxcolor[c] * amplitude/2;
        color = strip.Color(colors[0],colors[1],colors[2]);
        setLed(i, color);
      }

      delay(WAIT);
      
    }
    
    strip.show();
    

}


void setBackground(uint32_t color){
    for(uint16_t i=0; i < NUMLEDS; i++) { 
      strip.setPixelColor(i, color);
    }
}

void setLed(uint16_t pos, uint32_t color){
  pos %= NUMLEDS;
  strip.setPixelColor(pos, color);
}



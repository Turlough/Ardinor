#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUMLEDS 144
#define WAIT 50  
#define BRIGHTNESS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_RGB + NEO_KHZ800);


uint32_t off = strip.Color(0,0,0);
//uint32_t yellow = strip.Color(50,50,30);
//uint32_t blue = strip.Color(10,75,100);
uint8_t blue []  {30, 75, 255};
uint8_t yellow []  {200, 255, 30};
//uint8_t orange []  {100, 70, 20};
uint32_t accumulator[NUMLEDS]{};

void setup() {
  Serial.begin(115200);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setBackground(off);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    
  for(uint16_t i=0; i<NUMLEDS; i++) {
    
    setBackground(off);
    //setBlur(i*4, 15, blue);
    setBlur(i +  0, 7, yellow);
    setBlur(i + 15, 7, yellow);  
    setBlur(i + 30, 7, yellow);   
    setBlur(i + 45, 7, yellow);
    setBlur(i*5, 3, blue);
    
    strip.show();
    delay(WAIT);
    
  }

}

uint8_t* toArray(uint32_t v){
  
  static uint8_t a[4];

  a[0] = (uint8_t) (v);
  a[1] = (uint8_t) (v >>  8);
  a[2] = (uint8_t) (v >> 16);
  a[3] = (uint8_t) (v >> 24);
  return a;
}

uint32_t fromArray(uint8_t* b){

  uint32_t u;
  u = b[0];
  u = (u  << 8) + b[1];
  u = (u  << 8) + b[2];
  u = (u  << 8) + b[3];
  return u;
  

}

//void setBlur(uint16_t center, uint16_t radius, uint32_t color){
//  setBlur(center, radius, toArray(color));
//  
//}

void setBlur(uint16_t center, uint16_t radius, uint8_t grb[]){
  
  uint8_t colors[3];
  uint32_t color;
  
  for(int i=0;i<radius;i++){
    
    float r = (float) (radius);
    float intensity = (1 - ( i / r));
    
    colors[0] = grb[0] * intensity;
    colors[1] = grb[1] * intensity;
    colors[2] = grb[2] * intensity;
    color = strip.Color(colors[0],colors[1],colors[2]);

    uint16_t c = center + radius;
    setLed(c + i, color);
    setLed(c - i, color);
  }
}

void setBackground(uint32_t color){
    for(uint16_t i=0; i<strip.numPixels(); i++) { 
//      accumulator[i] = color;
      strip.setPixelColor(i, color);
    }
}

void setLed(uint16_t pos, uint32_t color){
//  accumulator[pos] = color;
  pos %= NUMLEDS;
  strip.setPixelColor(pos, color);
}



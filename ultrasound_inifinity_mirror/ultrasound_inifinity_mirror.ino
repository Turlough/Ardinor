#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <NewPing.h>

//LED vars
#define LED_CONTROL_PIN 10 //blue
#define NUMLEDS 60
#define WAIT 10                                                                                                                                                                                                                                                      
#define BRIGHTNESS 100

//ultrasound pins
#define TRIGGER_PIN  9  //orange; Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     8  //brown:  Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 180 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, LED_CONTROL_PIN, NEO_RGB + NEO_KHZ800);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

uint32_t off = strip.Color(0,0,0);
 unsigned int numLeds = 0;

void setup() {
  Serial.begin(115200);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setBackground(off);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    

    unsigned int uS = sonar.ping(); 
    float multiplier = NUMLEDS / (float) MAX_DISTANCE;
    unsigned int cm = uS / US_ROUNDTRIP_CM;
    if(cm > 0){                                                                                                                                                                                                                                                 
      cm = MAX_DISTANCE - cm;
      unsigned int target = multiplier * cm;
      if(numLeds < target) numLeds ++;
      if(numLeds > target) numLeds --;
    }
    else {
      if(numLeds > 0) numLeds--;
    }
     
    

    uint8_t yellow []  {200, 255, 30};
    
    Serial.print(cm);
    Serial.print('\t');
    Serial.println(numLeds);
    
    setBackground(off);
    setBlur(30, numLeds/2, yellow);
    
    strip.show();
    delay(WAIT);

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

    setLed(center + i, color);
    setLed(center - i, color);
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



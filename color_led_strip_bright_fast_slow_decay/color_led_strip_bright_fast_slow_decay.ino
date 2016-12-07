#include <NewPing.h>

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUMLEDS 144
#define CENTRE NUMLEDS/2
#define WAIT 50
#define BRIGHTNESS 85
#define TIMEOUT 500

//ultrasound pins
#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin present the ultrasonic sensor.
#define ECHO_PIN     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

uint32_t off = strip.Color(0, 0, 0);
uint32_t orange = strip.Color(25, 8, 0);
uint8_t yellow []  {255, 150, 0};

bool isRampingUp = false;
unsigned int brightness = 0;
unsigned int radius = 0;
unsigned long lastDetected = 0;

void setup() {
  Serial.begin(115200);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setBackground(off);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  setBackground( off );


  if (detected()) { //block the loop while fading in

    fadein();

  }

  fade();//likewise
  delay(100);
}

void log(String s) {
  Serial.print(s + "\n");
}

bool detected() {
  
  //debounce. The unit sometimes hallucinates
  int detections = 0;
  
  for (int i = 0; i < 5; i++) {
    int distance = sonar.ping();
    bool detected = distance > 3;
    
    if (detected) {
      Serial.print("detected at ");
      Serial.print(distance);
      Serial.print(" cm\n");
      lastDetected = millis();
      detections ++;
      delay(50);
    }
  }

  return detections >3;
}

boolean timerExpired() {
  bool result = millis() - lastDetected > TIMEOUT;
  if (result) {
    log("timer expired");
  }
  return result;
}

void fadein() {
  log("fade in");
  isRampingUp = true;
  for (; brightness < BRIGHTNESS; brightness++) {
    setBackground(orange);
    strip.setBrightness(brightness);
    strip.show();
    delay(5);
  }
  for (; radius < NUMLEDS; radius++) {
    setBackground(orange);
    setBlur(CENTRE, radius / 2, yellow);
    strip.show();
    delay(25);
  }
  delay(5000);
  isRampingUp = false;
}


void fade() {

  log("fadeout");

  for (; brightness > 0; brightness--) {

    for (; radius > 0; radius--) {

      if (detected()) {
        fadein();
        return;
      }

      setBackground(orange);
      setBlur(CENTRE, radius / 2, yellow);
      strip.show();
      delay(200);

    }

    if (detected()) {
      fadein();
      return;
    }

    setBackground(orange);
    strip.setBrightness(brightness);
    strip.show();
    delay(100);

  }

}

void setBlur(uint16_t center, uint16_t radius, uint8_t grb[]) {

  uint8_t colors[3];
  uint32_t color;

  for (int i = 0; i < radius; i++) {

    float r = (float) (radius);
    float intensity = (1 - ( i / r));

    colors[0] = grb[0] * intensity;
    colors[1] = grb[1] * intensity;
    colors[2] = grb[2] * intensity;
    color = strip.Color(colors[0], colors[1], colors[2]);

    setLed(center + i, color);
    setLed(center - i, color);
  }
}

void setBackground(uint32_t color) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}

void setLed(int pos, uint32_t color) {
  pos %= NUMLEDS;
  strip.setPixelColor(pos, color);
}

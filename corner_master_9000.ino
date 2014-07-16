//corner master 9000
//Nicholas Mykulowycz
//Created jul 15, 2014

#include <Adafruit_NeoPixel.h>

#define DISPLAY_PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();  //turn pixels off
}

void loop() {
  //set neopixel parameters
  //strip.setPixelColor(8, strip.Color(100,0,0));
  
  //send data to neopixels
  //strip.show();
  
  centerWipeFade(strip.Color(100, 0, 0), 22);
  delay(1500);
  //clearDisplay();
  //delay(1500);
}

//fading wipe neopixel function, plays when microcontroller first starts
void centerWipeFade(uint32_t c, uint8_t wait){
  //
  int red = 127;
  int fade = 30;
  int tmp_red = red;
  int fade_length = red/fade;
  
  //primary fade loop
  for(int i=0; i<(strip.numPixels()+fade_length+1); i++) {
    for(int j=i; j>=0; j--) {
      tmp_red = red-((i-j)*fade);
      if(tmp_red < 0){
        tmp_red = 0;
      }
      //only write to pixel if it physically exists
      if(j<strip.numPixels()){
        strip.setPixelColor(j, strip.Color(tmp_red, 0, 0));
      }
    }
    strip.show();
    delay(wait);
  } 
  
  
  
}

//clears all LED states
void clearDisplay() {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
}

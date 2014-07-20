//corner master 9000
//Nicholas Mykulowycz
//Created jul 15, 2014

#include <Adafruit_NeoPixel.h>

#define DISPLAY_PIN 6
#define HIGH_INTENSITY 127
#define MAX_FORCE 1.5

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);

int offset = strip.numPixels()/2;

void setup() {
  //intitialize strip
  strip.begin();
  //send all off to strip
  strip.show();
  
  //start of fade sequence
  centerWipeFade(HIGH_INTENSITY, 30);
  
  int intensity = 0;
  int fade = 6;
  //fade in zero force indicator
  while(intensity < HIGH_INTENSITY) {
    intensity = intensity + fade;
    strip.setPixelColor(7, strip.Color(intensity, 0, 0));
    strip.setPixelColor(8, strip.Color(intensity, 0, 0));
    delay(50);
    strip.show();
  }
    
    
  
  
  //Serial.begin(9600);
}

void loop() {
  //set neopixel parameters
  //strip.setPixelColor(8, strip.Color(100,0,0));
  
  //send data to neopixels
  //strip.show();
  
  //centerWipeFade(strip.Color(100, 0, 0), 30);
  //delay(1500);
  //clearDisplay();
  //delay(1500);
  
}

//fading wipe neopixel function, plays when microcontroller first starts
void centerWipeFade(uint8_t red, uint8_t wait){
  //
  
  int red_fade = 13;
  int tmp_red = red;
  int fade_length = red/red_fade;

  int loop_cycles = offset+fade_length+2;
  int addr = 0;
  
  
  //primary fade loop
  for(int i=0; i<loop_cycles; i++) {
    
    //positive half
    for(int j=0; j<=i; j++) {
      tmp_red = red-(j*red_fade);
      //prevent a "negative" intensity from being written
      if(tmp_red < 0){
        tmp_red = 0;
      }
      //calculate pixel address
      addr = i + offset - j -1;
      //only write to pixel if it physically exists on positive half
      if(addr>=offset && addr<=15){
        strip.setPixelColor(addr, strip.Color(tmp_red, 0, 0));
        //strip.setPixelColor(j-8, strip.Color(tmp_red, 0, 0));
      }
    }
    
    //negative half
    for(int k=0; k<=i; k++) {
      tmp_red = red-(k*red_fade);
      //prevent a "negative" intensity from being written
      if(tmp_red < 0){
        tmp_red = 0;
      }
      //calculate pixel address
      addr = offset - i + k;
      //only write to the negative pixels that actually exist
      if(addr>=0 && addr<=offset) {
        strip.setPixelColor(addr, strip.Color(tmp_red, 0, 0));
      }
     
    }
    
    strip.show();
    delay(wait);
  } 
  
  
  
}

//write to neopixel display
void setForceDisplay(float force_value) {
  //figure out value corresponding to a single pixel
  float segment = MAX_FORCE/offset;
  
  
}

//clears all LED states
void clearDisplay() {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
}

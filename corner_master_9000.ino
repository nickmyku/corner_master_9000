//corner master 9000
//Nicholas Mykulowycz
//Created jul 15, 2014

#include <Wire.h>                 //i2c library
#include <Adafruit_Sensor.h>      //generic sensor library
#include <Adafruit_NeoPixel.h>    //for display
#include <Adafruit_ADXL345_U.h>   //for acceleraometer

#define DISPLAY_PIN 6
#define HIGH_INTENSITY 127
#define MAX_FORCE 1.5

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);

int offset = strip.numPixels()/2;
float avg_val = 0;

void setup() {
  //intitialize strip
  strip.begin();
  //send all off to strip
  strip.show();
  
   /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  
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
  
  //Get a new sensor event 
  sensors_event_t event; 
  accel.getEvent(&event);
  
  /* Get a new acceleration */ 
  //avg_val = getAverageForce(3, 'x');
  avg_val = event.acceleration.x;
  
  //set the display to corresponding x force value
  setForceDisplay(avg_val);
  
  delay(75);
  
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
  float segment_value = MAX_FORCE/offset;
  float g_force_value = force_value/9.8;
  float tmp_force_value= 0;
  int pixels = 0;
  int full_red = HIGH_INTENSITY;
  int tmp_red = 0;
  int addr = 0;
  
  //calculate number of pixels to illuminate
  pixels = abs(g_force_value/segment_value);
  
  //clear display but dont refresh
  clearDisplay(false); 
  
  //if force value is fairly small then just illuminate center pixels
  if(g_force_value < (segment_value) && g_force_value > (-1*segment_value)){   
    //calculate intensity
    tmp_red = full_red/4;
    strip.setPixelColor(7, strip.Color(tmp_red, 0, 0));
    strip.setPixelColor(8, strip.Color(tmp_red, 0, 0));
  }   
  
  //if force value is positive illuminate positive half of display
  else if(g_force_value > 0){
    //cycle through entire display
    for(int i=0; i<offset; i++){
      //calculate pixel address
      addr = i+offset;
      //if pixel value is greater then loop index then pixel should be illuminated
      if(pixels > i){
        strip.setPixelColor(addr, strip.Color(full_red, 0, 0));
      }
      //if pixel is equal to index then set to partial brightness
      /*
      else if(pixels == i){
        //calculate intermediate force
        tmp_force_value = force_value-(i*segment_value);
        //scale partial brigness to ratio of intermediate force over value of single segment
        tmp_red = full_red*(tmp_force_value/segment_value);
        //send data to pixel
        strip.setPixelColor(addr, strip.Color(tmp_red, 0, 0));
      }
      */
      //otherwise turn pixel off
      else{
        strip.setPixelColor(addr, strip.Color(0, 0, 0));
      }
    }
    
  }
  
  //if force value is negative illuminate negative half of display
  else if(g_force_value < 0){
    //cycle through entire display
    for(int i=0; i<offset; i++){
      //calculate pixel address
      addr = offset-i-1;
      //if pixel value is greater then loop index then pixel should be illuminated
      if(pixels > i){
        strip.setPixelColor(addr, strip.Color(full_red, 0, 0));
      }
      //otherwise turn pixel off
      else{
        strip.setPixelColor(addr, strip.Color(0, 0, 0));
      }
    }
    
  }
  
  //send data to display
  strip.show();
  
  
}

float getAverageForce(int samples, char axis){
  double total_value = 0;
  float avg_value = 0;
  
  for(int i=0; i<samples; i++){
    /* Get a new sensor event */ 
    sensors_event_t event; 
    accel.getEvent(&event);
    //assing value to array
    total_value += event.acceleration.x;
  }
  
  //calculate average
  avg_value = total_value/samples;
  
  return avg_value;  
}

//clears all LED states
void clearDisplay(boolean refresh) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  if(refresh){
    strip.show();
  }
}

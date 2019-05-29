/* soundmems_demo
By: Andrew Tuline
Date: December 2017
Updated: September 2018
https://github.com/atuline/FastLED-Demos/tree/master/soundmems_demo
*/
// Use qsuba for smooth pixel filtering and qsubd for digital pixel filtering.
#define qsubd(x, b)  ((x>b)?b:0)                                                // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                                              // Analog Unsigned subtraction macro. if result <0, then => 0. By Andrew Tuline.
uint8_t peakAvg;
bool thisdir = 0; 
TBlendType    currentBlending = LINEARBLEND;                                    // NOBLEND or LINEARBLEND 

void lineit() {                                                                 // Send the pixels one or the other direction down the line.
  
  if (thisdir == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS-1 ; i++ ) leds[i] = leds[i+1];
  }
  
} // waveit()

void addGlitter2( fract8 chanceOfGlitter) {                                      // Let's add some glitter, thanks to Mark
  
  if( random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }

} // addGlitter2()

void waveit() {                                                                 // Shifting pixels from the center to the left and right.
  
  for (int i = NUM_LEDS - 1; i > NUM_LEDS/2; i--) {                             // Move to the right.
    leds[i] = leds[i - 1];
  }

  for (int i = 0; i < NUM_LEDS/2; i++) {                                        // Move to the left.
    leds[i] = leds[i + 1];
  }
  
} // waveit()



void besin() {                                                             // Add a Perlin noise soundbar. This looks really cool.

// Local definitions


// Persistent local variables


  timeval = 30;                                                                 // Our EVERY_N_MILLIS_I timer value.


// This works.
  leds[NUM_LEDS/2] = ColorFromPalette(currentPalette, millis(), spectrumAvg, NOBLEND);
  leds[NUM_LEDS/2-1] = ColorFromPalette(currentPalette, millis(), spectrumAvg, NOBLEND);


//  leds[NUM_LEDS/2] = ColorFromPalette(currentPalette, spectrumAvg, beatsin8(spectrumAvg,0,255), NOBLEND);
//  leds[NUM_LEDS/2-1] = ColorFromPalette(currentPalette, spectrumAvg, beatsin8(spectrumAvg,0,255), NOBLEND);


  waveit();                                                                     // Move the pixels to the left/right, but not too fast.

//  fadeToBlackBy(leds+NUM_LEDS/2-1, 2, 128);                                     // Fade the center, while waveit moves everything out to the edges.
  fadeToBlackBy(leds, NUM_LEDS, 2);                                                                                 
 Serial.println(spectrumAvg);
} // besin()

void fillnoise() {                                                             // Add a Perlin noise soundbar. This looks really cool.

// Local definitions
  #define xscale 160
  #define yscale 160

// Persistent local variables
  static int16_t xdist;                                                         // A random number for our noise generator.
  static int16_t ydist;


  timeval = 40;                                                                 // Our EVERY_N_MILLIS_I timer value.

  if (spectrumAvg > NUM_LEDS) spectrumAvg = NUM_LEDS;                               // Clip the spectrumAvg to maximize at NUM_LEDS.
  
  for (int i= (NUM_LEDS-spectrumAvg/2)/2; i<(NUM_LEDS+spectrumAvg/2)/2; i++) {      // The louder the sound, the wider the soundbar.
    uint8_t index = inoise8(i*spectrumAvg+xdist, ydist+i*spectrumAvg);              // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, spectrumAvg, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                             // Effect is a NOISE bar the width of spectrumAvg. Very fun. By Andrew Tuline.

  xdist=xdist+beatsin8(5,0,3);                                                  // Moving forward in the NOISE field, but with a sine motion.
  ydist=ydist+beatsin8(4,0,3);                                                  // Moving sideways in the NOISE field, but with a sine motion.

  addGlitter2(spectrumAvg/2);                                                      // Add glitter based on sample and not peaks.

  waveit();                                                                     // Move the pixels to the left/right, but not too fast.
  
  fadeToBlackBy(leds+NUM_LEDS/2-1, 2, 128);                                     // Fade the center, while waveit moves everything out to the edges.
     Serial.println(spectrumAvg);                                                                          
} // fillnoise()

void noisefire() {                                                              // Create fire based on noise and spectrumAvg. 

// Local definitions
  #define xscale 20                                                             // How far apart they are
  #define yscale 3                                                              // How fast they move

// Temporary local variable
  uint8_t index = 0;                                                            // Current colour lookup value.

  timeval = 0;                                                                  // Our EVERY_N_MILLIS_I timer value.

  currentPalette = CRGBPalette16(CHSV(0,255,2), CHSV(0,255,4), CHSV(0,255,8), CHSV(0, 255, 8),    // Fire palette definition. Lower value = darker.
                                 CHSV(0, 255, 16), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                 CRGB::DarkOrange,CRGB::DarkOrange, CRGB::Orange, CRGB::Orange,
                                 CRGB::Yellow, CRGB::Orange, CRGB::Yellow, CRGB::Yellow);
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                     // X location is constant, but we move along the Y at the rate of millis(). By Andrew Tuline.

    index = (255 - i*256/NUM_LEDS) * index/128;                                 // Now we need to scale index so that it gets blacker as we get close to one of the ends
                                                                                // This is a simple y=mx+b equation that's been scaled. index/128 is another scaling.
    leds[i] = ColorFromPalette(currentPalette, index, spectrumAvg, NOBLEND);      // With that value, look up the 8 bit colour palette value and assign it to the current LED. 
  }                                                                             // The higher the value of i => the higher up the palette index (see palette definition).
  Serial.println(spectrumAvg);                                                                                                    
} // noisefire()

void noisefiretest() {                                                          // Create fire based on noise and spectrumAvg. 

// Local definitions
  #define xscale 20                                                             // How far apart they are
  #define yscale 3                                                              // How fast they move

// Temporary local variable
  uint16_t index = 0;                                                            // Current colour lookup value.

  timeval = 10;                                                                  // Our EVERY_N_MILLIS_I timer value.

  currentPalette = CRGBPalette16(CHSV(0,255,2), CHSV(0,255,4), CHSV(0,255,8), CHSV(0, 255, 8),    // Fire palette definition. Lower value = darker.
                                 CHSV(0, 255, 16), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                 CRGB::DarkOrange,CRGB::DarkOrange, CRGB::Orange, CRGB::Orange,
                                 CRGB::Yellow, CRGB::Orange, CRGB::Yellow, CRGB::Yellow);
  
  for(int i = 0; i < NUM_LEDS; i++) {

//    Serial.print(i);
//    Serial.print("  ");
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                     // X location is constant, but we move along the Y at the rate of millis(). By Andrew Tuline.
//    Serial.print(index);
//    Serial.print("  ");

//    index = (255 - *i*128/NUM_LEDS);                                            // Now we need to scale index so that it gets blacker as we get close to one of the ends

    index = (255 - i*256/NUM_LEDS) * index / 128;                                 // Now we need to scale index so that it gets blacker as we get close to one of the ends
    
    
//    Serial.print(index);
//    Serial.println(" ");
                                                                                
                                                                                
    leds[NUM_LEDS/2-i/2+1] = ColorFromPalette(currentPalette, index, spectrumAvg, NOBLEND);      // With that value, look up the 8 bit colour palette value and assign it to the current LED. 
    leds[NUM_LEDS/2+i/2-1] = ColorFromPalette(currentPalette, index, spectrumAvg, NOBLEND);      // With that value, look up the 8 bit colour palette value and assign it to the current LED. 
    
  }                                                                             // The higher the value of i => the higher up the palette index (see palette definition).
 Serial.println(spectrumAvg);

} // noisefiretest()


void jugglep() {                                                                // Use the juggle routine, but adjust the timebase based on spectrumAvg for some randomness.

// Persistent local variables
  static uint8_t curhue;                                                        // Let's rotate the hue.
  static int thistime = 20;                                                     // Time shifted value keeps changing thus interrupting the juggle pattern.

  int timeval = 20;                                                                 // Our EVERY_N_MILLIS_I timer value.

  curhue = curhue + 4;                                                          // Change the hue pretty quickly, so that the juggle changes colours on the fly.
  
  fadeToBlackBy(leds, NUM_LEDS, 12);                                            // Fade the strand.
  
  leds[beatsin16(thistime,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, curhue, spectrumAvg, currentBlending);
  leds[beatsin16(thistime-3,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, curhue, spectrumAvg, currentBlending);
  

  EVERY_N_MILLISECONDS(250) {
    thistime = spectrumAvg/2;                                                     // Change the beat frequency every 250 ms. By Andrew Tuline.
  }

  addGlitter2(spectrumAvg/2);                                                      // Add glitter based on spectrumAvg.
  Serial.println(spectrumAvg);
}


void plasma() {

// Persistent local variables
  static int16_t thisphase = 0;                                                 // Phase of a cubicwave8.
  static int16_t thatphase = 0;                                                 // Phase of the cos8.

// Temporary local variables
  uint16_t thisbright;
  uint16_t colorIndex;

  timeval = 20;                                                                 // Our EVERY_N_MILLIS_I timer value.
  
  thisphase += beatsin8(6,-4,4);                                                // You can change direction and speed individually.
  thatphase += beatsin8(7,-4,4);                                                // Two phase values to make a complex pattern. By Andrew Tuline.

  for (int k=0; k<NUM_LEDS; k++) {                                              // For each of the LED's in the strand, set a brightness based on a wave as follows.
    thisbright = cubicwave8((k*8)+thisphase)/2;    
    thisbright += cos8((k*10)+thatphase)/2;                                     // Let's munge the brightness a bit and animate it all with the phases.
    colorIndex=thisbright;
    thisbright = qsuba(thisbright, 255-spectrumAvg);                              // qsuba chops off values below a threshold defined by spectrumAvg. Gives a cool effect.
    
    leds[k] = ColorFromPalette( currentPalette, colorIndex, thisbright, currentBlending);   // Let's now add the foreground colour.
  }

  addGlitter2(spectrumAvg/2);                                                      // Add glitter based on spectrumAvg.
   
} // plasma()

void rainbowbit() {

  timeval = 10;                                                                 // Our EVERY_N_MILLIS_I timer value.
  
  uint8_t beatA = beatsin8(17, 0, 255);                                         // Starting hue.
  
  if (samplepeak == 1) {                                                        // Trigger a rainbow with a peak.
    fill_rainbow(leds + random8(0,NUM_LEDS/2), random8(0,NUM_LEDS/2), beatA, 8);  // Use FastLED's fill_rainbow routine.
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 40);                                            // Fade everything. By Andrew Tuline.

  addGlitter2(spectrumAvg);                                                        // Add glitter based on spectrumAvg.

} // rainbowbit()


void rainbowg() {                                                               // Make a complex rainbow with glitter based on spectrumAvg.
  
  uint8_t beatA = beatsin8(17, 0, 255);                                         // Starting hue.
  uint8_t beatB = beatsin8(13, 0, 255);
  uint8_t beatC = beatsin8(11, 0, 255);

//  currentPalette = PartyColors_p;
  
  for (int i=0; i < NUM_LEDS; i++) {
    int colorIndex = (beatA+beatB+beatC)/3 * i * 4 / NUM_LEDS;
    leds[i] = ColorFromPalette( currentPalette, colorIndex, spectrumAvg, currentBlending); // Variable brightness 
  }

  addGlitter2(spectrumAvg);                                                        // Add glitter baesd on spectrumAvg. By Andrew Tuline.

} // rainbowg()


void ripple() {                                                                 // Display ripples triggered by peaks.

// Local definitions
  #define maxsteps 16                                                           // Maximum number of steps.

// Persistent local variables.
  static uint8_t colour;                                                        // Ripple colour is based on samples.
  static uint16_t center = 0;                                                   // Center of current ripple.
  static int8_t step = -1;                                                      // Phase of the ripple as it spreads out.

  timeval = 20;                                                                 // Our EVERY_N_MILLIS_I timer value.
  
  if (samplepeak == 1) {step = -1;}                                             // Trigger a new ripple if we have a peak.
  
  fadeToBlackBy(leds, NUM_LEDS, 64);                                            // Fade the strand, where 1 = slow, 255 = fast

  switch (step) {

    case -1:                                                                    // Initialize ripple variables. By Andrew Tuline.
      center = random(NUM_LEDS);
      for(int x=0; x< 7; x++){
       peakAvg+=spectrumPeaks[x];
      }
      peakAvg=peakAvg/7;
      colour = (peakAvg) % 255;                                               // More peaks/s = higher the hue colour.
      step = 0;
      break;

    case 0:
      leds[center] += ColorFromPalette(currentPalette, colour, 255, currentBlending); // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                              // At the end of the ripples.
      // step = -1;
      break;

    default:                                                                    // Middle of the ripples.

      leds[(center + step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, 255/step*2, currentBlending);  // A spreading and fading pattern up the strand.
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, 255/step*2, currentBlending);  // A spreading and fading pattern down the strand.
      step ++;                                                                  // Next step.
      break;  
      
  } // switch step


  addGlitter2(spectrumAvg);                                                        // Add glitter baesd on spectrumAvg.
  
} // ripple()

#define VUFadeFactor 5
#define VUScaleFactor 2.0
#define VUPaletteFactor 1.5
void drawVU() {
  CRGB pixelColor;

  const float xScale = 255.0 / (NUM_LEDS / 2);
  float specCombo = (spectrumDecay[0] + spectrumDecay[1] + spectrumDecay[2] + spectrumDecay[3]) / 4.0;
  float specComboHigh = (spectrumDecay[4] + spectrumDecay[5] + spectrumDecay[6]) / 3.0;

  for (byte x = 0; x < NUM_LEDS / 2; x++) {
    int senseValue = specCombo / VUScaleFactor - xScale * x;
    int pixelBrightness = senseValue * VUFadeFactor;
    if (pixelBrightness > 255) pixelBrightness = 255;
    if (pixelBrightness < 0) pixelBrightness = 0;

    int pixelPaletteIndex = senseValue / VUPaletteFactor - 15;
    if (pixelPaletteIndex > 240) pixelPaletteIndex = 240;
    if (pixelPaletteIndex < 0) pixelPaletteIndex = 0;

    pixelColor = ColorFromPalette(palettes[currentPaletteIndex], pixelPaletteIndex, pixelBrightness);

    leds[x] = pixelColor;
    leds[NUM_LEDS - x - 1] = pixelColor;
  }

//  for (byte x = (NUM_LEDS / 2)-1 ; x > 0; x--) {
//    int senseValue = specCombo / VUScaleFactor - xScale * ( (NUM_LEDS / 2) - x);
//    int pixelBrightness = senseValue * VUFadeFactor;
//    if (pixelBrightness > 255) pixelBrightness = 255;
//    if (pixelBrightness < 0) pixelBrightness = 0;
//
//    int pixelPaletteIndex = senseValue / VUPaletteFactor - 15;
//    if (pixelPaletteIndex > 240) pixelPaletteIndex = 240;
//    if (pixelPaletteIndex < 0) pixelPaletteIndex = 0;
//
//    pixelColor = ColorFromPalette(palettes[currentPaletteIndex], pixelPaletteIndex, pixelBrightness);
//
//    leds[x] = pixelColor;
//    leds[NUM_LEDS - x - 1] = pixelColor;
//  }
}

void drawVU2() {
  uint8_t avg = map8(spectrumAvg, 0, NUM_LEDS - 1);
  
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if(i <= avg) {
      leds[i] = ColorFromPalette(palettes[currentPaletteIndex], (240 / NUM_LEDS) * i);
    }
    else {
      leds[i] = CRGB::Black;
    }
  }
}

void spectrumPaletteWaves()
{
//  fade_down(1);

  CRGB color6 = ColorFromPalette(currentPalette, spectrumByte[6], spectrumByte[6]);
  CRGB color5 = ColorFromPalette(currentPalette, spectrumByte[5] / 8, spectrumByte[5] / 8);
  CRGB color1 = ColorFromPalette(currentPalette, spectrumByte[1] / 2, spectrumByte[1] / 2);

  CRGB color = nblend(color6, color5, 256 / 8);
  color = nblend(color, color1, 256 / 2);

  leds[CENTER_LED] = color;
  leds[CENTER_LED].fadeToBlackBy(spectrumByte[3] / 12);

  leds[CENTER_LED - 1] = color;
  leds[CENTER_LED - 1].fadeToBlackBy(spectrumByte[3] / 12);

  //move to the left
  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
    leds[i] = leds[i - 1];
  }
  // move to the right
  for (int i = 0; i < CENTER_LED; i++) {
    leds[i] = leds[i + 1];
  }
}

void spectrumPaletteWaves2()
{
//  fade_down(1);

  CRGBPalette16 palette = palettes[currentPaletteIndex];

  CRGB color6 = ColorFromPalette(palette, 255 - spectrumByte[6], spectrumByte[6]);
  CRGB color5 = ColorFromPalette(palette, 255 - spectrumByte[5] / 8, spectrumByte[5] / 8);
  CRGB color1 = ColorFromPalette(palette, 255 - spectrumByte[1] / 2, spectrumByte[1] / 2);

  CRGB color = nblend(color6, color5, 256 / 8);
  color = nblend(color, color1, 256 / 2);

  leds[CENTER_LED] = color;
  leds[CENTER_LED].fadeToBlackBy(spectrumByte[3] / 12);

  leds[CENTER_LED - 1] = color;
  leds[CENTER_LED - 1].fadeToBlackBy(spectrumByte[3] / 12);

  //move to the left
  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
    leds[i] = leds[i - 1];
  }
  // move to the right
  for (int i = 0; i < CENTER_LED; i++) {
    leds[i] = leds[i + 1];
  }
}


void myvumeter() {                                                        // A vu meter. Grabbed the falling LED from Reko MeriÃ¶.

// Local definitions
  #define GRAVITY 2

// Persistent local variables
  static uint8_t topLED;
  static int gravityCounter = 0;

// Temporary local variables
  uint8_t tempsamp = constrain(spectrumAvg,0,NUM_LEDS-1);                     // Keep the sample from overflowing.


  fadeToBlackBy(leds, NUM_LEDS, 160);
    
  for (int i=0; i<tempsamp; i++) {
    uint8_t index = inoise8(i*spectrumAvg+millis(), 5000+i*spectrumAvg); 
    leds[i] = ColorFromPalette(currentPalette, index, spectrumAvg, currentBlending);
  }

  if (tempsamp >= topLED)
    topLED = tempsamp;
  else if (gravityCounter % GRAVITY == 0)
    topLED--;

  if (topLED > 0) {
    leds[topLED] = ColorFromPalette(currentPalette, millis(), 255, LINEARBLEND);   // LED falls when the volume goes down.
  }
  
  gravityCounter = (gravityCounter + 1) % GRAVITY;
    
} // myvumeter()

//
void spectrumWaves()
{
//   audioReader();
  fade_down(2);
  Serial.print("band 0:");
  Serial.println(spectrumByte[0]);
  Serial.print("band 3:");
  Serial.println(spectrumByte[3]);
  Serial.print("band 6:");
  Serial.println(spectrumByte[6]);

  CRGB color = CRGB(spectrumByte[6], spectrumByte[5] / 8, spectrumByte[1] / 2);
  
  leds[CENTER_LED] = color;
  leds[CENTER_LED].fadeToBlackBy(spectrumByte[3] / 12);

  leds[CENTER_LED - 1] = color;
  leds[CENTER_LED - 1].fadeToBlackBy(spectrumByte[3] / 12);

  //move to the left
  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
    leds[i] = leds[i - 1];
  }
  // move to the right
  for (int i = 0; i < CENTER_LED; i++) {
    leds[i] = leds[i + 1];
  }
}
//
//void spectrumWaves2()
//{
//   audioReader();
//  fade_down(2);
//
//  CRGB color = CRGB(spectrumByte[5] / 8, spectrumByte[6], spectrumByte[1] / 2);
//
//  leds[CENTER_LED] = color;
//  leds[CENTER_LED].fadeToBlackBy(spectrumByte[3] / 12);
//
//  leds[CENTER_LED - 1] = color;
//  leds[CENTER_LED - 1].fadeToBlackBy(spectrumByte[3] / 12);
//
//  //move to the left
//  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
//    leds[i] = leds[i - 1];
//  }
//  // move to the right
//  for (int i = 0; i < CENTER_LED; i++) {
//    leds[i] = leds[i + 1];
//  }
//}
//
//void spectrumWaves3()
//{
//  // audioReader();
//  fade_down(2);
//
//  CRGB color = CRGB(spectrumByte[1] / 2, spectrumByte[5] / 8, spectrumByte[6]);
//
//  leds[CENTER_LED] = color;
//  leds[CENTER_LED].fadeToBlackBy(spectrumByte[3] / 12);
//
//  leds[CENTER_LED - 1] = color;
//  leds[CENTER_LED - 1].fadeToBlackBy(spectrumByte[3] / 12);
//
//  //move to the left
//  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
//    leds[i] = leds[i - 1];
//  }
//  // move to the right
//  for (int i = 0; i < CENTER_LED; i++) {
//    leds[i] = leds[i + 1];
//  }
//}
//
//void analyzerColumns()
//{
//  
//  fill_solid(leds, NUM_LEDS, CRGB::Black);
//
//  const uint8_t columnSize = NUM_LEDS / 7;
//
//  for (uint8_t i = 0; i < 7; i++) {
//    uint8_t columnStart = i * columnSize;
//    uint8_t columnEnd = columnStart + columnSize;
//
//    if (columnEnd >= NUM_LEDS) columnEnd = NUM_LEDS - 1;
//
//    uint8_t columnHeight = map8(spectrumByte[i], 1, columnSize);
//
//    for (uint8_t j = columnStart; j < columnStart + columnHeight; j++) {
//      if (j >= NUM_LEDS || j >= columnEnd)
//        continue;
//
//      leds[j] = CHSV(i * 40, 255, 255);
//    }
//  }
//}
//
//void analyzerPeakColumns()
//{
//  // audioReader();
//  fill_solid(leds, NUM_LEDS, CRGB::Black);
//
//  const uint8_t columnSize = NUM_LEDS / 7;
//
//  for (uint8_t i = 0; i < 7; i++) {
//    uint8_t columnStart = i * columnSize;
//    uint8_t columnEnd = columnStart + columnSize;
//
//    if (columnEnd >= NUM_LEDS) columnEnd = NUM_LEDS - 1;
//
//    uint8_t columnHeight = map(spectrumValue[i], 0, 1023, 0, columnSize);
//    uint8_t peakHeight = map(spectrumPeaks[i], 0, 1023, 0, columnSize);
//
//    for (uint8_t j = columnStart; j < columnStart + columnHeight; j++) {
//      if (j < NUM_LEDS && j <= columnEnd) {
//        leds[j] = CHSV(i * 40, 255, 128);
//      }
//    }
//
//    uint8_t k = columnStart + peakHeight;
//    if (k < NUM_LEDS && k <= columnEnd)
//      leds[k] = CHSV(i * 40, 255, 255);
//  }
//}
//
//void beatWaves()
//{
//  // audioReader();
//  fade_down(2);
//
//  if (beatDetect()) {
//    leds[CENTER_LED] = CRGB::Red;
//  }
//
//  //move to the left
//  for (int i = NUM_LEDS - 1; i > CENTER_LED; i--) {
//    leds[i] = leds[i - 1];
//  }
//  // move to the right
//  for (int i = 0; i < CENTER_LED; i++) {
//    leds[i] = leds[i + 1];
//  }
//}




void radiate() {
  //int SPEED = mono[0] * 0.004;
  //HALF_POS = beatsin8(40, 30 + SPEED, 80 + SPEED);
  int MILLISECONDS  = 0;
  int HALF_POS=NUM_LEDS/2;
  //lowPass_audio = 0.10;
  //filter_min    = 100;

  //  EVERY_N_MILLISECONDS(50) {
  //    hue++;
  //    if ( hue > 255) hue = 0;
  //  }

  leds[HALF_POS] = CRGB(spectrumByte[0], spectrumByte[3], spectrumByte[6]);
  leds[HALF_POS] = CRGB(spectrumByte[0], spectrumByte[3],spectrumByte[6]);
  //leds[HALF_POS].fadeToBlackBy(30);
  
  EVERY_N_MILLISECONDS(random(50)) {
    
    for (int i = NUM_LEDS - 1; i > HALF_POS + 1; i--) {
      leds[i].blue = leds[i - 1].blue;
    }
    for (int i = 0; i < HALF_POS; i++) {
      leds[i].blue = leds[i + 1].blue;
    }
  }
  EVERY_N_MILLISECONDS(27) {
    for (int i = NUM_LEDS - 1; i > HALF_POS + 1; i--) {
      leds[i].green = leds[i - 1].green;
    }
    for (int i = 0; i < HALF_POS; i++) {
      leds[i].green = leds[i + 1].green;
    }
  }
  EVERY_N_MILLISECONDS(52) {
    for (int i = NUM_LEDS - 1; i > HALF_POS + 1; i--) {
      leds[i].red = leds[i - 1].red;
    }
    for (int i = 0; i < HALF_POS; i++) {
      leds[i].red = leds[i + 1].red;
    }
  }

  EVERY_N_MILLISECONDS(2) {
    //blur1d(leds, NUM_LEDS, 1);
  }
}

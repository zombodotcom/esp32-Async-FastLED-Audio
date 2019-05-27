#define VUFadeFactor 5
#define VUScaleFactor 2.0
#define VUPaletteFactor 1.5
//void drawVU() {
//  CRGB pixelColor;
//
//  const float xScale = 255.0 / (NUM_LEDS / 2);
//  float specCombo = (spectrumDecay[0] + spectrumDecay[1] + spectrumDecay[2] + spectrumDecay[3]) / 4.0;
//
//  for (byte x = 0; x < NUM_LEDS / 2; x++) {
//    int senseValue = specCombo / VUScaleFactor - xScale * x;
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
//}

//void drawVU2() {
//  uint8_t avg = map8(spectrumAvg, 0, NUM_LEDS - 1);
//  
//  for (uint8_t i = 0; i < NUM_LEDS; i++) {
//    if(i <= avg) {
//      leds[i] = ColorFromPalette(palettes[currentPaletteIndex], (240 / NUM_LEDS) * i);
//    }
//    else {
//      leds[i] = CRGB::Black;
//    }
//  }
//}

//void spectrumPaletteWaves()
//{
////  fade_down(1);
//
//  CRGB color6 = ColorFromPalette(CurrentPalette, spectrumByte[6], spectrumByte[6]);
//  CRGB color5 = ColorFromPalette(CurrentPalette, spectrumByte[5] / 8, spectrumByte[5] / 8);
//  CRGB color1 = ColorFromPalette(CurrentPalette, spectrumByte[1] / 2, spectrumByte[1] / 2);
//
//  CRGB color = nblend(color6, color5, 256 / 8);
//  color = nblend(color, color1, 256 / 2);
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
//void spectrumPaletteWaves2()
//{
////  fade_down(1);
//
//  CRGBPalette16 palette = palettes[currentPaletteIndex];
//
//  CRGB color6 = ColorFromPalette(palette, 255 - spectrumByte[6], spectrumByte[6]);
//  CRGB color5 = ColorFromPalette(palette, 255 - spectrumByte[5] / 8, spectrumByte[5] / 8);
//  CRGB color1 = ColorFromPalette(palette, 255 - spectrumByte[1] / 2, spectrumByte[1] / 2);
//
//  CRGB color = nblend(color6, color5, 256 / 8);
//  color = nblend(color, color1, 256 / 2);
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

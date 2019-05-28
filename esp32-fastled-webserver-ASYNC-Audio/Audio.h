/*
   ESP8266 + FastLED + IR Remote: https://github.com/jasoncoon/esp8266-fastled-audio
   Copyright (C) 2015-2016 Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Portions of this file are adapted from the work of Stefan Petrick:
// https://plus.google.com/u/0/115124694226931502095

// Portions of this file are adapted from RGB Shades Audio Demo Code by Garrett Mace:
// https://github.com/macetech/RGBShadesAudio

// Pin definitions
#define MSGEQ7_AUDIO_PIN 34
#define MSGEQ7_STROBE_PIN 26
#define MSGEQ7_RESET_PIN  27
#define CENTER_LED NUM_LEDS/2

#define AUDIODELAY 0

// Smooth/average settings
#define SPECTRUMSMOOTH 0.08
#define PEAKDECAY 0.01
#define NOISEFLOOR 65

// AGC settings
#define AGCSMOOTH 0.004
#define GAINUPPERLIMIT 15.0
#define GAINLOWERLIMIT 0.1

// Global variables
unsigned int spectrumValue[7];  // holds raw adc values
float spectrumDecay[7] = {0};   // holds time-averaged values
float spectrumPeaks[7] = {0};   // holds peak values
float audioAvg = 270.0;
float gainAGC = 0.0;
uint8_t samplepeak=0;
uint8_t spectrumByte[7];        // holds 8-bit adjusted adc values

uint8_t spectrumAvg;

uint8_t timeval = 20; 
unsigned long currentMillis; // store current loop's millis value
unsigned long audioMillis; // store time of last audio update

void initializeAudio() {
  pinMode(MSGEQ7_AUDIO_PIN, INPUT);
  pinMode(MSGEQ7_RESET_PIN, OUTPUT);
  pinMode(MSGEQ7_STROBE_PIN, OUTPUT);

  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
}

void readAudio() {
  static PROGMEM const byte spectrumFactors[7] = {9, 11, 13, 13, 12, 12, 13};

  // reset MSGEQ7 to first frequency bin
  digitalWrite(MSGEQ7_RESET_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);

  // store sum of values for AGC
  int analogsum = 0;

  // cycle through each MSGEQ7 bin and read the analog values
  for (int i = 0; i < 7; i++) {

    // set up the MSGEQ7
    digitalWrite(MSGEQ7_STROBE_PIN, LOW);
    delayMicroseconds(50); // to allow the output to settle

    // read the analog value
    spectrumValue[i] = analogRead(MSGEQ7_AUDIO_PIN);
    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);

    // noise floor filter
    if (spectrumValue[i] < NOISEFLOOR) {
      spectrumValue[i] = 0;
    } else {
      spectrumValue[i] -= NOISEFLOOR;
    }

    // apply correction factor per frequency bin
    spectrumValue[i] = (spectrumValue[i] * pgm_read_byte_near(spectrumFactors + i)) / 10;

    // prepare average for AGC
    analogsum += spectrumValue[i];

    // apply current gain value
    spectrumValue[i] *= gainAGC;

    // process time-averaged values
    spectrumDecay[i] = (1.0 - SPECTRUMSMOOTH) * spectrumDecay[i] + SPECTRUMSMOOTH * spectrumValue[i];

    // process peak values
    if (spectrumPeaks[i] < spectrumDecay[i]) spectrumPeaks[i] = spectrumDecay[i];
    spectrumPeaks[i] = spectrumPeaks[i] * (1.0 - PEAKDECAY);
    spectrumByte[i] = spectrumValue[i] / 4;
  }

  // Calculate audio levels for automatic gain
  audioAvg = (1.0 - AGCSMOOTH) * audioAvg + AGCSMOOTH * (analogsum / 7.0);

  spectrumAvg = (analogsum / 7.0) / 4;

  // Calculate gain adjustment factor
  gainAGC = 270.0 / audioAvg;
  if (gainAGC > GAINUPPERLIMIT) gainAGC = GAINUPPERLIMIT;
  if (gainAGC < GAINLOWERLIMIT) gainAGC = GAINLOWERLIMIT;
}

void audioReader(){
     int currentMillis = millis(); // save the current timer value
  int audioMillis=0;
  // analyze the audio input
  if (currentMillis - audioMillis > AUDIODELAY) {
    audioMillis = currentMillis;
    readAudio();
  }
}

// Attempt at beat detection
byte beatTriggered = 0;
#define beatLevel 20.0
#define beatDeadzone 30.0
#define beatDelay 50
float lastBeatVal = 0;
byte beatDetect() {
  static float beatAvg = 0;
  static unsigned long lastBeatMillis;
  float specCombo = (spectrumDecay[0] + spectrumDecay[1]) / 2.0;
  beatAvg = (1.0 - AGCSMOOTH) * beatAvg + AGCSMOOTH * specCombo;

  if (lastBeatVal < beatAvg) lastBeatVal = beatAvg;
  if ((specCombo - beatAvg) > beatLevel && beatTriggered == 0 && currentMillis - lastBeatMillis > beatDelay) {
    beatTriggered = 1;
    lastBeatVal = specCombo;
    lastBeatMillis = currentMillis;
    return 1;
  } else if ((lastBeatVal - specCombo) > beatDeadzone) {
    beatTriggered = 0;
    return 0;
  } else {
    return 0;
  }
}

void fade_down(uint8_t value) {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].fadeToBlackBy(value);
  }
}

//
//void spectrumWaves()
//{
////   audioReader();
//  fade_down(2);
//
//  CRGB color = CRGB(spectrumByte[6], spectrumByte[5] / 8, spectrumByte[1] / 2);
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


  EVERY_N_MILLISECONDS(11) {
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








void audioPrinter(){

  for (int band = 0; band < 7; band++) {
    Serial.print(spectrumByte[band]);
    Serial.print("\t");

  }
  Serial.println();
}

// ----- IR remote
#define REMOTE_TYPE 1       // 0 - no remote, 1 - WAVGAT, 2 - KEYES, 3 - custom

// ----- settings
#define KEEP_SETTINGS 1     // save all settings in EEPROM
#define KEEP_STATE 1	      // save on/off state
#define RESET_SETTINGS 0    // reset settings in EEPROM during startup
#define SETTINGS_LOG 1      // display all EEPROM settings on startup

// ----- LED strip settings
#define NUM_LEDS 120        // number of LEDs (up to 410 supported)
const int MAX_CH = NUM_LEDS / 2;
#define CURRENT_LIMIT 3000  // current limit in mAh, adjusts brightness automatically. 0 - disable limit
byte BRIGHTNESS = 200;      // default brightness (0 - 255)

// ----- connection pins
#define SOUND_R A2         // analog audio input, right channel
#define SOUND_L A1         // analog audio input, left channel
#define SOUND_R_FREQ A3    // analog audio input, right channel for frequency mode
#define BTN_PIN 3          // mode switch button

#if defined(__AVR_ATmega32U4__)
#define MLED_PIN 17
#define MLED_ON LOW
#define LED_PIN 9
#else
#define MLED_PIN 13
#define MLED_ON HIGH
#define LED_PIN 12
#endif

#define POT_GND A0
#define IR_PIN 2

// ----- rainbow settings
float RAINBOW_STEP = 5.00;

// ----- rendering
#define DEFAULT_MODE 0
#define MAIN_LOOP 5               // main rendering cycle duration (default: 5)

// ----- audio signal
#define STEREO 1                  // 0 - mono sound (SOUND_R), 1 - stereo sound
#define EXP 1.4
#define POTENT 0
byte EMPTY_BRIGHTNESS = 30;
#define EMPTY_COLOR HUE_PURPLE
#define AVERAGE_DYNAMIC 0.006

uint16_t LOW_PASS = 100;
uint16_t SPEKTR_LOW_PASS = 40;
#define AUTO_LOW_PASS 0
#define EEPROM_LOW_PASS 1
#define LOW_PASS_ADD 13
#define LOW_PASS_FREQ_ADD 3

float SMOOTH = 0.3;
#define MAX_COEF 1.8

float SMOOTH_FREQ = 0.8;
float MAX_COEF_FREQ = 1.2;
#define SMOOTH_STEP 20
#define LOW_COLOR HUE_RED
#define MID_COLOR HUE_GREEN
#define HIGH_COLOR HUE_YELLOW

uint16_t STROBE_PERIOD = 140;
#define STROBE_DUTY 20
#define STROBE_COLOR HUE_YELLOW
#define STROBE_SAT 0
byte STROBE_SMOOTH = 200;

byte LIGHT_COLOR = 0;
byte LIGHT_SAT = 255;
byte COLOR_SPEED = 100;
int RAINBOW_PERIOD = 1;
float RAINBOW_STEP_2 = 0.5;
#define LOWER_TEMP 20.
#define UPPER_TEMP 50.

byte RUNNING_SPEED = 11;

byte HUE_START = 0;
byte HUE_STEP = 5;
#define LIGHT_SMOOTH 2

/*
  HUE_RED
  HUE_ORANGE
  HUE_YELLOW
  HUE_GREEN
  HUE_AQUA
  HUE_BLUE
  HUE_PURPLE
  HUE_PINK
*/

// ----- WAVGAT remote buttons -----
#if REMOTE_TYPE == 1
#define BUTT_UP     0xF39EEBAD
#define BUTT_DOWN   0xC089F6AD
#define BUTT_LEFT   0xE25410AD
#define BUTT_RIGHT  0x14CE54AD
#define BUTT_OK     0x297C76AD
#define BUTT_1      0x4E5BA3AD
#define BUTT_2      0xE51CA6AD
#define BUTT_3      0xE207E1AD
#define BUTT_4      0x517068AD
#define BUTT_5      0x1B92DDAD
#define BUTT_6      0xAC2A56AD
#define BUTT_7      0x5484B6AD
#define BUTT_8      0xD22353AD
#define BUTT_9      0xDF3F4BAD
#define BUTT_0      0xF08A26AD
#define BUTT_STAR   0x68E456AD
#define BUTT_HASH   0x151CD6AD
#endif

// ----- KEYES remote buttons -----
#if REMOTE_TYPE == 2
#define BUTT_UP     0xE51CA6AD
#define BUTT_DOWN   0xD22353AD
#define BUTT_LEFT   0x517068AD
#define BUTT_RIGHT  0xAC2A56AD
#define BUTT_OK     0x1B92DDAD
#define BUTT_1      0x68E456AD
#define BUTT_2      0xF08A26AD
#define BUTT_3      0x151CD6AD
#define BUTT_4      0x18319BAD
#define BUTT_5      0xF39EEBAD
#define BUTT_6      0x4AABDFAD
#define BUTT_7      0xE25410AD
#define BUTT_8      0x297C76AD
#define BUTT_9      0x14CE54AD
#define BUTT_0      0xC089F6AD
#define BUTT_STAR   0xAF3F1BAD
#define BUTT_HASH   0x38379AD
#endif

// ----- custom remote buttons -----
#if REMOTE_TYPE == 3
#define BUTT_UP     0xE51CA6AD
#define BUTT_DOWN   0xD22353AD
#define BUTT_LEFT   0x517068AD
#define BUTT_RIGHT  0xAC2A56AD
#define BUTT_OK     0x1B92DDAD
#define BUTT_1      0x68E456AD
#define BUTT_2      0xF08A26AD
#define BUTT_3      0x151CD6AD
#define BUTT_4      0x18319BAD
#define BUTT_5      0xF39EEBAD
#define BUTT_6      0x4AABDFAD
#define BUTT_7      0xE25410AD
#define BUTT_8      0x297C76AD
#define BUTT_9      0x14CE54AD
#define BUTT_0      0xC089F6AD
#define BUTT_STAR   0xAF3F1BAD  // *
#define BUTT_HASH   0x38379AD   // #
#endif


#define MODE_AMOUNT 9

#define STRIPE NUM_LEDS / 5
float freq_to_stripe = NUM_LEDS / 40;

#define FHT_N 64
#define LOG_OUT 1
#include <FHT.h>

#include <EEPROMex.h>

#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERNAL
#include "FastLED.h"
CRGB leds[NUM_LEDS];

#include "GyverButton.h"
GButton butt1(BTN_PIN);

#include "IRLremote.h"
CHashIR IRLremote;
uint32_t IRdata;

DEFINE_GRADIENT_PALETTE(soundlevel_gp) {
  0,    0,    255,  0,  // green
  100,  255,  255,  0,  // yellow
  150,  255,  100,  0,  // orange
  200,  255,  50,   0,  // red
  255,  255,  0,    0   // red
};
CRGBPalette32 myPal = soundlevel_gp;

int i, j;
int hue;
unsigned long main_timer, hue_timer, strobe_timer, running_timer, color_timer, rainbow_timer, eeprom_timer;
const float PALETTE_STEP = 255. / MAX_CH;
boolean lowFlag;
byte low_pass;
int colorMusic[3];
float colorMusic_f[3], colorMusic_aver[3];
boolean colorMusicFlash[3], strobeUp_flag, strobeDwn_flag;
byte this_mode = DEFAULT_MODE;
int thisBright[3], strobe_bright = 0;
unsigned int light_time = STROBE_PERIOD * STROBE_DUTY / 100;
volatile boolean ir_flag;
boolean settings_mode, ONstate = true;
int8_t freq_strobe_mode, light_mode;
int freq_max;
float freq_max_f, rainbow_steps;
int freq_f[32];
int this_color;
boolean running_flag[3], eeprom_flag;
float current_temperature;

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
#if CURRENT_LIMIT > 0
  FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
#endif
  FastLED.setBrightness(BRIGHTNESS);

#ifdef __AVR_ATmega32U4__
  TXLED1;
  delay (1000);
#endif
  pinMode(MLED_PIN, OUTPUT);
  digitalWrite(MLED_PIN, !MLED_ON);

  pinMode(POT_GND, OUTPUT);
  digitalWrite(POT_GND, LOW);
  butt1.setTimeout(900);

  IRLremote.begin(IR_PIN);

#if POTENT
  analogReference(EXTERNAL);
#else
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  analogReference(INTERNAL1V1);
  #else
  analogReference(INTERNAL);
  #endif
#endif

  // Increasing sampling rate to 19.2 KHz
  // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

#if RESET_SETTINGS
  EEPROM.write(100, 0);
# endif

#if AUTO_LOW_PASS && !EEPROM_LOW_PASS
  autoLowPass();
#endif

#if EEPROM_LOW_PASS
  LOW_PASS = EEPROM.readInt(70);
  SPEKTR_LOW_PASS = EEPROM.readInt(72);
#endif

#if KEEP_SETTINGS
  if (EEPROM.read(100) == 100) {
    readEEPROM();
  } else { // this is a first start
    EEPROM.write(100, 100);
    updateEEPROM();
  }
#endif

#if SETTINGS_LOG
  Serial.print(F("this_mode = ")); Serial.println(this_mode);
  Serial.print(F("freq_strobe_mode = ")); Serial.println(freq_strobe_mode);
  Serial.print(F("light_mode = ")); Serial.println(light_mode);
  Serial.print(F("RAINBOW_STEP = ")); Serial.println(RAINBOW_STEP);
  Serial.print(F("MAX_COEF_FREQ = ")); Serial.println(MAX_COEF_FREQ);
  Serial.print(F("STROBE_PERIOD = ")); Serial.println(STROBE_PERIOD);
  Serial.print(F("LIGHT_SAT = ")); Serial.println(LIGHT_SAT);
  Serial.print(F("RAINBOW_STEP_2 = ")); Serial.println(RAINBOW_STEP_2);
  Serial.print(F("HUE_START = ")); Serial.println(HUE_START);
  Serial.print(F("SMOOTH = ")); Serial.println(SMOOTH);
  Serial.print(F("SMOOTH_FREQ = ")); Serial.println(SMOOTH_FREQ);
  Serial.print(F("STROBE_SMOOTH = ")); Serial.println(STROBE_SMOOTH);
  Serial.print(F("LIGHT_COLOR = ")); Serial.println(LIGHT_COLOR);
  Serial.print(F("COLOR_SPEED = ")); Serial.println(COLOR_SPEED);
  Serial.print(F("RAINBOW_PERIOD = ")); Serial.println(RAINBOW_PERIOD);
  Serial.print(F("RUNNING_SPEED = ")); Serial.println(RUNNING_SPEED);
  Serial.print(F("HUE_STEP = ")); Serial.println(HUE_STEP);
  Serial.print(F("EMPTY_BRIGHTNESS = ")); Serial.println(EMPTY_BRIGHTNESS);
  Serial.print(F("ONstate = ")); Serial.println(ONstate);
#endif
}

void loop() {
  buttonTick();
#if REMOTE_TYPE != 0
  remoteTick();
#endif
  serialTick();
  mainLoop();
  eepromTick();
}

void mainLoop() {
  if (ONstate) {
    if (millis() - main_timer > MAIN_LOOP) {
      switch (this_mode) {
        case 0:
        case 1:
          vuMeterCalculation();
          break;
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
          analyzeAudio();
          colorMusic[0] = 0;
          colorMusic[1] = 0;
          colorMusic[2] = 0;
          for (i = 0 ; i < 32 ; ++i) {
            if (fht_log_out[i] < SPEKTR_LOW_PASS) fht_log_out[i] = 0;
          }

          for (i = 2; i < 6; ++i) {
            if (fht_log_out[i] > colorMusic[0]) colorMusic[0] = fht_log_out[i];
          }

          for (i = 6; i < 11; ++i) {
            if (fht_log_out[i] > colorMusic[1]) colorMusic[1] = fht_log_out[i];
          }

          for (i = 11; i < 32; ++i) {
            if (fht_log_out[i] > colorMusic[2]) colorMusic[2] = fht_log_out[i];
          }
          freq_max = 0;
          for (i = 0; i < 30; ++i) {
            if (fht_log_out[i + 2] > freq_max) freq_max = fht_log_out[i + 2];
            if (freq_max < 5) freq_max = 5;

            if (freq_f[i] < fht_log_out[i + 2]) freq_f[i] = fht_log_out[i + 2];
            if (freq_f[i] > 0) freq_f[i] -= LIGHT_SMOOTH;
            else freq_f[i] = 0;
          }
          freq_max_f = freq_max * AVERAGE_DYNAMIC + freq_max_f * (1 - AVERAGE_DYNAMIC);
          for (i = 0; i < 3; ++i) {
            colorMusic_aver[i] = colorMusic[i] * AVERAGE_DYNAMIC + colorMusic_aver[i] * (1 - AVERAGE_DYNAMIC);
            colorMusic_f[i] = colorMusic[i] * SMOOTH_FREQ + colorMusic_f[i] * (1 - SMOOTH_FREQ);
            if (colorMusic_f[i] > ((float)colorMusic_aver[i] * MAX_COEF_FREQ)) {
              thisBright[i] = 255;
              colorMusicFlash[i] = true;
              running_flag[i] = true;
            } else colorMusicFlash[i] = false;
            if (thisBright[i] >= 0) thisBright[i] -= SMOOTH_STEP;
            if (thisBright[i] < EMPTY_BRIGHTNESS) {
              thisBright[i] = EMPTY_BRIGHTNESS;
              running_flag[i] = false;
            }
          }
          render();
          break;
        case 5:
          if ((long)millis() - strobe_timer > STROBE_PERIOD) {
            strobe_timer = millis();
            strobeUp_flag = true;
            strobeDwn_flag = false;
          }
          if ((long)millis() - strobe_timer > light_time) {
            strobeDwn_flag = true;
          }
          if (strobeUp_flag) {
            if (strobe_bright < 255)
              strobe_bright += STROBE_SMOOTH;
            if (strobe_bright > 255) {
              strobe_bright = 255;
              strobeUp_flag = false;
            }
          }

          if (strobeDwn_flag) {
            if (strobe_bright > 0)
              strobe_bright -= STROBE_SMOOTH;
            if (strobe_bright < 0) {
              strobeDwn_flag = false;
              strobe_bright = 0;
            }
          }
          render();
          break;
        case 6:
          render();
          break;
      }

      if (!IRLremote.receiving()) {
        FastLED.show();
      }

      if (this_mode != 7 && !(this_mode == 6 && light_mode == 3)) {
        FastLED.clear();
      }
      main_timer = millis();
    }
  }
}

// EFFECTS

// VU Meter

#define VOLUME_READINGS 100
#define RAINBOW_SHIFT_TIME 30

int rightLength, leftLength;
float rightVolumeMeasurement, currentRightVolume, rightVolume;
float leftVolumeMeasurement, currentLeftVolume, leftVolume;

float averageVolume = 50;
int maxVolume = 100;

CHSV currentEmptyColor = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);

void vuMeterCalculation() {
  currentRightVolume = 0;
  currentLeftVolume = 0;

  for (i = 0; i < VOLUME_READINGS; ++i) {
    rightVolumeMeasurement = analogRead(SOUND_R);
#if STEREO
    leftVolumeMeasurement = analogRead(SOUND_L);
#endif

    if (currentRightVolume < rightVolumeMeasurement) {
      currentRightVolume = rightVolumeMeasurement;
    }

#if STEREO
    if (currentLeftVolume < leftVolumeMeasurement) {
      currentLeftVolume = leftVolumeMeasurement;
    }
#endif
  }

  currentRightVolume = map(currentRightVolume, LOW_PASS, 1023, 0, 500);
  currentRightVolume = constrain(currentRightVolume, 0, 500);
  currentRightVolume = pow(currentRightVolume, EXP);
  rightVolume = currentRightVolume * SMOOTH + rightVolume * (1 - SMOOTH);

#if STEREO
  currentLeftVolume = map(currentLeftVolume, LOW_PASS, 1023, 0, 500);
  currentLeftVolume = constrain(currentLeftVolume, 0, 500);
  currentLeftVolume = pow(currentLeftVolume, EXP);
  leftVolume = currentLeftVolume * SMOOTH + leftVolume * (1 - SMOOTH);
#else
  leftVolume = rightVolume;
#endif

  if (EMPTY_BRIGHTNESS > 5) {
    currentEmptyColor = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
    for (i = 0; i < NUM_LEDS; ++i) {
      leds[i] = currentEmptyColor;
    }
  }

  if (rightVolume > 15 && leftVolume > 15) {
    averageVolume = (rightVolume + leftVolume) * 0.5 * AVERAGE_DYNAMIC + averageVolume * (1 - AVERAGE_DYNAMIC);
    maxVolume = averageVolume * MAX_COEF;

    rightLength = map(rightVolume, 0, maxVolume, 0, MAX_CH);
    leftLength = map(leftVolume, 0, maxVolume, 0, MAX_CH);

    rightLength = constrain(rightLength, 0, MAX_CH);
    leftLength = constrain(leftLength, 0, MAX_CH);

    render();
  }
}

void renderVUMeter() {
  for (i = 0; i < rightLength; ++i) {
    leds[(MAX_CH - 1) - i] = ColorFromPalette(myPal, (i * PALETTE_STEP));
  }

  for (i = 0; i < leftLength; ++i) {
    leds[MAX_CH + i] = ColorFromPalette(myPal, (i * PALETTE_STEP));
  }
}

void renderVUMeterRainbow() {
  if (millis() - rainbow_timer > RAINBOW_SHIFT_TIME) { // TODO: global timer
    rainbow_timer = millis(); // TODO: global timer
    hue = floor((float)hue + RAINBOW_STEP);
  }

  for (i = 0; i < rightLength; ++i) {
    leds[(MAX_CH - 1) - i] = ColorFromPalette(RainbowColors_p, (i * PALETTE_STEP) / 2 - hue);
  }

  for (i = 0; i < leftLength; ++i) {
    leds[MAX_CH + i] = ColorFromPalette(RainbowColors_p, (i * PALETTE_STEP) / 2 - hue);
  }
}

// Zones

// 5 zones

void renderFiveZones() {
  for (i = 0; i < NUM_LEDS; ++i) {
    if (i < STRIPE)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
    else if (i < STRIPE * 2) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
    else if (i < STRIPE * 3) leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
    else if (i < STRIPE * 4) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
    else if (i < STRIPE * 5) leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
  }
}

// 3 zones

void renderThreeZones() {
  for (i = 0; i < NUM_LEDS; ++i) {
    if (i < NUM_LEDS / 3)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
    else if (i < NUM_LEDS * 2 / 3) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
    else if (i < NUM_LEDS)         leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
  }
}

// Frequency strobe

void renderFrequencyStrobe() {
  switch (freq_strobe_mode) {
    case 0:
      if (colorMusicFlash[2]) HIGHS();
      else if (colorMusicFlash[1]) MIDS();
      else if (colorMusicFlash[0]) LOWS();
      else SILENCE();
      break;
    case 1:
      if (colorMusicFlash[2]) HIGHS();
      else SILENCE();
      break;
    case 2:
      if (colorMusicFlash[1]) MIDS();
      else SILENCE();
      break;
    case 3:
      if (colorMusicFlash[0]) LOWS();
      else SILENCE();
      break;
  }
}

// Strobe

void renderStrobe() {
  if (strobe_bright > 0) {
    for (i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CHSV(STROBE_COLOR, STROBE_SAT, strobe_bright);
    }
  } else {
    for (i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
    }
  }
}

void render() {
  switch (this_mode) {
    case 0:
      renderVUMeter();
      break;
    case 1:
      renderVUMeterRainbow();
      break;
    case 2:
      renderFiveZones();
      break;
    case 3:
      renderThreeZones();
      break;
    case 4:
      renderFrequencyStrobe();
      break;
    case 5:
      renderStrobe();
      break;
    case 6:
      switch (light_mode) {
        case 0: for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(LIGHT_COLOR, LIGHT_SAT, 255);
          break;
        case 1:
          if (millis() - color_timer > COLOR_SPEED) {
            color_timer = millis();
            if (++this_color > 255) this_color = 0;
          }
          for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(this_color, LIGHT_SAT, 255);
          break;
        case 2:
          if (millis() - rainbow_timer > 30) {
            rainbow_timer = millis();
            this_color += RAINBOW_PERIOD;
            if (this_color > 255) this_color = 0;
            if (this_color < 0) this_color = 255;
          }
          rainbow_steps = this_color;
          for (i = 0; i < NUM_LEDS; ++i) {
            leds[i] = CHSV((int)floor(rainbow_steps), 255, 255);
            rainbow_steps += RAINBOW_STEP_2;
            if (rainbow_steps > 255) rainbow_steps = 0;
            if (rainbow_steps < 0) rainbow_steps = 255;
          }
          break;
        case 3:
          CRGB new_color = whiteColor(current_temperature);
          for (i = 0; i < NUM_LEDS; ++i) {
            leds[i] = new_color;
          }
          break;
      }
      break;
    case 7:
      switch (freq_strobe_mode) {
        case 0:
          if (running_flag[2]) leds[NUM_LEDS / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else if (running_flag[1]) leds[NUM_LEDS / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else if (running_flag[0]) leds[NUM_LEDS / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[NUM_LEDS / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
          break;
        case 1:
          if (running_flag[2]) leds[NUM_LEDS / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else leds[NUM_LEDS / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
          break;
        case 2:
          if (running_flag[1]) leds[NUM_LEDS / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else leds[NUM_LEDS / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
          break;
        case 3:
          if (running_flag[0]) leds[NUM_LEDS / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[NUM_LEDS / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
          break;
      }
      leds[(NUM_LEDS / 2) - 1] = leds[NUM_LEDS / 2];
      if (millis() - running_timer > RUNNING_SPEED) {
        running_timer = millis();
        for (i = 0; i < NUM_LEDS / 2 - 1; ++i) {
          leds[i] = leds[i + 1];
          leds[NUM_LEDS - i - 1] = leds[i];
        }
      }
      break;
    case 8:
      byte HUEPALETTE_STEP = HUE_START;
      for (i = 0; i < NUM_LEDS / 2; ++i) {
        byte this_bright = map(freq_f[(int)floor((NUM_LEDS / 2 - i) / freq_to_stripe)], 0, freq_max_f, 0, 255);
        this_bright = constrain(this_bright, 0, 255);
        leds[i] = CHSV(HUEPALETTE_STEP, 255, this_bright);
        leds[NUM_LEDS - i - 1] = leds[i];
        HUEPALETTE_STEP += HUE_STEP;
        if (HUEPALETTE_STEP > 255) HUEPALETTE_STEP = 0;
      }
      break;
  }
}

void HIGHS() {
  for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
}
void MIDS() {
  for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
}
void LOWS() {
  for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
}
void SILENCE() {
  for (i = 0; i < NUM_LEDS; ++i) leds[i] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHTNESS);
}

int smartIncr(int value, int incr_step, int mininmum, int maximum) {
  return constrain(value + incr_step, mininmum, maximum);
}

float smartIncrFloat(float value, float incr_step, float mininmum, float maximum) {
  return constrain(value + incr_step, mininmum, maximum);
}

CRGB whiteColor(float temperature) {
  double temp;
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  if (temperature <= 66) {
    red = 255;
    temp = 99.4708025861 * log(temperature) - 161.1195681661;
    temp = round(temp);
    green = constrain(temp, 0, 255);
    if (temperature <= 19)
      blue = 0;
    else {
      temp = 138.5177312231 * log(temperature - 10.) - 305.0447927307;
      temp = round(temp);
      blue = constrain(temp, 0, 255);
    }
  } else {
    temp = 329.698727446 * pow(temperature - 60., -0.1332047592);
    temp = round(temp);
    red = constrain(temp, 0, 255);
    temp = 288.1221695283 * pow(temperature - 60., -0.0755148492);
    temp = round(temp);
    green = constrain(temp, 0, 255);
    blue = 255;
  }

  return CRGB(red, green, blue);
}

String serialCommand;

void serialTick() {
  if (Serial.available()) {
    serialCommand = Serial.readStringUntil('\n');

    if (serialCommand.equals("power")) {
      ONstate = !ONstate;
      FastLED.clear();
      FastLED.show();
      updateEEPROM();
    } else if (serialCommand.equals("brightness")) {
      BRIGHTNESS = Serial.readStringUntil('\n').toInt();
      Serial.print(F("BRIGHTNESS = ")); Serial.println(BRIGHTNESS);
      FastLED.setBrightness(BRIGHTNESS);
    } else if (serialCommand.equals("temperature")) {
      current_temperature = constrain(Serial.readStringUntil('\n').toInt() / 100., LOWER_TEMP, UPPER_TEMP);
      Serial.print(F("TEMP = ")); Serial.println(current_temperature);
    } else if (serialCommand.equals("mode")) {
      this_mode = Serial.readStringUntil('\n').toInt();
      Serial.print(F("MODE = ")); Serial.println(this_mode);
    }
  }
}

#if REMOTE_TYPE != 0
void remoteTick() {
  if (IRLremote.available())  {
    auto data = IRLremote.read();
    IRdata = data.command;
    ir_flag = true;
  }
  if (ir_flag) {
    eeprom_timer = millis();
    eeprom_flag = true;
    switch (IRdata) {
      case BUTT_1: this_mode = 0;
        break;
      case BUTT_2: this_mode = 1;
        break;
      case BUTT_3: this_mode = 2;
        break;
      case BUTT_4: this_mode = 3;
        break;
      case BUTT_5: this_mode = 4;
        break;
      case BUTT_6: this_mode = 5;
        break;
      case BUTT_7: this_mode = 6;
        break;
      case BUTT_8: this_mode = 7;
        break;
      case BUTT_9: this_mode = 8;
        break;
      case BUTT_0: fullLowPass();
        break;
      case BUTT_STAR: ONstate = !ONstate; FastLED.clear(); FastLED.show(); updateEEPROM();
        break;
      case BUTT_HASH:
        switch (this_mode) {
          case 4:
          case 7: if (++freq_strobe_mode > 3) freq_strobe_mode = 0;
            break;
          case 6: if (++light_mode > 3) light_mode = 0;
            break;
        }
        break;
      case BUTT_OK: digitalWrite(MLED_PIN, settings_mode ^ MLED_ON); settings_mode = !settings_mode;
        break;
      case BUTT_UP:
        if (settings_mode) {
          EMPTY_BRIGHTNESS = smartIncr(EMPTY_BRIGHTNESS, 5, 0, 255);
        } else {
          switch (this_mode) {
            case 0:
              break;
            case 1: RAINBOW_STEP = smartIncrFloat(RAINBOW_STEP, 0.5, 0.5, 20);
              break;
            case 2:
            case 3:
            case 4: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, 0.1, 0, 5);
              break;
            case 5: STROBE_PERIOD = smartIncr(STROBE_PERIOD, 20, 1, 1000);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_SAT = smartIncr(LIGHT_SAT, 20, 0, 255);
                  break;
                case 1: LIGHT_SAT = smartIncr(LIGHT_SAT, 20, 0, 255);
                  break;
                case 2: RAINBOW_STEP_2 = smartIncrFloat(RAINBOW_STEP_2, 0.5, 0.5, 10);
                  break;
              }
              break;
            case 7: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, 0.1, 0.0, 10);
              break;
            case 8: HUE_START = smartIncr(HUE_START, 10, 0, 255);
              break;
          }
        }
        break;
      case BUTT_DOWN:
        if (settings_mode) {
          EMPTY_BRIGHTNESS = smartIncr(EMPTY_BRIGHTNESS, -5, 0, 255);
        } else {
          switch (this_mode) {
            case 0:
              break;
            case 1: RAINBOW_STEP = smartIncrFloat(RAINBOW_STEP, -0.5, 0.5, 20);
              break;
            case 2:
            case 3:
            case 4: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, -0.1, 0, 5);
              break;
            case 5: STROBE_PERIOD = smartIncr(STROBE_PERIOD, -20, 1, 1000);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_SAT = smartIncr(LIGHT_SAT, -20, 0, 255);
                  break;
                case 1: LIGHT_SAT = smartIncr(LIGHT_SAT, -20, 0, 255);
                  break;
                case 2: RAINBOW_STEP_2 = smartIncrFloat(RAINBOW_STEP_2, -0.5, 0.5, 10);
                  break;
                case 3: LIGHT_SAT = smartIncr(LIGHT_SAT, -20, 0, 255);
              }
              break;
            case 7: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, -0.1, 0.0, 10);
              break;
            case 8: HUE_START = smartIncr(HUE_START, -10, 0, 255);
              break;
          }
        }
        break;
      case BUTT_LEFT:
        if (settings_mode) {
          BRIGHTNESS = smartIncr(BRIGHTNESS, -20, 0, 255);
          FastLED.setBrightness(BRIGHTNESS);
        } else {
          switch (this_mode) {
            case 0:
            case 1: SMOOTH = smartIncrFloat(SMOOTH, -0.05, 0.05, 1);
              break;
            case 2:
            case 3:
            case 4: SMOOTH_FREQ = smartIncrFloat(SMOOTH_FREQ, -0.05, 0.05, 1);
              break;
            case 5: STROBE_SMOOTH = smartIncr(STROBE_SMOOTH, -20, 0, 255);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_COLOR = smartIncr(LIGHT_COLOR, -10, 0, 255);
                  break;
                case 1: COLOR_SPEED = smartIncr(COLOR_SPEED, -10, 0, 255);
                  break;
                case 2: RAINBOW_PERIOD = smartIncr(RAINBOW_PERIOD, -1, -20, 20);
                  break;
                case 3: current_temperature = smartIncr(current_temperature, -2.5, LOWER_TEMP, UPPER_TEMP);
                  break;
              }
              break;
            case 7: RUNNING_SPEED = smartIncr(RUNNING_SPEED, -10, 1, 255);
              break;
            case 8: HUE_STEP = smartIncr(HUE_STEP, -1, 1, 255);
              break;
          }
        }
        break;
      case BUTT_RIGHT:
        if (settings_mode) {
          BRIGHTNESS = smartIncr(BRIGHTNESS, 20, 0, 255);
          FastLED.setBrightness(BRIGHTNESS);
        } else {
          switch (this_mode) {
            case 0:
            case 1: SMOOTH = smartIncrFloat(SMOOTH, 0.05, 0.05, 1);
              break;
            case 2:
            case 3:
            case 4: SMOOTH_FREQ = smartIncrFloat(SMOOTH_FREQ, 0.05, 0.05, 1);
              break;
            case 5: STROBE_SMOOTH = smartIncr(STROBE_SMOOTH, 20, 0, 255);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_COLOR = smartIncr(LIGHT_COLOR, 10, 0, 255);
                  break;
                case 1: COLOR_SPEED = smartIncr(COLOR_SPEED, 10, 0, 255);
                  break;
                case 2: RAINBOW_PERIOD = smartIncr(RAINBOW_PERIOD, 1, -20, 20);
                  break;
                case 3: current_temperature = smartIncr(current_temperature, 2.5, LOWER_TEMP, UPPER_TEMP);
                  break;
              }
              break;
            case 7: RUNNING_SPEED = smartIncr(RUNNING_SPEED, 10, 1, 255);
              break;
            case 8: HUE_STEP = smartIncr(HUE_STEP, 1, 1, 255);
              break;
          }
        }
        break;
      default: eeprom_flag = false;
        break;
    }
    ir_flag = false;
  }
}
#endif

void autoLowPass() {
  delay(10);
  int thisMax = 0;
  int thisLevel;
  for (i = 0; i < 200; ++i) {
    thisLevel = analogRead(SOUND_R);
    if (thisLevel > thisMax)
      thisMax = thisLevel;
    delay(4);
  }
  LOW_PASS = thisMax + LOW_PASS_ADD;

  thisMax = 0;
  for (i = 0; i < 100; ++i) {
    analyzeAudio();
    for (j = 2; j < 32; ++j) {
      thisLevel = fht_log_out[j];
      if (thisLevel > thisMax)
        thisMax = thisLevel;
    }
    delay(4);
  }
  SPEKTR_LOW_PASS = thisMax + LOW_PASS_FREQ_ADD;
  if (EEPROM_LOW_PASS && !AUTO_LOW_PASS) {
    EEPROM.updateInt(70, LOW_PASS);
    EEPROM.updateInt(72, SPEKTR_LOW_PASS);
  }
}

void analyzeAudio() {
  for (i = 0 ; i < FHT_N ; ++i) {
    int sample = analogRead(SOUND_R_FREQ);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}

void buttonTick() {
  butt1.tick();
  if (butt1.isSingle())
    if (++this_mode >= MODE_AMOUNT) this_mode = 0;

  if (butt1.isHolded()) {
    fullLowPass();
  }
}
void fullLowPass() {
  digitalWrite(MLED_PIN, MLED_ON);
  FastLED.setBrightness(0);
  FastLED.clear();
  FastLED.show();
  delay(500);
  autoLowPass();
  delay(500);
  FastLED.setBrightness(BRIGHTNESS);
  digitalWrite(MLED_PIN, !MLED_ON);
}
void updateEEPROM() {
  EEPROM.updateByte(1, this_mode);
  EEPROM.updateByte(2, freq_strobe_mode);
  EEPROM.updateByte(3, light_mode);
  EEPROM.updateInt(4, RAINBOW_STEP);
  EEPROM.updateFloat(8, MAX_COEF_FREQ);
  EEPROM.updateInt(12, STROBE_PERIOD);
  EEPROM.updateInt(16, LIGHT_SAT);
  EEPROM.updateFloat(20, RAINBOW_STEP_2);
  EEPROM.updateInt(24, HUE_START);
  EEPROM.updateFloat(28, SMOOTH);
  EEPROM.updateFloat(32, SMOOTH_FREQ);
  EEPROM.updateInt(36, STROBE_SMOOTH);
  EEPROM.updateInt(40, LIGHT_COLOR);
  EEPROM.updateInt(44, COLOR_SPEED);
  EEPROM.updateInt(48, RAINBOW_PERIOD);
  EEPROM.updateInt(52, RUNNING_SPEED);
  EEPROM.updateInt(56, HUE_STEP);
  EEPROM.updateInt(60, EMPTY_BRIGHTNESS);
  if (KEEP_STATE) EEPROM.updateByte(64, ONstate);
  EEPROM.updateFloat(65, current_temperature);
}
void readEEPROM() {
  this_mode = EEPROM.readByte(1);
  freq_strobe_mode = EEPROM.readByte(2);
  light_mode = EEPROM.readByte(3);
  RAINBOW_STEP = EEPROM.readInt(4);
  MAX_COEF_FREQ = EEPROM.readFloat(8);
  STROBE_PERIOD = EEPROM.readInt(12);
  LIGHT_SAT = EEPROM.readInt(16);
  RAINBOW_STEP_2 = EEPROM.readFloat(20);
  HUE_START = EEPROM.readInt(24);
  SMOOTH = EEPROM.readFloat(28);
  SMOOTH_FREQ = EEPROM.readFloat(32);
  STROBE_SMOOTH = EEPROM.readInt(36);
  LIGHT_COLOR = EEPROM.readInt(40);
  COLOR_SPEED = EEPROM.readInt(44);
  RAINBOW_PERIOD = EEPROM.readInt(48);
  RUNNING_SPEED = EEPROM.readInt(52);
  HUE_STEP = EEPROM.readInt(56);
  EMPTY_BRIGHTNESS = EEPROM.readInt(60);
  if (KEEP_STATE) ONstate = EEPROM.readByte(64);
  current_temperature = constrain(EEPROM.readFloat(65), LOWER_TEMP, UPPER_TEMP);
}
void eepromTick() {
  if (eeprom_flag)
    if (millis() - eeprom_timer > 30000) {
      eeprom_flag = false;
      eeprom_timer = millis();
      updateEEPROM();
    }
}

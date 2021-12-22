// include libraries
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

// declare user variables
#define THRESHOLD   500
#define LED_COUNT   5
#define BRIGHTNESS  150 // NeoPixel brightness, 0 (min) to 255 (max) 

// declare pins
const int forceSensorPin = A0;
const int ledPin = 8;

// create objects
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_NeoPixel strip(LED_COUNT, ledPin, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// declare variables
bool trigger, blinkState;
int countdown;
long lastUpdateTime, lastCountdownTime, lastBlinkTime;

void setup() {
  // initialize LCD
  lcd.begin(16, 2);

  // initialize NeoPixel
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  // initialize variables
  countdown = 10000;
}

void loop() {
  // reduce count by 10ms every 10ms
  if (millis() - lastCountdownTime > 10) {
    if (countdown > 0) countdown -= 10;
    lastCountdownTime = millis();
  }

  // update the LCD every 25ms
  // if you refresh too fast our eyes cannot see the text displayed
  if (millis() - lastUpdateTime > 25) {
    updateLCD();
    lastUpdateTime = millis();
  }

  // check if timer is still running and sensor is triggered
  if (countdown == 0 && checkSensor()) {
    trigger = true;
  }

  if (trigger) {
    // toggle the blink state every 100ms
    if (millis() - lastBlinkTime > 100) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
    }

    if (blinkState) {
      // show red colour
      strip.fill(strip.Color(255, 0, 0, strip.gamma8(255)));
    } else {
      // show blank
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(255)));
    }
    strip.show();
  }
}

void updateLCD() {
  lcd.clear();
  lcd.print("Countdown:");
  lcd.setCursor(0, 1);
  lcd.print(countdown / 1000.0);
  lcd.print("s");
}

// get an average reading from sensor for stability
bool checkSensor() {
  float average = 0;
  for (int i = 0; i < 25; i++) {
    average += analogRead(forceSensorPin);
  }
  average = average / 25.0;

  if (average > THRESHOLD) return true;
  return false;
}

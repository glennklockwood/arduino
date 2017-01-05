/*
   Make an RGB LED smoothly change between colors using PWM.  Implemented on NodeMCU.
   Glenn K. Lockwood
*/
#define VERBOSE 0
uint8_t pin[3]; /* pins for red, green, and blue */
uint8_t brightness[3]; /* state of each pin's PWM */
uint8_t bright_max = 255;
uint8_t fade_amount = 5;

void setup() {
  for ( uint8_t i = 0; i < 3; i++ ) {
    pinMode(i, OUTPUT);
  }

  pin[0] = 3;
  pin[1] = 5;
  pin[2] = 6;
  brightness[0] = 0;
  brightness[1] = 0;
  brightness[2] = 0;

#if VERBOSE != 0
  Serial.begin(115200);
#endif
}

void loop() {
  uint8_t iters = bright_max / fade_amount + 1;
  uint8_t incr = fade_amount;
  uint8_t r = random(3);

  /* cycle the LED from 0 to bright_max or vice versa */
  for (uint8_t i = 0; i < iters; i++) {

    analogWrite(pin[r], brightness[r]);

    /* keep the brightness bouncing between 0 and bright_max */
    brightness[r] = brightness[r] + incr;
    if (brightness[r] <= 0 || brightness[r] >= bright_max) {
      incr = -incr;
    }
    brightness[r] = max(brightness[r], 0);
    brightness[r] = min(brightness[r], bright_max);

#if VERBOSE != 0
    /* print pwm state for each pin to serial output */
    Serial.print( brightness[0] );
    Serial.print( " " );
    Serial.print( brightness[1] );
    Serial.print( " " );
    Serial.println( brightness[2] );
#endif

    delay(30);
  }
}

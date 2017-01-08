
uint8_t pin[3]; /* pins for red, green, and blue */
uint8_t brightness[3]; /* state of each pin's PWM */
uint8_t i_drain;

#define MAX_BRIGHT 255
#define GRANULE 5

void setup() {
    pin[0] = 3;
    pin[1] = 5;
    pin[2] = 6;

    for ( uint8_t i = 0; i < 3; i++ )
        pinMode(pin[i], OUTPUT);

    brightness[0] = 255;
    brightness[1] = 0;
    brightness[2] = 0;
    
    i_drain = 1;
}

void push_brightness( void ) {
    /*
    uint8_t last_bit;
    last_bit = brightness[2] >> 7;
    brightness[2] <<= 1;
    brightness[2] |= brightness[1] >> 7;
    brightness[1] <<= 1;
    brightness[1] |= brightness[0] >> 7;
    brightness[0] <<= 1;
    brightness[0] |= last_bit;
    */
    if ( brightness[i_drain] < GRANULE ) {
        i_drain = (i_drain + 1) % 3;
    }
    brightness[i_drain] -= GRANULE;
    brightness[(i_drain+1)%3] += GRANULE;
    return;
}

void loop() {
    push_brightness( );

    for ( uint8_t i = 0; i < 3; i++ )
        analogWrite(pin[i], brightness[i]);

    delay(50);
}

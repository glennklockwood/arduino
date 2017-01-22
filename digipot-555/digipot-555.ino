/*
 * Sweep over a range of input voltages and measure analog output voltages.
 * For use with MCP41010 digipot.
 *
 * Glenn K. Lockwood, January 2017
 */
#include <SPI.h>

/* slave select SPI pin */
#define SPI_SS 10
/* drain-drain voltage */
#define V_DD 5.0
/* digipot maximum resistance */
#define MAX_RESIST 10000

void setup() {
    pinMode(SPI_SS, OUTPUT);
    SPI.begin();
    Serial.begin(9600);
}

void loop() {
    for (uint8_t value = 0; value < 255; value++ ) {

        digitalWrite(SPI_SS, LOW);
        SPI.transfer(B00010001); /* cmd: write to channel #1 */
        SPI.transfer(value);     /* resistivity */
        digitalWrite(SPI_SS, HIGH);
        delay(100);

        uint16_t timer_out = analogRead(A0);
        uint16_t timer_in = analogRead(A1);
        Serial.print(value * MAX_RESIST / 255.0);
        Serial.print(" ");
        Serial.print(timer_in * V_DD / 1024.0);
        Serial.print(" ");
        Serial.println(timer_out * V_DD / 1024.0);
    }
}

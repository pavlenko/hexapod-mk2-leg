#include <TWI.h>

#include <stdint.h>

#define STATE_READY 0
#define STATE_REQUESTED_DATA1 1
#define STATE_REQUESTED_DATA2 2

typedef uint8_t AS_WORD;
typedef uint8_t AS_DWORD;

static volatile uint8_t state = STATE_READY;

void onReceive(uint8_t *data, uint8_t length) {
    // Do something with received data as example set new app state
    uint8_t command = TWI.read();// Read byte from RX buffer

    uint16_t data = TWI.read();

    switch (command) {
        case 0x01:
            state = STATE_REQUESTED_DATA1;
            break;
        case 0x02:
            state = STATE_REQUESTED_DATA2;
            break;
    }
}

void onRequest() {
    // Send something to master, may depends on rx data
    switch (state) {
        case STATE_REQUESTED_DATA1:
            TWI.start();     // Reset TX buffer
            TWI.write(0xF1); // Write to TX buffer
            TWI.transmit();  // Start async send TX buffer
            break;
        case STATE_REQUESTED_DATA2:
            TWI.start();
            TWI.write(0xF2);
            TWI.transmit();
            break;
    }
}

int main() {
    TWI.setAddress(0x0F);
    TWI.setOnReceiveHandler(onReceive);
    TWI.setOnRequestHandler(onRequest);
}

/*#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}*/

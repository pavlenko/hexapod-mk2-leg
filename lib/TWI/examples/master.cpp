#include <TWI.h>

void onTransmit() {
    // Do something on transmission complete
    TWI.receive(0xFF, 4);
}

void onReceive() {
    // Do something with received data as example set new app state
    uint8_t value1;
    TWI.read(&value1);// Read byte from RX buffer

    uint16_t value2;
    TWI.read(&value2);// Read word from RX buffer

    uint32_t value3;
    TWI.read(&value3);// Read dword from RX buffer

    float value4;
    TWI.read(&value4);// Read float from RX buffer

    uint8_t values[4];
    TWI.read(values, 4);// Read 4 bytes array from RX buffer
}

int main() {
    // Enable TWI module
    TWI.enable();

    // Set TWI module frequency (optional)
    TWI.setFrequency(400000u);

    // Configure callbacks
    TWI.setOnTransmitHandler(onTransmit);
    TWI.setOnReceiveHandler(onReceive);

    // Configure transmission target
    TWI.start();

    uint8_t value1 = 0;
    TWI.write(value1);// Write byte to TX buffer

    uint16_t value2 = 0;
    TWI.write(value2);// Write word to TX buffer

    uint32_t value3 = 0;
    TWI.write(value3);// Write dword to TX buffer

    float value4 = 0;
    TWI.write(value4);// Write float to TX buffer

    uint8_t values[4];
    TWI.write(values, 4);// Write 4 bytes array to TX buffer

    // Trigger transmission process
    TWI.transmit(0xFFu);
}

/*#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}*/

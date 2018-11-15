#include <TWI.h>
#include <stdint.h>

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

void onRequest() {
    // Send something to master, may depends on rx data
    // There are only need to fill buffer data
    uint8_t value1 = 0;
    TWI.write(&value1);// Write byte to TX buffer

    uint16_t value2 = 0;
    TWI.write(&value2);// Write word to TX buffer

    uint32_t value3 = 0;
    TWI.write(&value3);// Write dword to TX buffer

    float value4 = 0;
    TWI.write(&value4);// Write float to TX buffer

    uint8_t values[4];
    TWI.write(values, 4);// Write 4 bytes array to TX buffer
}

int main() {
    // Enable TWI module
    TWI.enable();

    // Set TWI module frequency (optional)
    TWI.setFrequency(400000u);

    // Configure slave address
    TWI.setAddress(0x0F);

    // Configure callbacks
    TWI.setOnReceiveHandler(onReceive);
    TWI.setOnRequestHandler(onRequest);
}
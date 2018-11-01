#include <TWI.h>

void onTransmit() {
    // Do something on transmission complete
    TWI.receive(0xFF, 4, onReceive);
}

int main() {
    // Configure transmit completed handler if needed
    TWI.setOnTransmitHandler(onTransmit);

    // Configure transmission target
    TWI.start(0xFF);

    // Write data to TX buffer
    TWI.write(0);
    TWI.write(1);
    TWI.write(2);

    TWI.write(TWI_WORD {.value = 1000});
    TWI.write(TWI_DWORD {.value = 16000000});
    TWI.write(TWI_FLOAT {.value = 2.2f});

    // Trigger transmission process
    TWI.transmit(0xFF, onTransmit);
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

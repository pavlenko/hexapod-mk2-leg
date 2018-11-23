#ifndef PCD8544_H
#define PCD8544_H

#include <stdint.h>

#define PCD8544_WIDTH   84
#define PCD8544_HEIGHT  48

enum PCD8544_DC {
    PCD8544_DC_COMMAND,
    PCD8544_DC_DATA
};

class PCD8544 {
public:
    PCD8544();
    void setContrast(uint8_t contrast);
    void write(PCD8544_DC dc, uint8_t data);//TODO write to display
};

#endif //PCD8544_H

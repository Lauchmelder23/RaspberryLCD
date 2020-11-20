#include "../raspberrylcd.h"

int main(void)
{
    LCDScreen screen;
    configurePins(&screen, 7, 9, 8, 0, 0, 0, 0, 21, 22, 23, 24);

    initScreen(&screen, HALF_BYTE_INTERFACE, TWO_LINES, FONT_5x7, CURSOR_ON | CURSOR_BLINK, LEFT_TO_RIGHT);
    
    loadCustomChar(&screen, 0, 0b00000, 0b00000, 0b01010, 0b00000, 0b10001, 0b01110, 0b00000, 0b00000);

    returnCursor(&screen);
    sendChars(&screen, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    setCursor(&screen, 0, 1);
    sendChars(&screen, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    resetPins(&screen);

    return 0;
}
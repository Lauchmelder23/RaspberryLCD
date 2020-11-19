#include "../raspberrylcd.h"

int main(void)
{
    LCDScreen screen;
    configurePins(&screen, 7, 9, 8, 0, 0, 0, 0, 21, 22, 23, 24);

    initScreen(&screen, HALF_BYTE_INTERFACE, TWO_LINES, FONT_5x7, CURSOR_ON | CURSOR_BLINK, LEFT_TO_RIGHT);
    sendData(&screen, JAPANESE_QUOTE_START);
    sendData(&screen, JAPANESE_QUOTE_END);
    resetPins(&screen);

    return 0;
}
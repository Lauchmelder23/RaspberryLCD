#ifndef RASPBERRY_LCD_H
#define RASPBERRY_LCD_H

// Commands
#define SCREEN_CLEAR    0b00000001
#define CURSOR_RETURN   0b00000010
#define INPUT_SET       0b00000100
#define DISPLAY_SWITCH  0b00001000
#define SHIFT           0b00010000
#define FUNCTION_SET    0b00100000
#define CGRAM_AD_SET    0b01000000
#define DDRAM_AD_SET    0b10000000

// Parameters
#define LEFT_TO_RIGHT       0b10
#define RIGHT_TO_LEFT       0b00
#define DISPLAY_ON          0b100
#define DISPLAY_OFF         0b000
#define CURSOR_ON           0b010
#define CURSOR_OFF          0b000
#define CURSOR_BLINK        0b001
#define CURSOR_STATIC       0b000
#define DISPLAY_SHIFT       0b1000
#define CURSOR_SHIFT        0b0000
#define RIGHT_SHIFT         0b0100
#define LEFT_SHIFT          0b0000
#define FULL_BYTE_INTERFACE 0b10000
#define HALF_BYTE_INTERFACE 0b00000
#define ONE_LINE            0b00000
#define TWO_LINES           0b01000
#define FONT_5x10           0b00100
#define FONT_5x7            0b00000

// Special characters supported by the LCD
#define YEN_CURRENCY            0b01011100
#define ARROW_RIGHT             0b01111110
#define ARROW_LEFT              0b01111111
#define JAPANESE_PERIOD         0b10100001
#define JAPANESE_QUOTE_START    0b10100010
#define JAPANESE_QUOTE_END      0b10100011

#include <stdint.h>

extern void waitMs(uint32_t ms);

typedef struct {
    uint8_t RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7;
    uint8_t interface_bits;
} LCDScreen;

extern LCDScreen* configurePins(LCDScreen* screen, uint8_t RS, uint8_t RW, uint8_t E, 
                                uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3,
                                uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);

extern void initScreen(LCDScreen* screen, uint8_t interface_bits, uint8_t num_lines, uint8_t fontType, uint8_t cursor, uint8_t writeDirection);
extern void resetPins(LCDScreen* screen);

extern void sendCommand(LCDScreen* screen, uint8_t command);
extern void sendData(LCDScreen* screen, uint8_t data);
extern void sendText(LCDScreen* screen, const char* text);

#endif // RASPBERRY_LCD_H
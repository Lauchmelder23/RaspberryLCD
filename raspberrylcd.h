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
#define CURRENCY_YEN                    0b01011100
#define ARROW_RIGHT                     0b01111110
#define ARROW_LEFT                      0b01111111
#define JAPANESE_PERIOD                 0b10100001
#define JAPANESE_QUOTE_START            0b10100010
#define JAPANESE_QUOTE_END              0b10100011
#define JAPANESE_COMMA                  0b10100100
#define JAPANESE_INTERPUNCT             0b10100101
#define JAPANESE_KATAKANA_WO            0b10100110
#define JAPANESE_KATAKANA_CHIISAI_A     0b10100111
#define JAPANESE_KATAKANA_CHIISAI_I     0b10101000
#define JAPANESE_KATAKANA_CHIISAI_U     0b10101001
#define JAPANESE_KATAKANA_CHIISAI_E     0b10101010
#define JAPANESE_KATAKANA_CHIISAI_O     0b10101011
#define JAPANESE_KATAKANA_CHIISAI_YA    0b10101100
#define JAPANESE_KATAKANA_CHIISAI_YU    0b10101101
#define JAPANESE_KATAKANA_CHIISAI_YO    0b10101110
#define JAPANESE_KATAKANA_CHIISAI_TSU   0b10101111
#define JAPANESE_CHOONPU                0b10110000
#define JAPANESE_KATAKANA_A             0b10110001
#define JAPANESE_KATAKANA_I             0b10110010
#define JAPANESE_KATAKANA_U             0b10110011
#define JAPANESE_KATAKANA_E             0b10110100
#define JAPANESE_KATAKANA_O             0b10110101
#define JAPANESE_KATAKANA_KA            0b10110110
#define JAPANESE_KATAKANA_KI            0b10110111
#define JAPANESE_KATAKANA_KU            0b10111000
#define JAPANESE_KATAKANA_KE            0b10111001
#define JAPANESE_KATAKANA_KO            0b10111010
#define JAPANESE_KATAKANA_SA            0b10111011
#define JAPANESE_KATAKANA_SHI           0b10111100
#define JAPANESE_KATAKANA_SU            0b10111101
#define JAPANESE_KATAKANA_SE            0b10111110
#define JAPANESE_KATAKANA_SO            0b10111111
#define JAPANESE_KATAKANA_TA            0b11000000
#define JAPANESE_KATAKANA_CHI           0b11000001
#define JAPANESE_KATAKANA_TSU           0b11000010
#define JAPANESE_KATAKANA_TE            0b11000011
#define JAPANESE_KATAKANA_TO            0b11000100
#define JAPANESE_KATAKANA_NA            0b11000101
#define JAPANESE_KATAKANA_NI            0b11000110
#define JAPANESE_KATAKANA_NU            0b11000111
#define JAPANESE_KATAKANA_NE            0b11001000
#define JAPANESE_KATAKANA_NO            0b11001001
#define JAPANESE_KATAKANA_HA            0b11001010
#define JAPANESE_KATAKANA_HI            0b11001011
#define JAPANESE_KATAKANA_FU            0b11001100
#define JAPANESE_KATAKANA_HE            0b11001101
#define JAPANESE_KATAKANA_HO            0b11001110
#define JAPANESE_KATAKANA_MA            0b11001111
#define JAPANESE_KATAKANA_MI            0b11010000
#define JAPANESE_KATAKANA_MU            0b11010001
#define JAPANESE_KATAKANA_ME            0b11010010
#define JAPANESE_KATAKANA_MO            0b11010011
#define JAPANESE_KATAKANA_YA            0b11010100
#define JAPANESE_KATAKANA_YU            0b11010101
#define JAPANESE_KATAKANA_YO            0b11010110
#define JAPANESE_KATAKANA_RA            0b11010111
#define JAPANESE_KATAKANA_RI            0b11011000
#define JAPANESE_KATAKANA_RU            0b11011001
#define JAPANESE_KATAKANA_RE            0b11011010
#define JAPANESE_KATAKANA_RO            0b11011011
#define JAPANESE_KATAKANA_WA            0b11011100
#define JAPANESE_KATAKANA_N             0b11011101
#define JAPANESE_DAKUTEN                0b11011110
#define JAPANESE_HANDAKUTEN             0b11011111
#define MATH_ALPHA                      0b11100000
#define UMLAUT_A                        0b11100001
#define MATH_BETA                       0b11100010
#define MATH_EPSILON                    0b11100011
#define MATH_MU                         0b11100100
#define MATH_SIGMA                      0b11100101
#define MATH_RHO                        0b11100110
#define MATH_G                          0b11100111
#define MATH_SQRT                       0b11101000
#define MATH_RECIPROCAL                 0b11101001
#define MATH_J                          0b11101010
#define MATH_ASTERISK                   0b11101011
#define CURRENCY_CENT                   0b11101100
#define CURRENCY_UNKNOWN                0b11101101
#define TILDE_N                         0b11101110
#define UMLAUT_O                        0b11101111
#define MATH_P                          0b11110000
#define MATH_Q                          0b11110001
#define MATH_THETA                      0b11110010
#define MATH_INF                        0b11110011
#define MATH_OMEGA                      0b11110100
#define UMLAUT_U                        0b11110101
#define MATH_SUM                        0b11110110
#define MATH_PI                         0b11110111
#define MATH_ABS_X                      0b11111000
#define MATH_Y                          0b11111001
#define JAPANESE_KANJI_SEN              0b11111010
#define JAPANESE_KANJI_MAN              0b11111011
#define JAPANESE_KANJI_YEN              0b11111100
#define MATH_DIVISION                   0b11111101
#define CURSOR                          0b11111111

#include <stdint.h>
#include <stdarg.h>

// Just a wrapper for the delay() function from WiringPi
extern void waitMs(uint32_t ms);

// A struct containing the info about pin connections
typedef struct {
    uint8_t RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7;
    uint8_t interface_bits;
} LCDScreen;

// Set the fields of LCDScreen to the according values. The user could also set them directly
// without calling this function
extern LCDScreen* configurePins(LCDScreen* screen, uint8_t RS, uint8_t RW, uint8_t E, 
                                uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3,
                                uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);

// Performs the initialization sequence for the LCD screen
extern void initScreen(LCDScreen* screen, uint8_t interface_bits, uint8_t num_lines, uint8_t fontType, uint8_t cursor, uint8_t writeDirection);

// Pulls every pin low
extern void resetPins(LCDScreen* screen);


// Sends a command to the LCD screen
extern void sendCommand(LCDScreen* screen, uint8_t command);

// Sends a character to the LCD screen
extern void sendData(LCDScreen* screen, uint8_t data);

// Sends a string of text to the LCD screen
extern void sendText(LCDScreen* screen, const char* text);

// Sends a list of chars to the LCD screen
extern void sendChars(LCDScreen* screen, unsigned int len, ...);

// Writes a custom character into CGRAM
extern void loadCustomChar(LCDScreen* screen, uint8_t cgram_addr, ...);


// Clears the screen
#define clearScreen(screen)                                                     sendCommand(screen, SCREEN_CLEAR)
// Sets cursor to (0, 0)
#define returnCursor(screen)                                                    sendCommand(screen, CURSOR_RETURN); 
// Sets the writing direction
#define setWritingDirection(screen, direction)                                  sendCommand(screen, INPUT_SET | direction)
// Turns display on/off and changes cursor behaviour
#define setDisplaySettings(screen, display, cursor_shown, cursor_behaviour)     sendCommand(screen, DISPLAY_SWITCH | display | cursor_shown | cursor_behaviour)
// Moves cursor by one cell to the left or right
#define shiftCursor(screen, direction)                                          sendCommand(screen, SHIFT | CURSOR_SHIFT | direction)
// Shifts screen by one cell to the left or right
#define shiftScreen(screen, direction)                                          sendCommand(screen, SHIFT | DISPLAY_SHIFT | direction)
// Sets the cursor position
#define setCursor(screen, x, y)                                                 sendCommand(screen, DDRAM_AD_SET | (y << 6) | x)

#endif // RASPBERRY_LCD_H
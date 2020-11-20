# RaspberryLCD
A small library to communicate with an LCD screen, intended for use with a RaspberryPi and the WiringPi library.

This library handles the entire communication on the hardware level and supplies the user with high-level bindings for the most common uses. This library also encoded the non-ascii characters supported by LCD screens (Japanese Katakana and mathematical symbols), in order to make using the screen as easy as possible.

## Example
```c
#include "raspberrylcd.h"

int main(int argc, char** argv)
{
    LCDScreen screen;
    configurePins(&screen, 7, 9, 8, 0, 0, 0, 0, 21, 22, 23, 24);

    initScreen(&screen, HALF_BYTE_INTERFACE, TWO_LINES, FONT_5x7, CURSOR_ON | CURSOR_BLINK, LEFT_TO_RIGHT);
    sendText(&screen, "スウェーデン");
    setCursor(&screen, 0, 1);
    sendText(&screen, "オーストラリア");
    resetPins(&screen);

    return 0;
}
```
***
This snippet produces the following output on my LCD screen:

![LCD screen displays non-ascii characters](https://i.imgur.com/zpKTflG.jpg)

## Functions
*All functions take a pointer to `LCDScreen` as their first parameter*

***

```c 
LCDScreen* configurePins(LCDScreen* screen, uint8_t RS, uint8_t RW, uint8_t E, uint8_t D0,uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);
```

This function tells the `LCDScreen` what pins to use to controll the LCD screen. Make sure to check with WiringPi which GPIO pin has which pin number. If you choose to operate your screen in 4-bit mode you can set `D0-D4` to `0`.

***

```c 
void initScreen(LCDScreen* screen, uint8_t interface_bits, uint8_t num_lines, uint8_t fontType, uint8_t cursor, uint8_t writeDirection);
```

This function performs the initialization steps on the LCD screen. Make sure to choose your configuration wisely, as it cannot be changed without re-initializing the screen.

* `interface_bits` controls the amount of bits used to communicate with the screen.
  * 4-bit mode: `HALF_BYTE_INTERFACE`
  * 8-bit mode: `FULL_BYTE_INTERFACE`

* `num_lines` can either be `ONE_LINE` or `TWO_LINES`. If you choose to use the 5x10 font you must set this to `ONE_LINE`.

* `font_type` sets the font type. Can either be `FONT_5x10` or `FONT_5x7`.

* `cursor` configures the cursor behaviour. You can bitwise OR the following
  * `CURSOR_OFF` / `CURSOR_ON`
  * `CURSOR_STATIC` / `CURSOR_BLINK`

* `writeDirection` sets the direction of writing for the display. Either `LEFT_TO_RIGHT` or `RIGHT_TO_LEFT`.

***

```c
void resetPins(LCDScreen* screen);
```
This function pulls all the used pins low. This should be called after program flow reaches the end in order to full shut down the display. This does NOT discard the pin configuration!

***

```c
void sendCommand(LCDScreen* screen, uint8_t command);
```
Sends a command to the LCD screen. If a command takes parameters, you can supply them by bitwise ORing them with the command name:

* `SCREEN_CLEAR` clears the display. Use `clearScreen()` instead (WIP).

* `CURSOR_RETURN` sets the cursor to (0, 0). Use `cursorReturn()` instead (WIP).

* `INPUT_SET` sets the writing direction. Use `setWritingDirection()` instead (WIP).
   * Either `LEFT_TO_RIGHT` or `RIGHT_TO_LEFT`

* `DISPLAY_SWITCH` can turn the display on/off and change the cursor style
   * `DISPLAY_ON` / `DISPLAY_OFF`
   * `CURSOR_ON` / `CURSOR_OFF`
   * `CURSOR_BLINK` / `CURSOR_STATIC`

* `FUNCTION_SET` if you're using this you're using this library wrong

***

```c
void sendData(LCDScreen* screen, uint8_t data);
```
This function sends character data to the screen in order to be displayed. `data` can be an ASCII char (`sendData(&screen, 'H')`), or one of the many macros defined in the header file (`sendData(&screen, CURRENCY_YEN)`). 

***

```c
void sendText(LCDScreen* screen, const char* text);
```
This function can send entire strings to the screen. It even supports the special characters! You can put them directly in the string and the function will handle it.

***

```c
void sendChars(LCDScreen* screen, unsigned int len, ...);
```
This function sends a variable number of characters to the screen. Use `sendText()` instead.

***

```c
void setCursor(LCDScreen* screen, uint8_t x, uint8_t y);
```
This function sets the cursor position on the display.

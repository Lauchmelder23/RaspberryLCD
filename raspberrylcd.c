#include "raspberrylcd.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdarg.h>

void waitMs(uint32_t ms)
{
    delay(ms);
}

LCDScreen* configurePins(LCDScreen* screen, uint8_t RS, uint8_t RW, uint8_t E, 
                        uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3,
                        uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
{
    screen->RS = RS;
    screen->RW = RW;
    screen->E = E;

    screen->D0 = D0;
    screen->D1 = D1;
    screen->D2 = D2;
    screen->D3 = D3;
    screen->D4 = D4;
    screen->D5 = D5;
    screen->D6 = D6;
    screen->D7 = D7;

    wiringPiSetup();
    pinMode(screen->RS, OUTPUT);
    pinMode(screen->RW, OUTPUT);
    pinMode(screen->E, OUTPUT);

    pinMode(screen->D0, OUTPUT);
    pinMode(screen->D1, OUTPUT);
    pinMode(screen->D2, OUTPUT);
    pinMode(screen->D3, OUTPUT);
    pinMode(screen->D4, OUTPUT);
    pinMode(screen->D5, OUTPUT);
    pinMode(screen->D6, OUTPUT);
    pinMode(screen->D7, OUTPUT);

    return screen;
}

void initScreen(LCDScreen* screen, uint8_t interface_bits, uint8_t num_lines, uint8_t fontType, uint8_t cursor, uint8_t writeDirection)
{
    screen->interface_bits = interface_bits;
    sendCommand(screen, (FUNCTION_SET | HALF_BYTE_INTERFACE) >> 4);

    sendCommand(screen, FUNCTION_SET | num_lines | fontType);
    sendCommand(screen, DISPLAY_SWITCH | DISPLAY_ON | cursor);
    sendCommand(screen, SCREEN_CLEAR);
    sendCommand(screen, INPUT_SET | writeDirection);
}

void resetPins(LCDScreen* screen)
{
    digitalWrite(screen->D7, LOW);
    digitalWrite(screen->D6, LOW);
    digitalWrite(screen->D5, LOW);
    digitalWrite(screen->D4, LOW);
    digitalWrite(screen->D3, LOW);
    digitalWrite(screen->D2, LOW);
    digitalWrite(screen->D1, LOW);
    digitalWrite(screen->D0, LOW);

    digitalWrite(screen->E, LOW);
    digitalWrite(screen->RW, LOW);
    digitalWrite(screen->RS, LOW);
}

void helper_send(LCDScreen* screen, uint8_t byte)
{
    digitalWrite(screen->E, HIGH);
    digitalWrite(screen->D7, (byte >> 7) & 0x1);
    digitalWrite(screen->D6, (byte >> 6) & 0x1);
    digitalWrite(screen->D5, (byte >> 5) & 0x1);
    digitalWrite(screen->D4, (byte >> 4) & 0x1);

    if(screen->interface_bits == FULL_BYTE_INTERFACE)
    {
        digitalWrite(screen->D3, (byte >> 3) & 0x1);
        digitalWrite(screen->D2, (byte >> 2) & 0x1);
        digitalWrite(screen->D1, (byte >> 1) & 0x1);
        digitalWrite(screen->D0, (byte >> 0) & 0x1);
    }
    else if(screen->interface_bits == HALF_BYTE_INTERFACE)
    {
        delayMicroseconds(1000);

        digitalWrite(screen->E, LOW);
        delayMicroseconds(10000);

        digitalWrite(screen->E, HIGH);
        digitalWrite(screen->D7, (byte >> 3) & 0x1);
        digitalWrite(screen->D6, (byte >> 2) & 0x1);
        digitalWrite(screen->D5, (byte >> 1) & 0x1);
        digitalWrite(screen->D4, (byte >> 0) & 0x1);        
    }
    
    delayMicroseconds(1000);

    digitalWrite(screen->E, LOW);
    delayMicroseconds(10000);
}

void sendCommand(LCDScreen* screen, uint8_t command)
{
    digitalWrite(screen->RW, LOW);
    digitalWrite(screen->RS, LOW);

    helper_send(screen, command);
}

void sendData(LCDScreen* screen, uint8_t data)
{
    digitalWrite(screen->RW, LOW);
    digitalWrite(screen->RS, HIGH);

    helper_send(screen, data);   
}

void sendText(LCDScreen* screen, const char* text)
{
    for(const char* c = text; *c != '\x00'; c++)
        sendData(screen, *c);
}

void sendChars(LCDScreen* screen, size_t len, ...)
{
    va_list args;
    va_start(args, len);

    for(size_t i = 0; i < len; i++)
        sendData(screen, (uint8_t)va_arg(args, int));

    va_end(args);
}
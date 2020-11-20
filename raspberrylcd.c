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
    // Simply set all the fields to the supplied pins
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

    // Initialize the pins to be output
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
    // Set interface bits of screen
    screen->interface_bits = interface_bits;
    
    // Tell LCD screen to operate in intended bit mode
    sendCommand(screen, (FUNCTION_SET | interface_bits) >> 4);

    // Set number of display lines and font type
    sendCommand(screen, FUNCTION_SET | num_lines | fontType);
    // Turn display on and set cursor
    sendCommand(screen, DISPLAY_SWITCH | DISPLAY_ON | cursor);
    // Clear screen
    sendCommand(screen, SCREEN_CLEAR);
    // Set write direction
    sendCommand(screen, INPUT_SET | writeDirection);
}

void resetPins(LCDScreen* screen)
{
    // Pull all pins low
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

// A helper function to send bytes to the screen
// This function is not visible to the user
void helper_send(LCDScreen* screen, uint8_t byte)
{
    // Pull Enable high
    digitalWrite(screen->E, HIGH);

    // Write high nibble
    digitalWrite(screen->D7, (byte >> 7) & 0x1);
    digitalWrite(screen->D6, (byte >> 6) & 0x1);
    digitalWrite(screen->D5, (byte >> 5) & 0x1);
    digitalWrite(screen->D4, (byte >> 4) & 0x1);

    // If we're in 8-bit mode, write the lower nibbles as well
    if(screen->interface_bits == FULL_BYTE_INTERFACE)
    {
        digitalWrite(screen->D3, (byte >> 3) & 0x1);
        digitalWrite(screen->D2, (byte >> 2) & 0x1);
        digitalWrite(screen->D1, (byte >> 1) & 0x1);
        digitalWrite(screen->D0, (byte >> 0) & 0x1);
    }
    // If in 4-bit, then send the high nibbles and then set the lower nibbles
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
    
    // Send the data supplied on the pins
    delayMicroseconds(1000);

    digitalWrite(screen->E, LOW);
    delayMicroseconds(10000);
}

// Sends a command by pulling RW and RS low
void sendCommand(LCDScreen* screen, uint8_t command)
{
    digitalWrite(screen->RW, LOW);
    digitalWrite(screen->RS, LOW);

    helper_send(screen, command);
}

// Sends a character by pulling only RW low
void sendData(LCDScreen* screen, uint8_t data)
{
    digitalWrite(screen->RW, LOW);
    digitalWrite(screen->RS, HIGH);

    helper_send(screen, data);   
}

// Loops over a char* and sends each character individually. Supports the katakana and math as well
void sendText(LCDScreen* screen, const char* text)
{
    for(const char* c = text; *c != '\x00'; c++)
    {
        if(*c == '\xe3')
        {
            uint16_t symbol = ((uint16_t)(*(c + 1)) << 8) | ((uint16_t)(*(c + 2)));

            switch(symbol)
            {
                /*　*/ case 0x8080: sendData(screen, ' '); break;
                /*、*/ case 0x8081: sendData(screen, JAPANESE_COMMA); break;
                /*。*/ case 0x8082: sendData(screen, JAPANESE_PERIOD); break;
                /*「*/ case 0x808C: sendData(screen, JAPANESE_QUOTE_START); break;
                /*」*/ case 0x808D: sendData(screen, JAPANESE_QUOTE_END); break;
                /*ァ*/ case 0x82A1: sendData(screen, JAPANESE_KATAKANA_CHIISAI_A); break;
                /*ア*/ case 0x82A2: sendData(screen, JAPANESE_KATAKANA_A); break;
                /*ィ*/ case 0x82A3: sendData(screen, JAPANESE_KATAKANA_CHIISAI_I); break;
                /*イ*/ case 0x82A4: sendData(screen, JAPANESE_KATAKANA_I); break;
                /*ゥ*/ case 0x82A5: sendData(screen, JAPANESE_KATAKANA_CHIISAI_U); break;
                /*ウ*/ case 0x82A6: sendData(screen, JAPANESE_KATAKANA_U); break;
                /*ェ*/ case 0x82A7: sendData(screen, JAPANESE_KATAKANA_CHIISAI_E); break;
                /*エ*/ case 0x82A8: sendData(screen, JAPANESE_KATAKANA_E); break;
                /*ォ*/ case 0x82A9: sendData(screen, JAPANESE_KATAKANA_CHIISAI_O); break;
                /*オ*/ case 0x82AA: sendData(screen, JAPANESE_KATAKANA_O); break;
                /*カ*/ case 0x82AB: sendData(screen, JAPANESE_KATAKANA_KA); break;
                /*ガ*/ case 0x82AC: sendChars(screen, 2, JAPANESE_KATAKANA_KA, JAPANESE_DAKUTEN); break;
                /*キ*/ case 0x82AD: sendData(screen, JAPANESE_KATAKANA_KI); break;
                /*ギ*/ case 0x82AE: sendChars(screen, 2, JAPANESE_KATAKANA_KI, JAPANESE_DAKUTEN); break;
                /*ク*/ case 0x82AF: sendData(screen, JAPANESE_KATAKANA_KU); break;
                /*グ*/ case 0x82B0: sendChars(screen, 2, JAPANESE_KATAKANA_KU, JAPANESE_DAKUTEN); break;
                /*ケ*/ case 0x82B1: sendData(screen, JAPANESE_KATAKANA_KE); break;
                /*ゲ*/ case 0x82B2: sendChars(screen, 2, JAPANESE_KATAKANA_KE, JAPANESE_DAKUTEN); break;
                /*コ*/ case 0x82B3: sendData(screen, JAPANESE_KATAKANA_KO); break;
                /*ゴ*/ case 0x82B4: sendChars(screen, 2, JAPANESE_KATAKANA_KO, JAPANESE_DAKUTEN); break;
                /*サ*/ case 0x82B5: sendData(screen, JAPANESE_KATAKANA_SA); break;
                /*ザ*/ case 0x82B6: sendChars(screen, 2, JAPANESE_KATAKANA_SA, JAPANESE_DAKUTEN); break;
                /*シ*/ case 0x82B7: sendData(screen, JAPANESE_KATAKANA_SHI); break;
                /*ジ*/ case 0x82B8: sendChars(screen, 2, JAPANESE_KATAKANA_SHI, JAPANESE_DAKUTEN); break;
                /*ス*/ case 0x82B9: sendData(screen, JAPANESE_KATAKANA_SU); break;
                /*ズ*/ case 0x82BA: sendChars(screen, 2, JAPANESE_KATAKANA_SU, JAPANESE_DAKUTEN); break;
                /*セ*/ case 0x82BB: sendData(screen, JAPANESE_KATAKANA_SE); break;
                /*ゼ*/ case 0x82BC: sendChars(screen, 2, JAPANESE_KATAKANA_SE, JAPANESE_DAKUTEN); break;
                /*ソ*/ case 0x82BD: sendData(screen, JAPANESE_KATAKANA_SO); break;
                /*ゾ*/ case 0x82BE: sendChars(screen, 2, JAPANESE_KATAKANA_SO, JAPANESE_DAKUTEN); break;
                /*タ*/ case 0x82BF: sendData(screen, JAPANESE_KATAKANA_TA); break;
                /*ダ*/ case 0x8380: sendChars(screen, 2, JAPANESE_KATAKANA_TA, JAPANESE_DAKUTEN); break;
                /*チ*/ case 0x8381: sendData(screen, JAPANESE_KATAKANA_CHI); break;
                /*ヂ*/ case 0x8382: sendChars(screen, 2, JAPANESE_KATAKANA_CHI, JAPANESE_DAKUTEN); break;
                /*ッ*/ case 0x8383: sendData(screen, JAPANESE_KATAKANA_CHIISAI_TSU); break;
                /*ツ*/ case 0x8384: sendData(screen, JAPANESE_KATAKANA_TSU); break;
                /*ヅ*/ case 0x8385: sendChars(screen, 2, JAPANESE_KATAKANA_TSU, JAPANESE_DAKUTEN); break;
                /*テ*/ case 0x8386: sendData(screen, JAPANESE_KATAKANA_TE); break;
                /*デ*/ case 0x8387: sendChars(screen, 2, JAPANESE_KATAKANA_TE, JAPANESE_DAKUTEN); break;
                /*ト*/ case 0x8388: sendData(screen, JAPANESE_KATAKANA_TO); break;
                /*ド*/ case 0x8389: sendChars(screen, 2, JAPANESE_KATAKANA_TO, JAPANESE_DAKUTEN); break;
                /*ナ*/ case 0x838A: sendData(screen, JAPANESE_KATAKANA_NA); break;
                /*ニ*/ case 0x838B: sendData(screen, JAPANESE_KATAKANA_NI); break;
                /*ヌ*/ case 0x838C: sendData(screen, JAPANESE_KATAKANA_NU); break;
                /*ネ*/ case 0x838D: sendData(screen, JAPANESE_KATAKANA_NE); break;
                /*ノ*/ case 0x838E: sendData(screen, JAPANESE_KATAKANA_NO); break;
                /*ハ*/ case 0x838F: sendData(screen, JAPANESE_KATAKANA_HA); break;
                /*バ*/ case 0x8390: sendChars(screen, 2, JAPANESE_KATAKANA_HA, JAPANESE_DAKUTEN); break;
                /*パ*/ case 0x8391: sendChars(screen, 2, JAPANESE_KATAKANA_HA, JAPANESE_HANDAKUTEN); break;
                /*ヒ*/ case 0x8392: sendData(screen, JAPANESE_KATAKANA_HI); break;
                /*ビ*/ case 0x8393: sendChars(screen, 2, JAPANESE_KATAKANA_HI, JAPANESE_DAKUTEN); break;
                /*ピ*/ case 0x8394: sendChars(screen, 2, JAPANESE_KATAKANA_HI, JAPANESE_HANDAKUTEN); break;
                /*フ*/ case 0x8395: sendData(screen, JAPANESE_KATAKANA_FU); break;
                /*ブ*/ case 0x8396: sendChars(screen, 2, JAPANESE_KATAKANA_FU, JAPANESE_DAKUTEN); break;
                /*プ*/ case 0x8397: sendChars(screen, 2, JAPANESE_KATAKANA_FU, JAPANESE_HANDAKUTEN); break;
                /*ヘ*/ case 0x8398: sendData(screen, JAPANESE_KATAKANA_HE); break;
                /*ベ*/ case 0x8399: sendChars(screen, 2, JAPANESE_KATAKANA_HE, JAPANESE_DAKUTEN); break;
                /*ペ*/ case 0x839A: sendChars(screen, 2, JAPANESE_KATAKANA_HE, JAPANESE_HANDAKUTEN); break;
                /*ホ*/ case 0x839B: sendData(screen, JAPANESE_KATAKANA_HO); break;
                /*ボ*/ case 0x839C: sendChars(screen, 2, JAPANESE_KATAKANA_HO, JAPANESE_DAKUTEN); break;
                /*ポ*/ case 0x839D: sendChars(screen, 2, JAPANESE_KATAKANA_HO, JAPANESE_HANDAKUTEN); break;
                /*マ*/ case 0x839E: sendData(screen, JAPANESE_KATAKANA_MA); break;
                /*ミ*/ case 0x839F: sendData(screen, JAPANESE_KATAKANA_MI); break;
                /*ム*/ case 0x83A0: sendData(screen, JAPANESE_KATAKANA_MU); break;
                /*メ*/ case 0x83A1: sendData(screen, JAPANESE_KATAKANA_ME); break;
                /*モ*/ case 0x83A2: sendData(screen, JAPANESE_KATAKANA_MO); break;
                /*ャ*/ case 0x83A3: sendData(screen, JAPANESE_KATAKANA_CHIISAI_YA); break;
                /*ヤ*/ case 0x83A4: sendData(screen, JAPANESE_KATAKANA_YA); break;
                /*ュ*/ case 0x83A5: sendData(screen, JAPANESE_KATAKANA_CHIISAI_YU); break;
                /*ユ*/ case 0x83A6: sendData(screen, JAPANESE_KATAKANA_YU); break;
                /*ョ*/ case 0x83A7: sendData(screen, JAPANESE_KATAKANA_CHIISAI_YO); break;
                /*ヨ*/ case 0x83A8: sendData(screen, JAPANESE_KATAKANA_YO); break;
                /*ラ*/ case 0x83A9: sendData(screen, JAPANESE_KATAKANA_RA); break;
                /*リ*/ case 0x83AA: sendData(screen, JAPANESE_KATAKANA_RI); break;
                /*ル*/ case 0x83AB: sendData(screen, JAPANESE_KATAKANA_RU); break;
                /*レ*/ case 0x83AC: sendData(screen, JAPANESE_KATAKANA_RE); break;
                /*ロ*/ case 0x83AD: sendData(screen, JAPANESE_KATAKANA_RO); break;
                /*ワ*/ case 0x83AF: sendData(screen, JAPANESE_KATAKANA_WA); break;
                /*ヲ*/ case 0x83B2: sendData(screen, JAPANESE_KATAKANA_WO); break;
                /*ン*/ case 0x83B3: sendData(screen, JAPANESE_KATAKANA_N); break;
                /*ヴ*/ case 0x83B4: sendChars(screen, 2, JAPANESE_KATAKANA_U, JAPANESE_DAKUTEN); break;
                /*ヴ*/ case 0x83B5: sendChars(screen, 2, JAPANESE_KATAKANA_WA, JAPANESE_DAKUTEN); break;
                /*ヺ*/ case 0x83BA: sendChars(screen, 2, JAPANESE_KATAKANA_WO, JAPANESE_DAKUTEN); break;
                /*・*/ case 0x83BB: sendData(screen, JAPANESE_INTERPUNCT); break;
                /*ー*/ case 0x83BC: sendData(screen, JAPANESE_CHOONPU); break;

                default: printf("Unknown char: %x\n", symbol); sendData(screen, CURSOR); break;
            }
            c += 2;
        }
        else
        {
            sendData(screen, *c);
        }
        
    }
}

// Sends a list of chars, similar to sendText
void sendChars(LCDScreen* screen, size_t len, ...)
{
    va_list args;
    va_start(args, len);

    for(size_t i = 0; i < len; i++)
        sendData(screen, (uint8_t)va_arg(args, int));

    va_end(args);
}

// Loads a custom character into CGRAM
void loadCustomChar(LCDScreen* screen, uint8_t cgram_addr, ...)
{
    // Initialize va_args
    va_list args;
    va_start(args, cgram_addr);

    // Choose correct scram address
    sendCommand(screen, CGRAM_AD_SET | (cgram_addr << 3));
    // Write data to RAM
    for(int i = 0; i < 8; i++)
        sendData(screen, va_arg(args, int));

    // Free va_args
    va_end(args);
}
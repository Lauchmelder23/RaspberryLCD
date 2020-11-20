#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "raspberrylcd.h"

int done = 0;

void* await_user_input(void* args)
{
   char c = ' ';
   while((c != 'q') && (c != 'Q')) c = getchar();
   done = 1;
   return args;
}

int main(int argc, char** argv)
{
    LCDScreen* screen = (LCDScreen*)malloc(sizeof(LCDScreen));
    configurePins(screen, 7, 9, 8, 0, 0, 0, 0, 21, 22, 23, 24);

    initScreen(screen, HALF_BYTE_INTERFACE, TWO_LINES, FONT_5x7, CURSOR_ON | CURSOR_BLINK, LEFT_TO_RIGHT);

    loadCustomChar(screen, 0, 0b00000, 0b00000, 0b11111, 0b00000, 0b00000, 0b11111, 0b00000, 0b00000);
    loadCustomChar(screen, 1, 0b00000, 0b00000, 0b11111, 0b10000, 0b10000, 0b10011, 0b10010, 0b10010);
    loadCustomChar(screen, 2, 0b00000, 0b00000, 0b11111, 0b00001, 0b00001, 0b11001, 0b01001, 0b01001);
    loadCustomChar(screen, 3, 0b10010, 0b10010, 0b10011, 0b10000, 0b10000, 0b11111, 0b00000, 0b00000);
    loadCustomChar(screen, 4, 0b01001, 0b01001, 0b11001, 0b00001, 0b00001, 0b11111, 0b00000, 0b00000);

    returnCursor(screen);
    sendChars(screen, 1, 1);
    setCursor(screen, 15, 0);
    sendChars(screen, 1, 2);
    setCursor(screen, 0, 1);
    sendChars(screen, 4, 3, 0, 0, 0);
    setCursor(screen, 12, 1);
    sendChars(screen, 4, 0, 0, 0, 4);

    pthread_t thread;
    pthread_create(&thread, NULL, await_user_input, NULL);

    setenv("TZ", "GMT-1", 1);
    tzset();

    time_t rawtime;
    struct tm* info;
    char top_line[15], bottom_line[9];
    while(!done)
    {
        time(&rawtime);
        info = localtime(&rawtime);

        strftime(top_line, 15, "%a %d.%m.%Y", info);
        strftime(bottom_line, 9, "%H:%M:%S", info);

        setCursor(screen, 1, 0);
        sendText(screen, top_line);
        setCursor(screen, 4, 1);
        sendText(screen, bottom_line);
    }

    pthread_join(thread, NULL);
    resetPins(screen);
    free(screen);
    return 0;
}

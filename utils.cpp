#include <stdio.h>
#include <windows.h>
#include "utils.h"

void moveCursorTo(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Function to change terminal color
void change_colour(int a) {
    printf("\033[48;5;%dm", a + 232);
}

void reset_colour() {
    printf("\033[0m");
}

void fullscreen() {
	keybd_event(VK_MENU,0x38,0,0);
	keybd_event(VK_RETURN,0x1c,0,0);
	keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}
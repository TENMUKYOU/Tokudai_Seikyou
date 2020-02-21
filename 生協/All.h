// #pragma warning(disable: 4996)
#define SDL_MAIN_HANDLED //SDL_main.hの中にmain関数があるから定義しないといけないらしい．．．知るかよ！！！！

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <string>
#include <iostream>
#include"felicalib.h"

#define WINDOW_WIDTH 2000
#define WINDOW_HEIGHT 1300

namespace in{
extern int mouseX;
extern int mouseY;
extern bool mouseClick;
}
extern uint8_t screen;
extern bool push_button;
extern bool break_time;
extern bool push_complete_button;
extern bool complete_preparation;
extern int mode;
extern time_t t;
extern SDL_Window* window;
extern SDL_Renderer *renderer;

/********************************   FeliCa.cpp   ********************************/

void setFileName();
void FeliCa_INIT();
void initSystem();
void FeliCa();
void FeliCa_Polling();
void Out_File();
int setBefore9Hours(int time);
void Event();
void initWindow();
void display();
void load();

// #pragma warning(disable: 4996)
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include"felicalib.h"

pasori* p;
felica* f, * f2;
FILE* fp;
time_t t;

/********************************   FeliCa.c   ********************************/


void FeliCa_INIT();
void FeliCa();
void Out_File();
void File_INIT();
void FeliCa_Polling();


/**********************************   SDL.c   **********************************/


void UIInit();
void Event();
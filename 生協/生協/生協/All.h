#pragma once

#include <stdio.h>
#include "felicalib.h"

#define File_Name "Sample.csv"

pasori* p;
felica* f, * f2;
int i, j, k;

FILE* fp;
char* fname = File_Name;
char* s1 = "ID";
char* s2 = "出勤時間";

time_t t = time(NULL);


int Touch_Flag;

void SDL_INIT();
void FeliCa_INIT();
void FeliCa();
void Out_File();
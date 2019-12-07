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
<<<<<<< HEAD
localtime(&t);



//int Touch_Flag;
=======


int Touch_Flag;
>>>>>>> f252629ae3cf3f32d5dbdc1ba8eda59738d6d283

void SDL_INIT();
void FeliCa_INIT();
void FeliCa();
void Out_File();
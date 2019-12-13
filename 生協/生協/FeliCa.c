/*
  felicalib - FeliCa access wrapper library

  Copyright (c) 2007, Takuya Murakami, All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. Neither the name of the project nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**
   @file dump.c

   FeliCa �_���v
*/
#pragma warning(disable: 4996)

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "felicalib.h"
#include "All.h"

#define ID_Data 20683
#define File_Name "Sample.csv"

void Out_File();

int id = 0;
int i = 0;
char* s1 = "ID";
char* s2 = "ATT_TIME";
char* fname = File_Name;


void FeliCa_INIT() {
	/*FeliCa*/
	p = pasori_open(NULL);

	if (!p) {
		fprintf(stderr, "PaSoRi open failed.\n");
		exit(1);
	}

	pasori_init(p);

	f = felica_polling(p, POLLING_ANY, 0, 0);

	if (!f) {
		//fprintf(stderr, "Polling card failed.\n");
	}

	//Touch_Flag = 1;

	f = felica_enum_systemcode(p);
	if (!f) {
		exit(1);
	}


}
struct  Member {
	char ID[256];
	int Att_Time; //出勤時間
	int Work_Time; //退勤時間
};

struct Member Member[256];
void FeliCa() {
	f2 = felica_enum_service(p, N2HS(f->system_code[1]));
	if (!f2) {
		fprintf(stderr, "Enum service failed.\n");
		exit(1);
	}

	uint16 service = f2->service_code[13];
	uint8 data[16];
	int k = 0;
	if (felica_read_without_encryption02(f2, service, 0, (uint8)k, data)) {
		printf("Error");
	}
	snprintf(Member[id].ID, sizeof(Member[id].ID), "%x%x%02x%02x%02x%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
	Member[id].Att_Time = ((int)t / 3600 + 9) % 24;
	Member[id].Work_Time = t / 60 % 60;
	printf("%s %02d:%02d\n", Member[id].ID, Member[id].Att_Time, Member[id].Work_Time);
	id++;
	printf("\n");
	felica_free(f2);
}

void Out_File() {
	int Com_Flag = 0;
	fp = fopen(fname, "a");
	while (i <= id) {
		fprintf(fp, "%s, %02d:%02d\n", Member[i].ID, Member[i].Att_Time, Member[i].Work_Time);
		i++;
	}
	fclose(fp);

}

void File_INIT() {
	fp = fopen(fname, "w");
	if (!fp) {
		fprintf(stderr, "Can't Open File.\n");
	}
	fprintf(fp, "%s, %s\n", s1, s2);
	fclose(fp);
}
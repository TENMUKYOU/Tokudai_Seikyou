// #pragma warning(disable: 4996)

#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "felicalib.h"
#include "All.h"

#define ID_Data 20683
#define File_Name "Sample.csv"

int user_num = 0;
char* s1 = "ID";
char* s2 = "ATT_TIME";
char* fname = File_Name;

struct  Member {
	char ID[13];
	int Att_Time; //出勤時間
	int Work_Time; //退勤時間
};

struct Member Member[256];

void FeliCa_INIT() {
	/*FeliCa*/
	if ((p = pasori_open(NULL)) == NULL) {
		fprintf(stderr, "PaSoRi open failed.\n");
		exit(1);
	}

	pasori_init(p);
}

void FeliCa_Polling() {
	static bool Touch_Flag = false;

	if ((f = felica_polling(p, POLLING_ANY, 0, 0)) == NULL) {
		// fprintf(stderr, "Polling card failed.\n");
		Touch_Flag = false;
	}else{
		if(!Touch_Flag){
		f = felica_enum_systemcode(p);
		if ((f = felica_enum_systemcode(p)) == NULL) {
			exit(1);
		}
		FeliCa();
		}
		Touch_Flag = true;
	}
}

void FeliCa() {
	uint8 data[16];
	int k = 0;

	if ((f2 = felica_enum_service(p, N2HS(f->system_code[1]))) == NULL) {
		fprintf(stderr, "Enum service failed.\n");
		exit(1);
	}

	uint16 service = f2->service_code[13];

	if (felica_read_without_encryption02(f2, service, 0, (uint8)k, data)) {
		printf("Error");
	}

	snprintf(Member[user_num].ID, sizeof(Member[user_num].ID), "%x%x%02x%02x%02x%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
	Member[user_num].Att_Time = ((int)t / 3600 + 9) % 24;
	Member[user_num].Work_Time = t / 60 % 60;
	printf("%s %02d:%02d\n\n", Member[user_num].ID, Member[user_num].Att_Time, Member[user_num].Work_Time);
	
	user_num++;

	felica_free(f2);
}

void Out_File() {
	char univ_code[] = "6879";

	if ((fp = fopen(fname, "a")) == NULL) {
		fprintf(stderr, "Can't Open File.\n");
	}

	for(int i=0;i <= user_num;i++) {
		if(strncmp(Member[i].ID, univ_code, 4)==0){
			fprintf(fp, "%s, %02d:%02d\n", Member[i].ID, Member[i].Att_Time, Member[i].Work_Time);
		}
	}

	fclose(fp);
}

void File_INIT() {
	if ((fp = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "Can't Open File.\n");
	}
	fprintf(fp, "%s, %s\n", s1, s2);
	fclose(fp);
}
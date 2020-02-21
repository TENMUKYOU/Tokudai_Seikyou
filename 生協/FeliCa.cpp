// #pragma warning(disable: 4996)
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "All.h"
#include "felicalib.h"
#include "Text.h"
#include "image.h"

std::string filename;
pasori* p;
felica* f, * f2;
FILE* fp;
time_t t;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface *surface;
SDL_Texture *texture;
SDL_Rect dst, src;
TTF_Font* font;
std::string bufstr;
class Text *nameEx, *name, *nowTimeEx, *nowTime, *Touch, *Touch_recode, *NAME_label, *WS_label, *WE_label, *NAME_recode, *WS_recode, *WE_recode, *check_Break_Time, *check_Break_Time2, *Error_push_button;
class Img *WorkStartButton, *RestStartButton, *RestEndButton, *WorkEndButton,*CSVbutton, *HaveRestButton, *NotHaveRestButton, *COMPLETEButton;
bool Touch_Flag = false;
int mode = 0;
bool WorkEnd = false;

bool push_complete_button = false;
bool complete_preparation = false;
bool break_time = false;
bool push_button = false;
long long int tmp_ID = 123412341234;

namespace in{
int mouseX;
int mouseY;
bool mouseClick;
}

uint8_t screen = 0;

typedef struct{
	// long long int ID;	// 組合員番号
	std::string Name;		// 氏名
	int Work_Start_Time;	// 勤務開始時間
	// int Rest_Start_Time;	// 休憩開始時間
	// int Rest_End_Time;		// 休憩終了時間
	int Work_End_Time;		// 勤務終了時間
	std::uint8_t Break_Time;// 休憩時間
}Member_d;

std::vector<Member_d> Member_List;
std::unordered_map<long long, std::string> Member_Name;

void setFileName(){
	int year, month, day;
	tm *local;

	t = time(NULL);
	local = localtime(&t);

	year = local->tm_year + 1900;
    month = local->tm_mon + 1;
    day = local->tm_mday;

	filename = std::to_string(year);
	if(month < 10){
		filename += "0";
	}

	filename += std::to_string(month);

	if(day < 10){
		filename += "0";
	}

	filename += std::to_string(day) + "新サポ出退勤リスト.csv";
}

void FeliCa_INIT() {
	if ((p = pasori_open(NULL)) == NULL) {
		fprintf(stderr, "PaSoRi open failed.\n");
		exit(1);
	}
	pasori_init(p);
}

void initSystem() {
	int i = 0;
	int buf1, buf2, buf3, buf4, buf5, buf6, buf7, buf8;
	long long bufferID;
	char a[20];

	char *ptr1, *ptr2, *ptr3, *ptr4;

	int count = 0;
	char buf10[10];
	std::string nn;
	std::string space = " ";
	

	std::string bufferNAME;
	std::string buf;
	Member_d Member = {"NoMember", 0, 0, 0};

	Member_List.reserve(10);

	FeliCa_INIT();
	setFileName();
	initWindow();
	load();

	if ((fp = fopen("AllMemberListID.csv", "r")) == NULL) {
		printf("Can't Open File.\n");
	}else{
		puts("ID list loading");
		while(fscanf(fp, "%lld, %s", &bufferID, a) != EOF && !ferror(fp)){
			bufferNAME = a;
			Member_Name[bufferID] = bufferNAME;
		}
		puts("Complete ID list loading");
	}
	fclose(fp);

#pragma region std::getline model
	std::ifstream ifs(filename,std::ios_base::in);

	if(!ifs){
		puts("file read error");
	}

	std::string linestring;
    while (std::getline(ifs, linestring, ',')) {
		switch(count){
		case 0:
			std::cout << linestring << std::endl;
			linestring.erase(0,0);
			Member.Name = linestring;
			break;
		case 1:
			linestring.copy(buf10, 7);
			if(linestring == space){
				buf1 = 0;
				buf5 = 0;
			}else{
				ptr1 = strtok(buf10, ":");
				buf1 = atoi(ptr1);
				ptr1 = strtok(NULL, ",");
				buf5 = atoi(ptr1);
			}
			break;
		case 2:
			linestring.copy(buf10, 7);
			if(linestring == space){
				buf4 = 0;
				buf8 = 0;
			}else{
				ptr4 = strtok(buf10, ":");
				buf4 = atoi(ptr4);
				ptr4 = strtok(NULL, "\n");
				buf8 = atoi(ptr4);
			}
			break;
		case 3:
			if(linestring == space){
				Member.Break_Time = 0;
			}else{
				Member.Break_Time = stoi(linestring);
			}
			ifs.ignore();
			break;
		}
		count++;
		if(count == 4){
			count = 0;
			if(buf1 != 0){
				buf1 = setBefore9Hours(buf1);
			}
			// if(buf2 != 0){
				// buf2 = setBefore9Hours(buf2);
			// }
			// if(buf3 != 0){
				// buf3 = setBefore9Hours(buf3);
			// }
			if(buf4 != 0){
				buf4 = setBefore9Hours(buf4);
			}
			Member.Work_Start_Time = (buf1 * 3600) + (buf5 * 60);
			// Member.Rest_Start_Time = (buf2 * 3600) + (buf6 * 60);
			// Member.Rest_End_Time = (buf3 * 3600) + (buf7 * 60);
			Member.Work_End_Time = (buf4 * 3600) + (buf8 * 60);
			Member_List.emplace_back(Member);
			// printf("%lld, %s, %d:%d, %d:%d, %d:%d, %d:%d\n", n, nn.c_str(), buf1, buf5, buf2, buf6, buf3, buf7, buf4, buf8);
			printf("%s, %02d:%02d, %02d:%02d, %d\n", Member_List[i].Name.c_str()
					, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
					, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60
					, (int)Member_List[i].Break_Time);
			printf("i = %d\n", i);
			i++;
		}
	}
	ifs.close();
	printf("Member size = %d", Member_List.size());
#pragma endregion
#pragma region fscaf model
	/*
	if ((fp = fopen(fname, "r")) == NULL) {
		printf("Can't Open File.\n");
	}else{
		puts("read");
		while(fscanf(fp, "%lld, %[^,], %d:%d, %d:%d, %d:%d, %d:%d", &Member.ID, a , &buf1, &buf5, &buf2, &buf6, &buf3, &buf7, &buf4, &buf8) != EOF && !ferror(fp)){
*/
/*
			if( (strcmp(bufstr1, " ")) == 0 ){
				buf1 = 0;
				buf5 = 0;
			}else{
				ptr = strtok(str, ":");
				buf1 = atoi(ptr);
				buf1 = setBefore9Hours(buf1);
				ptr = strtok(NULL, ",");
				buf5 = atoi(ptr);
			}

			if( (strcmp(bufstr2, " ")) == 0 ){
				buf2 = 0;
				buf6 = 0;
			}else{
				ptr = strtok(str, ":");
				buf2 = atoi(ptr);
				buf2 = setBefore9Hours(buf2);
				ptr = strtok(NULL, ",");
				buf6 = atoi(ptr);
			}

			if( (strcmp(bufstr3, " ")) == 0 ){
				buf3 = 0;
				buf7 = 0;
			}else{
				ptr = strtok(str, ":");
				buf3 = atoi(ptr);
				buf3 = setBefore9Hours(buf3);
				ptr = strtok(NULL, ",");
				buf7 = atoi(ptr);
			}

			if( (strcmp(bufstr4, " ")) == 0 ){
				buf4 = 0;
				buf8 = 0;
			}else{
				ptr = strtok(str, ":");
				buf4 = atoi(ptr);
				buf4 = setBefore9Hours(buf4);
				ptr = strtok(NULL, ",");
				buf8 = atoi(ptr);
			}
*/
/*
			if(buf1 != 99){
				buf1 = setBefore9Hours(buf1);
			}else if(buf1 == 99){
				buf1 = 0;
				buf5 = 0;
			}

			if(buf2 != 99){
				buf2 = setBefore9Hours(buf2);
			}else if(buf2 == 99){
				buf2 = 0;
				buf6 = 0;
			}

			if(buf3 != 99){
				buf3 = setBefore9Hours(buf3);
			}else if(buf3 == 99){
				buf3 = 0;
				buf7 = 0;
			}

			if(buf4 != 99){
				buf4 = setBefore9Hours(buf4);
			}else if(buf4 == 99){
				buf4 = 0;
				buf8 = 0;
			}
*/
			// if(buf1 == 99){ // 0
				// if(buf2 == 99){ // 0 0
					// if(buf3 == 99){ // 0 0 0
						// if(buf4 == 99){ // 0 0 0 0
							// buf1 = 0;
							// buf2 = 0;
							// buf3 = 0;
							// buf4 = 0;
							// buf5 = 0;
							// buf6 = 0;
							// buf7 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 0 0 0 1
							// buf1 = 0;
							// buf2 = 0;
							// buf3 = 0;
							// buf4 = setBefore9Hours(buf4);
							// buf5 = 0;
							// buf6 = 0;
							// buf7 = 0;
						// }
					// }else if(buf3 != 99){ // 0 0 1
						// if(buf4 == 99){ // 0 0 1 0
							// buf1 = 0;
							// buf2 = 0;
							// buf3 = setBefore9Hours(buf3);
							// buf4 = 0;
							// buf5 = 0;
							// buf6 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 0 0 1 1
							// buf1 = 0;
							// buf2 = 0;
							// buf3 = setBefore9Hours(buf3);
							// buf4 = setBefore9Hours(buf4);
							// buf5 = 0;
							// buf6 = 0;
						// }
					// }
				// }else if(buf2 != 99){ // 0 1
					// if(buf3 == 99){ // 0 1 0
						// if(buf4 == 99){ // 0 1 0 0
							// buf1 = 0;
							// buf2 = setBefore9Hours(buf2);
							// buf3 = 0;
							// buf4 = 0;
							// buf5 = 0;
							// buf7 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 0 1 0 1
							// buf1 = 0;
							// buf2 = setBefore9Hours(buf2);
							// buf3 = 0;
							// buf4 = setBefore9Hours(buf4);
							// buf5 = 0;
							// buf7 = 0;
						// }
					// }else if(buf3 != 99){ // 0 1 1
						// if(buf4 == 99){ // 0 1 1 0
							// buf1 = 0;
							// buf2 = setBefore9Hours(buf2);
							// buf3 = setBefore9Hours(buf3);
							// buf4 = 0;
							// buf5 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 0 1 1 1
							// buf1 = 0;
							// buf2 = setBefore9Hours(buf2);
							// buf3 = setBefore9Hours(buf3);
							// buf4 = setBefore9Hours(buf4);
							// buf5 = 0;
						// }
					// }
				// }
			// }else if(buf1 != 99){ // 1
				// if(buf2 == 99){ // 1 0
					// if(buf3 == 99){ // 1 0 0
						// if(buf4 == 99){ // 1 0 0 0
							// buf1 = setBefore9Hours(buf1);
							// buf2 = 0;
							// buf3 = 0;
							// buf4 = 0;
							// buf6 = 0;
							// buf7 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 1 0 0 1
							// buf1 = setBefore9Hours(buf1);
							// buf2 = 0;
							// buf3 = 0;
							// buf4 = setBefore9Hours(buf4);
							// buf6 = 0;
							// buf7 = 0;
						// }
					// }else if(buf3 != 99){ // 1 0 1
						// if(buf4 == 99){ // 1 0 1 0
							// buf1 = setBefore9Hours(buf1);
							// buf2 = 0;
							// buf3 = setBefore9Hours(buf3);
							// buf4 = 0;
							// buf6 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 1 0 1 1
							// buf1 = setBefore9Hours(buf1);
							// buf2 = 0;
							// buf3 = setBefore9Hours(buf3);
							// buf4 = setBefore9Hours(buf4);
							// buf6 = 0;
						// }
					// }
				// }else if(buf2 != 99){ // 1 1
					// if(buf3 == 99){ // 1 1 0
						// if(buf4 == 99){ // 1 1 0 0
							// buf1 = setBefore9Hours(buf1);
							// buf2 = setBefore9Hours(buf2);
							// buf3 = 0;
							// buf4 = 0;
							// buf7 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 1 1 0 1
							// buf1 = setBefore9Hours(buf1);
							// buf2 = setBefore9Hours(buf2);
							// buf3 = 0;
							// buf4 = setBefore9Hours(buf4);
							// buf7 = 0;
						// }
					// }else if(buf3 != 99){ // 1 1 1
						// if(buf4 == 99){ // 1 1 1 0
							// buf1 = setBefore9Hours(buf1);
							// buf2 = setBefore9Hours(buf2);
							// buf3 = setBefore9Hours(buf3);
							// buf4 = 0;
							// buf8 = 0;
						// }else if(buf4 != 99){ // 1 1 1 1
							// buf1 = setBefore9Hours(buf1);
							// buf2 = setBefore9Hours(buf2);
							// buf3 = setBefore9Hours(buf3);
							// buf4 = setBefore9Hours(buf4);
						// }
					// }
				// }
			// }
/*
			Member.Name = a;
			Member.Work_Start_Time = (buf1 * 3600) + (buf5 * 60);
			Member.Rest_Start_Time = (buf2 * 3600) + (buf6 * 60);
			Member.Rest_End_Time = (buf3 * 3600) + (buf7 * 60);
			Member.Work_End_Time = (buf4 * 3600) + (buf8 * 60);
			Member_List.emplace_back(Member);

			printf("%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
					, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
					, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
					, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
					, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
			i++;
		}
	}
	fclose(fp);
	*/
#pragma endregion
}


void FeliCa_Polling() {
	if ((f = felica_polling(p, POLLING_ANY, 0, 0)) == NULL) {
		// fprintf(stderr, "Polling card failed.\n");
		Touch_Flag = false;
	}else{
		if(!Touch_Flag){
			// printf("Touch Felica\n");
			//f = felica_enum_systemcode(p);
			if ((f = felica_enum_systemcode(p)) == NULL) {
				printf("failed enum_systemcode\n");
				exit(1);
			}
			FeliCa();
		}
		Touch_Flag = true;
	}
}

void FeliCa() {
	uint8 data[16];
	Member_d Member = {"NoMember", 0, 0, 0};
	long long int ID;
	char buf[20];
	bool hasID = false;

	if ((f2 = felica_enum_service(p, N2HS(f->system_code[1]))) == NULL) {
		fprintf(stderr, "Enum service failed.\n");
		exit(1);
	}

	if (felica_read_without_encryption02(f2, 20683, 0, 0, data)) {
		printf("Error");
	}

	sprintf(buf, "%02x%02x%02x%02x%02x%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
	ID = atoll(buf);
	tmp_ID = ID;
	if(688000000000 > ID && ID > 687900000000){
		bufstr = buf;
		// ID->endText();
		// ID->setText(bufstr, 150, 350, 0.6);
		// ID_recode->endText();
		// ID_recode->setText(bufstr, 400, 500, 0.4, true);
		// ID_recode->setPosC(350, 950);
		Member.Name = Member_Name.at(ID);
		name->endText();
		name->setText(Member.Name, 750, 350, 0.6, true);
	}else{
		bufstr = "error";    ////////////////////////////////////////////////////////////////////////////
		// ID->endText();
		// ID->setText(bufstr, 150, 350, 0.6);
		// ID_recode->endText();
		// ID_recode->setText(bufstr, 400, 500, 0.4, true);
		// ID_recode->setPosC(350, 950);
		name->endText();
		name->setText(bufstr, 750, 350, 0.6, true);
	}

	int hours = (t /3600 + 9) % 24;
	int minutes = (t / 60) % 60;
	int seconds = t % 60;

	// 時間の文字列作成
	if(hours < 10){
		bufstr = "0";
		bufstr += std::to_string(hours);
	}else{
		bufstr = std::to_string(hours);
	}
	bufstr += ":";
	if(minutes < 10){
		bufstr += "0";
	}
	bufstr += std::to_string(minutes);
	bufstr += ":";
	if(seconds < 10){
		bufstr += "0";
	}
	bufstr += std::to_string(seconds);
	nowTime->endText();
	nowTime->setText(bufstr, 1500, 350, 0.6);

	printf("%d\n", mode);
	for(int i = 0;i < Member_List.size();i++){
		puts("tansaku");
		printf("Member_List.Name = %s\n", Member_List[i].Name.c_str());
		printf("Member.Name = %s\n", Member.Name.c_str());

		if(Member_List[i].Name == Member.Name){
			hasID = true;
			puts("hasID");
			Member_List[i].Work_End_Time = t;
			
			if(Member_List[i].Work_Start_Time != 0){
				hours = (Member_List[i].Work_Start_Time / 3600 + 9) % 24;
				minutes = (Member_List[i].Work_Start_Time / 60) % 60;
				if(hours < 10){
					bufstr = "0";
					bufstr += std::to_string(hours);
				}else{
					bufstr = std::to_string(hours);
				}
				bufstr += ":";
				if(minutes < 10){
					bufstr += "0";
				}
				bufstr += std::to_string(minutes);
				WS_recode->endText();
				WS_recode->setText(bufstr, 0, 0, 0.4, true);
				WS_recode->setPosC(1100, 950);
			}else{
				bufstr = " ";
				WS_recode->endText();
				WS_recode->setText(bufstr, 0, 0, 0.4, true);
				WS_recode->setPosC(1100, 950);
			}

			if(Member_List[i].Work_End_Time != 0){
				hours = (Member_List[i].Work_End_Time / 3600 + 9) % 24;
				minutes = (Member_List[i].Work_End_Time / 60) % 60;
				if(hours < 10){
					bufstr = "0";
					bufstr += std::to_string(hours);
				}else{
					bufstr = std::to_string(hours);
				}
				bufstr += ":";
				if(minutes < 10){
					bufstr += "0";
				}
				bufstr += std::to_string(minutes);
				WE_recode->endText();
				WE_recode->setText(bufstr, 0, 0, 0.4, true);
				WE_recode->setPosC(1700, 950);
				WorkEnd = true;
			}else{
				bufstr = " ";
				WE_recode->endText();
				WE_recode->setText(bufstr, 0, 0, 0.4, true);
				WE_recode->setPosC(1700, 950);
			}
			
			NAME_recode->endText();
			NAME_recode->setText(Member_List[i].Name, 600, 500, 0.4, true);
			NAME_recode->setPosC(750, 950);
			printf("%lld, %s, %02d:%02d, %02d:%02d\n\n", ID, Member_List[i].Name.c_str()
					, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
					, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
		}
	}

	if(!hasID){
		Member.Work_Start_Time = t;
		if(688000000000 >ID && ID > 687900000000){
			Member_List.emplace_back(Member);

			NAME_recode->endText();
			NAME_recode->setText(Member.Name, 600, 500, 0.4, true);
			NAME_recode->setPosC(750, 950);

			hours = (Member.Work_Start_Time / 3600 + 9) % 24;
			minutes = (Member.Work_Start_Time / 60) % 60;
			if(hours < 10){
				bufstr = "0";
				bufstr += std::to_string(hours);
			}else{
				bufstr = std::to_string(hours);
			}
			bufstr += ":";
			if(minutes < 10){
				bufstr += "0";
			}
			bufstr += std::to_string(minutes);
			WS_recode->endText();
			WS_recode->setText(bufstr, 0, 0, 0.4, true);
			WS_recode->setPosC(1100, 950);

			bufstr = " ";
			WE_recode->endText();
			WE_recode->setText(bufstr, 0, 0, 0.4, true);
			WE_recode->setPosC(1700, 950);
			
			printf("%lld, %s, %02d:%02d, %02d:%02d\n\n", ID, Member.Name.c_str()
					, ((Member.Work_Start_Time / 3600 + 9) % 24),  Member.Work_Start_Time / 60 % 60
					, ((Member.Work_End_Time / 3600 + 9) % 24),  Member.Work_End_Time / 60 % 60);
		}
	}
	felica_free(f2);
}

void Event() {
	printf("Ready for polling\n");
	static int TimeCount;
	SDL_Event event; // SDLによるイベントを検知するための構造体
	SDL_Event quit_event = {SDL_QUIT};	// 特定のイベント名を格納

	while (1) {
		switch(screen){
		case 0:
			// printf("%d\n",in::mouseClick);
			in::mouseClick = false;
			// printf("mode = %d\n", mode);
			if(push_complete_button){
				push_complete_button = false;
			}
			t = time(NULL);
			FeliCa_Polling();
			// イベントを取得したなら
			if(SDL_PollEvent(&event)){
				// イベントの種類ごとに処理
				switch (event.type) {
				case SDL_WINDOWEVENT:
					if(event.window.event == SDL_WINDOWEVENT_CLOSE){
						SDL_PushEvent(&quit_event);
					}
					break;
				case SDL_MOUSEMOTION: // マウスが移動した時
					in::mouseX = event.motion.x;
					in::mouseY = event.motion.y;
					// printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);	// マウスの座標を表示
					break;
				case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
					if(event.button.button == SDL_BUTTON_LEFT){
						in::mouseClick = true;
					}
 					// printf("Mouse button %d pressed at (%d,%d)\n", event.button.button, event.button.x, event.button.y);	// 押されたマウスのボタン等を表示
					break;
				case SDL_MOUSEBUTTONUP:
					break;
				case SDL_KEYDOWN:	// キーボードのキーが押された時
					// 押されたキーの名前等を表示
					// printf("The pressed key is %s.\n", SDL_GetKeyName(event.key.keysym.sym));
					// printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), event.key.keysym.mod);
					// 左シフトキーが押されたら
					if(SDL_GetModState()==KMOD_LSHIFT){
						printf("Left Shift + ");
					}
					// 右シフトキーが押されたら
					else if(event.key.keysym.mod==KMOD_RSHIFT){
						printf("Right Shift + ");
					}
					else{
						// 押されたキーごとに処理
						switch(event.key.keysym.sym){
						case SDLK_ESCAPE:	// Escキーが押された時
							printf("ESCAPE key -> EXIT.\n");
							SDL_PushEvent(&quit_event);
							break;
						case SDLK_s:
							Out_File();
						}
					}
					break;
				case SDL_QUIT:	// SDLの利用を終了する時
					Out_File();
					SDL_DestroyWindow(window);
					SDL_DestroyRenderer(renderer);
					IMG_Quit();
					TTF_Quit();
					SDL_Quit();
					exit(0);
					break;
				}
			}
			if(WorkEnd){
				TimeCount++;
			}

			if(TimeCount > 25){
				TimeCount = 0;
				screen = 1;
				WorkEnd = false;
			}
			break;
		case 1:
			in::mouseClick = false;
			// イベントを取得したなら
			if(SDL_PollEvent(&event)){
				// イベントの種類ごとに処理
				switch (event.type) {
				case SDL_WINDOWEVENT:
					if(event.window.event == SDL_WINDOWEVENT_CLOSE){
						SDL_PushEvent(&quit_event);
					}
					break;
				case SDL_MOUSEMOTION: // マウスが移動した時
					in::mouseX = event.motion.x;
					in::mouseY = event.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
					if(event.button.button == SDL_BUTTON_LEFT){
						in::mouseClick = true;
					}
					break;
				case SDL_KEYDOWN:	// キーボードのキーが押された時
					// 押されたキーごとに処理
					switch(event.key.keysym.sym){
					case SDLK_ESCAPE:	// Escキーが押された時
						printf("ESCAPE key -> EXIT.\n");
						SDL_PushEvent(&quit_event);
						break;
					}
					break;
				case SDL_QUIT:	// SDLの利用を終了する時
					SDL_DestroyWindow(window);
					SDL_DestroyRenderer(renderer);
					IMG_Quit();
					TTF_Quit();
					SDL_Quit();
					exit(0);
					break;
				}
			}
			if(push_button){
				// ボタンを押した後に実行したいなぁ
				std::string tmp_Name = Member_Name.at(tmp_ID);
				for(int i = 0;i < Member_List.size();i++){
					if(Member_List[i].Name == tmp_Name){
						if(break_time){
							Member_List[i].Break_Time = 30;
							puts("set break time 30");
						}else{
							Member_List[i].Break_Time = 0;
							puts("set break time 0");
						}
					}
				}
				break_time = false;
				push_button = false;
			}
			break;
		}
		display();
	}
}

void Out_File() {
std::ofstream outputfile(filename,std::ios_base::out);

if(!outputfile.is_open()){
      std::cout << "failed outputfile open" << std::endl;
}else{
	for(int i = 0;i<Member_List.size();i++){
		outputfile << Member_List[i].Name << "," << std::flush;
		if(Member_List[i].Work_Start_Time == 0){
			outputfile << " ," << std::flush;
		}else{
			if((Member_List[i].Work_Start_Time / 3600 + 9) % 24 < 10){
				outputfile << "0" << std::flush;
			}
			outputfile << (Member_List[i].Work_Start_Time / 3600 + 9) % 24 << ":" << std::flush;
			if(Member_List[i].Work_Start_Time / 60 % 60 < 10){
				outputfile << "0" << std::flush;
			}
			outputfile << Member_List[i].Work_Start_Time / 60 % 60 << "," << std::flush;
		}

		if(Member_List[i].Work_End_Time == 0){
			outputfile << " ," << std::flush;
		}else{
			if((Member_List[i].Work_End_Time / 3600 + 9) % 24 < 10){
				outputfile << "0" << std::flush;
			}
			outputfile << (Member_List[i].Work_End_Time / 3600 + 9) % 24 << ":" << std::flush;
			if(Member_List[i].Work_End_Time / 60 % 60 < 10){
				outputfile << "0" << std::flush;
			}
			outputfile << Member_List[i].Work_End_Time / 60 % 60 << "," << std::flush;
		}

		if(Member_List[i].Break_Time == 0){
			outputfile << " ," << std::endl;
		}else{
			outputfile << (int)Member_List[i].Break_Time << "," << std::endl;
		}
	}
	outputfile.close();
}

#pragma region fprintf dump
/*
	if ((fp = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "Can't Open File.\n");
	}

	for(int i=0;i < Member_List.size();i++) {
		if(688000000000 > Member_List[i].ID && Member_List[i].ID > 687900000000){
// 驚異の場合分け　16通り！！！　泣
			if(Member_List[i].Work_Start_Time == 0){ // WS 0
				if(Member_List[i].Rest_Start_Time == 0){ // WS 0 RS 0
					if(Member_List[i].Rest_End_Time == 0){ // WS 0 RS 0 RE 0
						if(Member_List[i].Work_End_Time == 0){ 								// WS 0 RS 0 RE 0 WE 0
							fprintf(fp, "%lld,%s, , , , ,\n", Member_List[i].ID, Member_List[i].Name.c_str());
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 0 RS 0 RE 0 WE 1
							fprintf(fp, "%lld,%s, , , ,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}else if(Member_List[i].Rest_End_Time != 0){ 	// WS 0 RS 0 RE 1
						if(Member_List[i].Work_End_Time == 0){ 								// WS 0 RS 0 RE 1 WE 0
							fprintf(fp, "%lld,%s, , ,%02d:%02d, ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 0 RS 0 RE 1 WE 1
							fprintf(fp, "%lld,%s, , ,%02d:%02d,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}
				}else if(Member_List[i].Rest_Start_Time != 0){ // WS 0 RS 1
					if(Member_List[i].Rest_End_Time == 0){ // WS 0 RS 1 RE 0
						if(Member_List[i].Work_End_Time == 0){ 								// WS 0 RS 1 RE 0 WE 0
							fprintf(fp, "%lld,%s, ,%02d:%02d, , ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 0 RS 1 RE 0 WE 1
							fprintf(fp, "%lld,%s, ,%02d:%02d, ,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}else if(Member_List[i].Rest_End_Time != 0){ // WS 0 RS 1 RE 1
						if(Member_List[i].Work_End_Time == 0){ 								// WS 0 RS 1 RE 1 WE 0
							fprintf(fp, "%lld,%s, ,%02d:%02d,%02d:%02d, ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 0 RS 1 RE 1 WE 1
							fprintf(fp, "%lld,%s, ,%02d:%02d,%02d:%02d,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}
				}
			}else if(Member_List[i].Work_Start_Time != 0){ // WS 1
				if(Member_List[i].Rest_Start_Time == 0){ // WS 1 RS 0
					if(Member_List[i].Rest_End_Time == 0){ // WS 1 RS 0 RE 0
						if(Member_List[i].Work_End_Time == 0){ 								// WS 1 RS 0 RE 0 WE 0
							fprintf(fp, "%lld,%s,%02d:%02d, , , ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 1 RS 0 RE 0 WE 1
							fprintf(fp, "%lld,%s,%02d:%02d, , ,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}else if(Member_List[i].Rest_End_Time != 0){ // WS 1 RS 0 RE 1
						if(Member_List[i].Work_End_Time == 0){ 								// WS 1 RS 0 RE 1 WE 0
							fprintf(fp, "%lld,%s,%02d:%02d, ,%02d:%02d, ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60);

						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 1 RS 0 RE 1 WE 1
							fprintf(fp, "%lld,%s,%02d:%02d, ,%02d:%02d,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);

						}
					}
				}else if(Member_List[i].Rest_Start_Time != 0){ // WS 1 RS 1
					if(Member_List[i].Rest_End_Time == 0){ // WS 1 RS 1 RE 0
						if(Member_List[i].Work_End_Time == 0){ 								// WS 1 RS 1 RE 0 WE 0
							fprintf(fp, "%lld,%s,%02d:%02d,%02d:%02d, , ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 1 RS 1 RE 0 WE 1
							fprintf(fp, "%lld,%s,%02d:%02d,%02d:%02d, ,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}else if(Member_List[i].Rest_End_Time != 0){ // WS 1 RS 1 RE 1
						if(Member_List[i].Work_End_Time == 0){ 								// WS 1 RS 1 RE 1 WE 0
							fprintf(fp, "%lld,%s,%02d:%02d,%02d:%02d,%02d:%02d, ,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60);
						}else if(Member_List[i].Work_End_Time != 0){ 						// WS 1 RS 1 RE 1 WE 1
							fprintf(fp, "%lld,%s,%02d:%02d,%02d:%02d,%02d:%02d,%02d:%02d,\n", Member_List[i].ID, Member_List[i].Name.c_str()
									, ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
									, ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
									, ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
									, ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
						}
					}
				}
			}
			*/
#pragma endregion
#pragma region fprintf dump2
			// if(Member_List[i].Rest_Start_Time == 0){
				// if(Member_List[i].Rest_End_Time == 0){
					// if(Member_List[i].Work_End_Time == 0){
						// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
								// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
								// , num, num
								// , num, num
								// , num, num);
					// }else if(Member_List[i].Work_End_Time != 0){ // WS 1 RS 0 RE 0 WE 1 
						// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
								// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
								// , num, num
								// , num, num
								// , ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
					// }
				// }else if(Member_List[i].Work_End_Time == 0){
					// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
								// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
								// , num, num
								// , ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
								// , num, num);
				// }else if(Member_List[i].Work_End_Time != 0){
					// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
								// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
								// , num, num
								// , ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
								// , ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
				// }
			// }else if(Member_List[i].Rest_End_Time == 0){
				// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
						// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
						// , ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
						// , num, num
						// , num, num);
			// }else if(Member_List[i].Work_End_Time == 0){
				// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
						// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
						// , ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
						// , ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
						// , num, num);
			// }else{
				// fprintf(fp, "%lld, %s, %02d:%02d, %02d:%02d, %02d:%02d, %02d:%02d\n", Member_List[i].ID, Member_List[i].Name.c_str()
						// , ((Member_List[i].Work_Start_Time / 3600 + 9) % 24),  Member_List[i].Work_Start_Time / 60 % 60
						// , ((Member_List[i].Rest_Start_Time / 3600 + 9) % 24),  Member_List[i].Rest_Start_Time / 60 % 60
						// , ((Member_List[i].Rest_End_Time / 3600 + 9) % 24),  Member_List[i].Rest_End_Time / 60 % 60
						// , ((Member_List[i].Work_End_Time / 3600 + 9) % 24),  Member_List[i].Work_End_Time / 60 % 60);
			// }
#pragma endregion
		// }
	// }
	// fclose(fp);
}

int setBefore9Hours(int time){
	switch(time){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:time += 15;break;
	default:time -= 9;break;
	}

	return time;
}

void initWindow (){
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}

	if ( TTF_Init() < 0 ) {
        printf("TTFcould not initialize! TTF_Error: %s\n", TTF_GetError());
    }

    if( (window = SDL_CreateWindow("Attendance Management Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL )) == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void load (){

	bufstr = "打刻履歴";
	Touch_recode = new Text();
	Touch_recode->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	Touch_recode->setText(bufstr, 400, 500, 0.6, true);
	Touch_recode->setPosC(325, 750);

	bufstr = " ";
	NAME_recode = new Text();
	NAME_recode->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	NAME_recode->setText(bufstr, 600, 500, 0.4, true);
	NAME_recode->setPosC(750, 950);

	bufstr = " ";
	WS_recode = new Text();
	WS_recode->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	WS_recode->setText(bufstr, 800, 500, 0.4, true);
	WS_recode->setPosC(1100, 950);


	bufstr = " ";
	WE_recode = new Text();
	WE_recode->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	WE_recode->setText(bufstr, 1400, 500, 0.4, true);
	WE_recode->setPosC(1700, 950);

	bufstr = "氏名";
	NAME_label = new Text();
	NAME_label->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	NAME_label->setText(bufstr, 600, 500, 0.4, true);
	NAME_label->setPosC(750, 850);

	bufstr = "勤務開始";
	WS_label = new Text();
	WS_label->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	WS_label->setText(bufstr, 800, 500, 0.4, true);
	WS_label->setPosC(1100, 850);

	bufstr = "勤務終了";
	WE_label = new Text();
	WE_label->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	WE_label->setText(bufstr, 1400, 500, 0.4, true);
	WE_label->setPosC(1700, 850);

	bufstr = "Touch";
	Touch = new Text();
	Touch->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");

	bufstr = "氏名";
	nameEx = new Text();
	nameEx->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	nameEx->setText(bufstr, 650, 200, 0.7, true);

	bufstr = "時間";
	nowTimeEx = new Text();
	nowTimeEx->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	nowTimeEx->setText(bufstr, 1400, 200, 0.7, true);

	bufstr = " ";
	name = new Text();
	name->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	name->setText(bufstr, 750, 350, 0.6, true);

	bufstr = " ";
	nowTime = new Text();
	nowTime->loadFont(renderer, ".\\Fonts\\YuGothic-Regular-01.ttf");
	nowTime->setText(bufstr, 1500, 350, 0.6);

	WorkStartButton = new Img();
	WorkStartButton->load(renderer, "./Assets/workStartButton2.png");
	WorkStartButton->setPosC(400, 600);

	RestStartButton = new Img();
	RestStartButton->load(renderer, "./Assets/restStartButton2.png");
	RestStartButton->setPosC(800, 600);

	RestEndButton = new Img();
	RestEndButton->load(renderer, "./Assets/restEndButton2.png");
	RestEndButton->setPosC(1200, 600);

	WorkEndButton = new Img();
	WorkEndButton->load(renderer, "./Assets/workEndButton2.png");
	WorkEndButton->setPosC(1600, 600);

	CSVbutton = new Img();
	CSVbutton->load(renderer, "./Assets/CSVbutton2.png");
	CSVbutton->setPos(1500, 1100);

	bufstr = "本日休憩しましたか？";
	check_Break_Time = new Text();
	check_Break_Time->loadFont(renderer,  ".\\Fonts\\YuGothic-Regular-01.ttf");
	check_Break_Time->setText(bufstr, 200, 400, 1, true);

	bufstr = "どちらかを選んで完了ボタンを押してください。";
	check_Break_Time2 = new Text();
	check_Break_Time2->loadFont(renderer,  ".\\Fonts\\YuGothic-Regular-01.ttf");
	check_Break_Time2->setText(bufstr, 200, 500, 1, true);

	bufstr = "どちらかを選んでください";
	Error_push_button = new Text();
	Error_push_button->loadFont(renderer,  ".\\Fonts\\YuGothic-Regular-01.ttf");
	Error_push_button->setText(bufstr, 300, 200, 1, true, 255);

	HaveRestButton = new Img(renderer,"./Assets/HaveRestButton.png");
	HaveRestButton->setPosC(WINDOW_WIDTH / 3, 800);

	NotHaveRestButton = new Img(renderer,"./Assets/NotHaveRestButton.png");
	NotHaveRestButton->setPosC(WINDOW_WIDTH / 3 * 2, 800);

	COMPLETEButton = new Img(renderer,"./Assets/COMPLETE.png");
	COMPLETEButton->setPosC(1500, 1100);
}

void display (){
	SDL_Rect recodeCell = {200, 800, 1600, 200};
	switch(screen){
	case 0:
		SDL_SetRenderDrawColor(renderer, 204, 204, 204, 204);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 204);
		SDL_RenderFillRect(renderer, &recodeCell);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &recodeCell);
		SDL_RenderDrawLine(renderer, 200, 900, 1800, 900);
		SDL_RenderDrawLine(renderer, 500, 800, 500, 1000);
		SDL_RenderDrawLine(renderer, 1000, 800, 1000, 1000);
		SDL_RenderDrawLine(renderer, 1200, 800, 1200, 1000);
		SDL_RenderDrawLine(renderer, 1400, 800, 1400, 1000);
		SDL_RenderDrawLine(renderer, 1600, 800, 1600, 1000);
		nameEx->show();
		nowTimeEx->show();
		name->show();
		nowTime->show();
		Touch_recode->show();
		NAME_label->show();
		WS_label->show();
		WE_label->show();
		NAME_recode->show();
		WS_recode->show();
		WE_recode->show();
		WorkStartButton->show(1);
		RestStartButton->show(2);
		RestEndButton->show(3);
		WorkEndButton->show(4);
		CSVbutton->show(5);
		break;
	case 1:
		SDL_SetRenderDrawColor(renderer, 204, 204, 204, 204);
		SDL_RenderClear(renderer);
		check_Break_Time->show();
		check_Break_Time2->show();
		HaveRestButton->show(6);
		NotHaveRestButton->show(7);
		COMPLETEButton->show(8);
		if(push_complete_button){
			if(!complete_preparation){
				Error_push_button->show();
			}
		}
		break;
	}
	SDL_RenderPresent(renderer);
}
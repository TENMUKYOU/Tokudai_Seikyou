#include "SDL.h"
#include "All.h"

typedef struct//フレームレート用の変数
{
	Uint32 now,//現在時間
		wit,//待ち時間
		lev;//経過時間
}timers;

extern timers timer;

bool Touch_Flag = false;

void UIInit() {
	/*SDL*/
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("AttendanceManagement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Event() {
	SDL_Event event;
	SDL_Event quit_event = {SDL_QUIT};	// ???????????
	File_INIT();
	while (1) {
		if (SDL_PollEvent(&event)) {
			// ????????????
			switch (event.type) {
			case SDL_MOUSEMOTION: // ?????????
				// printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);	// ?????????
				break;
			case SDL_MOUSEBUTTONDOWN:	// ?????????????
 				// printf("Mouse button %d pressed at (%d,%d)\n", event.button.button, event.button.x, event.button.y);	// ???????????????
				break;
			case SDL_KEYDOWN:	// ??????????????
				// ?????????????
				// printf("The pressed key is %s.\n", SDL_GetKeyName(event.key.keysym.sym));
				// printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), event.key.keysym.mod);
				// ????????????
				if(SDL_GetModState()==KMOD_LSHIFT){
					// printf("Left Shift + ");
				}
				// ????????????
				else if(event.key.keysym.mod==KMOD_RSHIFT){
					// printf("Right Shift + ");
				}
				else{
					// ???????????
					switch(event.key.keysym.sym){
					case SDLK_ESCAPE:	// Esc????????
						SDL_PushEvent(&quit_event); // ????????SDL_QUIT?????????????????
						break;
					case SDLK_TAB:	// Tab????????
						SDL_PushEvent(&quit_event);	// ????????SDL_QUIT?????????????????
						break;
					case SDLK_s:
						Out_File();
						puts("Out Put Data");
					}
				}
				break;
			case SDL_QUIT:	// SDL?????????
				SDL_Quit();
				felica_free(f);
				pasori_close(p);
				exit(0);
				break;
			}
		}
		FeliCa_Polling();
		SDL_Delay(5);  // ???1???200???????
	}
}
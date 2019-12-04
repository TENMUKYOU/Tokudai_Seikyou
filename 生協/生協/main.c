#include <stdio.h>
#include <SDL.h>
#include "felicalib.h"

#include "All.h"
//https://www.hiroom2.com/2015/05/11/sdl2%E3%81%AE%E5%88%9D%E6%9C%9F%E5%8C%96%E5%87%A6%E7%90%86-%E3%82%A6%E3%82%A3%E3%83%B3%E3%83%89%E3%82%A6%E4%BD%9C%E6%88%90-%E6%8F%8F%E7%94%BB%E5%87%A6%E7%90%86/#sec-1

void Event();

int main(int argc, char* argv[]) {
	SDL_INIT();
	FeliCa_INIT();
	Event();
	return 0;
}


void Event() {
	SDL_Event e;
	while(1) {
		FeliCa_INIT();
		if (Touch_Flag) {
			FeliCa();
		}
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				felica_free(f);
				pasori_close(p);
				break;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				felica_free(f);
				pasori_close(p);
				break;
			}
		}
	}
}


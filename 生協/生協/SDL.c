#include <SDL.h>
#include "All.h"


typedef struct//�t���[�����[�g�p�̕ϐ�
{
	Uint32 now,//���ݎ���
		wit,//�҂�����
		lev;//�o�ߎ���
}timers;

extern timers timer;

void SDL_INIT() {
	/*SDL*/
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

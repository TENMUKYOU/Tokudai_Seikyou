#include "felicalib.h"
#include "All.h"


// https://www.hiroom2.com/2015/05/11/sdl2%E3%81%AE%E5%88%9D%E6%9C%9F%E5%8C%96%E5%87%A6%E7%90%86-%E3%82%A6%E3%82%A3%E3%83%B3%E3%83%89%E3%82%A6%E4%BD%9C%E6%88%90-%E6%8F%8F%E7%94%BB%E5%87%A6%E7%90%86/#sec-1


int main(int argc, char* argv[]){
	t = time(NULL);
	localtime(&t);
	UIInit();
	FeliCa_INIT();
	printf("Ready for polling\n");
	Event();
	return 0;
}
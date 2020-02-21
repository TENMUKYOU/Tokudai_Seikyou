#include "image.h"


Img::Img(void):
    r(255),
    g(255),
    b(255)
{}


Img::Img(SDL_Renderer* ren, std::string path, bool manual_destruct):
    r(255),
    g(255),
    b(255)
{
    this->load(ren, path, manual_destruct);
}


Img::~Img() {
    if(!this->manual_destruct && this->texture) {
        SDL_DestroyTexture(this->texture);
        printf("destructed img\n");
    }
}


void Img::endImg(void) {
    if(this->texture) {
        SDL_DestroyTexture(this->texture);
        printf("end Img\n");
    }
}


void Img::load(SDL_Renderer* ren, std::string path, bool manual_destruct) { 
    this->img = IMG_Load(path.c_str());
    if(!this->img) {
        printf("failed load Img\n");
        this->~Img();
        throw "画像ファイルひらけんわ : " + path;
    }
    this->texture = SDL_CreateTextureFromSurface(ren, this->img);
    SDL_FreeSurface(this->img);
    if(!this->texture) {
        printf("failed texture Img\n");
        this->~Img();
        throw "テクスチャ作れんかったわ : " + path; 
    }

    this->ren = ren;
    this->manual_destruct = manual_destruct;
    //printf("img size %d, %d\n", this->img->w, this->img->h);
}


void Img::setPos(int x, int y, float ratio) {
    this->origin.x = 0;
    this->origin.y = 0;
    this->origin.w = this->img->w;
    this->origin.h = this->img->h;
    this->dst.x = x;
    this->dst.y = y;
    this->dst.w = (int)(this->img->w * ratio);
    this->dst.h = (int)(this->img->h * ratio);
}


void Img::setPosC(int x, int y, float ratio) {
    this->origin.x = 0;
    this->origin.y = 0;
    this->origin.w = this->img->w;
    this->origin.h = this->img->h;
    this->dst.w = (int)(this->img->w * ratio);
    this->dst.h = (int)(this->img->h * ratio);
    this->dst.x = x - (this->dst.w * 0.5);
    this->dst.y = y - (this->dst.h * 0.5);
    //printf("img.w=%d",this->img->w);
}



void Img::setPosU(int x, int y, float ratio) {
    // なんか知らんけどポインタのアクセス遅いんか？
    this->dst.x = x;
    this->dst.y = y;
}


void Img::setPosUC(int x, int y) {
    // なんか知らんけどポインタのアクセス遅いんか？
    this->dst.x = x - (this->dst.w * 0.5);
    this->dst.y = y - (this->dst.h * 0.5);
}


void Img::setPosDelta(int x, int y) {
    this->dst.x += x;
    this->dst.y += y;
}


SDL_Rect Img::getCharaSize(void){
    return dst;
}

void Img::mouseClick(int buttonType){
    if(this->dst.y < in::mouseY && in::mouseY < this->dst.y + this->dst.h && this->dst.x < in::mouseX && in::mouseX < this->dst.x + this->dst.w){
        if(in::mouseClick == true){
            if(buttonType == 8 && CSVonce == false || buttonType == 5 && CSVonce == false){
                this->r = 140;
                this->g = 140;
                this->b = 140;
                Event(buttonType);
                puts("OK");
                CSVonce = true;
            }else{
                if(this->r == 140){
                    this->r = 255;
                    this->g = 255;
                    this->b = 255;
                    push = false;
                    Event();
                }else{
                    this->r = 140;
                    this->g = 140;
                    this->b = 140;
                    Event(buttonType);
                    push = true;
                }
            }
        }
    }else{
        if(buttonType != 5){
            if(in::mouseClick == true){
                // printf("ffffffffffffffffff\n");
                this->r = 255;
                this->g = 255;
                this->b = 255;
                if(push == true){
                    mode = 0;
                }
            }
        }
    }

    if(CSVonce){
        count++;
    }

    if(count > 10){
        this->r = 255;
        this->g = 255;
        this->b = 255;
        CSVonce = false;
        count = 0;
    }
    // printf("count = %d\n", count);
    // printf("CSV = %d\n", CSVonce);
}

void Img::show(int buttonType) {
    mouseClick(buttonType);
    SDL_SetTextureColorMod(this->texture, this->r, this->g, this->b);
    //printf("%d  %d\n", this->origin.x, this->origin.y);
    if(SDL_RenderCopy(this->ren, this->texture, &origin, &dst) < 0) {
        printf("D\n");
    }
}

void Img::Event(int buttonType){
    
    switch(buttonType){
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 3;
        break;
    case 4:
        mode = 4;
        break;
    case 5:
        Out_File();
        break;
    case 6:
        break_time = true;
        push_button = true;
        complete_preparation = true;
        break;
    case 7:
        break_time = false;
        push_button = true;
        complete_preparation = true;
        break;
    case 8:
        push_complete_button = true;
        if(complete_preparation){
            screen = 0;
            complete_preparation = false;
        }
        break;
    }
}
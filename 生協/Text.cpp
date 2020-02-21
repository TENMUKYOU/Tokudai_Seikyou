/**
 * @file Text.cpp
 * @author 柴田博諄
 */

#include "Text.h"
#include "All.h"

Text::Text(void) {
    ;
}


Text::~Text() {
    // 再読み込みのとき，以前のポインタの指していたところは誰からも指されなくなる -> こいつが起動するはず
    // NULL でやろうとするとセグフォが起きる（確認済）
    //if(!this->manual_destruct && this->texture) {
    //    SDL_DestroyTexture(this->texture);
    //    printf("text distructed.\n");
    //}
    SDL_DestroyTexture(this->texture);
}


void Text::endText(void) {
    if(this->texture) {
        SDL_DestroyTexture(this->texture);
        //printf("end Text\n");
    }
}


void Text::loadFont(SDL_Renderer* ren, std::string path, int size, bool manual_destruct) {
    this->font = TTF_OpenFont(path.c_str(), size);
    if(!this->font)
        printf("failed load font\n");
    this->ren = ren;
    this->manual_destruct = manual_destruct;
}

void Text::inputText(std::string str, int x, int y, double ratio, bool mode){
	// /* 文字作成 */
	// if(mode == true){
		// text = Sjis2Utf8(text);
	// }
// 
	// surface = TTF_RenderUTF8_Blended(font, str.c_str(), black);
	// texture = SDL_CreateTextureFromSurface(renderer, surface);
	// int iw,ih;
    // SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
	// dst = {0, 0, iw, ih};
// 
    // src = {x, y, (int)(iw*ratio), (int)(ih*ratio)};
	// SDL_RenderCopy(renderer, texture, &dst, &src);
}

void Text::setText(std::string text, int x, int y, float ratio, bool mode, Uint8 r, Uint8 g, Uint8 b){
    if(mode == true){
		text = Sjis2Utf8(text);
	}
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
    this->text = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if(!this->text)
        printf("failed ttf\n");
    this->texture = SDL_CreateTextureFromSurface(this->ren, this->text);
    SDL_FreeSurface(this->text);
    if(!this->texture)
        printf("failed ttf texture\n");
    this->origin.x = 0;
    this->origin.y = 0;
    this->origin.w = this->text->w;
    this->origin.h = this->text->h;
    this->dst.x = x;
    this->dst.y = y;
    this->dst.w = (int)(this->text->w * ratio);
    this->dst.h = (int)(this->text->h * ratio);
}

void Text::setPosU(int x, int y) {
    this->dst.x = x;
    this->dst.y = y;
}

void Text::setPosC(int x, int y){
    this->dst.x = x - (this->dst.w * 0.5);
    this->dst.y = y - (this->dst.h * 0.5);
}

int Text::width(void) {
    return this->origin.w;
}

std::string Text::Sjis2Utf8(const std::string& src)
{
    if(src.empty())return "";

    //ワイド文字列のバッファ確保
    const int cchWideChar = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
    if( cchWideChar <= 0 )return "";
    std::wstring strw;
    strw.resize(cchWideChar);
    //ワイド文字列へ変換
    const int nUnicodeCount = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &strw[0], cchWideChar);
    if( nUnicodeCount <= 0 )return "";

    //UTF8のバッファ確保
    const int cchMultiByte = ::WideCharToMultiByte(CP_UTF8, 0, strw.c_str(), -1, NULL, 0, NULL, NULL);
    if( cchMultiByte <= 0 )return "";
    std::string ret;
    ret.resize(cchMultiByte);
    //UTF-8へ変換
    const int nMultiCount = ::WideCharToMultiByte(CP_UTF8, 0, strw.c_str(), -1, &ret[0], cchMultiByte, NULL, NULL);
    if( nMultiCount <= 0 )return "";
    return ret;
}

void Text::show(SDL_Renderer* ren) {
    if(SDL_RenderCopy(ren, this->texture, &origin, &dst) < 0) {
        printf("E\n");
    }
}

void Text::show(void) {
    if(SDL_RenderCopy(this->ren, this->texture, &origin, &dst) < 0) {
        printf("F\n");
    }
}
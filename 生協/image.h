/**
 * @file button.hpp
 */

#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "All.h"


class Img {
public:
    Img(void);

    Img(SDL_Renderer* ren, std::string path, bool manual_destruct=false);

    // 基底クラスのデストラクタはvirtualにすべき
    ~Img();

    /**
     * 描画系で落ちた場合、load()でmanual_destruct=trueにして、
     * これでUI::end()にて破棄すること
     */
    void endImg(void);

    /**
     * 表示する画像の読み込み
     * 描画先のレンダラーが必要
     */
    void load(SDL_Renderer* ren, std::string path, bool manual_destruct=false);

    /**
     *  upDateにこれ使って！　位置をセット　x, y は画像左上
     */
    void setPosU(int x=0, int y=0, float ratio=1);

    void setPosUC(int x, int y);

    /**
     * 普通試験用で使う
     * @param x y　加算値  e.g. 等速直線運動なら定数　等加速度直線運動なら x=i++ (const i)
     */
    void setPosDelta(int x, int y);

    /**
     *  位置をセット　x, y は画像左上
     */
    void setPos(int x=0, int y=0, float ratio=1);
    
    /**
     *  位置をセット　x, y は画像中央
     */
    void setPosC(int x=0, int y=0, float ratio=1);

    /**
     *  キャラのサイズを返す関数
     */
    SDL_Rect getCharaSize(void);

    void mouseClick(int buttonType = 0);

    /**
     * 表示させる
     */
    void show(int buttonType = 0);

    void Event(int bottonType = 0);

    bool push = false;
protected:
    SDL_Surface* img;
    SDL_Rect origin;
    SDL_Rect dst;
    bool manual_destruct;
    Uint8 r, g, b;

private:
    SDL_Renderer* ren;
    SDL_Texture* texture;
    bool CSVonce;
    int count;
};
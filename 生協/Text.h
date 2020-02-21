/**
 * @brief SDL�ŕ����\��
 * @author �ēc���x
 * @date 2019/10/25
 */

#pragma once

#include <SDL_ttf.h>
#include <string>

/**
 * SDL�ŕ�����̕\��������N���X
 * @author �ēc���x
 */
class Text {
public:
    Text(void);
    ~Text();

    /**
     * �t�H���g�̓ǂݍ���
     */
    void loadFont(SDL_Renderer* ren, std::string path, int size=100, bool manual_destruct=false);

    /**
     * ������̃Z�b�g�@x, y �͍���̈ʒu
     * �����̍ēǂݍ��݂��\
     * 
     * char hoge[32] = "Uwaaaa!";
     * std::string tmp = hoge;
     * tmp�������ɓn���B
     */
    void setText(std::string text, int x=0, int y=0, float ratio=1, bool mode = false, Uint8 r=0, Uint8 g=0, Uint8 b=0);

    void setPosU(int x, int y);

    void setPosC(int x, int y);

    int width(void);
    void inputText(std::string str, int x, int y, double ratio = 1, bool mode = false);
    std::string Sjis2Utf8(const std::string& src);
    void show(SDL_Renderer* ren);
    void show(void);

    void endText();

private:
    SDL_Renderer* ren;
    SDL_Surface* text;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Rect origin;
    SDL_Rect dst;
    SDL_Color color;
    bool manual_destruct;
};

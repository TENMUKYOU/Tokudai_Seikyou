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

    // ���N���X�̃f�X�g���N�^��virtual�ɂ��ׂ�
    ~Img();

    /**
     * �`��n�ŗ������ꍇ�Aload()��manual_destruct=true�ɂ��āA
     * �����UI::end()�ɂĔj�����邱��
     */
    void endImg(void);

    /**
     * �\������摜�̓ǂݍ���
     * �`���̃����_���[���K�v
     */
    void load(SDL_Renderer* ren, std::string path, bool manual_destruct=false);

    /**
     *  upDate�ɂ���g���āI�@�ʒu���Z�b�g�@x, y �͉摜����
     */
    void setPosU(int x=0, int y=0, float ratio=1);

    void setPosUC(int x, int y);

    /**
     * ���ʎ����p�Ŏg��
     * @param x y�@���Z�l  e.g. ���������^���Ȃ�萔�@�������x�����^���Ȃ� x=i++ (const i)
     */
    void setPosDelta(int x, int y);

    /**
     *  �ʒu���Z�b�g�@x, y �͉摜����
     */
    void setPos(int x=0, int y=0, float ratio=1);
    
    /**
     *  �ʒu���Z�b�g�@x, y �͉摜����
     */
    void setPosC(int x=0, int y=0, float ratio=1);

    /**
     *  �L�����̃T�C�Y��Ԃ��֐�
     */
    SDL_Rect getCharaSize(void);

    void mouseClick(int buttonType = 0);

    /**
     * �\��������
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
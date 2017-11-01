//
//  Texture.cpp
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/12.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "Texture.h"
#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdio.h>

/**
 * デストラクタ
 */
Texture::~Texture(){
    if( texId ){
        glDeleteTextures(1, &texId);
    }
}

/**
 * 2Dテクスチャを作成する
 *
 * @param   width   テクスチャの幅（ピクセル数）
 * @param   height  テクスチャの高さ（ピクセル数）
 * @param   iformat テクスチャのデータ形式
 * @param   format  アクセスする要素
 * @param   data    テクスチャデータへのポインタ
 *
 * @return  作成に成功した場合はテクスチャポインタを返す
 *          失敗した場合は nullptr を返す
 */
TexturePtr Texture::Create(int width, int height, GLenum iformat, GLenum format, const void *data){
    //構造体はクラスらしいぞ  ↓Texture継承してる
    struct Impl : Texture {};
    TexturePtr p = std::make_shared<Impl>();
    p->width = width ;
    p->height = height ;
    glGenTextures(1, &p->texId);
    glBindTexture(GL_TEXTURE_2D, p->texId);
    glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //エラー取得
    const GLenum result = glGetError();
    if( result != GL_NO_ERROR ){
        std::cerr << "ERROR テクスチャ作成に失敗: 0x" << std::hex << result << std::endl;
        return {};
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    return p;
}

/**
 * ファイルから2Dテクスチャを読み込む
 *
 * @param   filename    ファイル名
 *
 * @return  作成に成功した場合はテクスチャポインタを返す
 *          失敗した場合は nullptr を返す
 */
TexturePtr Texture::LoadFromFile(const char* filename){
    TextureData data;
    TextureLoader::getInstance().loadFile(filename, data);
    
    if( data.type == TEXTURE_TYPE::NONE ){
        return {};
    }
    return Create(data.width, data.height, data.iformat, data.format, data.data.data());
}

/**
 * 拡大縮小時の保管方法切り替え
 * デフォルト
 */
void Texture::defaultFilter(){
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}
/**
 * 拡大縮小時の保管方法切り替え
 * くっきり見える
 */
void Texture::dotFilter(){
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

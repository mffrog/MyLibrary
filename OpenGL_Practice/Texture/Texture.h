//
//  Texture.h
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/12.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include "TextureLoad.h"
#include <GL/glew.h>
#include <memory>
#include <sys/stat.h>


class Texture;
typedef std::shared_ptr<Texture> TexturePtr; ///テクスチャポインタ

/**
 * テクスチャクラス
 */
class Texture{
public:
    //テクスチャ生成
    static TexturePtr Create(int width, int height, GLenum iformat, GLenum format, const void* data);
    //画像ファイル読み込み
    static TexturePtr LoadFromFile( const char*);
    //ID取得
    GLuint Id() const { return texId; }
    //幅取得
    GLsizei Width() const { return width; }
    //高さ取得
    GLsizei Height() const { return height;}
    
    /**
     * 拡大縮小時の保管方法切り替え
     * デフォルト
     */
    void defaultFilter();
    /**
     * 拡大縮小時の保管方法切り替え
     * くっきり見える
     */
    void dotFilter();
    
private:
    Texture() = default ;
    ~Texture();
    Texture(const Texture&) = delete ;
    Texture& operator=(const Texture&) = delete ;

    GLuint texId = 0;
    int width = 0;
    int height = 0;
    TEXTURE_TYPE type;
};

#endif /* Texture_h */

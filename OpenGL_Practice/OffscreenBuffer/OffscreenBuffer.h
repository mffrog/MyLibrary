//
//  OffscreenBuffer.h
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/12.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef OffscreenBuffer_h
#define OffscreenBuffer_h
#include "Texture.h"
#include <GL/glew.h>
#include <memory>

class OffscreenBuffer;
typedef std::shared_ptr<OffscreenBuffer> OffscreenBufferPtr ;

/**
 * オフスクリーンバッファ
 */
class OffscreenBuffer {
public:
    static OffscreenBufferPtr Create(int w, int h);
    GLuint GetFrameBuffer() const {return frameBuffer ;};         ///フレームバッファを取得する
    GLuint GetTexture() const {return tex->Id();}               ///フレームバッファ用tクス茶を取得する

private:
    OffscreenBuffer() = default ;
    OffscreenBuffer(const OffscreenBuffer&) = delete ;
    OffscreenBuffer& operator= ( const OffscreenBuffer& ) = delete ;
    ~OffscreenBuffer();
    
private:
    TexturePtr tex;            ///フレームバッファ用テクスチャ
    GLuint depthBuffer = 0;     ///震度バッファオブジェクト
    GLuint frameBuffer = 0;     ///フレームバッファオブジェクト
};

#endif /* OffscreenBuffer_h */

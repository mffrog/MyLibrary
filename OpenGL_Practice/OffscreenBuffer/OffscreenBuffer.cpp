//
//  OffscreenBuffer.cpp
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/12.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "OffscreenBuffer.h"

/**
 * オフスクリーンバッファを作成する
 *
 * @param   w   オフスクリーンバッファの幅（ピクセル数）
 * @param   h  オフスクリーンバッファの高さ（ピクセル数）
 *
 * @return  作成したオフスクリーンバッファへのポインタ
 */
OffscreenBufferPtr OffscreenBuffer::Create(int w, int h){
    struct Impl : OffscreenBuffer {};
    OffscreenBufferPtr offscreen = std::make_shared<Impl>();
    if(!offscreen){
        return offscreen ;
    }
    
    offscreen->tex = Texture::Create(w, h, GL_RGBA8, GL_RGBA, nullptr);
    if( !offscreen->tex ){
        return {};
    }
    
    //デプスバッファ作成
    glGenRenderbuffers(1, &offscreen->depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, offscreen->depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    
    //フレームバッファ作成
    glGenFramebuffers(1, &offscreen->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, offscreen->frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offscreen->depthBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return offscreen ;
}

/**
 * デストラクタ
 */
OffscreenBuffer::~OffscreenBuffer(){
    if(frameBuffer){
        glDeleteFramebuffers(1,&frameBuffer);
    }
    if(depthBuffer){
        glDeleteRenderbuffers(1, &depthBuffer);
    }
}
 

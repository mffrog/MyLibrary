//
//  ZlibManager.cpp
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/20.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "ZlibManager.h"
#include <iostream>

ZlibManager::~ZlibManager(){
    if(inflateInitialized){
        inflateEnd(&strm);
    }
}
/**
 * 解凍関数
 *
 * @param compData  解凍元データ配列(uint8_t配列)
 * @param decoData  解凍先データ配列(uint8_t配列)
 */
bool ZlibManager::Inflate(std::vector<uint8_t>& compData, std::vector<uint8_t>& decoData){
    if( inflateInitialized == true ){
        inflateEnd(&strm);
    }
    else{
        inflateInitialized = true;
    }
    strm.zalloc     = Z_NULL;
    strm.zfree      = Z_NULL;
    strm.opaque     = Z_NULL;
    
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    int ret = inflateInit(&strm);
    if( ret != Z_OK ){
        std::cerr << "inflate の初期化に失敗" << std::endl;
        return false;
    }
    ret = Z_OK;
    
    int outCount = 0;
    strm.next_in = (Bytef*)compData.data();
    strm.avail_in = (unsigned int)compData.size();
    strm.next_out = (Bytef*)decoData.data();
    strm.avail_out = (unsigned int)decoData.size();
    
    while (ret != Z_STREAM_END) {
        if(strm.avail_in == 0){
        }
        ret = inflate(&strm, Z_NO_FLUSH);
        if(ret == Z_STREAM_END) break;
        if(ret != Z_OK){
            std::cout << "ERROR: inflate" << std::endl;
            return false;
        }
        if(strm.avail_out == 0){
            std::cout << "出力先バッファのサイズが足りません。拡張します" << std::endl;
            decoData.resize(decoData.size()+ 32768 * ( 1 + ++outCount));
            strm.avail_out = 32768;
        }
    }
    
    decoData.shrink_to_fit();
    
    return true;
}

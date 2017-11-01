//
//  ZlibManager.h
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/20.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef ZlibManager_h
#define ZlibManager_h

#include <vector>
#include <zlib.h>

class ZlibManager{
public:
    ZlibManager() = default;
    ~ZlibManager();
    /**
     * 解凍関数
     *
     * @param compData  解凍元データ配列(uint8_t配列)
     * @param decoData  解凍先データ配列(uint8_t配列)
     *
     * @retval true     解凍成功
     * @retval false    解凍失敗
     */
    bool Inflate(std::vector<uint8_t>& compData, std::vector<uint8_t>& decoData);
private:
    ZlibManager(const ZlibManager&) = delete;
    ZlibManager& operator= (const ZlibManager&) = delete ;
    
    bool inflateInitialized = false;
    z_stream strm;
};

#endif /* ZlibManager_h */

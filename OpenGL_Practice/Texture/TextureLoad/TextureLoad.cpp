//
//  TextureLoad.cpp
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/20.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "TextureLoad.h"
#include "ZlibManager.h"

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define SAFE_DELETE(p) if(p){delete p ; p = nullptr;}

#define HEX_IHDR 0x49484452
#define HEX_IDAT 0x49444154
#define HEX_IEND 0x49454e44

#define IHDR 1229472850
#define IDAT 1229209940
#define IEND 1229278788

#define SIGNATURE_SIZE      8

#define LENGTH_SIZE         4
#define CHUNK_TYPE_SIZE     4
#define CRC_SIZE            4

#define IHDR_WIDTH_SIZE         4
#define IHDR_HEIGHT_SIZE        4
#define IHDR_BIT_DEPTH_SIZE     1
#define IHDR_COLOR_TYPE_SIZE    1
#define IHDR_CM_SIZE            1
#define IHDR_FM_SIZE            1
#define IHDR_IM_SIZE            1

#define PNG_FILTER_NONE     0
#define PNG_FILTER_SUB      1
#define PNG_FILTER_UP       2
#define PNG_FILTER_AVERAGE  3
#define PNG_FILTER_PAETH    4

#define IMAGE_INDEX column + row * rowBytes
#define CURRENT_DATA column + rowDataBytes * ( info.height - row - 1 ) - 1

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
    unsigned long c;
    int n, k;
    
    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
 should be initialized to all 1's, and the transmitted value
 is the 1's complement of the final running CRC (see the
 crc() routine below)). */

unsigned long update_crc(unsigned long crc,const unsigned char *buf,
                         int len)
{
    unsigned long c = crc;
    int n;
    
    if (!crc_table_computed)
        make_crc_table();
    for (n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long crc(const unsigned char *buf, int len)
{
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

/**
 * バイト列から数値を復元する
 *
 * @param   p       バイト列へのポインタ
 * @param   offset  数値のオフセット
 * @param   size    数値のバイト数( 1 ~ 4 )
 *
 * @return  復元した数値
 */
uint32_t Get(const uint8_t* p, size_t offset, size_t size){
    uint32_t n = 0;
    p+= offset ;
    for( size_t i = 0; i < size; ++i ){
        n += p[i] << ( i * 8 );
    }
    return n;
}

uint32_t GetBig(const uint8_t* p, size_t offset, size_t size){
    uint32_t n = 0;
    p += offset;
    for( size_t i = 0; i < size ; i++ ){
        n += p[i] << ( ( (size - 1) - i ) * 8);
    }
    return n;
}

TextureLoader& TextureLoader::getInstance(){
    static TextureLoader loader;
    return loader;
}

//コンストラクタ
TextureLoader::TextureLoader(){
    make_crc_table();
    this->loaderes.insert({TEXTURE_TYPE::BMP, new BMPLoader()});
    this->loaderes.insert({TEXTURE_TYPE::PNG, new PNGLoader()});
}
TextureLoader::~TextureLoader(){
    auto itr = loaderes.begin();
    while (itr != loaderes.end()) {
        SAFE_DELETE(itr->second);
        itr++;
    }
}

void TextureLoader::loadFile(const char *filename, TextureData& textureData){
    //ファイルサイズ取得
    struct stat st;
    if(stat(filename, &st)){
        perror(filename);
        return;
    }
    
    FILE* fp = fopen(filename, "rb");
    if(!fp){
        return;
    }
    
    std::vector<uint8_t> buf;
    buf.resize(st.st_size);
    const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
    fclose(fp);
    if( readSize != st.st_size ){
        return;
    }
    
    loaderes.find(checkType(buf, st))->second->loadFile(buf, textureData);
}

TEXTURE_TYPE TextureLoader::checkType(const std::vector<uint8_t>& buf, const struct stat& st ){
    
    const uint8_t*  pHeader = buf.data();
    
    //BMPファイルかどうかのチェック（大きさで判断）
    const size_t bmpFileHeaderSize = 14;    //ビットマップファイルヘッダのバイト数
    const size_t windowsVIHeaderSize = 40;  //ビットマップ情報ヘッダのバイト数]
    if(st.st_size >= bmpFileHeaderSize + windowsVIHeaderSize ){
        if(pHeader[0] == 'B' || pHeader[1] == 'M'){
            return TEXTURE_TYPE::BMP;
        }
    }
    
    //PNGチェック
    if( pHeader[0] == 0x89 && pHeader[1] == 0x50 &&
       pHeader[2] == 0x4E && pHeader[3] == 0x47 &&
       pHeader[4] == 0x0D && pHeader[5] == 0x0A &&
       pHeader[6] == 0x1A && pHeader[7] == 0x0A){
        return TEXTURE_TYPE::PNG;
    }
    
    return TEXTURE_TYPE::NONE;
}


void BMPLoader::loadFile(const std::vector<uint8_t>& buf, TextureData& textureData ){
    const size_t windowsVIHeaderSize = 40;  //ビットマップ情報ヘッダのバイト数
    
    //本当にBMPファイルなのかチェック
    const uint8_t*  pHeader = buf.data();
    if(pHeader[0] != 'B' || pHeader[1] != 'M'){
        std::cout << "ERROR: BMP ファイルじゃない" << std::endl;
        return;
    }
    const size_t offsetByte = Get(pHeader, 10, 4);
    const uint32_t infoSize = Get(pHeader, 14, 4);
    textureData.width    = Get(pHeader, 18, 4);
    textureData.height   = Get(pHeader, 22, 4);
    const uint32_t bitCount = Get(pHeader, 28, 2);
    const uint32_t compression = Get(pHeader, 30, 4);
    const size_t pixelBytes = bitCount / 8 ;
    if(infoSize != windowsVIHeaderSize ||
       bitCount != 24 ||
       compression
       //||(width * pixelBytes) % 4
       )
    {
        return;
    }
    
    int padding = 4 - (textureData.width * pixelBytes) % 4;
    padding = padding == 4 ? 0 : padding ;
    //画像データが正しく格納できているかチェック(サイズでチェック)
    const size_t imageSize = (textureData.width * pixelBytes + padding ) * textureData.height;
    if(buf.size() < offsetByte + imageSize){
        return;
    }
    textureData.type = TEXTURE_TYPE::BMP;
    textureData.iformat = GL_RGB8;
    textureData.format = GL_BGR;
    textureData.data.resize(imageSize);
    for( int i = 0; i < imageSize; i++ ){
        textureData.data[i] = buf[offsetByte + i];
    }
}

void PNGLoader::loadFile(const std::vector<uint8_t>& buf, TextureData& textureData ){
    
    const uint8_t* pHeader = buf.data();
    int offset = 8;
    uint32_t length = 0;
    uint32_t chunkType = -1;
    
    HeaderInfo info;
    
    std::vector<uint8_t> idat(0);
    
    while (offset >= 0) {
        length = GetBig(pHeader, offset, LENGTH_SIZE);
        chunkType = GetBig(pHeader, offset + LENGTH_SIZE, CHUNK_TYPE_SIZE);
        std::vector<uint8_t> crcBuffer(length + CHUNK_TYPE_SIZE) ;
        for( int i = 0; i < length + CHUNK_TYPE_SIZE; ++i ){
            crcBuffer[i] = pHeader[offset + LENGTH_SIZE + i ];
        }
        
        if( GetBig(pHeader,offset +  LENGTH_SIZE + CHUNK_TYPE_SIZE + length,4) != crc(crcBuffer.data(), length + 4)){
            std::cout << "WARNING: CRCが異なります" << std::endl;
        }
        
        switch (chunkType) {
            case IHDR:
                info = getHeaderInfo(pHeader);
                break;
            case IDAT:
                if(idat.size() == 0){
                    readIDATData(pHeader, offset, length, idat);
                }
                else {
                    std::vector<uint8_t> buf ;
                    readIDATData(pHeader, offset, length, buf);
                    idat.insert(idat.end(), buf.begin(), buf.end());
                }
                break;
            case IEND:
                offset = -1;
                break;
            default:
                break;
        }
        //IENDじゃない時
        if( offset >= 0 ){
            int chunkSize = LENGTH_SIZE + CHUNK_TYPE_SIZE + length + CRC_SIZE;
            offset += chunkSize;
        }
    }
    
    size_t pixelBytes = getPixelBytes(info);
    size_t rowDataBytes = pixelBytes * info.width;
    size_t imageSize= rowDataBytes * info.height ;
    
    std::vector<uint8_t> imageBuffer;
    imageBuffer.resize(imageSize + info.height);
    ZlibManager inflater;
    if( !inflater.Inflate(idat, imageBuffer)){
        std::cout << "解凍失敗" << std::endl;
        return;
    }
    size_t rowBytes = imageBuffer.size() / info.height;
    textureData.data.resize(pixelBytes * info.width * info.height);
    
    for( int row = 0; row < info.height; ++row ){
        for( int column = 0; column < rowBytes; ++column ){
            static int filter = -1;
            if(column == 0){
                filter = (int)imageBuffer[IMAGE_INDEX];
                continue;
            }
            switch (filter) {
                case PNG_FILTER_NONE:
                    textureData.data.push_back(imageBuffer[IMAGE_INDEX]);
                    break;
                case PNG_FILTER_SUB:
                {
                    uint8_t subData = 0;
                    if( column > pixelBytes ){
                        subData = textureData.data[CURRENT_DATA - pixelBytes];
                    }
                    textureData.data[CURRENT_DATA] = imageBuffer[IMAGE_INDEX] + subData;
                }
                    break;
                case PNG_FILTER_UP:
                {
                    uint8_t upData = 0;
                    if(row != 0){
                        upData = textureData.data[CURRENT_DATA + rowDataBytes];
                    }
                    textureData.data[CURRENT_DATA] = (imageBuffer[IMAGE_INDEX] + upData);
                }
                    break;
                case PNG_FILTER_AVERAGE:
                {
                    uint16_t average = 0;
                    if( column > pixelBytes ){
                        average += textureData.data[CURRENT_DATA - pixelBytes];
                    }
                    if( row != 0){
                        average += textureData.data[CURRENT_DATA + rowDataBytes];
                    }
                    
                    average *= 0.5;
                    textureData.data[CURRENT_DATA] = (imageBuffer[IMAGE_INDEX] + (uint8_t)average);
                }
                    break;
                case PNG_FILTER_PAETH:
                {
                    uint8_t l = 0;
                    uint8_t t = 0;
                    uint8_t tl = 0;
                    bool hasLeft = false ;
                    if( column > pixelBytes ){
                        l = textureData.data[CURRENT_DATA - pixelBytes];
                        hasLeft = true ;
                    }
                    if( row != 0 ){
                        t = textureData.data[CURRENT_DATA + rowDataBytes];
                        if(hasLeft){
                            tl = textureData.data[CURRENT_DATA - pixelBytes + rowDataBytes];
                        }
                    }
                    textureData.data[CURRENT_DATA] = (imageBuffer[IMAGE_INDEX] + paethPredictor(l, t, tl));
                }
                    break;
                default:
                    std::cout << "ERROR: (PNGLoader) フィルターが存在しません" << std::endl;
                    textureData.data[CURRENT_DATA] = (imageBuffer[IMAGE_INDEX]);
                    break;
            }
        }
    }
    
    textureData.data.resize(imageSize);
    
    textureData.width = info.width;
    textureData.height = info.height;
    switch (info.colorType) {
        case 2:
            if( info.bitDepth == 8 ){
                textureData.iformat = GL_RGB8;
            }
            else if( info.bitDepth == 16 ){
                textureData.iformat = GL_RGB16;
            }
            textureData.format = GL_RGB;
            break;
        case 6:
            if( info.bitDepth == 8 ){
                textureData.iformat = GL_RGBA8;
            }
            else if( info.bitDepth == 16 ){
                textureData.iformat = GL_RGBA16;
            }
            textureData.format = GL_RGBA;
        default:
            break;
    }
    
    textureData.type = TEXTURE_TYPE::PNG;
}

PNGLoader::HeaderInfo PNGLoader::getHeaderInfo(const uint8_t* data){
    int offset = SIGNATURE_SIZE + LENGTH_SIZE + CHUNK_TYPE_SIZE;
    
    return {
        GetBig(data, offset, IHDR_WIDTH_SIZE),
        GetBig(data, offset += IHDR_WIDTH_SIZE, IHDR_HEIGHT_SIZE),
        data[offset += IHDR_HEIGHT_SIZE],
        data[offset += IHDR_BIT_DEPTH_SIZE],
        data[offset += IHDR_COLOR_TYPE_SIZE],
        data[offset += IHDR_CM_SIZE],
        data[offset += IHDR_FM_SIZE],
    };
}

size_t PNGLoader::getPixelBytes(const PNGLoader::HeaderInfo &info){
    switch (info.colorType) {
        case 2: return 3 * (info.bitDepth / 8);
        case 6: return 4 * (info.bitDepth / 8);
        default: return 0;
    }
}

void PNGLoader::readIDATData(const uint8_t *data, int offset, size_t length, std::vector<uint8_t> &buf){
    offset += LENGTH_SIZE + CHUNK_TYPE_SIZE;
    buf.resize(length);
    for( int i = 0; i < length; ++i){
        buf[i] = data[offset + i];
    }
}

/**
 * Peath取得
 *
 * @param l     左
 * @param t     上
 * @param tl    左上
 */
float PNGLoader::paethPredictor(float l, float t, float tl){
    float p = l + t - tl;
    float pl = abs(p - l);
    float pt = abs(p - t);
    float ptl = abs(p - tl);
    if( pl <= pt && pl <= ptl ){
        return l;
    } else if( pt <= ptl ){
        return t;
    } else {
        return tl;
    }
}


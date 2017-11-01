//
//  TextureLoad.h
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/20.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef TextureLoad_h
#define TextureLoad_h

#include <GL/glew.h>
#include <vector>
#include <map>
#include <sys/stat.h>

enum class TEXTURE_TYPE{
    BMP,
    PNG,
    
    NONE,
};

struct TextureData {
    //画像種類
    TEXTURE_TYPE type = TEXTURE_TYPE::NONE;
    //データ形式
    GLenum iformat;
    //読み込み方法
    GLenum format;
    
    //画像幅
    int width;
    //画像高さ
    int height;
    //画像データへのアドレス
    std::vector<uint8_t>data;
};

class Loader;

class TextureLoader {
public:
    static TextureLoader& getInstance();
    
    void loadFile( const char* filename, TextureData& textureData );
    
private:
    TextureLoader();
    ~TextureLoader();
    //画像種類チェック
    TEXTURE_TYPE checkType( const std::vector<uint8_t>& buf, const struct stat& st );
    std::map<TEXTURE_TYPE, Loader*> loaderes;
    
};

class Loader {
public:
    virtual ~Loader(){}
public:
    virtual void loadFile( const std::vector<uint8_t>& buf, TextureData& textureData  ) = 0;
};

class BMPLoader : public Loader{
public:
    void loadFile(const std::vector<uint8_t>& buf, TextureData& textureData ) override;
};

class PNGLoader : public Loader{
public:
    void loadFile(const std::vector<uint8_t>& buf, TextureData& textureData ) override;

private:
    struct HeaderInfo{
        uint32_t width;
        uint32_t height;
        uint32_t bitDepth;
        uint32_t colorType;
        uint32_t compressionMethod;
        uint32_t filteMethod;
        uint32_t interlaceMethod;
    };
    
    HeaderInfo getHeaderInfo(const uint8_t* data);
    /**
     * IDATAデータ読み込み関数
     *
     * @param data      読み込み元データへのアドレス
     * @param offset    読み込み開始位置
     * @param length    読み込む長さ
     * @param buf       読み込み先配列
     */
    void readIDATData(const uint8_t* data, int offset, size_t length, std::vector<uint8_t>& buf);
    //1ピクセルのバイト数取得
    size_t getPixelBytes(const HeaderInfo& info);
    
    /**
     * フィルター解除関数
     *
     * @param origin    元データ
     * @param info      ヘッダー情報
     * @param storage   保存先
     */
    void defiltering( std::vector<uint8_t>& origin,const HeaderInfo& info, std::vector<uint8_t>& storage);
    
    /**
     * Peath取得
     *
     * @param l     左
     * @param t     上
     * @param tl    左上
     */
    float paethPredictor(float l, float t, float tl);
};


#endif /* TextureLoad_h */

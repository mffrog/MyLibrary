//
//  main.cpp
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/08.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "Constants.h"
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "OffscreenBuffer.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <math.h>

#define TEXTURE_NAME    "Res/cosmo.png"
#define IMAGE_FILE_TOROID   "Res/Toroid/Toroid.bmp"
#define MODEL_FILE_TOROID   "Res/Toroid/Toroid.fbx"
#define IMAGE_FILE_BULLET   "Res/Model/tama.png"
#define MODEL_FILE_BULLET   "Res/Model/tama.fbx"

//==================================================================

float clamp( float val ){
    if( val > 1 ){
        return 1 ;
    }
    if( val < 0 ){
        return 0 ;
    }
    return val ;
}

//転地変換
void transpose( glm::mat4& mat ){
    glm::vec4 vector ;
    glm::mat4 result ;
    for( int i = 0; i < 4; ++i ){
        vector = mat[i];
        for( int j = 0; j < 4 ; ++j ){
            result[j][i] = vector[j];
        }
    }
    mat = result ;
}

//==================================================================

///頂点データ型
struct Vertex{
    glm::vec3 position;     ///座標
    glm::vec4 color;        ///色
    glm::vec2 texCoord;     ///テクスチャ座標
    glm::vec3 normal;       ///法線
};

float length = sqrt(2) * 0.5 ;
///頂点データ
const Vertex vertices[] = {
    { {-0.5f, -0.3f,  0.5f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 0.3f, -0.3f,  0.5f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 0.3f,  0.5f,  0.5f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    
    { {-0.3f,  0.3f,  0.1f} , {0.0f, 0.0f, 1.0f, 1.0f}, { 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    { {-0.3f, -0.5f,  0.1f} , {0.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.1f} , {0.0f, 0.0f, 1.0f, 1.0f}, { 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.1f} , {1.0f, 0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.3f,  0.1f} , {1.0f, 1.0f, 0.0f, 1.0f}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    { {-0.3f,  0.3f,  0.1f} , {1.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f} ,{0.0f, 0.0f, 1.0f} },
    
    { {-960.0f, -600.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 960.0f, -600.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { { 960.0f,  600.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    { {-960.0f,  600.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    
    { { -1.0f, -1.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { {  1.0f, -1.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
    { {  1.0f,  1.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
    { { -1.0f,  1.0f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
};

///インデックスデータ
const GLuint indices[] = {
    0, 1, 2, 2, 3, 0,
    
    4, 5, 6, 7, 8, 9,
    
    10, 11, 12, 12, 13, 10,
    
    14,15,16,16,17,14,
};

/**
 *頂点シェータのパラメータ型
 */
struct VertexData{
    glm::mat4 matMVP;
    glm::vec4 matPos;
};

/**
 * ライトデータ(点光源)
 */
struct PointLight{
    glm::vec4 position;     ///座標(ワールド座標)
    glm::vec4 color;        ///明るさ
};

const int maxLightCount = 4;    ///ライトの数

/**
 * ライティングパラメータ
 */
struct LightData{
    glm::vec4 ambinetColor;     ///環境光
    PointLight light[maxLightCount];  ///ライトのリスト
};

/**
 * ポストエフェクトデータ
 */
struct PostEffectData{
    glm::mat4x4 matColor;   ///色変換行列
};

/**
 * 部分描画データ
 */
struct RenderingPart{
    GLsizei size;       ///描画するインデックス数
    GLvoid* offset;     ///描画開始インデックスのバイトオフセット
};

/**
 * RenderingPart を作成する
 *
 * @param   size    描画するインデックス数
 * @param   offset  描画開始インデックス数のオフセット（インデックス単位）
 *
 * @return  作成した部分描画データ
 */
RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset){
    return {size, reinterpret_cast<GLvoid*>(offset* sizeof(GLuint))};
}

/**
 * 部分描画データリスト
 */
static const RenderingPart renderingParts[] = {
    MakeRenderingPart(12, 0),
    MakeRenderingPart(6, 12),
    MakeRenderingPart(6, 18),
};

/**
 *  Vertex Buffer Onject を作成する
 *
 * @param   size    頂点データのサイズ
 * @param   data    頂点データへのポインタ
 *
 * @return  作成した VBO
 */
GLuint CreateVBO( GLsizeiptr size,const GLvoid* data ){
    GLuint vbo = 0;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo ;
}


///インデックスデータを使うことでおんなじデータを複数作らなくてすむ
/**
 * Index Buffer Objectを作成する
 *
 * @param   size    インデックスデータのサイズ
 * @param   data    インデックスデータへのポインタ
 *
 * @return  作成したIBO
 */
GLuint CreateIBO( GLsizeiptr size, const GLvoid* data ){
    GLuint ibo = 0;
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    return ibo;
}

/**
 *  頂点アトリビュートを設定する
 *
 * @param   index   頂点アトリビュートのインデックス
 * @param   cls     頂点データ型名
 * @param   mbr     頂点アトリビュートに設定する cls のメンバ変数名
 */
#define SetVertexAttribPointer( index, cls, mbr ) SetVertexAttribPointerI ( \
index, \
sizeof( cls::mbr ) / sizeof( float ), \
sizeof(cls),\
reinterpret_cast<GLvoid*>(offsetof(cls,mbr)))

void SetVertexAttribPointerI( GLuint index, GLint size, GLsizei stride, const GLvoid* pointer ){
    glEnableVertexAttribArray(index);
    glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );
}

/**
 *  Vertex Array Object を作成する
 *
 * @param   vbo VAO に関連づけられる VBO
 * @param   ibo VAO に関連づけられる IBO
 *
 * @return  作成した VAO
 */
GLuint CreateVAO( GLuint vbo, GLuint ibo){
    GLuint vao = 0 ;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    SetVertexAttribPointer(0, Vertex, position);
    SetVertexAttribPointer(1, Vertex, color);
    SetVertexAttribPointer(2, Vertex, texCoord);
    SetVertexAttribPointer(3, Vertex, normal);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(0);
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ibo);
    return vao ;
}

/**
 * Uniform Block Object を作成する
 *
 * @param size  Uniform Block のサイズ
 * @param data  Uniform Block に転送するデータへのポインタ
 *
 * @return 作成したUBO
 */
GLuint CreateUBO(GLsizeiptr size, const GLvoid* data = nullptr){
    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return ubo;
}

int main(int argc, const char * argv[]) {
    
    GLFWEW::Window& window = GLFWEW::Window::Instance() ;
    if( !window.Init(800, 600, "OpenGL_Tutorial") ){
        return 1;
    }
    
    const GLuint vbo = CreateVBO( sizeof(vertices), vertices);
    const GLuint ibo = CreateIBO( sizeof(indices), indices);
    const GLuint vao = CreateVAO(vbo, ibo);
    const UniformBufferPtr uboVertex = UniformBuffer::Create(sizeof(VertexData), 0, "VertexData");
    const UniformBufferPtr uboLight = UniformBuffer::Create(sizeof(LightData), 1, "LightData");
    const UniformBufferPtr uboPostEffect = UniformBuffer::Create(sizeof(PostEffectData), 2, "PostEffectData");
    const Shader::ProgramPtr progTutorial = Shader::Program::Create
    ("/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Tutorial.vert",
     "/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Tutorial.frag");
    const Shader::ProgramPtr progColorFilter = Shader::Program::Create
    ("/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/ColorFilter.vert",
    "/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/ColorFilter.frag");
    const Shader::ProgramPtr progPostShader = Shader::Program::Create
    ("/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/Posterization/Posterization.vert",
     "/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/Posterization/Posterization.frag");
    const Shader::ProgramPtr progCellShader = Shader::Program::Create
    ("/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/CellShader/CellShader.vert",
     "/Users/tomoyafujii/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/ShaderCode/CellShader/CellShader.frag");
    if( !vbo || !ibo || !vao || !uboVertex || !uboLight || !uboPostEffect || !progTutorial || !progColorFilter || !progPostShader ){
        return 1;
    }
    
    Mouse& mouse = Mouse::getInstance();
    
    auto func = [](Mouse::MouseEvent* event){
        //std::cout << "Cursor  posX:" << event->position.m_x << " posY:" << event->position.m_y << std::endl;
    };
    
    mouse.setCallback(Mouse::CALLBACK_TYPE::MOVE, func);
    bool endFlag = false;

    progTutorial->UniformBlockBinding("VertexData",0);
    progTutorial->UniformBlockBinding("LightData", 1);
    progColorFilter->UniformBlockBinding("PostEffectData", 2);
    progCellShader->UniformBlockBinding("VertexData", 0);
    progCellShader->UniformBlockBinding("LightData", 1);
    
    /*
    //テクスチャデータ
    static const uint32_t textureData[] = {
        0xffffffff, 0xffcccccc, 0xffffffff, 0xffcccccc, 0xffffffff,
        0xff888888, 0xffffffff, 0xff888888, 0xffffffff, 0xff888888,
        0xffffffff, 0xff444444, 0xffffffff, 0xff444444, 0xffffffff,
        0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000,
        0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0xffffffff,
    };
    
    TexturePtr tex = Texture::Create(5, 5, GL_RGBA8, GL_RGBA, textureData);
    */
    TexturePtr tex = Texture::LoadFromFile(TEXTURE_NAME);
    TexturePtr texToroid = Texture::LoadFromFile(IMAGE_FILE_TOROID);
    TexturePtr texBullet = Texture::LoadFromFile(IMAGE_FILE_BULLET);
    if(!tex || !texToroid || !texBullet){
        return 1;
    }
    
    Mesh::BufferPtr meshBuffer = Mesh::Buffer::Create(10 * 1024, 30 * 1024);
    if( !meshBuffer ){
        return 1;
    }
    
    meshBuffer->LoadMeshFromFile(MODEL_FILE_TOROID);
    meshBuffer->LoadMeshFromFile(MODEL_FILE_BULLET);
    
    bool createFlag = false ;
    Keyboard& keyboard = Keyboard::getInstance();
    auto keyFunc = [&](Keyboard::KeyType type){
        if( Keyboard::KeyType::ESC == type ){
            endFlag = true ;
        }
        if( Keyboard::KeyType::A == type ){
            createFlag = true;
        }
        std::cout << "keyCode is " << (int)type << std::endl;
    };
    
    keyboard.setCallback(Keyboard::FUNC_TYPE::PRESSED, keyFunc);
    
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    
    const OffscreenBufferPtr offscreen = OffscreenBuffer::Create(1600, 1200);
    if(!offscreen){
        return 1;
    }

    int controllerId = -1;
    ControllerData* data = nullptr;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    //座標
    glm::vec3 position(0.0);
    //スピード
    float speed = 3.5f;
    //視点
    glm::vec3 viewPoint(0.0);
    //カメラ位置
    glm::vec3 viewPos(0.0,0.0,10);
    //カメラ距離
    float viewDistance = 10;
    //カメラ回転速度
    float cameraRotateSpeed = 2.0;
    //背景距離
    float bgDistance = 500;
    //背景幅
    float bgWidth = 1920;
    float bgHeight = 1200;
    //回転角
    float cameraHoriRotate = 0;
    float cameraVirtRotate = 0;
    const float cameraVirtRotateMax = 85.0f;
    
    //弾向きベクトル
    glm::vec3 bulletDirection(1.0,0.0,0.0);
    glm::vec3 bulletTopDire(0.0,1.0,0.0);
    //メインループ
    while (!window.ShouldClose() && !endFlag ) {
        
#ifdef USE_CONTROLLER
        if( controllerId == -1 ){
            controllerId = ControllerManager::getInstance().getEnableId();
            data = ControllerManager::getInstance().getJoystick(controllerId);
        }
        ControllerManager::getInstance().update();
#endif
        
        glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFrameBuffer());
        glClearColor(0.0f, 0.0f, 0.0f, .0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        static float degree = 0.0f;
        degree += 0.5f;
        if(degree >= 360.0f) { degree -= 360.0f; }
        
        glm::mat4 rotation(1.0);
        rotation[0][0] = cosf(degree * M_PI / 180);
        rotation[0][2] = sinf(degree * M_PI / 180);
        rotation[2][0] = -sinf(degree * M_PI / 180);
        rotation[2][2] = cosf(degree * M_PI / 180);
        transpose(rotation);
        
        progTutorial->UseProgram();
        glm::vec3 viewDire = viewPoint - viewPos ;

        if( data ){
            cameraHoriRotate += data->getRigthJoystick().axisX * cameraRotateSpeed;
            if( fabsf(cameraVirtRotate + data->getRigthJoystick().axisY * cameraRotateSpeed ) > cameraVirtRotateMax ){
                cameraVirtRotate = cameraVirtRotate >= 0 ? cameraVirtRotateMax : -cameraVirtRotateMax ;
            }
            else {
                cameraVirtRotate += data->getRigthJoystick().axisY * cameraRotateSpeed;
            }
            viewDire = glm::normalize(viewDire);
            glm::vec2 rightVec(viewDire[0],viewDire[2]);
            rightVec = glm::normalize(rightVec);
            
            speed = 3.5f;
            if( data->getPressed(PS4_L1_BUTTON) ){
                speed *= 2;
            }
            
            position[0] -= data->getLeftJoystick().axisX * rightVec[1] * speed;
            position[2] -= data->getLeftJoystick().axisX * -rightVec[0] * speed;
            position -= data->getLeftJoystick().axisY * viewDire * speed;

            viewPos[0] = viewDistance * -sinf(cameraHoriRotate * M_PI / 180) + position[0];
            viewPos[2] = viewDistance * cosf(cameraHoriRotate * M_PI / 180) + position[2];
            viewPos[1] = viewDistance * sinf(cameraVirtRotate * M_PI / 180) + position[1];
            viewPoint = position;

            if( data->getPressed(PS4_R3_BUTTON) ){
                viewPoint[0] = position[0];
                viewPoint[1] = position[1];
            }
            if( data->getPressed(PS4_L3_BUTTON) ){
                viewPos[0] = position[0];
                viewPos[1] = position[1];
                cameraHoriRotate = 0;
                cameraVirtRotate = 0;
                viewPoint[0] = position[0];
                viewPoint[1] = position[1];
            }

            endFlag = data->getPressed(PS4_OPTIONS_BUTTON);
        }
        
        const glm::mat4x4 matProj =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 7.5f, 5000.0f);
        const glm::mat4x4 matView =
        glm::lookAt(viewPos, viewPoint, glm::vec3(0, 1, 0));
        glm::mat4 pos(1.0);
        
        pos[3][0] = position[0];
        pos[3][1] = position[1];
        pos[3][2] = position[2] -10;
        VertexData vertexData;
        vertexData.matMVP = matProj * matView * pos * rotation;
        uboVertex->BufferSubData(&vertexData);
        
        LightData lightData;
        
        lightData.ambinetColor = glm::vec4(0.05f, 0.1f, 0.2f, 1);
        lightData.light[0].position = glm::vec4(1, 1, 1, 1);
        lightData.light[0].color    = glm::vec4(2, 2, 2, 1);
        lightData.light[1].position = glm::vec4(-0.2f, 0.0f, 0.6f, 1.0f);
        lightData.light[1].color    = glm::vec4(0.125f, 0.125f, 0.05f, 1.0f);
 
        lightData.ambinetColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        uboLight->BufferSubData(&lightData);
        
        VertexData modelData = vertexData;
        LightData modellightData = lightData;
        
        glBindVertexArray(vao);
        progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, tex->Id());
        
        pos = glm::mat4(1.0);
        pos[3][2] = -bgWidth * 0.5f;
        /*
        pos[3][0] = bgDistance * viewDire[0] + viewPos[0];
        pos[3][1] = bgDistance * viewDire[1] + viewPos[1];
        pos[3][2] = bgDistance * viewDire[2] + viewPos[2];
        */
        
        rotation = glm::mat4(1.0);
        vertexData.matMVP = matProj * matView * pos * rotation;
        uboVertex->BufferSubData(&vertexData);

        glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
        
        rotation[0][0] = cosf(M_PI/2);
        rotation[0][2] = sinf(M_PI/2);
        rotation[2][0] = -sinf(M_PI/2);
        rotation[2][2] = cosf(M_PI/2);
        transpose(rotation);
        pos[3][0] = -bgWidth * 0.5;
        pos[3][2] = 0;
        
        vertexData.matMVP = matProj * matView * pos * rotation;
        uboVertex->BufferSubData(&vertexData);
        glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

        rotation[0][0] = cosf(-M_PI/2);
        rotation[0][2] = sinf(-M_PI/2);
        rotation[2][0] = -sinf(-M_PI/2);
        rotation[2][2] = cosf(-M_PI/2);
        transpose(rotation);
        pos[3][0] = bgWidth * 0.5f;
        pos[3][2] = 0;
        
        vertexData.matMVP = matProj * matView * pos * rotation;
        uboVertex->BufferSubData(&vertexData);
        glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
        
        rotation[0][0] = cosf(M_PI);
        rotation[0][2] = sinf(M_PI);
        rotation[2][0] = -sinf(M_PI);
        rotation[2][2] = cosf(M_PI);
        transpose(rotation);
        pos[3][0] = 0;
        pos[3][2] = bgWidth * 0.5;
        
        vertexData.matMVP = matProj * matView * pos * rotation;
        uboVertex->BufferSubData(&vertexData);
        glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
        
        progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, texBullet->Id());
        uboVertex->BufferSubData(&modelData);
        uboLight->BufferSubData(&modellightData);
        glDepthMask(GL_FALSE);
        //glDisable(GL_DEPTH_TEST);
        glm::vec4 viewDirection = modelData.matMVP * glm::vec4(bulletDirection,1.0);
        meshBuffer->BindVAO();
        meshBuffer->GetMesh("pCylinder1")->Draw(meshBuffer);
        
        meshBuffer->GetMesh("pPlane1")->Draw(meshBuffer);
        meshBuffer->GetMesh("pPlane2")->Draw(meshBuffer);
        meshBuffer->GetMesh("pPlane3")->Draw(meshBuffer);

        //glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        
        glBindVertexArray(vao);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());
        
        /*
        progPostShader->UseProgram();
        progPostShader->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());
        */
        vertexData = {};
        
        /*
        vertexData.matMVP[0] = glm::vec4(0.5f, 0, 0, 0);
        vertexData.matMVP[1] = glm::vec4(0, 0.5f, 0, 0);
        vertexData.matMVP[2] = glm::vec4(0, 0, 0.5f, 0);
        vertexData.matMVP[3] = glm::vec4(0, 0, 0, 1);
        */
        
        uboVertex->BufferSubData(&vertexData);
         
        lightData = {};
        lightData.ambinetColor = glm::vec4(1.0f);
        uboLight->BufferSubData(&lightData);
        /*
        progColorFilter->UseProgram();
        progColorFilter->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());
        */
        PostEffectData postEffect;
        
        /*
        //セピア
        postEffect.matColor[0] = glm::vec4(0.393f, 0.349f, 0.272f, 0);
        postEffect.matColor[1] = glm::vec4(0.769f, 0.686f, 0.534f, 0);
        postEffect.matColor[2] = glm::vec4(0.189f, 0.168f, 0.131f, 0);
        postEffect.matColor[3] = glm::vec4(0, 0, 0, 1);
        */
        
        //モノトーンにしたい
        glm::vec4 mono(0.299, 0.587, 0.114, 0);
        postEffect.matColor[0] = mono;//glm::vec4(0.299, 0.299, 0.299, 0);
        postEffect.matColor[1] = mono;//glm::vec4(0.587, 0.587, 0.587, 0);;
        postEffect.matColor[2] = mono;//glm::vec4(0.114, 0.114, 0.114, 0);;
        postEffect.matColor[3] = glm::vec4(0, 0, 0, 1);
        transpose(postEffect.matColor);
        
        /*
        //無変換
        postEffect.matColor[0] = glm::vec4(1, 0, 0, 0);
        postEffect.matColor[1] = glm::vec4(0, 1, 0, 0);
        postEffect.matColor[2] = glm::vec4(0, 0, 1, 0);
        postEffect.matColor[3] = glm::vec4(0, 0, 0, 1);
        */
        
        /*
        //ネガポジ反転
        postEffect.matColor[0] = glm::vec4(-1, 0, 0, 1);
        postEffect.matColor[1] = glm::vec4( 0,-1, 0, 1);
        postEffect.matColor[2] = glm::vec4( 0, 0,-1, 1);
        postEffect.matColor[3] = glm::vec4( 0, 0, 0, 1);
        transpose(postEffect.matColor);
        */
        
        
        uboPostEffect->BufferSubData(&postEffect);
        
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, renderingParts[2].size, GL_UNSIGNED_INT, renderingParts[2].offset);
        glEnable(GL_DEPTH_TEST);
        window.SwapBuffers() ;
    }
    
    glDeleteVertexArrays(1,&vao);
    
    return 0;
}

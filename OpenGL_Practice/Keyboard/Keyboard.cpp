//
//  Keyboard.cpp
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "Keyboard.h"
/**
 * インスタンス取得
 */
Keyboard& Keyboard::getInstance(){
    static Keyboard instance;
    return instance;
}
/**
 * コンストラクタ
 */
Keyboard::Keyboard(): keyFunctions(3,nullptr){
}

/**
 * コールバック設定関数
 *
 * @param type  種類
 * @param func  コールバック関数
 */
void Keyboard::setCallback( Keyboard::FUNC_TYPE type, std::function<void(KeyType)> func ){
    keyFunctions[(int)type] = func;
}

//キーに変化があった時に呼び出される
void Keyboard::keyCallback(int key, int scancode, int action, int mods){
    switch (action) {
        case GLFW_PRESS:
            if( keyFunctions[(int)Keyboard::FUNC_TYPE::PRESSED] ){
                keyFunctions[(int)Keyboard::FUNC_TYPE::PRESSED](changeToKeyType(key));
            }
            break;
        case GLFW_RELEASE:
            if( keyFunctions[(int)Keyboard::FUNC_TYPE::RELEASED] ){
                keyFunctions[(int)Keyboard::FUNC_TYPE::RELEASED](changeToKeyType(key));
            }
        case GLFW_REPEAT:
            if( keyFunctions[(int)Keyboard::FUNC_TYPE::REPEATED] ){
                keyFunctions[(int)Keyboard::FUNC_TYPE::REPEATED](changeToKeyType(key));
            }
        default:
            break;
    }
}

//キータイプに変更
Keyboard::KeyType Keyboard::changeToKeyType(int key){
    switch (key) {
        case GLFW_KEY_0: return KeyType::KEY_0;
        case GLFW_KEY_1: return KeyType::KEY_1;
        case GLFW_KEY_2: return KeyType::KEY_2;
        case GLFW_KEY_3: return KeyType::KEY_3;
        case GLFW_KEY_4: return KeyType::KEY_4;
        case GLFW_KEY_5: return KeyType::KEY_5;
        case GLFW_KEY_6: return KeyType::KEY_6;
        case GLFW_KEY_7: return KeyType::KEY_7;
        case GLFW_KEY_8: return KeyType::KEY_8;
        case GLFW_KEY_9: return KeyType::KEY_9;
            
        case GLFW_KEY_Q: return KeyType::Q;
        case GLFW_KEY_W: return KeyType::W;
        case GLFW_KEY_E: return KeyType::E;
        case GLFW_KEY_R: return KeyType::R;
        case GLFW_KEY_T: return KeyType::T;
        case GLFW_KEY_Y: return KeyType::Y;
        case GLFW_KEY_U: return KeyType::U;
        case GLFW_KEY_I: return KeyType::I;
        case GLFW_KEY_O: return KeyType::O;
        case GLFW_KEY_P: return KeyType::P;
            
        case GLFW_KEY_A: return KeyType::A;
        case GLFW_KEY_S: return KeyType::S;
        case GLFW_KEY_D: return KeyType::D;
        case GLFW_KEY_F: return KeyType::F;
        case GLFW_KEY_G: return KeyType::G;
        case GLFW_KEY_H: return KeyType::H;
        case GLFW_KEY_J: return KeyType::J;
        case GLFW_KEY_K: return KeyType::K;
        case GLFW_KEY_L: return KeyType::L;

        case GLFW_KEY_Z: return KeyType::Z;
        case GLFW_KEY_X: return KeyType::X;
        case GLFW_KEY_C: return KeyType::C;
        case GLFW_KEY_V: return KeyType::V;
        case GLFW_KEY_B: return KeyType::B;
        case GLFW_KEY_N: return KeyType::N;
        case GLFW_KEY_M: return KeyType::M;
            
        case GLFW_KEY_SPACE:    return KeyType::SPACE;
            
        case GLFW_KEY_RIGHT_SHIFT:  return KeyType::RIGHT_SHIFT;
        case GLFW_KEY_LEFT_SHIFT:   return KeyType::LEFT_SHIFT;

        case GLFW_KEY_ESCAPE:   return KeyType::ESC;
            
        default: return KeyType::NONE;
    }
}

//
//  Keyboard.h
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef Keyboard_h
#define Keyboard_h

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

class Keyboard {
public:
    enum class KeyType : int{
        NONE = -1,
        KEY_0 = 0,
        KEY_1 = 1,
        KEY_2 = 2,
        KEY_3 = 3,
        KEY_4 = 4,
        KEY_5 = 5,
        KEY_6 = 6,
        KEY_7 = 7,
        KEY_8 = 8,
        KEY_9 = 9,
        Q = 10,
        W = 11,
        E = 12,
        R = 13,
        T = 14,
        Y = 15,
        U = 16,
        I = 17,
        O = 18,
        P = 19,
        A = 20,
        S = 21,
        D = 22,
        F = 23,
        G = 24,
        H = 25,
        J = 26,
        K = 27,
        L = 28,
        Z = 29,
        X = 30,
        C = 31,
        V = 32,
        B = 33,
        N = 34,
        M = 35,
        
        SPACE = 36,
        
        RIGHT_SHIFT = 37,
        LEFT_SHIFT = 38,
        
        ESC = 39,
    };
    
    enum class FUNC_TYPE{
        PRESSED = 0,
        RELEASED = 1,
        
        REPEATED = 2,
    };
    
public:
    
    /**
     * インスタンス取得
     */
    static Keyboard& getInstance();
    
    /**
     * コールバック設定関数
     *
     * @param type  種類
     * @param func  コールバック関数
     */
    void setCallback( Keyboard::FUNC_TYPE type, std::function<void(KeyType)> func );
    
    //キーに変化があった時に呼び出される
    void keyCallback(int key, int scancode, int action, int mods);
    
private:
    Keyboard();
    ~Keyboard() = default;
    //キータイプに変更
    KeyType changeToKeyType(int key);
    
    std::vector<std::function<void(KeyType)>> keyFunctions;
};

#endif /* Keyboard_h */

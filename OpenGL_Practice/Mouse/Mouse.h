//
//  Mouse.h
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef Mouse_h
#define Mouse_h

#include "Mathematics.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

class Mouse {
public:
    enum class CALLBACK_TYPE {
        CLICK       = 0,
        CLICK_END   = 1,
        MOVE        = 2,
        SCROLL      = 3,
    };
    
    enum class BUTTON_TYPE {
        LEFT,
        RIGHT,
        MIDDLE,
        BUTTON_1,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        
        NONE,
    };
    
    struct MouseEvent {
        BUTTON_TYPE buttonType;
        Vector2 position;
        Vector2 scroll;
    };
public:
    static Mouse& getInstance();
    
    float getX(){return x;}
    float getY(){return y;}
    
    /**
     * コールバック設定関数
     *
     * @param type      コールバックの種類
     * @param callback  コールバック関数
     */
    void setCallback(CALLBACK_TYPE type, std::function<void(MouseEvent*)> callback);
    
    //glfwに渡すコールバック
    void click(GLFWwindow* window, int button, int action, int mods);
    void move(GLFWwindow* window, double xpos, double ypos);
    void scroll(GLFWwindow* window, double xoffset, double yoffset);
    

private:
    Mouse();
    ~Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    
    /**
     * マウスボタン判定
     *
     * @param button     glfwから取得したボタン情報
     *
     * @return BUTTON_TYPE に変更した値
     */
    BUTTON_TYPE checkButton( int button );
    
    //マウス座標
    double x;
    double y;
    //コールバック
    std::vector<std::function<void(MouseEvent*)>> mouseCallbacks;
};

#endif /* Mouse_h */

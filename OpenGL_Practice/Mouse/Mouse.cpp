//
//  Mouse.cpp
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "Mouse.h"
/**
 * インスタンス取得
 */
Mouse& Mouse::getInstance(){
    static Mouse instance;
    return instance;
}
/**
 * コンストラクタ
 */
Mouse::Mouse() : mouseCallbacks(4,nullptr)
{
}

/**
 * コールバック設定関数
 *
 * @param type      コールバックの種類
 * @param callback  コールバック関数
 */
void Mouse::setCallback(CALLBACK_TYPE type, std::function<void(MouseEvent*)> callback){
    mouseCallbacks[(int)type] = callback;
}

//glfwに渡すコールバック
void Mouse::click(GLFWwindow* window, int button, int action, int mods){
    glfwGetCursorPos(window, &x, &y);
    MouseEvent event = {checkButton(button), Vector2(x,y)};
    if( action == GLFW_PRESS ){
        if( mouseCallbacks[(int)CALLBACK_TYPE::CLICK] ){
            mouseCallbacks[(int)CALLBACK_TYPE::CLICK](&event);
        }
    }
    else if( action == GLFW_RELEASE ){
        if( mouseCallbacks[(int)CALLBACK_TYPE::CLICK_END] ){
            mouseCallbacks[(int)CALLBACK_TYPE::CLICK_END](&event);
        }
    }
}
void Mouse::move(GLFWwindow* window, double xpos, double ypos){
    if( mouseCallbacks[(int)CALLBACK_TYPE::MOVE] != nullptr ){
    x = xpos;
    y = ypos;
    MouseEvent event = {checkButton(-1), Vector2(x,y), Vector2(0, 0)};
    mouseCallbacks[(int)CALLBACK_TYPE::MOVE](&event);
    }
}
void Mouse::scroll(GLFWwindow* window, double xoffset, double yoffset){
    if(mouseCallbacks[(int)CALLBACK_TYPE::SCROLL] != nullptr){
        MouseEvent event = {checkButton(-1), Vector2(x,y), Vector2(0,0)};
        mouseCallbacks[(int)CALLBACK_TYPE::SCROLL](&event);
    }
}

/**
 * マウスボタン判定
 *
 * @param button     glfwから取得したボタン情報
 *
 * @return BUTTON_TYPE に変更した値
 */
Mouse::BUTTON_TYPE Mouse::checkButton( int button ){
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:    return Mouse::BUTTON_TYPE::LEFT;
        case GLFW_MOUSE_BUTTON_RIGHT:   return Mouse::BUTTON_TYPE::RIGHT;
        case GLFW_MOUSE_BUTTON_MIDDLE:  return Mouse::BUTTON_TYPE::MIDDLE;
        case GLFW_MOUSE_BUTTON_4: return Mouse::BUTTON_TYPE::BUTTON_1;
        case GLFW_MOUSE_BUTTON_5: return Mouse::BUTTON_TYPE::BUTTON_2;
        case GLFW_MOUSE_BUTTON_6: return Mouse::BUTTON_TYPE::BUTTON_3;
        case GLFW_MOUSE_BUTTON_7: return Mouse::BUTTON_TYPE::BUTTON_4;
        case GLFW_MOUSE_BUTTON_8: return Mouse::BUTTON_TYPE::BUTTON_5;

        default: return Mouse::BUTTON_TYPE::NONE;
    }
}

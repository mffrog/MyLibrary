//
//  Joystick.cpp
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "Controller.h"
#include "Constants.h"
#include <iostream>
#include <math.h>
#define MAX_JOYSTICK    GLFW_JOYSTICK_8
#define JOYSTICK_NUM    2
#define BUTTON_NUM      18
#define JOYSTICK_EPSILON         0.1

ControllerData::ControllerData(int Id):joystickId(Id),joystickes(JOYSTICK_NUM,{0,0}),buttons(BUTTON_NUM,false){}
void ControllerData::clear(){
    for( auto& joystick: joystickes ){
        joystick.axisX = 0;
        joystick.axisY = 0;
    }
    for( int i = 0; i < buttons.size() ; ++i ){
        buttons[i] = false;
    }
}
bool ControllerData::getPressed(int buttonNum){
    if( buttonNum < buttons.size() ){
        return buttons[buttonNum];
    }
    std::cerr << "ボタンが存在しません" << std::endl;
    return false;
}

/**
 * インスタンス取得
 */
ControllerManager& ControllerManager::getInstance(){
    static ControllerManager instance;
    return instance;
}
/**
 * コンストラクタ
 */
ControllerManager::ControllerManager(): enableId(MAX_JOYSTICK,false){
    for(int i = 0 ; i < 8; ++i){
        if( glfwJoystickPresent(i) == GLFW_TRUE){
            controllerData[i] = new ControllerData(i);
            enableId[i] = true;
        }
        else {
            controllerData[i] = nullptr ;
        }
    }
}
/**
 * デストラクタ
 */
ControllerManager::~ControllerManager(){
    auto itr = controllerData.begin();
    while( itr != controllerData.end() ){
        SAFE_DELETE(itr->second);
        itr++;
    }
}

/**
 * 使用可能なコントローラIDを取得
 */
int ControllerManager::getEnableId(){
    for( int i = 0; i < MAX_JOYSTICK; ++i){
        if( enableId[i] == true ){
            enableId[i] = false ;
            return i;
        }
    }
    return -1;
}

/**
 * 使用をやめる時呼び出す
 *
 * @param Id    使用していたコントローラのID
 */
void ControllerManager::stopUse(int Id){
    enableId[Id] = true;
    controllerData[Id]->clear();
}
/**
 * データのアドレス取得
 */
ControllerData* ControllerManager::getJoystick(int Id){
    return controllerData[Id];
}

/**
 * データ更新関数
 */
void ControllerManager::update(){
    for( int i = 0; i < MAX_JOYSTICK; ++i ){
        if( controllerData[i] ){
            if( !enableId[i] ){
                acquisition(i);
            }
        }
    }
}


//コントローラ接続時非接続時コールバック
void ControllerManager::joystickCallback(int joy, int event){

    if( event == GLFW_CONNECTED ){
        if(joy > MAX_JOYSTICK){
            std::cerr << "Joystickがすでに最大数接続されています" << std::endl;
            return;
        }
        addJoystick(joy);
    }
    else if (event == GLFW_DISCONNECTED){
        removeJoystick(joy);
    }
}


void ControllerManager::addJoystick(int joy){
    controllerData[joy] = new ControllerData(joy);
    enableId[joy] = true;
}

void ControllerManager::removeJoystick(int joy){
    SAFE_DELETE(controllerData[joy]);
    enableId[joy] = false;
}

/**
 * GLFWからデータ取得
 */
void ControllerManager::acquisition(int Id){
    int axisNum;
    int buttonNum;
    const float* joystickData = glfwGetJoystickAxes(Id, &axisNum);
    const unsigned char* buttons = glfwGetJoystickButtons(Id, &buttonNum);
    if( axisNum >= 4){
        controllerData[Id]->joystickes[JOYSTICK_LEFT].axisX = fabsf(joystickData[0]) > JOYSTICK_EPSILON ? joystickData[0] : 0;
        controllerData[Id]->joystickes[JOYSTICK_LEFT].axisY = fabsf(joystickData[1]) > JOYSTICK_EPSILON ? joystickData[1] : 0;
        controllerData[Id]->joystickes[JOYSTICK_RIGHT].axisX = fabsf(joystickData[2]) > JOYSTICK_EPSILON ? joystickData[2] : 0;
        controllerData[Id]->joystickes[JOYSTICK_RIGHT].axisY = fabsf(joystickData[3]) > JOYSTICK_EPSILON ? joystickData[3] : 0;
    }
    for( int i = 0; i < buttonNum; ++i ){
        if(buttons[i] == GLFW_PRESS){
            controllerData[Id]->buttons[i] = true;
            std::cout << "Button " << i << " : " << controllerData[Id]->buttons[i] << std::endl;
        }
        else if( buttons[i] == GLFW_RELEASE ){
            controllerData[Id]->buttons[i] = false;
        }
    }
}


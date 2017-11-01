//
//  GLFWEW.cpp
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/08.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "GLFWEW.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"
#include <iostream>

///GLFW と GLEW をラップするための名前空間
namespace GLFWEW {
    /**
     * @desc    GLFW からのエラー報告を処理する
     *
     * @param   error   エラー番号
     * @param   desc    エラー内容
     */
    void ErrorCallback( int error, const char* desc ){
        std::cerr << "ERROR: " << desc << std::endl;
    }
    
    /**
     * マウスクリックコールバック
     */
    void click(GLFWwindow* window, int button, int action, int mods){
        Mouse::getInstance().click(window, button, action, mods);
    }
    /**
     * マウス移動コールバック
     */
    void move(GLFWwindow* window, double xpos, double ypos){
        Mouse::getInstance().move(window, xpos, ypos);
    }
    /**
     * マウススクロールコールバック
     */
    void scroll(GLFWwindow* window, double xoffset, double yoffset){
        Mouse::getInstance().scroll(window, xoffset, yoffset);
    }
    
    /**
     * キーボードに変化があった時のコールバック
     */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods ){
        Keyboard::getInstance().keyCallback(key, scancode, action, mods);
    }
    
    /**
     * コントローラー接続非接続時コールバック
     */
    void controllerCallbakck( int joy, int event){
        ControllerManager::getInstance().joystickCallback(joy, event);
    }
    /**
     * @desc    シングルトンインスタンスを取得する
     *
     * @return Window のシングルトンインスタンス
     */
    Window& Window::Instance(){
        static Window instance ;
        return instance ;
    }
    
    /**
     * @desc    コンストラクタ
     */
    Window::Window() : isGLFWInitialize(false), isInitialized(false), window(nullptr){
    }
    
    /**
     * @desc    デストラクタ
     */
    Window::~Window(){
        if( isInitialized ){
            glfwTerminate();
        }
    }
    
    /**
     * @desc    GLFW/GLEW の初期化
     *
     * @param   width   ウィンドウの描画範囲の幅(ピクセル)
     * @param   height  ウィンドウの描画範囲の高さ(ピクセル)
     * @param   title   ウィンドウのタイトル( UTF-8 の 0 終端文字列 )
     *
     * @retval  true    初期化成功
     * @retval  false   初期化失敗
     */
    bool Window::Init(int width, int height, const char *title ){
        if( isInitialized ){
            std::cerr << "ERROR: GLFWEW は既に初期化されています。" << std::endl;
            return false ;
        }
        
        if( !isGLFWInitialize ){
            glfwSetErrorCallback(ErrorCallback);
            if( glfwInit() != GL_TRUE ){
                return false ;
            }
            isGLFWInitialize = true ;
        }
        
        // OpenGL Version 4.1 Core Profile を選択する
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        if( !window ){
            window = glfwCreateWindow(width, height, title, nullptr, nullptr);
            if( !window ){
                return false ;
            }
            glfwMakeContextCurrent(window);
        }
        
        if( glewInit() != GLEW_OK ){
            std::cerr << "ERROR: GLEW の初期化に失敗しました。" << std::endl;
            return false ;
        }
        
        glfwSwapInterval(1);
        
        //マウスコールバック設定
        glfwSetMouseButtonCallback(window, click);
        glfwSetCursorPosCallback(window, move);
        glfwSetScrollCallback(window, scroll);
        
        //キーボードコールバック設定
        glfwSetKeyCallback(window, keyCallback);
        
        //コントローラーコールバック
        glfwSetJoystickCallback(controllerCallbakck);

        const GLubyte* renderer = glGetString(GL_RENDERER);
        std::cout << "Renderer:\t" << renderer << std::endl;
        const GLubyte* version = glGetString(GL_VERSION);
        std::cout << "Version:\t" << version << std::endl;
        const GLubyte* SLversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout << "SLVersion:\t" << SLversion << std::endl;
        isInitialized = true ;
        
        return true ;
    }
    
    /**
     * @desc    ウィンドウを閉じるべきか調べる
     *
     * @retval  true    閉じる
     * @retval  false   閉じない
     */
    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(window) != 0 ;
    }
    
    /**
     * @desc    フロントバッファとバックバッファを切り替える
     */
    void Window::SwapBuffers() const {
        glfwPollEvents() ;
        glfwSwapBuffers(window);
    }
}// namespace GLFWEW

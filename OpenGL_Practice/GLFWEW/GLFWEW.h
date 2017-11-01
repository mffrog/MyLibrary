//
//  GLFWEW.h
//  OpenGL_Tutorial
//
//  Created by Tomoya Fujii on 2017/10/08.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef GLFWEW_h
#define GLFWEW_h


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GLFWEW {
    
    /**
     * GLFW と GLEW のラッパークラス
     */
    class Window {
    public:
        static Window& Instance();
        bool Init( int width, int height, const char* title );
        bool ShouldClose() const ;
        void SwapBuffers() const ;
        
    private:
        Window();
        ~Window();
        Window( const Window& ) = delete ;
        Window& operator = ( const Window& ) = delete ;
        
        bool isGLFWInitialize ;
        bool isInitialized ;
        GLFWwindow*  window ;
    };
} // namespace GLFWEW

#endif /* GLFWEW_h */

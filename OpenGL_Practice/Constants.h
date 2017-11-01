//
//  Constants.h
//  OpenGL_Practice
//
//  Created by Tomoya Fujii on 2017/10/23.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

#define SAFE_DELETE(p) if(p){ delete p; p = nullptr;}
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(a[0]))

//コントローラーを使用するときはコメントを解除
#define USE_CONTROLLER    1


#endif /* Constants_h */

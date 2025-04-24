#pragma once
//ベクトル
//2D------------------------------
#define VECTOR2D			(1)

//整数型
#if !defined(VECTOR2D_INT)
#define VECTOR2D_INT
#endif

//実数型
#if defined(VECTOR2D_FLOAT)
#define VECTOR2D_FLOAT
#endif

//位置
#if !defined(VECTOR2D_POSITION)
#define VECTOR2D_POSITION
#endif
//サイズ
#if !defined(VECTOR2D_SIZE)
#define VECTOR2D_SIZE
#endif
//図形
#if defined(VECTOR2D_FIGURE)
#define VECTOR2D_FIGURE
#endif
//---------------------------------





//3D-------------------------------
#define VECTOR3D			(2)

//整数型
#if defined(VECTOR3D_INT)
#define VECTOR3D_INT
#endif

//実数型
#if !defined(VECTOR3D_FLOAT)
#define VECTOR3D_FLOAT
#endif

//位置
#if !defined(VECTOR3D_POSITION)
#define VECTOR3D_POSITION
#endif
//サイズ
#if !defined(VECTOR3D_SIZE)
#define VECTOR3D_SIZE
#endif
//回転情報
#if !defined(VECTOR3D_ROT)
#define VECTOR3D_ROT
#endif
//方向
#if !defined(VECTOR3D_DIR)
#define VECTOR3D_DIR
#endif
//色
#if !defined(VECTOR3D_COLOR)
#define VECTOR3D_COLOR
#endif

//図形
#if !defined(VECTOR3D_FIGURE)
#define VECTOR3D_FIGURE
#endif

//---------------------------------
#pragma once
#include"VectorType.h"
#include<DxLib.h>

//2D
#ifdef VECTOR2D

#ifdef VECTOR2D_INT
struct Vector2
{
	int x;
	int y;
	static Vector2 VGet2D(int gx, int gy)
	{
		return {gx, gy};
	}

	bool operator==(const Vector2& vec) { return (x == vec.x && y == vec.y) ? true : false; };
};

//位置
#ifdef VECTOR2D_POSITION
typedef Vector2 Position2D;
#endif

//サイズ
#ifdef VECTOR2D_SIZE
typedef Vector2 Size2D;
#endif

#ifdef VECTOR2D_FIGURE
//線
typedef struct tagLine2D
{
	Vector2 start;
	Vector2 end;
	unsigned int color;
}Line2D;

//平面
typedef struct tagPlane2D
{
	Line2D top;
	Line2D right;
	Line2D down;
	Line2D left;
}Plane2D;

//円
typedef struct tagCircle
{
	Vector2 pos;
	unsigned int r;
}Circle2D;

//楕円
typedef struct tagOval
{
	Vector2 pos;
	Vector2 rX;
	Vector2 rY;
	unsigned int color;
}Oval2D;

//三角形
typedef struct tagTriangle
{
	Vector2 top;
	Vector2 left;
	Vector2 right;
	unsigned int color;
}Triangle;
#endif

#endif

#ifdef VECTOR2D_FLOAT
struct Vector2F
{
	float x;
	float y;
};

//位置
#ifdef VECTOR2D_POSITION
typedef Vector2F Position2DF;
#endif

//サイズ
#ifdef VECTOR2D_SIZE
typedef Vector2F Size2DF;
#endif

#ifdef VECTOR2D_FIGURE
//線
typedef struct tagLine2DF
{
	Vector2F start;
	Vector2F end;
	unsigned int color;
}Line2DF;

//平面
typedef struct tagPlane2DF
{
	Line2DF top;
	Line2DF right;
	Line2DF down;
	Line2DF left;
}Plane2DF;

//円
typedef struct tagCircleF
{
	Vector2F pos;
	unsigned int r;
}Circle2DF;

//楕円
typedef struct tagOvalF
{
	Vector2F pos;
	Vector2F rX;
	Vector2F rY;
	unsigned int color;
}Oval2DF;

//三角形
typedef struct tagTriangleF
{
	Vector2F top;
	Vector2F left;
	Vector2F right;
	unsigned int color;
}TriangleF;
#endif

#endif

#endif





//3D
#ifdef VECTOR3D

#ifdef VECTOR3D_INT
struct Vector3D
{
	int x;
	int y;
	int z;
};

//位置
#ifdef VECTOR3D_POSITION
typedef Vector3D Position3D;
#endif

//サイズ
#ifdef VECTOR3D_SIZE
typedef Vector3D Size3D;
#endif

#ifdef VECTOR3D_FIGURE
//線
typedef struct tagLine3D
{
	Vector3D start;
	Vector3D end;
	unsigned int color;
}Line3D;

//平面
typedef struct tagPlane3D
{
	Line3D top;
	Line3D right;
	Line3D down;
	Line3D left;
}Plane3D;

//箱
typedef struct tagBox
{
	Plane3D plane1;
	Plane3D plane2;
	Plane3D plane3;
	Plane3D plane4;
	Plane3D plane5;
	Plane3D plane6;
	unsigned int color;
}Box;

//球
typedef struct tagSphere
{
	Vector3D pos;
	unsigned int r;
}Sphere;

//カプセル
typedef struct tagCapsule
{
	Vector3D pos;
	Vector3D top;
	Vector3D down
	unsigned int color;
}Capsule;

//三角形
typedef struct tagTriangle3D
{
	Vector3D top;
	Vector3D left;
	Vector3D right;
	unsigned int color;
}Triangle3D;

#endif
#endif

#ifdef VECTOR3D_FLOAT

//位置
#ifdef VECTOR3D_POSITION
typedef VECTOR Position3DF;
#endif

//サイズ
#ifdef VECTOR3D_SIZE
typedef VECTOR Size3DF;
#endif

//回転情報
#ifdef VECTOR3D_ROT
typedef VECTOR Rot3DF;
#endif

#ifdef VECTOR3D_DIR
typedef VECTOR Dir3DF;
#endif

#ifdef VECTOR3D_COLOR
typedef VECTOR Color3D;
#endif

#ifdef VECTOR3D_FIGURE
//線
typedef struct tagLine3DF
{
	VECTOR start;
	VECTOR end;
	unsigned int color;
}Line3DF;

//平面
typedef struct tagPlane3DF
{
	Line3DF top;
	Line3DF right;
	Line3DF down;
	Line3DF left;
}Plane3DF;

//箱
typedef struct tagBoxF
{
	Plane3DF plane1;
	Plane3DF plane2;
	Plane3DF plane3;
	Plane3DF plane4;
	Plane3DF plane5;
	Plane3DF plane6;
	unsigned int color;
}BoxF;

//球
typedef struct tagSphereF
{
	VECTOR pos;
	unsigned int r;
}SphereF;

//カプセル
typedef struct tagCapsuleF
{
	VECTOR pos;
	VECTOR top;
	VECTOR down;
	unsigned int r;
	unsigned int color;
}CapsuleF;

//三角形
typedef struct tagTriangle3DF
{
	VECTOR top;
	VECTOR left;
	VECTOR right;
	unsigned int color;
}Triangle3DF;

#endif
#endif

#endif
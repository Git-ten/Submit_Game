#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Scene/GUI/Image.h"
#include "../Utility/VectorHeader.h"
#include "../Common/Quaternion.h"

class Utility
{
public:

	//2Dのゼロベクトル
	static constexpr Vector2 VECTOR2D_ZERO = {0, 0};
	static constexpr Vector2F VECTOR2D_ONE = {1.0f, 1.0f};

	static constexpr Vector2F DIR_UP = { 0, -1 };

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);


	//3Dのゼロベクトル
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };


	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };


	// 方向
	static constexpr Dir3DF DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr Dir3DF DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr Dir3DF DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr Dir3DF DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr Dir3DF DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr Dir3DF DIR_D = { 0.0f, -1.0f, 0.0f };


	// 四捨五入
	static int Round(float v);


	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);


	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);


	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);


	//独自追加機能
	// 度(deg)からラジアン(rad)
	static Rot3DF Deg2RadV(const Rot3DF& deg);
	// ラジアン(rad)から度(deg)
	static Rot3DF Rad2DegV(const Rot3DF& rad);


	// 0～360度の範囲に収める
	static double DegIn360(double deg);


	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);
	static float RadIn2PI(float rad);


	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	

	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Position2D Lerp(const Position2D& start, const Position2D& end, float t);
	static Position3DF Lerp(const Position3DF& start, const Position3DF& end, float t);


	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);


	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);


	// ベジェ曲線
	static Position2D Bezier(const Position2D& p1, const Position2D& p2, const Position2D& p3, float t);
	static Position3DF Bezier(const Position3DF& p1, const Position3DF& p2, const Position3DF& p3, float t);
	

	// Y軸回転
	static Position3DF RotXZPos(const Position3DF& centerPos, const Position3DF& radiusPos, float rad);


	// ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Position2D& v1, const Position2D& v2);
	static double Distance(const Position3DF& v1, const Position3DF& v2);


	// ベクトルを半分にする
	static Vector2 GetHalfVector2(const Vector2& v);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const Position3DF& pos1, float radius1, const Position3DF& pos2, float radius2);


	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const Position3DF& sphPos, float sphRadius,
		const Position3DF& capPos1, const Position3DF& capPos2, float capRadius);


	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);


	// 正規化
	static Dir3DF Normalize(const Vector2& v);
	static Dir3DF VNormalize(const VECTOR& v);


	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);


	//重心
	static VECTOR CentVector(const VECTOR& v1, const VECTOR& v2, const VECTOR& v3);
	static VECTOR CentVector(const VECTOR& v1, const VECTOR& v2);

	// 横方向の残りゲージ計算
	static void GageRateX(Image* image, const float rate, Position2D& rightPos, Size2D& size);

	// 描画系
	static void DrawLineDir(const Position3DF& pos, const Dir3DF& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const Position3DF& pos, const MATRIX& rot, float len = 50.0f);
	// 描画系
	static void DrawLineXYZ(const Position3DF& pos, const Quaternion& rot, float len = 50.0f);
	//ベクトル補正
	//通常、理想に近づくまで何度も呼び出す
	//徐々に理想に合わせる
	static void RelativeQuaternion(double speed, const Quaternion& relativeQua, Quaternion& nowQua);


	//乱数系
	static VECTOR RandomVector(const int max);


	//ベクトル補正
	//通常、理想に近づくまで何度も呼び出す
	//徐々に理想に合わせる
	static void RelativeVector(float speed, const VECTOR& relativeVec, VECTOR& nowVec);


	//時間系
	//秒単位を分単位にする
	static int Second2Minute(float second);

};


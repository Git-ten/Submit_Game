#pragma once
#include"../../Utility/VectorHeader.h"
#include <DxLib.h>

class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	//コンストラクタ
	Quaternion(void);
	Quaternion(const Rot3DF& rad);
	Quaternion(double w, double x, double y, double z);

	//デストラクタ
	~Quaternion(void);

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(const Rot3DF& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// 座標を回転させる
	static Position3DF PosAxis(const Quaternion& q, const Position3DF& pos);
	Position3DF PosAxis(const Position3DF& pos) const;

	// クォータニオンからオイラー角へ変換
	static Rot3DF ToEuler(const Quaternion& q);
	Rot3DF ToEuler(void) const;

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix(void) const;

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(const Dir3DF& dir);
	static Quaternion LookRotation(Dir3DF dir, Dir3DF up);

	// 行列からクォータニオンに変換
	static Quaternion GetRotation(MATRIX mat);

	// 基本ベクトルを取得
	Dir3DF GetForward(void) const;
	Dir3DF GetBack(void) const;
	Dir3DF GetRight(void) const;
	Dir3DF GetLeft(void) const;
	Dir3DF GetUp(void) const;
	Dir3DF GetDown(void) const;

	// 内積
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	// 正規化
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized(void) const;
	void Normalize(void);

	// 逆クォータニオン
	Quaternion Inverse(void) const;

	// 球面補間
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	// ２つのベクトル間の回転量を取得する
	static Quaternion FromToRotation(const Dir3DF& fromDir, const Dir3DF& toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	// 対象方向の回転
	void ToAngleAxis(float* angle, VECTOR* axis);

	//クォータニオンの比較
	static bool Equals(const Quaternion& q1, const Quaternion& q2);

private:

	// 基本ベクトルを取得
	Dir3DF GetDir(const Dir3DF& dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
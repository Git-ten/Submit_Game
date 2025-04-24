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

	//2D�̃[���x�N�g��
	static constexpr Vector2 VECTOR2D_ZERO = {0, 0};
	static constexpr Vector2F VECTOR2D_ONE = {1.0f, 1.0f};

	static constexpr Vector2F DIR_UP = { 0, -1 };

	// ���W�A��(rad)�E�x(deg)�ϊ��p
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);


	//3D�̃[���x�N�g��
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };


	// ��]��
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };


	// ����
	static constexpr Dir3DF DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr Dir3DF DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr Dir3DF DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr Dir3DF DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr Dir3DF DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr Dir3DF DIR_D = { 0.0f, -1.0f, 0.0f };


	// �l�̌ܓ�
	static int Round(float v);


	// ������̕���
	static std::vector <std::string> Split(std::string& line, char delimiter);


	// ���W�A��(rad)����x(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);


	// �x(deg)���烉�W�A��(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);


	//�Ǝ��ǉ��@�\
	// �x(deg)���烉�W�A��(rad)
	static Rot3DF Deg2RadV(const Rot3DF& deg);
	// ���W�A��(rad)����x(deg)
	static Rot3DF Rad2DegV(const Rot3DF& rad);


	// 0�`360�x�͈̔͂Ɏ��߂�
	static double DegIn360(double deg);


	// 0(0)�`2��(360�x)�͈̔͂Ɏ��߂�
	static double RadIn2PI(double rad);
	static float RadIn2PI(float rad);


	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundRad(float from, float to);
	

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundDeg(float from, float to);
	

	// ���`���
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Position2D Lerp(const Position2D& start, const Position2D& end, float t);
	static Position3DF Lerp(const Position3DF& start, const Position3DF& end, float t);


	// �p�x�̐��`���
	static double LerpDeg(double start, double end, double t);


	// �F�̐��`���
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);


	// �x�W�F�Ȑ�
	static Position2D Bezier(const Position2D& p1, const Position2D& p2, const Position2D& p3, float t);
	static Position3DF Bezier(const Position3DF& p1, const Position3DF& p2, const Position3DF& p3, float t);
	

	// Y����]
	static Position3DF RotXZPos(const Position3DF& centerPos, const Position3DF& radiusPos, float rad);


	// �x�N�g���̒���
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Position2D& v1, const Position2D& v2);
	static double Distance(const Position3DF& v1, const Position3DF& v2);


	// �x�N�g���𔼕��ɂ���
	static Vector2 GetHalfVector2(const Vector2& v);

	// ���̓��m�̏Փ˔���
	static bool IsHitSpheres(
		const Position3DF& pos1, float radius1, const Position3DF& pos2, float radius2);


	// ���̂ƃJ�v�Z���̏Փ˔���
	static bool IsHitSphereCapsule(
		const Position3DF& sphPos, float sphRadius,
		const Position3DF& capPos1, const Position3DF& capPos2, float capRadius);


	// ��r
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);


	// ���K��
	static Dir3DF Normalize(const Vector2& v);
	static Dir3DF VNormalize(const VECTOR& v);


	// 2�̃x�N�g���̊Ԃ̊p�x
	static double AngleDeg(const VECTOR& from, const VECTOR& to);


	//�d�S
	static VECTOR CentVector(const VECTOR& v1, const VECTOR& v2, const VECTOR& v3);
	static VECTOR CentVector(const VECTOR& v1, const VECTOR& v2);

	// �������̎c��Q�[�W�v�Z
	static void GageRateX(Image* image, const float rate, Position2D& rightPos, Size2D& size);

	// �`��n
	static void DrawLineDir(const Position3DF& pos, const Dir3DF& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const Position3DF& pos, const MATRIX& rot, float len = 50.0f);
	// �`��n
	static void DrawLineXYZ(const Position3DF& pos, const Quaternion& rot, float len = 50.0f);
	//�x�N�g���␳
	//�ʏ�A���z�ɋ߂Â��܂ŉ��x���Ăяo��
	//���X�ɗ��z�ɍ��킹��
	static void RelativeQuaternion(double speed, const Quaternion& relativeQua, Quaternion& nowQua);


	//�����n
	static VECTOR RandomVector(const int max);


	//�x�N�g���␳
	//�ʏ�A���z�ɋ߂Â��܂ŉ��x���Ăяo��
	//���X�ɗ��z�ɍ��킹��
	static void RelativeVector(float speed, const VECTOR& relativeVec, VECTOR& nowVec);


	//���Ԍn
	//�b�P�ʂ𕪒P�ʂɂ���
	static int Second2Minute(float second);

};


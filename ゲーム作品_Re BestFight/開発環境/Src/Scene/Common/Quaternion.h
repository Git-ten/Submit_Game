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

	//�R���X�g���N�^
	Quaternion(void);
	Quaternion(const Rot3DF& rad);
	Quaternion(double w, double x, double y, double z);

	//�f�X�g���N�^
	~Quaternion(void);

	// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	static Quaternion Euler(const Rot3DF& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// �N�H�[�^�j�I���̍���
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// �w�莲���w��p���A��]������
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// ���W����]������
	static Position3DF PosAxis(const Quaternion& q, const Position3DF& pos);
	Position3DF PosAxis(const Position3DF& pos) const;

	// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	static Rot3DF ToEuler(const Quaternion& q);
	Rot3DF ToEuler(void) const;

	// �N�H�[�^�j�I������s��֕ϊ�
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix(void) const;

	// �x�N�g������N�H�[�^�j�I���ɕϊ�
	static Quaternion LookRotation(const Dir3DF& dir);
	static Quaternion LookRotation(Dir3DF dir, Dir3DF up);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	static Quaternion GetRotation(MATRIX mat);

	// ��{�x�N�g�����擾
	Dir3DF GetForward(void) const;
	Dir3DF GetBack(void) const;
	Dir3DF GetRight(void) const;
	Dir3DF GetLeft(void) const;
	Dir3DF GetUp(void) const;
	Dir3DF GetDown(void) const;

	// ����
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	// ���K��
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized(void) const;
	void Normalize(void);

	// �t�N�H�[�^�j�I��
	Quaternion Inverse(void) const;

	// ���ʕ��
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	// �Q�̃x�N�g���Ԃ̉�]�ʂ��擾����
	static Quaternion FromToRotation(const Dir3DF& fromDir, const Dir3DF& toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	// �Ώە����̉�]
	void ToAngleAxis(float* angle, VECTOR* axis);

	//�N�H�[�^�j�I���̔�r
	static bool Equals(const Quaternion& q1, const Quaternion& q2);

private:

	// ��{�x�N�g�����擾
	Dir3DF GetDir(const Dir3DF& dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
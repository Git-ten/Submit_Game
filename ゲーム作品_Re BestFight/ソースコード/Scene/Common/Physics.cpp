#include"../../Utility/MyUtility.h"
#include "Physics.h"

//�R���X�g���N�^
Physics::Physics()
{

}

//�f�X�g���N�^
Physics::~Physics()
{

}

void Physics::Gravity(VECTOR& vec, float weight)
{
	//0�҂�����ł͌v�Z�ł��Ȃ��̂ŁA�␳����
	if (MyUtility::EqualsVZero(vec))vec.y = 1.0f;
	VECTOR power = VScale(GRAVITY_DIR, GRAVITY_POWER + weight);
	vec = VAdd(vec, power);

	//����
	if (vec.y < -GRAVITY_MAX)
	{
		vec.y = -GRAVITY_MAX;
	}
}

void Physics::Friction(VECTOR& vec, float weight)
{
	if (MyUtility::EqualsVZero(vec))return;

	//�����x�N�g�����擾
	Dir3DF dir = VNorm(vec);
	//�t����
	dir = VScale(dir, -1.0f);

	VECTOR power = VScale(dir, FRICTION_POWER + weight);

	vec = VAdd(vec, power);

	//���������ɂȂ�����0�ɂ���
	if (VDot(dir, vec) > 0.0f)
	{
		vec = MyUtility::VECTOR_ZERO;
	}
}
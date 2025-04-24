#pragma once
#include<DxLib.h>
#include "../../Utility/Utility.h"
#include"../../Utility/VectorHeader.h"

class Physics
{
public:

	//�d�͂̃f�t�H���g����
	static constexpr Dir3DF GRAVITY_DIR = {0.0f, -1.0f, 0.0f};
	//�d�͂̍ŏ��l
	static constexpr float GRAVITY_MIN = 5.0f;
	//���C�͂̍ŏ��l
	static constexpr float SWAY_MIN = 40.0f;


	//�d�͏���
	static void Gravity(VECTOR& vec, float weight = 0.0f)
	{
		//�d�͍ő�l
		const float GRAVITY_MAX = 180.0f;
		//�d��
		const float GRAVITY_POWER = 2.4f;

		VECTOR power = VScale(GRAVITY_DIR, GRAVITY_POWER + weight);
		vec = VAdd(vec, power);

		//����
		if (vec.y < -GRAVITY_MAX)
		{
			vec.y = -GRAVITY_MAX;
		}
	}

	//���C����
	static void Inertia(VECTOR& vec, float weight = 0.0f)
	{
		//���C��
		const float FRICTION_POWER = 0.8f;

		if (Utility::EqualsVZero(vec))return;

		//�����x�N�g�����擾
		Dir3DF dir = VNorm(vec);
		//�t����
		dir = VScale(dir, -1.0f);

		VECTOR power = VScale(dir, FRICTION_POWER + weight);

		vec = VAdd(vec, power);

		//���������ɂȂ�����0�ɂ���
		if (VDot(dir, vec) > 0.0f)
		{
			vec = Utility::VECTOR_ZERO;
		}
	}

};
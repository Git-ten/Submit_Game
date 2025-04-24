#pragma once
#include<DxLib.h>
#include"../../Utility/VectorHeader.h"

class Physics
{
public:

	//�d�͂̃f�t�H���g����
	static constexpr Dir3DF GRAVITY_DIR = {0.0f, -1.0f, 0.0f};
	//�d��
	static constexpr float GRAVITY_POWER = 2.4f;
	//�d�͍ő�l
	static constexpr float GRAVITY_MAX = 180.0f;

	//���C��
	static constexpr float FRICTION_POWER = 7.6f;

	//�R���X�g���N�^
	Physics(void);

	//�f�X�g���N�^
	~Physics(void);

	//�d�͏���
	static void Gravity(VECTOR& vec, float weight);

	//���C����
	static void Friction(VECTOR& vec, float weight);

};
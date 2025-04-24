#pragma once
#include <memory>
#include "../../UTILITY/VectorHeader.h"

class ActorData
{
public:


	float maxHp_ = 100.0f;							// �ő�̗�
	float hp_ = 100.0f;								// �̗�
	Position3DF* pos_;								// ���W
	std::shared_ptr<Line3DF>collitionGravity_;		// �d�͗p�����蔻��

	std::shared_ptr<SphereF>collisionSphere_;		// �������蔻��
	float collitionSphereSize_ = 0.0f;				// �������蔻��̃T�C�Y
	float collisionSphereOffset_ = 0.0f;			// �������蔻��̒��S�̃I�t�Z�b�g

	std::shared_ptr<SphereF>collisionAttackSphere_;	// �U���p�����蔻��

	int speed_ = 0;									// ���x
	float attackPower_ = 0.0f;						// �U����

};
#pragma once
#include <string>
#include "../../Utility/VectorHeader.h"

class DataCameraControl
{
public:

	float distance;			// �J�����̋���
	float speed;			// �J�����̋쓮���x
	Position3DF targetPos;	// ���_���W
	Rot3DF angleRot;		// �J�����̊p�x
	std::string eventName;	// �s���C�x���g�̖��O
	float eventTime;		// �J�������o�̃^�C��

};
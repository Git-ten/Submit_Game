#pragma once
#include "../../Utility/VectorHeader.h"

class DataImage
{
public:

	int imgDataId;				// �摜��ID
	Plane2D imgPlane;			// �摜�̓����蔻��p�l�p�`
	Position2D posLeftUp;		// ������W
	Position2D posRightDown;	// �E�����W
	Position2D imgCenter;		// ���S���W
	double imgScl;				// �X�P�[��
	double imgAngle;			// �p�x

};
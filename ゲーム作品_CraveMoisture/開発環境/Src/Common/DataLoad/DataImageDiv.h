#pragma once
#include <vector>
#include "../../Utility/VectorHeader.h"

class DataImageDiv
{
public:

	std::vector<int>imgDataId;	// �����摜��ID
	Plane2D imgPlane;			// �摜�̓����蔻��p�l�p�`
	Position2D posLeftUp;		// ������W
	Position2D posRightDown;	// �E�����W
	Position2D imgCenter;		// ���S���W
	double imgScl;				// �X�P�[��
	double imgAngle;			// �p�x

};
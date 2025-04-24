#pragma once
#include "../../Common/Quaternion.h"
#include "../../Utility/VectorHeader.h"

class DataModel
{
public:

	int modelId;				// ���f���̃n���h��ID
	Size3DF scl;				// �傫��
	Rot3DF rot;					// ��]
	Position3DF position;		// �ʒu
	Quaternion quaRot;			// ��]
	Quaternion quaRotLocal;		// ���[�J����]

};


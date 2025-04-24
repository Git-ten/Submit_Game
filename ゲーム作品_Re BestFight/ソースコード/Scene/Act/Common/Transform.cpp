#include "../../../Utility/MyUtility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = MyUtility::VECTOR_ONE;
	rot = MyUtility::VECTOR_ZERO;
	pos = MyUtility::VECTOR_ZERO;
	localPos = MyUtility::VECTOR_ZERO;
	prevPos = pos;//�O�t���[���ł̈ʒu

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();
	//�f�t�H���g���[�J����]
	def_quaRotLocal = quaRotLocal;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = MyUtility::VECTOR_ONE;
	rot = MyUtility::VECTOR_ZERO;
	pos = MyUtility::VECTOR_ZERO;
	localPos = MyUtility::VECTOR_ZERO;
	prevPos = pos;//���W�ω��O�̍��W

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();
	//�f�t�H���g���[�J����]
	def_quaRotLocal = quaRotLocal;
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{
	// �傫��
	matScl = MGetScale(scl);

	// ��]
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// �ʒu
	matPos = MGetTranslate(pos);
	prevPos = pos;//�ω��O�̍��W���X�V

	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// �s������f���ɔ���
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}
}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId = model;
}

//���f��ID���擾
int Transform::GetModel()const
{
	return modelId;
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(MyUtility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(MyUtility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(MyUtility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(MyUtility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(MyUtility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(MyUtility::DIR_D);
}

Dir3DF Transform::GetDir(const Dir3DF& vec) const
{
	return quaRot.PosAxis(vec);
}

VECTOR Transform::GetForwardLocal(void) const
{
	return GetDirLocal(MyUtility::DIR_F);
}

VECTOR Transform::GetRightLocal(void) const
{
	return GetDirLocal(MyUtility::DIR_R);
}

VECTOR Transform::GetLeftLocal(void) const
{
	return GetDirLocal(MyUtility::DIR_L);
}

//���[�J���ȏ����
VECTOR Transform::GetUpLocal(void)const
{
	return GetDirLocal(MyUtility::DIR_U);
}

VECTOR Transform::GetBackLocal(void) const
{
	return GetDirLocal(MyUtility::DIR_B);
}

Dir3DF Transform::GetDirLocal(const Dir3DF& vec) const
{
	Quaternion q = quaRotLocal.Mult(def_quaRotLocal.Inverse());
	return q.PosAxis(vec);
}
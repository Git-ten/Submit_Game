#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "../../../Utility/VectorHeader.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>
class Transform
{

public:

	// ���f���̃n���h��ID
	int modelId;

	// �傫��
	Size3DF scl;
	// ��]
	Rot3DF rot;
	// �ʒu
	Position3DF pos;
	Position3DF localPos;
	Position3DF prevPos;//�O�t���[���ł̈ʒu

	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// ��]
	Quaternion quaRot;

	// ���[�J����]
	Quaternion quaRotLocal;
	//�f�t�H���g���[�J����]
	Quaternion def_quaRotLocal;

	// �R���X�g���N�^
	Transform(void);
	Transform(int model);

	// �f�X�g���N�^
	~Transform(void);

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	//�������J��
	void Release(void);

	//���f���n���h��ID�̃Z�b�g
	void SetModel(int modelHId);

	//���f���n���h��ID���擾
	int GetModel()const;

	// �O���������擾
	Dir3DF GetForward(void) const;

	// ����������擾
	Dir3DF GetBack(void) const;

	// �E�������擾
	Dir3DF GetRight(void) const;

	// ���������擾
	Dir3DF GetLeft(void) const;

	// ��������擾
	Dir3DF GetUp(void) const;

	// ���������擾
	Dir3DF GetDown(void) const;

	//���[�J���ȑO����
	Dir3DF GetForwardLocal(void)const;

	//���[�J���ȉE����
	Dir3DF GetRightLocal(void)const;

	//���[�J���ȍ�����
	Dir3DF GetLeftLocal(void)const;

	//���[�J���ȏ����
	Dir3DF GetUpLocal(void)const;

	//���[�J���Ȍ�����
	Dir3DF GetBackLocal(void)const;

private:

	// �Ώە������擾
	Dir3DF GetDir(const Dir3DF& vec) const;

	//���[�J���ȑΏە������擾
	Dir3DF GetDirLocal(const Dir3DF& vec)const;
};


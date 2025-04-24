#pragma once
#include <string>
#include "DataLoad/DataModel.h"
#include "DataLoad/LoadData.h"
#include "../RENDERER/ModelRenderer/ModelRenderer.h"
#include "../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>
class Transform
{
public:

	// ���f���^�C�v
	enum class TRANSFORM_TYPE
	{
		NONE,
		SKIN_MODEL,
		NORMAL_MODEL,
		NOISE_MODEL,
		METAL_MODEL,
	};


	// ���o�^�C�v
	enum class EVENT_TYPE
	{
		NONE,
		RIMLIGHT,
		RIMLIGHT_SKIN,
	};

	// �R���X�g���N�^
	Transform(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	// �f�X�g���N�^
	~Transform(void);

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	
	// �X�V�O����(���Ȃ��Ă����Ȃ��������AprevPosition������łȂ��Ȃ�)
	void PrevUpdate(void);

	void Update(void);
	void Update(const int modelId, const std::string& frameName);
	void UpdateVertexPS(const int index, const FLOAT4& param);

	void Draw(void);

	// ���f��ID�̎擾
	const int& GetModelId(void)const;

	// ���f��ID�̎Q�Ƃ��擾
	int& GetModelIdReference(void);

	// 1�t���[���O�̍��W���擾
	const Position3DF& GetPrevPos(void);

	// ���W���擾
	const Position3DF& GetPos(void)const;

	// 1�t���[���O�̍��W�̎Q�Ƃ��擾
	Position3DF& GetPrevPosReference(void);

	// ���W���擾(�Q��)
	Position3DF& GetPosReference(void);

	// �X�P�[�����擾
	const VECTOR& GetScl(void)const;

	// �p�x���擾(rad)
	const Rot3DF& GetRot(void)const;
	Rot3DF& GetRotReference(void);

	// ���W���Z�b�g����
	// �]���̂��ƂłȂ��Ǝg�p���Ȃ�
	void SetPos(const Position3DF& pos);

	// ���W��������������
	void MoveModelPos(const VECTOR& moveVec);
	// �p�x���w��̕����Ɉ�������]
	void RotationModelQuaRot(const Dir3DF& dir);
	// �p�x���w��̎��ň�������]
	void RotationModelAngleAxisLerp(const Rot3DF& angleRot);
	void RotationModelAngleAxis(const Rot3DF& angleRot);

	
	// ���_�V�F�[�_�̏�����
	void InitVertexRenderer(const TRANSFORM_TYPE& type);


	// �w�肵���C�x���g�n���_�V�F�[�_�̋N��
	void StartEventVertex(const EVENT_TYPE& type);
	// �C�x���g�n�V�F�[�_�̏I������
	void EndEventVertex(void);

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

	// ���C�e�B���O�̐F�𒲐�
	void SetLightColor(const Color3DF& color);
	// ���C�e�B���O�̋����𒲐�
	void SetLightPower(const float power);

private:

	// ���݂̃C�x���g�^�C�v
	EVENT_TYPE nowType_;

	// ���_�V�F�[�_
	std::unique_ptr<ModelRenderer>vertexRenderer_;

	// �C�x���g�n�V�F�[�_
	std::unique_ptr<ModelRenderer>eventVertexRenderer_;

	// ���f���f�[�^
	DataModel modInfo_;

	//�O�t���[���ł̈ʒu
	Position3DF prevPosition;

	//�f�t�H���g���[�J����]
	Quaternion def_quaRotLocal;

	//�d�͏������s��
	//Dir3DF gravDir_;

	// �v�Z�����Ɏg�p
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// ���݂̃C�x���g�n���_�V�F�[�_�̍X�V
	void EventVertexUpdate(void);
	
	// �Ώە������擾
	Dir3DF GetDir(const Dir3DF& vec) const;
	//���[�J���ȑΏە������擾
	Dir3DF GetDirLocal(const Dir3DF& vec)const;

	// �X�L�����b�V���̃V�F�[�_
	void InitVertexSkin(void);
	// �ʏ탂�f���̃V�F�[�_
	void InitVertexNormal(void);
	// �m�C�Y���f���̃V�F�[�_
	void InitVertexNoize(void);
	// ���^�����f���̃V�F�[�_
	void InitVertexMetal(void);

	// �������C�g�̏�����
	void InitRimLight(const ModelRenderer::SHADER_NAME& name);

	// �������C�g�̍X�V
	void UpdateRimLight(void);

	// ���^���̍X�V
	void UpdateMetal(void);

};
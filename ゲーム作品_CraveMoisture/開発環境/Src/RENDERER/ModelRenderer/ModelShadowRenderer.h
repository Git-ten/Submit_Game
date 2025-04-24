#pragma once
#include "ModelRenderer.h"

class ModelShadowRenderer : public ModelRenderer
{
public:

	//�A�\���̃s�N�Z���V�F�[�_�̃o�b�t�@�T�C�Y
	static constexpr int SHADOW_BUF_SIZE_PS = 5;
	static constexpr int SHADOW_BUF_SIZE_VS = 8;

	// �R���X�g���N�^
	ModelShadowRenderer(int handle);
	// �f�X�g���N�^
	~ModelShadowRenderer(void)override;

	//�[�x�}�b�v�ɕ`�悷�郂�f����ID��ǉ��o�^
	//�@�������p
	void AddDepthModel(int modelId);
	void SetDepthModel(int idx, int modelId);
	//�X�L�����b�V���p
	void AddSkinDepthModel(int modelId);
	void SetSkinDepthModel(int idx, int modelId);

private:

	//�[�x�}�b�v�ɕ`�悷�郂�f��(�@������)
	std::vector<int>depthModels_;
	//�[�x�}�b�v�ɕ`�悷�郂�f��(�X�L�����b�V��)
	std::vector<int>skinDepthModels_;

	//�J�����̃r���[�s��
	MATRIX cameraView_;
	//�J�����̎ˉe�s��
	MATRIX cameraProj_;

	//�[�x�}�b�v
	int depthHandle_;
	//�[�x�}�b�v�`��p�̃V�F�[�_�n���h��
	int shaderDepthPS_;
	int shaderDepthVS_;
	int shaderSkinDepthVS_;//�X�L�����b�V��

	// �p����Ŏg�p����ǉ��V�F�[�_���̏�����
	void InitNamesSpecial(void);

	// �p����Ŏg�p����V�F�[�_���Z�b�g����ۂɍs������
	void SetShaderSpecial(void)override;

	// �p����ōs���`��
	void DrawSpecial(void)override;

	//�[�x�}�b�v��`��
	void DrawDepth(void);

};
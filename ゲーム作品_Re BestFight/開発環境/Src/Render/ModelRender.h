#pragma once
#include"ModelMaterial.h"
class ModelRender
{
public:

	//���_�V�F�[�_�o�b�t�@�Ɏg�p����X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	//�s�N�Z���V�F�[�_�o�b�t�@�Ɏg�p����X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	//�R���X�g���N�^
	ModelRender(int modelId, ModelMaterial& modelMaterial);
	
	//�f�X�g���N�^
	~ModelRender(void);

	//�`��
	void Draw(void);

private:

	//���f��ID
	int modelId_;

	//�}�e���A���f�[�^
	ModelMaterial& modelMaterial_;

	//�V�F�[�_�̏���
	void SetReserveVS(void);
	void SetReservePS(void);

};


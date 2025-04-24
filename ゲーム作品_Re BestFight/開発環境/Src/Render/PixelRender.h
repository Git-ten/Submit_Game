#pragma once
#include"../Utility/VectorHeader.h"
#include"PixelMaterial.h"

class PixelRender
{
public:

	//�s�N�Z���V�F�[�_�o�b�t�@�Ɏg�p����X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	//�|���S���̒��_��
	static constexpr int NUM_VERTEX = 4;
	//�|���S���C���f�b�N�X�̐�
	static constexpr int NUM_VERTEX_IDX = 6;
	//�|���S����
	static constexpr int NUM_POLYGON = 2;

	//�R���X�g���N�^
	PixelRender(const PixelMaterial& pixelMaterial);

	//�f�X�g���N�^
	~PixelRender(void);

	//�|���S���쐬
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	void MakeSquereVertex(void);

	//���W�̃Z�b�g
	void SetPos(Vector2 pos);
	//�傫���̃Z�b�g
	void SetSize(Vector2 size);

	//�`��
	void Draw(void);

private:

	//���W
	Vector2 pos_;
	//�傫��
	Vector2 size_;

	//�|���S���f�[�^
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	//�|���S���C���f�b�N�X�f�[�^
	WORD indexes_[NUM_VERTEX_IDX];

	//�}�e���A���f�[�^
	const PixelMaterial& pixelMaterial_;

};
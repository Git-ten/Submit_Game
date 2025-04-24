#pragma once
#include <DxLib.h>
#include <vector>

class PoligonRenderer
{
public:

	// �R���X�g���N�^
	PoligonRenderer(void);
	// �f�X�g���N�^
	~PoligonRenderer(void);

	void Draw(std::vector<VERTEX3DSHADER>&vertex, std::vector<WORD>& indexs);

	// �e�N�X�`���̃Z�b�g
	void SetTexture(int texture);

private:

	// 3D�|���S���p�̃s�N�Z���V�F�[�_
	int shaderPoligonPS_;
	// 3D�|���S���p�̒��_�V�F�[�_
	int shaderPoligonVS_;

	// �\��t����e�N�X�`��
	int texture_;

};
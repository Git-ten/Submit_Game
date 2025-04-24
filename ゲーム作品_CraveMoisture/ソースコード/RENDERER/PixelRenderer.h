#pragma once
#include"../Utility/VectorHeader.h"
#include<vector>
#include<map>
#include<string>
#include<DxLib.h>

class PixelRenderer
{
public:

	//�����_�[�̃e�N�X�`���[�A�h���X
	enum class TEXADDRESS
	{
		NONE = 0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};


	//�V�F�[�_�[�̏��
	struct SHADER_INFO
	{
		int shader;						//�V�F�[�_�[�̃n���h��
		int constBufFloat4Size;			//�萔�o�b�t�@�̃f�[�^�T�C�Y(FLOAT4�̔{��)
		int constBuf;					//�萔�o�b�t�@�̃n���h��
		TEXADDRESS texAddress;			//�e�N�X�`���[�A�h���X
		std::vector<FLOAT4>constBufs;	//Pixel�萔�o�b�t�@
		std::vector<int>textures;		//�g�p����e�N�X�`���[
	};


	//�V�F�[�_�[��
	enum class SHADER_NAME
	{
		MONOTONE,		// ���m�g�[��
		SCANLINE,		// ������
		MIRAGE,			// 凋C�O
		CORONA,			// ���z�R���i
		LIGHT_CIRCLE,	// ����
		BRACK_OUT,		// �Ó]
		BLUR,			// �u���[
	};


	//�V�F�[�_�̃o�b�t�@���ňӖ��������Ȃ��Ƃ����Ӗ�
	static constexpr float SHADER_NONE_VALUE = 1.0f;


	//�o�b�t�@�T�C�Y��0
	static constexpr int BUF_SIZE_ZERO = 0;


	//凋C�O�̃o�b�t�@�T�C�Y
	static constexpr int BUF_SIZE_MIRAGE = 1;


	//���z�R���i�̃o�b�t�@�T�C�Y
	static constexpr int BUF_SIZE_CORONA = 1;
	

	//�R���X�g���N�^
	PixelRenderer(int num, Size2D size);
	//�f�X�g���N�^
	~PixelRenderer(void);


	// �C���f�b�N�X�|���S�����Z�b�g
	// ��ʑS�̂ɒ���t����悤�Ƀ|���S���𐶐�����
	void MakeSquereVertex(Position2D pos, Size2D size);


	//�����_�����O�������ʂ�`��
	void Draw(void);


	//�V�F�[�_����ǉ�
	void AddShaderInfo(SHADER_NAME name, int constBufFloat4Size);


	//�o�b�t�@��ǉ��o�^
	void AddConstBuf(SHADER_NAME name, FLOAT4 contBuf);
	void SetConstBuf(SHADER_NAME name, int idx, FLOAT4 contBuf);


	//�e�N�X�`���[���Z�b�g
	void SetTextureBuf(SHADER_NAME name, int handle);


	//�e�N�X�`���[�A�h���X���Z�b�g
	void SetTextureAddress(SHADER_NAME name, TEXADDRESS texA);

private:

	//�|���S���̒��_��
	static constexpr int NUM_VERTEX = 4;
	//�|���S���̐�
	static constexpr int NUM_POLYGON = 2;
	//�O�p�|���S���̒��_��
	static constexpr int NUM_VERTEX_IDX = NUM_POLYGON * 3;


	//�����_�����O���ʂ̕\�����W
	Position2D pos_;
	//�����_�����O���ʂ̉�ʂ̑傫��
	Size2D size_;


	//�|���S��
	VERTEX2DSHADER vertexs_[NUM_VERTEX];


	//�|���S���C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];


	//�X�N���[���̌�
	int screenNum_;
	//�X�N���[��
	int* screens_;


	//�V�F�[�_���
	std::map<SHADER_NAME, SHADER_INFO>shaderInfo_;
	std::map<SHADER_NAME, std::string>shaderNames_;



	//�����ł̂ݎg�p����֐�
	//�V�F�[�_����������
	void InitNames(void);

};
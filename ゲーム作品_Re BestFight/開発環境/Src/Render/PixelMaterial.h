#pragma once
#include<string>
#include<vector>
#include<DxLib.h>
class PixelMaterial
{
public:

	static constexpr int MAX_TEXTURES = 4;

	enum class TEXADDRESS
	{
		NONE=0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//�R���X�g���N�^
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);

	//�f�X�g���N�^
	~PixelMaterial(void);

	//�o�b�t�@
	void AddConstBuf(const FLOAT4& contBuf);
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	//���[�h�����V�F�[�_���擾
	int GetShader(void)const;
	//�o�^�����o�b�t�@���擾
	int GetConstBuf(void)const;

	//�o�b�t�@�̒��g���擾
	const std::vector<FLOAT4>& GetConstBufs(void)const;
	const std::vector<int>& GetTextures(void)const;

	//�e�L�X�g�o�b�t�@
	void SetTextureBuf(int handle);

	//�e�L�X�g�o�b�t�@�̎擾
	TEXADDRESS GetTextureAddress(void)const;

	//�e�L�X�g�A�h���X�̃Z�b�g
	void SetTextureAddress(TEXADDRESS texA);

private:

	//�V�F�[�_
	int shader_;

	//�o�b�t�@�̃T�C�Y
	int constBufFloat4Size_;

	//�쐬�����o�b�t�@
	int constBuf_;

	//�e�L�X�g�A�h���X
	TEXADDRESS texAddress_;

	//�o�^�����o�b�t�@
	std::vector<FLOAT4>constBufs_;

	//�o�^�����e�N�X�`��
	std::vector<int>textures_;

};


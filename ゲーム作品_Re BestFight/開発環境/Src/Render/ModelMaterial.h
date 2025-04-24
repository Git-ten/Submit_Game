#pragma once
#include<string>
#include<vector>
#include<map>
#include<DxLib.h>
class ModelMaterial
{
public:
	static constexpr int MAX_TEXTURES = 4;

	enum class TEXADDRESS
	{
		NONE = 0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//�R���X�g���N�^
	ModelMaterial(
		std::string shaderFileNameVS,
		int constBufFloat4SizeVS,
		std::string shaderFileNamePS,
		int constBufFloat4SizePS);

	//�f�X�g���N�^
	~ModelMaterial(void);

	//�o�b�t�@
	void AddConstBufVS(const FLOAT4& contBuf);
	void AddConstBufPS(const FLOAT4& contBuf);
	void SetConstBufVS(int idx, const FLOAT4& contBuf);
	void SetConstBufPS(int idx, const FLOAT4& contBuf);

	//�e�L�X�g�o�b�t�@
	void AddTextureBuf(int slot, int handle);
	void SetTextureBuf(int slot, int handle);

	//���[�h�����V�F�[�_���擾
	int GetShaderVS(void)const;
	int GetShaderPS(void)const;
	//�o�^�����o�b�t�@���擾
	int GetConstBufVS(void)const;
	int GetConstBufPS(void)const;

	//�o�b�t�@�̒��g���擾
	const std::vector<FLOAT4>& GetConstBufsVS(void)const;
	const std::vector<FLOAT4>& GetConstBufsPS(void)const;
	const std::map<int, int>& GetTextures(void)const;

	//�e�L�X�g�o�b�t�@�̎擾
	TEXADDRESS GetTextureAddress(void)const;

	//�e�L�X�g�A�h���X�̃Z�b�g
	void SetTextureAddress(TEXADDRESS texA);

private:

	//�V�F�[�_
	int shaderPS_;
	int shaderVS_;

	//�o�b�t�@�̃T�C�Y
	int constBufFloat4SizePS_;
	int constBufFloat4SizeVS_;

	//�쐬�����o�b�t�@
	int constBufPS_;
	int constBufVS_;

	//�e�L�X�g�A�h���X
	TEXADDRESS texAddress_;

	//�o�^�����o�b�t�@
	std::vector<FLOAT4>constBufsPS_;
	std::vector<FLOAT4>constBufsVS_;

	//�o�^�����e�N�X�`��
	//first:slot, second:�n���h��ID
	std::map<int, int>textures_;

};
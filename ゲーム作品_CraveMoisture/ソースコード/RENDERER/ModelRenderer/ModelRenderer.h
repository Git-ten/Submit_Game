#pragma once
#include"../../Utility/VectorHeader.h"
#include<vector>
#include<map>
#include<string>
#include<DxLib.h>

class ModelRenderer
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
		//���_
		int shaderPS;						// Pixel�V�F�[�_�[�̃n���h��
		int shaderVS;						// ���_�V�F�[�_�[�̃n���h��
		int constBufFloat4SizePS;			// Pixel�萔�o�b�t�@�̃f�[�^�T�C�Y(FLOAT4�̔{��)
		int constBufFloat4SizeVS;			// ���_�萔�o�b�t�@�̃f�[�^�T�C�Y(FLOAT4�̔{��)
		int constBufPS;						// Pixel�萔�o�b�t�@�̃n���h��
		int constBufVS;						// ���_�萔�o�b�t�@�̃n���h��
		TEXADDRESS texAddress;				// �e�N�X�`���[�A�h���X
		std::vector<FLOAT4>constBufsPS;		// Pixel�萔�o�b�t�@
		std::vector<FLOAT4>constBufsVS;		// ���_�萔�o�b�t�@
		std::map<int, int>textures;			// �g�p����e�N�X�`���[
	};


	//�V�F�[�_�[��
	enum class SHADER_NAME
	{
		NONE,
		NORMAL,						// ���ʂ̒��_�V�F�[�_
		NORMAL_POLYGON,				// �|���S����ʏ풸�_�V�F�[�_�ŕ`��
		NO_NORM_SHADOW,				// �e�\��(�@������)
		NORM_SHADOW,				// �A�\��
		SKIN_MESH_NORM_4,			// �ʏ�̃X�L�����b�V��
		SKIN_MESH_NORM_8,			// �ʏ��8�X�L�����b�V��
		METAL,						// ������
		SKIN_METAL,					// �X�L�����b�V���̋�����
		SKIN_METAL_NORM_8,			// �@������8�X�L�����b�V���̋�����
		RIM_LIGHT,					// �������C�g
		RIM_LIGHT_SKIN,				// �X�L�����b�V���̃������C�g
		DISSOLVE,					// �f�B�]���u
	};


	//�V�F�[�_�̃o�b�t�@���ňӖ��������Ȃ��Ƃ����Ӗ�
	static constexpr float SHADER_NONE_VALUE = 1.0f;


	//�o�b�t�@�T�C�Y��0
	static constexpr int BUF_SIZE_ZERO = 0;


	//�ʏ�̃X�L�����b�V���̂��ꂼ��̃V�F�[�_�[�̃o�b�t�@�T�C�Y
	static constexpr int SKIN_MASH_BUF_SIZE_PS = 3;


	//���^���b�N�̃s�N�Z���V�F�[�_�̃o�b�t�@�T�C�Y
	static constexpr int METAL_BUF_SIZE_PS = 2;
	//�X�L�����b�V�����^���b�N�̃s�N�Z���V�F�[�_�̃o�b�t�@�T�C�Y
	static constexpr int SKIN_METAL_BUF_SIZE_PS = 2;
	// �������C�g�̒��_�V�F�[�_�̃o�b�t�@�T�C�Y
	static constexpr int RIM_LIGHT_BUF_SIZE_VS = 1;
	// �f�B�]���u�̒��_�V�F�[�_�̃o�b�t�@�T�C�Y
	static constexpr int DISSOLVE_BUF_SIZE_PS = 1;

	// ���t�l�X�e�N�X�`���̃��W�X�^�ԍ�
	static constexpr int ROUGHNESS_INDEX = 11;
	// ���^���b�N�e�N�X�`���̃��W�X�^�ԍ�
	static constexpr int METALIC_INDEX = 12;
	// �X�t�B�A�}�b�v�e�N�X�`���̃��W�X�^�ԍ�
	static constexpr int SPHERE_INDEX = 13;

	//�F�R�[�h
	//�D�F
	static constexpr Color3DF GREY_COLOR = { 0.66f, 0.66f, 0.66f };
	// ����̓��̖��邳
	static constexpr Color3DF CLEAR_SKY = { 0.3f, 0.3f, 0.3f};
	//������Ɣ�����
	static constexpr Color3DF FEW_BLACK_COLOR = { 0.1f, 0.1f, 0.1f };
	//���z���̐F
	static constexpr Color3DF SUN_COLOR = { 0.99f, 0.72f, 0.42f };


	//�g�U��(�f�t�H���g)
	static constexpr Color3DF DEFAULT_DIFFUSE_COLOR = { 0.8f, 0.8f, 0.8f };
	//����(�f�t�H���g)
	static constexpr Color3DF DEFAULT_SPECULAR_COLOR = { 0.1f, 0.1f, 0.1f };
	//���ˌ�(�f�t�H���g)
	static constexpr Color3DF DEFAULT_REFLECTION_COLOR = { 1.0f, 1.0f, 1.0f };


	//���̋���(��)
	static constexpr float DIRECTION_LIGHT_POWER_WEAK = 0.1f;
	//���̋���(��)
	static constexpr float DIRECTION_LIGHT_POWER_MEDIUM = 0.45f;
	//���̋���(��)
	static constexpr float DIRECTION_LIGHT_POWER_STRONG = 0.7f;


	//�R���X�g���N�^
	ModelRenderer(int handle);
	//�f�X�g���N�^
	virtual ~ModelRenderer(void);


	// �`��
	void Draw(void);


	// �g�p����V�F�[�_���Z�b�g
	void SetShaderInfo(SHADER_NAME name,
		int constBufFloat4SizePS, int constBufFloat4SizeVS, int constBufFloat4SizeGE);


	// �o�b�t�@��ǉ��o�^
	void AddConstBufPS(FLOAT4 contBufPS);
	void AddConstBufVS(FLOAT4 contBufVS);
	void SetConstBufPS(int idx, FLOAT4 contBufPS);
	void SetConstBufVS(int idx, FLOAT4 contBufVS);


	//�e�N�X�`���[���Z�b�g
	void SetTextureBuf(int idx, int handle);


	//�e�N�X�`���[�A�h���X���Z�b�g
	void SetTextureAddress(TEXADDRESS texA);

protected:

	//���ݎg�p���̃V�F�[�_�[
	SHADER_NAME name_;


	//�Ώۃ��f��
	int modelId_;


	//�V�F�[�_���
	SHADER_INFO shaderInfo_;
	//�V�F�[�_�[�̎�ނ��Ƃ̃p�X���i�[
	std::map<SHADER_NAME, std::string>shaderNamesPS_;//�s�N�Z���V�F�[�_�[�p�X
	std::map<SHADER_NAME, std::string>shaderNamesVS_;//���_�V�F�[�_�[�p�X



	//�����ł̂ݎg�p����֐�
	//�V�F�[�_����������
	void InitNames(void);

	//�V�F�[�_�̃Z�b�g
	void SetReserveVS(void);
	void SetReservePS(void);

	// �p����Ŏg�p����V�F�[�_���Z�b�g����ۂɍs������
	virtual void SetShaderSpecial(void);

	// �p����ōs���`��
	virtual void DrawSpecial(void);

};
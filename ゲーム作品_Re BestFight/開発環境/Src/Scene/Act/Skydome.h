#pragma once
#include<memory>
#include"../../Utility/VectorHeader.h"

class Transform;
class ActorBase;
class ModelMaterial;
class ModelRender;

class SkyDome
{
public:

	//�R���X�g���N�^
	SkyDome(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy);

	//�f�X�g���N�^
	~SkyDome(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

private:

	//�傫��
	static constexpr Size3DF SKYDOME_SCALE = { 200.0f, 200.0f, 200.0f };
	//�f�B�t���[�Y�J���[
	static constexpr Color3D DIFFUSE_COLOR = { 0.8f, 0.8f, 0.8f };

	std::unique_ptr<Transform>transform_;

	//���_�}�e���A��
	std::unique_ptr<ModelMaterial>material_;
	//���_�����_�[
	std::unique_ptr<ModelRender>render_;

	//�v���C���[�̃f�[�^�����L
	std::weak_ptr<ActorBase>player_;
	//CPU�̃f�[�^�����L
	std::weak_ptr<ActorBase>enemy_;



	//�����ł̂ݎg�p����֐�
	void InitTransform(void);
	void InitShader(void);
	void DrawSkydome(void);

	//�f�B�t���[�Y�J���[
	void SetDiffuseColor(const Color3D& color);
	void SetDefDiffuseColor(void);

};
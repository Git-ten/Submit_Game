#pragma once
#include<memory>
#include"../../Utility/VectorHeader.h"

class Transform;
class ActorBase;
class Collider;
class ModelMaterial;
class ModelRender;

class Stage
{
public:

	static constexpr Size3DF STAGE_SCALE = { 2.0f, 2.0f, 2.0f };//�X�e�[�W�̑傫��
	static constexpr float COLL_RADIUS_CIRCLE = 2760.0f * 2.0f;//�X�e�[�W�̔��a

	//�R���X�g���N�^
	Stage(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy, std::weak_ptr<Camera>camera);

	//�f�X�g���N�^
	~Stage(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	//�J�����̓����蔻��
	void CollisionCamera(void);

private:

	//�|�C���g���C�g�̏������W
	static constexpr Position3DF POINT_LIGHT_POS = { 0.0f, 6000.0f, 3000.0f };
	//�|�C���g���C�g�̐F
	static constexpr Color3D POINT_LIGHT_COLOR = { 0.8f, 0.8f, 0.6f};
	//�|�C���g���C�g�̋���
	static constexpr float POINT_LIGHT_POWER = 6000.0f;
	//�f�B�t���[�Y�J���[
	static constexpr Color3D DIFFUSE_COLOR = {0.8f, 0.8f, 0.8f};

	//���_�}�e���A��
	std::unique_ptr<ModelMaterial>material_;
	//���_�����_�[
	std::unique_ptr<ModelRender>render_;
	//�f�o�b�O�p
	std::unique_ptr<Transform>transform_;
	//�v���C���[�̃f�[�^�����L
	std::weak_ptr<ActorBase>player_;
	//CPU�̃f�[�^�����L
	std::weak_ptr<ActorBase>enemy_;

	//�����蔻��̃C���X�^���X
	std::unique_ptr<Collider>collider_;

	//���L����J����
	std::weak_ptr<Camera>camera_;



	//�����ł̂ݎg�p����֐�
	void InitTransform(void);
	void InitShader(void);

	//�����蔻��
	void Collision(void);
	//�A�N�^�[�̓����蔻��
	void CollisionActor(void);
	//�X�e�[�W�̕`��
	void DrawStage(void);
	//�\���|�C���g���C�g�V�F�[�_�萔���Z�b�g����
	void SetSparePointLightShader(float power, const Position3DF& pos, const Color3D& color);
	//�f�B�t���[�Y�J���[
	void SetDiffuseColor(const Color3D& color);
	void SetDefDiffuseColor(void);

};


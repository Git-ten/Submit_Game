#pragma once
#include<memory>
#include<DxLib.h>

class ActorBase;

class Collider
{
public:

	//�f�t�H���g�̐��������蔻�苗��
	static constexpr float DEFAULT_DIS = 50.0f;

	//�R���X�g���N�^
	Collider(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy);
	Collider(std::weak_ptr<ActorBase>player);

	//�f�X�g���N�^
	~Collider(void);

	//�����Ɛ����̓����蔻��
	bool CollisionLine2Line(void);
	//�O�p�`�ƃJ�v�Z���̓����蔻��
	bool CollisionTriangle2Model(VERTEX3D* ver);
	//���ƃJ�v�Z���̓����蔻��
	bool CollisionSphere2Cap(const SphereF& sph);
	//�t�B�[���h�̓����蔻��(�~�`)
	void CollisionCircle2Vector(std::weak_ptr<ActorBase>actor, const float& radius);
	void CollisionCircle2Vector(std::weak_ptr<Camera>camera, const float& radius);

private:

	//�Ăяo�����̃A�N�^�[
	std::weak_ptr<ActorBase>player_;

	//�Ăяo�����̑���A�N�^�[
	std::weak_ptr<ActorBase>enemy_;

};
#include"../Act/ActorBase.h"
#include"../Act/Common/Transform.h"
#include"../Camera.h"
#include "Collider.h"
#include<DxLib.h>
#include<cmath>

//�R���X�g���N�^
Collider::Collider(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy)
	:player_(player),enemy_(enemy)
{

}

Collider::Collider(std::weak_ptr<ActorBase> player):player_(player)
{

}

//�f�X�g���N�^
Collider::~Collider()
{

}

bool Collider::CollisionLine2Line()
{
	float distance = Segment_Segment_MinLength(player_.lock()->transform_->prevPos, player_.lock()->transform_->pos,
											enemy_.lock()->transform_->prevPos, enemy_.lock()->transform_->pos);

	//������50,0f���Z���ꍇ
	if (distance < DEFAULT_DIS)
	{
		return true;
	}

	return false;
}

bool Collider::CollisionTriangle2Model(VERTEX3D* ver)
{
	//�|���S��1��(ver[0 �` 2])�������肵�Ȃ�
	CapsuleF cap = enemy_.lock()->Model2Capsule();

	bool hit = HitCheck_Capsule_Triangle(cap.top, cap.down, static_cast<float>(cap.r), ver[0].pos, ver[1].pos, ver[2].pos);

	return hit;
}

bool Collider::CollisionSphere2Cap(const SphereF& sph)
{
	//�|���S��1��(ver[0 �` 2])�������肵�Ȃ�
	CapsuleF cap = enemy_.lock()->Model2Capsule();

	bool hit = HitCheck_Sphere_Capsule(sph.pos, static_cast<float>(sph.r), cap.top, cap.down, static_cast<float>(cap.r));

	return hit;
}

void Collider::CollisionCircle2Vector(std::weak_ptr<ActorBase>actor, const float& radius)
{
	//���S�_����v���C���[�܂ł̋���
	float dis = VSize(actor.lock()->transform_->pos);
	Dir3DF dir = VNorm(actor.lock()->transform_->pos);

	Position3DF vec = MyUtility::VECTOR_ZERO;
	vec = VScale(dir, radius);

	if (actor.lock()->GetState() == ActorBase::STATE::MOVE)
	{
		dis = VSize(VAdd(actor.lock()->transform_->pos, VScale(dir, 100.0f)));
		vec = VAdd(vec, VScale(VScale(dir, -1.0f), 100.0f));
	}
	//�~�`�̓����蔻��
	if (radius < std::fabsf(dis))
	{
		actor.lock()->transform_->pos = vec;
	}
}

void Collider::CollisionCircle2Vector(std::weak_ptr<Camera>camera, const float& radius)
{
	//���S�_����v���C���[�܂ł̋���
	float dis = VSize(camera.lock()->GetPos());
	Dir3DF dir = VNorm(camera.lock()->GetPos());

	Position3DF vec = MyUtility::VECTOR_ZERO;
	vec = VScale(dir, radius);

	//�~�`�̓����蔻��
	if (radius < std::fabsf(dis))
	{
		camera.lock()->SetPos(vec);
		camera.lock()->CompCameraInfoUpdate();
	}
}
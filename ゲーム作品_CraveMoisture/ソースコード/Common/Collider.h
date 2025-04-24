#pragma once
#include<memory>
#include<DxLib.h>
#include"../../Utility/VectorHeader.h"

class Collider
{
public:

	//���f���S�̂��R���W�������ɂ���
	//���f���̃t���[���C���f�b�N�X�Ƃ��Ďg�p
	static constexpr int ALL_MODEL_INFO_FRAME_IDX = -1;

	//�R���X�g���N�^
	Collider(void);
	//�f�X�g���N�^
	~Collider(void);


	//2D�p
	//��`�Ɠ_�̓����蔻��
	static bool CollisionRectToMouse(const Plane2D& plane, const Position2D& target);
	// ��`�Ƌ�`�̓����蔻��
	static bool CollisionRectToRect(const Plane2D& plane1, const Plane2D& plane2);


	//3D
	//�����Ɛ����̓����蔻��
	static bool CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2);
	static bool CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2, float dis);
	//�O�p�`�ƃJ�v�Z���̓����蔻��
	static bool CollisionTriangle2Model(const VERTEX3D* ver, const CapsuleF& cap);
	//���ƃJ�v�Z���̓����蔻��
	static bool CollisionSphere2Cap(const SphereF& sph, const CapsuleF& cap);
	//���Ɛ����̓����蔻��
	static bool CollisionSphere2Line(const Line3DF& line, const SphereF& sph);
	//���Ƌ��̓����蔻��
	static bool CollisionSphere2Sphere(const SphereF& sph_1, const SphereF& sph_2);
	//�J�v�Z���ƃ��f���̓����蔻��
	static bool CollisionCap2Model(int id, int flameIdx, const CapsuleF& cap);
	//���ƃ��f���̓����蔻��
	static bool CollisionSphere2Model(int id, int flameIdx, const SphereF& sph);
	static MV1_COLL_RESULT_POLY_DIM CollisionSphere2ModelInfo(int id, int flameIdx, const SphereF& sph);
	//���ƃ��f���̓����蔻��
	static bool CollisionLine2Model(int id, int flameIdx, const Line3DF& line);
	static MV1_COLL_RESULT_POLY CollisionLine2ModelInfo(int id, int flameIdx, const Line3DF& line);
	//���ƎO�p�`�̓����蔻��
	static bool CollisionLine2Triangle(const Line3DF& line, const Triangle3DF& tri);

};
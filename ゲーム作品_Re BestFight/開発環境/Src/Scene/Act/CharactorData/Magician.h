#pragma once
#include "CharactorBase.h"

class Magician : public CharactorBase
{
public:

	Magician(std::weak_ptr<Camera> camera);
	~Magician()override;

	//������
	void Init(std::weak_ptr<ActorBase>enemy)override;
	void InitTransform(void)override;


	//�g�p���Ă��郂�f���̃��\�[�XID���擾
	SceneManager::CHARA_ID GetModelResource(void)override;
	//�U���͈�
	float GetAttackArea(void)const override;
	//����
	float GetAvoidPower(void)const override;
	//�W�����v��
	float GetJumpPower(void)const override;
	//�h���
	float GetDefencePower(void)const override;
	//�����̐F
	Color3D GetMyColor(void)const override;
	//���x
	float GetSpeed()const override;
	float GetAttackHappen(ActorBase::STATE type)const override;

	//�K�E�Z�̔�������
	bool IsUnleashSkill(void)const override;
	//�K�E�Z���̃J��������
	void SkillCamera(void)override;
	//��U���t�B�j�b�V���U���̃J��������
	void WeekFinishCamera(void)override;
	//���U���t�B�j�b�V���U���̃J��������
	void StrongFinishCamera(void)override;

private:

	static constexpr Size3DF PLAYER_SCALE = { 1.2f, 1.2f, 1.2f };
	static constexpr float JUMP_POWER = 40.0f;//�f�o�b�O�p50.0f
	static constexpr Rot3DF DEFAULT_ANGLE = { 0.0, 180.0, 0.0 };
	static constexpr float HP_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float STAMINA_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float DEFENCE_MAX = 3.0f;//�f�o�b�O�p
	static constexpr float SKILL_DEFAULT_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float SKILL_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float AVOID_POWER = 30.0f;//�f�o�b�O�p
	static constexpr float ATTACK_POWER = 1.2f;
	static constexpr float FORWARD_ATTACK_AREA = 900.0f;//�f�o�b�O�p
	static constexpr float SPEED = 45.0f;//���x

	static constexpr Position3DF FINISH_SKILL_CAMERA_POS = {200.0f, 50.0f, -700.0f };
	static constexpr Position3DF START_WEEK_CAMERA_POS = { 0.0f, 400.0f, -250.0f };
	static constexpr Position3DF FINISH_WEEK_CAMERA_POS = { 0.0f, 200.0f, -850.0f };
	static constexpr Position3DF FINISH_STRONG_CAMERA_POS = { 0.0f, 200.0f, -450.0f };
	static constexpr Color3D MY_COLOR = { 0.0f, 1.0f, 0.0f };



	//�����ł̂ݎg�p����֐�
	//���z������
	void InitAnim(void)override;
	void InitEffect(void)override;
	void InitEffectState(void)override;
	void InitSound(void)override;

};


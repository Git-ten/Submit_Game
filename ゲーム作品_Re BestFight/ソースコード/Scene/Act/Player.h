#pragma once
#include"ActorBase.h"

class Player : public ActorBase
{
public:

	//�R���X�g���N�^
	Player(std::weak_ptr<Camera> camera, bool flag);

	//�f�X�g���N�^
	~Player(void)override;

	//������
	void Init(std::weak_ptr<ActorBase>enemy, SceneManager::CHARA_ID id)override;
	void InitMark(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;

	SceneManager::CHARA_ID GetModelResource(void)override;

	float GetAttackArea(void)const override;
	float GetAvoidPower(void)const override;
	float GetJumpPower(void)const override;
	float GetStateDefenceMax(void)const override;
	float GetAttackPower(void)const override;
	Color3D GetMyColor(void)const override;
	float GetAttackHappen(void)const override;

	//�v���C���[ID�̎擾
	int GetPlayerId(void)const override;

	//���x���Q�b�g
	float GetSpeed(void)const override;

	//�v���C���[�̑O���̑��΍��W
	//�{���ŋ����ω�
	Position3DF GetPlayerForwardPos(const float& scl)const override;

	//�v���C���[�̉�]��K�؂ɕ␳����
	//�܂�A180�xY����]����
	Rot3DF GetCollectionPlayerRot(void)const override;

	//�K�E�Z�̔�������
	bool IsUnleashSkill(void)const override;

	//�K�E�Z���̃J��������
	void SkillCamera(void)override;
	//��U���t�B�j�b�V���U���̃J��������
	void WeekFinishCamera(void)override;
	//���U���t�B�j�b�V���U���̃J��������
	void StrongFinishCamera(void)override;

	//�N�����Ă����������[�v�G�t�F�N�g�̒�~
	void StopAllEffect(void)override;

private:

	//�萔
	static constexpr Size3DF PLAYER_SCALE = { 1.0f, 1.0f, 1.0f };
	static constexpr float JUMP_POWER = 40.0f;//�f�o�b�O�p50.0f
	static constexpr float HP_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float STAMINA_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float DEFENCE_MAX = 3.0f;//�f�o�b�O�p
	static constexpr float SKILL_DEFAULT_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float SKILL_MAX = 100.0f;//�f�o�b�O�p
	static constexpr float AVOID_POWER = 70.0f;//�f�o�b�O�p
	static constexpr float FORWARD_ATTACK_AREA = 300.0f;//�f�o�b�O�p

	static constexpr Position3DF START_SKILL_CAMERA_POS = {-100.0f, 100.0f, -450.0f };
	static constexpr Position3DF END_SKILL_CAMERA_POS = {-200.0f, 50.0f, -550.0f };
	static constexpr Position3DF FINISH_SKILL_CAMERA_POS = {-100.0f, 50.0f, -50.0f };
	static constexpr Position3DF FINISH_WEEK_CAMERA_POS = {0.0f, 200.0f, -450.0f };
	static constexpr Position3DF FINISH_STRONG_CAMERA_POS = {0.0f, 200.0f, -450.0f };
	static constexpr Color3D MY_COLOR = {1.0f, 0.0f, 0.0f};

	//�|�C���g���C�g�̏������W
	static constexpr Position3DF POINT_LIGHT_POS = { 0.0f, 6000.0f, 8000.0f };
	//�|�C���g���C�g�̐F
	static constexpr Color3D POINT_LIGHT_COLOR = { 0.0f, 6000.0f, 3000.0f };;
	//�|�C���g���C�g�̋���
	static constexpr float POINT_LIGHT_POWER = 6000.0f;

	//�����̕����\������
	static constexpr float STR_MAX_TIME = 3.0f;
	//�����̓_��
	static constexpr int LV_COUNT_MAX = 50;

	//���x���A�b�v�G�t�F�N�g
	std::weak_ptr<Effect>carsolAttackEff_;
	std::weak_ptr<Effect>carsolDefenceEff_;
	std::weak_ptr<Effect>carsolAvoidEff_;
	
	//�U���̓A�b�v�������\������
	//float attackStrCount_;
	//std::vector<int>attackTex_;
	//float defenceStrCount_;
	//std::vector<int>defenceTex_;
	//float avoidStrCount_;
	//std::vector<int>avoidTex_;
	bool attackStrFlag_;
	bool defenceStrFlag_;
	bool avoidStrFlag_;
	//���x���A�b�v���̐����̓_��
	int LvAtCount_;
	int LvDxCount_;
	int LvAvCount_;
	int prevLvAttack_;
	int prevLvDefence_;
	int prevLvAvoid_;



	//�����ł̂ݎg�p����֐�
	//������
	void InitCollider(void)override;
	void InitShader(void)override;
	//�����蔻��
	void Collision(void)override;

	//�v���C���[�̕`��
	void DrawActor(void)override;
	//�}�[�N�̕`��
	void DrawMark(void)override;
	//���x���A�b�v�̕`��
	void DrawLevelUp(void);
	//���x���A�b�v�̕`��(�~���[)
	void DrawLevelUpMirror(void);

};
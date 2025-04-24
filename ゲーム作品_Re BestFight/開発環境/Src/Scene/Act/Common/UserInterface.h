#pragma once
#include<memory>
#include"../../../Utility/VectorHeader.h"

class Sound;
class Effect;

class UserInterface
{
public:

	//�X�e�[�^�X�̍ő�l
	//�ʏ�A���L��STATE�ƈꏏ�Ɏg��
	struct MAX_STATE
	{
		//���ꂼ��̍ő�l
		float hpMax;//�q�b�g�|�C���gMAX
		float attackMax;//�U����MAX
		float staminaMax;//�X�^�~�iMAX
		float defenceMax;//�h���MAX
		float avoidMax;//����MAX
		float skillMax;//�K�E�ZMAX

		int attackLevelMax;//�U�����x��MAX
		int defenceLevelMax;//�h�䃌�x��MAX
		int avoidLevelMax;//������x��Max
	};

	//�o���l�̍ő�l
	struct MAX_EXPERIENCE
	{
		float attackExMax;//�U���o���l�̍ő�l
		float defenceExMax;//�h��o���l�̍ő�l
		float avoidExMax;//����o���l�̍ő�l
	};

	//�������x���̌o���l
	//�ʏ�A���L��STATE�ƈꏏ�Ɏg��
	struct EXPERIENCE
	{
		float attackEx;//�U�����x���o���l
		float defenceEx;//�h�䃌�x���o���l
		float avoidEx;//������x���o���l
		MAX_EXPERIENCE max;//�ő�o���l
	};

	//�X�e�[�^�X�p�����[�^
	struct STATE
	{
		float hp;//�q�b�g�|�C���g
		float attack;//�U����
		float stamina;//�X�^�~�i
		float defence;//�h���
		float avoid;//����
		float skill;//�K�E�Z
		MAX_STATE max;//�ő�l

		//�������x��
		int attackLevel;//�U�����x��
		int defenceLevel;//�h�䃌�x��
		int avoidLevel;//������x��
		EXPERIENCE ex;//�o���l
	};

	//�������x���̍ő�l
	static constexpr int LEVEL_MAX = 5;
	//�Z���o����Œ�X�^�~�i
	static constexpr float MIN_STAMINA = 20.0f;
	//�K�E�Z�Q�[�W�̍ő�l
	static constexpr float SKILL_MAX = 100.0f;

	//����UI(���p�t)
	//���̓Q�[�W�̍ő�l
	static constexpr float MAX_MAGIC = 100.0f;

	//�R���X�g���N�^
	UserInterface(float hp, float attack, float stamina, float defence, float avoid, float skill);

	//�f�X�g���N�^
	~UserInterface(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	//�X�e�[�^�X�̒l�擾
	float GetStateStamina(void)const;
	float GetStateHp(void)const;
	float GetStateAttackPower(void)const;
	float GetStateAvoidPower(void)const;
	float GetStateDefenceMax(void)const;
	float GetStateSkill(void)const;
	int GetStateAttackLevel(void)const;
	int GetStateDefenceLevel(void)const;
	int GetStateAvoidLevel(void)const;
	float GetStateMaxStamina(void)const;
	float GetStateMaxHp(void)const;

	//�`���[�W
	void ChargeStateStamina(float stamina);
	void ChargeStateSkill(float skill);
	void ChargeStateAttackEx(float ex);
	void ChargeStateDefenceEx(float ex);
	void ChargeStateAvoidEx(float ex);

	//�X�e�[�^�X�̒l�̑���
	void SubHp(float sub);
	void SubStamina(float sub);
	void AddStamina(float add);
	void AddSkill(float add);
	void SubDefence(float sub);
	void AddDefence(float add);
	void SetDefence(float value);
	//�_���[�W�ʂ����Z
	void AddDamagePower(float add);
	//�_���[�W�ʂ��Z�b�g�ɂ���
	void SetDamagePower(float power);
	//�K�E�Z�Q�[�W�E�������x���̏�����
	void InitSkillandLevel(void);

	//�~���[�t���O���Z�b�g
	void SetMirrorFlag(bool flg);
	//�~���[�t���O���擾
	bool GetMirrorFlag(void)const;

	//�K�E�Z�Q�[�W�}�b�N�X���̃G�t�F�N�g����
	void SkillMaxStartEff(void);
	//�K�E�Z�Q�[�W�g�p���̃G�t�F�N�g��~����
	void SkillMaxEndEff(void);

	//���͂��g�p���邩���Z�b�g
	void SetMagicPower(bool flg);
	//���͗ʂ����Z
	void AddMagicPower(float add);
	//���͗ʂ����Z
	void SubMagicPower(float sub);
	//�c�薂�͂��擾
	float GetMagicPower(void)const;

	//�ϊ�
	STATE ToState(float hp, float attack, float stamina,
		float defence, float avoid, float skill,
		const MAX_STATE& max, int attackLev,
		int defenceLev, int avoidLev,
		const EXPERIENCE& ex)const;

	MAX_STATE ToMaxState(float hp, float attack, float stamina,
		float defence, float avoid, float skill,
		int attackLev, int defenceLev, int avoidLev)const;

	EXPERIENCE ToExperienceState(float ex_attack, float ex_defence, float ex_avoid, const MAX_EXPERIENCE& max);
	MAX_EXPERIENCE ToMaxExperienceState(float ex_attack, float ex_defence, float ex_avoid);

private:

	//HP�Q�[�W�̍��W
	static constexpr int HP_GAGE_XPOS = 0;
	static constexpr int HP_GAGE_YPOS = 0;

	//�X�^�~�i�Q�[�W�̍��W
	static constexpr int STAMINA_GAGE_XPOS = 0;
	static constexpr int STAMINA_GAGE_YPOS = 45;

	//���x���Q�[�W�̍��W
	static constexpr int LEVEL_GAGE_XPOS = 70;
	static constexpr int LEVEL_GAGE_YPOS = 400;
	static constexpr float LEVEL_EX_MAX_ADD = 100.0f;
	static constexpr Size2D LEVEL_UP_IMG_SIZE = { 80, 80 };
	//�f�t�H���g�̍ő僌�x���o���l
	static constexpr float DEFAULT_LEVEL_MAX = 100.0f;

	//�K�E�Z�Q�[�W�̍��W
	static constexpr int SKILL_GAGE_XPOS = 120;
	static constexpr int SKILL_GAGE_YPOS = 500;

	//���̓Q�[�W�̍��W
	static constexpr int MAGIC_GAGE_XPOS = 0;
	static constexpr int MAGIC_GAGE_YPOS = 85;

	//�f�o�b�O�p�̃X�e�[�^�X
	STATE state_;
	//�_���[�W��
	float damagePower_;
	//�_���[�W�t���O
	float damageColorTime_;

	//�\�����]�t���O
	bool mirrorFlag_;

	//�X�e�[�^�X�\���摜
	int imgHp_;//HP�Q�[�W
	int imgStamina_;//�X�^�~�i�Q�[�W
	int imgCase_;//�P�[�X
	int imgSkill_;//�K�E�Z�Q�[�W
	int imgSkillCase_;//�K�E�Z�P�[�X
	int imgLevel_;//���x���Q�[�W
	int imgLevelAttack_;//�U���̓��x���Q�[�W
	int imgLevelDefence_;//�h��̓��x���Q�[�W
	int imgLevelAvoid_;//������x���Q�[�W
	int imgMagic_;//���̓Q�[�W
	
	//�c�薂��
	float magicPower_;
	//���̓Q�[�W���������ǂ���
	bool magicFlag_;

	//�X�L��MAX���̃G�t�F�N�g
	std::weak_ptr<Effect>skillEff_;
	//�X�L��MAX���̌��ʉ�
	std::weak_ptr<Sound>skillSod_;



	//�����ł����g��Ȃ��֐�
	//�X�e�[�^�X�̒l�̏���E��������
	void LimitState(void);
	//���x���A�b�v����
	void LevelUp();

	//�`��
	void DrawHp(void);
	void DrawStamina(void);
	void DrawSkill(void);
	void DrawLevel(void);
	void DrawLevelAttack(void);
	void DrawLevelDefence(void);
	void DrawLevelAvoid(void);
	void DrawMagicPower(void);

	//�����v�Z
	void PercentGageX(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img);
	void PercentGageY(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img);

};
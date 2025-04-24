#pragma once
#include<memory>
#include<map>
#include<functional>
#include<vector>
#include<DxLib.h>
#include"Common/Transform.h"
#include"Common/UserInterface.h"
#include"Common/LevelUpManager.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/EffectManager.h"
#include"../../Manager/Effect.h"
#include"../../Manager/SoundManager.h"
#include"../../Manager/Sound.h"
#include"../../Utility/VectorHeader.h"

class ActorAi;
class NpcAi;
class Camera;
class UserInterface;
class AnimationController;
class Collider;
class ModelMaterial;
class ModelRender;
class CharactorBase;

#define INT_CAST(a)(static_cast<int>(a))

class ActorBase : public std::enable_shared_from_this<ActorBase>
{
public:

	//�v���C���[�̏��
	enum class STATE
	{
		NONE = -1,
		//�ҋ@���
		IDLE,
		//�ړ�
		MOVE,

		//��U��---------
		WEEK_1,
		WEEK_2,
		WEEK_3,
		WEEK_4,
		WEEK_5,
		WEEK_6,
		//--------------

		//���U��----------
		STRONG_1,
		STRONG_2,
		STRONG_3,
		STRONG_4,
		//---------------

		//���
		AVOID,
		//�o�b�N�X�e�b�v
		BACKSTEP,

		//�W�����v
		JUMP,
		//�h��
		DEFENCE,

		//����
		//�����ړ�
		QUICK_MOVE,
		//�����U��
		FALL_ATTACK,
		//�K�E�Z
		UNLEASH_SKILL,

		//�J�E���^�[�U��
		COUNTER_ATTACK,

		//������΂���Ă���
		BACK_WARD,

		//��낯��
		STAGGER_1,
		STAGGER_2,
		STAGGER_3,

		//�~�X
		MISS,
	};

	//�g�p����G�t�F�N�g
	//�񋓌^��z��̗v�f�ԍ��Ƃ��Ďg�p����
	//�����G�t�F�N�g�̃��\�[�X���ȏ�g���Ƃ�
	//���\�[�X�𑝂₳����vector�z��œ���ID������[�h����
	//resource[STRONG], resource[STRONG_2]�̂悤�Ɏg��
	enum class USE_EFFECT
	{
		JUMP = 0,

		SWING = 0,

		FLASH = 0,

		STRONG = 0,
		STRONG_2,

		SORD = 0,
		SORD_2,
		SORD_3,

		SORD_F = 0,

		ATTACK_WIND = 0,
		SKILL_FLAME = 0,
		LASER_FIRE = 0,

		LIGHTNING = 0,
		SKILL_SORD = 0,

		BLUR = 0,

		LEVEL_UP = 0,

		FALL_ATTACK = 0,

		CRASH_GROUND = 0,

		BARIA = 0,

		QUICK_MOVE = 0,

		JUST = 0,

		LASER_MAGIC = 0,
		LASER_VIOLENCE = 0,
		AURA = 0,
		BIG_BANG = 0,
		FIRE_MINE = 0,
		TRNADE = 0,
		FHANTASM = 0,
		SOGEKI = 0,
		METEO = 0,
		MAHOUJIN = 0,

		GURD_BREAK = 0,
	};

	//�G�t�F�N�g�̏��
	//�G�t�F�N�g���Ƃɗp�ӂ���
	struct EFFECT_STATE
	{
		Dir3DF scl;//�傫��
		float speed;//�Đ����x
	};

	static constexpr float DEFAULT_WEIGHT = 0.01f;

	static constexpr float QUICK_SPEED = 100.0f;//�f�o�b�O�p10.0f

	//���[�J���J����
	std::shared_ptr<Camera>actorCam_;

	//�L�����̃f�[�^����ꏈ��
	std::unique_ptr<CharactorBase>charactor_;

	//�f�o�b�O�p�̃g�����X�t�H�[��
	std::shared_ptr<Transform>transform_;

	//�f�o�b�O�p�̃J�����A�h���X
	std::weak_ptr<Camera>camera_;

	//�f�o�b�O�p�̓����蔻��C���X�^���X
	std::unique_ptr<Collider>collider_;

	//�f�o�b�O�p��UI
	std::shared_ptr<UserInterface>userInterface_;

	//���x���A�b�v�}�l�[�W��
	std::unique_ptr<LevelUpManager>levelUpMng_;

	//�f�o�b�O�p�̃A�j���[�V�����R���g���[��
	std::shared_ptr<AnimationController>animController_;

	//AI
	std::shared_ptr<ActorAi>actAi_;
	std::unique_ptr<NpcAi>npcAi_;

	//�f�o�b�O�p�̓G�C���X�^���X
	std::weak_ptr<ActorBase>enemy_;

	//�X�V�\��
	bool updateFlg_;

	//�R���X�g���N�^
	ActorBase();

	//�f�X�g���N�^
	virtual ~ActorBase(void);

	//���z������
	virtual void Init(std::weak_ptr<ActorBase>enemy, SceneManager::CHARA_ID id) = 0;
	void InitCamera(void);
	virtual void InitMark(void) = 0;
	//���z�X�V
	virtual void Update(void) = 0;
	//���z�`��
	virtual void Draw(void) = 0;

	//�_���[�W���ɗ^�����郂�[�V�������Z�b�g
	void SetDamageMotion(const std::function<void(void)>&func);
	//�_���[�W���̃��[�V�����I������
	void EndDamageMotion(void);

	//���݂̏�Ԃ��擾
	STATE GetState(void)const;
	//�_���[�W���󂯂����ǂ���
	bool IsDamageAnimFlag(void)const;
	//�n�ʂɓ������Ă��邩
	bool IsCollGroundFlag(void)const;
	//3D���f���ɓ������Ă��邩
	bool IsCollObjFlag(void)const;

	Position3DF GetPos(void)const;

	//�g�p���Ă��郂�f���̃��\�[�XID���擾
	virtual SceneManager::CHARA_ID GetModelResource(void);

	//������̃x�N�g�����Z�b�g
	void SetJumpVector(const VECTOR& vec);
	//�������̃x�N�g�����Z�b�g
	void SetSwayVector(const VECTOR& vec);

	//�̏d���Z�b�g
	void SetWeight(float weight);
	//�̏d���f�t�H���g�ɂ���
	void SetDefaultWeight();
	//�X�V�\�t���O���Z�b�g
	void SetUpdateFlag(bool flg);
	//���W���Z�b�g
	void SetPos(const Position3DF& pos);
	//�X�g�b�v�t���[�����Z�b�g
	void SetStopFrame(float time);

	//���̃v���C���[�̏�ԑJ��
	void ChangeState(const STATE& state);
	//�x�N�g���ɐ����ȃ|���S�����쐬
	void PoligonVecNormInfo(VERTEX3D* ver, const VECTOR& vec);
	//�^��x�N�g���ɐ����ȃ|���S�����쐬
	void PoligonVecNormUpInfo(VERTEX3D* ver, const VECTOR& vec);
	//�����p�̃|���S��
	//void PolligonStrInfo(VERTEX3D* ver, VECTOR tpos, float tu, float tv, Color3D color, float ca);

	//���x���Q�b�g
	virtual float GetSpeed(void)const;
	//�U���͈�
	virtual float GetAttackArea(void)const;
	//����
	virtual float GetAvoidPower(void)const;
	//�W�����v��
	virtual float GetJumpPower(void)const;
	//�h���
	virtual float GetStateDefenceMax(void)const;
	//�U����
	virtual float GetAttackPower(void)const;
	//�����̐F
	virtual Color3D GetMyColor(void)const;
	//�����蔻��̔����^�C�~���O
	virtual float GetAttackHappen(void)const;

	//CPU�ł��邩
	bool GetCpuFlag(void)const;
	//�v���C���[ID�̎擾
	virtual int GetPlayerId(void)const;


	//�^����p�̃_���[�W���[�V����----------------------

	//�E�ɂ�낯��
	void DamageRightStagger(void);
	//���ɂ�낯��
	void DamageLeftStagger(void);
	//���ɂ�낯��
	void DamageBackStagger(void);
	//�������
	void DamageBlowStagger(void);
	//�K�[�h�u���C�N���ꂽ
	void DamageGurdBreak(void);
	//�_���[�W���[�V�����������^�����Ă��Ȃ��Ƃ�
	void NoneDamage(void);
	//--------------------------------------------------

	//�G�t�F�N�g�Đ�
	void PlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos, const Rot3DF& rot);
	//�G�t�F�N�g��~
	void StopEffect(const EffectManager::EFF& type, const USE_EFFECT& number);
	//�G�t�F�N�g���Đ����Ă��邩�ǂ���
	bool IsPlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number);
	//�w��̃G�t�F�N�g�̍��W���Z�b�g
	void SetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos);
	//�w��̃G�t�F�N�g�̉�]���Z�b�g
	void SetEffectRot(const EffectManager::EFF& type, const USE_EFFECT& number, const Rot3DF& rot);
	//�w��̃G�t�F�N�g�̍��W���擾
	Position3DF GetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number)const;

	//�T�E���h�Đ�
	void SoundPlay(const SoundManager::SOD& soundType, int playType);
	//�T�E���h��~
	void SoundStop(const SoundManager::SOD& soundType);

	//�v���C���[�̑O���̑��΍��W
	//�{���ŋ����ω�
	virtual Position3DF GetPlayerForwardPos(const float& scl)const;

	//�v���C���[�̉�]��K�؂ɕ␳����
	//�܂�A180�xY����]����
	virtual Rot3DF GetCollectionPlayerRot()const;

	//�K�E�Z�̔�������
	virtual bool IsUnleashSkill(void)const;
	//�K�E�Z���̃J��������
	virtual void SkillCamera(void);
	//��U���t�B�j�b�V���U���̃J��������
	virtual void WeekFinishCamera(void);
	//���U���t�B�j�b�V���U���̃J��������
	virtual void StrongFinishCamera(void);

	//�N�����Ă����������[�v�G�t�F�N�g�̒�~
	virtual void StopAllEffect(void);

	//�J�v�Z����
	CapsuleF Model2Capsule(void);
	CapsuleF Model2Capsule(float scl, unsigned int r);

	//����
	SphereF Model2Sphere();
	SphereF Model2Sphere(unsigned int r);

protected:

	static constexpr Size3DF MARK_SCALE = { 0.2f, 0.2f, 0.2f };

	using EffectData = std::map<EffectManager::EFF, std::vector<std::weak_ptr<Effect>>>;
	using EffectStateData = std::map<EffectManager::EFF, EFFECT_STATE>;
	using DamageAnimFunc = std::function<void(void)>;
	using SoundData = std::map<SoundManager::SOD, std::weak_ptr<Sound>>;

	//�G�t�F�N�g�̃x�N�g���z��
	EffectData effectsMap_;
	EffectStateData effectsState_;

	//�T�E���h�f�[�^
	SoundData soundsMap_;

	//CPU�Ƃ��Đ키��
	bool cpuFlag_;

	//�f�o�b�O�p�̃_���[�W�A�j���[�V�����t���O
	bool damageAnimFlag_;
	//�f�o�b�O�p�̗^������_���[�W���[�V����
	DamageAnimFunc damageAnim_;

	//���_�}�e���A��
	std::unique_ptr<ModelMaterial>material_;
	//���_�����_�[
	std::unique_ptr<ModelRender>render_;

	//�ꎞ��~�t���[���^�C��
	float stopFrame_;

	//�̏d
	float weight_;

	//���
	STATE state_;
	
	//������̃x�N�g��
	VECTOR jumpVec_;

	//�������̃x�N�g��
	VECTOR swayVec_;

	//�����蔻��t���O
	//��ނɌ��炸�A�����������ǂ���
	bool collFlag_;
	//3D���f���ɓ����������ǂ���
	bool collObjFlag_;

	//���g��\����
	std::unique_ptr<Transform>mark_;



	//�����ł̂ݎg�p����֐�
	//���z������
	virtual void InitCollider(void) = 0;
	virtual void InitShader(void);
	void InitCharactor(SceneManager::CHARA_ID id);
	void InitCharactorAi(SceneManager::CHARA_ID id);
	//void TextInit(std::vector<int>& texVec, std::string str);//�e�L�X�g�n���h��ID�̏�����

	//��̉�]����
	void MarkRotation(void);

	//���z�����蔻��
	virtual void Collision(void) = 0;

	//�d�͏���
	void Gravity(void);

	//���C����
	void Friction(void);

	//�X�g�b�v�t���[�����X�V
	void UpdateStopFlame(void);

	//���z�A�N�^�[�̕`��
	virtual void DrawActor(void) = 0;
	virtual void DrawMark(void) = 0;

};
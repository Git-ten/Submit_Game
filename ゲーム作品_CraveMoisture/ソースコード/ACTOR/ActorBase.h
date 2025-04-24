#pragma once
#include <memory>
#include <list>
#include "../COMMON/Transform.h"
#include "../Common/Time.h"
#include "../Common/EffectPlayLaunch.h"
#include "../Common/SoundPlayLaunch.h"
#include "../Common/AnimationController.h"
#include "../../Scene/GUI/Image.h"
#include "../../Common/DataLoad/DataCameraControl.h"
#include "ActorData.h"

class ActorBase
{
protected:

	static constexpr float GRAVITY_UP_OFFSET = 300.0f;
	static constexpr float GRAVITY_DOWN_OFFSET = 20.0f;

	static constexpr float GAGE_INIT = 0.0f;

	static constexpr float ATTACK_RAG_MAX = 1.0f;

	static constexpr float HP_MIN = 0.0f;

	static constexpr float EFFECT_DEFAULT_SPEED = 100.0f;

	struct ACTOR_COLLISION
	{
		Position3DF* lockOnPos = nullptr;

		std::weak_ptr<Line3DF>collisionGravity;
		std::weak_ptr<SphereF>collisionSphere;

		std::weak_ptr<SphereF>collisionAttackSphere;
		int* collisiionAttackModelId_;

		int* collisionModelId;
		int* collisionGravityModelId;

		bool isCollAttack_ = false;

		bool isCollHorizontal = true;
		bool isMyselfEnemy = false;

		float* damagePower = nullptr;
	};

public:

	static constexpr float ENEMY_LEVEL_EASY = 0.5f;
	static constexpr float ENEMY_LEVEL_NORMAL = 0.7f;
	static constexpr float ENEMY_LEVEL_HARD = 1.0f;

	// �R���X�g���N�^
	ActorBase(void);
	// �f�X�g���N�^
	virtual ~ActorBase(void);

	void Init(void);
	virtual void InitSpecial(void);

	void Update(void);
	virtual void Draw(void);

	void Release(void);

	// �X���[���[�V�������Z�b�g�A�b�v(���g�ɂ����e�����Ȃ�����)
	void SetUpMySlow(bool isSlow);

	// �A�N�^�[���m�̓����蔻����̎Q�Ƃ�ǉ�
	void SetOtherActorCollision(ACTOR_COLLISION& actorColl, const std::list<ACTOR_COLLISION*>& addActorList = std::list<ACTOR_COLLISION*>());
	// ���g�̓����蔻������擾
	ACTOR_COLLISION& GetMyCollision(void);

	// �����ȊO�œ��ʂɒǉ�����A�N�^�[�̓����蔻����
	const std::list<ACTOR_COLLISION*>& GetAddSpecialActorCollision(void);

	const std::vector<std::shared_ptr<Image>>& GetImagesUi(void);

	void SetOtherActorSlowFunc(const std::function<void(bool)> func);
	const std::function<void(bool)> GetMySlowFunc(void);

	// �o�g���X�^�[�g����(�K�p���邩�͊e�X�̌p����Ō��߂�)
	virtual void ActorBattleStart(void);

	// �A�C�R���`��p::�c��̃��L���X�g�^�C����`��
	virtual void DrawIconRecastTime(void);

	// �R�[���o�b�N�֐�
	virtual bool BattleStepCondition(void);

	int GetModelId(void);

	Position3DF GetPos(void);

	bool IsTouchGround(void);

	// �����ł������������Ă����true
	bool IsInertia(void);

	virtual float NowHp(void);

	// �v���C���[�݂̂Ɏg��
	virtual bool IsDash(void);
	virtual bool IsCharge(void);
	virtual bool IsTechnique(void);
	virtual bool IsParysOrJustAvoid(void);
	virtual bool IsSkill(void);

private:

	// �X�e�[�g���Ƃ̃J��������f�[�^
	using CameraControlData = std::map<int, std::vector<DataCameraControl>>;

protected:

	// �X�e�b�v���ƂɃo�g���}�l�[�W���[�֕Ԃ���������ω�������
	int actorBattleStep_;

	// �o�g���X�^�[�g�t���O
	bool isBattleStart_;

	// ���������̃X���[���[�V�����t���O
	bool isMySlow_;

	// �A�N�^�[�̃g�����X�t�H�[��
	std::shared_ptr<Transform>actorTransform_;

	// �A�N�^�[�̃A�j���[�V����
	std::unique_ptr<AnimationController>actorAnime_;

	ACTOR_COLLISION myActorCollision_;

	// �A�N�^�[���m�̓����蔻����ꗗ
	std::list<ACTOR_COLLISION*>otherActorCollisiton_;

	std::list<ACTOR_COLLISION*>addOtherActorCollision_;

	// �A�N�^�[��p��UI�摜
	std::vector<std::shared_ptr<Image>>actorUi_;
	
	// �A�N�^�[�̍U�����O
	std::unique_ptr<Time>actorAttackRagTime_;

	// �G�t�F�N�g�̃����`���[
	std::unique_ptr<EffectPlayLaunch>actorEffectLaunch_;
	// �G�t�F�N�g�̏��
	std::map<int, DataEffect>effectInfo_;

	// ���ʉ��̃����`���[
	std::unique_ptr<SoundPlayLaunch>waveLaunch_;

	// �ړ�����
	Dir3DF actorMoveDir_;

	// �n�ʂɑ������Ă��邩
	bool isTouchGround_;

	// �d��
	VECTOR gravity_;
	// ����
	VECTOR inertia_;

	// �X���[���[�V�������ł��邩
	bool isSlowActor_;

	// �����ȊO�̃A�N�^�[�ɃX���[���[�V���������L����ꍇ�g�p
	std::vector<std::function<void(bool)>>otherSlowActor_;

	// �J�������[�N
	CameraControlData cameraControle_;

	virtual void SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl);
	
	virtual void InitActorTransform(void);
	virtual void InitActorAnimation(void);
	virtual void InitActorUi(void);
	virtual void InitActorEffectLaunch(void);
	virtual void InitActorEffectInfo(void);
	virtual void InitActorCameraControl(void);
	virtual void InitActorSoundLaunch(void);

	// �w�肵���X�e�[�g�̃J��������f�[�^��ǉ�
	void AddStateCameraControlData(const int state, const LoadData::LoadDataInfo& info);

	void ActorCollision(void);
	virtual void CollisionedGravityActor(Position3DF pos);
	virtual void CollisionedHorizontalActor(void);
	virtual void CollisionedDamageActor(float* damagePower = nullptr);

	virtual void StopAttackEffect(void);
	virtual void StopWaveSound(void);

	virtual void UpdateSpecial(void);
	virtual void DrawSpecial(void);

	void Gravity(ActorData& data);
	void Inertia(ActorData& data);

	void SetActorSlow(bool isSlow);

	void ActorMoveVector(ActorData& data, Dir3DF vec, int addPower = 0);
	void ActorMoveDistance(ActorData& data, Dir3DF vec, float dis);
	void ActorMoveSet(ActorData& data, Position3DF pos);

	void ActorOtherLockOnRotation(const ActorData& data, bool isRotCamera = false, bool isRotActor = true, bool isLerpCamera = false);
	float GetDistanceActor(const ActorData& data);

	void ActorRotationDir(const ActorData& data, Dir3DF dir);
	Dir3DF GetActorOtherLockonDir(const ActorData& data, bool isRotCamera = false, 
		const Position3DF& pos = Position3DF(), Position3DF* getPos = nullptr, bool isLerpCamera = false);

	DataEffect GetEffectDataInfo(const int useNumber);

private:

	void ActorMove(ActorData& data, Dir3DF dir, float dis);

};
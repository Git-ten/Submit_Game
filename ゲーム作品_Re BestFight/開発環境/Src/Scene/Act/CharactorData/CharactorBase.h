#pragma once
#include<memory>
#include<map>
#include<functional>
#include"../../../Manager/ResourceManager.h"
#include"../../../Manager/EffectManager.h"
#include"../../../Manager/Effect.h"
#include"../../../Manager/SoundManager.h"
#include"../../../Manager/Sound.h"
#include"../../../Manager/SceneManager.h"
#include"../ActorBase.h"

class Camera;
class UserInterface;
class AnimationController;
class Transform;


class CharactorBase
{

public:

	//�f�o�b�O�p�̃g�����X�t�H�[��
	std::shared_ptr<Transform>transform_;

	//�f�o�b�O�p�̃J�����A�h���X
	std::weak_ptr<Camera>camera_;

	//�f�o�b�O�p��UI
	std::shared_ptr<UserInterface>userInterface_;

	//�f�o�b�O�p�̃A�j���[�V�����R���g���[��
	std::shared_ptr<AnimationController>animController_;

	//�f�o�b�O�p�̓G�C���X�^���X
	std::weak_ptr<ActorBase>enemy_;

	using EffectData = std::map<EffectManager::EFF, std::vector<std::weak_ptr<Effect>>>;
	using EffectStateData = std::map < EffectManager::EFF, ActorBase::EFFECT_STATE >;
	using DamageAnimFunc = std::function<void(void)>;
	using SoundData = std::map<SoundManager::SOD, std::weak_ptr<Sound>>;

	//�G�t�F�N�g�̃x�N�g���z��
	EffectData effectsMap_;
	EffectStateData effectsState_;

	//�T�E���h�f�[�^
	SoundData soundsMap_;

	CharactorBase(void);
	virtual ~CharactorBase();

	//������
	virtual void Init(std::weak_ptr<ActorBase>enemy) = 0;
	virtual void InitTransform(void) = 0;


	//�g�p���Ă��郂�f���̃L����ID���擾
	virtual SceneManager::CHARA_ID GetModelResource(void);
	//�U���͈�
	virtual float GetAttackArea(void)const;
	//����
	virtual float GetAvoidPower(void)const;
	//�W�����v��
	virtual float GetJumpPower(void)const;
	//�h���
	virtual float GetDefencePower(void)const;
	//�����̐F
	virtual Color3D GetMyColor(void)const;
	//�I�[�g���[�h���̍U���؂�ւ����^�C�~���O
	virtual float GetAttackHappen(ActorBase::STATE type)const;
	//���x
	virtual float GetSpeed()const;
	//�J�����̃Z�b�g
	void SetActorCamera(std::weak_ptr<Camera>actCam);

	//�K�E�Z�̔�������
	virtual bool IsUnleashSkill(void)const;
	//�K�E�Z���̃J��������
	virtual void SkillCamera(void);
	//��U���t�B�j�b�V���U���̃J��������
	virtual void WeekFinishCamera(void);
	//���U���t�B�j�b�V���U���̃J��������
	virtual void StrongFinishCamera(void);

protected:

	//�����ł̂ݎg�p����֐�
	//���z������
	virtual void InitAnim(void) = 0;
	virtual void InitEffect(void);
	virtual void InitEffectState(void);
	virtual void InitSound(void);

};


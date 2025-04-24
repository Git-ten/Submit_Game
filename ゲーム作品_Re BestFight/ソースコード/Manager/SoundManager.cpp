#include<DxLib.h>
#include"Sound.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}

	instance_->Init();
}

SoundManager& SoundManager::GetInstance()
{
	return *instance_;
}

void SoundManager::Init()
{
	std::string PATH_MUSIC = "Data/Sound/Music/";
	std::string PATH_WAVE = "Data/Sound/Wave/";

	
	Sound res;

	//�^�C�g��BGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Title.mp3");
	soundsMap_.emplace(SOD::TITLE_BGM, res);

	//�o�g��BGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Battle.mp3");
 	soundsMap_.emplace(SOD::BATTLE_BGM, res);
	
	//�Z���N�gBGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Select.mp3");
 	soundsMap_.emplace(SOD::SELECT_BGM, res);
	
	//���U���g��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Victory.mp3");
 	soundsMap_.emplace(SOD::VICTORY, res);
	
	//�N���b�V���U����
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Attack_Crash.mp3");
 	soundsMap_.emplace(SOD::ATTACK_CRASH, res);

	//���Ō���
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Attack_Strong.mp3");
	soundsMap_.emplace(SOD::ATTACK_STRONG, res);

	//��U�艹
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Swing.mp3");
	soundsMap_.emplace(SOD::SWING, res);

	//����
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SordFinish.mp3");
	soundsMap_.emplace(SOD::SORD_FINISH, res);

	//��������������
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SordHit.mp3");
	soundsMap_.emplace(SOD::SORD_HIT, res);

	//�T���_�[�{���g��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "ThunderBolt_S.mp3");
	soundsMap_.emplace(SOD::THUNDER_BOLT, res);

	//���K�E�Z��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SkilI_Flame.mp3");
	soundsMap_.emplace(SOD::SKILL_FLAME, res);

	//���K�E�Z
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Skill_Thunder.mp3");
	soundsMap_.emplace(SOD::SKILL_THUNDER, res);

	//�K�E�Z�Q�[�W�}�b�N�X
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SkillMax.mp3");
 	soundsMap_.emplace(SOD::SKILL_MAX, res);

	//���艹
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision.mp3");
	soundsMap_.emplace(SOD::DECISION, res);

	//���艹2
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision_2.mp3");
	soundsMap_.emplace(SOD::DECISION_2, res);

	//���艹3
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision_3.mp3");
	soundsMap_.emplace(SOD::DECISION_3, res);

	//�V�[���؂�ւ���
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "ChangeScene.mp3");
	soundsMap_.emplace(SOD::CHANGE_SCENE, res);

	//�W���X�g��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Just.mp3");
	soundsMap_.emplace(SOD::JUST, res);

	//��������
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Finish.mp3");
 	soundsMap_.emplace(SOD::FINISH, res);

	//覐Ή�
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Meteo.mp3");
 	soundsMap_.emplace(SOD::METEO, res);
	
	//覐΃q�b�g��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Meteo_Hit.mp3");
 	soundsMap_.emplace(SOD::METEO_HIT, res);
	
	//�X�i�C�v��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Snipe.mp3");
 	soundsMap_.emplace(SOD::SNIPE, res);
	
	//�K�[�h�u���C�N��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "GurdBreak.mp3");
 	soundsMap_.emplace(SOD::GURD_BREAK, res);

	//�����ړ���
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "QuickMove.mp3");
 	soundsMap_.emplace(SOD::QUICK_MOVE, res);
	
	//�s����
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "NotPossible.mp3");
 	soundsMap_.emplace(SOD::NOT_POSSIBLE, res);

	//�J�E���g�_�E��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "CountDown.mp3");
 	soundsMap_.emplace(SOD::COUNT_DOWN, res);

	//�X�^�[�g��
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "StartShout.mp3");
 	soundsMap_.emplace(SOD::START, res);
}

void SoundManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
	}

	loadedMap_.clear();
}

void SoundManager::Destroy(void)
{
	Release();
	soundsMap_.clear();
	delete instance_;
}

std::weak_ptr<Sound> SoundManager::Load(const SOD& sod)
{
	std::weak_ptr<Sound> res = _Load(sod);
	return res;
}

std::weak_ptr<Sound> SoundManager::_Load(const SOD& sod)
{
	const auto& lPair = loadedMap_.find(sod);
	std::shared_ptr<Sound>ptrSod = nullptr;
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& sPair = soundsMap_.find(sod);
	if (sPair != soundsMap_.end())
	{
		sPair->second.Load();

		ptrSod = std::make_shared<Sound>(sPair->second);
		loadedMap_.emplace(sod, ptrSod);
	}

	return ptrSod;
}

SoundManager::SoundManager()
{
}

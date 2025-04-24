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

	//タイトルBGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Title.mp3");
	soundsMap_.emplace(SOD::TITLE_BGM, res);

	//バトルBGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Battle.mp3");
 	soundsMap_.emplace(SOD::BATTLE_BGM, res);
	
	//セレクトBGM
	res = Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Select.mp3");
 	soundsMap_.emplace(SOD::SELECT_BGM, res);
	
	//リザルト音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Victory.mp3");
 	soundsMap_.emplace(SOD::VICTORY, res);
	
	//クラッシュ攻撃音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Attack_Crash.mp3");
 	soundsMap_.emplace(SOD::ATTACK_CRASH, res);

	//強打撃音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Attack_Strong.mp3");
	soundsMap_.emplace(SOD::ATTACK_STRONG, res);

	//空振り音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Swing.mp3");
	soundsMap_.emplace(SOD::SWING, res);

	//抜刀
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SordFinish.mp3");
	soundsMap_.emplace(SOD::SORD_FINISH, res);

	//剣が当たった音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SordHit.mp3");
	soundsMap_.emplace(SOD::SORD_HIT, res);

	//サンダーボルト音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "ThunderBolt_S.mp3");
	soundsMap_.emplace(SOD::THUNDER_BOLT, res);

	//炎必殺技音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SkilI_Flame.mp3");
	soundsMap_.emplace(SOD::SKILL_FLAME, res);

	//雷必殺技
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Skill_Thunder.mp3");
	soundsMap_.emplace(SOD::SKILL_THUNDER, res);

	//必殺技ゲージマックス
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "SkillMax.mp3");
 	soundsMap_.emplace(SOD::SKILL_MAX, res);

	//決定音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision.mp3");
	soundsMap_.emplace(SOD::DECISION, res);

	//決定音2
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision_2.mp3");
	soundsMap_.emplace(SOD::DECISION_2, res);

	//決定音3
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Decision_3.mp3");
	soundsMap_.emplace(SOD::DECISION_3, res);

	//シーン切り替え音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "ChangeScene.mp3");
	soundsMap_.emplace(SOD::CHANGE_SCENE, res);

	//ジャスト音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Just.mp3");
	soundsMap_.emplace(SOD::JUST, res);

	//勝負あり
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Finish.mp3");
 	soundsMap_.emplace(SOD::FINISH, res);

	//隕石音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Meteo.mp3");
 	soundsMap_.emplace(SOD::METEO, res);
	
	//隕石ヒット音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Meteo_Hit.mp3");
 	soundsMap_.emplace(SOD::METEO_HIT, res);
	
	//スナイプ音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "Snipe.mp3");
 	soundsMap_.emplace(SOD::SNIPE, res);
	
	//ガードブレイク音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "GurdBreak.mp3");
 	soundsMap_.emplace(SOD::GURD_BREAK, res);

	//高速移動音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "QuickMove.mp3");
 	soundsMap_.emplace(SOD::QUICK_MOVE, res);
	
	//不発音
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "NotPossible.mp3");
 	soundsMap_.emplace(SOD::NOT_POSSIBLE, res);

	//カウントダウン
	res = Sound(Sound::TYPE::WAVE, PATH_WAVE + "CountDown.mp3");
 	soundsMap_.emplace(SOD::COUNT_DOWN, res);

	//スタート音
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

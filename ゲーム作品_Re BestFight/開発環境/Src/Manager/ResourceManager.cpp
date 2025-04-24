#include <DxLib.h>
#include "../Application.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	resourcesMap_.clear();
	loadedMap_.clear();

	Resource res;
	std::string PATH_IMAGE = "Data/Image/";
	std::string PATH_MODEL = "Data/Model/";
	std::string PATH_EFFECT = "Data/Effect/";

	//タイトル画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title_1.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	//PlayGame
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "PlayGame.png");
	resourcesMap_.emplace(SRC::PLAYGAME, res);

	//PlayGame_Input
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "PlayGame_Input.png");
	resourcesMap_.emplace(SRC::PLAYGAME_IN, res);

	//AboutGame
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "AboutGame.png");
	resourcesMap_.emplace(SRC::ABOUTGAME, res);

	//AboutGameInput
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "AboutGame_Input.png");
	resourcesMap_.emplace(SRC::ABOUTGAME_IN, res);

	//リザルト画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Result.png");
	resourcesMap_.emplace(SRC::RESULT, res);

	//ゲーム説明画像_1
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ExPlain_1.png");
	resourcesMap_.emplace(SRC::EXPLAIN_1, res);
	
	//ゲーム説明画像_2
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ExPlain_2.png");
	resourcesMap_.emplace(SRC::EXPLAIN_2, res);
	
	//ゲーム説明画像_3
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ExPlain_3.png");
	resourcesMap_.emplace(SRC::EXPLAIN_3, res);
	
	//ゲーム説明画像_4
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Fighter.png");
	resourcesMap_.emplace(SRC::EXPLAIN_4, res);
	
	//ゲーム説明画像_4_2
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Fighter_2.png");
	resourcesMap_.emplace(SRC::EXPLAIN_4_2, res);
	
	//ゲーム説明画像_4_3
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Fighter_3.png");
	resourcesMap_.emplace(SRC::EXPLAIN_4_3, res);
	
	//ゲーム説明画像_5
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_SordMan.png");
	resourcesMap_.emplace(SRC::EXPLAIN_5, res);
	
	//ゲーム説明画像_5_2
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_SordMan_2.png");
	resourcesMap_.emplace(SRC::EXPLAIN_5_2, res);
	
	//ゲーム説明画像_5_3
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_SordMan_3.png");
	resourcesMap_.emplace(SRC::EXPLAIN_5_3, res);
	
	//ゲーム説明画像_6
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Magician.png");
	resourcesMap_.emplace(SRC::EXPLAIN_6, res);
	
	//ゲーム説明画像_6_2
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Magician_2.png");
	resourcesMap_.emplace(SRC::EXPLAIN_6_2, res);
	
	//ゲーム説明画像_6_3
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Magician_3.png");
	resourcesMap_.emplace(SRC::EXPLAIN_6_3, res);
	
	//ゲーム説明画像_6_4
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "About_Magician_4.png");
	resourcesMap_.emplace(SRC::EXPLAIN_6_4, res);
	
	//フェイド時のゲーム説明
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Explain_EX.png");
	resourcesMap_.emplace(SRC::EXPLAIN_EX, res);
	
	//メニュー画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Menyu.png");
	resourcesMap_.emplace(SRC::MENYU, res);
	
	//左ボタン
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Left_Key.png");
	resourcesMap_.emplace(SRC::LEFT_KEY, res);
	
	//右ボタン
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Right_Key.png");
	resourcesMap_.emplace(SRC::RIGHT_KEY, res);

	//プレイヤー1の印
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Object/Mark_1.mv1");
	resourcesMap_.emplace(SRC::MARK_1, res);

	//プレイヤー2の印
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Object/Mark_2.mv1");
	resourcesMap_.emplace(SRC::MARK_2, res);

	//プレイヤー(格闘家)
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/Fist/stickMan_4.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FIST, res);
	
	//プレイヤー(剣士)
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/Sord/stickMan_4_β.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SORD, res);
	
	//プレイヤー(魔術師)
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/Magic/stickMan_4_Γ.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MAGIC, res);

	//ステージ
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Stage/Stage_3.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

	//空画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Back_Title.png");
	resourcesMap_.emplace(SRC::SKY, res);

	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Skydome/Skydome.mv1");
	resourcesMap_.emplace(SRC::SKYDOME_1, res);

	//UI
	//HPゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Hp_2.png");
	resourcesMap_.emplace(SRC::HP_GAGE, res);

	//HPゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_Hp_2.png");
	resourcesMap_.emplace(SRC::B_HP_GAGE, res);

	//スタミナゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Stamina_2.png");
	resourcesMap_.emplace(SRC::STAMINA_GAGE, res);
	
	//スタミナゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_Stamina_2.png");
	resourcesMap_.emplace(SRC::B_STAMINA_GAGE, res);

	//ケース
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Case_2.png");
	resourcesMap_.emplace(SRC::CASE, res);

	//必殺技ゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Skill.png");
	resourcesMap_.emplace(SRC::SKILL_GAGE, res);
	
	//必殺技ゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_Skill.png");
	resourcesMap_.emplace(SRC::B_SKILL_GAGE, res);

	//必殺技ゲージケース
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "SkillCase.png");
	resourcesMap_.emplace(SRC::SKILL_CASE, res);

	//レベルゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "LevelUp.png");
	resourcesMap_.emplace(SRC::LEVEL_GAGE, res);

	//攻撃力レベルゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "LevelUp_Attack.png");
	resourcesMap_.emplace(SRC::LEVEL_ATTACK_GAGE, res);

	//攻撃力レベルゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_LevelUp_Attack.png");
	resourcesMap_.emplace(SRC::B_LEVEL_ATTACK_GAGE, res);

	//防御力レベルゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "LevelUp_Defence.png");
	resourcesMap_.emplace(SRC::LEVEL_DEFENCE_GAGE, res);

	//防御力レベルゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_LevelUp_Defence.png");
	resourcesMap_.emplace(SRC::B_LEVEL_DEFENCE_GAGE, res);

	//回避力レベルゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "LevelUp_Avoid.png");
	resourcesMap_.emplace(SRC::LEVEL_AVOID_GAGE, res);

	//回避力レベルゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_LevelUp_Avoid.png");
	resourcesMap_.emplace(SRC::B_LEVEL_AVOID_GAGE, res);

	//魔力ゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Magic.png");
	resourcesMap_.emplace(SRC::MAGIC_GAGE, res);

	//魔力ゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_Magic.png");
	resourcesMap_.emplace(SRC::B_MAGIC_GAGE, res);

	//セレクトカーソル画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "SelectCarsol.png");
	resourcesMap_.emplace(SRC::SELECT_CARSOL, res);

	//セレクト画面画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Select.png");
	resourcesMap_.emplace(SRC::SELECT, res);

	//開始ボタン画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Start.png");
	resourcesMap_.emplace(SRC::START, res);

	//開始ボタン入力画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Start_Input.png");
	resourcesMap_.emplace(SRC::START_INPUT, res);

	//チェーン画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "chain.png");
	resourcesMap_.emplace(SRC::CHAIN, res);
	
	//戻るボタン
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ReturnButton.png");
	resourcesMap_.emplace(SRC::RETURN_BUTTON, res);

	//戻るボタンインプット
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ReturnButton_Input.png");
	resourcesMap_.emplace(SRC::RETURN_BUTTON_INPUT, res);
	
	//戻るボタンゲージ
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "ReturnButton_Gage.png");
	resourcesMap_.emplace(SRC::RETURN_BUTTGAGE, res);
	
	//戻るボタンゲージ(Big)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "B_ReturnButton_Gage.png");
	resourcesMap_.emplace(SRC::B_RETURN_BUTTGAGE, res);

	//電源ボタン
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Switch.png");
	resourcesMap_.emplace(SRC::SWITCH, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Release(const SRC& src, int handleId)
{
	std::weak_ptr<Resource> res = _Load(src);
	if (res.lock() == nullptr)
	{
		return;
	}

	res.lock()->Release(handleId);
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

std::weak_ptr<Resource> ResourceManager::Load(const SRC& src)
{
	std::weak_ptr<Resource> res = _Load(src);
	return res;
}

int ResourceManager::LoadModelDuplicate(const SRC& src)
{
	std::weak_ptr<Resource> res = _Load(src);
	if (res.lock() == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.lock()->handleId_);
	res.lock()->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

std::weak_ptr<Resource> ResourceManager::_Load(const SRC& src)
{
	const auto& lPair = loadedMap_.find(src);
	std::shared_ptr<Resource>ptrRes = nullptr;
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair != resourcesMap_.end())
	{
		rPair->second.Load();

		ptrRes = std::make_shared<Resource>(rPair->second);
		loadedMap_.emplace(src, ptrRes);
	}

	return ptrRes;
}

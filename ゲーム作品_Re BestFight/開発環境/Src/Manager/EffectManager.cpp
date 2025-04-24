#include<string>
#include <DxLib.h>
#include<EffekseerForDxLib.h>
#include "EffectManager.h"
#include "Effect.h"

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EffectManager();
	}
	instance_->Init();
}

EffectManager& EffectManager::GetInstance(void)
{
	return *instance_;
}

void EffectManager::Init(void)
{
	resourcesMap_.clear();
	loadedMap_.clear();

	Effect res;
	const std::string PATH_EFFECT = "Data/Effect/";


	//カーソルエフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "CarsolEff.efkefc");
	resourcesMap_.emplace(EFF::CARSOL_EFF, res);
	
	//ジャンプの土煙
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Smoke.efkefc");
	resourcesMap_.emplace(EFF::JUMP, res);

	//空振りエフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "swing.efkefc");
	resourcesMap_.emplace(EFF::SWING, res);

	//強エフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "week_attack_1.efkefc");
	resourcesMap_.emplace(EFF::STRONG, res);

	//攻撃風
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "SonicBoom.efkefc");
	resourcesMap_.emplace(EFF::ATTACK_WIND, res);

	//フラッシュ(通常攻撃時)
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Flash.efkefc");
	resourcesMap_.emplace(EFF::FLASH, res);

	//斬撃エフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Sord.efkefc");
	resourcesMap_.emplace(EFF::SORD, res);

	//落下攻撃エフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "fall_attack.efkefc");
	resourcesMap_.emplace(EFF::FALL_ATTACK, res);

	//バリアエフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "baria.efkefc");
	resourcesMap_.emplace(EFF::BARIA, res);

	//高速移動エフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "quick_move.efkefc");
	resourcesMap_.emplace(EFF::QUICK_MOVE, res);

	//ジャストエフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "just.efkefc");
	resourcesMap_.emplace(EFF::JUST, res);

	//必殺技MAXエフェクト
	res = Effect(Effect::TYPE::EFFKSEER2D, PATH_EFFECT + "fire_flower.efkefc");
	resourcesMap_.emplace(EFF::SKILL_MAX, res);

	//必殺フレイム
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Flame_2.efkefc");
	resourcesMap_.emplace(EFF::SKILL_FLAME, res);

	//ソード必殺ビーム
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "SordSkill.efkefc");
	resourcesMap_.emplace(EFF::SKILL_SORD, res);

	//落雷
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Lightning_2.efkefc");
	resourcesMap_.emplace(EFF::LIGHTNING, res);

	//炎レーザー
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_fire.efkefc");
	resourcesMap_.emplace(EFF::LASER_FIRE, res);

	//ブラー効果
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Blur.efkefc");
	resourcesMap_.emplace(EFF::BLUR, res);

	//戦闘開始エフェクト
	res = Effect(Effect::TYPE::EFFKSEER2D, PATH_EFFECT + "FireCircle.efkefc");
	resourcesMap_.emplace(EFF::FIRE_CIRCLE, res);

	//魔術レーザー
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_magic.efkefc");
	resourcesMap_.emplace(EFF::LASER_MAGIC, res);

	//魔術レーザー(乱)
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_violence.efkefc");
	resourcesMap_.emplace(EFF::LASER_VIOLENCE, res);

	//ワープオーラキューブ
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Aura.efkefc");
	resourcesMap_.emplace(EFF::AURA, res);

	//ビッグバン
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "BigBang.efkefc");
	resourcesMap_.emplace(EFF::BIG_BANG, res);

	//竜巻
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Trnade.efkefc");
	resourcesMap_.emplace(EFF::TRNADE, res);

	//幻影エフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Phantasm.efkefc");
	resourcesMap_.emplace(EFF::FHANTASM, res);

	//狙撃魔導陣
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Sogeki.efkefc");
	resourcesMap_.emplace(EFF::SOGEKI, res);

	//隕石
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Meteo_2.efkefc");
	resourcesMap_.emplace(EFF::METEO, res);

	//隕石魔導陣
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "mahoujin.efkefc");
	resourcesMap_.emplace(EFF::MAHOUJIN, res);

	//ガードブレイクエフェクト
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "GurdBreak.efkefc");
	resourcesMap_.emplace(EFF::GURD_BREAK, res);
}

void EffectManager::Update(void)
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	//エフェクトの更新
	UpdateEffekseer3D();
	UpdateEffekseer2D();
}

void EffectManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		for (auto& v : p.second)
		{
			v->Release();
		}
	}

	loadedMap_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

std::weak_ptr<Effect> EffectManager::Load(const EFF& eff)
{
	//ロードに失敗した場合空のポインタを返す
	std::weak_ptr<Effect> res = _Load(eff);
	return res;
}

EffectManager::EffectManager()
{

}

std::weak_ptr<Effect> EffectManager::_Load(const EFF& eff)
{
	const auto& loite = loadedMap_.find(eff);
	std::shared_ptr<Effect> ptrEff = nullptr;//空のベクトル配列
	//すでにロードされている場合
	if (loite != loadedMap_.end())
	{
		//二重ロード
		ptrEff = std::make_shared<Effect>(resourcesMap_.at(eff));
		loadedMap_.at(eff).push_back(ptrEff);
		//配列の最後のポインタを返す
		return loadedMap_.at(eff)[loadedMap_.at(eff).size() - 1];
	}

	const auto& reite = resourcesMap_.find(eff);
	//リソースがある
	//新しくロードする
	if (reite != resourcesMap_.end())
	{
		reite->second.Load();

		ptrEff = std::make_shared<Effect>(reite->second);

		std::vector<std::shared_ptr<Effect>>vecEff;
		vecEff.push_back(ptrEff);
		loadedMap_.emplace(eff, vecEff);
	}

	return ptrEff;
}

void EffectManager::Draw2DEffect(void)
{
	//エフェクトの描画
	DrawEffekseer2D();
}

void EffectManager::Draw3DEffect(void)
{
	//エフェクトの描画
	DrawEffekseer3D();
}

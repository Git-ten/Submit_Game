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


	//�J�[�\���G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "CarsolEff.efkefc");
	resourcesMap_.emplace(EFF::CARSOL_EFF, res);
	
	//�W�����v�̓y��
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Smoke.efkefc");
	resourcesMap_.emplace(EFF::JUMP, res);

	//��U��G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "swing.efkefc");
	resourcesMap_.emplace(EFF::SWING, res);

	//���G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "week_attack_1.efkefc");
	resourcesMap_.emplace(EFF::STRONG, res);

	//�U����
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "SonicBoom.efkefc");
	resourcesMap_.emplace(EFF::ATTACK_WIND, res);

	//�t���b�V��(�ʏ�U����)
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Flash.efkefc");
	resourcesMap_.emplace(EFF::FLASH, res);

	//�a���G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Sord.efkefc");
	resourcesMap_.emplace(EFF::SORD, res);

	//�����U���G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "fall_attack.efkefc");
	resourcesMap_.emplace(EFF::FALL_ATTACK, res);

	//�o���A�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "baria.efkefc");
	resourcesMap_.emplace(EFF::BARIA, res);

	//�����ړ��G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "quick_move.efkefc");
	resourcesMap_.emplace(EFF::QUICK_MOVE, res);

	//�W���X�g�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "just.efkefc");
	resourcesMap_.emplace(EFF::JUST, res);

	//�K�E�ZMAX�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER2D, PATH_EFFECT + "fire_flower.efkefc");
	resourcesMap_.emplace(EFF::SKILL_MAX, res);

	//�K�E�t���C��
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Flame_2.efkefc");
	resourcesMap_.emplace(EFF::SKILL_FLAME, res);

	//�\�[�h�K�E�r�[��
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "SordSkill.efkefc");
	resourcesMap_.emplace(EFF::SKILL_SORD, res);

	//����
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Lightning_2.efkefc");
	resourcesMap_.emplace(EFF::LIGHTNING, res);

	//�����[�U�[
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_fire.efkefc");
	resourcesMap_.emplace(EFF::LASER_FIRE, res);

	//�u���[����
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Blur.efkefc");
	resourcesMap_.emplace(EFF::BLUR, res);

	//�퓬�J�n�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER2D, PATH_EFFECT + "FireCircle.efkefc");
	resourcesMap_.emplace(EFF::FIRE_CIRCLE, res);

	//���p���[�U�[
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_magic.efkefc");
	resourcesMap_.emplace(EFF::LASER_MAGIC, res);

	//���p���[�U�[(��)
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Laser_violence.efkefc");
	resourcesMap_.emplace(EFF::LASER_VIOLENCE, res);

	//���[�v�I�[���L���[�u
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Aura.efkefc");
	resourcesMap_.emplace(EFF::AURA, res);

	//�r�b�O�o��
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "BigBang.efkefc");
	resourcesMap_.emplace(EFF::BIG_BANG, res);

	//����
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Trnade.efkefc");
	resourcesMap_.emplace(EFF::TRNADE, res);

	//���e�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Phantasm.efkefc");
	resourcesMap_.emplace(EFF::FHANTASM, res);

	//�_�������w
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Sogeki.efkefc");
	resourcesMap_.emplace(EFF::SOGEKI, res);

	//覐�
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "Meteo_2.efkefc");
	resourcesMap_.emplace(EFF::METEO, res);

	//覐Ζ����w
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "mahoujin.efkefc");
	resourcesMap_.emplace(EFF::MAHOUJIN, res);

	//�K�[�h�u���C�N�G�t�F�N�g
	res = Effect(Effect::TYPE::EFFKSEER3D, PATH_EFFECT + "GurdBreak.efkefc");
	resourcesMap_.emplace(EFF::GURD_BREAK, res);
}

void EffectManager::Update(void)
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	//�G�t�F�N�g�̍X�V
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
	//���[�h�Ɏ��s�����ꍇ��̃|�C���^��Ԃ�
	std::weak_ptr<Effect> res = _Load(eff);
	return res;
}

EffectManager::EffectManager()
{

}

std::weak_ptr<Effect> EffectManager::_Load(const EFF& eff)
{
	const auto& loite = loadedMap_.find(eff);
	std::shared_ptr<Effect> ptrEff = nullptr;//��̃x�N�g���z��
	//���łɃ��[�h����Ă���ꍇ
	if (loite != loadedMap_.end())
	{
		//��d���[�h
		ptrEff = std::make_shared<Effect>(resourcesMap_.at(eff));
		loadedMap_.at(eff).push_back(ptrEff);
		//�z��̍Ō�̃|�C���^��Ԃ�
		return loadedMap_.at(eff)[loadedMap_.at(eff).size() - 1];
	}

	const auto& reite = resourcesMap_.find(eff);
	//���\�[�X������
	//�V�������[�h����
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
	//�G�t�F�N�g�̕`��
	DrawEffekseer2D();
}

void EffectManager::Draw3DEffect(void)
{
	//�G�t�F�N�g�̕`��
	DrawEffekseer3D();
}

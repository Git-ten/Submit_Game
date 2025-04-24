#include <chrono>
#include <DxLib.h>
#include<EffekseerForDxLib.h>
#include"Event/Fade.h"
#include "../Scene/TitleScene.h"
#include"../Scene/GameScene.h"
#include"../Scene/SelectScene.h"
#include"../Scene/ResultScene.h"
#include"../Scene/DemoScene.h"
#include"../Scene/AboutGameScene.h"
#include"../Scene/Camera.h"
#include"../Application.h"
#include"ResourceManager.h"
#include"EffectManager.h"
#include"SoundManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//���C���X�N���[��
	Size2D size = Application::GetInstance().GetScreenSize();
	mainScreen_ = MakeScreen(size.x, size.y);

	//�^�C�g���V�[��ID
	sceneId_ = SCENE_ID::TITLE;

	//�V�[���C���X�^���X
	scene_ = nullptr;

	fader_ = std::make_unique<Fade>();

	//�V�[����ύX�����ۂ�
	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	//�v���C���[
	playerID_ = CHARA_ID::NONE;
	//�G
	enemyID_ = CHARA_ID::NONE;

	//CPU�Ɛ키��
	cpuFlag_ = false;

	//�o�C�u���[�V������
	vibrationCnt_ = 0;

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Init3D(void)
{
	// �w�i�F�ݒ�
	SetBackgroundColor(250, 153, 153);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	//// ���C�g�̐ݒ�
	SetUseLighting(true);

	// �^�ォ�牺�Ɍ����������C�g
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	// �t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 130000.0f);
}

void SceneManager::Update(void)
{
	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	if (!isSceneChanging_)
	{
		//�V�[���̍X�V
		scene_->Update();
	}
	else
	{
		//�t�F�C�h
		FadeManual();
	}
}

void SceneManager::Draw(void)
{
	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)

	//����ʂɕ`��
	SetDrawScreen(mainScreen_);
	// ��ʂ��N���A
	ClearDrawScreen();

	//�`��
	scene_->Draw();
	fader_->Draw();

	DrawMainScreen();
}

void SceneManager::Destroy(void)
{
	scene_->Release();
	delete instance_;
}

void SceneManager::ChangeScene(const SCENE_ID& nextId)
{
	isSceneChanging_ = true;
	sceneId_ = nextId;
	fader_->FadeStart(Fade::FADE_STATE::FADE_OUT);
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)const
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

void SceneManager::SetPlayerAndEnemy(CHARA_ID player, CHARA_ID enemy)
{
	//�v���C���[
	playerID_ = player;
	//�G
	enemyID_ = enemy;
}

void SceneManager::SetCpuFlag(bool flg)
{
	cpuFlag_ = flg;
}

bool SceneManager::GetCpuFlag(void) const
{
	return cpuFlag_;
}

int SceneManager::GetScreen(void) const
{
	return mainScreen_;
}

void SceneManager::SetVibrationCnt(int count)
{
	vibrationCnt_ = count;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;
}

SceneManager::~SceneManager(void)
{
	DeleteGraph(mainScreen_);
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{
	// ���\�[�X�̉��
	// ��U�A���ׂẴ��\�[�X���J������
	if (sceneId != SCENE_ID::RESULT)
	{
		ResourceManager::GetInstance().Release();
		EffectManager::GetInstance().Release();
		SoundManager::GetInstance().Release();
	}
	if (scene_ != nullptr)
	{
		scene_->Release();
	}

	// �V�[����ύX����
	sceneId_ = sceneId;

	//�ύX��̃V�[���̃C���X�^���X�𐶐�
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>(playerID_, enemyID_);
		break;
	case SCENE_ID::SELECT:
		scene_ = std::make_unique<SelectScene>(cpuFlag_);
		break;
	case SCENE_ID::RESULT:
		scene_ = std::make_unique<ResultScene>(playerID_, enemyID_);
		break;
	case SCENE_ID::DEMO:
	{
		CHARA_ID chara_1 = CHARA_ID::NONE;
		CHARA_ID chara_2 = CHARA_ID::NONE;
		RandomChara(&chara_1, &chara_2);
		scene_ = std::make_unique<DemoScene>(chara_1, chara_2);
		break;
	}
	case SCENE_ID::ABOUT:
		scene_ = std::make_unique<AboutGameScene>(AboutGameScene::MODE::TITLE_ABOUT_GAME);
		break;
	default:
		break;
	}

	//�V�[���̏�����
	scene_->Init();

	//�f���^�^�C�����Z�b�g
	ResetDeltaTime();
}

void SceneManager::FadeManual(void)
{
	fader_->Update();
	if (!fader_->IsFadeFlag() && !fader_->IsEndFlag())
	{
		fader_->FadeStart(Fade::FADE_STATE::FADE_IN);
		fader_->SetIsEndFlag(true);
		DoChangeScene(sceneId_);
	}
	else if (!fader_->IsFadeFlag() && fader_->IsEndFlag())
	{
		isSceneChanging_ = false;
	}
}

void SceneManager::RandomChara(CHARA_ID* chara_1, CHARA_ID* chara_2)
{
	std::vector<CHARA_ID>charaIds;
	charaIds.push_back(CHARA_ID::FIGHTER_ID);
	charaIds.push_back(CHARA_ID::SORDMAN_ID);
	charaIds.push_back(CHARA_ID::MAGICIAN_ID);


	int random = GetRand(2);

	SRand((unsigned int)time(NULL));

	*chara_1 = charaIds[random];
	charaIds.erase(charaIds.begin() + random);

	random = GetRand(1);
	*chara_2 = charaIds[random];

	charaIds.clear();
}

void SceneManager::DrawMainScreen(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	//�`��ΏۃX�N���[�����؂�ւ��J���������Z�b�g���ꂽ����
	//�J�����������X�V
	if (scene_->GetCamera().lock() != nullptr)
	{
		scene_->GetCamera().lock()->CompCameraInfoUpdate();
	}
	DrawGraph(0, 0, mainScreen_, false);


	//�o�C�u���[�V��������
	if ((vibrationCnt_ % 2) != 0)
	{
		DrawGraph(4, 0, mainScreen_, false);
	}
	vibrationCnt_--;
	if (vibrationCnt_ < 0)vibrationCnt_ = 0;
}
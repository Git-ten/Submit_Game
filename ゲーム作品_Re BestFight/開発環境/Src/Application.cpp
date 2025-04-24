#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include"Manager/EffectManager.h"
#include"Manager/SoundManager.h"
#include"Debuger.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

//#define WINDOW_MODE (1)

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	//�Q�[���I���t���O
	isGameEnd_ = false;

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("Re:Best Fight");

	//�ݒ肵���E�B���h�E���[�h��ݒ�
	//true = �E�B���h�E���[�h,false = �t���X�N���[�����[�h
#ifdef WINDOW_MODE
	windowMode_ = true;
#else
	windowMode_ = false;
#endif


	if (windowMode_)
	{
		// �E�B���h�E�T�C�Y
		SetGraphMode(WINDOW_SCREEN_SIZE_X, WINDOW_SCREEN_SIZE_Y, 32);
		screenSize_ = { WINDOW_SCREEN_SIZE_X , WINDOW_SCREEN_SIZE_Y };
		//�}�b�v�̑傫�����i�[
		mapSize_ = {MAP_WSIZE_X, MAP_WSIZE_Y};
	}
	else
	{
		// �E�B���h�E�T�C�Y
		SetGraphMode(FULL_SCREEN_SIZE_X, FULL_SCREEN_SIZE_Y, 32);
		screenSize_ = { FULL_SCREEN_SIZE_X , FULL_SCREEN_SIZE_Y };
		//�}�b�v�̑傫�����i�[
		mapSize_ = { MAP_FSIZE_X, MAP_FSIZE_Y };
	}

	//true�ŃE�B���h�E���[�h�Afalse�Ńt���X�N���[�����[�h
	ChangeWindowMode(windowMode_);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}
	//UD Digi Kyokasho NK - B Bold
	ChangeFont("UD �f�W�^�� ���ȏ��� N-B");

	//�}�E�X�̕\���ݒ�
	mouseFlag_ = true;
	SetMouseDispFlag(true);

	// �L�[���䏉����
	SetUseDirectInputFlag(true);

	//�G�t�F�N�g�V�[�J�[�̏�����
	InitEffekseer();

	//�f�o�b�O�N���X
	Debuger::CreateInstance();

	//���͊Ǘ�������
	InputManager::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	//�G�t�F�N�g�Ǘ�������
	EffectManager::CreateInstance();

	//�T�E���h�Ǘ�������
	SoundManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

}

//�G�t�F�N�g�̏�����
void Application::InitEffekseer()
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	Size2D size = GetScreenSize();
	Effekseer_Set2DSetting(size.x, size.y);
//	Effekseer_InitDistortion();
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& debuger = Debuger::GetInstance();

	// �Q�[�����[�v
	// && !CheckHitKey(KEY_INPUT_ESCAPE)
	while (ProcessMessage() == 0 && !isGameEnd_)
	{

		inputManager.Update();
		if (GetJoypadInputState(DX_INPUT_PAD1) ||
			GetJoypadInputState(DX_INPUT_PAD2))
		{
			//�}�E�X�̕\���ݒ�
			SetMouseFlag(false);
			SetMousePoint(screenSize_.x / 2, screenSize_.y / 5);

		}
		else if (CheckHitKeyAll())
		{
			//�}�E�X�̕\���ݒ�
			SetMouseFlag(true);
		}

		sceneManager.Update();

		debuger.Update();

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{
	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	Debuger::GetInstance().Destroy();
	
	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;
}

//���\�[�X�̊J��
void Application::Release(void)
{

	// Effekseer���I������B
	Effkseer_End();

	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

}

//�X�N���[���̃T�C�Y���擾
Size2D Application::GetScreenSize() const
{
	return screenSize_;
}

//�}�b�v�̃T�C�Y���擾
Size2D Application::GetMapSize()const
{
	return mapSize_;
}

//���݂̃E�B���h�E���[�h���擾
bool Application::IsWidowMode()const
{
	return windowMode_;
}

void Application::SetMouseFlag(bool flg)
{
	mouseFlag_ = flg;
	SetMouseDispFlag(mouseFlag_);
}

bool Application::IsMouseFlag(void) const
{
	return mouseFlag_;
}

void Application::EndGame(void)
{
	isGameEnd_ = true;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	windowMode_ = true;
	isGameEnd_ = false;
	mapSize_ = Size2D();
	mouseFlag_ = false;
	screenSize_ = Size2D();
}
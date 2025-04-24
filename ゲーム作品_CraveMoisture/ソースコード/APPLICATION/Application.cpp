#include <cassert>
#include <EffekseerForDxLib.h>
#include "Singleton_Manager/SingletonManagerFactory.h"
#include "../Utility/Utility.h"
#include "../Common/DataLoad/DataWindows.h"
#include "../Common/DataLoad/LoadDataPaths.h"
#include "../Common/DataLoad/LoadDataWindows.h"
#include "ApplicationRun.h"
#include "Application.h"

Application* Application::instance_ = nullptr;
DataPaths Application::dataPaths_ = LoadDataPaths::LoadedDataPaths();

#pragma region �萔�錾

//�J���[�r�b�g��(16bit Or 32bit)
const int COLOR_BIT_NUM = 32;

//��ʂɕ\���ł���G�t�F�N�g�̍ő�p�[�e�B�N����
const int EFFECT_PARTICLE_MAX = 9200;

// ��O�q
const int EXCEPTION_VALUE = -1;

#pragma endregion

void Application::CreateInstance(void)
{
	assert(instance_ == nullptr || !"Application::CreateInstance�ŃC���X�^���X���쐬����Ă���̂ɍĂѐ������悤�Ƃ��Ă��܂�");

	instance_ = new Application();
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	// �X�N���[���X�e�[�^�X�̃��[�h
	LoadScreenStatus();

	//�E�B���h�E�̏�����
	InitWindow();

	// DxLib�̏�����
	InitDxLib();

	// �X�N���[���̃Z�b�g�A�b�v
	SetUpScreenStatus();

	//�G�t�F�N�g�V�[�J�[�̏�����
	InitEffekseer();

	//�V���O���g���C���X�^���X�𐶐�
	CreateSingleton();
}

void Application::InitWindow(void)
{
	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("Crave Moisture");

	// �f�X�N�g�b�v�̑傫��
	screenStatus_.desctopSize = GetDesktopSize();

	if (screenStatus_.isWidowMode)
	{
		SetGraphMode(screenStatus_.screenSize.x, screenStatus_.screenSize.y, COLOR_BIT_NUM);
	}
	else
	{
		SetGraphMode(screenStatus_.desctopSize.x, screenStatus_.desctopSize.y, COLOR_BIT_NUM);
	}

	// true�ŃE�B���h�E���[�h�Afalse�Ńt���X�N���[�����[�h
	ChangeWindowMode(screenStatus_.isWidowMode);

	// �X�N���[���̃J���[
	SetBackgroundColor(screenStatus_.screenColor.x, screenStatus_.screenColor.y, screenStatus_.screenColor.z);

}

void Application::InitDxLib(void)
{
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	assert(DxLib_Init() != EXCEPTION_VALUE || !"DxLib�̏��������s");

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
}

void Application::LoadScreenStatus(void)
{
	// �E�B���h�E�̏������[�h
	const DataWindows& dataWindows = LoadDataWindows::LoadedDataWindows(dataPaths_);

	// �X�N���[���̃J���[
	Color3D screenColor = dataWindows.screenColor;

	// �t���X�N���[�����Ɉ�a���Ȃ����W���擾���邽�߂̃p�����[�^
	Size2DF diffrence = Utility::VECTOR2D_ONE;
	Size2DF reDiffrence = Utility::VECTOR2D_ONE;

	// ���[�h����������ɃZ�b�e�B���O
	// ��ʂ̒��S
	const Size2D& screenCenter = Utility::GetHalfVector2(dataWindows.screenSize);

	//�X�N���[���̃X�e�[�^�X���Z�b�g
	screenStatus_ = { dataWindows.screenSize,
					  screenColor,
					  screenCenter,
					  Size2D(),
					  EXCEPTION_VALUE,
					  dataWindows.frameFps,
					  dataWindows.isWindowMode,
					  dataWindows.isMouseShow,
					  diffrence,
					  reDiffrence };
}

void Application::SetUpScreenStatus(void)
{
	// ���C���X�N���[���̍쐬
	screenStatus_.mainScreenId = MakeScreen(screenStatus_.screenSize.x, screenStatus_.screenSize.y);

	// �t���X�N���[�����Ɉ�a���Ȃ����W���擾���邽�߂̃p�����[�^
	if (!screenStatus_.isWidowMode)
	{
		screenStatus_.diffrence = Vector2F::ToVector2F(screenStatus_.desctopSize) /
			Vector2F::ToVector2F(screenStatus_.screenSize);

		screenStatus_.reDiffrence = Vector2F::ToVector2F(screenStatus_.screenSize) /
			Vector2F::ToVector2F(screenStatus_.desctopSize);
	}


	// �}�E�X�̕\���ݒ�
	SetMouseDispFlag(screenStatus_.isMouseShow);
}

//�G�t�F�N�g�̏�����
void Application::InitEffekseer(void)
{
	if (Effekseer_Init(EFFECT_PARTICLE_MAX) == EXCEPTION_VALUE)
	{
		DxLib_End();
	}

	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// �X�N���[���̑傫��
	// Size2D size = screenStatus_.screenSize;
	// Effekseer_Set2DSetting(size.x, size.y);
	// Effekseer_InitDistortion();
}

void Application::SetScreenStatus(void)
{

}

const Size2D Application::GetDesktopSize(void)
{
	RECT Wsize;
	GetWindowRect(GetDesktopWindow(), &Wsize);
	Size2D size = { Wsize.right - Wsize.left, Wsize.bottom - Wsize.top };

	return size;
}

void Application::CreateSingleton(void)
{
	SINGLETON_MANAGER_FACTORY(INPUT_MANAGER);
	SINGLETON_MANAGER_FACTORY(RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(EFFECT_RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(SOUND_RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(SCENE_MANAGER);
}

void Application::Run(void)
{
	ApplicationRun::Running(screenStatus_);
}

const DataPaths& Application::GetDataPaths(void) const
{
	return dataPaths_;
}

const int Application::GetMainScreen(void) const
{
	return screenStatus_.mainScreenId;
}

const Size2D& Application::GetScreenSize(void) const
{
	return screenStatus_.screenSize;
}

const Position2D& Application::GetScreenCenter(void) const
{
	return screenStatus_.screenCenter;
}

const Size2D& Application::GetScreenDesktopSize(void)
{
	return screenStatus_.desctopSize;
}

const Size2DF& Application::GetReDeffrenceSize(void) const
{
	return screenStatus_.reDiffrence;
}

const Size2DF& Application::GetDeffrenceSize(void) const
{
	return screenStatus_.diffrence;
}

const bool Application::IsWindowMode(void) const
{
	return screenStatus_.isWidowMode;
}

void Application::DefaultMouseDisp(void)
{
	SetMouseDispFlag(screenStatus_.isMouseShow);
}

void Application::Destroy(void)
{
	// �V���O���g���C���X�^���X�����ׂč폜
	SingletonManagerFactory::DestroyInstance();

#ifdef DEBUG
	Debuger::GetInstance().Destroy();
#endif

	// DxLib�I��
	if (DxLib_End() == EXCEPTION_VALUE)assert(!"DxLib�̏I���Ɏ��s");

	delete instance_;
}

//���\�[�X�̊J��
void Application::Release(void)
{

	// Effekseer���I������B
	Effkseer_End();

	// DxLib�I��
	if (DxLib_End() == EXCEPTION_VALUE)assert(!"DxLib�̏I���Ɏ��s");

}

Application::Application(void) :
	screenStatus_(ScreenStatus())
{
}
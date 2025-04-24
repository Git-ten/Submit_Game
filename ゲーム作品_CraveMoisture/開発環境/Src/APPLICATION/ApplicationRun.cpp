#include <cassert>
#include <DxLib.h>
#include "Singleton_Manager/SceneManager/SceneManager.h"
#include "Singleton_Manager/InputManager/InputManager.h"
#include "Singleton_Manager/EffectResourceManager/EffectResourceManager.h"
#include "ApplicationRun.h"

//#define DRAW_FRAME_RATE
#pragma region �萔�錾

// ���C���X�N���[���̍�����W
const Position2D MAIN_SCREEN_LEFT_POS = { 0, 0 };

#pragma endregion

void ApplicationRun::Running(const Application::ScreenStatus& status)
{
	auto& sceneManager = SceneManager::GetInstance();
	auto& inputManager = InputManager::GetInstance();
	bool isGameEnd = false;

	// �t���[�����[�g�v���ϐ���������
	float prevFrameTime = static_cast<float>(GetNowCount());
	float nowFrameTime = prevFrameTime;

	// �~���P�ʂɂ���
	float frameRate = (1.0f / status.screenFps) * 1000.0f;

	while (ProcessMessage() == 0 && !isGameEnd)
	{
		inputManager.Update();
		sceneManager.Update();

		// �`���O���t�B�b�N�̈�̎w��
		//(�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
		SetDrawScreen(status.mainScreenId);
		// ��ʂ��N���A
		ClearDrawScreen();
		sceneManager.Draw();

#ifdef DRAW_FRAME_RATE
		//�t���[�����[�g��`��
		const int screenSizeX = Application::GetInstance().GetScreenSize().x;
		DrawFormatString(screenSizeX - 90, 0, 0xff0000, "FPS[%.1f]", GetFPS());
#endif

		//����ʂɕ`��
		SetDrawScreen(DX_SCREEN_BACK);
		// ��ʂ��N���A
		ClearDrawScreen();
		// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɐݒ�
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		if (status.isWidowMode)
		{
			DrawGraph(MAIN_SCREEN_LEFT_POS.x, MAIN_SCREEN_LEFT_POS.y, status.mainScreenId, true);
		}
		else
		{
			DrawExtendGraph(MAIN_SCREEN_LEFT_POS.x, MAIN_SCREEN_LEFT_POS.y, status.desctopSize.x, status.desctopSize.y, status.mainScreenId, true);
		}
		// �`�惂�[�h�����ɖ߂�
		SetDrawMode(DX_DRAWMODE_NEAREST);


		// ���[�v�ɂ����������Ԃ��v��
		nowFrameTime = static_cast<float>(GetNowCount());

		// �t���[�����[�g��frameRate_�ɋ����I�ɍ��킹��
		// �ݒ肳��Ă���t���[�����[�g(frameRate_)���i��ł��Ȃ��ꍇ
		const float rate = prevFrameTime + frameRate;
		if (nowFrameTime < rate)
		{
			//���ԍ����X���[�v����
			Sleep(static_cast<DWORD>(rate - nowFrameTime));
		}
		prevFrameTime = nowFrameTime;


		ScreenFlip();

		EndGame(isGameEnd);
	}
}

void ApplicationRun::EndGame(bool& flg)
{
	if (SceneManager::GetInstance().IsEndGame())flg = true;
}
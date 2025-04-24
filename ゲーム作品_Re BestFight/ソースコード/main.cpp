#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <DxLib.h>
#include "Application.h"

// WinMain�֐�
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �C���X�^���X�̐���
	Application::CreateInstance();

	// �C���X�^���X�̎擾
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// ���������s
		OutputDebugString("�C���X�^���X�̏��������s");
		return -1;
	}

	// ���s
	instance.Run();

	//���\�[�X�̊J��
	instance.Release();

	if (instance.IsReleaseFail())
	{
		//���\�[�X�J�����s
		OutputDebugString("���\�[�X�̊J���Ɏ��s���܂���");
		return -1;
	}

	// ���
	instance.Destroy();

	return 0;
}

//����---------------------
// 
// �|�X�g�G�t�F�N�g������
// 
// �X�e�[�W����
//		�Ⴆ�΁A�ȒP�ȃe�N�X�`���Ŋϋq�����
//		�X�g�[���[��
// 
// �������łȂ����Ńt�@�C�g�Ƃ���
//		�t���[�f�ނŌ����Ă���
//		�킢�̎n�܂���A���؂ɂ���
//		
// 
// �X�N���[���̑傫��
//		GetWindowRect(GetDesktopWindow(), &rc);
// 
// 
//-------------------------
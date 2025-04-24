#pragma once
#include <memory>
#include <string>
#include "../Common/DataLoad/DataPaths.h"
#include "../Common/DataLoad/DataWindows.h"
#include "../Utility/VectorHeader.h"

//�V���O���g���p�^�[��
class ApplicationRun;

class Application
{
public:

	// �X�N���[���̃X�e�[�^�X
	// �O�����烍�[�h����
	struct ScreenStatus
	{
		Size2D screenSize;
		Color3D screenColor;
		Size2D screenCenter;
		Size2D desctopSize;
		int mainScreenId;
		float screenFps;
		bool isWidowMode;
		bool isMouseShow;
		Size2DF diffrence;
		Size2DF reDiffrence;
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);
	// ���\�[�X�̉��
	void Release(void);
	// ���\�[�X�̔j��
	void Destroy(void);

	// ���s
	void Run(void);

	// �f�[�^�p�X�̎擾
	const DataPaths& GetDataPaths(void)const;

	// ���C���X�N���[��ID�̎擾
	const int GetMainScreen(void)const;

	// �X�N���[���̃T�C�Y���擾
	const Size2D& GetScreenSize(void)const;

	// �X�N���[���̒��S���W���擾
	const Position2D& GetScreenCenter(void)const;

	// �f�X�N�g�b�v�̃T�C�Y���擾
	const Size2D& GetScreenDesktopSize(void);

	// �X�N���[���̓f�X�N�g�b�v�̉��{��(�S��ʕ\���̎��̂ݗL��)
	const Size2DF& GetReDeffrenceSize(void)const;

	// �f�X�N�g�b�v�̓X�N���[���̉��{��(�S��ʕ\���̎��̂ݗL��)
	const Size2DF& GetDeffrenceSize(void)const;

	// �E�B���h�E���[�h���擾
	const bool IsWindowMode(void)const;

	// �}�E�X�̕\����Ԃ������ݒ�ɂ���
	void DefaultMouseDisp(void);

private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	// �X�N���[���̃X�e�[�^�X
	ScreenStatus screenStatus_;

	// �f�[�^�p�X
	static DataPaths dataPaths_;



	// private�ŃR���X�g���N�^�𐶐�����
	Application(void);
	// �R�s�[�R���X�g���N�^�����l
	Application(const Application&);
	//�f�X�g���N�^
	~Application(void) = default;

	// �E�B���h�E�̏�����
	void InitWindow(void);
	// DxLib�̏�����
	void InitDxLib(void);

	//�X�N���[���X�e�[�^�X�̃��[�h
	void LoadScreenStatus(void);
	// �X�N���[���̃Z�b�g�A�b�v
	void SetUpScreenStatus(void);
	// �X�N���[���̃X�e�[�^�X���Z�b�e�B���O
	void SetScreenStatus(void);

	//�G�t�F�N�V�A�̏�����
	void InitEffekseer(void);
	// �f�X�N�g�b�v�̃T�C�Y���擾
	const Size2D GetDesktopSize(void);

	//�V���O���g���C���X�^���X�𐶐�
	void CreateSingleton(void);

};
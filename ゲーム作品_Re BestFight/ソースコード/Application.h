#pragma once
#include <string>
#include"Utility/VectorHeader.h"

class Application
{
public:

	// �X�N���[���T�C�Y
	static constexpr int WINDOW_SCREEN_SIZE_X = 1024;
	static constexpr int WINDOW_SCREEN_SIZE_Y = 640;
	static constexpr int FULL_SCREEN_SIZE_X = 1920;
	static constexpr int FULL_SCREEN_SIZE_Y = 1080;

	//�}�b�v���W�̃T�C�Y
	static constexpr int MAP_WSIZE_X = 64;
	static constexpr int MAP_WSIZE_Y = 64;
	static constexpr int MAP_FSIZE_X = 120;
	static constexpr int MAP_FSIZE_Y = 102;

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);
	void InitEffekseer(void);
	// ���\�[�X�̉��
	void Release(void);
	// ���\�[�X�̔j��
	void Destroy(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

	//�X�N���[���̃T�C�Y���擾
	Size2D GetScreenSize(void)const;
	//�}�b�v�̃T�C�Y���擾
	Size2D GetMapSize(void)const;

	//���݂̃E�B���h�E���[�h���擾
	bool IsWidowMode(void)const;
	//�}�E�X�̕\����Ԃ��Z�b�g
	void SetMouseFlag(bool flg);
	//�}�E�X�̕\����Ԃ��擾
	bool IsMouseFlag(void)const;

	//�Q�[���̏I��
	void EndGame(void);

private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	//�Q�[���I���t���O
	bool isGameEnd_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	//�X�N���[���̑傫�����i�[
	Size2D screenSize_;
	//�}�b�v�̑傫�����i�[
	Size2D mapSize_;

	//�ݒ肵���E�B���h�E���[�h���i�[
	//true = �E�B���h�E���[�h,false = �t���X�N���[�����[�h
	bool windowMode_;
	//�}�E�X��\�����邩
	bool mouseFlag_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);

	// �R�s�[�R���X�g���N�^�����l
	Application(const Application&);

	// �f�X�g���N�^�����l
	~Application(void) = default;

};
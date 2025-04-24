#pragma once
#include <map>
#include <functional>
#include <string>

class SimpleInput
{
public:

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SimpleInput& GetInstance(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ������s���L�[��ǉ�
	void Add(const std::string& keyName, const std::function<bool(void)> keyFunc);

	// �w�肵�����̂̃L�[�̓��͏�Ԃ��擾
	const bool GetInput(const std::string& keyName)const;

private:

	using SIMPLE_KEY = std::map<std::string, std::function<bool(void)>>;

private:

	// �V���O���g���p�C���X�^���X
	static SimpleInput* instance_;

	// �o�^�L�[
	SIMPLE_KEY input_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SimpleInput(void);
	// �R�s�[�R���X�g���N�^�����l
	SimpleInput(const SimpleInput&);
	//�f�X�g���N�^
	~SimpleInput(void) = default;

};
#pragma once
#include <chrono>

class DeltaTime
{
public:

	// �R���X�g���N�^
	DeltaTime(void);
	// �f�X�g���N�^
	~DeltaTime(void);

	// ���ݎ��Ԃ��X�V
	// �X�V�O�ƌ��݂�CPU���ԂƂō����Ƃ�
	void UpdateTime(void);

	// ���ݎ��Ԃ��擾
	const float GetNowTime(void)const;

private:

	// �f���^�^�C��(�X�V�O)
	std::chrono::system_clock::time_point prevTime_;
	// �f���^�^�C��(�X�V��)
	float nowTime_;
};


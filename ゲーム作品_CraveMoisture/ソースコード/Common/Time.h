#pragma once
#include <chrono>

class Time
{
public:

	// �R���X�g���N�^
	Time(void);
	// �f�X�g���N�^
	~Time(void);

	// �����Ԃ�����������
	void InitTotalNowTime(void);

	// �����Ԃ������I�Ɉ����̒l�ɂ���
	void SetTotalNowTime(const float time);

	// ���ݎ��Ԃ��X�V
	// �X�V�O�ƌ��݂�CPU���ԂƂō����Ƃ�
	void UpdateTime(void);
	void UpdateTime(const float speed);

	// �����Ԃ��擾
	const float GetTotalNowTime(void)const;
	// �����Ԃ̎Q�Ƃ��擾
	float& GetTotalNowTimeReference(void);

private:

	// ���^�C��
	float totalNowTime_;

};

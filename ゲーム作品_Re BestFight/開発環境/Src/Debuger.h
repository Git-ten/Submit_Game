#pragma once
#include<string>

#define DEBUG_STRING(line) (Debuger::GetInstance().DrawDebugString(line))
#define DEBUG_FVALUE_1(line, v1) (Debuger::GetInstance().DrawDebugValueStringF(line, v1))
#define DEBUG_FVALUE_2(line, v1, v2) (Debuger::GetInstance().DrawDebugValueStringF(line, v1, v2))
#define DEBUG_FVALUE_3(line, v1, v2, v3) (Debuger::GetInstance().DrawDebugValueStringF(line, v1, v2, v3))
#define DEBUG_DVALUE_1(line, v1) (Debuger::GetInstance().DrawDebugValueStringD(line, v1))
#define DEBUG_DVALUE_2(line, v1, v2) (Debuger::GetInstance().DrawDebugValueStringD(line, v1, v2))
#define DEBUG_DVALUE_3(line, v1, v2, v3) (Debuger::GetInstance().DrawDebugValueStringD(line, v1, v2, v3))

class Debuger
{
public:

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Debuger& GetInstance(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);
	// ���\�[�X�̔j��
	void Destroy(void);

	//�f�o�b�O�p�̕�����\��
	void DrawDebugString(std::string line);
	//�f�o�b�O�p�̒l�\��
	void DrawDebugValueStringF(std::string line, float v1);
	void DrawDebugValueStringD(std::string line, int v1);
	void DrawDebugValueStringF(std::string line, float v1, float v2);
	void DrawDebugValueStringD(std::string line, int v1, int v2);
	void DrawDebugValueStringF(std::string line, float v1, float v2, float v3);
	void DrawDebugValueStringD(std::string line, int v1, int v2, int v3);

private:

	//�V���O���g���Q��
	static Debuger* instance_;

	//������̐����L�^����
	int lineCounter;

	//�����I�ɐ錾���邱�Ƃő��N���X����A�N�Z�X�����Ȃ�
	//�R���X�g���N�^
	Debuger(void);
	//�R�s�[�R���X�g���N�^�����l
	Debuger(const Debuger&);
	//�f�X�g���N�^
	~Debuger(void) = default;

};


#pragma once
#include<map>
#include"../Utility/VectorHeader.h"
#include "SceneBase.h"

class StopScene : public SceneBase
{
public:

	enum class STATE
	{
		FADE_IN,
		FADE_OUT,
		SELECT_MENYU,
		RETURN_SELECT,
		RETURN_MENYU,
		ABOUT_GAME,
	};

	//�R���X�g���N�^
	StopScene();

	//�f�X�g���N�^
	~StopScene()override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

	//���肵����Ԃ��擾
	STATE GetDecision(void)const;
	//�O�����猈���Ԃ��Z�b�g
	void SetDecision(const STATE& stat);

private:

	//���j���[�̃T�C�Y
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Size2D MENYU_WSIZE = { 400, 288 };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Size2D MENYU_FSIZE = { 908, 310 };
	//�t�F�C�h�̑��x
	static constexpr float SPEED = 16.0f;

	//�Z���N�g:�^�C�g���ɖ߂�̃}�b�v���W
	static constexpr Position2D RE_TITLE_MAP_POS = { 5, 4 };

	//���j���[�w�i
	int imgMenyu_;

	//���݂̏��
	STATE state_;
	//���肵�����
	STATE decisionState_;

	//�������V�t�g���邩
	std::map<STATE, bool>shiftFlag_;

	//���݂̃T�C�Y
	Size2D nowSize_;
	//���艹
	std::weak_ptr<Sound>decisionSod_;



	//�����ł̂ݎg�p����֐�
	//���j���[�̃t�F�C�h�A�E�g
	void UpdateFadeOut(void);
	//���j���[�̃t�F�C�h�C��
	void UpdateFadeIn(void);
	//���j���[�̃Z���N�g����
	void UpdateSelect(void);

	//���j���[�̕`��
	void DrawMenyu(void);


};


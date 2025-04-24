#pragma once
#include"../Utility/VectorHeader.h"
#include<map>
#include "SceneBase.h"

class Sound;

class ResultMenyu : public SceneBase
{
public:

	enum class STATE
	{
		FADE_IN,
		SELECT_MENYU,
		RETURN_TITLE,
		RETURN_SELECT,
	};

	//�R���X�g���N�^
	ResultMenyu(void);

	//�f�X�g���N�^
	~ResultMenyu()override;

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

private:

	//���j���[�̃T�C�Y
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Size2D MENYU_WSIZE = { 374, 288 };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Size2D MENYU_FSIZE = { 748, 310 };
	//�t�F�C�h�̑��x
	static constexpr float SPEED = 16.0f;

	//�Z���N�g:�^�C�g���̃}�b�v���W
	static constexpr Position2D TITLE_MAP_POS = { 6, 4 };

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
	//���j���[�̃t�F�C�h�C��
	void UpdateFadeIn(void);
	//���j���[�̃Z���N�g����
	void UpdateSelect(void);

	//�`��
	void DrawMenyu(void);

};


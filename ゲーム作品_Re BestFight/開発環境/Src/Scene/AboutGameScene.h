#pragma once
#include "SceneBase.h"

class Transform;
class AnimationController;
class Sound;

class AboutGameScene :public SceneBase
{
public:

	enum class MODE
	{
		TITLE_ABOUT_GAME,//�^�C�g����ʂŊJ���ꂽ
		GAME_STOPMENYU,//�퓬���ŊJ���ꂽ
	};

	//�R���X�g���N�^
	AboutGameScene(const MODE& mode);

	//�f�X�g���N�^
	~AboutGameScene(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

	//�퓬���ɐ�����ʂ��J������
	void OpenAboutGameScene(void);
	//�퓬���ɖ߂�{�^����������
	bool IsEnd(void);

private:

	//�A�N�^�[�������o������
	static constexpr int ACTOR_STEP_MAX = 100;
	//�f���������܂ł̎���
	static constexpr float DEMO_PLAY_TIME = 30.0f;
	//�y�[�W��
	static constexpr int PAGE_MAX = 13;
	//�L�[�̒������ő厞��
	static constexpr float KEY_MAX_TIME = 1.5f;
	//����̃}�b�v�T�C�Y
	static constexpr Size2D MOVIE_SIZE = { 13, 6 };
	//��U������̃}�b�v���W
	static constexpr Position2D MOVIE_WEEK_POS = {8, 3};

	//���[�h
	MODE mode_;

	//�퓬�����[�h:������ʂ���邩�ǂ���
	bool endFlag_;

	//�i����
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	//���m
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	//���p�t
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;

	//���݂̃y�[�W
	//�v�f�ԍ��Ƃ��Ďg�p���邽�߁A0����X�^�[�g
	int nowPageCounter_;

	//�^�C�g���ɖ߂镶��
	int imgReturn_[2];
	//�߂�{�^���摜
	int imgReturnGage_;
	//�߂邩�ۂ�
	bool returnFlag_;
	//�^�C�g���ɖ߂�L�[�̒���������
	float retTitleTime_;

	//�I����
	std::weak_ptr<Sound>decisionSod_;
	//�����p�摜�̃n���h���ԍ����i�[
	int imgExplain_[PAGE_MAX];
	//�ړ��L�[�摜(��)
	int imgLeftKey_;
	//���ړ��L�[�������Ă��邩�ǂ���
	bool leftFlag_;
	//�ړ��L�[�摜(�E)
	int imgRightKey_;
	//�E�ړ��L�[�������Ă��邩�ǂ���
	bool rightFlag_;

	//�i���Ǝ�U������n���h��
	int fighterWMovie_;
	//���m��U������n���h��
	int sordmanWMovie_;
	//�����m��U������n���h��
	int magicianWMovie_;
	//�i���Ƌ��U������n���h��
	int fighterSMovie_;
	//���m��U������n���h��
	int sordmanSMovie_;
	//�����m��U������n���h��
	int magicianSMovie_;
	//�i���ƕK�E����n���h��
	int fighterSkiMovie_;
	//���m�K�E����n���h��
	int sordmanSkiMovie_;
	//���p�t�K�E����n���h��
	int magicianSkiMovie_;



	//�����ł����g��Ȃ��֐�
	//�L�������f���̏�����
	void InitActor(void);
	//�Q�[�������摜�̏�����
	void InitExplain(void);
	//�L�����̓���̏�����
	void InitMovie(void);

	//�L�����̕`��
	void DrawActor(void);
	//�Q�[�������摜�̕`��
	void DrawExPlain(void);
	//�߂�{�^���̕`��
	void DrawReturn(void);
	//�y�[�W�߂���p�摜�̕`��
	void DrawKey(void);
	//���݃y�[�W�̕`��
	void DrawPage(void);
	//�L�����̓����`��
	void DrawMovie(void);

};
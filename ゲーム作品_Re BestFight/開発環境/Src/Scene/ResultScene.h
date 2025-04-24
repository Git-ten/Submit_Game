#pragma once
#include<memory>
#include "SceneBase.h"
#include"../Manager/SceneManager.h"

class Transform;
class AnimationController;
class ResultMenyu;
class Sound;

class ResultScene :public SceneBase
{
public:

	//���݂̃Q�[���V�[���̏��
	enum class SCENE_STATE
	{
		RESULT,
		MENYU,
	};


	//�R���X�g���N�^
	ResultScene(SceneManager::CHARA_ID winner, SceneManager::CHARA_ID loser);

	//�f�X�g���N�^
	~ResultScene(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

private:

	//�s�N�Z���V�F�[�_�̐�
//�ŏ����K�����C���X�N���[��������̂�2�ȏ�ɂ���
	static constexpr int PIXEL_NUM = 2;

	//���U���g�摜�̃n���h���ԍ����i�[
	int imgResultHandle_;

	//����
	SceneManager::CHARA_ID winnerId_;
	std::unique_ptr<Transform>winner_;
	std::unique_ptr<AnimationController>winnerAnim_;

	//�s��
	SceneManager::CHARA_ID loserId_;
	std::unique_ptr<Transform>loser_;
	std::unique_ptr<AnimationController>loserAnim_;

	std::unique_ptr<Camera>camera_;

	//���j���[�V�[��
	std::unique_ptr<ResultMenyu>menyu_;

	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	//�X�V�֐��|�C���^
	UpdateFunc update_;
	//�`��֐��|�C���^
	DrawFunc draw_;
	//���݂̏��
	SCENE_STATE nowState_;
	//���艹
	std::weak_ptr<Sound> decisionSod_;
	//���U���g��(��x������Ȃ�)
	std::weak_ptr<Sound>resultSod_;



	//�����ł����g��Ȃ��֐�
	//�L�������f���̏�����
	void InitCharactor(void);
	//�L�����A�j���̏�����
	void InitCharaAnim(void);
	//��Ԃ̕ω�
	void ChangeState(const SCENE_STATE& stat);
	//���U���g�̍X�V
	void UpdateResult(void);
	//���U���g���j���[�̍X�V
	void UpdateMenyu(void);
	//���U���g�摜��`��
	void DrawResult(void);
	//�V�[���S�̂̕`��
	void DrawResultScene(void);
	//���U���g���j���[�̕`��
	void DrawMenyu(void);

};
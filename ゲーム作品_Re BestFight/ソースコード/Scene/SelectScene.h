#pragma once
#include"../Render/ModelMaterial.h"
#include"../Render/ModelRender.h"
#include "SceneBase.h"
#include<memory>

class Transform;
class ActorBase;
class AnimationController;
class Sound;

class SelectScene : public SceneBase
{
public:

	enum class PLAYER_ID
	{
		PLAYER_1,
		PLAYER_2,
		CPU,
	};

	//�R���X�g���N�^
	SelectScene(bool flg);

	//�f�X�g���N�^
	~SelectScene(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

private:

	//���͏��
	enum class INPUT
	{
		NONE = -1,

		//�J�n�����摜�̗v�f�ԍ��Ƃ��Ďg��
		//-----------------
		START_TRGUP = 0,
		START_TRGDOWN = 1,
		//----------------

		//�L�����I���̍ۂɎg��
		//-----------------
		CHARA_FIGHTER,
		CHARA_SORDMAN,
		CHARA_MAGICIAN,
		//-----------------

		MAX,
	};

	//�����̃Z���N�g�摜���W
	static constexpr Position2D SELECT_CARSOL_WPOS = {218, 425};
	static constexpr Position2D SELECT_CARSOL_FPOS = {436, 808};
	//�i���ƍ��W
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Position3DF FIGHTER__WPOS = { -100.0f, 100.0f, 600.0f };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Position3DF FIGHTER__FPOS = { 100.0f, 100.0f, 600.0f };
	//�}�b�v���W
	static constexpr Position2D FIGHTER_MAP_POS = { 3, 4 };

	//���m���W
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Position3DF SORDMAN_WPOS = { 470.0f, 100.0f, 600.0f };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Position3DF SORDMAN_FPOS = { 900.0f, 100.0f, 600.0f };
	//�}�b�v���W
	static constexpr Position2D SORDMAN_MAP_POS = { 7, 4 };

	//���p�t���W
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Position3DF MAGICIAN_WPOS = { 1100.0f, 100.0f, 600.0f };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Position3DF MAGICIAN_FPOS = { 1800.0f, 100.0f, 600.0f };
	//�}�b�v���W
	static constexpr Position2D MAGICIAN_MAP_POS = { 12, 4 };

	//�L�����̃T�C�Y
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr float CHARA_WSIZE = 2.0f;
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr float CHARA_FSIZE = 3.0f;

	//�J�n�摜�̃}�b�v���W
	static constexpr Position2D START_MAP_POS = {6, 7};
	//�J�n�摜�̃T�C�Y
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Position2D START_WSIZE = { 316, 183};
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Position2D START_FSIZE = { 516, 333 };

	//�L�[�̒������ő厞��
	static constexpr float KEY_MAX_TIME = 1.5f;

	//�`�F�[���̑��x
	static constexpr int CHAIN_SPEED = 2;
	//�`�F�[���̃T�C�Y
	static constexpr int CHAIN_SIZE_X = 32;
	static constexpr int CHAIN_SIZE_Y = 1080;

	//�Z���N�g���
	int imgSelect_;
	//�`�F�[���摜
	int imgChain_;
	//�`�F�[���̍��W
	Position2D chainPos_;

	//�^�C�g���ɖ߂镶��
	int imgReturn_[2];
	int imgReturnGage_;
	bool returnFlag_;

	//�J�n����(���͑O�Ɠ��͌�)
	int imgStart_[2];
	//���݂̓��͏��
	INPUT inputState_;
	//�L�����ɃJ�[�\�������킹�Ă��邩
	bool charaCarsolFlag_;
	//�L�������W�����炷��
	std::map<INPUT, bool>charaPosShif_;
	//�I���J�[�\���̍��W
	Position2D selectPos_;
	//���肵�悤�Ƃ��Ă��邩
	bool decisionFlag_;
	//�V�[���`�F���W�t���O
	bool changeFlag_;

	//���݂̃v���C���[ID
	PLAYER_ID nowPlayerId_;

	//CPU�Ɛ키��
	bool cpuFlag_;

	//�I�𒆂̃L����
	SceneManager::CHARA_ID charaType_;
	SceneManager::CHARA_ID charaType_2_;

	//�i����
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	Position3DF plPos1_;
	//���m
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	Position3DF plPos2_;
	//���p�t
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;
	Position3DF plPos3_;

	//�L���������ׂđI�яI�������
	bool endFlag_;
	//�^�C�g���ɖ߂�L�[�̒���������
	float retTitleTime_;

	//�J�n��
	std::weak_ptr<Sound>startSod_;
	//�I����
	std::weak_ptr<Sound>decisionSod_;
	std::weak_ptr<Sound>decisionSod_2_;
	std::weak_ptr<Sound>decisionSod_3_;
	//�Z���N�gBGM
	std::weak_ptr<Sound>selectSod_;



	//�����ł����g��Ȃ��֐�
	void InitShader(void);
	//�L�����̃Z���N�g����
	void SelectCharactor(const Position2D& mousePos);
	//�X�^�[�g�{�^���̃Z���N�g����
	void SelectStartButton(const Position2D& mousePos);
	//�L�����̌���
	void DecisionCharactor(const INPUT& inputType, SceneManager::CHARA_ID& charaType);
	//�L�����̍��W�V�t�g
	void ShiftCharactor(const SceneManager::CHARA_ID& type);
	//�v���C���[�̕ύX
	void ChangePlayer(void);
	//�I�тȂ�������
	void ReturnCharaSelect(void);
	//�`�F�[���X�N���[���̍X�V
	void UpdateChain(void);

	//�߂�{�^���̕`��
	void DrawReturn(void);
	//�`�F�[���̕`��
	void DrawChain(void);
	//�Z���N�g�摜�̕`��
	void DrawSelect(void);
	//�X�^�[�g�摜�̕`��
	void DrawStart(void);
	//�L�������f���̕`��
	void DrawCharactor(void);

	//�L�������ꂼ��̕`��
	void DrawFighter(void);
	void DrawSordMan(void);
	void DrawMagician(void);

};
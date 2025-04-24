#include<DxLib.h>
#include"Act/Common/Transform.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"../Manager/InputManager.h"
#include"MenyuScene.h"
#include"../Utility/MyUtility.h"
#include"Act/Common/AnimationController.h"
#include"../Application.h"
#include "TitleScene.h"

#define TITLE_MODEL (ResourceManager::SRC::TITLE_MODEL)

//�R���X�g���N�^
TitleScene::TitleScene()
{
	imgTitleHandle_ = -1;
	imgSelectHandle_ = -1;
	imgBack_ = -1;
	nowCarsolSelect_ = SELECT();
	for (int ii = 0; ii < INT_CAST(SELECT::MAX); ii++)carsolMapPos_[ii] = MyUtility::VECTOR2D_ZERO;
	carsolSize_ = MyUtility::VECTOR2D_ZERO;
	//�����̃T�C�Y
	playAboutSize_ = MyUtility::VECTOR2D_ZERO;
	//���j���[�V�[��
	menyu_ = nullptr;
	
	update_ = nullptr;
	draw_ = nullptr;
	nowScene_ = IN_SCENE_ID::NONE;

	demoCounter_ = 0.0f;
}

//�f�X�g���N�^
TitleScene::~TitleScene()
{

}

void TitleScene::Init()
{
	//�J�[�\���̏�����
	InitCarsol();
	//�����̏�����
	InitPlayAboutSwitch();

	//�d���{�^��
	imgSwitch_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SWITCH).lock()->handleId_;
	//�^�C�g���w�i
	imgBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKY).lock()->handleId_;
	//�^�C�g���摜�̃n���h�����i�[
	imgTitleHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE).lock()->handleId_;
	//�J�[�\���摜�̃n���h�����i�[385,520
	imgSelectHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SELECT_CARSOL).lock()->handleId_;
	//PlayGame�摜
	imgPlayGameHandle_[0] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYGAME).lock()->handleId_;
	imgPlayGameHandle_[1] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYGAME_IN).lock()->handleId_;
	//AboutGame�摜
	imgAboutGameHandle_[0] = ResourceManager::GetInstance().Load(ResourceManager::SRC::ABOUTGAME).lock()->handleId_;
	imgAboutGameHandle_[1] = ResourceManager::GetInstance().Load(ResourceManager::SRC::ABOUTGAME_IN).lock()->handleId_;

	//���艹
	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();

	//���͂��Ă��邩
	inputSelect_.clear();
	inputSelect_.emplace(SELECT::PLAY_GAME, false);
	inputSelect_.emplace(SELECT::ABOUT_GAME, false);
	inputSelect_.emplace(SELECT::SWITCH, false);

	//�^�C�g��BGM
	titleSound_ = SoundManager::GetInstance().Load(SoundManager::SOD::TITLE_BGM);

	Position3DF pos1 = PLAYER_1_WPOS;
	Position3DF pos2 = PLAYER_2_WPOS;
	Position3DF pos3 = PLAYER_3_WPOS;
	float plSize = PLAYER_WSIZE;
	if (!Application::GetInstance().IsWidowMode())
	{
		pos1 = PLAYER_1_FPOS;
		pos2 = PLAYER_2_FPOS;
		pos3 = PLAYER_3_FPOS;
		plSize = PLAYER_FSIZE;
	}

	//�i����-------------------------------------------
	plTrans1_ = std::make_unique<Transform>();
	plTrans1_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIST));
	plTrans1_->rot = MyUtility::VECTOR_ZERO;
	plTrans1_->pos = pos2;
	plTrans1_->scl = { plSize - 0.5f, plSize - 0.5f, plSize - 0.5f };
	plTrans1_->quaRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(30), MyUtility::AXIS_Y);
	plTrans1_->quaRotLocal = Quaternion();
	plTrans1_->Update();
	plAnim1_ = std::make_unique<AnimationController>(plTrans1_->modelId);
	plAnim1_->ClearAnimMap();
	std::string PATH_ANIM = "Data/Model/Player/Fist/";
	plAnim1_->Add(0, PATH_ANIM + "Idle.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim1_->Play(0);
	//------------------------------------------------

	//���m--------------------------------------------
	plTrans2_ = std::make_unique<Transform>();
	plTrans2_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_SORD));
	plTrans2_->rot = MyUtility::VECTOR_ZERO;
	plTrans2_->pos = pos1;
	plTrans2_->scl = { plSize, plSize, plSize };
	plTrans2_->quaRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(-10), MyUtility::AXIS_Y);
	plTrans2_->quaRotLocal = Quaternion();
	plTrans2_->Update();
	plAnim2_ = std::make_unique<AnimationController>(plTrans2_->modelId);
	plAnim2_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Sord/";
	plAnim2_->Add(0, PATH_ANIM + "Idle_��.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim2_->Play(0);
	//------------------------------------------------

	//���p�t--------------------------------------------
	plTrans3_ = std::make_unique<Transform>();
	plTrans3_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGIC));
	plTrans3_->rot = MyUtility::VECTOR_ZERO;
	plTrans3_->pos = pos3;
	plTrans3_->scl = { plSize, plSize, plSize };
	plTrans3_->quaRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(0), MyUtility::AXIS_Y);
	plTrans3_->quaRotLocal = Quaternion();
	plTrans3_->Update();
	plAnim3_ = std::make_unique<AnimationController>(plTrans3_->modelId);
	plAnim3_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Magic/";
	plAnim3_->Add(0, PATH_ANIM + "Idle_��.mv1", 20.0f, 0.0f, 35.0f, true);
	plAnim3_->Play(0);
	//------------------------------------------------

	InitShader();

	//�^�C�g��BGM���Đ�
	titleSound_.lock()->Play(DX_PLAYTYPE_LOOP);

	//���j���[�V�[��
	menyu_ = std::make_unique<MenyuScene>();

	//�^�C�g���V�[���ŏ�����
	ChangeScene(IN_SCENE_ID::TITLE_ID);
}

void TitleScene::Update()
{
	update_();
}

void TitleScene::Draw()
{
	draw_();
}

void TitleScene::Release()
{

}

void TitleScene::InitCarsol()
{
	//���ݍ��킹�Ă���J�[�\��
	nowCarsolSelect_ = SELECT::PLAY_GAME;

	carsolMapPos_[INT_CAST(SELECT::PLAY_GAME)] = Vector2::VGet2D(PLAYGAME_MAP_POSX, PLAYGAME_MAP_POSY);
	carsolMapPos_[INT_CAST(SELECT::ABOUT_GAME)] = Vector2::VGet2D(ABOUT_MAP_POSX, ABOUT_MAP_POSY);
	carsolMapPos_[INT_CAST(SELECT::SWITCH)] = Vector2::VGet2D(SWITCH_MAP_POSX, SWITCH_MAP_POSY);
	//�I��ԍ����Ƃ̍��W(2D)
	//�X�N���[�����E�B���h�E���[�h�̎�
	if (Application::GetInstance().IsWidowMode())
	{
		//�J�[�\���̃T�C�Y
		carsolSize_ = {CARSOL_WSIZE_X, CARSOL_WSIZE_Y};
	}
	//�X�N���[�����t���X�N���[�����[�h�̎�
	else
	{
		//�J�[�\���̃T�C�Y
		carsolSize_ = Vector2::VGet2D(CARSOL_FSIZE_X, CARSOL_FSIZE_Y);
	}
}

void TitleScene::InitPlayAboutSwitch(void)
{
	//�I��ԍ����Ƃ̍��W(2D)
	//�X�N���[�����E�B���h�E���[�h�̎�
	if (Application::GetInstance().IsWidowMode())
	{
		//�J�[�\���̃T�C�Y
		playAboutSize_ = { PLAYABOUT_WSIZE_X, PLAYABOUT_WSIZE_Y };
		switchSize_ = { SWITCH_WSIZE_X , SWITCH_WSIZE_Y};
	}
	//�X�N���[�����t���X�N���[�����[�h�̎�
	else
	{
		//�J�[�\���̃T�C�Y
		playAboutSize_ = Vector2::VGet2D(PLAYABOUT_FSIZE_X, PLAYABOUT_FSIZE_Y);
		switchSize_ = { SWITCH_FSIZE_X , SWITCH_FSIZE_Y};
	}
}

void TitleScene::ChangeScene(const IN_SCENE_ID& id)
{
	nowScene_ = id;
	switch (nowScene_)
	{
	case IN_SCENE_ID::TITLE_ID:
		demoCounter_ = DEMO_PLAY_TIME;
		update_ = [&]() {UpdateTitle(); };
		draw_ = [&]() {DrawSceneTitle(); };
		break;
	case IN_SCENE_ID::MENYU_ID:
		menyu_->Init();
		update_ = [&]() {UpdateMenyu(); };
		draw_ = [&]() {DrawSceneMenyu(); };
		break;
	default:
		break;
	}
}

void TitleScene::UpdateTitle()
{
	//���̃��f��
	//�_�l�ԏo������A�j���[�V��������ŕ`��ł���悤�ɂ���

	plAnim1_->Update();
	plAnim2_->Update();
	plAnim3_->Update();

	InputManager& ins = InputManager::GetInstance();
	bool clickTrgUpFlag = ins.IsTrgUpMouseLeft();
	bool clickTrgDownFlag = ins.IsTrgDownMouseLeft();
	bool clickFlag = ins.IsClickMouseLeft();

	//�}�E�X�A�������̓R���g���[���őI������
	if (Application::GetInstance().IsMouseFlag())
	{
		Position2D mousePos = ins.GetMousePos();
		mousePos = World2MapPos(mousePos);
		//�J�[�\������------------------------------------------------
		if (mousePos.y == PLAYGAME_MAP_POSY)
		{
			nowCarsolSelect_ = SELECT::PLAY_GAME;
			inputSelect_.at(SELECT::ABOUT_GAME) = false;
			inputSelect_.at(SELECT::SWITCH) = false;

			//���͊m�F
			//�}�E�X���͂Ői��
			if (clickTrgUpFlag && inputSelect_.at(SELECT::PLAY_GAME))
			{
				inputSelect_.at(SELECT::PLAY_GAME) = false;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				ChangeScene(IN_SCENE_ID::MENYU_ID);
			}

			if (clickTrgDownFlag)
			{
				inputSelect_.at(SELECT::PLAY_GAME) = true;
			}
		}
		else if (mousePos.y == ABOUT_MAP_POSY)
		{
			nowCarsolSelect_ = SELECT::ABOUT_GAME;
			inputSelect_.at(SELECT::PLAY_GAME) = false;
			inputSelect_.at(SELECT::SWITCH) = false;

			//���͊m�F
			//�}�E�X���͂Ői��
			if (clickTrgUpFlag && inputSelect_.at(SELECT::ABOUT_GAME))
			{
				inputSelect_.at(SELECT::PLAY_GAME) = false;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::ABOUT);
			}
			if (clickTrgDownFlag)
			{
				inputSelect_.at(SELECT::ABOUT_GAME) = true;
			}
		}
		else if ((mousePos.x == SWITCH_MAP_POSX &&
			mousePos.y == SWITCH_MAP_POSY))
		{
			nowCarsolSelect_ = SELECT::SWITCH;
			inputSelect_.at(SELECT::PLAY_GAME) = false;
			inputSelect_.at(SELECT::ABOUT_GAME) = false;
			//���͊m�F
			//�}�E�X���͂Ői��
			if (clickTrgUpFlag && !inputSelect_.at(SELECT::SWITCH))
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				//�Q�[�����I��
				Application::GetInstance().EndGame();
			}
			if (clickTrgDownFlag)
			{
				inputSelect_.at(SELECT::SWITCH) = false;
			}
			if (!clickFlag)
			{
				inputSelect_.at(SELECT::SWITCH) = true;
			}
		}
		//�L�[���͑���
		else
		{
			//�����
			if (ins.IsTrgDown(KEY_INPUT_W))
			{
				if (nowCarsolSelect_ == SELECT::ABOUT_GAME)
				{
					nowCarsolSelect_ = SELECT::PLAY_GAME;
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					inputSelect_.at(SELECT::ABOUT_GAME) = false;
				}
			}
			//������
			else if (ins.IsTrgDown(KEY_INPUT_S))
			{
				if (nowCarsolSelect_ == SELECT::PLAY_GAME)
				{
					nowCarsolSelect_ = SELECT::ABOUT_GAME;
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					inputSelect_.at(SELECT::PLAY_GAME) = false;
				}
			}

			//���菈��
			if (nowCarsolSelect_ == SELECT::PLAY_GAME)
			{
				if (ins.IsTrgUp(KEY_INPUT_RETURN))
				{
					inputSelect_.at(SELECT::PLAY_GAME) = false;
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					ChangeScene(IN_SCENE_ID::MENYU_ID);
				}
				if (ins.IsTrgDown(KEY_INPUT_RETURN))
				{
					inputSelect_.at(SELECT::PLAY_GAME) = true;
				}
			}
			if (nowCarsolSelect_ == SELECT::ABOUT_GAME)
			{
				if (ins.IsTrgUp(KEY_INPUT_RETURN))
				{
					inputSelect_.at(SELECT::PLAY_GAME) = false;
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::ABOUT);
				}
				if (ins.IsTrgDown(KEY_INPUT_RETURN))
				{
					inputSelect_.at(SELECT::ABOUT_GAME) = true;
				}
			}
		}
	}
	//------------------------------------------------------------
	else
	{
		//�R���g���[������--------------------------------------------
		auto controll_1 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		auto controll_2 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2);

		//�����
		if (controll_1.aKeyLY < 0 ||
			controll_2.aKeyLY < 0)
		{
			if (nowCarsolSelect_ == SELECT::ABOUT_GAME)
			{
				nowCarsolSelect_ = SELECT::PLAY_GAME;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				inputSelect_.at(SELECT::ABOUT_GAME) = false;
			}
		}
		//������
		else if (controll_1.aKeyLY > 0 ||
			controll_2.aKeyLY > 0)
		{
			if (nowCarsolSelect_ == SELECT::PLAY_GAME)
			{
				nowCarsolSelect_ = SELECT::ABOUT_GAME;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				inputSelect_.at(SELECT::PLAY_GAME) = false;
			}
		}

		//���菈��
		if (nowCarsolSelect_ == SELECT::PLAY_GAME)
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				inputSelect_.at(SELECT::PLAY_GAME) = false;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				ChangeScene(IN_SCENE_ID::MENYU_ID);
			}
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				inputSelect_.at(SELECT::PLAY_GAME) = true;
			}
		}
		if (nowCarsolSelect_ == SELECT::ABOUT_GAME)
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				inputSelect_.at(SELECT::PLAY_GAME) = false;
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::ABOUT);
			}
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				inputSelect_.at(SELECT::ABOUT_GAME) = true;
			}
		}

		//�d���{�^��������
		if ((ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L_TRIGGER)) &&
			(ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER)))
		{
			inputSelect_.at(SELECT::SWITCH) = false;
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			//�Q�[�����I��
			Application::GetInstance().EndGame();
			//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		if ((ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L_TRIGGER)) &&
			(ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER) ||
				ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER)))
		{
			inputSelect_.at(SELECT::SWITCH) = true;
		}
		
		//------------------------------------------------------------
	}
	float delta = SceneManager::GetInstance().GetDeltaTime();
	demoCounter_ -= delta;
	if (demoCounter_ < 0.0f)
	{
		demoCounter_ = DEMO_PLAY_TIME;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::DEMO);
	}
}

void TitleScene::UpdateMenyu()
{
	//���j���[�̕`��
	menyu_->Update();

	MenyuScene::STATE decision = menyu_->GetDecision();
	switch (decision)
	{
	case MenyuScene::STATE::RETURN_MENYU:
		ChangeScene(IN_SCENE_ID::TITLE_ID);
		break;
	case MenyuScene::STATE::SELECT_VS_CPU:
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		SceneManager::GetInstance().SetCpuFlag(true);
		titleSound_.lock()->Stop();
		break;
	case MenyuScene::STATE::SELECT_VS_PLAYER:
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		SceneManager::GetInstance().SetCpuFlag(false);
		titleSound_.lock()->Stop();
		break;
	}
}

void TitleScene::DrawSceneTitle()
{
	//�^�C�g���摜��`��
	DrawTitle();

	MV1DrawModel(plTrans1_->modelId);
	MV1DrawModel(plTrans2_->modelId);
	MV1DrawModel(plTrans3_->modelId);

	//�J�[�\���摜��`��
	DrawSelect();

	DrawPlayAboutSwitch();
}

void TitleScene::DrawSceneMenyu()
{
	//�^�C�g���摜��`��
	DrawTitle();

	MV1DrawModel(plTrans1_->modelId);
	MV1DrawModel(plTrans2_->modelId);
	MV1DrawModel(plTrans3_->modelId);

	//�J�[�\���摜��`��
	DrawSelect();

	DrawPlayAboutSwitch();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//���j���[�̕`��
	menyu_->Draw();
}

//�^�C�g���摜��`��
void TitleScene::DrawTitle()
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	SetDrawBright(210, 210, 210);
	DrawExtendGraph(0, 0, screenSize.x, screenSize.y, imgBack_, true);
	SetDrawBright(255, 255, 255);
	DrawExtendGraph(0, 0, screenSize.x -1, screenSize.y -1, imgTitleHandle_, true);
}

//�J�[�\���摜��`��
void TitleScene::DrawSelect()
{
	if (nowCarsolSelect_ == SELECT::SWITCH)return;

	Position2D pos = carsolMapPos_[INT_CAST(nowCarsolSelect_)];
	Size2D mapSize = Application::GetInstance().GetMapSize();
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;
	DrawExtendGraph(pos.x, pos.y, pos.x + carsolSize_.x, pos.y + carsolSize_.y, imgSelectHandle_, true);
}

void TitleScene::DrawPlayAboutSwitch(void)
{
	Position2D pos = carsolMapPos_[INT_CAST(SELECT::PLAY_GAME)];
	Size2D mapSize = Application::GetInstance().GetMapSize();
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;

	if (!inputSelect_.at(SELECT::PLAY_GAME))
	{
		DrawExtendGraph(pos.x + carsolSize_.x, pos.y, pos.x + playAboutSize_.x, pos.y + playAboutSize_.y, imgPlayGameHandle_[0], true);
	}
	else
	{
		DrawExtendGraph(pos.x + carsolSize_.x, pos.y, pos.x + playAboutSize_.x, pos.y + playAboutSize_.y, imgPlayGameHandle_[1], true);
	}

	pos = carsolMapPos_[INT_CAST(SELECT::ABOUT_GAME)];
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;

	if (!inputSelect_.at(SELECT::ABOUT_GAME))
	{
		DrawExtendGraph(pos.x + carsolSize_.x, pos.y, pos.x + playAboutSize_.x, pos.y + playAboutSize_.y, imgAboutGameHandle_[0], true);
	}
	else
	{
		DrawExtendGraph(pos.x + carsolSize_.x, pos.y, pos.x + playAboutSize_.x, pos.y + playAboutSize_.y, imgAboutGameHandle_[1], true);
	}

	pos = carsolMapPos_[INT_CAST(SELECT::SWITCH)];
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;

	if (!inputSelect_.at(SELECT::SWITCH))
	{
		DrawExtendGraph(pos.x, pos.y, pos.x + switchSize_.x, pos.y + switchSize_.y, imgSwitch_, true);
	}
	else
	{
		DrawExtendGraph(pos.x, pos.y, pos.x + switchSize_.x + 10, pos.y + switchSize_.y + 10, imgSwitch_, true);
	}

	int fontSize = 32;
	if (!Application::GetInstance().IsWidowMode())fontSize *= 2;

	if (!Application::GetInstance().IsMouseFlag())
	{
		SetFontSize(fontSize);
		DrawFormatString(pos.x - fontSize * 4, pos.y + 6, 0xffffff, "L2 + R2");
		SetFontSize(16);
	}
	else
	{
		SetFontSize(fontSize);
		DrawFormatString(pos.x - fontSize * 4, pos.y + 6, 0xffffff, "Click");
		SetFontSize(16);
	}

}
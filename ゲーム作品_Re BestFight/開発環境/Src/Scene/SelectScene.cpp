#include<DxLib.h>
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"Act/Common/Transform.h"
#include"../Application.h"
#include"../Utility/MyUtility.h"
#include"Act/Common/AnimationController.h"
#include "SelectScene.h"

#define SELECT (ResourceManager::SRC::SELECT)
#define START (ResourceManager::SRC::START)
#define START_INPUT (ResourceManager::SRC::START_INPUT)
#define FIGHTER (ResourceManager::SRC::PLAYER_FIST)
#define SORDMAN (ResourceManager::SRC::PLAYER_SORD)
#define MAGICIAN (ResourceManager::SRC::PLAYER_MAGIC)
#define CHAIN (ResourceManager::SRC::CHAIN)
#define RETURN (ResourceManager::SRC::RETURN_BUTTON)
#define RETURN_INPUT (ResourceManager::SRC::RETURN_BUTTON_INPUT)
#define RETURN_GAGE (ResourceManager::SRC::RETURN_BUTTGAGE)
#define B_RETURN_GAGE (ResourceManager::SRC::B_RETURN_BUTTGAGE)

SelectScene::SelectScene(bool flg)
{
	cpuFlag_ = flg;
	endFlag_ = false;
	nowPlayerId_ = PLAYER_ID::PLAYER_1;
	imgSelect_ = -1;
	imgChain_ = -1;
	chainPos_ = Position2D();
	inputState_ = INPUT::NONE;
	charaCarsolFlag_ = false;
	//決定しようとしているか
	decisionFlag_ = false;
	//シーンチェンジフラグ
	changeFlag_ = false;
	charaType_ = SceneManager::CHARA_ID::NONE;
	charaType_2_ = SceneManager::CHARA_ID::NONE;
	//選択カーソルの座標
	selectPos_ = {-1, -1};
	for (int ii = 0; ii < 2; ii++)
	{
		imgStart_[ii] = -1;
		imgReturn_[ii] = -1;
	}
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	//セレクト画面
	imgSelect_ = ResourceManager::GetInstance().Load(SELECT).lock()->handleId_;

	//チェーン画像
	imgChain_ = ResourceManager::GetInstance().Load(CHAIN).lock()->handleId_;

	imgStart_[0] = ResourceManager::GetInstance().Load(START).lock()->handleId_;
	imgStart_[1] = ResourceManager::GetInstance().Load(START_INPUT).lock()->handleId_;

	//タイトルに戻るボタン
	imgReturn_[0] = ResourceManager::GetInstance().Load(RETURN).lock()->handleId_;
	imgReturn_[1] = ResourceManager::GetInstance().Load(RETURN_INPUT).lock()->handleId_;
	if (Application::GetInstance().IsWidowMode())
	{
		imgReturnGage_ = ResourceManager::GetInstance().Load(RETURN_GAGE).lock()->handleId_;
	}
	else
	{
		imgReturnGage_ = ResourceManager::GetInstance().Load(B_RETURN_GAGE).lock()->handleId_;
	}

	returnFlag_ = false;

	inputState_ = INPUT::CHARA_FIGHTER;

	changeFlag_ = false;
	decisionFlag_ = false;

	nowPlayerId_ = PLAYER_ID::PLAYER_1;

	charaPosShif_.clear();
	charaPosShif_.emplace(INPUT::CHARA_FIGHTER, false);
	charaPosShif_.emplace(INPUT::CHARA_SORDMAN, false);
	charaPosShif_.emplace(INPUT::CHARA_MAGICIAN, false);


	//選択カーソルの座標
	selectPos_ = { SELECT_CARSOL_WPOS.x , SELECT_CARSOL_WPOS.y };

	plPos1_ = FIGHTER__WPOS;
	plPos2_ = SORDMAN_WPOS;
	plPos3_ = MAGICIAN_WPOS;
	float charaSize = CHARA_WSIZE;

	if (!Application::GetInstance().IsWidowMode())
	{
		selectPos_ = { SELECT_CARSOL_FPOS.x , SELECT_CARSOL_FPOS.y };
		plPos1_ = FIGHTER__FPOS;
		plPos2_ = SORDMAN_FPOS;
		plPos3_ = MAGICIAN_FPOS;
		charaSize = CHARA_FSIZE;
	}

	plTrans1_ = std::make_unique<Transform>();
	plTrans1_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(FIGHTER));
	plTrans1_->rot = MyUtility::VECTOR_ZERO;
	plTrans1_->pos = plPos1_;
	plTrans1_->scl = { charaSize, charaSize, charaSize };
	plTrans1_->quaRot = Quaternion();
	plTrans1_->quaRotLocal = Quaternion();
	plTrans1_->Update();

	plAnim1_ = std::make_unique<AnimationController>(plTrans1_->modelId);
	plAnim1_->ClearAnimMap();
	std::string PATH_ANIM = "Data/Model/Player/Fist/";
	plAnim1_->Add(0, PATH_ANIM + "Idle.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim1_->Play(0);

	plTrans2_ = std::make_unique<Transform>();
	plTrans2_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(SORDMAN));
	plTrans2_->rot = MyUtility::VECTOR_ZERO;
	plTrans2_->pos = plPos2_;
	plTrans2_->scl = { charaSize, charaSize, charaSize };
	plTrans2_->quaRot = Quaternion();
	plTrans2_->quaRotLocal = Quaternion();
	plTrans2_->Update();

	plAnim2_ = std::make_unique<AnimationController>(plTrans2_->modelId);
	plAnim2_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Sord/";
	plAnim2_->Add(0, PATH_ANIM + "Idle_β.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim2_->Play(0);
	
	plTrans3_ = std::make_unique<Transform>();
	plTrans3_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(MAGICIAN));
	plTrans3_->rot = MyUtility::VECTOR_ZERO;
	plTrans3_->pos = plPos3_;
	plTrans3_->scl = { charaSize, charaSize, charaSize };
	plTrans3_->quaRot = Quaternion();
	plTrans3_->quaRotLocal = Quaternion();
	plTrans3_->Update();

	plAnim3_ = std::make_unique<AnimationController>(plTrans3_->modelId);
	plAnim3_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Magic/";
	plAnim3_->Add(0, PATH_ANIM + "Idle_Γ.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim3_->Play(0);

	InitShader();

	startSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::CHANGE_SCENE);
	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION_2).lock();
	decisionSod_2_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION_3).lock();
	decisionSod_3_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();
	selectSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::SELECT_BGM).lock();
	selectSod_.lock()->Play(DX_PLAYTYPE_LOOP);
}

void SelectScene::Update(void)
{
	//チェーン
	UpdateChain();

	plAnim1_->Update();
	plAnim2_->Update();
	plAnim3_->Update();

	ChangePlayer();

	InputManager& ins = InputManager::GetInstance();
	Position2D mousePos = ins.GetMousePos();
	mousePos = World2MapPos(mousePos);

	//マウス,もしくはコントローラで選択する
	if (!endFlag_)
	{
		//カーソル処理
		SelectCharactor(mousePos);

		if (nowPlayerId_ == PLAYER_ID::PLAYER_1)ShiftCharactor(charaType_);
		else									ShiftCharactor(charaType_2_);
	}
	else
	{
		//選びなおすか
		ReturnCharaSelect();
		//開始ボタン
		SelectStartButton(mousePos);
	}

	if (changeFlag_)
	{
		if (charaType_ != SceneManager::CHARA_ID::NONE &&
			charaType_2_ != SceneManager::CHARA_ID::NONE)
		{
			startSod_.lock()->Play(DX_PLAYTYPE_BACK);
			selectSod_.lock()->Stop();
			SceneManager::GetInstance().SetPlayerAndEnemy(charaType_, charaType_2_);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		else
		{
			changeFlag_ = false;
		}
	}

	//タイトルシーンに戻る
	if (ins.IsNew(KEY_INPUT_Z) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT))
	{
		returnFlag_ = true;
		float delta = SceneManager::GetInstance().GetDeltaTime();
		//長押しして、100%になったらタイトルに戻る
		retTitleTime_ += delta;
		if (retTitleTime_ >= KEY_MAX_TIME)
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	else
	{
		returnFlag_ = false;
		retTitleTime_ = 0.0f;
	}
}

void SelectScene::Draw(void)
{
	DrawSelect();
	DrawChain();
	DrawCharactor();

	//セレクト
	int color = 0xffffff;
	std::string playerNum = "";
	if (nowPlayerId_ == PLAYER_ID::PLAYER_1)
	{
		color = 0xff0000;
		playerNum = "P1";
	}
	else if (nowPlayerId_ == PLAYER_ID::PLAYER_2)
	{
		color = 0x00ff00;
		playerNum = "P2";
	}
	else
	{
		color = 0x0000ff;
		playerNum = "CPU";
	}

	Size2D boxSize = {128, 15};
	int circleR = 50;
	int fontSize = 45;
	if (!Application::GetInstance().IsWidowMode())
	{
		boxSize = { 236, 30 };
		circleR *= 2;
		circleR -= 20;
		fontSize = 75;
	}

	if (selectPos_.x >= 0 && selectPos_.y >= 0)
	{
		DrawBox(selectPos_.x - boxSize.x, selectPos_.y - 10,
			selectPos_.x + boxSize.x, selectPos_.y + boxSize.y, color, true);

		DrawCircle(selectPos_.x - boxSize.x - 27, selectPos_.y - boxSize.y * 2, circleR, color);

		SetFontSize(fontSize);
		DrawFormatString(selectPos_.x - boxSize.x - circleR, selectPos_.y - boxSize.y * 2 - 20, 0x000000, playerNum.c_str());
		SetFontSize(16);
	}

	//セレクトモード表示
	if (cpuFlag_)
	{
		SetFontSize(fontSize + 15);
		DrawFormatString(55, 0, 0x0000ff, "VS CPU");
		SetFontSize(16);
	}
	else
	{
		SetFontSize(fontSize + 15);
		DrawFormatString(55, 0, 0x00ff00, "VS PLAYER");
		SetFontSize(16);
	}

	//戻るボタン
	DrawReturn();

	if (endFlag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		Size2D screenSize = Application::GetInstance().GetScreenSize();
		DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


		SetFontSize(fontSize);
		DrawFormatString(screenSize.x / 2 - fontSize * 5, screenSize.y / 2, 0xffffff, "Start By Click or A");
		if (!Application::GetInstance().IsWidowMode())
		{
			fontSize -= 12;
			SetFontSize(fontSize);
			DrawFormatString(screenSize.x - fontSize * 10, screenSize.y - 100, 0xffffff, "Return By Z or B");
		}
		else
		{
			fontSize -= 12;
			SetFontSize(fontSize);
			DrawFormatString(screenSize.x - fontSize * 9, screenSize.y - 60, 0xffffff, "Return By Z or B");
		}
		SetFontSize(16);
		DrawStart();
	}

//	DrawMap();
}

void SelectScene::Release(void)
{
}

void SelectScene::InitShader(void)
{
	//material_ = std::make_unique<ModelMaterial>(
	//	"RimModelSkinVS.cso", 1,
	//	"RimModelPS.cso", 0
	//);

	////頂点シェーダ定数セット
	//Position3DF cameraPos = { 320.0f, 240.0f, 0.0f};
	//material_->AddConstBufVS({ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });

	//render_ = std::make_unique<ModelRender>(plTrans1_->modelId, *material_);
}

void SelectScene::SelectCharactor(const Position2D& mousePos)
{
	//マップ座標
	Position3DF pl_1Pos = Position3DF();
	Position3DF pl_2Pos = Position3DF();
	Position3DF pl_3Pos = Position3DF();
	Position2D pl_1MapPos = Position2D();
	Position2D pl_2MapPos = Position2D();
	Position2D pl_3MapPos = Position2D();

	//格闘家
	pl_1Pos = ConvWorldPosToScreenPos(plTrans1_->pos);
	pl_1MapPos = World2MapPos({ INT_CAST(pl_1Pos.x), INT_CAST(pl_1Pos.y) });
	pl_1MapPos = {3, 4};
	//剣士
	pl_2Pos = ConvWorldPosToScreenPos(plTrans2_->pos);
	pl_2MapPos = World2MapPos({ INT_CAST(pl_2Pos.x), INT_CAST(pl_2Pos.y) });
	pl_2MapPos = {7, 4};
	//魔術師
	pl_3Pos = ConvWorldPosToScreenPos(plTrans3_->pos);
	pl_3MapPos = World2MapPos({ INT_CAST(pl_3Pos.x), INT_CAST(pl_3Pos.y) });
	pl_3MapPos = {12, 4};

	InputManager& ins = InputManager::GetInstance();
	if (Application::GetInstance().IsMouseFlag())
	{
		bool clickTrgUpFlag = ins.IsTrgUpMouseLeft();
		bool clickTrgDownFlag = ins.IsTrgDownMouseLeft();
		bool clickFlag = ins.IsClickMouseLeft();

		//マウス処理-------------------------------------------------------
		if ((pl_1MapPos.y <= mousePos.y && mousePos.y <= pl_1MapPos.y + 4) &&
			(pl_1MapPos.x == mousePos.x))
		{
			inputState_ = INPUT::CHARA_FIGHTER;
			charaCarsolFlag_ = true;

			selectPos_ = { INT_CAST(pl_1Pos.x), INT_CAST(pl_1Pos.y) };

			//決定するか
			if (clickTrgUpFlag && decisionFlag_)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				if (nowPlayerId_ == PLAYER_ID::PLAYER_1)DecisionCharactor(inputState_, charaType_);
				else									DecisionCharactor(inputState_, charaType_2_);
			}
			//決定入力の更新
			if (clickTrgDownFlag)
			{
				decisionFlag_ = true;
			}
			else if (!clickFlag)
			{
				decisionFlag_ = false;
			}
		}
		else if ((pl_2MapPos.y <= mousePos.y && mousePos.y <= pl_2MapPos.y + 4) &&
			(pl_2MapPos.x == mousePos.x))
		{
			inputState_ = INPUT::CHARA_SORDMAN;
			charaCarsolFlag_ = true;

			selectPos_ = { INT_CAST(pl_2Pos.x), INT_CAST(pl_2Pos.y) };

			//決定するか
			if (clickTrgUpFlag && decisionFlag_)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				if (nowPlayerId_ == PLAYER_ID::PLAYER_1)DecisionCharactor(inputState_, charaType_);
				else									DecisionCharactor(inputState_, charaType_2_);
			}
			//決定入力の更新
			if (clickTrgDownFlag)
			{
				decisionFlag_ = true;
			}
			else if (!clickFlag)
			{
				decisionFlag_ = false;
			}
		}
		else if ((pl_3MapPos.y <= mousePos.y && mousePos.y <= pl_3MapPos.y + 4) &&
			(pl_3MapPos.x == mousePos.x))
		{
			inputState_ = INPUT::CHARA_MAGICIAN;
			charaCarsolFlag_ = true;

			selectPos_ = { INT_CAST(pl_3Pos.x), INT_CAST(pl_3Pos.y) };

			//決定するか
			if (clickTrgUpFlag && decisionFlag_)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				if (nowPlayerId_ == PLAYER_ID::PLAYER_1)DecisionCharactor(inputState_, charaType_);
				else									DecisionCharactor(inputState_, charaType_2_);
			}
			//決定入力の更新
			if (clickTrgDownFlag)
			{
				decisionFlag_ = true;
			}
			else if (!clickFlag)
			{
				decisionFlag_ = false;
			}
		}
		//------------------------------------------------------------------
	}
	else
	{
		//コントローラ処理--------------------------------------------------
		auto controll_1 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		auto controll_2 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2);
		bool TrgDown = false;
		bool TrgUpR = false;
		bool TrgUpD = false;
		int ControllLX = 0;
		if (nowPlayerId_ == PLAYER_ID::PLAYER_1 ||
			nowPlayerId_ == PLAYER_ID::CPU)
		{
			TrgDown = ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD1);
			TrgUpD = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);
			TrgUpR = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);
			ControllLX = controll_1.aKeyLX;
		}
		else if (nowPlayerId_ == PLAYER_ID::PLAYER_2)
		{
			TrgDown = ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD2);
			TrgUpD = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN);
			TrgUpR = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT);
			ControllLX = controll_2.aKeyLX;
		}

		if (ControllLX < 0 &&
			TrgDown)
		{
			if (inputState_ == INPUT::CHARA_SORDMAN)
			{
				inputState_ = INPUT::CHARA_FIGHTER;
				pl_1Pos = ConvWorldPosToScreenPos(plTrans1_->pos);
				selectPos_ = { INT_CAST(pl_1Pos.x), INT_CAST(pl_1Pos.y) };
			}
			else if (inputState_ == INPUT::CHARA_MAGICIAN)
			{
				inputState_ = INPUT::CHARA_SORDMAN;
				pl_2Pos = ConvWorldPosToScreenPos(plTrans2_->pos);
				selectPos_ = { INT_CAST(pl_2Pos.x), INT_CAST(pl_2Pos.y) };
			}

			decisionSod_3_.lock()->Play(DX_PLAYTYPE_BACK);
		}
		else if (ControllLX > 0 &&
			TrgDown)
		{
			if (inputState_ == INPUT::CHARA_FIGHTER)
			{
				inputState_ = INPUT::CHARA_SORDMAN;
				pl_2Pos = ConvWorldPosToScreenPos(plTrans2_->pos);
				selectPos_ = { INT_CAST(pl_2Pos.x), INT_CAST(pl_2Pos.y) };
			}
			else if (inputState_ == INPUT::CHARA_SORDMAN)
			{
				inputState_ = INPUT::CHARA_MAGICIAN;
				pl_3Pos = ConvWorldPosToScreenPos(plTrans3_->pos);
				selectPos_ = { INT_CAST(pl_3Pos.x), INT_CAST(pl_3Pos.y) };
			}

			decisionSod_3_.lock()->Play(DX_PLAYTYPE_BACK);
		}
		//決定処理
		if (TrgUpD)
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			if (nowPlayerId_ == PLAYER_ID::PLAYER_1)
			{
				DecisionCharactor(inputState_, charaType_);
			}
			else
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				DecisionCharactor(inputState_, charaType_2_);
			}
		}
		else if (TrgUpR)
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			if (nowPlayerId_ == PLAYER_ID::PLAYER_1)
			{
				charaType_ = SceneManager::CHARA_ID::NONE;
			}
			else
			{
				charaType_2_ = SceneManager::CHARA_ID::NONE;
			}
		}
		//------------------------------------------------------------------
	}
}

void SelectScene::SelectStartButton(const Position2D& mousePos)
{
	const int START_BUTTON_POSX_MIN = 5;
	const int START_BUTTON_POSX_MAX = 10;
	const int START_BUTTON_POSY_MIN = 7;
	const int START_BUTTON_POSY_MAX = 10;

	InputManager& ins = InputManager::GetInstance();
	if (Application::GetInstance().IsMouseFlag())
	{
		bool clickTrgUpFlag = ins.IsTrgUpMouseLeft();
		bool clickTrgDownFlag = ins.IsTrgDownMouseLeft();
		bool clickFlag = ins.IsClickMouseLeft();

		//マウス処理---------------------------------------
		if (START_BUTTON_POSX_MIN <= mousePos.x &&
			mousePos.x <= START_BUTTON_POSX_MAX &&
			START_BUTTON_POSY_MIN <= mousePos.y &&
			mousePos.y <= START_BUTTON_POSY_MAX)
		{
			switch (inputState_)
			{
			case INPUT::START_TRGUP:
				break;
			case INPUT::START_TRGDOWN:
				if (clickTrgUpFlag && decisionFlag_)
				{
					inputState_ = INPUT::START_TRGUP;
					//シーンを切り替える
					changeFlag_ = true;
				}
				break;
			default:
				if (clickTrgDownFlag)
				{
					inputState_ = INPUT::START_TRGDOWN;
					decisionFlag_ = true;
				}
				break;
			}

			charaCarsolFlag_ = true;
		}
		else
		{
			inputState_ = INPUT::NONE;
		}
		//-------------------------------------------------
	}
	else
	{
		//コントローラ処理----------------------------------------------------
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			inputState_ = INPUT::START_TRGDOWN;
		}
		if ((ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN)))
		{
			inputState_ = INPUT::START_TRGUP;
			//シーンを切り替える
			changeFlag_ = true;
		}
		//------------------------------------------------------------------
	}
}

void SelectScene::DecisionCharactor(const INPUT& inputType, SceneManager::CHARA_ID& charaType)
{
	if (inputType == INPUT::CHARA_FIGHTER)
	{
		if (charaType == SceneManager::CHARA_ID::FIGHTER_ID)
		{
			charaType = SceneManager::CHARA_ID::NONE;
			charaPosShif_.at(inputType) = false;
		}
		else
		{
			charaType = SceneManager::CHARA_ID::FIGHTER_ID;
			charaPosShif_.at(inputType) = true;
		}
	}
	else if (inputType == INPUT::CHARA_SORDMAN)
	{
		if (charaType == SceneManager::CHARA_ID::SORDMAN_ID)
		{
			charaType = SceneManager::CHARA_ID::NONE;
			charaPosShif_.at(inputType) = false;
		}
		else
		{
			charaType = SceneManager::CHARA_ID::SORDMAN_ID;
			charaPosShif_.at(inputType) = true;
		}
	}
	else if (inputType == INPUT::CHARA_MAGICIAN)
	{
		if (charaType == SceneManager::CHARA_ID::MAGICIAN_ID)
		{
			charaType = SceneManager::CHARA_ID::NONE;
			charaPosShif_.at(inputType) = false;
		}
		else
		{
			charaType = SceneManager::CHARA_ID::MAGICIAN_ID;
			charaPosShif_.at(inputType) = true;
		}
	}

}

void SelectScene::ShiftCharactor(const SceneManager::CHARA_ID& type)
{
	if (type == SceneManager::CHARA_ID::FIGHTER_ID)
	{
		charaPosShif_.at(INPUT::CHARA_FIGHTER) = true;
		charaPosShif_.at(INPUT::CHARA_SORDMAN) = false;
		charaPosShif_.at(INPUT::CHARA_MAGICIAN) = false;
	}
	else if (type == SceneManager::CHARA_ID::SORDMAN_ID)
	{
		charaPosShif_.at(INPUT::CHARA_FIGHTER) = false;
		charaPosShif_.at(INPUT::CHARA_SORDMAN) = true;
		charaPosShif_.at(INPUT::CHARA_MAGICIAN) = false;
	}
	else if(type == SceneManager::CHARA_ID::MAGICIAN_ID)
	{
		charaPosShif_.at(INPUT::CHARA_FIGHTER) = false;
		charaPosShif_.at(INPUT::CHARA_SORDMAN) = false;
		charaPosShif_.at(INPUT::CHARA_MAGICIAN) = true;
	}
	else
	{
		charaPosShif_.at(INPUT::CHARA_FIGHTER) = false;
		charaPosShif_.at(INPUT::CHARA_SORDMAN) = false;
		charaPosShif_.at(INPUT::CHARA_MAGICIAN) = false;
	}
}

void SelectScene::ChangePlayer(void)
{
	bool inputEnter = InputManager::GetInstance().IsTrgUp(KEY_INPUT_RETURN);
	bool TrgUpT = false;

	bool decision = false;
	if (Application::GetInstance().IsMouseFlag())
	{
		decision = inputEnter;
	}
	else
	{
		if (nowPlayerId_ == PLAYER_ID::PLAYER_1 ||
			nowPlayerId_ == PLAYER_ID::CPU)
		{
			TrgUpT = InputManager::GetInstance().
				IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP);
		}
		else
		{
			TrgUpT = InputManager::GetInstance().
				IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::TOP);
		}
		decision = TrgUpT;
	}

	if (decision)
	{
		switch (nowPlayerId_)
		{
		case PLAYER_ID::PLAYER_1:
			if (charaType_ == SceneManager::CHARA_ID::NONE)return;

			decisionSod_2_.lock()->Play(DX_PLAYTYPE_BACK);
			if (cpuFlag_)nowPlayerId_ = PLAYER_ID::CPU;
			else		 nowPlayerId_ = PLAYER_ID::PLAYER_2;
			charaPosShif_.at(INPUT::CHARA_FIGHTER) = false;
			charaPosShif_.at(INPUT::CHARA_SORDMAN) = false;
			break;
		case PLAYER_ID::CPU:
		case PLAYER_ID::PLAYER_2:
			if (charaType_2_ == SceneManager::CHARA_ID::NONE)return;
			decisionSod_2_.lock()->Play(DX_PLAYTYPE_BACK);
			endFlag_ = true;
			break;
		default:
			break;
		}
	}
}

void SelectScene::ReturnCharaSelect(void)
{
	InputManager& ins = InputManager::GetInstance();

	bool decision = false;
	if (Application::GetInstance().IsMouseFlag())
	{
		decision = ins.IsTrgUp(KEY_INPUT_Z);
	}
	else
	{
		decision = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT);
	}

	if (decision)
	{
		endFlag_ = false;
		charaType_ = SceneManager::CHARA_ID::NONE;
		charaType_2_ = SceneManager::CHARA_ID::NONE;
		nowPlayerId_ = PLAYER_ID::PLAYER_1;
	}
}

void SelectScene::UpdateChain(void)
{
	chainPos_.y -= CHAIN_SPEED;
	if (chainPos_.y < -CHAIN_SIZE_Y)
	{
		chainPos_.y = 0;
	}
}

void SelectScene::DrawReturn(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D pos = {30, screenSize.y - 64};
	Size2D size = {45, 45};
	int fontSize = 32;
	if (!Application::GetInstance().IsWidowMode())
	{
		pos.x += 30;
		pos.y -= 64;
		size.x *= 2;
		size.y *= 2;
		fontSize = 64;
	}

	DrawExtendGraph(pos.x, pos.y, pos.x + size.x, pos.y + size.y, imgReturn_[static_cast<int>(returnFlag_)], true);

	float sizeF_X = static_cast<float>(size.x);

	int rate = static_cast<int>(sizeF_X * (retTitleTime_ / KEY_MAX_TIME));
	DrawRectGraph(pos.x, pos.y, 0, 0, size.y, rate, imgReturnGage_, true);
	SetFontSize(fontSize);
	int lightP = 255;
	if (!returnFlag_)
	{
		lightP = 255;
	}
	else
	{
		lightP = 100;
	}
	SetDrawBright(lightP, lightP, lightP);
	if(Application::GetInstance().IsMouseFlag())DrawFormatString(pos.x + 4, pos.y + fontSize / 2 - 4, 0xbb0000, "Z");
	else DrawFormatString(pos.x + 4, pos.y + fontSize / 2 - 4, 0xbb0000, "B");
	SetDrawBright(255, 255, 255);
	DrawFormatString(pos.x + 4 + fontSize, pos.y + fontSize / 2 - 4, 0xffffff, "Return Title");
	SetFontSize(16);
}

void SelectScene::DrawChain(void)
{
	Size2D cahinSize = { CHAIN_SIZE_X , CHAIN_SIZE_Y };
	if (!Application::GetInstance().IsWidowMode())
	{
		cahinSize.x *= 2;
		cahinSize.y *= 2;
	}

	//左
	DrawExtendGraph(chainPos_.x, chainPos_.y, 
		chainPos_.x + cahinSize.x, chainPos_.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(chainPos_.x, chainPos_.y + cahinSize.y,
		chainPos_.x + cahinSize.x, chainPos_.y + cahinSize.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(chainPos_.x, -chainPos_.y, 
		chainPos_.x + cahinSize.x, -chainPos_.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(chainPos_.x, -chainPos_.y - cahinSize.y,
		chainPos_.x + cahinSize.x, -chainPos_.y - cahinSize.y + cahinSize.y, imgChain_, true);

	//右
	Size2D size = Application::GetInstance().GetScreenSize();
	DrawExtendGraph(size.x - cahinSize.x, chainPos_.y,
		size.x - cahinSize.x + cahinSize.x, chainPos_.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(size.x - cahinSize.x, chainPos_.y + cahinSize.y,
		size.x - cahinSize.x + cahinSize.x, chainPos_.y + cahinSize.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(size.x - cahinSize.x, -chainPos_.y,
		size.x - cahinSize.x + cahinSize.x, -chainPos_.y + cahinSize.y, imgChain_, true);

	DrawExtendGraph(size.x - cahinSize.x, -chainPos_.y - cahinSize.y,
		size.x - cahinSize.x + cahinSize.x, -chainPos_.y - cahinSize.y + cahinSize.y, imgChain_, true);
}

void SelectScene::DrawSelect(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawExtendGraph(0, 0, screenSize.x, screenSize.y, imgSelect_, true);
}

void SelectScene::DrawStart(void)
{
	Size2D mapSize = Application::GetInstance().GetMapSize();
	Size2D size = START_WSIZE;
	Position2D pos = START_MAP_POS;
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;
	if (!Application::GetInstance().IsWidowMode())size = START_FSIZE;

	switch (inputState_)
	{
	case INPUT::START_TRGDOWN:
		//DrawGraph(pos.x, pos.y, imgStart_[INT_CAST(INPUT::START_TRGDOWN)], true);
		DrawExtendGraph(pos.x, pos.y, pos.x + size.x, pos.y + size.y, imgStart_[INT_CAST(INPUT::START_TRGDOWN)], true);
		break;
	case INPUT::START_TRGUP:
		//DrawGraph(pos.x, pos.y, imgStart_[INT_CAST(INPUT::START_TRGUP)], true);
		DrawExtendGraph(pos.x, pos.y, pos.x + size.x, pos.y + size.y, imgStart_[INT_CAST(INPUT::START_TRGUP)], true);
		break;
	default:
		DrawExtendGraph(pos.x, pos.y, pos.x + size.x, pos.y + size.y, imgStart_[INT_CAST(INPUT::START_TRGUP)], true);
		break;
	}
}

void SelectScene::DrawCharactor(void)
{
	DrawMagician();
	DrawSordMan();
	DrawFighter();
}

void SelectScene::DrawFighter(void)
{
	MV1DrawModel(plTrans1_->modelId);
	int fontSize = 45;
	Size3DF shiftPos = {200.0f, 50.0f};
	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 75;
		shiftPos = {0.0f, 180.0f };
	}
	//render_->Draw();
	if (charaPosShif_.at(INPUT::CHARA_FIGHTER))
	{
		SetFontSize(fontSize);
		DrawFormatString(static_cast<int>(plTrans1_->pos.x + shiftPos.x), static_cast<int>(plTrans1_->pos.y + shiftPos.y),
			0xffffff, "Enter or Y");
		SetFontSize(16);
		plTrans1_->pos = plPos1_;
		plTrans1_->pos = VAdd(plTrans1_->pos, VScale(plTrans1_->GetBackLocal(), 150.0f));
	}
	else
	{
		plTrans1_->pos = plPos1_;
	}

	plTrans1_->Update();
}

void SelectScene::DrawSordMan(void)
{
	MV1DrawModel(plTrans2_->modelId);
	int fontSize = 45;
	Size3DF shiftPos = {450, 60.0f};
	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 75;
		shiftPos = { 530.0f, 180.0f };
	}
	if (charaPosShif_.at(INPUT::CHARA_SORDMAN))
	{
		SetFontSize(fontSize);
		DrawFormatString(static_cast<int>(plTrans1_->pos.x + shiftPos.x), static_cast<int>(plTrans1_->pos.y + shiftPos.y),
			0xffffff, "Enter or Y");
		SetFontSize(16);
		plTrans2_->pos = plPos2_;
		plTrans2_->pos = VAdd(plTrans2_->pos, VScale(plTrans2_->GetBackLocal(), 150.0f));
	}
	else
	{
		plTrans2_->pos = plPos2_;
	}

	plTrans2_->Update();
}

void SelectScene::DrawMagician(void)
{
	MV1DrawModel(plTrans3_->modelId);
	int fontSize = 45;
	Size3DF shiftPos = {650.0f, 60.0f};
	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 75;
		shiftPos = { 1100.0f, 180.0f };
	}
	if (charaPosShif_.at(INPUT::CHARA_MAGICIAN))
	{
		SetFontSize(fontSize);
		DrawFormatString(static_cast<int>(plTrans1_->pos.x + shiftPos.x), static_cast<int>(plTrans1_->pos.y + shiftPos.y),
			0xffffff, "Enter or Y");
		SetFontSize(16);
		plTrans3_->pos = plPos3_;
		plTrans3_->pos = VAdd(plTrans3_->pos, VScale(plTrans3_->GetBackLocal(), 150.0f));
	}
	else
	{
		plTrans3_->pos = plPos3_;
	}

	plTrans3_->Update();
}

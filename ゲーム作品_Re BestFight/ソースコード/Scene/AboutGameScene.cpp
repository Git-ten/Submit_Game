#include<DxLib.h>
#include"Act/Common/Transform.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"../Manager/InputManager.h"
#include"../Utility/MyUtility.h"
#include"Act/Common/AnimationController.h"
#include"../Application.h"
#include "AboutGameScene.h"

#define EXPLAIN_1 (ResourceManager::SRC::EXPLAIN_1)
#define EXPLAIN_2 (ResourceManager::SRC::EXPLAIN_2)
#define EXPLAIN_3 (ResourceManager::SRC::EXPLAIN_3)
#define EXPLAIN_4 (ResourceManager::SRC::EXPLAIN_4)
#define EXPLAIN_4_2 (ResourceManager::SRC::EXPLAIN_4_2)
#define EXPLAIN_4_3 (ResourceManager::SRC::EXPLAIN_4_3)
#define EXPLAIN_5 (ResourceManager::SRC::EXPLAIN_5)
#define EXPLAIN_5_2 (ResourceManager::SRC::EXPLAIN_5_2)
#define EXPLAIN_5_3 (ResourceManager::SRC::EXPLAIN_5_3)
#define EXPLAIN_6 (ResourceManager::SRC::EXPLAIN_6)
#define EXPLAIN_6_2 (ResourceManager::SRC::EXPLAIN_6_2)
#define EXPLAIN_6_3 (ResourceManager::SRC::EXPLAIN_6_3)
#define EXPLAIN_6_4 (ResourceManager::SRC::EXPLAIN_6_4)
#define RETURN (ResourceManager::SRC::RETURN_BUTTON)
#define RETURN_INPUT (ResourceManager::SRC::RETURN_BUTTON_INPUT)
#define RETURN_GAGE (ResourceManager::SRC::RETURN_BUTTGAGE)
#define B_RETURN_GAGE (ResourceManager::SRC::B_RETURN_BUTTGAGE)
#define LEFT_KEY (ResourceManager::SRC::LEFT_KEY)
#define RIGHT_KEY (ResourceManager::SRC::RIGHT_KEY)

AboutGameScene::AboutGameScene(const MODE& mode)
{
	mode_ = mode;
	endFlag_ = false;
	fighterWMovie_ = -1;
	sordmanWMovie_ = -1;
	magicianWMovie_ = -1;
	fighterSMovie_ = -1;
	sordmanSMovie_ = -1;
	magicianSMovie_ = -1;
	fighterSkiMovie_ = -1;
	sordmanSkiMovie_ = -1;
	magicianSkiMovie_ = -1;
	for (int ii = 0; ii < PAGE_MAX; ii++)imgExplain_[ii] = -1;
	imgLeftKey_ = -1;
	imgReturnGage_ = -1;
	for (int jj = 0; jj < 2; jj++)imgReturn_[jj] = -1;
	imgRightKey_ = -1;
	leftFlag_ = false;
	nowPageCounter_ = 0;
	retTitleTime_ = 0.0f;
	returnFlag_ = false;
	rightFlag_ = false;
}

AboutGameScene::~AboutGameScene(void)
{
}

void AboutGameScene::Init(void)
{
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

	//移動キー画像(左)
	imgLeftKey_ = ResourceManager::GetInstance().Load(LEFT_KEY).lock()->handleId_;
	leftFlag_ = true;
	//移動キー画像(右)
	imgRightKey_ = ResourceManager::GetInstance().Load(RIGHT_KEY).lock()->handleId_;
	rightFlag_ = true;

	returnFlag_ = false;

	nowPageCounter_ = 0;

	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION_2).lock();

	endFlag_ = false;

	InitActor();
	InitExplain();
	InitMovie();
}

void AboutGameScene::Update(void)
{
	plAnim1_->Update();
	plAnim2_->Update();
	plAnim3_->Update();

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgUp(KEY_INPUT_A) || ins.IsTrgUp(KEY_INPUT_LEFT) ||
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L) ||
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L))
	{
		nowPageCounter_--;
		leftFlag_ = true;
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
	}
	else if(ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_LEFT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L))
	{
		leftFlag_ = false;
	}

	if (ins.IsTrgUp(KEY_INPUT_D) || ins.IsTrgUp(KEY_INPUT_RIGHT) ||
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R) ||
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R))
	{
		nowPageCounter_++;
		rightFlag_ = true;
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
	}
	else if(ins.IsNew(KEY_INPUT_D) || ins.IsNew(KEY_INPUT_RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R))
	{
		rightFlag_ = false;
	}

	//上限・下限
	if (nowPageCounter_ < 0)
	{
		nowPageCounter_ = 0;
	}
	if (nowPageCounter_ > PAGE_MAX - 1)
	{
		nowPageCounter_ = PAGE_MAX - 1;
	}


	//タイトルシーンに戻る
	if (mode_ == MODE::TITLE_ABOUT_GAME)
	{
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
	else
	{
		if (retTitleTime_ >= KEY_MAX_TIME)
		{
			retTitleTime_ = KEY_MAX_TIME;
			if (ins.IsTrgUp(KEY_INPUT_Z) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT))
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				endFlag_ = true;
			}
		}
		else if (ins.IsNew(KEY_INPUT_Z) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT))
		{
			returnFlag_ = true;
			float delta = SceneManager::GetInstance().GetDeltaTime();
			//長押しして、100%になったらタイトルに戻る
			retTitleTime_ += delta;
		}
		else
		{
			returnFlag_ = false;
			retTitleTime_ = 0.0f;
			endFlag_ = false;
		}
	}
}

void AboutGameScene::Draw(void)
{
	DrawExPlain();
	DrawActor();
	DrawReturn();
	DrawKey();
	DrawPage();
}

void AboutGameScene::Release(void)
{
	DeleteGraph(fighterWMovie_);
	DeleteGraph(fighterSMovie_);
	DeleteGraph(sordmanWMovie_);
	DeleteGraph(sordmanSMovie_);
	DeleteGraph(magicianWMovie_);
	DeleteGraph(magicianSMovie_);
}

void AboutGameScene::OpenAboutGameScene(void)
{
	endFlag_ = false;
	nowPageCounter_ = 0;
	retTitleTime_ = 0.0f;
}

bool AboutGameScene::IsEnd(void)
{
	return endFlag_;
}

void AboutGameScene::InitActor(void)
{
	Size3DF plSize = {3.0f, 3.0f, 3.0f};

	if (!Application::GetInstance().IsWidowMode())plSize = {4.0f, 4.0f, 4.0f };

	//格闘家-------------------------------------------
	plTrans1_ = std::make_unique<Transform>();
	plTrans1_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIST));
	plTrans1_->rot = MyUtility::VECTOR_ZERO;
	plTrans1_->pos = { -300.0f, -200.0f, 800.0f };
	plTrans1_->scl = plSize;
	plTrans1_->quaRot = Quaternion();
	plTrans1_->quaRotLocal = Quaternion();
	plTrans1_->Update();
	plAnim1_ = std::make_unique<AnimationController>(plTrans1_->modelId);
	plAnim1_->ClearAnimMap();
	std::string PATH_ANIM = "Data/Model/Player/Fist/";
	plAnim1_->Add(0, PATH_ANIM + "Idle.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim1_->Play(0);
	//------------------------------------------------

	//剣士--------------------------------------------
	plTrans2_ = std::make_unique<Transform>();
	plTrans2_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_SORD));
	plTrans2_->rot = MyUtility::VECTOR_ZERO;
	plTrans2_->pos = { -300.0f, -200.0f, 800.0f };
	plTrans2_->scl = plSize;
	plTrans2_->quaRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(-10), MyUtility::AXIS_Y);
	plTrans2_->quaRotLocal = Quaternion();
	plTrans2_->Update();
	plAnim2_ = std::make_unique<AnimationController>(plTrans2_->modelId);
	plAnim2_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Sord/";
	plAnim2_->Add(0, PATH_ANIM + "Idle_β.mv1", 20.0f, 0.0f, 30.0f, true);
	plAnim2_->Play(0);
	//------------------------------------------------

	//魔術師--------------------------------------------
	plTrans3_ = std::make_unique<Transform>();
	plTrans3_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGIC));
	plTrans3_->rot = MyUtility::VECTOR_ZERO;
	plTrans3_->pos = { -300.0f, -200.0f, 800.0f };
	plTrans3_->scl = plSize;
	plTrans3_->quaRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(0), MyUtility::AXIS_Y);
	plTrans3_->quaRotLocal = Quaternion();
	plTrans3_->Update();
	plAnim3_ = std::make_unique<AnimationController>(plTrans3_->modelId);
	plAnim3_->ClearAnimMap();
	PATH_ANIM = "Data/Model/Player/Magic/";
	plAnim3_->Add(0, PATH_ANIM + "Idle_Γ.mv1", 20.0f, 0.0f, 35.0f, true);
	plAnim3_->Play(0);
	//------------------------------------------------
}

void AboutGameScene::InitExplain(void)
{
	imgExplain_[0] = ResourceManager::GetInstance().Load(EXPLAIN_1).lock()->handleId_;
	imgExplain_[1] = ResourceManager::GetInstance().Load(EXPLAIN_2).lock()->handleId_;
	imgExplain_[2] = ResourceManager::GetInstance().Load(EXPLAIN_3).lock()->handleId_;
	imgExplain_[3] = ResourceManager::GetInstance().Load(EXPLAIN_4).lock()->handleId_;
	imgExplain_[4] = ResourceManager::GetInstance().Load(EXPLAIN_4_2).lock()->handleId_;
	imgExplain_[5] = ResourceManager::GetInstance().Load(EXPLAIN_4_3).lock()->handleId_;
	imgExplain_[6] = ResourceManager::GetInstance().Load(EXPLAIN_5).lock()->handleId_;
	imgExplain_[7] = ResourceManager::GetInstance().Load(EXPLAIN_5_2).lock()->handleId_;
	imgExplain_[8] = ResourceManager::GetInstance().Load(EXPLAIN_5_3).lock()->handleId_;
	imgExplain_[9] = ResourceManager::GetInstance().Load(EXPLAIN_6).lock()->handleId_;
	imgExplain_[10] = ResourceManager::GetInstance().Load(EXPLAIN_6_2).lock()->handleId_;
	imgExplain_[11] = ResourceManager::GetInstance().Load(EXPLAIN_6_3).lock()->handleId_;
	imgExplain_[12] = ResourceManager::GetInstance().Load(EXPLAIN_6_4).lock()->handleId_;
}

void AboutGameScene::InitMovie(void)
{
	const std::string MOVIE = "Data/Movie/";
	
	//格闘家
	std::string path = MOVIE + "Fighter_Week.mp4";
	fighterWMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Fighter_Strong.mp4";
	fighterSMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Fighter_Skill.mp4";
	fighterSkiMovie_ = LoadGraph(path.c_str());

	//剣士
	path = MOVIE + "Sordman_Week.mp4";
	sordmanWMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Sordman_Strong.mp4";
	sordmanSMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Sordman_Skill.mp4";
	sordmanSkiMovie_ = LoadGraph(path.c_str());

	//魔術師
	path = MOVIE + "Magician_Week.mp4";
	magicianWMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Magician_Strong.mp4";
	magicianSMovie_ = LoadGraph(path.c_str());
	path = MOVIE + "Magician_Skill.mp4";
	magicianSkiMovie_ = LoadGraph(path.c_str());
}

void AboutGameScene::DrawActor(void)
{
	//現在のページが4ページ目
	if (nowPageCounter_ == 3)
	{
		//格闘家
		MV1DrawModel(plTrans1_->modelId);
	}
	//現在のページが7ページ目
	if (nowPageCounter_ == 6)
	{
		//剣士
		MV1DrawModel(plTrans2_->modelId);
	}
	//現在のページが8ページ目
	if (nowPageCounter_ == 9)
	{
		//魔術師
		MV1DrawModel(plTrans3_->modelId);
	}
}

void AboutGameScene::DrawExPlain(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawExtendGraph(0, 0, screenSize.x, screenSize.y, imgExplain_[nowPageCounter_], true);
}

void AboutGameScene::DrawReturn(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D pos = { 30, screenSize.y - 64 };
	Size2D size = { 45, 45 };
	int fontSize = 32;
	if (!Application::GetInstance().IsWidowMode())
	{
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
	if (Application::GetInstance().IsMouseFlag())DrawFormatString(pos.x + 8, pos.y + fontSize / 2 - 4, 0xbb0000, "Z");
	else DrawFormatString(pos.x + 8, pos.y + fontSize / 2 - 4, 0xbb0000, "B");
	SetDrawBright(255, 255, 255);
	if (mode_ == MODE::TITLE_ABOUT_GAME)DrawFormatString(pos.x + 8 + fontSize, pos.y + fontSize / 2 - 4, 0xffffff, "Return Title");
	else DrawFormatString(pos.x + 8 + fontSize, pos.y + fontSize / 2 - 4, 0xffffff, "Return");
	SetFontSize(16);
}

void AboutGameScene::DrawKey(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D posL = { 0, screenSize.y / 4 };
	Position2D posR = { screenSize.x, screenSize.y / 4 };
	Size2D size = {64, 64};
	int fontSize = 64;

	if (!Application::GetInstance().IsWidowMode())
	{
		size.x *= 2;
		size.y *= 2;
		fontSize *= 2;
	}

	if (!leftFlag_) SetDrawBright(255,255,255);
	else SetDrawBright(0, 0, 0);

	DrawExtendGraph(posL.x, posL.y, posL.x + size.x, posL.y + size.y, imgLeftKey_, true);

	if(!rightFlag_) SetDrawBright(255, 255, 255);
	else SetDrawBright(0, 0, 0);

	DrawExtendGraph(posR.x - size.x, posR.y, posR.x, posR.y + size.y, imgRightKey_, true);

	SetDrawBright(255, 255, 255);

	int color = 0xffffff;
	if (Application::GetInstance().IsMouseFlag())
	{
		SetFontSize(fontSize);
		if (!leftFlag_)color = 0x000000;
		DrawFormatString(posL.x, posL.y + 5, color, "←");
		color = 0xffffff;
		if (!rightFlag_)color = 0x000000;
		DrawFormatString(posR.x - size.x, posR.y + 5, color, "→");
		color = 0xffffff;
	}

	else
	{
		SetFontSize(fontSize / 2);
		if (!leftFlag_)color = 0x000000;
		DrawFormatString(posL.x + fontSize / 3, posL.y + fontSize / 3, color, "LB");
		color = 0xffffff;
		if (!rightFlag_)color = 0x000000;
		DrawFormatString(posR.x + fontSize / 3 - size.x, posR.y + fontSize / 3, color, "RB");
		color = 0xffffff;
	}
	SetFontSize(16);
}

void AboutGameScene::DrawPage(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D pos = { screenSize.x - 250, screenSize.y - 43 };
	int fontSize = 45;
	if (!Application::GetInstance().IsWidowMode())
	{
		pos.x -= 170;
		pos.y -= 30;
		fontSize = 90;
	}

	DrawMovie();

	SetFontSize(fontSize);
	DrawFormatString(pos.x, pos.y, 0x000000, "%d / %d", nowPageCounter_ + 1, PAGE_MAX);
	SetFontSize(16);
}

void AboutGameScene::DrawMovie(void)
{
	Position2D pos = MOVIE_WEEK_POS;
	Size2D movieSize = { MOVIE_SIZE.x - pos.x, MOVIE_SIZE.y - pos.y};
	Size2D mapSize = Application::GetInstance().GetMapSize();
	int shift = 16;

	pos.x *= mapSize.x;
	pos.y *= mapSize.y;
	movieSize.x *= mapSize.x;
	movieSize.y *= mapSize.y;

	//現在のページが5ページ目
	if (nowPageCounter_ == 4)
	{
		if (!GetMovieStateToGraph(fighterWMovie_))
		{
			PlayMovieToGraph(fighterWMovie_, DX_PLAYTYPE_LOOP);
		}
		if (!GetMovieStateToGraph(fighterSMovie_))
		{
			PlayMovieToGraph(fighterSMovie_, DX_PLAYTYPE_LOOP);
		}

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, fighterWMovie_, true);
		pos.y += movieSize.y;
		DrawExtendGraph(pos.x, pos.y + shift, pos.x + movieSize.x, pos.y + shift + movieSize.y, fighterSMovie_, true);
	}
	//現在のページが6ページ目
	else if (nowPageCounter_ == 5)
	{
		if (!GetMovieStateToGraph(fighterSkiMovie_))
		{
			PlayMovieToGraph(fighterSkiMovie_, DX_PLAYTYPE_LOOP);
		}
		movieSize.x += mapSize.x * 2;
		movieSize.y += mapSize.y * 2;

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, fighterSkiMovie_, true);
	}
	//現在のページが8ページ目
	else if (nowPageCounter_ == 7)
	{
		if (!GetMovieStateToGraph(sordmanWMovie_))
		{
			PlayMovieToGraph(sordmanWMovie_, DX_PLAYTYPE_LOOP);
		}
		if (!GetMovieStateToGraph(sordmanSMovie_))
		{
			PlayMovieToGraph(sordmanSMovie_, DX_PLAYTYPE_LOOP);
		}

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, sordmanWMovie_, true);
		pos.y += movieSize.y;
		DrawExtendGraph(pos.x, pos.y + shift, pos.x + movieSize.x, pos.y + shift + movieSize.y, sordmanSMovie_, true);
	}
	//現在のページが9ページ目
	else if (nowPageCounter_ == 8)
	{
		if (!GetMovieStateToGraph(sordmanSkiMovie_))
		{
			PlayMovieToGraph(sordmanSkiMovie_, DX_PLAYTYPE_LOOP);
		}
		movieSize.x += mapSize.x * 2;
		movieSize.y += mapSize.y * 2;

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, sordmanSkiMovie_, true);
	}
	//現在のページが10ページ目
	else if (nowPageCounter_ == 10)
	{
		if (!GetMovieStateToGraph(magicianWMovie_))
		{
			PlayMovieToGraph(magicianWMovie_, DX_PLAYTYPE_LOOP);
		}
		if (!GetMovieStateToGraph(magicianSMovie_))
		{
			PlayMovieToGraph(magicianSMovie_, DX_PLAYTYPE_LOOP);
		}

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, magicianWMovie_, true);
		pos.y += movieSize.y;
		DrawExtendGraph(pos.x, pos.y + shift, pos.x + movieSize.x, pos.y + shift + movieSize.y, magicianSMovie_, true);
	}
	//現在のページが11ページ目
	else if (nowPageCounter_ == 11)
	{
		if (!GetMovieStateToGraph(magicianSkiMovie_))
		{
			PlayMovieToGraph(magicianSkiMovie_, DX_PLAYTYPE_LOOP);
		}
		movieSize.x += mapSize.x * 2;
		movieSize.y += mapSize.y * 2;

		DrawExtendGraph(pos.x, pos.y, pos.x + movieSize.x, pos.y + movieSize.y, magicianSkiMovie_, true);
	}
	else if(4 <= nowPageCounter_ && nowPageCounter_ > 11)
	{
		PauseMovieToGraph(fighterWMovie_);
		PauseMovieToGraph(fighterSMovie_);
		PauseMovieToGraph(fighterSkiMovie_);
		PauseMovieToGraph(sordmanWMovie_);
		PauseMovieToGraph(sordmanSMovie_);
		PauseMovieToGraph(magicianWMovie_);
		PauseMovieToGraph(magicianSMovie_);
		PauseMovieToGraph(magicianSkiMovie_);
	}
}
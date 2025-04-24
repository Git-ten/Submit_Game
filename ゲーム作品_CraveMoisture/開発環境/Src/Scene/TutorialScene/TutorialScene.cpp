#include "../../ACTOR/Player/Player.h"
#include "../../ACTOR/Enemy/EnemyBear.h"
#include "../../ACTOR/Stage/StageGround.h"
#include "../../ACTOR/SkyDome/SkySun.h"
#include "../../Common/Collider.h"
#include "../../APPLICATION/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void) :
	GameScene(),
	isEndTutorial_(false),
	nowTutorialStep_(0),
	maxTutorialStep_(0),
	tutorialCounter_(0),
	tutorialMaxCounter_(0)
{
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::StartTutorial(void)
{
	isGuiDraw_ = false;
	PlayGameScene();
	SceneManager::GetInstance().GetCamera().lock()->UnLockIsCameraUpdate();
	if (movieTutorial_[nowTutorialStep_] != nullptr)
		movieTutorial_[nowTutorialStep_]->Stop();
}

void TutorialScene::SetUpTutorial(LoadData::LoadDataInfo info, ResourceManager::RESOURCE_ID imgId,
	ResourceManager::RESOURCE_ID movieId, ResourceManager::RESOURCE_ID imgExplainId,
	TUTORIAL_UI uiId, std::function<void(void)> func)
{
	if (imgId != ResourceManager::RESOURCE_ID::NONE)
		imgTutorial_.push_back(std::make_unique<Image>(info, imgId));
	else
		imgTutorial_.push_back(nullptr);


	info.dataName = "TutorialMovie";
	if(movieId != ResourceManager::RESOURCE_ID::NONE)
		movieTutorial_.push_back(std::make_unique<Movie>(info, movieId));
	else
		movieTutorial_.push_back(nullptr);


	info.dataName = "TutorialExplain";
	if (imgExplainId != ResourceManager::RESOURCE_ID::NONE)
		imgTutorialExplain_.push_back(std::make_unique<Image>(info, imgExplainId));
	else
		imgTutorialExplain_.push_back(nullptr);


	tutorialFunc_.push_back(func);

	tutorialUi_.push_back(uiId);

	maxTutorialStep_++;
}

void TutorialScene::InitAddSpecial(void)
{
	isGuiDraw_ = true;

	LoadData::LoadDataInfo info = { "TutorialSceneInfo.json", "Tutorial"};

	TUTORIAL_UI uiId = TUTORIAL_UI::NONE;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_UI,
		ResourceManager::RESOURCE_ID::NONE,
		ResourceManager::RESOURCE_ID::NONE,
		uiId,
		[&]() { PlayBgm();
			    NextStepTutorial();});

	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::MOUSE;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA,
		ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_EXPLAIN,
		uiId,
		[&]() { TutorialCamera(); });

	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_WASD;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_MOVE,
		ResourceManager::RESOURCE_ID::TUTORIAL_MOVE_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_MOVE_EXPLAIN,
		uiId,
		[&]() { TutorialMove(); });
	
	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_SPACE;
	else
		uiId = TUTORIAL_UI::A_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_JUMP,
		ResourceManager::RESOURCE_ID::TUTORIAL_JUMP_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_JUMP_EXPLAIN,
		uiId,
		[&]() { TutorialJump(); });
	
	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::MOUSE;
	else
		uiId = TUTORIAL_UI::B_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_AVOID,
		ResourceManager::RESOURCE_ID::TUTORIAL_AVOID_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_AVOID_EXPLAIN,
		uiId,
		[&]() { TutorialAvoid(); });
	
	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::MOUSE;
	else
		uiId = TUTORIAL_UI::X_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK,
		ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK_EXPLAIN,
		uiId,
		[&]() { TutorialAttack(); });
	
	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_ALL;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_DASH,
		ResourceManager::RESOURCE_ID::TUTORIAL_DASH_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_DASH_EXPLAIN,
		uiId,
		[&]() { TutorialDash(); });
	
	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE,
		ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE_EXPLAIN,
		uiId,
		[&]() { TutorialCharge(); });
	
	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL,
		ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL_EXPLAIN,
		uiId,
		[&]() { TutorialSpecial(); });
	
	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK,
		ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK_EXPLAIN,
		uiId,
		[&]() { TutorialJustAvoidAttack(); });

	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_R;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE,
		ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE_EXPLAIN,
		uiId,
		[&]() { TutorialTechnique(); });

	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_Q;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_SKILL,
		ResourceManager::RESOURCE_ID::TUTORIAL_SKILL_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_SKILL_EXPLAIN,
		uiId,
		[&]() { TutorialSkill(); });
	
	if (InputManager::GetInstance().GetJPadNum() <= 0)
		uiId = TUTORIAL_UI::KEY_ALL;
	else
		uiId = TUTORIAL_UI::CONTOROLLER_BUTTON;

	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE,
		ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE_MOVIE,
		ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE_EXPLAIN,
		uiId,
		[&]() { TutorialBattle(); });
	
	SetUpTutorial(info, ResourceManager::RESOURCE_ID::TUTORIAL_END,
		ResourceManager::RESOURCE_ID::NONE,
		ResourceManager::RESOURCE_ID::NONE,
		TUTORIAL_UI::NONE,
		[&]() { NextStepTutorial(); });


	info.dataName = "DeleteButton";

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[]() { return InputManager::GetInstance().IsPadBtnTrgUp(
			InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		InputManager::GetInstance().IsTrgUp(KEY_INPUT_ESCAPE);},
		[&]() { StartTutorial(); });

	guiDeleteButton_ = std::make_unique<ImageGUI>(
		[]() { return InputManager::GetInstance().IsTrgUpMouseLeft(); },
		[&]() { StartTutorial(); });
	guiDeleteButton_->SetImage(info, ResourceManager::RESOURCE_ID::DELETE_TUTORIAL);
	guiDeleteButton_->AddCondition([&]() { return Collider::CollisionRectToMouse(guiDeleteButton_->GetCollPlane(),
		InputManager::GetInstance().GetMousePos()); });

	// 数字画像
	info.dataName = "NumberMax";
	imgNumbers_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS);

	info.dataName = "Number";
	std::unique_ptr<ImageDiv>temp = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS);
	numbersPos_ = temp->GetPos();

	// 記号画像
	info.dataName = "SymBol";
	imgSymbols_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::SYMBOLS);


	info.dataName = "TutorialMouse";
	imgMouse_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_MOUSE);

	info.dataName = "Key";
	imgWASD_Key_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_WASD_KEY);

	info.dataName = "Key";
	imgR_Key_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_R_KEY);

	imgQ_Key_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_Q_KEY);

	info.dataName = "Space_Key";
	imgSpace_Key_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_SPACE_KEY);

	info.dataName = "AllKey";
	imgAll_Key_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_ALL_KEY);


	info.dataName = "Button";
	imgContoroller_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_CONTOROLLER);
	imgX_Button_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_X_BUTTON);
	imgA_Button_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_A_BUTTON);
	imgB_Button_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_B_BUTTON);
	imgD_Pad_Button_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_D_PAD);

	StopGameScene();
}

void TutorialScene::UpdateAddSpecial(void)
{
	if (nowTutorialStep_ >= maxTutorialStep_)
	{
		nextSceneId_ = SceneID::TITLE;
		return;
	}

	if (isGuiDraw_)
		guiDeleteButton_->UpdateGUI();
	else
	{
		if (tutorialFunc_[nowTutorialStep_] != nullptr)
			tutorialFunc_[nowTutorialStep_]();
	}
}

void TutorialScene::SetUpActors(void)
{
	// 必ず、ACTOR_IDの順番
	actors_.reserve(ALL_ACTOR_NUM);
	actors_.push_back(std::make_unique<Player>());
	actors_.push_back(std::make_unique<EnemyBear>());
	actors_.push_back(std::make_unique<StageGround>());
	actors_.push_back(std::make_unique<SkySun>());
}

void TutorialScene::InitBattleManager(void)
{
	// バトルマネージャーに条件式を追加
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::NONE_BATTLE),
		[]() { return false; });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_START),
		[]() { return false; });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE),
		[&]() { return false; });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_WIN),
		[&]() { return false; });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_GAMEOVER),
		[]() { return false; });
}

void TutorialScene::DrawAddSpecial(void)
{
	if (nowTutorialStep_ >= maxTutorialStep_)return;
	if (!isGuiDraw_)
	{
		if (imgTutorialExplain_[nowTutorialStep_] != nullptr)
			imgTutorialExplain_[nowTutorialStep_]->DrawImage();

		DrawTutorialCounter();

		return;
	}

	SetMouseDispFlag(true);


	if(imgTutorial_[nowTutorialStep_] != nullptr)
		imgTutorial_[nowTutorialStep_]->DrawImage();


	if(movieTutorial_[nowTutorialStep_] != nullptr)
		movieTutorial_[nowTutorialStep_]->DrawMovie();

	DrawTutorialUi();

	guiDeleteButton_->DrawGUI();
}

void TutorialScene::DrawTutorialCounter(void)
{
	if (tutorialMaxCounter_ <= 0)return;

	imgNumbers_->DrawImageDiv(tutorialCounter_, numbersPos_);
	imgSymbols_->DrawImageDiv(1);
	imgNumbers_->DrawImageDiv(tutorialMaxCounter_);
}

void TutorialScene::DrawTutorialUi(void)
{
	if (nowTutorialStep_ >= maxTutorialStep_)return;

	switch (tutorialUi_[nowTutorialStep_])
	{
	case TUTORIAL_UI::MOUSE:
		imgMouse_->DrawImage();
		break;
	case TUTORIAL_UI::KEY_WASD:
		imgWASD_Key_->DrawImage();
		break;
	case TUTORIAL_UI::KEY_R:
		imgR_Key_->DrawImage();
		break;
	case TUTORIAL_UI::KEY_Q:
		imgQ_Key_->DrawImage();
		break;
	case TUTORIAL_UI::KEY_SPACE:
		imgSpace_Key_->DrawImage();
		break;
	case TUTORIAL_UI::KEY_ALL:
		imgAll_Key_->DrawImage();
		break;
	case TUTORIAL_UI::X_BUTTON:
		imgX_Button_->DrawImage();
		break;
	case TUTORIAL_UI::A_BUTTON:
		imgA_Button_->DrawImage();
		break;
	case TUTORIAL_UI::B_BUTTON:
		imgB_Button_->DrawImage();
		break;
	case TUTORIAL_UI::D_PAD_BUTTON:
		imgD_Pad_Button_->DrawImage();
		break;
	case TUTORIAL_UI::CONTOROLLER_BUTTON:
		imgContoroller_->DrawImage();
		break;
	default:
		break;
	}
}

void TutorialScene::NextStepTutorial(void)
{
	StopGameScene();

	SceneManager::GetInstance().GetCamera().lock()->LockIsCameraUpdate();

	// カウンターを初期化
	tutorialCounter_ = 0;
	tutorialMaxCounter_ = 0;

	isGuiDraw_ = true;
	nowTutorialStep_++;

	auto& input = InputManager::GetInstance();
	input.SetMousePos(Application::GetInstance().GetScreenCenter());

	if (nowTutorialStep_ >= maxTutorialStep_)return;

	if(movieTutorial_[nowTutorialStep_] != nullptr)
		movieTutorial_[nowTutorialStep_]->Play(DX_PLAYTYPE_LOOP);
}

void TutorialScene::TutorialCamera(void)
{
	if (pause_->IsOpen())return;

	const float MAX_SUM_ANGLE = 200.0f;
	static Rot3DF prevAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();
	static float counter = 0.0f;


	Rot3DF subAngle = VSub(prevAngle,
		SceneManager::GetInstance().GetCamera().lock()->GetAngle());

	// 正負逆転
	if (subAngle.x < 0.0f)
		subAngle.x *= -1.0f;
	// 正負逆転
	if (subAngle.y < 0.0f)
		subAngle.y *= -1.0f;
	// 正負逆転
	if (subAngle.z < 0.0f)
		subAngle.z *= -1.0f;

	counter += subAngle.x + subAngle.y + subAngle.z;

	if (counter > MAX_SUM_ANGLE)
	{
		counter = 0.0f;
		prevAngle = Rot3DF();

		NextStepTutorial();
	}
}

void TutorialScene::TutorialMove(void)
{
	const float MAX_SUM_DISTANCE = 8000.0f;
	static float sumDistance = 0.0f;

	static Position3DF prevPos = actors_[0]->GetPos();
	Position3DF nowPos = actors_[0]->GetPos();

	sumDistance += VSize(VSub(prevPos, nowPos));
	prevPos = actors_[0]->GetPos();

	if (sumDistance > MAX_SUM_DISTANCE)
	{
		sumDistance = 0.0f;
		prevPos = Position3DF();
		NextStepTutorial();
	}
}

void TutorialScene::TutorialJump(void)
{
	tutorialMaxCounter_ = 2;
	static bool prevFlag = false;

	auto& input = SimpleInput::GetInstance();

	if (input.GetInput("jump") && !actors_[0]->IsTouchGround() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (actors_[0]->IsTouchGround())
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialAvoid(void)
{
	tutorialMaxCounter_ = 2;

	static bool prevFlag = false;

	auto& input = SimpleInput::GetInstance();

	if (input.GetInput("avoid") && actors_[0]->IsInertia() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsInertia())
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialAttack(void)
{
	tutorialMaxCounter_ = 6;

	static bool prevFlag = false;

	static float prevHp = actors_[1]->NowHp();
	float nowHp = actors_[1]->NowHp();

	auto& input = SimpleInput::GetInstance();

	if (input.GetInput("normalAttack") && nowHp != prevHp && !prevFlag)
	{
		prevFlag = true;
		prevHp = nowHp;
		tutorialCounter_++;
	}
	else if (prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialDash(void)
{
	tutorialMaxCounter_ = 2;

	static bool prevFlag = false;

	if (actors_[0]->IsDash() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsDash() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialCharge(void)
{
	tutorialMaxCounter_ = 3;

	static bool prevFlag = false;

	if (actors_[0]->IsCharge() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsCharge() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialTechnique(void)
{
	tutorialMaxCounter_ = 1;

	static bool prevFlag = false;

	if (actors_[0]->IsTechnique() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsTechnique() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialSpecial(void)
{
	tutorialMaxCounter_ = 3;

	static bool prevFlag = false;

	if (actors_[0]->IsParysOrJustAvoid() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsParysOrJustAvoid() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialJustAvoidAttack(void)
{
	tutorialMaxCounter_ = 1;

	static bool prevFlag = false;

	if (actors_[0]->IsCharge() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsCharge() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialSkill(void)
{
	tutorialMaxCounter_ = 1;

	static bool prevFlag = false;

	if (actors_[0]->IsSkill() && !prevFlag)
	{
		prevFlag = true;
		tutorialCounter_++;
	}
	else if (!actors_[0]->IsSkill() && prevFlag)
	{
		prevFlag = false;

		if (tutorialCounter_ >= tutorialMaxCounter_)
			NextStepTutorial();
	}
}

void TutorialScene::TutorialBattle(void)
{
	if (actors_[1]->NowHp() <= 0.0f)
		NextStepTutorial();
}
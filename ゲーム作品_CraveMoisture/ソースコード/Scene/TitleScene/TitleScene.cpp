#include <string>
#include "../../Utility/Utility.h"
#include "../../Common/Collider.h"
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Application/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/InputManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "TitleScene.h"

#pragma region 定数宣言

// 透明度の最大値
const float ALPHA_MAX = 0.6f;

// 透明度の最小値
const float ALPHA_MIN = 0.0f;

// PixcelRendererの枚数
const int PIXCELRENDERER_NUM = 2;

// 定数バッファのインデックス(蜃気楼)
const int CONST_BUF_INDEX = 0;

#pragma endregion

TitleScene::TitleScene(void)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::LoadSceneData(void)
{
	LoadData::LoadDataInfo info = {"TitleSceneInfo.json", "ShutDown"};


	// GUIの登録
	if (InputManager::GetInstance().GetJPadNum() <= 0)
	{
		SetUpGUI(info, ResourceManager::RESOURCE_ID::SHUT_DOWN,
			[]() { return SimpleInput::GetInstance().GetInput("shutDown"); },
			[]() {SceneManager::GetInstance().EndGame(); });

		AddGUICondition(0, [&]() { return Collider::CollisionRectToMouse(guiImages_[0]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos());});
	}
	else
		SetUpGUI(info, ResourceManager::RESOURCE_ID::SHUT_DOWN,
			[]() { return SimpleInput::GetInstance().GetInput("shutDownContoroller"); },
			[]() {SceneManager::GetInstance().EndGame(); });


	info.dataName = "TitleBack";
	imgBack_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TITLE_BACK);
	
	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[]() { return SimpleInput::GetInstance().GetInput("anyButton") &&
		!(SimpleInput::GetInstance().GetInput("moveLeft") ||
			SimpleInput::GetInstance().GetInput("moveRight")); },
		[&]() {titleMenyu_->SetMenyuIsOpen(!titleMenyu_->IsOpen());
			   InputManager::GetInstance().SetMousePos(titleMenyu_->GetPos()); });

	AddGUICondition(1, [&]() {
		if (!titleMenyu_->IsOpen())
			return Collider::CollisionRectToMouse(guiImages_[1]->GetCollPlane(),
				InputManager::GetInstance().GetMousePos());
		else
			return !titleMenyu_->IsCollMenyu() && Collider::CollisionRectToMouse(guiImages_[1]->GetCollPlane(),
				InputManager::GetInstance().GetMousePos());});



	info.dataName = "TitleName";
	imgTitleName_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TITLE_NAME);
	info.dataName = "PushAnyButton";
	imgPushAnyButton_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::PUSH_ANY_BUTTON);
	
	info.dataName = "ShutDownKey";
	imgShutDownKey_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SHUT_DOWN_DELETE);
}

void TitleScene::InitSpecial(void)
{
	isGuiDraw_ = true;

	titleMenyu_ = std::make_unique<TitleMenyu>();
	titleMenyu_->Init();
}

void TitleScene::UpdateSpecial(void)
{
	imgPushAnyButton_->AnimationImage([&]() { PushAnyButtonAnimation(); });

	if (titleMenyu_->IsOpen())
		titleMenyu_->Update();
}

void TitleScene::InitPixcelRenderer(void)
{
	const Size2D& screenSize = Application::GetInstance().GetScreenSize();

	// 背景画像のレンダリング
	pixcelRenderer_ = std::make_unique<PixelRenderer>(PIXCELRENDERER_NUM, screenSize);
	// ポリゴン作成
	pixcelRenderer_->MakeSquereVertex(Utility::VECTOR2D_ZERO, screenSize);
	pixcelRenderer_->AddShaderInfo(PixelRenderer::SHADER_NAME::MIRAGE,
		PixelRenderer::BUF_SIZE_MIRAGE);

	pixcelRenderer_->AddConstBuf(PixelRenderer::SHADER_NAME::MIRAGE,
		FLOAT4(PixelRenderer::SHADER_NONE_VALUE,
			   PixelRenderer::SHADER_NONE_VALUE, 
			   PixelRenderer::SHADER_NONE_VALUE,
			   PixelRenderer::SHADER_NONE_VALUE));

	pixcelRenderer_->AddShaderInfo(PixelRenderer::SHADER_NAME::BRACK_OUT,
		PixelRenderer::BUF_SIZE_ZERO);


	pixcelTime_ = std::make_unique<Time>();
}

void TitleScene::UpdatePixcelRenderer(void)
{
	pixcelTime_->UpdateTime();

	pixcelRenderer_->SetConstBuf(PixelRenderer::SHADER_NAME::MIRAGE,
		CONST_BUF_INDEX,
		FLOAT4(pixcelTime_->GetTotalNowTime(),
			   PixelRenderer::SHADER_NONE_VALUE,
			   PixelRenderer::SHADER_NONE_VALUE,
			   PixelRenderer::SHADER_NONE_VALUE));
}

void TitleScene::InitSoundPlayLaunch(void)
{
	soundPlayLaunch_ = std::make_unique<SoundPlayLaunch>();

	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::TITLE;
	soundPlayLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);
	PlayBgm();
}

void TitleScene::PlayBgm(void)
{
	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::TITLE;

	soundPlayLaunch_->Play(id, DX_PLAYTYPE_LOOP);
}

void TitleScene::PushAnyButtonAnimation(void)
{
	static float nowAlpha_ = ALPHA_MIN;
	nowAlpha_ += SceneManager::GetInstance().GetDeltaTime();

	const float ANIMATION_COUNTER_MAX = 3.0f;

	//アルファ値の制限
	if (nowAlpha_ > ANIMATION_COUNTER_MAX)
	{
		nowAlpha_ = ALPHA_MIN;
	}


	float s = sinf(nowAlpha_);
	if (s < ALPHA_MAX)s = ALPHA_MAX;
	imgPushAnyButton_->SetAlpha(s);
}

void TitleScene::DrawSpecial(void)
{
	// TitleNameの描画
	DrawTitleName();

	// PushAnyButtonの描画
	DrawPushAnyButton();

	imgShutDownKey_->DrawImage();

	titleMenyu_->Draw();
}

void TitleScene::DrawBack(void)
{
	imgBack_->DrawImage();
}

void TitleScene::DrawTitleName(void)
{
	imgTitleName_->DrawImage();
}

void TitleScene::DrawPushAnyButton(void)
{
	imgPushAnyButton_->DrawImage();
}
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void):
	isGuiDraw_(false),
	nextSceneId_(SceneID::SCENE_ID::NONE)
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
	LoadSceneData();
	InitSoundPlayLaunch();

	InitPixcelRenderer();

	InitSpecial();
}

void SceneBase::Update(void)
{
	if (SceneTransition())return;

	UpdateShadowShader();

	// ポストエフェクトの更新
	UpdatePixcelRenderer();

	if (isGuiDraw_)
	{
		for (const auto& gui : guiImages_)
			gui->UpdateGUI();
	}

	UpdateSpecial();
}

void SceneBase::Draw(void)
{
	DrawBack();

	DrawPixcelRenderer();

	if (isGuiDraw_)
	{
		int size = static_cast<int>(guiImages_.size());
		for (int ii = size - 1; ii >= 0; ii--)
			guiImages_[ii]->DrawGUI();
	}

	DrawSpecial();
}

void SceneBase::Release(void)
{
}

void SceneBase::InitNumbersImage(LoadData::LoadDataInfo info)
{
	info.dataName = "Numbers";
	imgNumber_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS);
	// ダメージ用の数字データをロード
	// 画像が上記と同じなため、座標情報のみ登録
	info.dataName = "DamageNumbers";
	std::unique_ptr<ImageDiv>temp = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS);
	damageNumberPos_ = temp->GetPos();
	temp.reset();

	info.dataName = "Symbols";
	imgResultSymbol_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::SYMBOLS);
}

void SceneBase::InitSpecial(void)
{
}

void SceneBase::InitPixcelRenderer(void)
{
}

void SceneBase::InitSoundPlayLaunch(void)
{
}

void SceneBase::PlayBgm(void)
{
}

void SceneBase::SetUpGUI(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id,
	const std::function<bool(void)>& conditionFunc, const std::function<void(void)>& runFunc)
{
	std::unique_ptr<ImageGUI>imageGui = std::make_unique<ImageGUI>(conditionFunc, runFunc);

	imageGui->SetImage(info, id);

	guiImages_.push_back(std::move(imageGui));
}

void SceneBase::AddGUICondition(const int number, const std::function<bool(void)>& conditionFunc)
{
	if (number < 0)									  return;
	if (number >= static_cast<int>(guiImages_.size()))return;

	guiImages_[number]->AddCondition(conditionFunc);
}

void SceneBase::UpdatePixcelRenderer(void)
{
}

void SceneBase::DrawPixcelRenderer(void)
{
	if (pixcelRenderer_ != nullptr)pixcelRenderer_->Draw();
}

void SceneBase::InitShadowShader(void)
{
}

void SceneBase::UpdateShadowShader(void)
{
	if (shadowVertex_ == nullptr)return;

	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	shadowVertex_->SetConstBufPS(4, FLOAT4(
		ModelRenderer::DIRECTION_LIGHT_POWER_MEDIUM,
		cameraPos.x,
		cameraPos.y,
		cameraPos.z));
}

void SceneBase::LoadSceneData(void)
{
}

void SceneBase::UpdateSpecial(void)
{
}

void SceneBase::DrawBack(void)
{
}

void SceneBase::DrawSpecial(void)
{
}

void SceneBase::DrawTime(const float param)
{
	if (imgNumber_ == nullptr)return;

	Position2D pos = imgNumber_->GetPos();
	int time = static_cast<int>(param);
	//オフセット
	int offSet = imgNumber_->GetSize().x;


	//時間の表示------------------------------------------

	//秒-------------------------------

	//一の位
	imgNumber_->DrawImageDiv((time % 60) % 10, pos);

	pos.x -= offSet;

	//十の位
	imgNumber_->DrawImageDiv((time % 60) / 10, pos);
	//---------------------------------


	pos.x -= offSet;

	//記号「:」
	imgResultSymbol_->DrawImageDiv(8, pos);
	//分-------------------------------

	pos.x -= offSet;

	//一の位
	imgNumber_->DrawImageDiv((time / 60) % 10, pos);

	pos.x -= offSet;

	//十の位
	imgNumber_->DrawImageDiv((time / 60) / 10, pos);
	//---------------------------------

	//----------------------------------------------------
}

void SceneBase::DrawDamage(const float param)
{
	if (imgNumber_ == nullptr)return;

	//現在のダメージ量
	int damage = static_cast<int>(param);
	if (damage > 99999)
	{
		damage = 99999;
	}

	const int DIGIT_NEXT = 10;

	//ダメージの最大桁を計測
	int damageDigits = 1;

	// 桁数が二つ以上か
	while ((damage / damageDigits) > 9)
		damageDigits *= DIGIT_NEXT;



	//オフセット
	int offSet = imgNumber_->GetSize().x;
	// 座標
	Position2D pos = damageNumberPos_;

	//ダメージ量を一の位から最上位まで描画
	while (damageDigits > 0)
	{
		int num = damage / damageDigits;
		if (damageDigits > 1)num = damage % damageDigits;

		imgNumber_->DrawImageDiv(num % DIGIT_NEXT, pos);
		pos.x -= offSet;

		damageDigits /= DIGIT_NEXT;
		damage /= DIGIT_NEXT;
	}
}

bool SceneBase::SceneTransition(void)
{
	if (nextSceneId_ != SceneID::SCENE_ID::NONE)
	{
		SceneManager::GetInstance().ChangeSceneState(nextSceneId_);
		return true;
	}
	return false;
}
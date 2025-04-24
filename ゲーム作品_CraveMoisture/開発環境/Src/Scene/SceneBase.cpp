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

	// �|�X�g�G�t�F�N�g�̍X�V
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
	// �_���[�W�p�̐����f�[�^�����[�h
	// �摜����L�Ɠ����Ȃ��߁A���W���̂ݓo�^
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
	//�I�t�Z�b�g
	int offSet = imgNumber_->GetSize().x;


	//���Ԃ̕\��------------------------------------------

	//�b-------------------------------

	//��̈�
	imgNumber_->DrawImageDiv((time % 60) % 10, pos);

	pos.x -= offSet;

	//�\�̈�
	imgNumber_->DrawImageDiv((time % 60) / 10, pos);
	//---------------------------------


	pos.x -= offSet;

	//�L���u:�v
	imgResultSymbol_->DrawImageDiv(8, pos);
	//��-------------------------------

	pos.x -= offSet;

	//��̈�
	imgNumber_->DrawImageDiv((time / 60) % 10, pos);

	pos.x -= offSet;

	//�\�̈�
	imgNumber_->DrawImageDiv((time / 60) / 10, pos);
	//---------------------------------

	//----------------------------------------------------
}

void SceneBase::DrawDamage(const float param)
{
	if (imgNumber_ == nullptr)return;

	//���݂̃_���[�W��
	int damage = static_cast<int>(param);
	if (damage > 99999)
	{
		damage = 99999;
	}

	const int DIGIT_NEXT = 10;

	//�_���[�W�̍ő包���v��
	int damageDigits = 1;

	// ��������ȏォ
	while ((damage / damageDigits) > 9)
		damageDigits *= DIGIT_NEXT;



	//�I�t�Z�b�g
	int offSet = imgNumber_->GetSize().x;
	// ���W
	Position2D pos = damageNumberPos_;

	//�_���[�W�ʂ���̈ʂ���ŏ�ʂ܂ŕ`��
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
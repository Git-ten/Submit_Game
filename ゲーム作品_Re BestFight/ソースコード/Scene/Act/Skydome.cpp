#include"../../Manager/ResourceManager.h"
#include"../../Render/ModelMaterial.h"
#include"../../Render/ModelRender.h"
#include"ActorBase.h"
#include"../Camera.h"
#include"Common/Transform.h"
#include"Common/AnimationController.h"
#include"../../Utility/MyUtility.h"
#include "Skydome.h"

#define SKYDOME ResourceManager::SRC::SKYDOME_1

SkyDome::SkyDome(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy)
{
	player_ = player;
	enemy_ = enemy;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{
	InitTransform();
	InitShader();
}

void SkyDome::Update(void)
{
	if (player_.lock()->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
		player_.lock()->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		if (player_.lock()->animController_->IsPlayAnim())
		{
			SetDiffuseColor({ 0.1f, 0.1f, 0.1f });
		}
		else
		{
			SetDefDiffuseColor();
		}
	}
	else if (enemy_.lock()->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
		enemy_.lock()->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		if (enemy_.lock()->animController_->IsPlayAnim())
		{
			SetDiffuseColor({ 0.2f, 0.2f, 0.2f });
		}
		else
		{
			SetDefDiffuseColor();
		}
	}
	else
	{
		SetDefDiffuseColor();
	}
}

void SkyDome::Draw(void)
{
	DrawSkydome();
}

void SkyDome::InitTransform(void)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(SKYDOME));

	transform_->scl = SKYDOME_SCALE;
	transform_->rot = MyUtility::VECTOR_ZERO;
	transform_->pos = MyUtility::VECTOR_ZERO;
	transform_->quaRot = Quaternion::Euler(MyUtility::Deg2RadV({ 0.0f, -90.0f, 45.0f }));
	transform_->quaRotLocal = Quaternion();

	transform_->Update();
}

void SkyDome::InitShader(void)
{
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", 4,
		"ReflectModelPS.cso", 5
	);

	//頂点シェーダ定数セット
	material_->AddConstBufVS({0.0f, 0.0f, 0.0f, 0.0f });
	//フォグの始まりと終わり
	float fogStart = 0.0f;
	float fogEnd = 0.0f;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->AddConstBufVS({0.0f, 0.0f, fogStart, fogEnd });
	//ポイントライト
	material_->AddConstBufVS({0.0f, 6000.0f, 16000.0f, 10000.0f });
	material_->AddConstBufVS({ 0.0f, 0.0f, 0.0f, 0.0f });

	//ピクセルシェーダ定数セット
	//拡散光
	material_->AddConstBufPS({ 0.8f, 0.8f, 0.8f, 1.0f });
	//環境光
	material_->AddConstBufPS({ 0.5f, 0.5f, 0.5f, 1.0f });
	
	//ライトの色
	material_->AddConstBufPS({ 0.6f, 0.6f, 0.6f, 0.0f });
	material_->AddConstBufPS({ 0.6f, 0.6f, 0.6f, 0.0f });

	render_ = std::make_unique<ModelRender>(transform_->modelId, *material_);
}

void SkyDome::DrawSkydome(void)
{
	//MV1DrawModel(transform_->modelId);
	render_->Draw();
}

void SkyDome::SetDiffuseColor(const Color3D& color)
{
	material_->SetConstBufPS(0, { color.x, color.y, color.z, 1.0f });
}

void SkyDome::SetDefDiffuseColor(void)
{
	material_->SetConstBufPS(0, { DIFFUSE_COLOR.x, DIFFUSE_COLOR.y, DIFFUSE_COLOR.z, 1.0f });
}
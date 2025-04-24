#include"../../Manager/ResourceManager.h"
#include"../../Render/ModelMaterial.h"
#include"../../Render/ModelRender.h"
#include"../../Utility/MyUtility.h"
#include"ActorBase.h"
#include"../Camera.h"
#include"Common/Transform.h"
#include"Common/AnimationController.h"
#include"../Common/Quaternion.h"
#include"../Common/Collider.h"
#include "Stage.h"

#define STAGE_MODEL ResourceManager::SRC::STAGE

Stage::Stage(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy, std::weak_ptr<Camera>camera)
{
	player_ = player;
	enemy_ = enemy;
	camera_ = camera;
}

Stage::~Stage()
{

}

void Stage::Init()
{
	InitTransform();
	InitShader();

	collider_ = std::make_unique<Collider>(player_.lock(), enemy_.lock());
}

void Stage::Update()
{
	Collision();
	if (player_.lock()->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
		player_.lock()->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		SetSparePointLightShader(1000.0f, player_.lock()->transform_->pos, player_.lock()->GetMyColor());
		if (player_.lock()->animController_->IsPlayAnim())
		{
			SetDiffuseColor({0.2f, 0.2f, 0.2f});
		}
		else
		{
			SetDefDiffuseColor();
		}
	}
	else if (enemy_.lock()->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
			enemy_.lock()->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		SetSparePointLightShader(1000.0f, enemy_.lock()->transform_->pos, enemy_.lock()->GetMyColor());
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
		SetSparePointLightShader(0.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		SetDefDiffuseColor();
	}

}

void Stage::Draw()
{
	//カメラの仕組み上、描画時に当たり判定し
	//その処理をしなければならない
	CollisionCamera();
	DrawStage();
}

void Stage::Collision()
{
	CollisionActor();
}

void Stage::CollisionActor()
{
	collider_->CollisionCircle2Vector(player_.lock(), COLL_RADIUS_CIRCLE);
	collider_->CollisionCircle2Vector(enemy_.lock(), COLL_RADIUS_CIRCLE);
}

void Stage::CollisionCamera()
{
	collider_->CollisionCircle2Vector(camera_.lock(), COLL_RADIUS_CIRCLE);
}

void Stage::InitTransform()
{
	transform_ = std::make_unique<Transform>();

	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(STAGE_MODEL));

	transform_->scl = STAGE_SCALE;
	transform_->rot = MyUtility::VECTOR_ZERO;
	transform_->pos = MyUtility::VECTOR_ZERO;
	transform_->quaRot = Quaternion();
	transform_->quaRotLocal = Quaternion();
	transform_->Update();
}

void Stage::InitShader(void)
{
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", 4,
		"ReflectModelPS.cso", 5
	);

	//頂点シェーダ定数セット
	Position3DF cameraPos = camera_.lock()->GetPos();
	material_->AddConstBufVS({ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f});
	//フォグの始まりと終わり
	float fogStart = 0.0f;
	float fogEnd = 0.0f;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->AddConstBufVS({0.0f, 0.0f, fogStart, fogEnd });
	//ポイントライト
	material_->AddConstBufVS({POINT_LIGHT_POS.x, POINT_LIGHT_POS.y, POINT_LIGHT_POS.z , POINT_LIGHT_POWER });
	material_->AddConstBufVS({0.0f, 0.0f, 0.0f, 0.0f});


	//ピクセルシェーダ定数セット
	//拡散光
	material_->AddConstBufPS({0.8f, 0.8f, 0.8f, 1.0f});
	//環境光
	material_->AddConstBufPS({0.0f, 0.0f, 0.0f, 1.0f});

	material_->AddConstBufPS({ POINT_LIGHT_POS.x, POINT_LIGHT_POS.y, POINT_LIGHT_POS.z , 0.0f });

	//ライトの色
	material_->AddConstBufPS({ POINT_LIGHT_COLOR.x, POINT_LIGHT_COLOR.y, POINT_LIGHT_COLOR.z, 
		0.0f});
	material_->AddConstBufPS({0.0f, 0.0f, 0.0f, 0.0f});

	render_ = std::make_unique<ModelRender>(transform_->modelId, *material_);
}

void Stage::DrawStage()
{
	Position3DF cameraPos = camera_.lock()->GetPos();
	material_->SetConstBufVS(0, { cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });

	//MV1DrawModel(transform_->modelId);
	render_->Draw();
}

void Stage::SetSparePointLightShader(float power, const Position3DF& pos, const Color3D& color)
{
	material_->SetConstBufVS(3, {pos.x, pos.y, pos.z, power });
	material_->SetConstBufPS(4, { color.x, color.y, color.z, 0.0f });
}

void Stage::SetDiffuseColor(const Color3D& color)
{
	material_->SetConstBufPS(0, { color.x, color.y, color.z, 1.0f });
}

void Stage::SetDefDiffuseColor(void)
{
	material_->SetConstBufPS(0, { DIFFUSE_COLOR.x, DIFFUSE_COLOR.y, DIFFUSE_COLOR.z, 1.0f });
}
#include <cassert>
#include "../../Utility/Utility.h"
#include "../Common/DataLoad/LoadDataModel.h"
#include "../Application/Application.h"
#include "../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "Transform.h"

#pragma region �萔�錾

// �V�F�[�_::���C�g
const int SHADER_LIGHT_INDEX = 1;

// �V�F�[�_::�J����
const int SHADER_CAMERA_INDEX = 2;

#pragma endregion

Transform::Transform(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id):
	modInfo_(DataModel()),
	prevPosition(Utility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()), 
	def_quaRotLocal(Quaternion()),
	eventVertexRenderer_(nullptr)
{
	modInfo_ = LoadDataModel::LoadedDataModel(Application::GetInstance().GetDataPaths(), id, info);
	assert(modInfo_.modelId != -1 || !"Transform�Ń��f���̃��[�h�Ɏ��s");

	//���W�ω��O�̍��W
	prevPosition = modInfo_.position;
	//�f�t�H���g���[�J����]
	def_quaRotLocal = modInfo_.quaRotLocal;


	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();

	Update();
}

Transform::~Transform(void)
{
}

void Transform::PrevUpdate(void)
{
	prevPosition = modInfo_.position;
}

void Transform::Update(void)
{
	// �傫��
	matScl = MGetScale(modInfo_.scl);


	// ��]
	modInfo_.rot = modInfo_.quaRot.ToEuler();
	matRot = modInfo_.quaRot.ToMatrix();


	// �ʒu
	matPos = MGetTranslate(modInfo_.position);


	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = modInfo_.quaRot.Mult(modInfo_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);


	// �s������f���ɃZ�b�g
	MV1SetMatrix(modInfo_.modelId, mat);

	EventVertexUpdate();
}

void Transform::Update(const int modelId, const std::string& frameName)
{
	int frameIdx = MV1SearchFrame(modelId, frameName.c_str());

	//���W�ϊ��s��(���[�J��)
	MATRIX weaponMat = MV1GetFrameLocalWorldMatrix(modelId, frameIdx);

	MATRIX transMat = MGetScale(modInfo_.scl);
	transMat = MMult(transMat, weaponMat);

	modInfo_.position = MGetTranslateElem(transMat);
	modInfo_.rot = Quaternion::ToEuler(Quaternion::GetRotation(MGetRotElem(transMat)));
	modInfo_.quaRot = Quaternion::GetRotation(MGetRotElem(transMat));

	MV1SetMatrix(modInfo_.modelId, transMat);
}

void Transform::UpdateVertexPS(const int index, const FLOAT4& param)
{
	if (vertexRenderer_ == nullptr)return;

	vertexRenderer_->SetConstBufPS(index, param);
}

void Transform::Draw(void)
{
	if (eventVertexRenderer_ == nullptr)
	{
		if (vertexRenderer_ == nullptr)MV1DrawModel(modInfo_.modelId);
		else						   vertexRenderer_->Draw();
	}
	else
	{
		eventVertexRenderer_->Draw();
	}
}

const int& Transform::GetModelId(void) const
{
	return modInfo_.modelId;
}

int& Transform::GetModelIdReference(void)
{
	return modInfo_.modelId;
}

const Position3DF& Transform::GetPrevPos(void)
{
	return prevPosition;
}

const Position3DF& Transform::GetPos(void) const
{
	return modInfo_.position;
}

Position3DF& Transform::GetPrevPosReference(void)
{
	return prevPosition;
}

Position3DF& Transform::GetPosReference(void)
{
	return modInfo_.position;
}

const VECTOR& Transform::GetScl(void) const
{
	return modInfo_.scl;
}

const Rot3DF& Transform::GetRot(void) const
{
	return modInfo_.rot;
}

Rot3DF& Transform::GetRotReference(void)
{
	return modInfo_.rot;
}

void Transform::SetPos(const Position3DF& pos)
{
	modInfo_.position = pos;
	prevPosition = pos;
}

void Transform::MoveModelPos(const VECTOR& moveVec)
{
	if (Utility::EqualsVZero(moveVec))return;

	modInfo_.position = VAdd(modInfo_.position, moveVec);
}

void Transform::RotationModelQuaRot(const Dir3DF& dir)
{
	if (Utility::EqualsVZero(dir))return;

	//�ړ�������؂�ւ���܂ł̎���(0.1�`1.0)
	const double ROT_TIME_MIN = 0.1;
	const double ROT_TIME_MAX = 1.0;
	const double ROT_TIME_HALF = 0.5;

	static double rotTime_ = ROT_TIME_MIN;
	static Quaternion nowQua = modInfo_.quaRot;


	Dir3DF cameraRightDir = SceneManager::GetInstance().
		GetCamera().lock()->GetRotXZ().GetRight();

	static Dir3DF prevCamRightDir = cameraRightDir;


	// ��]�����s����p�x��
	const double ROTATION_ANGLE = 0.05;
	if (Quaternion::Angle(nowQua, Quaternion::LookRotation(dir)) > ROTATION_ANGLE)
	{
		nowQua = Quaternion::LookRotation(dir);

		//�J�����ʒu��؂�ւ��Ă���r���������ꍇ
		if (!Utility::Equals(prevCamRightDir, cameraRightDir))rotTime_ = ROT_TIME_HALF;
		else													rotTime_ = ROT_TIME_MIN;
	}


	prevCamRightDir = cameraRightDir;

	//���g�����ԕ��ړ������ɉ�]
	modInfo_.quaRot = Quaternion::Slerp(modInfo_.quaRot, nowQua, rotTime_);

	const float TIME_SPEED = 2.0f;

	rotTime_ += SceneManager::GetInstance().GetDeltaTime() * TIME_SPEED;
	if (rotTime_ > ROT_TIME_MAX)
	{
		rotTime_ = ROT_TIME_MAX;
	}
}

void Transform::RotationModelAngleAxisLerp(const Rot3DF& angleRot)
{
	//if (Utility::EqualsVZero(angleRot))return;

	//�ړ�������؂�ւ���܂ł̎���(0.1�`1.0)
	const double ROT_TIME_MIN = 0.1;
	const double ROT_TIME_MAX = 1.0;
	const double ROT_TIME_HALF = 0.5;

	static double rotTime_ = ROT_TIME_MIN;
	static Quaternion nowQua = modInfo_.quaRot;
	const Rot3DF quaZangle = modInfo_.quaRot.ToEuler();


	// ��]�����s����p�x��
	if (!Utility::Equals(quaZangle, angleRot))
	{
		nowQua = def_quaRotLocal;
		nowQua = nowQua.Mult(Quaternion::Euler(angleRot));
		rotTime_ = ROT_TIME_MIN;
	}

	//���g�����ԕ��ړ������ɉ�]
	modInfo_.quaRot = Quaternion::Slerp(modInfo_.quaRot, nowQua, rotTime_);

	// ���`��Ԏ��Ԃ̍X�V
	const float TIME_SPEED = 2.0f;
	rotTime_ += SceneManager::GetInstance().GetDeltaTime() * TIME_SPEED;
	if (rotTime_ > ROT_TIME_MAX)
	{
		rotTime_ = ROT_TIME_MAX;
	}
}

void Transform::RotationModelAngleAxis(const Rot3DF& angleRot)
{
	const double ROT_TIME_MAX = 1.0;

	Quaternion nowQua = modInfo_.quaRot;
	nowQua = nowQua.Mult(Quaternion::Euler(angleRot));

	//���g�����ԕ��ړ������ɉ�]
	modInfo_.quaRot = Quaternion::Slerp(modInfo_.quaRot, nowQua, ROT_TIME_MAX);
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

void Transform::InitVertexRenderer(const TRANSFORM_TYPE& type)
{
	switch (type)
	{
	case TRANSFORM_TYPE::SKIN_MODEL:
		InitVertexSkin();
		break;
	case TRANSFORM_TYPE::NORMAL_MODEL:
		InitVertexNormal();
		break;
	case TRANSFORM_TYPE::NOISE_MODEL:
		InitVertexNoize();
		break;
	case TRANSFORM_TYPE::METAL_MODEL:
		InitVertexMetal();
		break;
	default:
		break;
	}
}

void Transform::StartEventVertex(const EVENT_TYPE& type)
{
	nowType_ = type;

	switch (nowType_)
	{
	case EVENT_TYPE::RIMLIGHT:
		InitRimLight(ModelRenderer::SHADER_NAME::RIM_LIGHT);
		break;
	case EVENT_TYPE::RIMLIGHT_SKIN:
		InitRimLight(ModelRenderer::SHADER_NAME::RIM_LIGHT_SKIN);
		break;
	default:
		break;
	}
}

void Transform::EndEventVertex(void)
{
	if (eventVertexRenderer_ != nullptr)
	{
		eventVertexRenderer_.reset();
		eventVertexRenderer_ = nullptr;
	}
}

void Transform::EventVertexUpdate(void)
{
	if (eventVertexRenderer_ == nullptr)return;

	switch (nowType_)
	{
	case EVENT_TYPE::RIMLIGHT:
		UpdateRimLight();
		break;
	case EVENT_TYPE::RIMLIGHT_SKIN:
		UpdateRimLight();
		break;
	default:
		break;
	}
}

Dir3DF Transform::GetDir(const Dir3DF& vec) const
{
	return modInfo_.quaRot.PosAxis(vec);
}

VECTOR Transform::GetForwardLocal(void) const
{
	return GetDirLocal(Utility::DIR_F);
}

VECTOR Transform::GetRightLocal(void) const
{
	return GetDirLocal(Utility::DIR_R);
}

VECTOR Transform::GetLeftLocal(void) const
{
	return GetDirLocal(Utility::DIR_L);
}

//���[�J���ȏ����
VECTOR Transform::GetUpLocal(void)const
{
	return GetDirLocal(Utility::DIR_U);
}

VECTOR Transform::GetBackLocal(void) const
{
	return GetDirLocal(Utility::DIR_B);
}

void Transform::SetLightColor(const Color3DF& color)
{
	vertexRenderer_->SetConstBufPS(SHADER_LIGHT_INDEX, { color.x, color.y, color.z, ModelRenderer::SHADER_NONE_VALUE });
}

void Transform::SetLightPower(const float power)
{
	//�J�������W
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	vertexRenderer_->SetConstBufPS(SHADER_CAMERA_INDEX, { power, cameraPos.x, cameraPos.y, cameraPos.z });
}

Dir3DF Transform::GetDirLocal(const Dir3DF& vec) const
{
	return modInfo_.quaRotLocal.PosAxis(vec);
}

void Transform::InitVertexSkin(void)
{
	vertexRenderer_ = std::make_unique<ModelRenderer>(modInfo_.modelId);
	vertexRenderer_->SetShaderInfo(ModelRenderer::SHADER_NAME::SKIN_MESH_NORM_4,
		ModelRenderer::SKIN_MASH_BUF_SIZE_PS,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::BUF_SIZE_ZERO);

	// ���C�g�̕���
	Dir3DF lightDir = SceneManager::GetInstance().GetLightDir();
	vertexRenderer_->AddConstBufPS({ lightDir.x, lightDir.y, lightDir.z,
		ModelRenderer::SHADER_NONE_VALUE });

	// ���C�g�̐F
	Color3DF color = ModelRenderer::FEW_BLACK_COLOR;
	vertexRenderer_->AddConstBufPS({ color.x, color.y, color.z,
		ModelRenderer::SHADER_NONE_VALUE });

	// ���C�g�̋���
	float lightPower = ModelRenderer::DIRECTION_LIGHT_POWER_MEDIUM;
	//�J�������W
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	vertexRenderer_->AddConstBufPS({ lightPower, cameraPos.x, cameraPos.y, cameraPos.z });


	//�e�N�X�`���̃Z�b�g
	vertexRenderer_->SetTextureBuf(11,
		ResourceManager::GetInstance().Load(ResourceManager::RESOURCE_ID::ROUGHNESS).handleId_);
}

void Transform::InitVertexNormal(void)
{
	vertexRenderer_ = std::make_unique<ModelRenderer>(modInfo_.modelId);
	vertexRenderer_->SetShaderInfo(ModelRenderer::SHADER_NAME::NORMAL,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::BUF_SIZE_ZERO);
}

void Transform::InitVertexNoize(void)
{
	vertexRenderer_ = std::make_unique<ModelRenderer>(modInfo_.modelId);

	vertexRenderer_->SetShaderInfo(ModelRenderer::SHADER_NAME::DISSOLVE,
		ModelRenderer::DISSOLVE_BUF_SIZE_PS,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::BUF_SIZE_ZERO);

	vertexRenderer_->AddConstBufPS(
		{ 0.0f, 0.0f,
		  ModelRenderer::SHADER_NONE_VALUE,
		  ModelRenderer::SHADER_NONE_VALUE
		});

	vertexRenderer_->SetTextureBuf(7,
		ResourceManager::GetInstance().Load(ResourceManager::RESOURCE_ID::NOISE).handleId_);
}

void Transform::InitVertexMetal(void)
{
	vertexRenderer_ = std::make_unique<ModelRenderer>(modInfo_.modelId);
	vertexRenderer_->SetShaderInfo(ModelRenderer::SHADER_NAME::METAL,
		ModelRenderer::METAL_BUF_SIZE_PS,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::BUF_SIZE_ZERO);

	// ���C�g�̕���
	Dir3DF lightDir = SceneManager::GetInstance().GetLightDir();
	vertexRenderer_->AddConstBufPS({ lightDir.x, lightDir.y, lightDir.z,
		ModelRenderer::SHADER_NONE_VALUE });

	//�J�������W
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	vertexRenderer_->AddConstBufPS({
		ModelRenderer::SHADER_NONE_VALUE,
		cameraPos.x, cameraPos.y, cameraPos.z });


	//�e�N�X�`���̃Z�b�g
	vertexRenderer_->SetTextureBuf(ModelRenderer::ROUGHNESS_INDEX,
		ResourceManager::GetInstance().Load(ResourceManager::RESOURCE_ID::SWORD_ROUGHNESS).handleId_);
	vertexRenderer_->SetTextureBuf(ModelRenderer::METALIC_INDEX,
		ResourceManager::GetInstance().Load(ResourceManager::RESOURCE_ID::SWORD_METALIC).handleId_);
	vertexRenderer_->SetTextureBuf(ModelRenderer::SPHERE_INDEX,
		ResourceManager::GetInstance().Load(ResourceManager::RESOURCE_ID::SPHERE_METAL).handleId_);
}

void Transform::InitRimLight(const ModelRenderer::SHADER_NAME& name)
{
	eventVertexRenderer_ = std::make_unique<ModelRenderer>(modInfo_.modelId);
	eventVertexRenderer_->SetShaderInfo(name,
		ModelRenderer::BUF_SIZE_ZERO,
		ModelRenderer::RIM_LIGHT_BUF_SIZE_VS,
		ModelRenderer::BUF_SIZE_ZERO);

	//�J�������W
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	eventVertexRenderer_->AddConstBufVS({
	ModelRenderer::SHADER_NONE_VALUE,
		cameraPos.x,
		cameraPos.y,
		cameraPos.z });
}

void Transform::UpdateRimLight(void)
{
	//�J�������W
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	eventVertexRenderer_->SetConstBufVS(0, {
	ModelRenderer::SHADER_NONE_VALUE,
		cameraPos.x,
		cameraPos.y,
		cameraPos.z });
}

void Transform::UpdateMetal(void)
{
}

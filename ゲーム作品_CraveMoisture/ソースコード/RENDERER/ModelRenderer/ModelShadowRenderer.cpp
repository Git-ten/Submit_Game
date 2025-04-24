#include <cassert>
#include "../../Application/Application.h"
#include "../../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "ModelShadowRenderer.h"

#pragma region �萔�錾

const float DEPTH_DISTANCE = 8000.0f;

// ��O�q
const int EXCEPTION_VALUE = -1;

#pragma endregion

ModelShadowRenderer::ModelShadowRenderer(int handle) :
    ModelRenderer(handle),
    shaderDepthPS_(EXCEPTION_VALUE),
    shaderDepthVS_(EXCEPTION_VALUE),
    shaderSkinDepthVS_(EXCEPTION_VALUE),
    cameraView_(MATRIX()),
    cameraProj_(MATRIX())
{
    //�X�N���[���̑傫��
    Size2D screenSize = Application::GetInstance().GetScreenSize();

    // �[�x�o�b�t�@�p�X�N���[��
   // ( �P�`�����l�����������_�Q�S�r�b�g�e�N�X�`�� )
    SetCreateDrawValidGraphChannelNum(1);
    SetDrawValidFloatTypeGraphCreateFlag(TRUE);
    SetCreateGraphChannelBitDepth(24);
    depthHandle_ = MakeScreen(screenSize.x * 10, screenSize.y * 10, true);
    SetCreateDrawValidGraphChannelNum(0);
    SetDrawValidFloatTypeGraphCreateFlag(FALSE);
    SetCreateGraphChannelBitDepth(0);

    InitNamesSpecial();
}

ModelShadowRenderer::~ModelShadowRenderer(void)
{
    DeleteShader(shaderDepthPS_);
    DeleteShader(shaderDepthVS_);
    DeleteShader(shaderSkinDepthVS_);
}

void ModelShadowRenderer::AddDepthModel(int modelId)
{
    depthModels_.push_back(modelId);
}

void ModelShadowRenderer::SetDepthModel(int idx, int modelId)
{
    if (idx < depthModels_.size())
    {
        depthModels_[idx] = modelId;
    }
}

void ModelShadowRenderer::AddSkinDepthModel(int modelId)
{
    skinDepthModels_.push_back(modelId);
}

void ModelShadowRenderer::SetSkinDepthModel(int idx, int modelId)
{
    if (idx < depthModels_.size())
    {
        skinDepthModels_[idx] = modelId;
    }
}

void ModelShadowRenderer::DrawSpecial(void)
{
    DrawDepth();
}

void ModelShadowRenderer::DrawDepth(void)
{
    MV1SetUseOrigShader(true);
    SetDrawScreen(depthHandle_);
    ClearDrawScreen();

    std::weak_ptr<Camera>camera = SceneManager::GetInstance().GetCamera();
    SetupCamera_Ortho(13250.0f);
    SetCameraNearFar(Camera::CAMERA_NEAR, Camera::CAMERA_FAR);
    Position3DF target = Position3DF();
    Dir3DF light = SceneManager::GetInstance().GetLightDir();
    Position3DF pos = VAdd(target, VScale(light, -DEPTH_DISTANCE));
    SetCameraPositionAndTarget_UpVecY(pos, target);

    // �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
    cameraView_ = GetCameraViewMatrix();
    cameraProj_ = GetCameraProjectionMatrix();

    for (int ii = 0; ii < 4; ii++)
    {
        float a = cameraView_.m[ii][0];
        float b = cameraView_.m[ii][1];
        float c = cameraView_.m[ii][2];
        float d = cameraView_.m[ii][3];
        SetConstBufVS(ii, { a, b, c, d });
    }
    for (int jj = 0; jj < 4; jj++)
    {
        float e = cameraProj_.m[jj][0];
        float f = cameraProj_.m[jj][1];
        float g = cameraProj_.m[jj][2];
        float h = cameraProj_.m[jj][3];
        SetConstBufVS(jj + 4, { e, f, g, h });
    }
    SetRenderTargetToShader(1, depthHandle_);

    SetUsePixelShader(shaderDepthPS_);

    //�@�������̃��f��
    SetUseVertexShader(shaderDepthVS_);
    for (const auto& m : depthModels_)
    {
        MV1DrawModel(m);
    }

    //�X�L�����b�V��
    SetUseVertexShader(shaderSkinDepthVS_);
    for (const auto& sm : skinDepthModels_)
    {
        MV1DrawModel(sm);
    }

    SetRenderTargetToShader(1, -1);
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
    MV1SetUseOrigShader(false);

    SetDrawScreen(Application::GetInstance().GetMainScreen());
    camera.lock()->DrawUpdate();
}

void ModelShadowRenderer::InitNamesSpecial(void)
{
    DataPaths dataPath = Application::GetInstance().GetDataPaths();
    std::string shaderPath;

    shaderPath = dataPath.PATH_SHADER_VERTEX + "DepthMapPS.cso";
    shaderDepthPS_ = LoadPixelShader(shaderPath.c_str());

    shaderPath = dataPath.PATH_SHADER_VERTEX + "DepthMapVS.cso";
    shaderDepthVS_ = LoadVertexShader(shaderPath.c_str());

    shaderPath = dataPath.PATH_SHADER_VERTEX + "DepthSkinMapVS.cso";
    shaderSkinDepthVS_ = LoadVertexShader(shaderPath.c_str());
}

void ModelShadowRenderer::SetShaderSpecial(void)
{
    assert((name_ == SHADER_NAME::NO_NORM_SHADOW ||
            name_ == SHADER_NAME::NORM_SHADOW) ||
            !"ModelShadowRenderer�ŉe�\���ȊO�̃V�F�[�_���g�p���悤�Ƃ��Ă��܂�");

    if (name_ == SHADER_NAME::NO_NORM_SHADOW ||
        name_ == SHADER_NAME::NORM_SHADOW)
    {
        SetTextureBuf(8, depthHandle_);

        for (int ii = 0; ii < SHADOW_BUF_SIZE_VS; ii++)
        {
            AddConstBufVS({ 0.0f, 0.0f, 0.0f, 0.0f });
        }
    }
}
#include <cassert>
#include "../../Application/Application.h"
#include "../../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "ModelShadowRenderer.h"

#pragma region 定数宣言

const float DEPTH_DISTANCE = 8000.0f;

// 例外子
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
    //スクリーンの大きさ
    Size2D screenSize = Application::GetInstance().GetScreenSize();

    // 深度バッファ用スクリーン
   // ( １チャンネル浮動小数点２４ビットテクスチャ )
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

    // 設定したカメラのビュー行列と射影行列を取得しておく
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

    //法線無しのモデル
    SetUseVertexShader(shaderDepthVS_);
    for (const auto& m : depthModels_)
    {
        MV1DrawModel(m);
    }

    //スキンメッシュ
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
            !"ModelShadowRendererで影表現以外のシェーダを使用しようとしています");

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
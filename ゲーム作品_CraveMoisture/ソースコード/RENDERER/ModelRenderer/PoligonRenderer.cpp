#include "../../Application/Application.h"
#include "ModelRenderer.h"
#include "PoligonRenderer.h"

#pragma region 定数宣言

// 例外子
const int EXCEPTION_VALUE = -1;

// ポリゴンを生成するのに必要な最低数
const int POLIGON_CREATE_MIN = 3;

#pragma endregion

PoligonRenderer::PoligonRenderer(void):
    texture_(EXCEPTION_VALUE)
{
    DataPaths dataPath = Application::GetInstance().GetDataPaths();
    std::string shaderPath;

    shaderPath = dataPath.PATH_SHADER_VERTEX + "PoligonPS.cso";
    shaderPoligonPS_ = LoadPixelShader(shaderPath.c_str());

    shaderPath = dataPath.PATH_SHADER_VERTEX + "PoligonVS.cso";
    shaderPoligonVS_ = LoadVertexShader(shaderPath.c_str());
}

PoligonRenderer::~PoligonRenderer(void)
{
    DeleteShader(shaderPoligonPS_);
    DeleteShader(shaderPoligonVS_);
}

void PoligonRenderer::Draw(std::vector<VERTEX3DSHADER>& vertex, std::vector<WORD>& indexs)
{
    if (vertex.size() <= POLIGON_CREATE_MIN)return;

    MV1SetUseOrigShader(true);

    SetUseVertexShader(shaderPoligonVS_);
    SetUsePixelShader(shaderPoligonPS_);
    SetUseTextureToShader(0, texture_);

    int texAType = static_cast<int>(ModelRenderer::TEXADDRESS::CLAMP);

    SetTextureAddressModeUV(texAType, texAType);

    // 描画
    int size = static_cast<int>(vertex.size());
    VERTEX3DSHADER* shaderVer = nullptr;
    shaderVer = &vertex[0];
    WORD* idx = nullptr;
    idx = &indexs[0];

    DrawPolygonIndexed3DToShader(shaderVer, size, idx, size - 2);

    SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

    SetUseTextureToShader(0, -1);

    SetUsePixelShader(-1);

    SetUseVertexShader(-1);

    MV1SetUseOrigShader(false);
}

void PoligonRenderer::SetTexture(int texture)
{
    texture_ = texture;
}
#include<assert.h>
#include "../../Application/Application.h"
#include"../../Common/Camera.h"
#include "ModelRenderer.h"
#include<memory>
#include<algorithm>

#pragma region 定数宣言

//頂点定数バッファのスロット
const int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
//Pixel定数バッファのスロット
const int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

// 例外子
const int EXCEPTION_VALUE = -1;

#pragma endregion

ModelRenderer::ModelRenderer(int handle):
    modelId_(handle),
    name_(SHADER_NAME::NONE),
    shaderInfo_(SHADER_INFO())
{
    InitNames();
}

ModelRenderer::~ModelRenderer(void)
{
    DeleteShader(shaderInfo_.shaderPS);
    DeleteShader(shaderInfo_.shaderVS);

    DeleteShaderConstantBuffer(shaderInfo_.constBufPS);
    DeleteShaderConstantBuffer(shaderInfo_.constBufVS);

    shaderNamesPS_.clear();
    shaderNamesVS_.clear();
}

void ModelRenderer::Draw(void)
{
    DrawSpecial();

    MV1SetUseOrigShader(true);
    SetReserveVS();
    SetReservePS();

    auto texA = shaderInfo_.texAddress;
    int texAType = static_cast<int>(texA);

    SetTextureAddressModeUV(texAType, texAType);

    MV1DrawModel(modelId_);

    SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

    const auto& textures = shaderInfo_.textures;
    size_t size = textures.size();

    // 画像を一つも登録していないなら
    // デフォルトの画像を消去
    if (size == 0)
    {
        SetUseTextureToShader(0, -1);
    }
    else
    {
        for (auto tex : textures)
        {
            SetUseTextureToShader(tex.first, -1);
        }
    }

    SetUsePixelShader(-1);

    SetUseVertexShader(-1);

    MV1SetUseOrigShader(false);

}

void ModelRenderer::SetShaderInfo(SHADER_NAME name,
    int constBufFloat4SizePS, int constBufFloat4SizeVS, int constBufFloat4SizeGE)
{
    auto nameIte = shaderNamesPS_.find(name);
    if (nameIte == shaderNamesPS_.end())return;
    nameIte = shaderNamesVS_.find(name);
    if (nameIte == shaderNamesVS_.end())return;

    SHADER_INFO info;

    info.shaderVS = LoadVertexShader(shaderNamesVS_.at(name).c_str());
    assert(info.shaderVS != EXCEPTION_VALUE);
    info.constBufFloat4SizeVS = constBufFloat4SizeVS;
    info.constBufVS = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);

    info.shaderPS = LoadPixelShader(shaderNamesPS_.at(name).c_str());
    assert(info.shaderPS != EXCEPTION_VALUE);
    info.constBufFloat4SizePS = constBufFloat4SizePS;
    info.constBufPS = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);
    info.texAddress = TEXADDRESS::CLAMP;

    shaderInfo_ = info;
    name_ = name;

    SetShaderSpecial();
}

void ModelRenderer::AddConstBufPS(FLOAT4 contBufPS)
{
    if (shaderInfo_.constBufFloat4SizePS > shaderInfo_.constBufsPS.size())
    {
        shaderInfo_.constBufsPS.emplace_back(contBufPS);
    }
}

void ModelRenderer::AddConstBufVS(FLOAT4 contBufVS)
{
    if (shaderInfo_.constBufFloat4SizeVS > shaderInfo_.constBufsVS.size())
    {
        shaderInfo_.constBufsVS.emplace_back(contBufVS);
    }
}

void ModelRenderer::SetConstBufPS(int idx, FLOAT4 contBufPS)
{
    if (idx >= shaderInfo_.constBufsPS.size())
    {
        return;
    }

    shaderInfo_.constBufsPS[idx] = contBufPS;
}

void ModelRenderer::SetConstBufVS(int idx, FLOAT4 contBufVS)
{
    if (idx >= shaderInfo_.constBufsVS.size())
    {
        return;
    }

    shaderInfo_.constBufsVS[idx] = contBufVS;
}

void ModelRenderer::SetTextureBuf(int idx, int handle)
{
    const auto& ite = shaderInfo_.textures.find(handle);
    if (ite == shaderInfo_.textures.end())
    {
        shaderInfo_.textures.emplace(idx, handle);
    }
    else
    {
        ite->second = handle;
    }
}

void ModelRenderer::SetTextureAddress(TEXADDRESS texA)
{
    shaderInfo_.texAddress = texA;
}

void ModelRenderer::InitNames(void)
{
    DataPaths dataPath = Application::GetInstance().GetDataPaths();

    shaderNamesPS_.clear();
    shaderNamesVS_.clear();

    shaderNamesPS_.emplace(SHADER_NAME::NORMAL, dataPath.PATH_SHADER_VERTEX + "ModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::NORMAL, dataPath.PATH_SHADER_VERTEX + "ModelVS.cso");

    shaderNamesPS_.emplace(SHADER_NAME::NORMAL_POLYGON, dataPath.PATH_SHADER_VERTEX + "ModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::NORMAL_POLYGON, dataPath.PATH_SHADER_VERTEX + "ModelVS.cso");

    shaderNamesPS_.emplace(SHADER_NAME::SKIN_MESH_NORM_4, dataPath.PATH_SHADER_VERTEX + "SkinMeshPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::SKIN_MESH_NORM_4, dataPath.PATH_SHADER_VERTEX + "SkinMeshNorm_4VS.cso");
    shaderNamesPS_.emplace(SHADER_NAME::SKIN_MESH_NORM_8, dataPath.PATH_SHADER_VERTEX + "SkinMeshPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::SKIN_MESH_NORM_8, dataPath.PATH_SHADER_VERTEX + "SkinMeshNorm_8VS.cso");
   
    shaderNamesPS_.emplace(SHADER_NAME::METAL, dataPath.PATH_SHADER_VERTEX + "MetalModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::METAL, dataPath.PATH_SHADER_VERTEX + "MetalModelVS.cso");
    shaderNamesPS_.emplace(SHADER_NAME::SKIN_METAL, dataPath.PATH_SHADER_VERTEX + "MetalModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::SKIN_METAL, dataPath.PATH_SHADER_VERTEX + "MetalSkinModelVS.cso");
    shaderNamesPS_.emplace(SHADER_NAME::SKIN_METAL_NORM_8, dataPath.PATH_SHADER_VERTEX + "MetalModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::SKIN_METAL_NORM_8, dataPath.PATH_SHADER_VERTEX + "MetalSkinNormModel_8VS.cso");
   
    shaderNamesPS_.emplace(SHADER_NAME::RIM_LIGHT, dataPath.PATH_SHADER_VERTEX + "RimModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::RIM_LIGHT, dataPath.PATH_SHADER_VERTEX + "RimModelVS.cso");
    shaderNamesPS_.emplace(SHADER_NAME::RIM_LIGHT_SKIN, dataPath.PATH_SHADER_VERTEX + "RimModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::RIM_LIGHT_SKIN, dataPath.PATH_SHADER_VERTEX + "RimSkinModelVS.cso");

    shaderNamesPS_.emplace(SHADER_NAME::NO_NORM_SHADOW, dataPath.PATH_SHADER_VERTEX + "ShadowModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::NO_NORM_SHADOW, dataPath.PATH_SHADER_VERTEX + "ShadowModelVS.cso");
    shaderNamesPS_.emplace(SHADER_NAME::NORM_SHADOW, dataPath.PATH_SHADER_VERTEX + "ShadowModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::NORM_SHADOW, dataPath.PATH_SHADER_VERTEX + "ShadowNormModelVS.cso");
    
    shaderNamesPS_.emplace(SHADER_NAME::DISSOLVE, dataPath.PATH_SHADER_VERTEX + "DissolveModelPS.cso");
    shaderNamesVS_.emplace(SHADER_NAME::DISSOLVE, dataPath.PATH_SHADER_VERTEX + "DissolveModelVS.cso");
}

void ModelRenderer::SetReserveVS(void)
{
    SetUseVertexShader(shaderInfo_.shaderVS);

    int constBuf = shaderInfo_.constBufVS;

    if (constBuf < 0)return;

    FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
    const auto& constBufs = shaderInfo_.constBufsVS;

    size_t size = constBufs.size();
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            constBufsPtr++;
        }
        constBufsPtr->x = constBufs[i].x;
        constBufsPtr->y = constBufs[i].y;
        constBufsPtr->z = constBufs[i].z;
        constBufsPtr->w = constBufs[i].w;
    }

    UpdateShaderConstantBuffer(constBuf);

    SetShaderConstantBuffer(
        constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS
    );
}

void ModelRenderer::SetReservePS(void)
{
    SetUsePixelShader(shaderInfo_.shaderPS);

    const auto& textures = shaderInfo_.textures;
    size_t size = textures.size();
    if (size == 0)
    {
        SetUseTextureToShader(0, -1);
    }
    else
    {
        for (auto tex : textures)
        {
           SetUseTextureToShader(tex.first, tex.second);
        }
    }

    int constBuf = shaderInfo_.constBufPS;

    if (constBuf < 0)return;

    FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
    const auto& constBufs = shaderInfo_.constBufsPS;

    size = constBufs.size();
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            constBufsPtr++;
        }
        constBufsPtr->x = constBufs[i].x;
        constBufsPtr->y = constBufs[i].y;
        constBufsPtr->z = constBufs[i].z;
        constBufsPtr->w = constBufs[i].w;
    }

    UpdateShaderConstantBuffer(constBuf);

    SetShaderConstantBuffer(
        constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS
    );
}

void ModelRenderer::SetShaderSpecial(void)
{
}

void ModelRenderer::DrawSpecial(void)
{
}

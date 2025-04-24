#include"../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(std::string shaderFileNameVS, int constBufFloat4SizeVS, 
    std::string shaderFileNamePS, int constBufFloat4SizePS)
{
    std::string PATH_SHADER = "Data/Shader/";

    //頂点シェーダ
    shaderVS_ = LoadVertexShader(
        (PATH_SHADER + shaderFileNameVS).c_str());

    constBufFloat4SizeVS_ = constBufFloat4SizeVS;

    constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);

    texAddress_ = TEXADDRESS::CLAMP;


    //ピクセルシェーダ
    shaderPS_ = LoadPixelShader(
        (PATH_SHADER + shaderFileNamePS).c_str());

    constBufFloat4SizePS_ = constBufFloat4SizePS;

    constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);

    texAddress_ = TEXADDRESS::CLAMP;
}

ModelMaterial::~ModelMaterial(void)
{
    DeleteShader(shaderVS_);
    DeleteShaderConstantBuffer(constBufVS_);

    DeleteShader(shaderPS_);
    DeleteShaderConstantBuffer(constBufPS_);
}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{
    if (constBufFloat4SizeVS_ > constBufsVS_.size())
    {
        constBufsVS_.emplace_back(contBuf);
    }
}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{
    if (constBufFloat4SizePS_ > constBufsPS_.size())
    {
        constBufsPS_.emplace_back(contBuf);
    }
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{
    if (idx >= constBufsVS_.size())
    {
        return;
    }

    constBufsVS_[idx] = contBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{
    if (idx >= constBufsPS_.size())
    {
        return;
    }

    constBufsPS_[idx] = contBuf;
}

void ModelMaterial::AddTextureBuf(int slot, int handle)
{
    if (textures_.count(slot) == 0)
    {
        textures_.emplace(slot, handle);
    }
}

void ModelMaterial::SetTextureBuf(int slot, int handle)
{
    if (textures_.count(slot) == 0)
    {
        textures_[slot] = handle;
    }
}

int ModelMaterial::GetShaderVS(void) const
{
    return shaderVS_;
}

int ModelMaterial::GetShaderPS(void) const
{
    return shaderPS_;
}

int ModelMaterial::GetConstBufVS(void) const
{
    return constBufVS_;
}

int ModelMaterial::GetConstBufPS(void) const
{
    return constBufPS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsVS(void) const
{
    return constBufsVS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsPS(void) const
{
    return constBufsPS_;
}

const std::map<int, int>& ModelMaterial::GetTextures(void) const
{
    return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void) const
{
    return texAddress_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texA)
{
    texAddress_ = texA;
}
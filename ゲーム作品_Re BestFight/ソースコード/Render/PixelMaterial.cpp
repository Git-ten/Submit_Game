#include"../Application.h"
#include "PixelMaterial.h"
#include<algorithm>

PixelMaterial::PixelMaterial(std::string shaderFileName, int constBufFloat4Size)
{
	std::string PATH_SHADER = "Data/Shader/";

	shader_ = LoadPixelShader(
		(PATH_SHADER + shaderFileName).c_str());

	constBufFloat4Size_ = constBufFloat4Size;

	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size);

	texAddress_ = TEXADDRESS::CLAMP;
}

PixelMaterial::~PixelMaterial(void)
{
	textures_.clear();
	DeleteShader(shader_);
	DeleteShaderConstantBuffer(constBuf_);
}

void PixelMaterial::AddConstBuf(const FLOAT4& contBuf)
{
	if (constBufFloat4Size_ > constBufs_.size())
	{
		constBufs_.emplace_back(contBuf);
	}
}

void PixelMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufs_.size())
	{
		return;
	}
	
	constBufs_[idx] = contBuf;
}

int PixelMaterial::GetShader(void) const
{
	return shader_;
}

int PixelMaterial::GetConstBuf(void) const
{
	return constBuf_;
}

const std::vector<FLOAT4>& PixelMaterial::GetConstBufs(void) const
{
	return constBufs_;
}

const std::vector<int>& PixelMaterial::GetTextures(void) const
{
	return textures_;
}

void PixelMaterial::SetTextureBuf(int handle)
{
	const auto& ite = std::find(textures_.begin(), textures_.end(), handle);
	if (ite == textures_.end())
	{
		textures_.push_back(handle);
	}
	else
	{
		*ite = handle;
	}
}

PixelMaterial::TEXADDRESS PixelMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void PixelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}
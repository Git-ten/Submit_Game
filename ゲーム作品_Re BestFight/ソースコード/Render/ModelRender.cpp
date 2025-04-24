#include"../Application.h"
#include "ModelRender.h"

ModelRender::ModelRender(int modelId, ModelMaterial& modelMaterial)
	:modelMaterial_(modelMaterial)
{
	modelId_ = modelId;
}

ModelRender::~ModelRender(void)
{
}

void ModelRender::Draw(void)
{
	MV1SetUseOrigShader(true);

	//シェーダの準備
	SetReserveVS();
	SetReservePS();

	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	//使用するテクスチャーアドレスUV
	SetTextureAddressModeUV(texAType, texAType);

	MV1DrawModel(modelId_);

	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	//すべてのテクスチャを初期化
	const auto& textures = modelMaterial_.GetTextures();
	for (const auto& tex : textures)
	{
		SetUseTextureToShader(tex.first, -1);
	}

	SetUsePixelShader(-1);
	SetUseVertexShader(-1);

	MV1SetUseOrigShader(false);
}

void ModelRender::SetReserveVS(void)
{
	SetUseVertexShader(modelMaterial_.GetShaderVS());

	int constBuf = modelMaterial_.GetConstBufVS();
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsVS();

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

void ModelRender::SetReservePS(void)
{
	SetUsePixelShader(modelMaterial_.GetShaderPS());

	const auto& textures = modelMaterial_.GetTextures();
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

	int constBuf = modelMaterial_.GetConstBufPS();
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsPS();

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
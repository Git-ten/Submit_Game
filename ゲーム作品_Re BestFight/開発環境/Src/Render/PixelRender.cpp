#include "PixelRender.h"

PixelRender::PixelRender(const PixelMaterial& pixelMaterial)
	: pixelMaterial_(pixelMaterial)
{
}

PixelRender::~PixelRender(void)
{
}

void PixelRender::MakeSquereVertex(Vector2 pos, Vector2 size)
{
	pos_ = pos;
	size_ = size;

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	for (int i = 0; i< 4;i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255,255,255,255);
		vertexs_[i].spc = GetColorU8(255,255,255,255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}
	
	//¶ã
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	//‰Eã
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	//¶‰º
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	//‰E‰º
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;

	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;
	indexes_[cnt++] = 2;
}

void PixelRender::MakeSquereVertex(void)
{
	MakeSquereVertex(pos_,size_);
}

void PixelRender::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void PixelRender::SetSize(Vector2 size)
{
	size_ = size;
}

void PixelRender::Draw(void)
{
	MV1SetUseOrigShader(true);
	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);

	SetUsePixelShader(pixelMaterial_.GetShader());

	size_t size;

	const auto& textures = pixelMaterial_.GetTextures();
	size = textures.size();
	for (int i = 0;i < size; i++)
	{
		SetUseTextureToShader(i, textures[i]);
	}

	int constBuf = pixelMaterial_.GetConstBuf();
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = pixelMaterial_.GetConstBufs();

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

	auto texA = pixelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	SetTextureAddressModeUV(texAType, texAType);

	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);
	
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, -1);
	}

	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);
}
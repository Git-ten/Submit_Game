#include "PixelRenderer.h"
#include"../Application/Application.h"
#include "../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include<DxLib.h>

#pragma region 定数宣言

//ピクセルシェーダの定数バッファのスロット
static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

#pragma endregion

PixelRenderer::PixelRenderer(int num, Size2D size):
	pos_(Position2D()),
	size_(Size2D()),
	screenNum_(num)
{
	InitNames();

	if (screenNum_ <= 0)return;

	screens_ = new int[screenNum_];
	for (int ii = 0; ii < screenNum_; ii++)
	{
		screens_[ii] = MakeScreen(size.x, size.y);
	}
}

PixelRenderer::~PixelRenderer(void)
{
	for (int ii = 0; ii < screenNum_; ii++)
	{
		DeleteGraph(screens_[ii]);
	}

	delete screens_;

	for (auto& info : shaderInfo_)
	{
		DeleteShader(info.second.shader);
		DeleteShaderConstantBuffer(info.second.constBuf);
	}

	shaderInfo_.clear();
	shaderNames_.clear();
}

void PixelRenderer::MakeSquereVertex(Position2D pos, Size2D size)
{
	pos_ = pos;
	size_ = size;

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	for (int i = 0; i < 4;i++)
	{
		// 適当でよい
		vertexs_[i].rhw = 1.0f;

		// 色は何でもよい
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);

		// ここも適当でよい
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	//左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	//右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	//右下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	//左下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;

	cnt = 0;
	// 右側の三角ポリゴンにあたる
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;

	// 左側の三角ポリゴンにあたる
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;
	indexes_[cnt++] = 2;
}

void PixelRenderer::Draw(void)
{
	//描画するためのスクリーンが一つもなければ何もしない
	if (screenNum_ <= 0)return;

	MV1SetUseOrigShader(true);
	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);

	int num = screenNum_;
	int count = 0;

	for (auto& info : shaderInfo_)
	{
		if (count >= num)
		{
			break;
		}
		SetDrawScreen(screens_[count]);
		ClearDrawScreen();

		count++;

		SetUsePixelShader(info.second.shader);

		size_t size;

		const auto& textures = info.second.textures;
		size = textures.size();
		for (int i = 0;i < size; i++)
		{
			SetUseTextureToShader(i, textures[i]);
		}

		int constBuf = info.second.constBuf;
		FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
		const auto& constBufs = info.second.constBufs;

		size = constBufs.size();
		for (int i = 0;i < size;i++)
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

		auto texA = info.second.texAddress;
		int texAType = static_cast<int>(texA);

		SetTextureAddressModeUV(texAType, texAType);

		DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

		SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

		size = textures.size();
		for (int i = 0; i < size; i++)
		{
			SetUseTextureToShader(i, -1);
		}
	}

	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);

	SetDrawScreen(Application::GetInstance().GetMainScreen());
	ClearDrawScreen();
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawExtendGraph(pos_.x, pos_.y, size_.x, size_.y, screens_[count - 1], true);

	// カメラが初期化されてしまうので、再び更新
	SceneManager::GetInstance().GetCamera().lock()->DrawUpdate();
}

void PixelRenderer::AddShaderInfo(SHADER_NAME name, int constBufFloat4Size)
{
	auto nameIte = shaderNames_.find(name);
	if (nameIte == shaderNames_.end())return;

	SHADER_INFO info;

	info.shader = LoadPixelShader(nameIte->second.c_str());
	info.constBufFloat4Size = constBufFloat4Size;
	info.constBuf = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size);
	info.texAddress = TEXADDRESS::CLAMP;
	shaderInfo_.emplace(name, info);

	// 最初に追加したときのみメインスクリーンをテクスチャとして追加
	if (shaderInfo_.size() == 1)
	{
		SetTextureBuf(name, Application::GetInstance().GetMainScreen());
	}
	else
	{
		if (shaderInfo_.size() <= screenNum_)
		{
			SetTextureBuf(name, screens_[shaderInfo_.size() - 2]);
		}
	}
}

void PixelRenderer::AddConstBuf(SHADER_NAME name, FLOAT4 contBuf)
{
	auto ite = shaderInfo_.find(name);
	if (ite == shaderInfo_.end())return;

	if (ite->second.constBufFloat4Size > ite->second.constBufs.size())
	{
		ite->second.constBufs.emplace_back(contBuf);
	}
}

void PixelRenderer::SetConstBuf(SHADER_NAME name, int idx, FLOAT4 contBuf)
{
	auto ite = shaderInfo_.find(name);
	if (ite == shaderInfo_.end())return;

	if (idx >= ite->second.constBufs.size())
	{
		return;
	}

	ite->second.constBufs[idx] = contBuf;
}

void PixelRenderer::SetTextureBuf(SHADER_NAME name, int handle)
{
	auto ite = shaderInfo_.find(name);
	if (ite == shaderInfo_.end())return;

	const auto& vecIte = std::find(ite->second.textures.begin(),
								   ite->second.textures.end(), handle);
	if (vecIte == ite->second.textures.end())
	{
		ite->second.textures.push_back(handle);
	}
	else
	{
		*vecIte = handle;
	}
}

void PixelRenderer::SetTextureAddress(SHADER_NAME name, TEXADDRESS texA)
{
	auto ite = shaderInfo_.find(name);
	if (ite == shaderInfo_.end())return;

	ite->second.texAddress = texA;
}

void PixelRenderer::InitNames(void)
{
	std::string PATH = "Data/Shader/Pixel/";
	shaderNames_.clear();

	shaderNames_.emplace(SHADER_NAME::MONOTONE, PATH + "Monotone.cso");
	shaderNames_.emplace(SHADER_NAME::SCANLINE, PATH + "ScanLine.cso");
	shaderNames_.emplace(SHADER_NAME::MIRAGE, PATH + "Mirage.cso");
	shaderNames_.emplace(SHADER_NAME::CORONA, PATH + "Corona.cso");
	shaderNames_.emplace(SHADER_NAME::LIGHT_CIRCLE, PATH + "LightCircle.cso");
	shaderNames_.emplace(SHADER_NAME::BRACK_OUT, PATH + "BrackOut.cso");
	shaderNames_.emplace(SHADER_NAME::BLUR, PATH + "Blur.cso");
}
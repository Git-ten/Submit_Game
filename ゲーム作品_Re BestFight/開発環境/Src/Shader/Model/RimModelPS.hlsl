// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float3 g_camera_pos; //カメラの位置
    float g_dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 color;

	//テクスチャーの色を取得
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	
    if (color.a < 0.01f)
    {
        discard;
    }

    
    float3 rgb = color.rgb;
    rgb += PSInput.diffuse.rgb;
    
    return float4(rgb, color.a);
}
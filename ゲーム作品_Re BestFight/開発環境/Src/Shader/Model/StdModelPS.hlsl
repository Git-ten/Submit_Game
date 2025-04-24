// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLitFog

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float4 g_diff_color; //拡散光
    float4 g_ambient_color; //環境光
    float3 g_light_dir; //光の方向
    float g_dummy;
    float3 g_light_color;//光の色
    float g_dummy_2;
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

	//関数の戻り値がラスタライザに渡される
   // return color * g_diff_color;
	
	//ライト方向(ビュー)
    //float3 lightDir = g_common.light[0].direction;
    //return float4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
	
	//ライト方向(ワールド)
	//return float4(g_light_dir.x, g_light_dir.y, g_light_dir.z, 1.0f)
	
	//法線
	//return float4(
	//PSInput.normal.x, PSInput.normal.y, PSInput.normal.z, 1.0f);
	
	
	//同じ方向：-1.0
	//反対方向：+1.0
	//垂直方向：0.0
	
    float lightDot = dot(PSInput.normal, -g_light_dir);
    float3 rgb = color.rgb * (lightDot * g_diff_color.rgb);
    rgb += g_ambient_color.rgb;
    rgb = float3(rgb.r * PSInput.fogFactor, rgb.g * PSInput.fogFactor, rgb.b * PSInput.fogFactor);
    rgb += (g_light_color * PSInput.lightPower);
	
	
    return float4(rgb, color.a);
}
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
    float3 g_lightPos; //ポイントライトの座標
    float g_dummy_1;
    float3 g_light_color; //光の色
    float g_dummy_2;
    float3 g_light_color_1;//予備光の色
    float g_dummy_3;
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
	
	//同じ方向：-1.0
	//反対方向：+1.0
	//垂直方向：0.0
	
    float3 lightDir = PSInput.worldPos - g_lightPos;
    lightDir = normalize(lightDir);
    float lightDot = dot(PSInput.normal, -lightDir);
    lightDot = clamp(lightDot, 0.5f, 1.0f);
    float3 rgb = color.rgb * (lightDot * g_diff_color.rgb);
    rgb += g_ambient_color.rgb;
    rgb = float3(rgb.r * PSInput.fogFactor, rgb.g * PSInput.fogFactor, rgb.b * PSInput.fogFactor);
    rgb += (g_light_color * PSInput.lightPower);
    rgb += (g_light_color_1 * PSInput.lightPower_1);
	
	
    return float4(rgb, color.a);
}
// VS/PS‹¤’Ê
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float4 g_diff_color;
	//float4 g_ambient_color;
	//float4 g_specular_color;
    //float3 g_light_dir;
	//float g_specular_pow;
    //float3 g_camera_pos;
	//float g_dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    return g_diff_color;
}
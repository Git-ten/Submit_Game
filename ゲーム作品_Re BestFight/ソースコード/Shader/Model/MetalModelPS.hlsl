// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float4 g_diff_color;//拡散光
    float4 g_ambient_color;//環境光
	float4 g_specular_color;//反射光
    float3 g_light_dir;//光の方向
	float g_specular_pow;//反射光の強さ
    float3 g_camera_pos;//カメラの位置
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

    //ランバート反射
    //float lightDot = dot(PSInput.normal, -g_light_dir);
    //float3 rgb = color.rgb * (lightDot * g_diff_color.rgb);
    //rgb += g_ambient_color.rgb;
    
    //return float4(rgb, color.a);

    
    float3 dir = g_light_dir + PSInput.normal + PSInput.normal;
    dir = normalize(dir);
    dir += dir * 0.1;
    float3 vec = g_camera_pos - PSInput.worldPos;
    vec = normalize(vec);
	
    float refDot = dot(dir, vec);
	
    float refDotEx = pow(refDot, g_specular_pow);

    float3 rgb = color.rgb;
    rgb *= g_diff_color.rgb + (refDotEx * g_specular_color.rgb) + g_ambient_color.rgb;
    return float4(rgb, color.a);
}
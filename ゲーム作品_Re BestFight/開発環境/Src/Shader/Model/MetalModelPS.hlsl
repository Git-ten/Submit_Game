// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float4 g_diff_color;//�g�U��
    float4 g_ambient_color;//����
	float4 g_specular_color;//���ˌ�
    float3 g_light_dir;//���̕���
	float g_specular_pow;//���ˌ��̋���
    float3 g_camera_pos;//�J�����̈ʒu
	float g_dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 color;

	//�e�N�X�`���[�̐F���擾
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	
    if (color.a < 0.01f)
    {
        discard;
    }

    //�����o�[�g����
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
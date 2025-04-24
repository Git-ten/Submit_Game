// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParm : register(b4)
{
    float g_time; //�o�ߎ���
    float3 g_dummy;
}

SamplerState g_SrcSampler
{
    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 color;
      //�X�N���[������
    float speed = 0.8f;
    PSInput.uv.x += g_time * speed;

	//�e�N�X�`���[�̐F���擾
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	
    if (color.a < 0.01f)
    {
        discard;
    }

    
    float3 rgb = color.rgb; 
    return float4(rgb, color.a);
}
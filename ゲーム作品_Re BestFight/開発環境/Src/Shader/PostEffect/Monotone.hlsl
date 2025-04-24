#include"../Common/Pixel/PixelShader2DHeader.hlsli"

//cbuffer cbColor : register(b0)
//{
//    float4 g_color;
//    float2 g_size; //�摜�T�C�Y
//    float g_scale; // ���U�C�N�X�P�[��
//    float dummy;
//}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV���W�ƃe�N�X�`�����Q�Ƃ��āA�Œ�ȐF���擾����
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        //�`�悵�Ȃ�
        discard;
    }
    
    //float4 test = float4(1.0f, 1.0f, 0.8f, 0.6f);
    //test *= 0.2f;
    
    float4 dstColor = srcCol;
    float m = dot(dstColor.rgb, float3(0.299, 0.587, 0.114)); 
    //��L�Ɠ�������

    
    //���m�N��
    return float4(m, m, m, 1.0f);
    //�Z�s�A��
   // return float4(m, m * 0.898, m* 0.8,1.0f);
    //�m�C�Y
    //float noise = frac(sin(dot(PSInput.uv , float2(12.9898, 78.233))) * 43758.5453);
    //return float4(noise, noise, noise, 1.0f) * dstColor;
}
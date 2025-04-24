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
   
    float4 dstColor = srcCol * 0.5f;
    
    //���m�N��
    return dstColor;
}
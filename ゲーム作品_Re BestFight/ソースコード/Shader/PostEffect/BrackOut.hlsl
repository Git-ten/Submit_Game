#include"../Common/Pixel/PixelShader2DHeader.hlsli"

//cbuffer cbColor : register(b0)
//{
//    float4 g_color;
//    float2 g_size; //画像サイズ
//    float g_scale; // モザイクスケール
//    float dummy;
//}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標とテクスチャを参照して、最低な色を取得する
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        //描画しない
        discard;
    }
   
    float4 dstColor = srcCol * 0.5f;
    
    //モノクロ
    return dstColor;
}
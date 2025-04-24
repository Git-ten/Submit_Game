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
    
    //float4 test = float4(1.0f, 1.0f, 0.8f, 0.6f);
    //test *= 0.2f;
    
    float4 dstColor = srcCol;
    float m = dot(dstColor.rgb, float3(0.299, 0.587, 0.114)); 
    //上記と同じ処理

    
    //モノクロ
    return float4(m, m, m, 1.0f);
    //セピア調
   // return float4(m, m * 0.898, m* 0.8,1.0f);
    //ノイズ
    //float noise = frac(sin(dot(PSInput.uv , float2(12.9898, 78.233))) * 43758.5453);
    //return float4(noise, noise, noise, 1.0f) * dstColor;
}
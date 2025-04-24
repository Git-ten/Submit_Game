#include"../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbP : register(b4)
{
    float g_time;
}

SamplerState g_SrcSampler
{
    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    
    //UV座標とテクスチャを参照して、最低な色を取得する
    float4 srcCol = tex.Sample(texSampler, uv);
    
    clip(srcCol.a);
    
     //スクロール処理
    uv.y += g_time * 0.1f;
    srcCol.rgb -= abs(sin(uv.y * 60.0f)) * 0.10f;
    return srcCol;
}
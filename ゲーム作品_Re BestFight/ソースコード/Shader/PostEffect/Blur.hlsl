
#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_color;
    //float time;
    //float speed;
    //float2 pad;
}

//Texture2D noiseTex : register(t1);
//SamplerState noiseSampler : register(s1);

float4 main(PS_INPUT psIn) : SV_TARGET
{
    float4 texCol = tex.Sample(texSampler, psIn.uv);
    
    float2 uv = psIn.uv;
    
    float cnt = 1.0f;
    float3 col = texCol.rgb;
    float power = 0.002f;
    
    col += tex.Sample(texSampler, float2(uv.x + power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x - power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y + power * cnt)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y - power * cnt++)).rgb;

    
    col += tex.Sample(texSampler, float2(uv.x + power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x - power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y + power * cnt)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y - power * cnt++)).rgb;
    
    
    col += tex.Sample(texSampler, float2(uv.x + power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x - power * cnt, uv.y)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y + power * cnt)).rgb;
    col += tex.Sample(texSampler, float2(uv.x, uv.y - power * cnt++)).rgb;
    
    col /= cnt * 4.0f + 1.0f;
    
    //float4 noiseCol = noiseTex.Sample(noiseSampler, psIn.uv + speed * time);
    
    //float d = dot(texCol.rgb, noiseCol.rgb);
    
    clip(texCol.a);
    
    
    float4 finCol = texCol;
    finCol.rgb = col;
    finCol.a = texCol.a;
    
    return finCol;
}
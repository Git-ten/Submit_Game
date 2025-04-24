// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME
// OUT
#define VS_OUTPUT VertexToPixelLitFog
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbParm : register(b7)
{
    float3 g_camera_pos;//カメラ座標
    float g_dummy1;
    float2 g_dummy2;
    float g_fog_start; //フォグのスタート地点
    float g_fog_end; //フォグのエンド地点
    float3 g_lightPos; //ポイントライトの座標
    float g_power; //ポイントライトの強さ
    float3 g_lightPos_1;//予備のポイントライトの座標
    float g_power_1; //予備のポイントライトの強さ
}

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;

    // 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    
    // float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
    
    // ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    ret.worldPos = lWorldPosition.xyz;

    // ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    // ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
    // 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )


    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
    // UV座標
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;
    // 法線
    //ret.normal = VSInput.norm;
    ret.normal = normalize(
        mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    // ディフューズカラー
    ret.diffuse = VSInput.diffuse;
    // ライト方向(ローカル)
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    // ライトから見た座標
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
    //フォグの強さ
    float dis = length(lWorldPosition.xyz - g_camera_pos);
    float factor = (g_fog_end - length(dis)) / (g_fog_end - g_fog_start);
    ret.fogFactor = clamp(factor, 0.0f, 1.0f);
    
    //光の強さ
    float lightDis = length(lWorldPosition.xyz - g_lightPos);
    float lightPower = lightDis / g_power;
    ret.lightPower = clamp(lightPower, 0.0f, 1.0f);
    ret.lightPower = 1.0f - ret.lightPower;
    
    //予備光
    lightDis = length(lWorldPosition.xyz - g_lightPos_1);
    lightPower = lightDis / g_power_1;
    ret.lightPower_1 = clamp(lightPower, 0.0f, 1.0f);
    ret.lightPower_1 = 1.0f - ret.lightPower_1;
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )


    // 出力パラメータを返す
    return ret;
}
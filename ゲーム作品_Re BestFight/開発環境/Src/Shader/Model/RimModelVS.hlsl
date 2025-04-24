// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbParm : register(b7)
{
    float3 g_camera_pos; //カメラの位置
    float g_dummy;
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
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )

    //リムライト処理-----------------------------
    float3 targetdir = ret.worldPos - g_camera_pos;
    targetdir = normalize(targetdir);
    
    //内積を取る
    float cDot = dot(ret.normal, targetdir);
    cDot = abs(cDot);
    cDot = 1.0 - cDot;
    
    float rimDot = pow(cDot, 2.0f); // 強弱を強める
    float4 rimColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
    ret.diffuse = rimColor * rimDot;
    //-------------------------------------------
    
    // 出力パラメータを返す
    return ret;
}
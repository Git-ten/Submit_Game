
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

//追加構造体
//フォグ
struct VertexToPixelLitFog
{
    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 vwPos : TEXCOORD1; // 座標( ビュー座標 )
    float3 normal : TEXCOORD2; // 法線( ビュー座標 )
    float4 diffuse : COLOR0; // ディフューズカラー
    float3 lightDir : TEXCOORD3; // ライト方向(ローカル)
    float3 lightAtPos : TEXCOORD4; // ライトから見た座標
    float3 worldPos : TEXCOORD5; //ワールド座標
    float fogFactor : TEXCOORD6; //フォグの強さ(0.0:フォグが濃い,1.0:フォグが薄い)
    float lightPower : TEXCOORD7;//ライトの強さ
    float lightPower_1 : TEXCOORD8;//ライトの強さ
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float3 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
	float3 normal       : TEXCOORD2;	// 法線( ビュー座標 )
	float4 diffuse      : COLOR0;		// ディフューズカラー
	float3 lightDir     : TEXCOORD3;	// ライト方向(ローカル)
	float3 lightAtPos   : TEXCOORD4;	// ライトから見た座標
    float3 worldPos		: TEXCOORD5;	//ワールド座標
    float lightPower    : TEXCOORD6; //ライトの強さ
    float lightPower_1  : TEXCOORD7; //ライトの強さ
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float4 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
};

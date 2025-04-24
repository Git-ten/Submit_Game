
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

//�ǉ��\����
//�t�H�O
struct VertexToPixelLitFog
{
    float4 svPos : SV_POSITION; // ���W( �v���W�F�N�V������� )
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 vwPos : TEXCOORD1; // ���W( �r���[���W )
    float3 normal : TEXCOORD2; // �@��( �r���[���W )
    float4 diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float3 lightDir : TEXCOORD3; // ���C�g����(���[�J��)
    float3 lightAtPos : TEXCOORD4; // ���C�g���猩�����W
    float3 worldPos : TEXCOORD5; //���[���h���W
    float fogFactor : TEXCOORD6; //�t�H�O�̋���(0.0:�t�H�O���Z��,1.0:�t�H�O������)
    float lightPower : TEXCOORD7;//���C�g�̋���
    float lightPower_1 : TEXCOORD8;//���C�g�̋���
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// ���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	// �e�N�X�`�����W
	float3 vwPos		: TEXCOORD1;	// ���W( �r���[���W )
	float3 normal       : TEXCOORD2;	// �@��( �r���[���W )
	float4 diffuse      : COLOR0;		// �f�B�t���[�Y�J���[
	float3 lightDir     : TEXCOORD3;	// ���C�g����(���[�J��)
	float3 lightAtPos   : TEXCOORD4;	// ���C�g���猩�����W
    float3 worldPos		: TEXCOORD5;	//���[���h���W
    float lightPower    : TEXCOORD6; //���C�g�̋���
    float lightPower_1  : TEXCOORD7; //���C�g�̋���
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// ���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	// �e�N�X�`�����W
	float4 vwPos		: TEXCOORD1;	// ���W( �r���[���W )
};

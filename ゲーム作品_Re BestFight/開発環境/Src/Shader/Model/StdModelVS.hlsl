// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME
// OUT
#define VS_OUTPUT VertexToPixelLitFog
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbParm : register(b7)
{
    float3 g_camera_pos;//�J�������W
    float g_dummy1;
    float2 g_dummy2;
    float g_fog_start; //�t�H�O�̃X�^�[�g�n�_
    float g_fog_end; //�t�H�O�̃G���h�n�_
    float3 g_lightPos; //�|�C���g���C�g�̍��W
    float g_power; //�|�C���g���C�g�̋���
    float3 g_lightPos_1;//�\���̃|�C���g���C�g�̍��W
    float g_power_1; //�\���̃|�C���g���C�g�̋���
}

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;

    // ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    
    // float3 �� float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
    
    // ���[�J�����W�����[���h���W�ɕϊ�(����)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    ret.worldPos = lWorldPosition.xyz;

    // ���[���h���W���r���[���W�ɕϊ�
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    // �r���[���W���ˉe���W�ɕϊ�
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
    // ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �I�� )


    // ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �J�n )
    // UV���W
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;
    // �@��
    //ret.normal = VSInput.norm;
    ret.normal = normalize(
        mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    // �f�B�t���[�Y�J���[
    ret.diffuse = VSInput.diffuse;
    // ���C�g����(���[�J��)
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    // ���C�g���猩�����W
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
    //�t�H�O�̋���
    float dis = length(lWorldPosition.xyz - g_camera_pos);
    float factor = (g_fog_end - length(dis)) / (g_fog_end - g_fog_start);
    ret.fogFactor = clamp(factor, 0.0f, 1.0f);
    
    //���̋���
    float lightDis = length(lWorldPosition.xyz - g_lightPos);
    float lightPower = lightDis / g_power;
    ret.lightPower = clamp(lightPower, 0.0f, 1.0f);
    ret.lightPower = 1.0f - ret.lightPower;
    
    //�\����
    lightDis = length(lWorldPosition.xyz - g_lightPos_1);
    lightPower = lightDis / g_power_1;
    ret.lightPower_1 = clamp(lightPower, 0.0f, 1.0f);
    ret.lightPower_1 = 1.0f - ret.lightPower_1;
    // ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �I�� )


    // �o�̓p�����[�^��Ԃ�
    return ret;
}
// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_4FRAME
// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix

cbuffer cbParm : register(b7)
{
    float3 g_lightPos; //�|�C���g���C�g�̍��W
    float g_power; //�|�C���g���C�g�̋���
    float3 g_lightPos_1; //�\���̃|�C���g���C�g�̍��W
    float g_power_1; //�\���̃|�C���g���C�g�̋���
}

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;

    
// �X�L�����b�V���p�̃��[�J���˃��[���h�ϊ��s����쐬+++( �J�n )
    int4 lBoneIdx;
    float4 lL_W_Mat[3];
    float4 lWeight;
    // BONE4
    lBoneIdx = VSInput.blendIndices0;
    lWeight = VSInput.blendWeight0;
    lL_W_Mat[0] = L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
    lL_W_Mat[1] = L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
    lL_W_Mat[2] = L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
    lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
    lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
    lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;

    lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;
    //// BONE8
    //lBoneIdx = VSInput.blendIndices1;
    //lWeight = VSInput.blendWeight1;
    //lL_W_Mat[0] += L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
    //lL_W_Mat[1] += L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
    //lL_W_Mat[2] += L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
    //lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
    //lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
    //lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
    //lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
    //lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
    //lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
    //lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
    //lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
    //lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;
// �X�L�����b�V���p�̃��[�J���˃��[���h�ϊ��s����쐬+++( �I�� )
    
    
    
    // ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    
    // float3 �� float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
    
    // ���[�J�����W�����[���h���W�ɕϊ�(����)
    lWorldPosition.w = 1.0f;
    //lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    lWorldPosition.x = dot(lLocalPosition, lL_W_Mat[0]);
    lWorldPosition.y = dot(lLocalPosition, lL_W_Mat[1]);
    lWorldPosition.z = dot(lLocalPosition, lL_W_Mat[2]);
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
    //ret.normal = normalize(
    //    mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    ret.normal.x = dot(VSInput.norm, lL_W_Mat[0]);
    ret.normal.y = dot(VSInput.norm, lL_W_Mat[1]);
    ret.normal.z = dot(VSInput.norm, lL_W_Mat[2]);
    
    // �f�B�t���[�Y�J���[
    ret.diffuse = VSInput.diffuse;
    // ���C�g����(���[�J��)
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    // ���C�g���猩�����W
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
    
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
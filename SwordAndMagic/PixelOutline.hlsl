// FBX�p�s�N�Z���V�F�[�_ (FbxModelPixel.hlsl)

// �O���[�o��
cbuffer global : register(b0) {
	matrix	g_WorldViewProj;	// ���[���h�~�r���[�~�ˉe�s��
	matrix	g_World;			// ���[���h�s��
	float4	g_cameraPos;		// ���_���W(���[���h���)
	float4	g_lightDir;			// ��������(���[���h���)
	float4	g_lightAmbient;		// ����
	float4	g_lightDiffuse;		// �g�U��
	float4	g_lightSpecular;	// ���ʔ��ˌ�
};

// �}�e���A��
cbuffer global2 : register(b1) {
	float4	g_Ambient;			// ���F
	float4	g_Diffuse;			// �g�U�F
	float4	g_Specular;			// ���ʔ��ːF
	float4	g_Emissive;			// �����F
};

Texture2D    g_texture		: register(t0);	// �e�N�X�`��
Texture2D    g_texEmissive	: register(t1);	// �����e�N�X�`��
SamplerState g_sampler		: register(s0);	// �T���v��

// �p�����[�^
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
	float3	wPos		:TEXCOORD3;
};

//struct LightInfo {
//	float3 pos;
//	float type;
//	float3 direction;
//	float power;
//	float4 value;
//};
//
//cbuffer Light : register(b2)
//{
//	LightInfo light[100];
//};

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	return float4(0,0,0,1);
}

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

struct LightInfo {
	float3 pos;
	float type;
	float3 direction;
	float power;
	float4 value;
};

cbuffer Light : register(b2)
{
	LightInfo light[100];
};

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;
	float dd = 1.0f;
	if (g_Ambient.a != 0.0f) {									// �e�N�X�`���L��
		float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
		Diff *= TexDiff.rgb;
		Alpha *= TexDiff.a;
	}
	if (Alpha <= 0.0f) discard;

	if (g_lightDir.x != 0.0f || g_lightDir.y != 0.0f || g_lightDir.z != 0.0f) {
		float3 L = normalize(-g_lightDir.xyz);					// �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);						// �@���x�N�g��
		float3 V = normalize(g_cameraPos.xyz - input.PosForPS);	// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);							// �n�[�t�x�N�g��
		dd = dot(N, L);
		Diff = g_lightAmbient.rgb * g_Ambient.rgb +
			g_lightDiffuse.rgb * Diff * saturate(dot(L, N));	// �g�U�F + ���F
		float3 Spec = g_Specular.rgb * g_lightSpecular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a);				// ���ʔ��ːF
		Diff += Spec;
	}

	float3 Emis = g_Emissive.rgb;								// �����F
	if (g_Emissive.a != 0.0f) {
		float4 Emis4 = g_texEmissive.Sample(g_sampler, input.Tex);
		Emis *= (Emis4.rgb * Emis4.a);
	}
	Diff += Emis;

	float totalDiffuse = 0.0f;

	for (int i = 0; i < 100; i++) {
		float3 dir = light[i].pos - input.wPos.xyz;

		float len = length(dir);

		dir = normalize(dir);

		float d = dot(input.Normal, dir);

		d = d * 0.5f + 0.5f;

		//���̃p���[����Z����
		d *= light[i].power;

		int isSpot = step(1, light[i].type);

		//�����ɉ����Č����キ����(����)
		//���`�Ō��������߂�̂͌����ł͂Ȃ�
		float attenuation = saturate(1.0f / (1.0f + 10 * len + 25 * len * len * len));
		float attenuationSave = attenuation;
		//d *= attenuation;

		float pi = 3.1415926535f; //�~����
		float rad = pi / 180.0f; //1���̃��W�A���p

		float3 spotL = normalize(light[i].direction);

		float3 spotV = -dir;

		float spotD = dot(spotL, spotV);

		//2�̃x�N�g���̂Ȃ��p���X�|�b�g���C�g�̏Ǝ˔͈͂Ɏ��܂邩����
		//�p�x�Ŕ�����s���Ȃ�A���ς̌v�Z���ʂ���
		//�p�x�����߂Ȃ��Ƃ����Ȃ����Aacos�̌v�Z���d���̂ŁA
		//�X�|�b�g���C�g�̏Ǝ˔͈͂�cos�l�ɕϊ����Ĕ�r
		float spotInner = light[i].value.z;

		spotInner = cos(spotInner * 0.5f * rad);

		float spotOuter = light[i].value.w;

		spotOuter = cos(spotOuter * 0.5f * rad);

		//�l���������قǃX�|�b�g���C�g�̏Ǝ˔͈͊O
		attenuation = step(spotOuter, spotD) * attenuation;

		//��������O���܂ł̊p�x�̍���1�Ƃ����Ƃ��A���݂̊p�x�̊��������߂�
		float diffAngle = spotInner - spotOuter;
		float diffVecAngle = spotD - spotOuter;
		float rate = diffVecAngle / diffAngle;

		attenuation *= pow(saturate(rate), 5);

		totalDiffuse += d * attenuation * 2 * isSpot + d * attenuationSave * (1 - isSpot);
	}

	return float4(Diff * dd * totalDiffuse * 10000, Alpha);
}

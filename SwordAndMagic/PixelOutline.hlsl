// FBX用ピクセルシェーダ (FbxModelPixel.hlsl)

// グローバル
cbuffer global : register(b0) {
	matrix	g_WorldViewProj;	// ワールド×ビュー×射影行列
	matrix	g_World;			// ワールド行列
	float4	g_cameraPos;		// 視点座標(ワールド空間)
	float4	g_lightDir;			// 光源方向(ワールド空間)
	float4	g_lightAmbient;		// 環境光
	float4	g_lightDiffuse;		// 拡散光
	float4	g_lightSpecular;	// 鏡面反射光
};

// マテリアル
cbuffer global2 : register(b1) {
	float4	g_Ambient;			// 環境色
	float4	g_Diffuse;			// 拡散色
	float4	g_Specular;			// 鏡面反射色
	float4	g_Emissive;			// 発光色
};

Texture2D    g_texture		: register(t0);	// テクスチャ
Texture2D    g_texEmissive	: register(t1);	// 発光テクスチャ
SamplerState g_sampler		: register(s0);	// サンプラ

// パラメータ
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
// ピクセルシェーダ
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	return float4(0,0,0,1);
}

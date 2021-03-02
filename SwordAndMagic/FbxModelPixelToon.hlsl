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
// ピクセルシェーダ
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float3 L = normalize(-g_lightDir.xyz);					// 光源へのベクトル
	float3 N = normalize(input.Normal);						// 法線ベクトル
	

	float totalDiffuse = 0.0f;

	for (int i = 0; i < 100; i++) {
		float3 dir = light[i].pos - input.wPos.xyz;

		float len = length(dir);

		dir = normalize(dir);

		float d = dot(input.Normal, dir);

		d = d * 0.5f + 0.5f;

		//光のパワーを乗算する
		d *= light[i].power;

		int isSpot = step(1, light[i].type);

		//距離に応じて光を弱くする(減衰)
		//線形で減衰を求めるのは厳密ではない
		float attenuation = saturate(1.0f / (1.0f + light[i].value.x * len + light[i].value.y * len * len * len));
		float attenuationSave = attenuation;
		//d *= attenuation;

		float pi = 3.1415926535f; //円周率
		float rad = pi / 180.0f; //1°のラジアン角

		float3 spotL = normalize(light[i].direction);

		float3 spotV = -dir;

		float spotD = dot(spotL, spotV);

		//2つのベクトルのなす角がスポットライトの照射範囲に収まるか判定
		//角度で判定を行うなら、内積の計算結果から
		//角度を求めないといけないが、acosの計算が重いので、
		//スポットライトの照射範囲をcos値に変換して比較
		float spotInner = light[i].value.z;

		spotInner = cos(spotInner * 0.5f * rad);

		float spotOuter = light[i].value.w;

		spotOuter = cos(spotOuter * 0.5f * rad);

		//値が小さいほどスポットライトの照射範囲外
		attenuation = step(spotOuter, spotD) * attenuation;

		//内側から外側までの角度の差を1としたとき、現在の角度の割合を求める
		float diffAngle = spotInner - spotOuter;
		float diffVecAngle = spotD - spotOuter;
		float rate = diffVecAngle / diffAngle;

		attenuation *= pow(saturate(rate), 5);

		totalDiffuse += d * attenuation * 2 * isSpot + d * attenuationSave * (1 - isSpot);
	}

	float dd = 1.0f;
	dd = dot(L, N);

	dd = dd * 0.5f + 0.5f;

	dd = (int)(dd * 5) / 5.0f;

	return float4(totalDiffuse * dd * Diff * 10000, g_Diffuse.a);
	//return float4(Diff * totalDiffuse, g_Diffuse.a);
}

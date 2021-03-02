#include "ShaderLight.h"
#include "GraphicsManager.h"



/**
 * @brief コンストラクタ
 */
ShaderLightBuffer::ShaderLightBuffer() {
	m_RegCount = 0;
}

/**
 * @brief デストラクタ
 */
ShaderLightBuffer::~ShaderLightBuffer() {

}

/**
 * @brief 光源の登録。
 * @param[in] light 光源情報
 * @return 光源の通し番号(溢れたら-1が返る)
 */
int ShaderLightBuffer::Register(LightInfo light) {
	for (int i = 0; i < LIGHT_NUM; i++) {
		if (UsingNumber[i]) continue;
		m_Lights[i] = light;
		UsingNumber[i] = true;
		m_RegCount++;
		return i;
	}
}

/**
 * @brief 光源の登録解除。
 * @param[in] lightNo 光源通し番号
 */
void ShaderLightBuffer::Unregister(int lightNo) {
	if (!UsingNumber[lightNo]) return;
	UsingNumber[lightNo] = false;
	m_RegCount--;
}


/**
 * @brief 光源データのバッファへの流し込み
 * @param[in] pConstantBuffers Constantバッファ
 * @param[in] ppConstantBuffers Constantバッファのアドレス
 * @return 光源の通し番号(溢れたら-1が返る)
 */
void ShaderLightBuffer::UpdateLight(ID3D11Buffer* pConstantBuffers, ID3D11Buffer** ppConstantBuffers) {
	if (m_RegCount <= 0) return;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(GraphicsManager::GetInstance().GetDeviceContext()->Map(pConstantBuffers, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)m_Lights, sizeof(m_Lights));
		GraphicsManager::GetInstance().GetDeviceContext()->Unmap(pConstantBuffers, 0);
	}
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(2, 1, ppConstantBuffers);
}

/**
 * @brief 光源データ取得
 * @param[in] lightNo 光源番号
 * @return 光源への参照
 */
LightInfo& ShaderLightBuffer::GetLight(int lightNo) {
	return m_Lights[lightNo];
}
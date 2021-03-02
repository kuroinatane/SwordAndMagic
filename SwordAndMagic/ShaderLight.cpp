#include "ShaderLight.h"
#include "GraphicsManager.h"



/**
 * @brief �R���X�g���N�^
 */
ShaderLightBuffer::ShaderLightBuffer() {
	m_RegCount = 0;
}

/**
 * @brief �f�X�g���N�^
 */
ShaderLightBuffer::~ShaderLightBuffer() {

}

/**
 * @brief �����̓o�^�B
 * @param[in] light �������
 * @return �����̒ʂ��ԍ�(��ꂽ��-1���Ԃ�)
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
 * @brief �����̓o�^�����B
 * @param[in] lightNo �����ʂ��ԍ�
 */
void ShaderLightBuffer::Unregister(int lightNo) {
	if (!UsingNumber[lightNo]) return;
	UsingNumber[lightNo] = false;
	m_RegCount--;
}


/**
 * @brief �����f�[�^�̃o�b�t�@�ւ̗�������
 * @param[in] pConstantBuffers Constant�o�b�t�@
 * @param[in] ppConstantBuffers Constant�o�b�t�@�̃A�h���X
 * @return �����̒ʂ��ԍ�(��ꂽ��-1���Ԃ�)
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
 * @brief �����f�[�^�擾
 * @param[in] lightNo �����ԍ�
 * @return �����ւ̎Q��
 */
LightInfo& ShaderLightBuffer::GetLight(int lightNo) {
	return m_Lights[lightNo];
}
/**
 * @file GraphicsManager.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Main.h"
#include "Singleton.h"
#include <wrl/client.h>



class C_Scene;

/**
 * @class GraphicsManager
 * @brief �`��n�̊Ǘ��N���X�B
 *		  ���������ۂɕ`����s���̂̓��f���⃁�b�V���̃R���|�[�l���g�̓����̏����ƂȂ�B
 */
class GraphicsManager : public Singleton<GraphicsManager>
{
public:
	/**
	* @enum BLENDSTATE
	* @brief �������@�̒�` 
	*/
	enum BLENDSTATE {
		BS_NONE = 0,		// ��������������
		BS_ALPHABLEND,		// ����������
		BS_ADDITIVE,		// ���Z����
		BS_SUBTRACTION,		// ���Z����
		MAX_BLENDSTATE
	};

	/**
	* @enum CULLMODE
	* @brief �J�����O���@�̒�`
	*/
	enum CULLMODE {
		CULLMODE_NONE = 0,	// �J�����O���Ȃ�
		CULLMODE_CW,		// �O�ʃJ�����O
		CULLMODE_CCW,		// �w�ʃJ�����O

		MAX_CULLMODE
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;				//!< @brief �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		//!< @brief �f�o�C�X �R���e�L�X�g
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;			//!< @brief �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	//!< @brief �t���[���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencilTexture;	//!< @brief Z�o�b�t�@�p������
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	//!< @brief Z�o�b�t�@
	UINT						m_uSyncInterval;							//!< @brief �������� (0:��, 1:�L)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRs[MAX_CULLMODE];	//!< @brief ���X�^���C�U �X�e�[�g
	Microsoft::WRL::ComPtr <ID3D11BlendState>		m_pBlendState[MAX_BLENDSTATE];//!< @brief �u�����h �X�e�[�g
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState>	m_pDSS[2];			//!< @brief Z�o�b�t�@/�X�e���V�� �X�e�[�g
	float	m_nWidth = SCREEN_WIDTH;
	float	m_nHeight = SCREEN_HEIGHT;
private:
	HRESULT CreateBackBuffer();

public:
	HRESULT Init(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Uninit();
	void Update();
	void Draw(C_Scene* pScene);

	/**
	* @brief �f�o�C�X���̎擾
	* @return �f�o�C�X���̐��|�C���^
	*/
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }

	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	* @return �f�o�C�X�R���e�L�X�g�̐��|�C���^
	*/
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	void SetZBuffer(bool bEnable);
	void SetZWrite(bool bEnable);
	void SetBlendState(int nBlendState);
	void SetCullMode(int nCullMode);
};

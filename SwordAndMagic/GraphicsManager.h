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
 * @brief 描画系の管理クラス。
 *		  ただし実際に描画を行うのはモデルやメッシュのコンポーネントの内部の処理となる。
 */
class GraphicsManager : public Singleton<GraphicsManager>
{
public:
	/**
	* @enum BLENDSTATE
	* @brief 合成方法の定義 
	*/
	enum BLENDSTATE {
		BS_NONE = 0,		// 半透明合成無し
		BS_ALPHABLEND,		// 半透明合成
		BS_ADDITIVE,		// 加算合成
		BS_SUBTRACTION,		// 減算合成
		MAX_BLENDSTATE
	};

	/**
	* @enum CULLMODE
	* @brief カリング方法の定義
	*/
	enum CULLMODE {
		CULLMODE_NONE = 0,	// カリングしない
		CULLMODE_CW,		// 前面カリング
		CULLMODE_CCW,		// 背面カリング

		MAX_CULLMODE
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;				//!< @brief デバイス
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		//!< @brief デバイス コンテキスト
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;			//!< @brief スワップチェーン
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	//!< @brief フレームバッファ
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencilTexture;	//!< @brief Zバッファ用メモリ
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	//!< @brief Zバッファ
	UINT						m_uSyncInterval;							//!< @brief 垂直同期 (0:無, 1:有)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRs[MAX_CULLMODE];	//!< @brief ラスタライザ ステート
	Microsoft::WRL::ComPtr <ID3D11BlendState>		m_pBlendState[MAX_BLENDSTATE];//!< @brief ブレンド ステート
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState>	m_pDSS[2];			//!< @brief Zバッファ/ステンシル ステート
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
	* @brief デバイス情報の取得
	* @return デバイス情報の生ポインタ
	*/
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }

	/**
	* @brief デバイスコンテキストの取得
	* @return デバイスコンテキストの生ポインタ
	*/
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	void SetZBuffer(bool bEnable);
	void SetZWrite(bool bEnable);
	void SetBlendState(int nBlendState);
	void SetCullMode(int nCullMode);
};

/**
 * @file Curtain.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "mesh.h"


constexpr int PARTICLE_HORIZONTAL = 50;				//!< @brief 粒子　横方向総数
constexpr int PARTICLE_VERTICAL = 50;				//!< @brief 粒子　縦方向総数
constexpr int SPRING_NUMS = (PARTICLE_HORIZONTAL*(PARTICLE_VERTICAL + 1) + PARTICLE_VERTICAL*(PARTICLE_HORIZONTAL + 1) + PARTICLE_VERTICAL*PARTICLE_HORIZONTAL * 2);	//!< @brief スプリング総数

constexpr float SPRING_DEFAULT_LENGTH =5.0f;			//!< @brief バネ自然長
constexpr float SPRING_COEFFICIENT	=40.0f;			//!< @brief バネ係数
constexpr float SPRING_MASS			=2.0f;			//!< @brief バネ質量
constexpr float ATT_COEFFICIENT		=0.5f;			//!< @brief 減衰係数(Attenuation)
constexpr float DELTA_TIME			=0.1f;			//!< @brief 積分単位時間
constexpr float WIND_FORCE_DEF_X	=40.0f;			//!< @brief 風力Ｘ成分
constexpr float WIND_FORCE_DEF_Y	=20.0f;			//!< @brief 風力Ｙ成分
constexpr float WIND_FORCE_DEF_Z	=30.0f;			//!< @brief 風力Ｚ成分

/**
 * @class Curtain.h
 * @brief カーテン実装用のコンポーネント。
 */
class CurtainComponent : public C_Mesh
{
private:
	struct WINDFORCE {
		int		status;				//!< @brief ステータス
		DirectX::XMFLOAT3 force;				//!< @brief 風力ベクトル
	};

	struct PARTICLE {
		bool	onLockX;				//!< @brief 固定フラグ
		bool	onLockY;				//!< @brief 固定フラグ
		bool	onLockZ;				//!< @brief 固定フラグ
		DirectX::XMFLOAT3	position;			//!< @brief 位置座標 (Position)
		DirectX::XMFLOAT3	velocity;			//!< @brief 速度 (Velocity)
		DirectX::XMFLOAT3	acceleration;		//!< @brief 加速度 (Acceleration)
		DirectX::XMFLOAT3	resultant;			//!< @brief 合力 (Resultant)
		DirectX::XMFLOAT3	gravity;			//!< @brief 重力 (Gravity)
	};

	struct P_REF {
		int		horz;				//!< @brief 横方向インデックス
		int		vert;				//!< @brief 縦方向インデックス
	};

	struct SPRING {
		P_REF	p1;					//!< @brief 接続されている粒子１
		P_REF	p2;					//!< @brief 接続されている粒子２
		float	length;				//!< @brief 自然長
	};

	enum CURTAIN_STATE {
		CURTAIN_CLOSED,
		CURTAIN_CLOSING,
		CURTAIN_OPENING,
		CURTAIN_OPENED
	};

	struct CLOTH {
		int			status;														//!< @brief ステータス
		PARTICLE	Particle[PARTICLE_VERTICAL + 1][PARTICLE_HORIZONTAL + 1];		//!< @brief 構成粒子
		SPRING		Spring[SPRING_NUMS];										//!< @brief 粒子間のバネ
	};
	CLOTH		m_Cloth;					//!< @brief 布オブジェクト
	WINDFORCE	m_WindForce;				//!< @brief 風力オブジェクト
	bool		m_bWind = false;			//!< @brief 風の有効/無効
	bool		m_isMoving = false;			//!< @brief カーテンの開閉
	CURTAIN_STATE m_curtainState = CURTAIN_CLOSED; //!< @brief カーテンの状態
	bool m_isTitle;	//!< @brief タイトル画面ならばボタンで開閉、それ以外はオート開閉
	int m_dir;	//!< @brief カーテンの開く方向
	int m_moveCnt = 0; //!< @brief カーテン移動用カウント
public:
	CurtainComponent(C_GameObject* obj, int dir, bool isTitle = true);
	~CurtainComponent();
	void Update();
	void Draw();
	void CurtainUpdate();
	void StartOpen();
	void StartClose();
};


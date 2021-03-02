/**
 * @file Curtain.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "mesh.h"


constexpr int PARTICLE_HORIZONTAL = 50;				//!< @brief ���q�@����������
constexpr int PARTICLE_VERTICAL = 50;				//!< @brief ���q�@�c��������
constexpr int SPRING_NUMS = (PARTICLE_HORIZONTAL*(PARTICLE_VERTICAL + 1) + PARTICLE_VERTICAL*(PARTICLE_HORIZONTAL + 1) + PARTICLE_VERTICAL*PARTICLE_HORIZONTAL * 2);	//!< @brief �X�v�����O����

constexpr float SPRING_DEFAULT_LENGTH =5.0f;			//!< @brief �o�l���R��
constexpr float SPRING_COEFFICIENT	=40.0f;			//!< @brief �o�l�W��
constexpr float SPRING_MASS			=2.0f;			//!< @brief �o�l����
constexpr float ATT_COEFFICIENT		=0.5f;			//!< @brief �����W��(Attenuation)
constexpr float DELTA_TIME			=0.1f;			//!< @brief �ϕ��P�ʎ���
constexpr float WIND_FORCE_DEF_X	=40.0f;			//!< @brief ���͂w����
constexpr float WIND_FORCE_DEF_Y	=20.0f;			//!< @brief ���͂x����
constexpr float WIND_FORCE_DEF_Z	=30.0f;			//!< @brief ���͂y����

/**
 * @class Curtain.h
 * @brief �J�[�e�������p�̃R���|�[�l���g�B
 */
class CurtainComponent : public C_Mesh
{
private:
	struct WINDFORCE {
		int		status;				//!< @brief �X�e�[�^�X
		DirectX::XMFLOAT3 force;				//!< @brief ���̓x�N�g��
	};

	struct PARTICLE {
		bool	onLockX;				//!< @brief �Œ�t���O
		bool	onLockY;				//!< @brief �Œ�t���O
		bool	onLockZ;				//!< @brief �Œ�t���O
		DirectX::XMFLOAT3	position;			//!< @brief �ʒu���W (Position)
		DirectX::XMFLOAT3	velocity;			//!< @brief ���x (Velocity)
		DirectX::XMFLOAT3	acceleration;		//!< @brief �����x (Acceleration)
		DirectX::XMFLOAT3	resultant;			//!< @brief ���� (Resultant)
		DirectX::XMFLOAT3	gravity;			//!< @brief �d�� (Gravity)
	};

	struct P_REF {
		int		horz;				//!< @brief �������C���f�b�N�X
		int		vert;				//!< @brief �c�����C���f�b�N�X
	};

	struct SPRING {
		P_REF	p1;					//!< @brief �ڑ�����Ă��闱�q�P
		P_REF	p2;					//!< @brief �ڑ�����Ă��闱�q�Q
		float	length;				//!< @brief ���R��
	};

	enum CURTAIN_STATE {
		CURTAIN_CLOSED,
		CURTAIN_CLOSING,
		CURTAIN_OPENING,
		CURTAIN_OPENED
	};

	struct CLOTH {
		int			status;														//!< @brief �X�e�[�^�X
		PARTICLE	Particle[PARTICLE_VERTICAL + 1][PARTICLE_HORIZONTAL + 1];		//!< @brief �\�����q
		SPRING		Spring[SPRING_NUMS];										//!< @brief ���q�Ԃ̃o�l
	};
	CLOTH		m_Cloth;					//!< @brief �z�I�u�W�F�N�g
	WINDFORCE	m_WindForce;				//!< @brief ���̓I�u�W�F�N�g
	bool		m_bWind = false;			//!< @brief ���̗L��/����
	bool		m_isMoving = false;			//!< @brief �J�[�e���̊J��
	CURTAIN_STATE m_curtainState = CURTAIN_CLOSED; //!< @brief �J�[�e���̏��
	bool m_isTitle;	//!< @brief �^�C�g����ʂȂ�΃{�^���ŊJ�A����ȊO�̓I�[�g�J��
	int m_dir;	//!< @brief �J�[�e���̊J������
	int m_moveCnt = 0; //!< @brief �J�[�e���ړ��p�J�E���g
public:
	CurtainComponent(C_GameObject* obj, int dir, bool isTitle = true);
	~CurtainComponent();
	void Update();
	void Draw();
	void CurtainUpdate();
	void StartOpen();
	void StartClose();
};


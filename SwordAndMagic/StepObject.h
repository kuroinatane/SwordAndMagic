/**
 * @file StepObject.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once

typedef unsigned short STEPNUM; 

 /**
  * @class C_StepObject
  * @brief �X�e�b�v���s�Ǘ��p�̃R���|�W�V������p�N���X�B�X�e�b�v�͎g�p����N���X���ŗ񋓒�`���ēK�X�L���X�g���đ��삷��B
  */
class C_StepObject {
private:
	STEPNUM m_Step; //!< @brief unsigned short�^�B�X�e�b�v�������ϐ�
	STEPNUM m_MaxStep; //!< @brief unsigned short�^�B�ő�X�e�b�v��ۑ����Ă����ϐ��B
	bool m_StepFirst; //!< @brief ���̃X�e�b�v��������s���ǂ�����ۑ����Ă����ϐ��B
public:
	C_StepObject(STEPNUM maxStep);
	~C_StepObject();
	bool CheckStepFirst();
	bool GetStepFirst();
	STEPNUM GetStep();
	void SetStep(STEPNUM step);
	void SetStepNext();
};

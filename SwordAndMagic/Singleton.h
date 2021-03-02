#pragma once
/**
 * @file Singleton.h
 * @brief �V���O���g���N���X�p�e���v���[�g�B
 *		�@Qiita�L��	https://qiita.com/narumi_/items/b205e59e7fc81695f380 �����p���Ďg�p�B
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return instance;
	}
protected:
	Singleton() {} // �O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~Singleton() {}
private:
	void operator=(const Singleton& obj) {} // ������Z�q�֎~
	Singleton(const Singleton &obj) {} // �R�s�[�R���X�g���N�^�֎~
};
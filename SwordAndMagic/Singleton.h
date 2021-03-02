#pragma once
/**
 * @file Singleton.h
 * @brief シングルトンクラス用テンプレート。
 *		　Qiita記事	https://qiita.com/narumi_/items/b205e59e7fc81695f380 を引用して使用。
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
	Singleton() {} // 外部でのインスタンス作成は禁止
	virtual ~Singleton() {}
private:
	void operator=(const Singleton& obj) {} // 代入演算子禁止
	Singleton(const Singleton &obj) {} // コピーコンストラクタ禁止
};
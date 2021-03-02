#pragma once
//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "main.h"

#include <xaudio2.h>
#include "Singleton.h"
// Windows7の場合は上記を削除して以下に置き換え.
//#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>

#define MAX_SOUND (100)
//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_BGM01,
	SOUND_BGM02,
	SOUND_BGM03,
	SOUND_SE_HIT,
	SOUND_SE_EARTH,
	SOUND_SE_SHOT,
	SOUND_SE_BUTTONMOVE,
	SOUND_MAX
} SoundName;

typedef struct
{
	SoundName	pFilename;		// ファイル名
	BYTE	nCntLoop;		// ループカウント
	IXAudio2SourceVoice* source;
	BYTE* data;
	DWORD dataSize;
} SoundData;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

class SoundManager : public Singleton<SoundManager>{
public:
	SoundManager();
	~SoundManager();
	void PlayOneShot(SoundName name);
	void PlayScheduled(SoundName name, int playTime);
	void PlayLoopSound(SoundName name);
	//void Update();
	void SetSound(SoundName name, LPCWSTR fileName);
	void StopSound(SoundName name);
	void StopSound();
	void SetVolume(float fVol);
	IXAudio2* GetAudio();
	//void Draw();
private:
	SoundData soundsData[MAX_SOUND];
	IXAudio2 *pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *pMasteringVoice = NULL;			// マスターボイス
};


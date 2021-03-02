//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : DAIKI MIURA
//
//=============================================================================
#define USE_SOUND
#include "SoundManager.h"
#if (_WIN32_WINNT >= 0x0602)
#pragma comment(lib, "xaudio2.lib")
#endif

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化
//=============================================================================
SoundManager::SoundManager() {
	HRESULT hr = S_OK;

	XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		return;
	}
	pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		if (pXAudio2)
		{
			// XAudio2オブジェクトの開放
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		return;
	}
}
SoundManager::~SoundManager() {
	for (SoundData sound : soundsData)
	{
		if (sound.source)
		{
			// 一時停止
			sound.source->Stop(0);

			// ソースボイスの破棄
			sound.source ->DestroyVoice();
			sound.source = NULL;

			// オーディオデータの開放
			free(sound.data);
			sound.data = NULL;
		}
	}

	// マスターボイスの破棄
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	if (pXAudio2)
	{
		// XAudio2オブジェクトの開放
		pXAudio2->Release();
		pXAudio2 = NULL;
	}
}

void SoundManager::SetSound(SoundName name, LPCWSTR fileName) {
	HRESULT hr = S_OK;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(hWnd, _T("サウンドデータファイルの生成に失敗！(1)"), _T("警告！"), MB_ICONWARNING);
		//return HRESULT_FROM_WIN32(GetLastError());
		return;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		//MessageBox(hWnd, _T("サウンドデータファイルの生成に失敗！(2)"), _T("警告！"), MB_ICONWARNING);
		//return HRESULT_FROM_WIN32(GetLastError());
		return;
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(1)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(2)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	if (dwFiletype != 'EVAW')
	{
		//MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(3)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("フォーマットチェックに失敗！(1)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("フォーマットチェックに失敗！(2)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &soundsData[name].dataSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("オーディオデータ読み込みに失敗！(1)"), _T("警告！"), MB_ICONWARNING);
		//return S_FALSE;
	}
	soundsData[name].data = (BYTE*)malloc(soundsData[name].dataSize);
	hr = ReadChunkData(hFile, soundsData[name].data, soundsData[name].dataSize, dwChunkPosition);
	if (FAILED(hr))
	{
		/*MessageBox(hWnd, _T("オーディオデータ読み込みに失敗！(2)"), _T("警告！"), MB_ICONWARNING);
		return S_FALSE;*/
		return;
	}

	// ソースボイスの生成
	hr = pXAudio2->CreateSourceVoice(&soundsData[name].source, &(wfx.Format));
	if (FAILED(hr))
	{
		/*MessageBox(hWnd, _T("ソースボイスの生成に失敗！"), _T("警告！"), MB_ICONWARNING);
		return S_FALSE;*/
		return;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = soundsData[name].nCntLoop;
	// オーディオバッファの登録
	soundsData[name].source->SubmitSourceBuffer(&buffer);
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{

}
void SoundManager::PlayOneShot(SoundName name) {
	#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// 状態取得
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		soundsData[name].source->Stop(0);

		// オーディオバッファの削除
		soundsData[name].source->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// 再生
	soundsData[name].source->Start(0);
#endif
}
void SoundManager::PlayLoopSound(SoundName name) {
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	// 状態取得
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		soundsData[name].source->Stop(0);

		// オーディオバッファの削除
		soundsData[name].source->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// 再生
	soundsData[name].source->Start(0);
#endif
}

void SoundManager::PlayScheduled(SoundName name, int cnt) {
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = cnt;

	// 状態取得
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		soundsData[name].source->Stop(0);

		// オーディオバッファの削除
		soundsData[name].source->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// 再生
	soundsData[name].source->Start(0);
#endif
}

//=============================================================================
// セグメント停止
//=============================================================================
void SoundManager::StopSound(SoundName name)
{
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		soundsData[name].source->Stop(0);

		// オーディオバッファの削除
		soundsData[name].source->FlushSourceBuffers();
	}
#endif
}

//=============================================================================
// セグメント停止
//=============================================================================
void SoundManager::StopSound(void)
{
#ifdef USE_SOUND
	// 一時停止
	for (SoundData sound : soundsData)
	{
		if (sound.source)
		{
			// 一時停止
			sound.source->Stop(0);
		}
	}
#endif
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

// 音量設定
void SoundManager::SetVolume(float fVol)
{
	if (pMasteringVoice) {
		pMasteringVoice->SetVolume(fVol);
	}
}

IXAudio2* SoundManager::GetAudio() {
	return pXAudio2;
}

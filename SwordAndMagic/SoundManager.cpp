//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : DAIKI MIURA
//
//=============================================================================
#define USE_SOUND
#include "SoundManager.h"
#if (_WIN32_WINNT >= 0x0602)
#pragma comment(lib, "xaudio2.lib")
#endif

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ������
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
			// XAudio2�I�u�W�F�N�g�̊J��
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
			// �ꎞ��~
			sound.source->Stop(0);

			// �\�[�X�{�C�X�̔j��
			sound.source ->DestroyVoice();
			sound.source = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(sound.data);
			sound.data = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	if (pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
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

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(hWnd, _T("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)"), _T("�x���I"), MB_ICONWARNING);
		//return HRESULT_FROM_WIN32(GetLastError());
		return;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		//MessageBox(hWnd, _T("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)"), _T("�x���I"), MB_ICONWARNING);
		//return HRESULT_FROM_WIN32(GetLastError());
		return;
	}

	// WAVE�t�@�C���̃`�F�b�N
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	if (dwFiletype != 'EVAW')
	{
		//MessageBox(hWnd, _T("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
		return;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(hFile, 'atad', &soundsData[name].dataSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		//MessageBox(hWnd, _T("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)"), _T("�x���I"), MB_ICONWARNING);
		//return S_FALSE;
	}
	soundsData[name].data = (BYTE*)malloc(soundsData[name].dataSize);
	hr = ReadChunkData(hFile, soundsData[name].data, soundsData[name].dataSize, dwChunkPosition);
	if (FAILED(hr))
	{
		/*MessageBox(hWnd, _T("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)"), _T("�x���I"), MB_ICONWARNING);
		return S_FALSE;*/
		return;
	}

	// �\�[�X�{�C�X�̐���
	hr = pXAudio2->CreateSourceVoice(&soundsData[name].source, &(wfx.Format));
	if (FAILED(hr))
	{
		/*MessageBox(hWnd, _T("�\�[�X�{�C�X�̐����Ɏ��s�I"), _T("�x���I"), MB_ICONWARNING);
		return S_FALSE;*/
		return;
	}

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = soundsData[name].nCntLoop;
	// �I�[�f�B�I�o�b�t�@�̓o�^
	soundsData[name].source->SubmitSourceBuffer(&buffer);
}


//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{

}
void SoundManager::PlayOneShot(SoundName name) {
	#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// ��Ԏ擾
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		soundsData[name].source->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundsData[name].source->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// �Đ�
	soundsData[name].source->Start(0);
#endif
}
void SoundManager::PlayLoopSound(SoundName name) {
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	// ��Ԏ擾
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		soundsData[name].source->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundsData[name].source->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// �Đ�
	soundsData[name].source->Start(0);
#endif
}

void SoundManager::PlayScheduled(SoundName name, int cnt) {
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundsData[name].dataSize;
	buffer.pAudioData = soundsData[name].data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = cnt;

	// ��Ԏ擾
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		soundsData[name].source->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundsData[name].source->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	soundsData[name].source->SubmitSourceBuffer(&buffer);

	// �Đ�
	soundsData[name].source->Start(0);
#endif
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void SoundManager::StopSound(SoundName name)
{
#ifdef USE_SOUND
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	soundsData[name].source->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		soundsData[name].source->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundsData[name].source->FlushSourceBuffers();
	}
#endif
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void SoundManager::StopSound(void)
{
#ifdef USE_SOUND
	// �ꎞ��~
	for (SoundData sound : soundsData)
	{
		if (sound.source)
		{
			// �ꎞ��~
			sound.source->Stop(0);
		}
	}
#endif
}

//=============================================================================
// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

// ���ʐݒ�
void SoundManager::SetVolume(float fVol)
{
	if (pMasteringVoice) {
		pMasteringVoice->SetVolume(fVol);
	}
}

IXAudio2* SoundManager::GetAudio() {
	return pXAudio2;
}

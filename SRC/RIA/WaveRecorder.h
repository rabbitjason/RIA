#pragma once

//WAVE文件格式，不包括附加块
#include <PshPack1.h>
typedef struct tagWAVEHEADER
{
    CHAR    RIFF[4];
    DWORD   dwFileLen;
    CHAR    WAVE[4];
    CHAR    Format[4];
    DWORD   dwFormatLen;
    WORD    wFormat;
    WORD    wChannel;
    DWORD   dwSamplesPerSec;
    DWORD   dwAvgBytesPerSec;
    WORD    wBlockAlign;
    WORD    wBitsPerSample;
    CHAR    DATA[4];
    DWORD   dwDataLen;
public:
    tagWAVEHEADER() : dwFileLen(0), dwFormatLen(16), wFormat(WAVE_FORMAT_PCM), dwDataLen(0)
    {
        memcpy(RIFF, "RIFF", sizeof(RIFF));
        memcpy(WAVE, "WAVE", sizeof(WAVE));
        memcpy(Format, "fmt ", sizeof(Format));
        memcpy(DATA, "data", sizeof(DATA));
    }
}WAVEHEADER;
#include <PopPack.h>

//about 1 sec
#define WAVE_BLOCK_SCALE 1.0
typedef vector<LPWAVEHDR> WAVE_HDR_ARRAY;

class CMMNotificationClient;

class CWaveRecorder
{
public:
    CWaveRecorder(void);
    ~CWaveRecorder(void);
    DWORD Open(HWND hWnd = NULL);
    void Close();
    DWORD StartRecord(LPCTSTR lpstrWaveFile);
    DWORD StopRecord();
    //返回0~100之间的数字
    UINT  GetWaveInVolume();
    //0~100
    void SetWaveInVolume(UINT nVolume);
    //DWORD GetWaveTimeLen();
    LRESULT OnDevChanged(WPARAM wParam, LPARAM lParam);
    LRESULT OnLineChange(WPARAM wParam, LPARAM lParam);
    LRESULT OnControlChange(WPARAM wParam, LPARAM lParam);
    LRESULT OnStateChange(DWORD dwNewState);
protected:
    static void CALLBACK mywaveInProc(
        HWAVEIN hwi,       
        UINT uMsg,         
        DWORD_PTR dwInstance,  
        DWORD_PTR dwParam1,    
        DWORD_PTR dwParam2     
        );
    static unsigned WINAPI thread_address(void* pParam);
    //wParam: hInputDev , or null, lParam:lpwvhdr, or null
    void WIM_MSG_Handler(UINT wimMsg, WPARAM wParam, LPARAM lParam);
    MMRESULT GetVolumeControl(MIXERCONTROL* pVolCtrl);

    friend class CMMNotificationClient;
    HRESULT  RegisterDeviceEndPoint();
    HRESULT  UnRegisterDeviceEndPoint();
    DWORD _OpenWaveIn();
    void  _CloseWaveIn();
private:
    WAVEHEADER  m_WaveFileHdr;
    CString     m_strWaveFile;
    CFile       m_WaveFile;
    HWAVEIN m_hWaveIn;
    HMIXER  m_hMixer;
    WAVEFORMATEX m_waveform;
    DWORD m_dwWaveBlockSize;
    DWORD m_dwWaveHdrNum;
    WAVE_HDR_ARRAY m_WaveOutHdrArray;
    DWORD   m_dwRecordSize;
    //解决waveInUnprepareHeader可能的死锁
    LONG m_lReset;
    LPBYTE   m_lpDatBuffer;
    DWORD    m_dwBufferSize;
    HANDLE   m_hThread;
    unsigned m_dwThreadId;

    //Vista or later
    WCHAR    m_wstrDeviceId[1024*2];
    CComPtr<IMMDeviceEnumerator> m_cpMMDevEnum;
    CMMNotificationClient*  m_pMMClient;
    HWND     m_hMsgWnd;
};

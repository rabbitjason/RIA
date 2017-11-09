#ifndef __MCI_WAVE_H__
#define __MCI_WAVE_H__

class CWave
{
    //Construction
public:
    CWave();
    virtual ~CWave();

    //Operations
public:
    DWORD OpenDevice();
    DWORD CloseDevice();
    DWORD Play(HWND hWnd,LPCTSTR pFileName, DWORD dwFlag=MCI_NOTIFY);
    DWORD PlayWait(HWND hWnd,LPCTSTR pFileName)
    {
        DuiLib::CWaitCursor cwait;
        return Play(hWnd, pFileName, MCI_WAIT);
    }
    DWORD Stop();

    //Implementation
protected:
    void DisplayErrorMsg(DWORD dwError);

    //Members
protected:
    MCIDEVICEID m_nDeviceID;
    MCIDEVICEID m_nElementID;
};

#endif
#include "stdafx.h"
#include "CMciWave.h"

//Cwave类的实现代码，Cwave.cpp
CWave::CWave()
{
    m_nDeviceID=0;
    m_nElementID=0;
}

CWave::~CWave()
{
    if(m_nElementID)
        Stop();
    if(m_nDeviceID)
        CloseDevice();
}

DWORD CWave::OpenDevice()
{
    DWORD dwResult=0;

    if (!m_nDeviceID)
    {
        MCI_OPEN_PARMS mciOpenParms = {0};

        mciOpenParms.lpstrDeviceType=(LPCTSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;

        //open the wave device
        dwResult = mciSendCommand(NULL,
            MCI_OPEN,
            MCI_OPEN_TYPE|MCI_OPEN_TYPE_ID|MCI_WAIT,
            (DWORD_PTR)(LPVOID)&mciOpenParms);

        //save device identifier,will use with other MCI commands
        m_nDeviceID = mciOpenParms.wDeviceID;

        //display error message if failed 
        if(dwResult)
        {
            TRACE(_T("MCI OpenDevice failed\n"));
            DisplayErrorMsg(dwResult);
        }
    }
    //return result of MCI operation
    return dwResult;
}

DWORD CWave::CloseDevice()
{
    DWORD dwResult=0;

    //close if currently open
    if(m_nDeviceID)
    {
        //close the MCI device
        dwResult=mciSendCommand(m_nDeviceID,MCI_CLOSE,NULL,NULL);

        //display error message if failed
        if(dwResult)
        {
            TRACE(_T("MCI closedevice failed\n"));
            DisplayErrorMsg(dwResult);
        }
        //set identifier to close state
        else 
            m_nDeviceID=0;
    }

    //return result of MCI operation
    return dwResult;
}

DWORD CWave::Play(HWND hWnd,LPCTSTR pFileName, DWORD dwFlag/*=MCI_NOTIFY*/)
{
    MCI_OPEN_PARMS mciOpenParms;
    //initialize structure
    memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS));

    //set the WAV file name to be played
    mciOpenParms.lpstrElementName=pFileName;
    //mciOpenParms.lpstrDeviceType=(LPCTSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;

    //first open the device
    DWORD dwResult=mciSendCommand(m_nDeviceID,MCI_OPEN,
        MCI_OPEN_ELEMENT,(DWORD_PTR)(LPVOID)&mciOpenParms);

    //display error message if failed
    if(dwResult)
    {
        TRACE(_T("MCI play, MCI_OPEN_ELEMENT failed\n"));
        DisplayErrorMsg(dwResult);
    }
    //if successful,instruct the device to play the WAV file
    else
    {
        //save element identifier
        m_nElementID=mciOpenParms.wDeviceID;

        MCI_PLAY_PARMS mciPlayParms;

        //set the window that will receive notification message
        if (MCI_NOTIFY == dwFlag)
        {
            mciPlayParms.dwCallback=(DWORD_PTR)hWnd;
        }

        //instruct device to play file
        dwResult=mciSendCommand(m_nElementID,MCI_PLAY,
            dwFlag,(DWORD_PTR)(LPVOID)&mciPlayParms);

        //display error and close element if failed
        if(dwResult)
        {
            TRACE(_T("MCI play, MCI_NOTIFY failed\n"));
            DisplayErrorMsg(dwResult);
            Stop();
        }
        else if (MCI_WAIT == dwFlag)
        {
            Stop();
        }
    }

    //return result of MCI operation
    return dwResult;
}

DWORD CWave::Stop()
{
    DWORD dwResult=0;

    //close if element is currently open

    if(m_nElementID)
    {
        MCI_GENERIC_PARMS gp = {0};
        dwResult=mciSendCommand(m_nElementID,MCI_STOP,NULL,(DWORD_PTR)&gp);
        if(dwResult)
        {
            TRACE(_T("MCI stop failed:%d\n"), dwResult);
            DisplayErrorMsg(dwResult);
        }

        dwResult=mciSendCommand(m_nElementID,MCI_CLOSE,NULL,NULL);

        //display error message if failed
        if(dwResult)
        {
            TRACE(_T("MCI close failed:%d\n"), dwResult);
            DisplayErrorMsg(dwResult);
        }
        //set identifier to closed state
        else
            m_nElementID=0;
    }
    return dwResult;
}

void CWave::DisplayErrorMsg(DWORD dwError)
{
    //check if there was an error
    if(dwError)
    {
        //character string that contains error message
        TCHAR szErrorMsg[MAXERRORLENGTH];

        //retrieve string associated error message
        if(!mciGetErrorString(dwError,szErrorMsg,sizeof(szErrorMsg)))
        {
            StringCchCopy(szErrorMsg, _countof(szErrorMsg), _T("Unknown Error"));
        }
        //display error string in message box
        //AfxMessageBox(szErrorMsg);
        TRACE(_T("%s\n"), szErrorMsg);
    }
}

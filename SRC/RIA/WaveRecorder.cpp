#include "StdAfx.h"
#include "WaveRecorder.h"

//需要新的SDK或DDK，在mmddk.h中定义
#define DRV_QUERYDEVNODE             (DRV_RESERVED + 2)
#define DRV_QUERYMAPPABLE            (DRV_RESERVED + 5)
#define DRV_QUERYMODULE              (DRV_RESERVED + 9)
#define DRV_PNPINSTALL               (DRV_RESERVED + 11)
#define DRV_QUERYDEVICEINTERFACE     (DRV_RESERVED + 12)
#define DRV_QUERYDEVICEINTERFACESIZE (DRV_RESERVED + 13)
#define DRV_QUERYSTRINGID            (DRV_RESERVED + 14)
#define DRV_QUERYSTRINGIDSIZE        (DRV_RESERVED + 15)
#define DRV_QUERYIDFROMSTRINGID      (DRV_RESERVED + 16)
#define DRV_QUERYFUNCTIONINSTANCEID  (DRV_RESERVED + 17)
#define DRV_QUERYFUNCTIONINSTANCEIDSIZE (DRV_RESERVED + 18)


//#ifndef DRV_QUERYFUNCTIONINSTANCEIDSIZE
//#define DRV_QUERYFUNCTIONINSTANCEIDSIZE (DRV_RESERVED + 18)
//#endif
//#ifndef DRV_QUERYFUNCTIONINSTANCEID
//#define DRV_QUERYFUNCTIONINSTANCEID     (DRV_RESERVED + 17)
//#endif



//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------
// Example implementation of IMMNotificationClient interface.
// When the status of audio endpoint devices change, the
// MMDevice module calls these methods to notify the client.
//-----------------------------------------------------------

#define SAFE_RELEASE(punk)  \
    if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

class CMMNotificationClient : public IMMNotificationClient
{
private:
    LONG _cRef;
    CWaveRecorder*  m_pWaveRecorder;

    // Private function to print device-friendly name
    HRESULT _PrintDeviceName(LPCWSTR  pwstrId);

public:
    CMMNotificationClient(CWaveRecorder* pRecorder = NULL) :
      _cRef(1),
      m_pWaveRecorder(pRecorder)
      {
          ASSERT(pRecorder);
      }

      ~CMMNotificationClient()
      {
      }

      // IUnknown methods -- AddRef, Release, and QueryInterface

      ULONG STDMETHODCALLTYPE AddRef()
      {
          return InterlockedIncrement(&_cRef);
      }

      ULONG STDMETHODCALLTYPE Release()
      {
          ULONG ulRef = InterlockedDecrement(&_cRef);
          if (0 == ulRef)
          {
              delete this;
          }
          return ulRef;
      }

      HRESULT STDMETHODCALLTYPE QueryInterface(
          REFIID riid, VOID **ppvInterface)
      {
          if (IID_IUnknown == riid)
          {
              AddRef();
              *ppvInterface = (IUnknown*)this;
          }
          else if (__uuidof(IMMNotificationClient) == riid)
          {
              AddRef();
              *ppvInterface = (IMMNotificationClient*)this;
          }
          else
          {
              *ppvInterface = NULL;
              return E_NOINTERFACE;
          }
          return S_OK;
      }

      // Callback methods for device-event notifications.

      HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
          EDataFlow flow, ERole role,
          LPCWSTR pwstrDeviceId)
      {
#ifdef _DEBUG
          TCHAR  *pszFlow = _T("?????");
          TCHAR  *pszRole = _T("?????");

          _PrintDeviceName(pwstrDeviceId);

          switch (flow)
          {
          case eRender:
              pszFlow = _T("eRender");
              break;
          case eCapture:
              pszFlow = _T("eCapture");
              break;
          }

          switch (role)
          {
          case eConsole:
              pszRole = _T("eConsole");
              break;
          case eMultimedia:
              pszRole = _T("eMultimedia");
              break;
          case eCommunications:
              pszRole = _T("eCommunications");
              break;
          }

          TRACE(_T("  -->New default device: flow = %s, role = %s\n"),
              pszFlow, pszRole);
#endif
          return S_OK;
      }

      HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
      {
#ifdef _DEBUG
          _PrintDeviceName(pwstrDeviceId);

          TRACE(_T("  -->Added device\n"));
#endif
          return S_OK;
      };

      HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
      {
#ifdef _DEBUG
          _PrintDeviceName(pwstrDeviceId);

          TRACE(_T("  -->Removed device\n"));
#endif
          return S_OK;
      }

      HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
          LPCWSTR pwstrDeviceId,
          DWORD dwNewState)
      {
#ifdef _DEBUG
          TCHAR  *pszState = _T("?????");

          _PrintDeviceName(pwstrDeviceId);

          switch (dwNewState)
          {
          case DEVICE_STATE_ACTIVE:
              pszState = _T("ACTIVE");
              break;
          case DEVICE_STATE_DISABLED:
              pszState = _T("DISABLED");
              break;
          case DEVICE_STATE_NOTPRESENT:
              pszState = _T("NOTPRESENT");
              break;
          case DEVICE_STATE_UNPLUGGED:
              pszState = _T("UNPLUGGED");
              break;
          }

          TRACE(_T("  -->New device state is DEVICE_STATE_%s (0x%8.8x)\n"),
              pszState, dwNewState);
#endif

          if (lstrcmpW(m_pWaveRecorder->m_wstrDeviceId, pwstrDeviceId) == 0)
          {
              //it's our device
              if (m_pWaveRecorder->m_hMsgWnd)
              {
                  ::PostMessage(m_pWaveRecorder->m_hMsgWnd, WM_WAVEINSTATE, (WPARAM)dwNewState, 0);
              }
          }
          return S_OK;
      }

      HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
          LPCWSTR pwstrDeviceId,
          const PROPERTYKEY key)
      {
#ifdef _DEBUG
          return S_OK;
          _PrintDeviceName(pwstrDeviceId);

          TRACE(_T("  -->Changed device property ")
              _T("{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x}#%d\n"),
              key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3,
              key.fmtid.Data4[0], key.fmtid.Data4[1],
              key.fmtid.Data4[2], key.fmtid.Data4[3],
              key.fmtid.Data4[4], key.fmtid.Data4[5],
              key.fmtid.Data4[6], key.fmtid.Data4[7],
              key.pid);
#endif
          return S_OK;
      }
};

// Given an endpoint ID string, print the friendly device name.
HRESULT CMMNotificationClient::_PrintDeviceName(LPCWSTR pwstrId)
{
    HRESULT hr = S_OK;
    IMMDevice *pDevice = NULL;
    IPropertyStore *pProps = NULL;
    PROPVARIANT varString;

    PropVariantInit(&varString);

    if (hr == S_OK)
    {
        hr = m_pWaveRecorder->m_cpMMDevEnum->GetDevice(pwstrId, &pDevice);
    }
    if (hr == S_OK)
    {
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
    }
    if (hr == S_OK)
    {
        // Get the endpoint device's friendly-name property.
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varString);
    }
    TRACE("----------------------\nDevice name: \"%S\"\n"
        "  Endpoint ID string: \"%S\"\n",
        (hr == S_OK) ? varString.pwszVal : L"null device",
        (pwstrId != NULL) ? pwstrId : L"null ID");

    PropVariantClear(&varString);

    SAFE_RELEASE(pProps)
    SAFE_RELEASE(pDevice)
    return hr;
}



CWaveRecorder::CWaveRecorder(void) : m_hWaveIn(NULL), m_lReset(0), m_lpDatBuffer(NULL)
, m_hMixer(NULL)
, m_hThread(NULL)
, m_dwThreadId(0)
, m_dwRecordSize(0)
{
    m_pMMClient = new CMMNotificationClient(this);
}

CWaveRecorder::~CWaveRecorder(void)
{
    if (m_pMMClient)
    {
        m_pMMClient->Release();
    }
    Close();
}

void CWaveRecorder::WIM_MSG_Handler(UINT wimMsg, WPARAM wParam, LPARAM lParam)
{
    switch(wimMsg)
    {
    case WIM_OPEN:      //MM_WIM_OPEN
        {
            TRACE(_T("wave device opened\n"));
        }
        break;
    case WIM_DATA:
        {
            PWAVEHDR pHdr = (PWAVEHDR)lParam;
            if ((HWAVEIN)wParam != m_hWaveIn)
            {
                TRACE(_T("not our device:%p\n"), wParam);
                break;
            }
            //TRACE(_T("2:m_lReset:%d\n"), m_lReset);
            if (InterlockedCompareExchange(&m_lReset, 1, 1) > 0)
            {
                TRACE(_T("reset wave device...\n"));
                break;
            }
            //TRACE(_T("wave done:%d\n"), pHdr->dwFlags & WHDR_DONE ? 1 : 0);

            if (0 == pHdr->dwBytesRecorded)
            {
                TRACE(_T("zero audio data\n"));
                //pHdr->dwFlags = 0;
                //waveInPrepareHeader(pView->m_hWaveIn, pHdr, sizeof(WAVEHDR));
                //waveInAddBuffer(pView->m_hWaveIn, (PWAVEHDR)dwParam1, sizeof (WAVEHDR)) ;

                //Vista下，拔出麦，会收到WIM_DATA，dwBytesRecorded为0，和reset一样处理
                break;
            }

            waveInUnprepareHeader(m_hWaveIn, pHdr, sizeof(WAVEHDR));

            //最好不要在回调函数写文件...
            //pView->m_WaveFile.Write(pHdr->lpData, pHdr->dwBytesRecorded);
            while(pHdr->dwBytesRecorded + m_dwRecordSize > m_dwBufferSize)
            {
                TRACE(_T("buffer too small:%d, need:%d\n"), m_dwBufferSize, pHdr->dwBytesRecorded + m_dwRecordSize);
                //it seems that call HeapReAlloc is ok
                LPBYTE pNew = (LPBYTE)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                    m_lpDatBuffer, m_dwBufferSize * 2);
                if (NULL == pNew)
                {
                    TRACE(_T("Out of memory!\n"));
                    break;
                }
                m_lpDatBuffer = pNew;
                m_dwBufferSize = HeapSize(GetProcessHeap(), 0, m_lpDatBuffer);
            }
            if (m_dwBufferSize > (pHdr->dwBytesRecorded + m_dwRecordSize))
            {
                memcpy(&m_lpDatBuffer[m_dwRecordSize], pHdr->lpData, pHdr->dwBytesRecorded);
                m_dwRecordSize += pHdr->dwBytesRecorded;
            }

            pHdr->dwFlags = 0;
            waveInPrepareHeader(m_hWaveIn, pHdr, sizeof(WAVEHDR));
            waveInAddBuffer(m_hWaveIn, (PWAVEHDR)pHdr, sizeof (WAVEHDR)) ;
            TRACE("done input data\n");
        }
        break;
    case WIM_CLOSE:
        {
            TRACE(_T("wave device closed\n"));
            //WAVE_HDR_ARRAY::size_type i, num = pView->m_WaveOutHdrArray.size();
            //for (i = 0; i < num; i++)
            //{
            //    waveInUnprepareHeader(pView->m_hWaveIn, pView->m_WaveOutHdrArray[i], sizeof(WAVEHDR));
            //    delete[] pView->m_WaveOutHdrArray[i]->lpData;
            //    delete[] pView->m_WaveOutHdrArray[i];
            //}
            //pView->m_WaveOutHdrArray.clear();
        }
        break;
    default:
        TRACE(_T("other WIM_msg:%d\n"), wimMsg);
        break;
    }
}

void CALLBACK CWaveRecorder::mywaveInProc(
    HWAVEIN hwi,       
    UINT uMsg,         
    DWORD_PTR dwInstance,  
    DWORD_PTR dwParam1,    
    DWORD_PTR dwParam2     
    )
{
    CWaveRecorder *pView = (CWaveRecorder*)dwInstance;
    pView->WIM_MSG_Handler(uMsg, (WPARAM)pView->m_hWaveIn, dwParam1);
}

unsigned WINAPI CWaveRecorder::thread_address(void* pParam)
{
    BOOL bRet;
    MSG msg;
    TRACE(_T("recorder thread start\n"));
    while ( (bRet = ::GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (-1 == bRet)
        {
            TRACE(_T("call GetMessage failed:%s\n"), MyFormatMessage(GetLastError()));
        }
        else
        {
            //TRACE(_T("recv msg:%s\n"), DUI__TraceMsg(msg.message));
            CWaveRecorder* pView = (CWaveRecorder*)pParam;
            if (msg.message <= MM_WIM_DATA &&
                msg.message >= MM_WIM_OPEN)
            {
                pView->WIM_MSG_Handler(msg.message, msg.wParam, msg.lParam);
            }
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
    TRACE(_T("recorder thread quit\n"));
    return 0;
}

DWORD CWaveRecorder::_OpenWaveIn()
{
    DWORD dwRet = ERROR_SUCCESS;
    MMRESULT e;
    TCHAR err[MAXERRORLENGTH];

    do 
    {
        WAVEFORMATEX *pwf = (WAVEFORMATEX*)&m_waveform;
        
        InterlockedExchange(&m_lReset, 0);

        e = waveInOpen(&m_hWaveIn, 0, pwf, (DWORD_PTR)mywaveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
        //e = waveInOpen(&m_hWaveIn,WAVE_MAPPER, pwf, (DWORD_PTR)m_dwThreadId, (DWORD_PTR)this, CALLBACK_THREAD);
        if (MMSYSERR_NOERROR != e) {
            waveInGetErrorText(e, err, _countof(err));
            TRACE(_T("wave in open fail <%d:%s>.\n"), e, err);
            MessageBeep(MB_ICONEXCLAMATION);
            dwRet = ERROR_BAD_DEVICE;
            break;
        }

        UINT nMixId;
        ::mixerGetID((HMIXEROBJ)m_hWaveIn, &nMixId, MIXER_OBJECTF_HWAVEIN);

#ifdef _DEBUG
        MIXERCAPS mcap = {0};
        e = ::mixerGetDevCaps((UINT_PTR)nMixId, &mcap, sizeof(mcap));
        if (e == MMSYSERR_NOERROR)
        {
            TRACE(_T("mixerDev name:%s\n"), mcap.szPname);
        }
#endif

        TRACE(_T("mixerOpen, hWnd:%p\n"), m_hMsgWnd);
        e = ::mixerOpen(&m_hMixer, nMixId, (DWORD_PTR)m_hMsgWnd, 0, m_hMsgWnd ? CALLBACK_WINDOW : 0);

        WAVE_HDR_ARRAY::const_iterator p;
        for (p = m_WaveOutHdrArray.begin(); p != m_WaveOutHdrArray.end(); ++p)
        {
            e = waveInPrepareHeader(m_hWaveIn, *p, sizeof(WAVEHDR));
            if (MMSYSERR_NOERROR != e)
            {
                waveInGetErrorText(e, err, _countof(err));
                TRACE(_T("waveInPrepareHeader fail <%d:%s>.\n"), e, err);
            }
            e = waveInAddBuffer(m_hWaveIn, *p, sizeof(WAVEHDR));
            if (MMSYSERR_NOERROR != e)
            {
                waveInGetErrorText(e, err, _countof(err));
                TRACE(_T("waveInAddBuffer fail <%d:%s>.\n"), e, err);
            }
        }

        if (IsVistaOrLater())
        {
            UINT devid;
            size_t cbEndpointId = 0;
            e = ::waveInGetID(m_hWaveIn, &devid);
            if (MMSYSERR_NOERROR != e)
            {
                waveInGetErrorText(e, err, _countof(err));
                TRACE(_T("waveInGetID fail <%d:%s>.\n"), e, err);
            }
            TRACE(_T("devid:%d\n"), devid);
            if (devid == WAVE_MAPPER)
            {
                devid = 0;
            }
            ZeroMemory(m_wstrDeviceId, sizeof(m_wstrDeviceId));
            e = ::waveInMessage((HWAVEIN)IntToPtr(devid), DRV_QUERYFUNCTIONINSTANCEIDSIZE, (DWORD_PTR)&cbEndpointId, NULL);
            if (MMSYSERR_NOERROR != e)
            {
                waveInGetErrorText(e, err, _countof(err));
                TRACE(_T("waveInMessage DRV_QUERYFUNCTIONINSTANCEIDSIZE fail <%d:%s>.\n"), e, err);
            }
            else
            {
                TRACE(_T("DRV_QUERYFUNCTIONINSTANCEIDSIZE:%d\n"), cbEndpointId);
                ASSERT(cbEndpointId < sizeof(m_wstrDeviceId));
                e = ::waveInMessage((HWAVEIN)IntToPtr(devid), DRV_QUERYFUNCTIONINSTANCEID, (DWORD_PTR)&m_wstrDeviceId, cbEndpointId);
                if (MMSYSERR_NOERROR != e)
                {
                    waveInGetErrorText(e, err, _countof(err));
                    TRACE(_T("waveInMessage DRV_QUERYFUNCTIONINSTANCEID fail <%d:%s>.\n"), e, err);
                }
                TRACE("wave in id:%S\n", m_wstrDeviceId);
            }
        }

    } while (false);

    return dwRet;
}

DWORD CWaveRecorder::Open(HWND hWnd/* = NULL*/)
{
    DWORD dwRet = ERROR_SUCCESS;

    do 
    {
        //open waveform audio for input
        if (m_hWaveIn != NULL)
        {
            TRACE(_T("already opened\n"));
            break;
        }

        m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CWaveRecorder::thread_address, this, 0, &m_dwThreadId);
        if (NULL == m_hThread)
        {
            dwRet = errno;
            AfxMessageBox(_T("系统资源不足，无法创建线程！"));
            break;
        }

        m_hMsgWnd = hWnd;

        int rt = IDCANCEL;
        TCHAR err[MAXERRORLENGTH];
        memset(err, 0, sizeof(err));
        UINT nNum;
        do 
        {
            nNum = waveInGetNumDevs();
            TRACE(_T("wave in device num:%d\n"), nNum);

            if (0 == nNum)
            {
                //if (IsVistaOrLater())
                {
                    rt = AfxMessageBox(_T("打开录音设备失败，请确认麦克风是否插入。"),
                        MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_DEFBUTTON2);
                }
                //else
                //{
                //    AfxMessageBox(_T("系统无录音设备。"), MB_OK | MB_ICONINFORMATION);
                //    rt = IDCANCEL;
                //}
            }
            else
            {
                break;
            }
        } while (rt == IDRETRY);
        if (IDABORT == rt)
        {
            dwRet = ERROR_NO_MORE_DEVICES;
            break;
        }
#ifdef _DEBUG
        MMRESULT e;
        UINT i;
        WAVEINCAPS wc;
        // !!!how about more than one wave in device?
        for (i = 0; i < nNum; i++)
        {
            memset(&wc, 0, sizeof(wc));
            e = waveInGetDevCaps(i, &wc, sizeof(wc));
            TRACE(_T("Product name:%s, MID:%d, PID:%d, format:0x%x, channel:%d\n"),
                wc.szPname, wc.wMid, wc.wPid, wc.dwFormats, wc.wChannels);
            if (MMSYSERR_NOERROR != e) {
                waveInGetErrorText(e, err, _countof(err));
                TRACE(_T("waveInGetDevCaps fail <%d:%s>.\n"), e, err);
            }
        }
#endif

        //prepare

        memset(&m_waveform, 0, sizeof(m_waveform));
        m_waveform.wFormatTag=WAVE_FORMAT_PCM;
        m_waveform.nChannels=g_wChannel;
        m_waveform.wBitsPerSample=g_wBitsPerSample;
        m_waveform.nBlockAlign=m_waveform.wBitsPerSample * m_waveform.nChannels / 8;
        m_waveform.nSamplesPerSec=g_dwSamplesPerSec;//22050;WAVE_FORMAT_2M16
        m_waveform.nAvgBytesPerSec=m_waveform.nSamplesPerSec * m_waveform.nBlockAlign;
        m_waveform.cbSize=0;

        TRACE(_T("input nAvgBytesPerSec:%lu\n"), m_waveform.nAvgBytesPerSec);

        WAVEFORMATEX *pwf = (WAVEFORMATEX*)&m_waveform;

        //1秒的数据量/通知块的数量，得到的每个通知的数据量小于0.2秒的数据量
        //语音延迟200ms为优质
        m_dwWaveBlockSize = DWORD(WAVE_BLOCK_SCALE * pwf->nAvgBytesPerSec);
        m_dwWaveHdrNum = 4;//pwf->nAvgBytesPerSec / m_dwWaveBlockSize;
        DWORD inum;
        if (pwf->nAvgBytesPerSec % m_dwWaveBlockSize)
        {
            m_dwWaveHdrNum++;
        }
        TRACE(_T("m_dwWaveBlockSize:%d, m_dwWaveHdrNum:%d\n"), m_dwWaveBlockSize, m_dwWaveHdrNum);
        for (inum = 0; inum < m_dwWaveHdrNum; inum++)
        {
            LPWAVEHDR p = new WAVEHDR;
            memset(p, 0, sizeof(WAVEHDR));
            p->dwBufferLength = m_dwWaveBlockSize;
            p->lpData = new CHAR[m_dwWaveBlockSize];
            m_WaveOutHdrArray.push_back(p);
        }

        //缓存大概5分钟的语音
        m_dwBufferSize = m_dwWaveBlockSize * 5 * 60;
        m_lpDatBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_dwBufferSize);
        if (NULL == m_lpDatBuffer)
        {
            dwRet = ERROR_OUTOFMEMORY;
            AfxMessageBox(_T("录音设备打开失败：内存不足！"));
            break;
        }
        m_dwBufferSize = HeapSize(GetProcessHeap(), 0, m_lpDatBuffer);
        TRACE(_T("m_dwBufferSize:%d\n"), m_dwBufferSize);

        m_WaveFileHdr.dwAvgBytesPerSec = m_waveform.nAvgBytesPerSec;
        m_WaveFileHdr.dwSamplesPerSec = m_waveform.nSamplesPerSec;
        m_WaveFileHdr.wBitsPerSample = m_waveform.wBitsPerSample;
        m_WaveFileHdr.wChannel = m_waveform.nChannels;
        m_WaveFileHdr.wBlockAlign = m_waveform.nBlockAlign;

        dwRet = _OpenWaveIn();
        if (ERROR_SUCCESS != dwRet)
        {
            //忽略错误
            //break;
        }

        RegisterDeviceEndPoint();
    } while (FALSE);

    if (ERROR_SUCCESS != dwRet)
    {
        Close();
    }
    return dwRet;
}

void CWaveRecorder::_CloseWaveIn()
{
    WAVE_HDR_ARRAY::size_type i, num;

    StopRecord();

    if (m_hMixer)
    {
        ::mixerClose(m_hMixer);
        m_hMixer = NULL;
    }
    if (m_hWaveIn)
    {
        //m_lReset = true;
        InterlockedExchange(&m_lReset, 1);
        Sleep(0);
        //Vista:如果麦拔出，此时系统已经可能waveInReset、WaveInClose了
        //TRACE(_T("1:m_lReset:%d\n"), m_lReset);
        waveInReset(m_hWaveIn);
        Sleep(0);
        num = m_WaveOutHdrArray.size();
        for (i = 0; i < num; i++)
        {
            waveInUnprepareHeader(m_hWaveIn, m_WaveOutHdrArray[i], sizeof(WAVEHDR));
        }
        waveInClose(m_hWaveIn);
        m_hWaveIn = NULL;
    }
}

void CWaveRecorder::Close()
{
    WAVE_HDR_ARRAY::size_type i, num;

    _CloseWaveIn();

    if (m_hThread)
    {
        ::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
        ::WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        m_hThread = NULL;
        m_dwThreadId = 0;
    }

    num = m_WaveOutHdrArray.size();
    for (i = 0; i < num; i++)
    {
        delete[] m_WaveOutHdrArray[i]->lpData;
        delete[] m_WaveOutHdrArray[i];
    }
    m_WaveOutHdrArray.clear();

    if (m_lpDatBuffer)
    {
        HeapFree(GetProcessHeap(), 0, m_lpDatBuffer);
        m_lpDatBuffer = NULL;
        m_dwBufferSize = 0;
    }

    UnRegisterDeviceEndPoint();
}

DWORD CWaveRecorder::StartRecord(LPCTSTR lpstrWaveFile)
{
    DWORD dwRet = ERROR_SUCCESS;

    do 
    {
        if (NULL == m_hWaveIn)
        {
            dwRet = ERROR_BAD_DEVICE;
            break;
        }
        
        CFileException fe;
        if (!m_WaveFile.Open(lpstrWaveFile, CFile::modeCreate | CFile::shareDenyWrite | CFile::modeWrite, &fe))
        {
            TCHAR errmsg[1024];
            fe.GetErrorMessage(errmsg, _countof(errmsg), NULL);
            dwRet = fe.m_lOsError;
            MyAfxMessageBox(_T("Wave文件打开失败:%s 错误码:%d\n"),
                errmsg, dwRet);
            break;
        }
        //m_WaveFile.Write(&m_WaveFileHdr, sizeof(m_WaveFileHdr));
        m_strWaveFile = lpstrWaveFile;
        m_dwRecordSize = 0;

        // Begin sampling
        MMRESULT mr = waveInStart(m_hWaveIn);
//#ifdef _DEBUG
        if (MMSYSERR_NOERROR != mr)
        {
            TCHAR err[MAXERRORLENGTH];
            memset(err, 0, sizeof(err));
            waveInGetErrorText(mr, err, _countof(err));
            MyAfxMessageBox(_T("开始录音失败：%s\n"), err);
            dwRet = ERROR_BAD_DEVICE;
            break;
        }
//#endif
    } while (FALSE);

    return dwRet;
}

DWORD CWaveRecorder::StopRecord()
{
    DWORD dwRet = ERROR_SUCCESS;

    do 
    {
        if (m_hWaveIn)
        {
            //waveInReset(m_hWaveIn);
            waveInStop(m_hWaveIn);
            Sleep(200);
        }
        if (m_WaveFile.m_hFile != CFile::hFileNull)
        {
            m_WaveFileHdr.dwFileLen = m_dwRecordSize + sizeof(WAVEHEADER) - FIELD_OFFSET(WAVEHEADER, WAVE);
            m_WaveFileHdr.dwDataLen = m_dwRecordSize;
            m_WaveFile.SeekToBegin();
            m_WaveFile.Write(&m_WaveFileHdr, sizeof(m_WaveFileHdr));
            m_WaveFile.Write(m_lpDatBuffer, m_dwRecordSize);
            m_WaveFile.Close();
            TRACE("close wave file, size:%d\n", m_dwRecordSize);
        }
    } while (FALSE);

    return dwRet;
}

MMRESULT CWaveRecorder::GetVolumeControl(MIXERCONTROL* pVolCtrl)
{
    MMRESULT mmr;

    ASSERT(pVolCtrl);
    if (m_hMixer == NULL)
    {
        return 0;
    }

    MIXERLINE mxl = {0};

    do 
    {
        mxl.cbStruct = sizeof(mxl);
        mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
        mmr = ::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);
        TRACE(_T("mixerGetLineInfo return:%d, cConnections:%d\n"), mmr, mxl.cConnections);
        if (MMSYSERR_NOERROR != mmr)
        {
            break;
        }
        DWORD   dwConnections   =   mxl.cConnections;   
        //   准备获取麦克风设备的ID   
        //DWORD   dwLineID   =   0;   
        bool exist = false;
        for(DWORD i = 0; i < dwConnections; i++)   
        {   
            //   枚举每一个设备，当Source的ID等于当前的迭代记数   
            mxl.cbStruct = sizeof(mxl);
            mxl.dwSource   =   i;   
            //   根据SourceID获得连接的信息   
            mmr   =   ::mixerGetLineInfo(   (HMIXEROBJ)m_hMixer,   &mxl, MIXER_GETLINEINFOF_SOURCE);
            //TRACE(_T("%d, mixerGetLineInfo return:%d\n"), i+1, mmr);
            //   判断函数执行错误   
            if(mmr != MMSYSERR_NOERROR)
            {   
                break;   
            }   
            //TRACE(_T("dwComponentType:0x%x\n"), mxl.dwComponentType);
            //   如果当前设备类型是麦克风，则跳出循环。   
            if(mxl.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)
            {   
                exist = true;
                break;   
            }   
        }
        if (!exist)
        {
            break;
        }
        MIXERLINECONTROLS mxlc = {0};
        mxlc.cbStruct = sizeof(mxlc);
        mxlc.dwLineID = mxl.dwLineID;
        mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
        mxlc.cControls = 1;
        mxlc.cbmxctrl = sizeof(MIXERCONTROL);
        mxlc.pamxctrl = pVolCtrl;
        mmr = ::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);
        //TRACE(_T("mixerGetLineControls return:%d\n"), mmr);
        if (MMSYSERR_NOERROR != mmr)
        {
            break;
        }
    } while (false);

    return mmr;
}

UINT CWaveRecorder::GetWaveInVolume()
{
    UINT nVolume = 0;
    MMRESULT mmr;

    if (m_hMixer == NULL)
    {
        return 0;
    }

    do 
    {
        MIXERCONTROL mxc;
        mmr = GetVolumeControl(&mxc);
        if (MMSYSERR_NOERROR != mmr)
        {
            break;
        }
        
        MIXERCONTROLDETAILS mxcd;
        MIXERCONTROLDETAILS_UNSIGNED vol; 
        vol.dwValue=0;
        mxcd.hwndOwner = NULL;
        mxcd.cbStruct = sizeof(mxcd);
        mxcd.dwControlID = mxc.dwControlID;
        mxcd.cbDetails = sizeof(vol);
        mxcd.paDetails = &vol;
        mxcd.cChannels = 1;
        mmr = ::mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
        TRACE(_T("mixerGetControlDetails return:%d\n"), mmr);
        if(mmr == MMSYSERR_NOERROR)
        {
            //TRACE(_T("volume:%d\n"), vol.dwValue);
            nVolume = vol.dwValue * 100 / mxc.Bounds.lMaximum;
        }
    } while (false);

    return nVolume;
}

void CWaveRecorder::SetWaveInVolume(UINT nVolume)
{
    MMRESULT mmr;
    do 
    {
        MIXERCONTROL mxc;
        mmr = GetVolumeControl(&mxc);
        if (MMSYSERR_NOERROR != mmr)
        {
            break;
        }
        nVolume *= (mxc.Bounds.lMaximum/100);

        MIXERCONTROLDETAILS mxcd;
        MIXERCONTROLDETAILS_UNSIGNED vol;
        vol.dwValue = nVolume;
        mxcd.hwndOwner = 0;
        mxcd.dwControlID = mxc.dwControlID;
        mxcd.cbStruct = sizeof(mxcd);
        mxcd.cbDetails = sizeof(vol);
        mxcd.paDetails = &vol;
        mxcd.cChannels = 1;
        ::mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE);
    } while (false);
}

//DWORD CWaveRecorder::GetWaveTimeLen()
//{
//    //WAV文件的播放时间＝样本数据长度÷数据传送速率
//    ASSERT(m_waveform.nAvgBytesPerSec != 0);
//    return m_dwRecordSize / m_waveform.nAvgBytesPerSec;
//}

LRESULT CWaveRecorder::OnDevChanged(WPARAM wParam, LPARAM lParam)
{
    LRESULT lRt = TRUE;

    if (DBT_DEVNODES_CHANGED == wParam)
    {
        //UINT nNum = waveInGetNumDevs();
        //TRACE(_T("OnDevChanged, num:%d\n"), nNum);
        //if (0 == nNum)
        //{
        //    waveInClose(m_hWaveIn);
        //    m_hWaveIn = NULL;
        //}
        //else
        //{
        //    ;
        //}
    }
    //DBT_CONFIGCHANGECANCELED;DBT_DEVICEARRIVAL

    return lRt;
}

LRESULT CWaveRecorder::OnLineChange(WPARAM wParam, LPARAM lParam)
{
    LRESULT lRt = TRUE;

    if (m_hMixer && (HMIXER)wParam == m_hMixer)
    {
    }

    return lRt;
}

LRESULT CWaveRecorder::OnControlChange(WPARAM wParam, LPARAM lParam)
{
    LRESULT lRt = TRUE;

    if (m_hMixer && (HMIXER)wParam == m_hMixer)
    {
    }

    return lRt;
}

LRESULT CWaveRecorder::OnStateChange(DWORD dwNewState)
{
    LRESULT lRt = TRUE;
    switch (dwNewState)
    {
    case DEVICE_STATE_UNPLUGGED:
        {
            TRACE(_T("microphone unplugged\n"));
            //此处调用，为了将m_hWaveIn等置为NULL
            _CloseWaveIn();
            TRACE(_T("microphone closed\n"));
        }
        break;
    case DEVICE_STATE_ACTIVE:
        {
            TRACE(_T("microphone actived\n"));
            _OpenWaveIn();
            TRACE(_T("microphone reopen\n"));
        }
        break;
    default:
        break;
    }

    return lRt;
}

HRESULT CWaveRecorder::RegisterDeviceEndPoint()
{
#define EXIT_ON_ERROR(hres)  \
    if (FAILED(hres)) { break; }

    HRESULT Hr = S_OK;

    do 
    {
        if (m_cpMMDevEnum)
        {
            break;
        }
        if (!IsVistaOrLater())
        {
            Hr = E_NOTIMPL;
            break;
        }

        Hr = m_cpMMDevEnum.CoCreateInstance(__uuidof(MMDeviceEnumerator)/*CLSID_MMDeviceEnumerator*/);
        if (FAILED(Hr))
        {
            break;
        }

#ifdef _DEBUG

        CComPtr<IMMDevice> pEndpoint = NULL;
        CComPtr<IPropertyStore> pProps = NULL;
        CComPtr<IMMDeviceCollection> pCollection = NULL;
        LPWSTR pwszID = NULL;

        Hr = m_cpMMDevEnum->EnumAudioEndpoints(
            eCapture, DEVICE_STATEMASK_ALL,
            &pCollection);
        EXIT_ON_ERROR(Hr);

        UINT  count;
        Hr = pCollection->GetCount(&count);
        EXIT_ON_ERROR(Hr);

        if (count == 0)
        {
            TRACE("No endpoints found.\n");
        }

        // Each loop prints the name of an endpoint device.

        for (ULONG i = 0; i < count; i++)
        {
            // Get pointer to endpoint number i.

            Hr = pCollection->Item(i, &pEndpoint);
            EXIT_ON_ERROR(Hr);

            // Get the endpoint ID string.

            Hr = pEndpoint->GetId(&pwszID);
            EXIT_ON_ERROR(Hr);

            DWORD dwState;
            pEndpoint->GetState(&dwState);
            LPCWSTR pszState;
            switch (dwState)
            {
            case DEVICE_STATE_ACTIVE:
                pszState = L"ACTIVE";
                break;
            case DEVICE_STATE_DISABLED:
                pszState = L"DISABLED";
                break;
            case DEVICE_STATE_NOTPRESENT:
                pszState = L"NOTPRESENT";
                break;
            case DEVICE_STATE_UNPLUGGED:
                pszState = L"UNPLUGGED";
                break;
            }

            Hr = pEndpoint->OpenPropertyStore(
                STGM_READ, &pProps);
            EXIT_ON_ERROR(Hr)

            PROPVARIANT varName;
            // Initialize container for property value.

            PropVariantInit(&varName);

            // Get the endpoint's friendly-name property.

            Hr = pProps->GetValue(
                PKEY_Device_FriendlyName, &varName);
            EXIT_ON_ERROR(Hr)

            // Print endpoint friendly name and endpoint ID.

            TRACE("Endpoint %d: \"%S\" (%S), state:DEVICE_STATE_%S (0x%8.8x)\n",
                i, varName.pwszVal, pwszID, pszState, dwState);

            CoTaskMemFree(pwszID);
            pwszID = NULL;
            PropVariantClear(&varName);
            pProps.Release();

            pEndpoint.Release();
        }
        pCollection.Release();
        CoTaskMemFree(pwszID);
#endif

        if (NULL == m_pMMClient)
        {
            Hr = E_OUTOFMEMORY;
            break;
        }
        Hr = m_cpMMDevEnum->RegisterEndpointNotificationCallback(m_pMMClient);
    } while (false);

    return Hr;
}

HRESULT CWaveRecorder::UnRegisterDeviceEndPoint()
{
    if (m_cpMMDevEnum)
    {
        ASSERT(m_pMMClient);
        m_cpMMDevEnum->UnregisterEndpointNotificationCallback(m_pMMClient);
        m_cpMMDevEnum.Release();
        m_cpMMDevEnum = NULL;
    }

    return S_OK;
}

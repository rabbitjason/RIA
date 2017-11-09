// RIA.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RIA.h"

#include "readingbarsetup.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
8E147C69-BD50-11d1-B137-0000F8753F5D
MSCHART�ؼ���lincense����֤��û�а�װ.NET Framework 3.5 SP1�Ļ�����Ҳ������������
*/
static WCHAR g_pwchChartLicenseKey[] =
{
    0x0038,	0x0045,	0x0031,	0x0034,	0x0037,	0x0043,	
    0x0036,	0x0039,	0x002D,	0x0042,	0x0044,	0x0035,	
    0x0030,	0x002D,	0x0031,	0x0031,	0x0064,	0x0031,	
    0x002D,	0x0042,	0x0031,	0x0033,	0x0037,	0x002D,	
    0x0030,	0x0030,	0x0030,	0x0030,	0x0046,	0x0038,	
    0x0037,	0x0035,	0x0033,	0x0046,	0x0035,	0x0044
};

/*
//8E147C69-BD50-11d1-B137-0000F8753F5D
WMP�ؼ���lincense
*/
//static WCHAR g_pwchWMPLicenseKey[] =
//{
//    ////0x0038,	0x0045,	0x0031,	0x0034,	0x0037,	0x0043,	
//    ////0x0036,	0x0039,	0x002D,	0x0042,	0x0044,	0x0035,	
//    ////0x0030,	0x002D,	0x0031,	0x0031,	0x0064,	0x0031,	
//    ////0x002D,	0x0042,	0x0031,	0x0033,	0x0037,	0x002D,	
//    ////0x0030,	0x0030,	0x0030,	0x0030,	0x0046,	0x0038,	
//    ////0x0037,	0x0035,	0x0033,	0x0046,	0x0035,	0x0044
//};

BSTR CRIAApp::m_bstrChartLicense = NULL;
BSTR CRIAApp::m_bstrWMPLicense = NULL;

// CRIAApp

BEGIN_MESSAGE_MAP(CRIAApp, CWinApp)
END_MESSAGE_MAP()


// CRIAApp ����

CRIAApp::CRIAApp() : m_hMutex(NULL)
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRIAApp ����

CRIAApp theApp;


static _invalid_parameter_handler g_old_invalid_parameter_handler = NULL;

//The parameters all have the value NULL unless a debug version of the CRT library is used.
void myInvalidParameterHandler(const wchar_t* expression,
                               const wchar_t* function, 
                               const wchar_t* file, 
                               unsigned int line, 
                               uintptr_t pReserved)
{
    TRACE(L"Invalid parameter detected in function %s."
        L" File: %s Line: %d\n", function, file, line);
    TRACE(L"Expression: %s\n", expression);
    if (g_old_invalid_parameter_handler)
    {
        g_old_invalid_parameter_handler(expression, function, file, line, pReserved);
    }
}

CString g_strVersion;
CString g_strCopyright;

#define DMP_FILE    _T("ria")
LPTOP_LEVEL_EXCEPTION_FILTER g_lpOldExcetion = NULL;
LONG WINAPI myUnhandledExceptionFilter(
                                       __in          struct _EXCEPTION_POINTERS* ExceptionInfo
                                       )
{
    TRACE(_T("ExceptionInfo, code:0x%x, address:%p\n"),
        ExceptionInfo->ExceptionRecord->ExceptionCode,
        ExceptionInfo->ExceptionRecord->ExceptionAddress);
    CTime ctnow = CTime::GetCurrentTime();
    CString strDump;

    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

#ifdef _DEBUG
    strDump.Format(_T("%sSystem\\%s_%s_dbg_%04d-%02d-%02d_%02d-%02d-%02d.dmp"),
        CPaintManagerUI::GetInstancePath().GetData(), DMP_FILE,
        g_strVersion,
        ctnow.GetYear(), ctnow.GetMonth(), ctnow.GetDay(),
        ctnow.GetHour(), ctnow.GetMinute(), ctnow.GetSecond());
#else
    strDump.Format(_T("%sSystem\\%s_%s_%04d-%02d-%02d_%02d-%02d-%02d.dmp"),
        CPaintManagerUI::GetInstancePath().GetData(), DMP_FILE,
        g_strVersion,
        ctnow.GetYear(), ctnow.GetMonth(), ctnow.GetDay(),
        ctnow.GetHour(), ctnow.GetMinute(), ctnow.GetSecond());
#endif
    HANDLE hFile = CreateFile(strDump,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        API_VERSION DbgVersion;
        memcpy(&DbgVersion, ImagehlpApiVersion(), sizeof(API_VERSION));
        TRACE(_T("DbgAPI version:%d.%d\n"), DbgVersion.MajorVersion, DbgVersion.MinorVersion);
        DWORD dwFlags = MiniDumpWithFullMemory |
            MiniDumpWithDataSegs |
            MiniDumpWithHandleData;/* |
            MiniDumpFilterMemory |
            MiniDumpScanMemory;*/
        if (DbgVersion.MajorVersion > 5)
        {
            dwFlags |= MiniDumpWithProcessThreadData | MiniDumpWithIndirectlyReferencedMemory;
        }
        //DbgHelp 6.1 and earlier:  This value is not supported
        if ((DbgVersion.MajorVersion > 6) ||
            ( (DbgVersion.MajorVersion == 6) && (DbgVersion.MinorVersion > 1) ))
        {
            dwFlags |= (MiniDumpWithFullMemoryInfo |
                MiniDumpWithThreadInfo/* |
                MiniDumpWithCodeSegs*/);
        }
        MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)dwFlags;
        MINIDUMP_EXCEPTION_INFORMATION mei;
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = ExceptionInfo;
        mei.ClientPointers = FALSE;
        BOOL bWr = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
            mdt,
            &mei, NULL, NULL);
#ifdef _DEBUG
        if (!bWr)
        {
            TRACE(_T("write dump file failed:%s\n"), MyFormatMessage(GetLastError()));
        }
#endif
        CloseHandle(hFile);
    }
#ifdef _DEBUG
    else
    {
        MyAfxMessageBox(_T("create dump file eror:%s"), MyFormatMessage(GetLastError()));
    }
#endif
    MyAfxMessageBox(_T("�����쳣�����˳���\r\n�뽫%s�ļ��ύ����֧����Ա��"),
        strDump);
    //or EXCEPTION_CONTINUE_SEARCH ??
    return g_lpOldExcetion ? g_lpOldExcetion(ExceptionInfo) : EXCEPTION_EXECUTE_HANDLER;
}

// CRIAApp ��ʼ��

BOOL CRIAApp::InitInstance()
{
    //_CrtSetReportMode(_CRT_ASSERT, 0);
#ifndef _DEBUG
    if (IsDebuggerPresent())
    {
        return FALSE;
    }
#endif
    g_old_invalid_parameter_handler = _set_invalid_parameter_handler(myInvalidParameterHandler);
    g_lpOldExcetion = SetUnhandledExceptionFilter(myUnhandledExceptionFilter);
    GetApplicationVersion();

    API_VERSION DbgVersion = {0};
    DbgVersion.MajorVersion = 6;
    DbgVersion.MinorVersion = 11;
    memcpy(&DbgVersion, ImagehlpApiVersionEx(&DbgVersion), sizeof(API_VERSION));
    TRACE(_T("Ex DbgAPI version:%d.%d\n"), DbgVersion.MajorVersion, DbgVersion.MinorVersion);

    memset(&DbgVersion, 0, sizeof(DbgVersion));
    memcpy(&DbgVersion, ImagehlpApiVersion(), sizeof(API_VERSION));
    TRACE(_T("DbgAPI version:%d.%d\n"), DbgVersion.MajorVersion, DbgVersion.MinorVersion);

#if _MFC_VER <= 0x0900
    _tsetlocale(LC_ALL, _T("chs"));
#endif

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    HANDLE hMutex = ::CreateMutex(NULL, TRUE, MUTEX_NAME_LOADER );
    if( hMutex != NULL )
    {
        DWORD dwErrx = GetLastError();
        CloseHandle(hMutex);
        if  ( dwErrx == ERROR_ALREADY_EXISTS)
        {
            AfxMessageBox( _T("������ڽ�����������ȴ���������ٽ���������") );
            return FALSE;
        }
    }
    else
    {
        AfxMessageBox( _T("��Դ����ʧ��!!") );
        return FALSE;
    }

#if SKIN==SKN_WIN8
    LPCTSTR pszName = MUTEX_NAME_QJL;
#else
    LPCTSTR pszName = MUTEX_NAME_RIA;
#endif
    m_hMutex = ::CreateMutex(NULL, TRUE, pszName );
    if( m_hMutex != NULL )
    {
        DWORD dwErr = GetLastError();
        if  ( dwErr == ERROR_ALREADY_EXISTS)
        {
            AfxMessageBox( _T("���Ѿ������˱������ллʹ��") );  //�Ѿ������˳���
            CloseHandle(m_hMutex);
            return FALSE;
        }
    }
    else
    {
        AfxMessageBox( _T("��Դ����ʧ�ܣ�") );  //�Ѿ������˳���
        return FALSE;
    }

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    if (NULL == m_bstrChartLicense)
    {
        m_bstrChartLicense = ::SysAllocStringLen(g_pwchChartLicenseKey, _countof(g_pwchChartLicenseKey));
        if (NULL == m_bstrChartLicense)
        {
            //MyAfxMessageBox(_T("����MSChart�ؼ�ʧ�ܣ��ڴ治�㡣"));
            TRACE(_T("����MSChart�ؼ�ʧ�ܣ��ڴ治�㡣"));
            return FALSE;
        }
    }

    //if (NULL == m_bstrWMPLicense)
    //{
    //    m_bstrWMPLicense = ::SysAllocStringLen(g_pwchWMPLicenseKey, _countof(g_pwchWMPLicenseKey));
    //    if (NULL == m_bstrWMPLicense)
    //    {
    //        //MyAfxMessageBox(_T("����MSChart�ؼ�ʧ�ܣ��ڴ治�㡣"));
    //        TRACE(_T("����MSWMP�ؼ�ʧ�ܣ��ڴ治�㡣"));
    //        return FALSE;
    //    }
    //}

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

    CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    //TCHAR szPath[1024] = {_T('\0'),};
    //if (SUCCEEDED(::SHGetFolderPath(NULL,
    //    CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath)))
    //{
    //    CString strTmp;
    //    strTmp.Format(_T("%s\\aiengine"), szPath);
    //    TRACE(_T("sn failed, delete %s\n"), strTmp);
    //    MyRemoveDir(strTmp);
    //}

#ifdef _DEBUG
    TCHAR szOut[1024] = {_T('\0')};
    //int f = (int)::FindExecutable(_T("cmd.exe"), _T("C:\\windows\\system32"), szOut);
    //TRACE(_T("FindExecutable return:0x%x, %s\n"), f, szOut);

    CString strExt = _T(".exe");
    DWORD dwOut = _countof(szOut);
    HRESULT Hr = ::AssocQueryString(ASSOCF_REMAPRUNDLL | ASSOCF_NOFIXUPS,
        ASSOCSTR_COMMAND, strExt, NULL, szOut, &dwOut);
    TRACE(_T("AssocQueryString return:0x%x, %s\n"), Hr, szOut);
    szOut[0] = _T('\0');
    Hr = ::AssocQueryString(ASSOCF_REMAPRUNDLL | ASSOCF_NOFIXUPS,
        ASSOCSTR_COMMAND, strExt, _T("open"), szOut, &dwOut);
    TRACE(_T("AssocQueryString with open return:0x%x, %s\n"), Hr, szOut);

    //IQueryAssociations* pAssoc = NULL;
    //Hr = ::AssocCreate(CLSID_QueryAssociations, IID_IQueryAssociations, (void**)&pAssoc);
    //if (SUCCEEDED(Hr))
    //{
    //    Hr = pAssoc->Init(NULL, CT2W(strExt), NULL, NULL);
    //    Hr = pAssoc->GetString(ASSOCF_REMAPRUNDLL | ASSOCF_NOFIXUPS, ASSOCSTR_COMMAND, _T("open"), szOut, &dwOut);
    //    pAssoc->Release();
    //}
#endif
    
    LPWSTR lpCmd = ::GetCommandLineW();
    int num = 0, i;
    LPWSTR *szArglist = CommandLineToArgvW(lpCmd, &num);
    bool bRun = false;
    CString strExe = (LPTSTR)CW2T(szArglist[0]);
    for (i = 1; i < num; i++)
    {
        LPWSTR lpC = szArglist[i];
        if (L'/' == *lpC ||
            L'-' == *lpC)
        {
            lpC++;
        }
        TRACE("%d, %ws\n", i, szArglist[i]);
        if (_wcsicmp(lpC, L"run") == 0)
        {
            bRun = true;
        }
    }
    ::LocalFree(szArglist);

    MainFrame* pFrame = NULL;
    try
    {
        pFrame = new MainFrame();
    }
    catch(CMemoryException* e)
    {
        e->Delete();
    }
    catch(CUserException* e)
    {
        e->Delete();
    }
    catch(CException* e)
    {
        e->ReportError();
        e->Delete();
    }

    if( pFrame == NULL ) return FALSE;

    if (!pFrame->CheckUpdate(bRun, strExe))
    {
        MainFrame::ReleaseZIPBuffer();
        delete pFrame;
        return FALSE;
    }
    //pFrame->SetCurrentResourceID(IDR_ZIPRES);
    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);
#if defined(WIN32) && !defined(UNDER_CE)
    //��Ҫʹ��UI_WNDSTYLE_FRAME��Ӧ����UI_WNDSTYLE_DIALOG
    pFrame->Create(NULL, strTitle, UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 1000, 750);
#else
    pFrame->Create(NULL, strTitle, UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
    pFrame->SetIcon(IDR_MAINFRAME);
    pFrame->CenterWindow();
    
    m_pMainWnd = NULL;//CWnd::FromHandle(pFrame->GetHWND());

    ::ShowWindow(*pFrame, SW_SHOW);

    TCHAR errmsg[1024];
    try
    {
        CPaintManagerUI::MessageLoop();
        CPaintManagerUI::Term();
    }
    catch (CMemoryException* e)
    {
        e->GetErrorMessage(errmsg, _countof(errmsg));
        TRACE(_T("CMemoryException:%s\n"), errmsg);
    }
    catch (CFileException* e)
    {
        e->GetErrorMessage(errmsg, _countof(errmsg));
        TRACE(_T("CFileException:%s\n"), errmsg);
    }
    catch (CException* e)
    {
        e->GetErrorMessage(errmsg, _countof(errmsg));
        TRACE(_T("CException:%s\n"), errmsg);
    }
    catch(...)
    {
        TRACE(_T("CException\n"));
    }

    MainFrame::ReleaseZIPBuffer();

	return FALSE;
}

int CRIAApp::ExitInstance()
{
    if (m_bstrChartLicense)
    {
        ::SysFreeString(m_bstrChartLicense);
        m_bstrChartLicense = NULL;
    }
    if (m_bstrWMPLicense)
    {
        ::SysFreeString(m_bstrWMPLicense);
        m_bstrWMPLicense = NULL;
    }

    GdiplusShutdown(m_gdiplusToken);
    CloseHandle(m_hMutex);
    return CWinApp::ExitInstance();
}

// CRIAApp ��Ϣ�������

void CRIAApp::GetApplicationVersion()
{
    TCHAR szFullPath[MAX_PATH];
    DWORD dwVerInfoSize = 0;
    DWORD dwVerHnd;
    VS_FIXEDFILEINFO * pFileInfo;

    GetModuleFileName(NULL, szFullPath, _countof(szFullPath));
    dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
    if (dwVerInfoSize)
    {
        // If we were able to get the information, process it:
        HANDLE  hMem;
        LPVOID  lpvMem;
        unsigned int uInfoSize = 0;

        hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
        lpvMem = GlobalLock(hMem);
        GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);

        ::VerQueryValue(lpvMem, _T("\\"), (void**)&pFileInfo, &uInfoSize);

        //int ret = GetLastError();
        WORD m_nProdVersion[4];

        // Product version from the FILEVERSION of the version info resource 
        m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS); 
        m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
        m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
        m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS); 

        g_strVersion.Format(_T("%d.%d.%d.%d"),m_nProdVersion[0],
            m_nProdVersion[1],m_nProdVersion[2],m_nProdVersion[3]);
        TRACE(_T("ProductVersion:%s\n"), g_strVersion);

        LPTSTR lpCR;
        if (::VerQueryValue(lpvMem, _T("\\StringFileInfo\\080403a8\\LegalCopyright"), (LPVOID*)&lpCR, &uInfoSize))
        {
            lpCR[uInfoSize] = _T('\0');
            g_strCopyright = lpCR;
        }

        GlobalUnlock(hMem);
        GlobalFree(hMem);

    }
}
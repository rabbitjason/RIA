// RIA.cpp : 定义应用程序的类行为。
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
MSCHART控件的lincense，保证在没有安装.NET Framework 3.5 SP1的机器上也可以正常运行
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
WMP控件的lincense
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


// CRIAApp 构造

CRIAApp::CRIAApp() : m_hMutex(NULL)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRIAApp 对象

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
    MyAfxMessageBox(_T("程序异常即将退出。\r\n请将%s文件提交技术支持人员。"),
        strDump);
    //or EXCEPTION_CONTINUE_SEARCH ??
    return g_lpOldExcetion ? g_lpOldExcetion(ExceptionInfo) : EXCEPTION_EXECUTE_HANDLER;
}

// CRIAApp 初始化

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

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    HANDLE hMutex = ::CreateMutex(NULL, TRUE, MUTEX_NAME_LOADER );
    if( hMutex != NULL )
    {
        DWORD dwErrx = GetLastError();
        CloseHandle(hMutex);
        if  ( dwErrx == ERROR_ALREADY_EXISTS)
        {
            AfxMessageBox( _T("软件正在进行升级，请等待升级完成再进行启动。") );
            return FALSE;
        }
    }
    else
    {
        AfxMessageBox( _T("资源申请失败!!") );
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
            AfxMessageBox( _T("您已经运行了本软件！谢谢使用") );  //已经运行了程序
            CloseHandle(m_hMutex);
            return FALSE;
        }
    }
    else
    {
        AfxMessageBox( _T("资源申请失败！") );  //已经运行了程序
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
            //MyAfxMessageBox(_T("创建MSChart控件失败，内存不足。"));
            TRACE(_T("创建MSChart控件失败，内存不足。"));
            return FALSE;
        }
    }

    //if (NULL == m_bstrWMPLicense)
    //{
    //    m_bstrWMPLicense = ::SysAllocStringLen(g_pwchWMPLicenseKey, _countof(g_pwchWMPLicenseKey));
    //    if (NULL == m_bstrWMPLicense)
    //    {
    //        //MyAfxMessageBox(_T("创建MSChart控件失败，内存不足。"));
    //        TRACE(_T("创建MSWMP控件失败，内存不足。"));
    //        return FALSE;
    //    }
    //}

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
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
    //不要使用UI_WNDSTYLE_FRAME，应该是UI_WNDSTYLE_DIALOG
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

// CRIAApp 消息处理程序

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
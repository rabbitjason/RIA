// MFCDlgSR.cpp : implementation file
//

#include "stdafx.h"
#include "RIA.h"
#include "CMciWave.h"
#include "WaveRecorder.h"
#include "main_frame.h"
#include "MFCDlgSR.h"

// copy from UIRender.cpp [2016-3-4 Arthur]
///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
    int index;                 // index of this file within the zip
    char name[MAX_PATH];       // filename within the zip
    DWORD attr;                // attributes, as in GetFileAttributes.
    FILETIME atime,ctime,mtime;// access, create, modify filetimes
    long comp_size;            // sizes of item, compressed and uncompressed. These
    long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
    int index;                 // index of this file within the zip
    TCHAR name[MAX_PATH];      // filename within the zip
    DWORD attr;                // attributes, as in GetFileAttributes.
    FILETIME atime,ctime,mtime;// access, create, modify filetimes
    long comp_size;            // sizes of item, compressed and uncompressed. These
    long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////


// CMFCDlgSR dialog

IMPLEMENT_DYNAMIC(CMFCDlgSR, CDialog)

CMFCDlgSR::CMFCDlgSR(MainFrame* pMyParent/* = NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CMFCDlgSR::IDD, pParent)
    , m_pParent(pMyParent)
    , m_pBkImage(NULL)
    , m_pPrgsBkImage(NULL)
    , m_pPrgsFrImage(NULL)
    , m_nValue(0)
{
    //125, 155, 250, 20
    m_Rct.left = 125;
    m_Rct.top = 155;
    m_Rct.right = m_Rct.left + 250;
    m_Rct.bottom = m_Rct.top + 25;
}

CMFCDlgSR::~CMFCDlgSR()
{
    if (m_pBkImage)
    {
        delete m_pBkImage;
    }
    if (m_pPrgsBkImage)
    {
        delete m_pPrgsBkImage;
    }
    if (m_pPrgsFrImage)
    {
        delete m_pPrgsFrImage;
    }
}

void CMFCDlgSR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMFCDlgSR, CDialog)
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCDlgSR message handlers

BOOL CMFCDlgSR::OnInitDialog()
{
    ASSERT(m_pParent);

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_pParent->m_hSubWnd = GetSafeHwnd();

    m_pBkImage = MyLoadImage(_T("Image/loadImg.png"));
    m_pPrgsBkImage = MyLoadImage(_T("Image/progress_back.png"));
    m_pPrgsFrImage = MyLoadImage(_T("Image/progress_fore.png"));
    //TRACE(_T("LoadImage return:0x%p, %p, %p\n"), m_pBkImage, m_pPrgsBkImage, m_pPrgsFrImage);

    if(m_pBkImage)
    {
        int width = m_pBkImage->GetWidth();
        int height = m_pBkImage->GetHeight();
        //TRACE(_T("width:%d, height:%d\n"), width, height);
        SetWindowPos(NULL,0,0,width,height,SWP_NOMOVE);
        CenterWindow();
    }

    SetTimer(100, 1000, NULL);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

Image* CMFCDlgSR::MyLoadImage(LPCTSTR lpstImagePath)
{
    Image* pImage = NULL;
    HGLOBAL pData = NULL;
    DWORD dwErr = ERROR_SUCCESS;

    do 
    {
        DWORD dwSize;
        CDuiString sFile = CPaintManagerUI::GetResourcePath();
        if( CPaintManagerUI::GetResourceZip().IsEmpty() )
        {
            sFile += lpstImagePath;
            HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                FILE_ATTRIBUTE_NORMAL, NULL);
            if( hFile == INVALID_HANDLE_VALUE )
            {
                dwErr = GetLastError();
                break;
            }
            dwSize = ::GetFileSize(hFile, NULL);
            if (INVALID_FILE_SIZE == dwSize)
            {
                dwErr = GetLastError();
                ::CloseHandle( hFile );
                break;
            }
            if( dwSize == 0 )
            {
                dwErr = ERROR_NO_MORE_ITEMS;
                ::CloseHandle( hFile );
                break;
            }

            DWORD dwRead = 0;
            pData = GlobalAlloc(GHND, dwSize);
            if (NULL == pData)
            {
                dwErr = GetLastError();
                ::CloseHandle( hFile );
                break;
            }
            PVOID p = GlobalLock(pData);
            if (NULL == p)
            {
                dwErr = GetLastError();
                GlobalFree(pData);
                pData = NULL;
                break;
            }
            ::ReadFile( hFile, p, dwSize, &dwRead, NULL );
            ::CloseHandle( hFile );

            if( dwRead != dwSize ) {
                dwErr = ERROR_INVALID_PARAMETER;
                GlobalUnlock(p);
                GlobalFree(pData);
                pData = NULL;
                break;
            }
            GlobalUnlock(p);
        }
        else
        {
            sFile += CPaintManagerUI::GetResourceZip();
            HZIP hz = NULL;
            if( CPaintManagerUI::IsCachedResourceZip() ) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
            else hz = OpenZip((void*)sFile.GetData(), 0, 2);
            if( hz == NULL )
            {
                dwErr = ERROR_INVALID_HANDLE;
                break;
            }
            ZIPENTRY ze; 
            int i; 
            if( FindZipItem(hz, lpstImagePath, true, &i, &ze) != 0 )
            {
                dwErr = ERROR_NO_MORE_ITEMS;
                if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
                break;
            }
            dwSize = ze.unc_size;
            if( dwSize == 0 )
            {
                dwErr = ERROR_NO_MORE_ITEMS;
                break;
            }
            pData = GlobalAlloc(GHND, dwSize);
            if (NULL == pData)
            {
                dwErr = GetLastError();
                if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
                break;
            }
            PVOID p = GlobalLock(pData);
            if (NULL == p)
            {
                dwErr = GetLastError();
                GlobalFree(pData);
                pData = NULL;
                if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
                break;
            }
            int res = UnzipItem(hz, i, p, dwSize, 3);
            if( res != 0x00000000 && res != 0x00000600)
            {
                dwErr = ERROR_INVALID_PARAMETER;
                GlobalUnlock(p);
                GlobalFree(pData);
                pData = NULL;
                if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
                break;
            }
            if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
            GlobalUnlock(p);
        }
    } while (false);

    if (pData)
    {
        IStream* pStream;
        CreateStreamOnHGlobal(pData, FALSE, &pStream);
        pImage = Image::FromStream(pStream);
        //TRACE(_T("FromStream return:%p\n"), pImage);
        pStream->Release();
        GlobalFree(pData);
    }

    return pImage;
}

void CMFCDlgSR::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == 100)
    {
        TRACE(_T("m_pParent->m_bAICompleted:%d\n"), m_pParent->m_AiEngine.IsCallbackQuit());
        if (m_pParent->m_AiEngine.IsCallbackQuit())
        {
            KillTimer(nIDEvent);
            EndDialog(IDOK);
        }
        else
        {
            m_nValue++;
            if (m_nValue > 10)
            {
                m_nValue = 0;
            }
            InvalidateRect(m_Rct);
            UpdateWindow();
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CMFCDlgSR::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages

    //CDC* pDC = GetDC();
    //Graphics grc(pDC->m_hDC);
    Graphics grc(dc.m_hDC);
    if(m_pBkImage)
    {
        grc.DrawImage(m_pBkImage, 0, 0);
    }
    //½ø¶ÈÌõ£¬125,155, 250*20
    if (m_pPrgsBkImage)
    {
        grc.DrawImage(m_pPrgsBkImage, m_Rct.left, m_Rct.top, m_Rct.Width(), m_Rct.Height());
    }
    if (m_pPrgsFrImage)
    {
        grc.DrawImage(m_pPrgsFrImage, m_Rct.left, m_Rct.top, m_Rct.Width() * m_nValue / 10, m_Rct.Height());
    }
    //ReleaseDC(pDC);
}

void CMFCDlgSR::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    m_pParent->m_hSubWnd = NULL;
}

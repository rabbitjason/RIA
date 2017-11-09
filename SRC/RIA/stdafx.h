// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类


 // 此处要求最小 DB 支持。未选择任何视图。

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>

#include <afxsock.h>            // MFC 套接字扩展
#include <atlenc.h>


#include <atlbase.h>
#include <afxoledb.h>
#include <atlplus.h>
#include <atlutil.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <locale>
#include <map>
using namespace std;

//There is a bug with TRACE and _CrtDbgReport when working with the wide string.
//_CrtDbgReport can't handle the wide string correctly when the wide string contains non-ascii characters. 
//并运行setlocale(LC_ALL, "chs");
//_MSC_VER
#if _MFC_VER <= 0x0900
#include <locale.h>
#endif

#include <wmpids.h>
#include <wmp.h>

#include <sapi.h>           // SAPI includes
//#pragma warning(push)
//#pragma warning(disable:4995)
#include <sphelper.h>
//#pragma warning(pop)
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

#include <mmsystem.h>
#include <mmreg.h>
#include <Dbt.h>
//#include <dsound.h>

#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>

//for richedit
#include <InitGuid.h>
#include <Richedit.h>
#include <richole.h>
#include <imm.h>
#include <textserv.h>
#include <TOM.h>

DEFINE_GUID(IID_ITextDocument,0x8CC497C0,0xA1DF,0x11CE,0x80,0x98,
               0x00,0xAA,0x00,0x47,0xBE,0x5D);

#include <GdiPlus.h>
using namespace Gdiplus;

#include <tchar.h>
#include <strsafe.h>

//#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename ("EOF", "adoEOF")
#import "C:\\Program Files\\Common Files\\System\\ado\\msado28.tlb" no_namespace rename ("EOF", "adoEOF")
//将Window XP SP3的msado15.dll文件拷贝到此目录下，Win7 SP1编译在老OS上的数据库CreateInstance失败问题
//#import "../lib/msado15.dll" no_namespace rename ("EOF", "adoEOF")
#import "./NYRlib.dll" no_namespace

//#include "dbghelp.h"
#include "RIA.h"
#include "../include/common.h"

//for MSCHART control
#define MSCHART_CLASS_NAME  _T("MyMsChartOcx")
#include "mschart_def.h"

//#include "C:/windows/system32/wmp.dll"
#define MSWMP_CLASS_NAME    _T("MyMsWMPOcx")
#include "./wmp/CWMPPlayer4.h"
#include "./wmp/CWMPSettings.h"
#include "./wmp/CWMPControls.h"
#include "./wmp/CWMPMedia.h"

#include "dbfile.h"

//使用静态库
#include "../include/DuiLib/UIlib.h"
using namespace DuiLib;

#include "resource.h"

#include "StringUtils.h"
#include "../include/json/json.h"
#include "aiengine.h"


//练听力用的
typedef pair<CString, CString>  CString_Pair;
typedef map<CString, CString>   MyDictionary;

//##__VA_ARGS__
BOOL IsNumeric(LPCSTR strString);
BOOL IsNumeric(LPCWSTR strString);
bool IncludeChinese(LPCTSTR pstrString);

const TCHAR* const g_strDBaseFile = _T("Data.db");
const TCHAR* const g_strStudyDBaseFile = _T("User.db");

//录音的声道等，见CWaveRecorder::Open()函数及MainFrame::SpeechRecognitionCompare2()
const WORD  g_wChannel = 1;
const WORD  g_wBitsPerSample = 16;
const DWORD g_dwSamplesPerSec = 16000;

//0 <= return value < 1000
int SQLRand();
CString GetCollect(_RecordsetPtr& pPtr, LPCTSTR strName);
CString GetCollect(_RecordsetPtr& pPtr, long nItem);
CString GetCollect(_RecordsetPtr& pPtr, _variant_t vName);
CString GetColumnName(_RecordsetPtr& pPtr, long nCol);
int GetCollectCount(_RecordsetPtr& pPtr);

void SpeakCheckText(__inout wstring& strText);
LPWSTR SpeakCheckText(__inout LPWSTR lpwstrText, __in DWORD dwBufCharsLen);

int GetSentenceTime(LPCTSTR lpstrSentenc);

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

#ifdef _DEBUG
void dump_hexf(TCHAR* msg, const void* packet, int packet_len);
#define dump_hex(msg, p, flags) dump_hexf(msg, p, flags)
#else
#define dump_hex(msg, p, flags) __noop
#endif

#define USER_CONTROL_ID_BASE    WM_APP
#define USER_WMP_MP3_ID         (USER_CONTROL_ID_BASE + 1)
#define USER_WMP_WMV_ID         (USER_CONTROL_ID_BASE + 2)

//const HMENU nListenComboID = HMENU(USER_CONTROL_ID_BASE + 1);

//TTS消息
#define WM_TTSAPPCUSTOMEVENT    (WM_USER+0x64)
//SR消息
#define WM_SRAPPCUSTOMEVENT     (WM_USER+0x65)
//识别完成消息
#define WM_SRENDSTREAM          (WM_USER+0x66)
//microphone state change
#define WM_WAVEINSTATE          (WM_USER+0x67)


//公共按钮
const TCHAR* const kCloseButtonName  = _T("closebtn");
const TCHAR* const kReturnButtonName = _T("returnbtn");
const TCHAR* const kMinButtonName    = _T("minbtn");
const TCHAR* const kDictButtonName   = _T("dictbtn");
const TCHAR* const kSetButtonName    = _T("setbtn");
const TCHAR* const kSubReturnButtonName = _T("subreturnbtn");

//typedef vector<CString> MyListText;

#define DUI_ALPHA(x)    ((x) << 24)
#define XRGB(r,g,b)     ( (((DWORD)((BYTE)(r))) << 16) | (((WORD)((BYTE)(g))) << 8) | ((BYTE)(b)) )
#define DUI_RGB(r,g,b)    (DUI_ALPHA(0xff) | XRGB(r,g,b))
#define DUI_BLACK   DUI_RGB(0,0,0)
#define DUI_WHITE   DUI_RGB(255,255,255)

#define DEFAULT_TEXT_COLOR  DUI_WHITE

#define xFindControl(T, name) static_cast<T*>(m_PaintManager.FindControl(name))

#define SKIN_FOLDER _T("")
#define SKIN_TYPE   UILIB_ZIPRESOURCE
//#define SKIN_FOLDER _T("skin")
//#define SKIN_TYPE   UILIB_FILE

#define SKN_SMART   0
#define SKN_WIN8    1

//基础版本定义此宏
//#define APP_DEMO

#define DEMO_TIP    _T("您使用的软件为基础版，暂不支持此功能，购买高级版本软件后可正常使用。")
#ifndef __AIENGINE_H__
#define __AIENGINE_H__

typedef BOOLEAN (WINAPI *PRtlTimeToSecondsSince1970)(__in          PLARGE_INTEGER Time,
                                                     __out         PULONG ElapsedSeconds
                                                     );

//aiengine_callback(ByVal usrdata As IntPtr, <MarshalAs(UnmanagedType.LPStr)> ByVal id As String, ByVal type As Integer, <MarshalAs(UnmanagedType.LPArray, ArraySubType:=UnmanagedType.I1, SizeParamIndex:=4)> ByVal message As Byte(), ByVal size As Integer) As Integer

typedef int (WINAPI *paiengine_callback)(PVOID lpUsrdata, LPCSTR strID, int nType, LPBYTE lpMessage, int nSize);

typedef int (WINAPI *paiengine_cancel)(HANDLE hEngine);
typedef int (WINAPI *paiengine_delete)(HANDLE hEengine);
typedef int (WINAPI *paiengine_feed)(HANDLE hEengine, LPBYTE lpData, int nSize);
typedef int (WINAPI *paiengine_get_device_id)(LPBYTE lpDeviceId);
typedef HANDLE (WINAPI *paiengine_new)(LPCSTR strCfg);
typedef int (WINAPI *paiengine_opt)(HANDLE hEngine, int nOpt, LPBYTE lpData, int nSize);
typedef int (WINAPI *paiengine_start)(HANDLE hEngine, LPCSTR strParam, LPBYTE lpID,
                               paiengine_callback callback, PVOID lpUsrdata);
typedef int (WINAPI *paiengine_stop)(HANDLE hEngine);

#define AI_DAT_LEN  64

class CAiEngine
{
public:
    CAiEngine();
    ~CAiEngine();

    static DWORD LoadFunc();
    static void Release();
    //检查参考文字，替换/删除不支持、不允许的字符等
    static void CheckRefText(__inout CString& strReftext);
    static LPSTR CheckRefText(__inout LPSTR lpstrReftext, __in DWORD dwBufCharsLen);
    static LPWSTR CheckRefText(__inout LPWSTR lpstrReftext, __in DWORD dwBufCharsLen);

    static int aiengine_cancel(HANDLE hEngine);
    static int aiengine_delete(HANDLE hEngine);
    static int aiengine_feed(HANDLE hEngine, LPBYTE lpData, int nSize);
    static int aiengine_get_device_id(LPBYTE lpDeviceId);
    static HANDLE aiengine_new(LPCSTR strCfg);
    static int aiengine_opt(HANDLE hEngine, int nOpt, LPBYTE lpData, int nSize);
    static int aiengine_start(HANDLE hEngine, LPCSTR strParam, LPBYTE lpID,
        paiengine_callback callback, PVOID lpUsrdata);
    static int __stdcall aiengine_callback(PVOID volatile lpUsrdata, LPCSTR strID, int nType, LPBYTE lpMessage, int nSize);
    static int aiengine_stop(HANDLE hEngine);
    static string GetTimeStamp();
    static string GetDeviceId();
    static string GetSAH(const string& strTimeStamp, const string& strId);
    static wstring GetErrMsg(UINT nId);
    static CString GetSpeechChar(const CString& strCode);
    
    //return TRUE if callback quit
    static BOOL IsCallbackQuit();

    static const int AIENGINE_MESSAGE_TYPE_JSON = 1;
    static const int AIENGINE_MESSAGE_TYPE_BIN = 2;
    static const int AIENGINE_OPT_GET_VERSION = 1;
    static const int AIENGINE_OPT_GET_MODULES = 2;
    static const int AIENGINE_OPT_GET_TRAFFIC = 3;
    static const int AIENGINE_SET_WIFI_STATUS = 4;
    static const int AIENGINE_OPT_GET_PROVISION = 5;
    static const int AIENGINE_GET_SERIAL_NUMBER = 6;
    static const char* m_appKey;
    static const char* m_secretKey;
    static const char* m_appKey0;
    static const char* m_secretKey0;
    typedef map<CString, CString> AI_SCODE;
    typedef pair <CString, CString> SCODE_Pair;
protected:
    static void  SetEventState(BOOL bSigned);
    //byte  m_device_id[AI_DAT_LEN+1];
private:
    static paiengine_cancel m_paiengine_cancel;
    static paiengine_delete m_paiengine_delete;
    static paiengine_feed m_paiengine_feed;
    static paiengine_get_device_id m_paiengine_get_device_id;
    static paiengine_new m_paiengine_new;
    static paiengine_opt m_paiengine_opt;
    static paiengine_start m_paiengine_start;
    static paiengine_stop m_paiengine_stop;

    static HMODULE m_hModule;
    static HANDLE  m_hEvent;

    static PRtlTimeToSecondsSince1970 m_PRtlTimeToSecondsSince1970;
    static AI_SCODE  m_SpeechEncode;
#ifdef _DEBUG
    static CStdioFile* m_flog;
#endif
};

#endif
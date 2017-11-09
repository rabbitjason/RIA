// stdafx.cpp : 只包括标准包含文件的源文件
// RIA.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "CMciWave.h"
#include "WaveRecorder.h"
#include "main_frame.h"

template<typename T>
BOOL _IsNumeric(T strString)
{
    if (!strString || !*strString)
    {
        return FALSE;
    }

    //skip the first - char
    if (*strString == _T('-'))
    {
        strString++;
    }
    while(*strString)
    {
        if (!isdigit(*strString, MainFrame::m_loc))
        {
            return FALSE;
        }
        strString++;
    }

    return TRUE;
}

BOOL IsNumeric(LPCSTR strString)
{
    return _IsNumeric(strString);
}

BOOL IsNumeric(LPCWSTR strString)
{
    return _IsNumeric(strString);
}

bool IncludeChinese(LPCTSTR pstrString)
{
    int i, n = lstrlen(pstrString);
    int c;
#if defined(UNICODE) || defined(_UNICODE)
    //UNICODE编码，汉字在0x4e00~0x9fbb（4.1）或0x4e00~0x9fc3（5.1）
    //Windows XP采用UTF16
    //0x3400~0x4db5
    //0x4e00~9fc3
    //0xf900~fa20
    //0xfa30~0xfa6a
    //0xfa70~0xfad9
    //0x20000~0x2a6d6
    //0x2d800~0x2fa1d
    for (i = 0; i < n; i++)
    {
        c = (int)(*pstrString);
        if ( (c >= 0x4e00) &&
             (c <= 0x9fc3) )
        {
            return true;
        }
        pstrString++;
    }
    return false;
#else
    //假定（也只支持）为GB2312编码,高位0XB0-0XF7，低位是0XA1-0XFE
    //DBCS编码,第一字节>=0xB0，第二字节>=0xa1
    n--;
    for (i = 0; i < n; i++)
    {
        c = pstrString[i];
        if (c >= 0xb0 &&
            c <= 0xf7)
        {
            c = pstrString[i+1];
            if (c >= 0xa1 &&
                c <= 0xfe)
            {
                return true;
            }
            //也认为是中文?
        }
    }
    return false;
#endif
}

//CHAR* myT2A(__out CHAR* lpszOutBuffer, __in DWORD dwCharSize, const TCHAR* pszOrigString)
//{
//#if defined(_UNICODE) || defined(UNICODE)
//    ::WideCharToMultiByte(CP_ACP, 0, pszOrigString, -1,
//        lpszOutBuffer, dwCharSize,
//        NULL, NULL);
//#else
//    StringCchCopy(lpszOutBuffer, dwCharSize, pszOrigString);
//#endif
//    return lpszOutBuffer;
//}

//TCHAR* myA2T(__out TCHAR* lpszOutBuffer, __in DWORD dwCharSize, __in const CHAR* pszOrigString)
//{
//#if defined(_UNICODE) || defined(UNICODE)
//    ::MultiByteToWideChar(CP_ACP, 0, pszOrigString, -1, lpszOutBuffer, dwCharSize);
//#else
//    StringCchCopy(lpszOutBuffer, dwCharSize, pszOrigString);
//#endif
//    return lpszOutBuffer;
//}

//WCHAR* myT2W(__out WCHAR* lpszOutBuffer, __in DWORD dwCharSize, const TCHAR* pszOrigString)
//{
//#if defined(_UNICODE) || defined(UNICODE)
//    StringCchCopy(lpszOutBuffer, dwCharSize, pszOrigString);
//#else
//    ::MultiByteToWideChar(CP_ACP, 0, pszOrigString, -1,
//        lpszOutBuffer, dwCharSize);
//#endif
//    return lpszOutBuffer;
//}

//TCHAR* myW2T(__out TCHAR* lpszOutBuffer, __in DWORD dwCharSize, __in const WCHAR* pszOrigString)
//{
//#if defined(_UNICODE) || defined(UNICODE)
//    StringCchCopy(lpszOutBuffer, dwCharSize, pszOrigString);
//#else
//    ::WideCharToMultiByte(CP_ACP, 0, pszOrigString, -1, lpszOutBuffer, dwCharSize, NULL, NULL);
//#endif
//    return lpszOutBuffer;
//}

int SQLRand()
{
    return (int)((double)rand() / (RAND_MAX + 1) * (1000 - 0) + 0);
}

CString GetColumnName(_RecordsetPtr& pPtr, long nCol)
{
    CString strValue = _T("");
    if (pPtr == NULL)
    {
        return strValue;
    }
    try
    {
        FieldsPtr fields = pPtr->GetFields();
        if (nCol >= fields->GetCount())
        {
            return strValue;
        }
        FieldPtr fd = fields->GetItem(nCol);
        strValue = (LPCTSTR)fd->GetName();
    }
    catch(_com_error &e)
    {
        UNREFERENCED_PARAMETER(e);
#ifdef _DEBUG
        MyAfxMessageBox(e.Description());
#endif
        strValue.Empty();
    }

    return strValue;
}

CString GetCollect(_RecordsetPtr& pPtr, long nItem)
{
    _variant_t val(nItem);
    return GetCollect(pPtr, val);

}

CString GetCollect(_RecordsetPtr& pPtr, LPCTSTR strName)
{
    _variant_t val(strName);
    return GetCollect(pPtr, val);
#if 0
    CString strVal = _T("");
    _variant_t val;

    if (pPtr == NULL)
    {
        return strVal;
    }
    try{
        val = pPtr->GetCollect(strName);
        strVal = (val.vt == VT_NULL) ? _T("") : CString(val);
    }
    catch(_com_error &e)
    {
        UNREFERENCED_PARAMETER(e);
#ifdef _DEBUG
        MyAfxMessageBox(e.Description());
#endif
        strVal.Empty();
    }

    return strVal;
#endif
}

CString GetCollect(_RecordsetPtr& pPtr, _variant_t vName)
{
    CString strVal = _T("");
    _variant_t val;

    if (pPtr == NULL)
    {
        return strVal;
    }
    try{
        val = pPtr->GetCollect(vName);
        strVal = (val.vt == VT_NULL) ? _T("") : CString(val);
    }
    catch(_com_error &e)
    {
        UNREFERENCED_PARAMETER(e);
#ifdef _DEBUG
        MyAfxMessageBox(e.Description());
#endif
        strVal.Empty();
    }

    return strVal;
}

int GetCollectCount(_RecordsetPtr& pPtr)
{
#if 0
    return pPtr->GetRecordCount();
#else
    int sum = 0;
    do 
    {
        if (pPtr == NULL)
        {
            break;
        }
        if(!pPtr->GetBOF())
            pPtr->MoveFirst();
        while(!pPtr->GetadoEOF())
        {
            sum++;
            pPtr->MoveNext();
        }
        pPtr->MoveFirst();
    } while (FALSE);

    return sum;
#endif
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }    
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

int GetSentenceTime(LPCTSTR lpstrSentenc)
{
    SplitResultVector sptTmp;
    SplitString(TString(lpstrSentenc), _T(" "), sptTmp);
    return (int)((sptTmp.size() / 2.0f + 2) * 10 * 1.5f);
}

void SpeakCheckText(__inout wstring& strText)
{
    //Hyphen,U+2010
    WCHAR wHyphen = 0x2010;
    wstring::size_type pos;
    while((pos = strText.find(wHyphen)) != wstring::npos)
    {
        strText.erase(pos, 1);
    }

    //en dash, U+2013
    wHyphen = 0x2013;
    while((pos = strText.find(wHyphen)) != wstring::npos)
    {
        strText.erase(pos, 1);
    }
    
    //em dash,U+2014
    wHyphen = 0x2014;
    while((pos = strText.find(wHyphen)) != wstring::npos)
    {
        strText.replace(pos, 1, L" ");
    }

    wHyphen = L'-';
    while((pos = strText.find(wHyphen)) != wstring::npos)
    {
        strText.erase(pos, 1);
    }
}

LPWSTR SpeakCheckText(__inout LPWSTR lpwstrText, __in DWORD dwBufCharsLen)
{
    wstring wTxt(lpwstrText);
    SpeakCheckText(wTxt);
    StringCchCopyW(lpwstrText, dwBufCharsLen, wTxt.c_str());
    return lpwstrText;
}


#ifdef _DEBUG

HANDLE g_hMutexTrace = NULL;

static
void _cdecl _myprintf(const TCHAR *pszFmt, ...)
{
    TCHAR buffer[4096];
    va_list ptr;
    va_start(ptr, pszFmt);
    StringCbVPrintf(buffer, _countof(buffer), pszFmt, ptr);
    va_end(ptr);
    OutputDebugString(buffer);
}

static
void _printhex(unsigned char* p, int len)
{
    int j;
    _myprintf(_T("%p    "), p);
    for (j = 0; j < len; j++)
    {
        _myprintf(_T("%02x "), p[j]);
    }
    _myprintf(_T("   "));
    for (j = 0; j < len; j++)
    {
        _myprintf(_T("%c"), isprint(p[j]) ? p[j] : _T('.'));
    }
    _myprintf(_T("\n"));
}

void dump_hexf(TCHAR* msg, const void* packet, int packet_len)
{
    if (NULL == g_hMutexTrace)
    {
        g_hMutexTrace = CreateMutex(NULL, FALSE, NULL);
    }
    WaitForSingleObject(g_hMutexTrace, INFINITE);
    //if (g_debug)
    {
        int i, j;
        int line = packet_len / 16, left_byte = packet_len % 16;
        unsigned char* p = (unsigned char*)packet;
        _myprintf(_T("%s, dump the buf(len=%d):\n"), msg, packet_len);
        for (i = 0; i < line; i++)
        {
            _printhex(&p[i*16], 16);
        }
        if (left_byte)
        {
            _printhex(&p[i*16], left_byte);
        }
    }
    ReleaseMutex(g_hMutexTrace);
}
#endif
#include "StdAfx.h"
#include "Utility.h"

CUtility::CUtility(void)
{
}

CUtility::~CUtility(void)
{
}

wstring CUtility::ANSIToUnicode(LPCSTR lpsContent)
{
    int len = 0;
    len = strlen(lpsContent);
    int unicodeLen = ::MultiByteToWideChar( CP_ACP,
        0,
        lpsContent,
        -1,
        NULL,
        0 ); 
    wchar_t * pUnicode; 
    pUnicode = new wchar_t[unicodeLen+1]; 
    memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
    ::MultiByteToWideChar( CP_ACP,
        0,
        lpsContent,
        -1,
        (LPWSTR)pUnicode,
        unicodeLen ); 
    wstring rt; 
    rt = ( wchar_t* )pUnicode;
    delete pUnicode; 
    return rt; 
}

wstring CUtility::ANSIToUnicode( const string& str )
{
    return ANSIToUnicode(str.c_str());
}


string CUtility::UnicodeToANSI(LPCWSTR lpwsContent)
{
    char*  pElementText;
    int iTextLen;
    // wide char to multi char
    iTextLen = WideCharToMultiByte( CP_ACP,
        0,
        lpwsContent,
        -1,
        NULL,
        0,
        NULL,
        NULL );
    pElementText = new char[iTextLen + 1];
    memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
    ::WideCharToMultiByte( CP_ACP,
        0,
        lpwsContent,
        -1,
        pElementText,
        iTextLen,
        NULL,
        NULL );
    string strText;
    strText = pElementText;
    delete[] pElementText;
    return strText;
}

string CUtility::UnicodeToANSI( const wstring& str )
{
   return UnicodeToANSI(str.c_str());
}
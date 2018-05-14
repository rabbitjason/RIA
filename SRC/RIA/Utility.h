#pragma once
//#include <windows.h>
#include <string>
using namespace std;

class CUtility
{
public:
    CUtility(void);
    ~CUtility(void);

    static wstring ANSIToUnicode( const string& str );
   
    static wstring ANSIToUnicode(LPCSTR lpsContent);

    static string UnicodeToANSI( const wstring& str );

    static string UnicodeToANSI(LPCWSTR lpwsContent);
};

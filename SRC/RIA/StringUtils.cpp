#include "stdafx.h" // comment if not using precompiled headers in MVC++
#include "StringUtils.h"

static LPCTSTR sg_HTML[][2] =
{
    {_T("&lt;"), _T("<")},
    {_T("&gt;"), _T(">")},
    {_T("&quot;"), _T("\"")},
    {_T("&amp;"), _T("&")},
    //{_T("&copy;"), _T("(C)")},
};

void HtmlDecode(__inout CString& strText)
{
    int i;
    for (i = 0; i < _countof(sg_HTML); i++)
    {
        strText.Replace(sg_HTML[i][0], sg_HTML[i][1]);
    }
    strText.Replace(_T("src=\"/upload"), _T("src=\"upload"));
}

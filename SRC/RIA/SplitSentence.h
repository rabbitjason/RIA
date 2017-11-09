#pragma once

typedef vector<CString>   SplitList;

class CSplitSentence
{
public:
    CSplitSentence(void);
    ~CSplitSentence(void);
    static CString GetSplitSentence(const CString& strInputSourceText);
};

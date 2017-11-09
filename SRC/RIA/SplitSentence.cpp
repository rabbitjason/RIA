#include "StdAfx.h"
#include "SplitSentence.h"
#include "CMciWave.h"
#include "WaveRecorder.h"
#include "main_frame.h"

CSplitSentence::CSplitSentence(void)
{
}

CSplitSentence::~CSplitSentence(void)
{
}

CString CSplitSentence::GetSplitSentence(const CString& strInputSourceText)
{
    SplitList rlist;
    CString strTmp;
    CString strSourceText = strInputSourceText;
    strSourceText.Trim();
    int i = 0, n_count;
    CString tChar, tPre, tNext;


    //Regex.Replace(box.Text, "(?s)\n\s*\n", _T('\n'))
    strSourceText.Remove(_T('\r'));
    TString ustr(strSourceText);
    TString::const_iterator pend = unique(ustr.begin(), ustr.end(), s_equal);
    strSourceText = ustr.substr(0, pend - ustr.begin()).c_str();
    ustr = strSourceText;
    pend = unique(ustr.begin(), ustr.end(), n_equal);
    strSourceText = ustr.substr(0, pend - ustr.begin()).c_str();

    ustr = strSourceText;
    CString strReturn = _T("");
    SplitResultVector sptSrc;
    SplitResultVector::const_iterator pspt;
    SplitString(ustr, _T("\n"), sptSrc);
    for (pspt = sptSrc.begin(); pspt != sptSrc.end(); ++pspt)
    {
        //TRACE(_T("split:%s\n"), pspt->c_str());
        strSourceText = pspt->c_str();
        if (strSourceText.IsEmpty())
        {
            continue;
        }
        strTmp.Empty();
        rlist.clear();
        n_count = strSourceText.GetLength();
        //DeletePunctationBlank，奇怪的算法，删除逗号等前面后面的空格？DeletePunctuationBlank?
        for (i = 0; i < n_count; i++)
        {
            tChar = strSourceText[i];
            if (tChar != _T(" "))
            {
                strTmp += tChar;
            }
            else
            {
                if (i == 0)
                {
                    tPre.Empty();
                }
                else
                {
                    tPre = strSourceText[i-1];
                }
                if (i == n_count-1)
                {
                    tNext.Empty();
                }
                else
                {
                    tNext = strSourceText[i+1];
                }

                if ( (isalnum(tPre[0], MainFrame::m_loc) && isalnum(tNext[0], MainFrame::m_loc)) ||
                    ( ((tPre == _T("'")) || (tPre == _T("\"")) && isalnum(tNext[0], MainFrame::m_loc)) ||
                    ((tNext == _T("'")) || (tNext == _T("\"")) && isalnum(tPre[0], MainFrame::m_loc))
                    ) )
                {
                    strTmp += tChar;
                }
            }
        }

        //TRACE(_T("after DeletePunctationBlank:*****\n%s\n**********\n"), strTmp);

        static CString OneSplitPunct[] = { _T(":'"), _T(".'"), _T(",'"), _T("!'"), _T("?'"), _T(":\""), _T(".\""), _T(",\""), _T("!\""), _T("?\"") };

        SplitList onesplit;
        n_count = strTmp.GetLength();
        int j, num2=0;
        for (i = 0; i < n_count; i++)
        {
            if (i == n_count - 1)
            {
                onesplit.push_back(strTmp.Mid(num2, i+1-num2));
                break;
            }
            tChar = strTmp.Mid(i, 2);
            for (j = 0; j < _countof(OneSplitPunct); j++)
            {
                if (OneSplitPunct[j] == tChar)
                {
                    onesplit.push_back(strTmp.Mid(num2, i+2-num2));
                    num2 = i + 2;
                    break;
                }
            }
        }
        SplitList::iterator p;
        SplitList::iterator p2;

        //#ifdef _DEBUG
        //    TRACE(_T("after onesplit, number:%d\n"), onesplit.size());
        //    for (p = onesplit.begin(); p != onesplit.end(); ++p)
        //    {
        //        TRACE(_T("*****\n%s\n*****\n"), *p);
        //    }
        //    TRACE(_T("end onesplit\n"));
        //#endif

        static CString TwoSplitPunct[] = { _T(";"), _T("."), _T("!"), _T("?") };

        SplitList twosplit, list2;

        for (p = onesplit.begin(); p != onesplit.end(); ++p)
        {
            n_count = p->GetLength();
            twosplit.clear();
            num2 = 0;
            for (i = 0; i < n_count; i++)
            {
                if (i == n_count - 1)
                {
                    twosplit.push_back(p->Mid(num2, i + 1 - num2));
                    break;
                }
                tChar = p->Mid(i, 1);
                tNext = p->Mid(i+1, 1);
                if (i > 0)
                {
                    tPre = p->Mid(i-1, 1);
                }
                else
                {
                    tPre.Empty();
                }
                for (j = 0; j < _countof(TwoSplitPunct); j++)
                {
                    if ((TwoSplitPunct[j] == tChar) &&
                        (tNext != _T("'")) &&
                        (tNext != _T("\"")) &&
                        (tNext != _T(".")) &&
                        ( (!tPre.IsEmpty() && !IsNumeric(tPre)) || !IsNumeric(tNext) ) )
                    {
                        twosplit.push_back(p->Mid(num2, i+1-num2));
                        num2 = i + 1;
                        break;
                    }
                }
            }

            for (p2 = twosplit.begin(); p2 != twosplit.end(); ++p2)
            {
                list2.push_back(*p2);
            }
        }

        int num3 = 0;
        for (p = list2.begin(); p != list2.end(); ++p)
        {
            twosplit.clear();
            n_count = p->GetLength();
            num2 = 0;
            num3 = 0;
            for (i = 0; i < n_count; i++)
            {
                if (i == n_count - 1)
                {
                    twosplit.push_back(p->Mid(num3, i + 1 - num3));
                    break;
                }
                tChar = p->Mid(i, 1);
                tNext = p->Mid(i+1, 1);
                if (tChar == _T(" "))
                {
                    num2++;
                }
                if ((tChar == _T(",")) &&
                    (tNext != _T("'")) &&
                    (tNext != _T("\"")) &&
                    (num2 >= 4) )
                {
                    twosplit.push_back(p->Mid(num3, i + 1 - num3));
                    num3 = i + 1;
                }
            }

            for (p2 = twosplit.begin(); p2 != twosplit.end(); ++p2)
            {
                rlist.push_back(*p2);
            }
        }

        for (p = rlist.begin(); p != rlist.end(); ++p)
        {
            //p->Replace(_T("\n"), _T(" "));
            //p->Replace(_T("\r\n"), _T(""));
            //p->Replace(_T("\r"), _T(""));
            p->Trim();
            if (!p->IsEmpty())
            {
                strReturn += *p;
                strReturn += _T("|");
            }
        }
        //TRACE(_T("return:%s\n"), strReturn);
    }
    //strReturn.Replace(_T("\n"), _T(""));
    //strReturn.Replace(_T("\r\n"), _T(""));
    //strReturn.Replace(_T("\r"), _T(""));
    strReturn.Trim();
    strReturn.TrimRight(_T('|'));

    //TRACE(_T("return:%s\n"), strReturn);
    return strReturn;
}

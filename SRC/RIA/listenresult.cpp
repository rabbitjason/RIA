//
// main_frame.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "stdafx.h"
#include "reader_setup.h"
#include "readingbarsetup.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "listenresult.h"
#include "dictionary.h"
#include "main_frame.h"

CListenResult::CListenResult(MainFrame* pParent) : m_pParent(pParent)
{}

CListenResult::~CListenResult()
{
	//PostQuitMessage(0);
}

LPCTSTR CListenResult::GetWindowClassName() const
{
	return _T("RIAListenResultWindow");
}

CControlUI* CListenResult::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CListenResult::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CListenResult::GetSkinFile()
{
	return _T("listenresult.xml");
}

CDuiString CListenResult::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CListenResult::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CListenResult::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//#if defined(WIN32) && !defined(UNDER_CE)
//    if( wParam == SC_CLOSE ) {
//        ::PostQuitMessage(0L);
//        bHandled = TRUE;
//        return 0;
//    }
//	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//#else
//	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
//#endif
//
//	return 0;
//}

LRESULT CListenResult::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CListenResult::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CListenResult::OnTimer(TNotifyUI& msg)
{
}

void CListenResult::OnExit(TNotifyUI& msg)
{
	Close();
}

void CListenResult::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CListenResult::OnPrepare(TNotifyUI& msg)
{
    CLabelUI* pLbl;
    CString strTmp;

    strTmp.Format(_T("%d"), m_nNumber);
    pLbl = xFindControl(CLabelUI, _T("listenresulalltxt"));
    pLbl->SetText(strTmp);

    strTmp.Format(_T("%d"), m_nTrueNumber);
    pLbl = xFindControl(CLabelUI, _T("listenresultruetxt"));
    pLbl->SetText(strTmp);

    strTmp.Empty();
    float frate = 0.0f;
    if (m_nNumber > 0)
    {
        m_pParent->StudyLog(STUDY_TYPE_LISTEN, LOG_TYPE_ACHIEV, m_nNumber, m_nTrueNumber);
        frate = (float)m_nTrueNumber * 100 / (float)m_nNumber;
    }
    strTmp.Format(_T("%d%%"), (int)frate);
    pLbl = xFindControl(CLabelUI, _T("listenresulratetxt"));
    pLbl->SetText(strTmp);

    CControlUI* pCtrl = xFindControl(CControlUI, _T("listenresultresult"));
    strTmp = (frate >= 60.0f) ? _T("Image/AnswerRight.png") : _T("Image/AnswerWrong.png");
    pCtrl->SetBkImage(strTmp);

    m_strQuestion.TrimRight(_T(";"));
    SplitString(TString(m_strQuestion), _T(";"), m_sptQuestion);

    CListUI* pList = xFindControl(CListUI, _T("listenresultlist"));
    pList->RemoveAll();
    LPCTSTR strAttrTrue = _T("textpadding=\"16,0,0,0\" align=\"left\" font=\"7\" normalimage=\"file='Image/icoTrue.png' dest='0,0,16,16'\"");
    LPCTSTR strAttrFalse = _T("textpadding=\"16,0,0,0\" align=\"left\" font=\"7\" normalimage=\"file='Image/icoFalse.png' dest='0,0,16,16'\"");

    m_strReviewQuestion.Empty();
    SplitResultVector::const_iterator p;
    CListContainerElementUI* pItem;
    CButtonUI* pBtn;
    int idx = 1;
    for (p = m_sptQuestion.begin(); p != m_sptQuestion.end(); ++p)
    {
        if (p->empty())
        {
            continue;
        }
        TString::size_type pos = p->find(_T(','));
        strTmp = p->substr(0, pos).c_str();
        m_strReviewQuestion += strTmp;
        m_strReviewQuestion += _T(",");

        strTmp.Format(_T("  第%d题"),
            idx);
        pItem = new CListContainerElementUI;
        pItem->SetAttribute(_T("height"), _T("16"));
        pBtn = new CButtonUI;
        if (p->substr(p->length() - 1, 1) == _T("0"))
        {
            pBtn->ApplyAttributeList(strAttrFalse);
        }
        else
        {
            pBtn->ApplyAttributeList(strAttrTrue);
        }
        pBtn->SetText(strTmp);
        pItem->Add(pBtn);

        pList->Add(pItem);

        idx++;
    }
}

void CListenResult::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    //CSliderUI* pSlider;
    //CTextUI* pTxt;
    CString strTmp;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if ((strSender == kCloseButtonName) ||
            (strSender == kReturnButtonName) ||
            (strSender == kSubReturnButtonName) ||
            (strSender == _T("readbarcancelbtn")) )
		{
            OnExit(msg);
            //IDCANCEL是表明复习
            //Close(IDCANCEL);
		}
		else if (strSender == kMinButtonName)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
        else if (strSender == kDictButtonName)
        {
            CDictionary *pdlgdic = new CDictionary(m_pParent);
            m_pParent->StopSpeak();
            m_pParent->OpenModalWnd(pdlgdic, g_SkinInfo[sDictSkin].title);
        }
        else if (_T("listenresultclearwrongbtn") == strSender)
        {
            m_pParent->ExecuteSql(_T("Delete from 听力错题"));
            MyAfxMessageBox(_T("操作成功。"));
        }
        else if (_T("listenresultaddwrongbtn") == strSender)
        {
            SplitResultVector sptWrong;
            SplitResultVector::const_iterator p;
            int num2 = 0, numall = 0;
            for (p = m_sptQuestion.begin(); p != m_sptQuestion.end(); ++p)
            {
                if (p->empty())
                {
                    continue;
                }
                SplitString(*p, _T(","), sptWrong);
                //修改为检查是否已经在错题库中，不存在才添加
                //if ( (sptWrong[1] == _T("0")) &&
                //     m_pParent->GetSingleField(_T("Select Id from 听力错题 where 习题=%s"),
                //        sptWrong[0].c_str()).IsEmpty() &&
                //     m_pParent->ExecuteSql(_T("Insert into 听力错题(习题) Values(%s)"),
                //        sptWrong[0].c_str()) )
                //{
                //    num2++;
                //}
                if (sptWrong[1] == _T("0"))
                {
                    numall++;
                    strTmp = m_pParent->GetSingleField(_T("Select Id from 听力错题 where 习题=%s"),
                        sptWrong[0].c_str());
                    if (strTmp.IsEmpty())
                    {
                        m_pParent->ExecuteSql(_T("Insert into 听力错题(习题) Values(%s)"),
                            sptWrong[0].c_str());
                        num2++;
                    }
                }
            }

            if (numall > 0)
            {
                if (num2 == numall)
                {
                    MyAfxMessageBox(_T("共添加了%d个错题到错题库中。"),
                        num2);
                }
                else if (num2 == 0)
                {
                    MyAfxMessageBox(_T("错题在错题库中已经存在。"));
                }
                else
                {
                    MyAfxMessageBox(_T("共添加了%d个错题到错题库中，%d个错题在错题库中已经存在未添加。"),
                        num2, numall - num2);
                }
            }
        }
        else if ( (_T("listenresultagainbtn") == strSender) ||
            (kSetButtonName == strSender) )
        {
            Close(IDRETRY);
        }
        else if (_T("listenresultreviewbtn") == strSender)
        {
            m_pParent->m_strListenReviewQuestion = m_strReviewQuestion;
            Close(IDCANCEL);
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CListenResult::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CListenResult::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

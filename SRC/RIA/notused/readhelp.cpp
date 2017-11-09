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
#include "readhelp.h"
#include "main_frame.h"

CWordHelp::CWordHelp()
{}

CWordHelp::~CWordHelp()
{
	//PostQuitMessage(0);
}

#if 0
LPCTSTR CWordHelp::GetWindowClassName() const
{
	return _T("RIAReadWordHelpWindow");
}

CControlUI* CWordHelp::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordHelp::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordHelp::GetSkinFile()
{
	return _T("readhlp.xml");
}

CDuiString CWordHelp::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE CWordHelp::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LRESULT CWordHelp::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
    if( wParam == SC_CLOSE ) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT CWordHelp::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordHelp::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordHelp::OnTimer(TNotifyUI& msg)
{
}

void CWordHelp::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordHelp::InitWindow()
{
    ASSERT(m_pFrame);
}

//各窗口的初始化在此完成
void CWordHelp::OnPrepare(TNotifyUI& msg)
{
    //::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW |SWP_NOSIZE);
}
#endif

void CWordHelp::HelpNotify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		//OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if ((strSender == kCloseButtonName) ||
            (strSender == kReturnButtonName) ||
            (strSender == kSubReturnButtonName) ||
            (strSender == _T("readbarcancelbtn")) )
		{
            //OnExit(msg);
		}
		else if (strSender == kMinButtonName)
		{
//#if defined(UNDER_CE)
//			::ShowWindow(m_hWnd, SW_MINIMIZE);
//#else
//			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
//#endif
		}
        else if (strSender == _T("readhlpup"))
        {
            CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));
            if (pList->GetCount() >= 10)
            {
                //只会<= 10
                pList->SelectItem(0);
                strTmp = pList->GetItemAt(0)->GetUserData();
                _RecordsetPtr pRecdPtr = m_pFrame->GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%' and words<'%s'"),
                    m_strKeyWord, strTmp);
                if (pRecdPtr != NULL)
                {
                    _UpdateList(pRecdPtr);
                }
            }
        }
        else if (strSender == _T("readhlpdown"))
        {
            CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));
            if (pList->GetCount() >= 10)
            {
                //只会<= 10
                pList->SelectItem(9);
                strTmp = pList->GetItemAt(9)->GetUserData();
                _RecordsetPtr pRecdPtr = m_pFrame->GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%' and words>'%s'"),
                    m_strKeyWord, strTmp);
                if (pRecdPtr != NULL)
                {
                    _UpdateList(pRecdPtr);
                }
            }
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
		//OnTimer(msg);
	}

    return;
}

#if 0
LRESULT CWordHelp::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordHelp::GetResourceID() const
{
    return MAKEINTRESOURCE(IDR_ZIPRES);
}
#endif

void CWordHelp::_UpdateList(_RecordsetPtr& pRecdPtr)
{
    CString strTmp;
    CListLabelElementUI* pItem;
    CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));
    pList->RemoveAll();

    if (pRecdPtr == NULL)
    {
        return;
    }

    LPCTSTR strAttr = _T("height=\"14\" width=\"9999\"");
    int idx = 1;
    while(!pRecdPtr->GetadoEOF())
    {
        pItem = new CListLabelElementUI;
        //指定高度和宽度，能够提高效率
        pItem->ApplyAttributeList(strAttr);
        pItem->SetUserData(GetCollect(pRecdPtr, (long)0));
        strTmp.Format(_T("%d.%s"), idx % 10, GetCollect(pRecdPtr, (long)0));
        pItem->SetText(strTmp);
        pList->Add(pItem);

        pRecdPtr->MoveNext();

        idx++;
    }
}

BOOL CWordHelp::UpdateList(CString& strKeyWord)
{
    BOOL bFound = FALSE;
    m_strKeyWord = strKeyWord;
    
    do 
    {
        m_strKeyWord.Replace(_T("'"), _T("''"));
        _RecordsetPtr pRecdPtr = m_pFrame->GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%'"),
            m_strKeyWord);
        
        if (NULL == pRecdPtr)
        {
            break;
        }

        _UpdateList(pRecdPtr);

        bFound = TRUE;
    } while (FALSE);
    
    return bFound;
}

void CWordHelp::Relayout(int cx, int cy, int nWidth, int nHeight)
{
    CRect rct, newRct;
    //::GetWindowRect(GetHWND(), &rct);
    rct = GetPos();

    if (nWidth - cx > rct.Width())
    {
        newRct.left = cx + 5;
    }
    else
    {
        newRct.left = cx - rct.Width() - 15;
    }
    newRct.right = newRct.left + rct.Width();

    if (nHeight + 0x95 - cy > rct.Height())
    {
        newRct.top = cy - 20;
    }
    else
    {
        newRct.top = cy - rct.Height() + 20;
    }
    newRct.bottom = newRct.top + rct.Height();

    CString strTmp;
    strTmp.Format(_T("padding=\"%d,%d,0,0\""), newRct.left, newRct.top);
    ApplyAttributeList(strTmp);
    //::MoveWindow(GetHWND(), newRct.left, newRct.top, newRct.Width(), newRct.Height(), TRUE);
}

int CWordHelp::GetHelpCount() const
{
    CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));
    return pList->GetCount();
}

void CWordHelp::SelectItem(int nIndx)
{
    CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));

    if (nIndx < pList->GetCount() &&
        nIndx >= 0)
    {
        pList->SelectItem(nIndx);
    }
}

CString CWordHelp::GetHelpString() const
{
    CString strTmp;
    CListUI* pList = xxFindControl(CListUI, _T("readhlplistlist"));

    if (pList->GetCount() > 0)
    {
        strTmp = pList->GetItemAt(pList->GetCurSel())->GetUserData();
    }
    return strTmp;
}

void CWordHelp::SetMainFrame(MainFrame* pParent)
{
    m_pFrame = pParent;
    m_pPaintManager_ = &m_pFrame->GetPaintManager();
}

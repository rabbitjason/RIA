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
#include "talkcount.h"
#include "main_frame.h"

CTalkCount::CTalkCount(MainFrame* pParent) : m_pParent(pParent)
{}

CTalkCount::~CTalkCount()
{
	//PostQuitMessage(0);
}

LPCTSTR CTalkCount::GetWindowClassName() const
{
	return _T("RIATalkCountWindow");
}

CControlUI* CTalkCount::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CTalkCount::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CTalkCount::GetSkinFile()
{
	return _T("readstat.xml");
}

CDuiString CTalkCount::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CTalkCount::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CTalkCount::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CTalkCount::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CTalkCount::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CTalkCount::OnTimer(TNotifyUI& msg)
{
}

void CTalkCount::OnExit(TNotifyUI& msg)
{
	Close();
}

void CTalkCount::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CTalkCount::OnPrepare(TNotifyUI& msg)
{
    TRACE(_T("unkown word:%s, unknown symbol:%s\n"), m_strWord, m_strSymbol);
    m_strWord.Replace(_T("\""), _T(""));
    m_strWord.Replace(_T("'"), _T(""));
    static LPCTSTR strTrimChars = _T("\r\n ,;");
    m_strWord.Trim(strTrimChars);
    m_strSymbol.Trim(strTrimChars);

    SplitString(TString(m_strWord), _T(" ,"), m_sptWord, false);
    SplitString(TString(m_strSymbol), _T(" ,"), m_sptSymbol, false);

    //CListUI* pList = xFindControl(CListUI, _T("talkcountwordlist"));
    //CListTextElementUI* pItem;

    //pItem = new CListTextElementUI;
    //pItem->ApplyAttributeList(_T("height=\"26\" width=\"9999\""));
    //pList->Add(pItem);

    //pItem->SetText(0, _T("1"));
    //pItem->SetText(1, _T("test"));
    //pItem->SetText(2, _T("2"));

    //pList = xFindControl(CListUI, _T("talkcountphonemelist"));
    //pItem = new CListTextElementUI;
    //pItem->ApplyAttributeList(_T("height=\"26\" width=\"9999\""));
    //pList->Add(pItem);

    //pItem->SetText(0, _T("1"));
    //pItem->SetText(1, _T("testsymbolB:"));
    //pItem->SetText(2, _T("23"));

    InitCtrl(false);
    InitCtrl(true);
}

void CTalkCount::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();

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
            //OnExit(msg);
            Close(IDCANCEL);
		}
		else if (strSender == kMinButtonName)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
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

LRESULT CTalkCount::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CTalkCount::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CTalkCount::InitCtrl(bool bSymbol)
{
    CListUI* pList = xFindControl(CListUI, bSymbol ? _T("talkcountphonemelist") : _T("talkcountwordlist"));
    if (pList == NULL)
    {
        return;
    }
    CListTextElementUI* pItem;
    CString strTmp;
    SplitResultVector& spt = bSymbol ? m_sptSymbol : m_sptWord;

    LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");

    bool bFlag;
    //num，当前单词;num4，序号
    int num = 0, num3, num4 = 1;
    int num5 = spt.size();
    //TRACE(_T("num5:%d\n"), num5);
    for (num = 0; num < num5; num++)
    {
        //TRACE(_T("spt[%d]:%s\n"), num, spt[num].c_str());
        int num2 = 1;
        int num6 = num - 1;
        bFlag = false;
        num3 = 0;
        while(num3 <= num6)
        {
            if (spt[num3] == spt[num])
            {
                bFlag = true;
                break;
            }
            num3++;
        }

        //如果前面有重复的，跳过
        if (bFlag)
        {
            continue;
        }

        //检查后面是否有重复的，次数加1
        int num7 = (int)spt.size();
        num3 = num + 1;
        while(num3 < num7)
        {
            if (spt[num3] == spt[num])
            {
                num2++;
            }
            num3++;
        }

        if (bSymbol)
        {
            static LPCTSTR strLblAttr = _T("height=\"26\" width=\"114\" align=\"center\"");

            CListContainerElementUI* pCon = new CListContainerElementUI;
            pCon->ApplyAttributeList(strAttr);
            
            CLabelUI* pLbl = new CLabelUI;
            pLbl->ApplyAttributeList(strLblAttr);
            pLbl->SetFont(1);
            strTmp.Format(_T("%d"), num4);
            pLbl->SetText(strTmp);
            pCon->Add(pLbl);

            pLbl = new CLabelUI;
            pLbl->ApplyAttributeList(strLblAttr);
            pLbl->SetFont(3);
            pLbl->SetText(spt[num].c_str());
            pCon->Add(pLbl);

            pLbl = new CLabelUI;
            pLbl->ApplyAttributeList(strLblAttr);
            pLbl->SetFont(1);
            strTmp.Format(_T("%d"), num2);
            pLbl->SetText(strTmp);
            pCon->Add(pLbl);

            pList->Add(pCon);
        }
        else
        {
            pItem = new CListTextElementUI;
            pItem->ApplyAttributeList(strAttr);
            pList->Add(pItem);

            strTmp.Format(_T("%d"), num4);
            pItem->SetText(0, strTmp);
            pItem->SetText(1, spt[num].c_str());
            strTmp.Format(_T("%d"), num2);
            pItem->SetText(2, strTmp);
        }

        num4++;
    }
}

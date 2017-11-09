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
#include "wordaddnew.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "wordmgtnew.h"
#include "main_frame.h"

CWordMgtnew::CWordMgtnew(MainFrame* pParent) : m_pParent(pParent), m_nBook(0)
{}

CWordMgtnew::~CWordMgtnew()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordMgtnew::GetWindowClassName() const
{
	return _T("RIAWordMgtnewWindow");
}

CControlUI* CWordMgtnew::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordMgtnew::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordMgtnew::GetSkinFile()
{
	return _T("wordmngnew.xml");
}

CDuiString CWordMgtnew::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordMgtnew::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordMgtnew::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordMgtnew::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordMgtnew::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordMgtnew::OnTimer(TNotifyUI& msg)
{
}

void CWordMgtnew::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordMgtnew::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CWordMgtnew::OnPrepare(TNotifyUI& msg)
{
    InitControls(msg);
}

void CWordMgtnew::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CListUI* pList;
    //CEditUI* pEdt;
    //CButtonUI* pBtn;
    CListContainerElementUI* pItem;
    CCheckBoxUI* pChk;
    int i, n;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if (strSender == kCloseButtonName)
		{
            //OnExit(msg);
            Close(IDCANCEL);
		}
        else if (strSender == _T("wordmgallbtn"))
        {
            pList = xFindControl(CListUI, _T("wordmanagelist"));
            n = pList->GetCount();
            int x = pList->GetCurSel();
            for (i = 0; i < n; i++)
            {
                pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
                pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                pChk->SetCheck(true);
            }
            //pChk->SetCheck(true);导致新的item处于selected状态，还原
            if (n > 0)
            {
                pList->SelectItem(x);
            }
        }
        else if (strSender == _T("wordmgdelbtn"))
        {
            pList = xFindControl(CListUI, _T("wordmanagelist"));
            n = pList->GetCount();
            strTmp.Empty();
            CString strTmp2;
            for (i = 0; i < n; i++)
            {
                pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
                pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                if (pChk->IsSelected())
                {
                    strTmp2 = pChk->GetText().GetData();
                    strTmp2.Replace(_T("'"), _T("''"));
                    strTmp += _T("'") + strTmp2 + _T("',");
                }
            }
            //去掉末尾的逗号
            strTmp.TrimRight(_T(','));
            if (strTmp.IsEmpty())
            {
                MyAfxMessageBox(_T("没有当前选定项。"));
                return;
            }
            m_pParent->ExecuteSql(_T("Delete from 生词 where 生词 in (Select Id from 单词 where 单词 in (%s)) and 生词本=%d"),
                strTmp, m_nBook);
            InitControls(msg);
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        pList = xFindControl(CListUI, _T("wordmanagelist"));
        pChk = static_cast<CCheckBoxUI*>(msg.pSender);
        //发送消息的是CheckBox，ChekBox被ListContainerElement包含
        i = pList->GetItemIndex(pChk->GetParent());
        //TRACE(_T("sender:%p, parent:%p, item index:%d\n"), pItem, pItem->GetParent(), i);
        //应该是List接收不到Click，但是CheckBox接收到，并发送selectchanged消息
        pList->SelectItem(i);
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CWordMgtnew::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordMgtnew::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CWordMgtnew::InitControls(TNotifyUI& msg)
{
    CListUI* pList = xFindControl(CListUI, _T("wordmanagelist"));
    CListContainerElementUI* pItem;
    CCheckBoxUI* pChk;
    CString strTmp;

    _RecordsetPtr pSetPtr = m_pParent->GetTablesBySql(_T("Select * from View_生词 where 生词本=%d"),
        m_nBook);
    pList->RemoveAll();
    LPCTSTR strAttr = _T("textpadding=\"23,3,0,0\" align=\"left\" font=\"1\" normalimage=\"file='Image/checkbox_unchk.png' dest='0,0,23,23'\" selectedimage=\"file='Image/checkbox_chk.png' dest='0,0,23,23'\"");
    if (pSetPtr != NULL)
    {
        while(!pSetPtr->GetadoEOF())
        {
            pItem = new CListContainerElementUI;
            pItem->SetAttribute(_T("height"), _T("23"));
            pChk = new CCheckBoxUI;
            pChk->ApplyAttributeList(strAttr);
            strTmp = GetCollect(pSetPtr, _T("单词"));
            pChk->SetText(strTmp);
            pItem->Add(pChk);
            //TRACE(_T("add item:%p,%p\n"), pItem, pChk);

            pList->Add(pItem);

            pSetPtr->MoveNext();
        }
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(0);
    }
}

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
#include "dialoglist.h"
#include "main_frame.h"

CDialogList::CDialogList(MainFrame* pParent) : m_pParent(pParent), m_nCurrentIdx(0)
{}

CDialogList::~CDialogList()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogList::GetWindowClassName() const
{
	return _T("RIADialogListWindow");
}

CControlUI* CDialogList::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogList::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogList::GetSkinFile()
{
	return _T("dialoglist.xml");
}

CDuiString CDialogList::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogList::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogList::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogList::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogList::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogList::OnTimer(TNotifyUI& msg)
{
}

void CDialogList::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogList::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CDialogList::OnPrepare(TNotifyUI& msg)
{
    CString strTmp;
    if (m_pParent->m_strDialogGroup == _T("0"))
    {
        strTmp.Format(_T("Select * from %s where 场景=%s Order by 顺序"),
            s_strDlgSqlCotents[m_pParent->m_nDialogSelectPageId - 1], m_pParent->m_strDialogScene);
    }
    else
    {
        strTmp.Format(_T("Select * from %s where 分组=%s and (内容 IS NOT NULL) and (内容 <> '') Order by 顺序"),
            s_strDlgSqlCotents[m_pParent->m_nDialogSelectPageId - 1], m_pParent->m_strDialogGroup);
    }

    if (_tstoi(m_pParent->m_strDialogScene) > 0)
    {
        _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), strTmp);
        if (pRecdPtr)
        {
            CListUI* pList = xFindControl(CListUI, _T("dlglstlist"));
            pList->RemoveAll();

            CListLabelElementUI* pItem;
            while(!pRecdPtr->GetadoEOF())
            {
                pItem = new CListLabelElementUI;
                //指定高度和宽度，能够提高效率
                LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");
                pItem->ApplyAttributeList(strAttr);
                strTmp = GetCollect(pRecdPtr, _T("Id"));
                pItem->SetUserData(strTmp);
                strTmp = GetCollect(pRecdPtr, _T("内容"));
                pItem->SetText(strTmp);
                pList->Add(pItem);

                pRecdPtr->MoveNext();
            }

            if (m_nCurrentIdx >= pList->GetCount())
            {
                m_nCurrentIdx = 0;
            }
            pList->SelectItem(m_nCurrentIdx);
            //调用EnsureVisible前，设置下Pos，否则子项的Pos现在都为0
            pList->SetPos(pList->GetPos());
            pList->EnsureVisible(m_nCurrentIdx);
            TRACE(_T("all sentence:%d\n"), pList->GetCount());
        }
    }
}

void CDialogList::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    //CSliderUI* pSlider;
    //CTextUI* pTxt;
    //CString strTmp;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if ((strSender == kCloseButtonName) ||
            /*(strSender == kReturnButtonName) ||*/
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
        else if (strSender == kReturnButtonName)
        {
            CListUI* pList = xFindControl(CListUI, _T("dlglstlist"));
            if (pList->GetCount() > 0 &&
                pList->GetCurSel() >= 0)
            {
                int i = pList->GetCurSel();
                if (m_pParent->m_nDialogSpokenIndex > i)
                {
                    m_pParent->m_nDialogSpokenIndex = 0;
                    m_pParent->m_pDialogSpokenPtr->MoveFirst();
                }
                while (m_pParent->m_nDialogSpokenIndex < i &&
                    !m_pParent->m_pDialogSpokenPtr->GetadoEOF())
                {
                    m_pParent->m_nDialogSpokenIndex++;
                    m_pParent->m_pDialogSpokenPtr->MoveNext();
                }
#if 0

                //暂不支持MovePrevious
                try
                {
                    while(m_pParent->m_nDialogSpokenIndex > i &&
                        !m_pParent->m_pDialogSpokenPtr->GetBOF())
                    {
                        m_pParent->m_nDialogSpokenIndex--;
                        m_pParent->m_pDialogSpokenPtr->MovePrevious();
                    }

                }
                catch (_com_error e)
                {
                    TRACE("error her:%s, %d\n", __FUNCTION__, __LINE__);
                }
#endif
            }
            OnExit(msg);
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

LRESULT CDialogList::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDialogList::GetResourceID() const
{
    return MAKEINTRESOURCE(IDR_ZIPRES);
}

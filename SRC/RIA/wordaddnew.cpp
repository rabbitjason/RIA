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
#include "wordmgtnew.h"
#include "wordaddnew.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"

CWordAddnew::CWordAddnew(MainFrame* pParent) : m_pParent(pParent), m_nNewWord(0)
{}

CWordAddnew::~CWordAddnew()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordAddnew::GetWindowClassName() const
{
	return _T("RIAWordAddnewWindow");
}

CControlUI* CWordAddnew::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordAddnew::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordAddnew::GetSkinFile()
{
	return _T("wordaddnew.xml");
}

CDuiString CWordAddnew::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordAddnew::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordAddnew::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordAddnew::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordAddnew::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordAddnew::OnTimer(TNotifyUI& msg)
{
}

void CWordAddnew::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordAddnew::InitWindow()
{
    ASSERT(m_pParent);
}

//�����ڵĳ�ʼ���ڴ����
void CWordAddnew::OnPrepare(TNotifyUI& msg)
{
    InitControls(msg);
}

void CWordAddnew::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CListUI* pList;
    CEditUI* pEdt;
    CButtonUI* pBtn;
    int i;

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
        else if (strSender == _T("wordaddnewcancelbookbtn"))
        {
            CControlUI* pUI = xFindControl(CControlUI, _T("hlnewbook"));
            pUI->SetVisible(false);
        }
        else if (strSender == _T("wordaddnewnewbookbtn"))
        {
            CControlUI* pUI = xFindControl(CControlUI, _T("hlnewbook"));
            if (!pUI->IsVisible())
            {
                pUI->SetVisible(true);
                pEdt = xFindControl(CEditUI, _T("wordaddnewbookedt"));
                pEdt->SetFocus();
            }
        }
        else if (strSender == _T("wordaddnewaddbookbtn"))
        {
            pEdt = xFindControl(CEditUI, _T("wordaddnewbookedt"));
            CString strBook = pEdt->GetText().GetData();
            if (strBook.GetLength() <= 0)
            {
                MyAfxMessageBox(_T("����д�����ʱ������ơ�"));
                pEdt->SetFocus();
                return;
            }
            strBook.Replace(_T("'"), _T("''"));
            m_pParent->ExecuteSql(_T("Insert into ���ʱ�(����) Values('%s')"),
                strBook);
            InitControls(msg);
            pList = xFindControl(CListUI, _T("wordaddnewlist"));
            pList->SelectItem(pList->GetCount() - 1);
            m_pParent->SaveSetup(_T("word_nwb"), pList->GetItemAt(pList->GetCount() - 1)->GetTag());

            pBtn = xFindControl(CButtonUI, _T("wordaddnewcancelbookbtn"));
            m_PaintManager.SendNotify(pBtn, DUI_MSGTYPE_CLICK);
        }
        else if (strSender == _T("wordaddnewdelbookbtn"))
        {
            pList = xFindControl(CListUI, _T("wordaddnewlist"));
            if (pList->GetCount() <= 0)
            {
                return;
            }
            i = pList->GetItemAt(pList->GetCurSel())->GetTag();
            strTmp = m_pParent->GetSingleField(_T("Select count(Id) from ���� where ���ʱ�=%d"),
                i);
            if (strTmp != _T("0"))
            {
                if (AfxMessageBox(_T("ȷ��ɾ�����ʱ��Լ������ʱ��е�������"),
                        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
                {
                    return;
                }
                m_pParent->ExecuteSql(_T("Delete from ���� where ���ʱ�=%d"),
                    i);
            }
            m_pParent->ExecuteSql(_T("Delete from ���ʱ� where Id=%d"),
                i);
            InitControls(msg);
        }
        else if (strSender == _T("wordaddnewmgbtn"))
        {
            //m_pParent->OpenModalWnd(new CWordMgtnew(m_pParent), g_SkinInfo[sWordManageNewSkin].title);
            pList = xFindControl(CListUI, _T("wordaddnewlist"));
            if (pList->GetCount() > 0)
            {
                CWordMgtnew* pWnd = new CWordMgtnew(m_pParent);
                if (pWnd)
                {
                    HWND hWnd = pWnd->Create(GetHWND(), g_SkinInfo[sWordManageNewSkin].title, UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW);
                    if (hWnd)
                    {
                        pWnd->m_nBook = pList->GetItemAt(pList->GetCurSel())->GetTag();
                        pWnd->CenterWindow();
                        pWnd->ShowModal();
                    }
                }
            }
        }
        else if (strSender == _T("wordaddnewaddbtn"))
        {
            pList = xFindControl(CListUI, _T("wordaddnewlist"));
            if (pList->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("�����½�һ�����ʱ���"));
                return;
            }
            i = pList->GetItemAt(pList->GetCurSel())->GetTag();
            strTmp = m_pParent->GetSingleField(_T("Select Id from ���� where ���ʱ�=%d and ����=%d"),
                i, m_nNewWord);
            m_pParent->SaveSetup(_T("word_nwb"), i);
            if (strTmp.IsEmpty())
            {
                m_pParent->ExecuteSql(_T("Insert into ����(���ʱ�, ����) Values(%d, %d)"),
                    i, m_nNewWord);
                MyAfxMessageBox(_T("������ϡ�"));
            }
            else
            {
                MyAfxMessageBox(_T("Ҫ���ӵ������Ѿ����ڡ�"));
                return;
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
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CWordAddnew::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordAddnew::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CWordAddnew::InitControls(TNotifyUI& msg)
{
    CListUI* pList = xFindControl(CListUI, _T("wordaddnewlist"));
    CListLabelElementUI* pItem;
    CString strId, strTmp;
    int x = 0, nIdx = 0;

    strId = m_pParent->ReadSetup(_T("word_nwb"));
    _RecordsetPtr pSetPtr = m_pParent->GetTablesBySql(_T("%s"), _T("Select * from ���ʱ�"));
    pList->RemoveAll();
    if (pSetPtr != NULL)
    {
        while(!pSetPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            //ָ���߶ȺͿ�ȣ��ܹ����Ч��
            //_stprintf(szBuf, _T("height=\"26\" width=\"9999\""));
            //ApplyAttributeList(szBuf)
            strTmp = GetCollect(pSetPtr, _T("Id"));
            pItem->SetTag(_tstoi(strTmp));
            if (strTmp == strId)
            {
                nIdx = x;
            }
            strTmp = GetCollect(pSetPtr, _T("����"));
            pItem->SetText(strTmp);
            pList->Add(pItem);

            pSetPtr->MoveNext();
            x++;
        }
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(nIdx);
    }
}

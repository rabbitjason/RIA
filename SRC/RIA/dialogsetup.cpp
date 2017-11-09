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
#include "dialogsetup.h"
#include "main_frame.h"

CDialogSetup::CDialogSetup(MainFrame* pParent) : m_pParent(pParent), m_nType(0)
{}

CDialogSetup::~CDialogSetup()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogSetup::GetWindowClassName() const
{
	return _T("RIADialogSetupWindow");
}

CControlUI* CDialogSetup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogSetup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogSetup::GetSkinFile()
{
	return _T("dialogsetup.xml");
}

CDuiString CDialogSetup::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogSetup::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogSetup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogSetup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogSetup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogSetup::OnTimer(TNotifyUI& msg)
{
}

void CDialogSetup::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogSetup::InitWindow()
{
    ASSERT(m_pParent);
    ASSERT(m_nType >= 1 && m_nType <= 3);
}

//static LPCTSTR s_strSqlScenes[] =
//{
//    _T("跟读场景"),
//    _T("人机对话场景"),
//    _T("情景对话场景"),
//};
//
//static LPCTSTR s_strSqlCotents[] =
//{
//    _T("跟读内容"),
//    _T("人机对话内容"),
//    _T("情景对话内容"),
//};
//
//static LPCTSTR s_strSpeakers[] =
//{
//    _T("Spoken_Speaker"),
//    _T("Talk_Speaker"),
//    _T("SceneTalk_Speaker"),
//};
//
//static LPCTSTR s_strSpeeds[] =
//{
//    _T("Spoken_Speed"),
//    _T("Talk_Speed"),
//    _T("SceneTalk_Speed"),
//};
//
//static LPCTSTR s_strLevels[] =
//{
//    _T("Spoken_Level"),
//    _T("Talk_Level"),
//    _T("SceneTalk_Level"),
//};

//各窗口的初始化在此完成
void CDialogSetup::OnPrepare(TNotifyUI& msg)
{
    static LPCTSTR s_Title[] =
    {
        _T("跟我读设置"),
        _T("人机对话设置"),
        _T("情景对话设置"),
    };
    //static LPCTSTR s_strScenes[] =
    //{
    //    _T("Spoken_Scene"),
    //    _T("Talk_Scene"),
    //    _T("SceneTalk_Scene"),
    //};

    CComboUI* pCombo;
    CSliderUI* pSld;
    CString strTmp;
    CListLabelElementUI* pItem;
    int i;

    ::SetWindowText(GetHWND(), s_Title[m_nType - 1]);

    m_pParent->InitVoiceRateControl(&m_PaintManager, s_strDlgSpeakers[m_nType - 1], s_strDlgSpeeds[m_nType - 1]);
    
    pCombo = xFindControl(CComboUI, _T("dlgsetupscenegrpcmb"));
    pCombo->SetVisible(false);
    if (DIALOG_TALK == m_nType)   //人机对话
    {
        pCombo->SetVisible(true);
    }

    static LPCTSTR ds_cfg[] =
    {
        _T("Spoken_Scene"),
        _T("Talk_Scene"),
        _T("SceneTalk_Scene")
    };
    static LPCTSTR ds_sql[] =
    {
        _T("Select * from 跟读场景"),
        _T("Select * from 人机对话场景"),
        _T("Select * from 情景对话场景")
    };

    CString strSetup = m_pParent->ReadSetup(ds_cfg[m_nType - 1]);

    pCombo = xFindControl(CComboUI, _T("dlgsetupscenecmb"));
    pCombo->RemoveAll();
    int nSel = 0, idx = 0;
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), ds_sql[m_nType - 1]);
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pItem->SetUserData(strTmp);
            if (strTmp == strSetup)
            {
                nSel = idx;
            }
            pCombo->Add(pItem);

            pRecdPtr->MoveNext();
            idx++;
        }
        pCombo->SelectItem(nSel);
    }

    strTmp = m_pParent->ReadSetup(s_strDlgLevels[m_nType - 1]);
    i = _tstoi(strTmp);
    if (i < 0 || i > 6)
    {
        i = 3;
    }
    pSld = xFindControl(CSliderUI, _T("dlgsetuplevel"));
    pSld->SetValue(i);

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("dlgsetupleveltxt"));
    pLbl->SetText(strTmp);
}

void CDialogSetup::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CSliderUI* pSlider;
    //CTextUI* pTxt;
    CString strTmp;
    CComboUI* pCombo;
    CLabelUI* pLbl;

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
        else if (strSender == _T("dlgsetupdefaultbtn"))
        {
            m_pParent->DefaultVoiceRateControl(&m_PaintManager);
            pSlider = xFindControl(CSliderUI, _T("dlgsetuplevel"));
            pSlider->SetValue(3);
            pCombo = xFindControl(CComboUI, _T("dlgsetupscenecmb"));
            if (pCombo->GetCount() > 0)
            {
                pCombo->SelectItem(0);
            }
        }
        else if (_T("dlgsetupokbtn") == strSender)
        {
            m_pParent->SaveVoiceRate(&m_PaintManager, s_strDlgSpeakers[m_nType - 1], s_strDlgSpeeds[m_nType - 1]);
            pSlider = xFindControl(CSliderUI, _T("dlgsetuplevel"));
            m_pParent->SaveSetup(s_strDlgLevels[m_nType - 1], pSlider->GetValue());

            pCombo = xFindControl(CComboUI, _T("dlgsetupscenecmb"));
            if (pCombo->GetCount() > 0 &&
                pCombo->GetCurSel() >= 0)
            {
                m_pParent->SaveSetup(s_strDlgSqlScenes[m_nType - 1], pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
            }
            pCombo = xFindControl(CComboUI, _T("dlgsetupscenegrpcmb"));
            if (pCombo->IsVisible() &&
                pCombo->GetCount() > 0 &&
                pCombo->GetCurSel() >= 0)
            {
                m_pParent->m_strDialogGroup = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
            }
            else
            {
                m_pParent->m_strDialogGroup = _T("0");
            }
            OnExit(msg);
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        if (_T("dlgsetupscenecmb") == strSender)
        {
            OnSceneCmbSelChanged();
        }
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
        if (_T("dlgsetuplevel") == strSender)
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            pLbl = xFindControl(CLabelUI, _T("dlgsetupleveltxt"));
            pLbl->SetText(strTmp);
        }
        else if (_T("listenspeed") == strSender)
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            pLbl = xFindControl(CLabelUI, _T("listenspeedtxt"));
            pLbl->SetText(strTmp);
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CDialogSetup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDialogSetup::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CDialogSetup::OnSceneCmbSelChanged()
{
    CComboUI* pCombo;
    CString strTmp;

    if (m_nType == DIALOG_SCENETALK)
    {
        return;
    }
    pCombo = xFindControl(CComboUI, _T("dlgsetupscenecmb"));
    CString strId = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData();
    if (m_nType == DIALOG_FOLLOWME)
    {
        strTmp = m_pParent->GetSingleField(_T("select 是否导入 from %s where Id=%s"),
            s_strDlgSqlScenes[m_nType], strId);
    }
    else
    {
        //人机对话，总是开启
        strTmp = _T("1");
    }
    pCombo = xFindControl(CComboUI, _T("dlgsetupscenegrpcmb"));
    if (strTmp == _T("1"))
    {
        CListLabelElementUI* pItem;
        _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("Select * from %s where 场景=%s order by %s"),
            s_strDlgSqlGroups[m_nType - 1], strId, m_nType == DIALOG_FOLLOWME ? _T("顺序") : _T("名称"));
        pCombo->RemoveAll();
        pCombo->SetVisible(true);
        if (pRecdPtr)
        {
            while(!pRecdPtr->GetadoEOF())
            {
                pItem = new CListLabelElementUI;
                pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
                strTmp = GetCollect(pRecdPtr, _T("Id"));
                pItem->SetUserData(strTmp);
                pCombo->Add(pItem);

                pRecdPtr->MoveNext();
            }
            pCombo->SelectItem(0);
        }
    }
    else
    {
        pCombo->SetVisible(false);
    }
}
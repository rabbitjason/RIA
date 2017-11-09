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
#include "dialogwrittingsetup.h"
#include "main_frame.h"

CDialogWrittingSetup::CDialogWrittingSetup(MainFrame* pParent) : m_pParent(pParent)
{}

CDialogWrittingSetup::~CDialogWrittingSetup()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogWrittingSetup::GetWindowClassName() const
{
	return _T("RIADialogWrittingSetupWindow");
}

CControlUI* CDialogWrittingSetup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogWrittingSetup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogWrittingSetup::GetSkinFile()
{
	return _T("dialogwrittingsetup.xml");
}

CDuiString CDialogWrittingSetup::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogWrittingSetup::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogWrittingSetup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogWrittingSetup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogWrittingSetup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogWrittingSetup::OnTimer(TNotifyUI& msg)
{
}

void CDialogWrittingSetup::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogWrittingSetup::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CDialogWrittingSetup::OnPrepare(TNotifyUI& msg)
{
    CString strTmp;
    int i;

    m_pParent->InitVoiceRateControl(&m_PaintManager, _T("Writing_Speaker"), _T("Writing_Speed"));

    strTmp = m_pParent->ReadSetup(_T("Writing_RecordLast"));
    i = _tstoi(strTmp);
    if (i < 0 || i > 99)
    {
        i = 90;
    }
    CEditUI* pEdt = xFindControl(CEditUI, _T("dlgwrittingsetuptimeedt"));
    strTmp.Format(_T("%d"), i);
    pEdt->SetText(strTmp);
}

void CDialogWrittingSetup::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CSliderUI* pSlider;
    //CTextUI* pTxt;
    CString strTmp;
    CLabelUI* pLbl;
    CEditUI* pEdt;
    int i;

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
        else if (_T("dlgwrittingsetupdefaultbtn") == strSender)
        {
            m_pParent->DefaultVoiceRateControl(&m_PaintManager);
            pEdt = xFindControl(CEditUI, _T("dlgwrittingsetuptimeedt"));
            pEdt->SetText(_T("90"));
        }
        else if (_T("dlgwrittingsetupokbtn") == strSender)
        {
            pEdt = xFindControl(CEditUI, _T("dlgwrittingsetuptimeedt"));
            strTmp = pEdt->GetText();
            if (!IsNumeric(strTmp))
            {
                MyAfxMessageBox(_T("%s"), _T("录音时间必须是数字。"));
                pEdt->SetFocus();
                return;
            }
            i = _tstoi(strTmp);
            if (i < 0 || i > 99)
            {
                MyAfxMessageBox(_T("%s"), _T("请输入0~99之间的数字。"));
                pEdt->SetFocus();
                return;
            }
            m_pParent->SaveSetup(_T("Writing_RecordLast"), strTmp);
            m_pParent->SaveVoiceRate(&m_PaintManager, _T("Writing_Speaker"), _T("Writing_Speed"));

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
        if (_T("dlgwrittingsetuptimeedt") == strSender)
        {
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

LRESULT CDialogWrittingSetup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDialogWrittingSetup::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

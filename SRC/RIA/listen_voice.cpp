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
#include "listen_voice.h"
#include "main_frame.h"

CListenVoiceSetup::CListenVoiceSetup(MainFrame* pParent) : m_pParent(pParent)
{}

CListenVoiceSetup::~CListenVoiceSetup()
{
	//PostQuitMessage(0);
}

LPCTSTR CListenVoiceSetup::GetWindowClassName() const
{
	return _T("RIAListenVoiceSetupWindow");
}

CControlUI* CListenVoiceSetup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CListenVoiceSetup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CListenVoiceSetup::GetSkinFile()
{
	return _T("listensetup.xml");
}

CDuiString CListenVoiceSetup::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CListenVoiceSetup::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CListenVoiceSetup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CListenVoiceSetup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CListenVoiceSetup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CListenVoiceSetup::OnTimer(TNotifyUI& msg)
{
}

void CListenVoiceSetup::OnExit(TNotifyUI& msg)
{
	Close();
}

void CListenVoiceSetup::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CListenVoiceSetup::OnPrepare(TNotifyUI& msg)
{
    m_pParent->InitVoiceRateControl(&m_PaintManager, _T("Listen_Speaker"), _T("Listen_Speed"));
}

void CListenVoiceSetup::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CSliderUI* pSlider;
    CTextUI* pTxt;
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
        else if (strSender == _T("listendefaultbtn"))
        {
            m_pParent->DefaultVoiceRateControl(&m_PaintManager);
        }
        else if (strSender == _T("listenokbtn"))
        {
            m_pParent->SaveVoiceRate(&m_PaintManager, _T("Listen_Speaker"), _T("Listen_Speed"));
            m_pParent->SetVoiceRateBySetting(_T("Listen_Speaker"), _T("Listen_Speed"));
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
        if (strSender == _T("listenspeed"))
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("listenspeedtxt")));
            pTxt->SetText(strTmp);
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CListenVoiceSetup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CListenVoiceSetup::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

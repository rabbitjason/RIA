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
#include "readingbarsetup.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"

CReadingbarSetup::CReadingbarSetup(MainFrame* pParent) : m_pParent(pParent)
{}

CReadingbarSetup::~CReadingbarSetup()
{
	//PostQuitMessage(0);
}

LPCTSTR CReadingbarSetup::GetWindowClassName() const
{
	return _T("RIAReadingbarSetupWindow");
}

CControlUI* CReadingbarSetup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CReadingbarSetup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CReadingbarSetup::GetSkinFile()
{
	return _T("readingbarcfg.xml");
}

CDuiString CReadingbarSetup::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CReadingbarSetup::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CReadingbarSetup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CReadingbarSetup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CReadingbarSetup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CReadingbarSetup::OnTimer(TNotifyUI& msg)
{
}

void CReadingbarSetup::OnExit(TNotifyUI& msg)
{
	Close();
}

void CReadingbarSetup::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CReadingbarSetup::OnPrepare(TNotifyUI& msg)
{
    m_pParent->InitVoiceRateControl(&m_PaintManager, _T("ReadingBar_Speaker"), _T("ReadingBar_Speed"));
}

void CReadingbarSetup::Notify(TNotifyUI& msg)
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
        else if (strSender == _T("readbardefaultbtn"))
        {
            m_pParent->DefaultVoiceRateControl(&m_PaintManager);
        }
        else if (strSender == _T("readbarokbtn"))
        {
            m_pParent->SaveVoiceRate(&m_PaintManager, _T("ReadingBar_Speaker"), _T("ReadingBar_Speed"));
            MyAfxMessageBox(_T("保存成功。"));
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
            pSlider = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("listenspeed")));
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

LRESULT CReadingbarSetup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CReadingbarSetup::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

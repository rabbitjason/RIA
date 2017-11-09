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
#include "sr.h"
#include "main_frame.h"

CDialogSR::CDialogSR(MainFrame* pParent) : m_pParent(pParent)
{}

CDialogSR::~CDialogSR()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogSR::GetWindowClassName() const
{
	return _T("RIADialogSRWindow");
}

CControlUI* CDialogSR::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogSR::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogSR::GetSkinFile()
{
	return _T("sr.xml");
}

CDuiString CDialogSR::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogSR::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogSR::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogSR::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //DUITRACE(_T("CDialogSR,recv %s"), DUI__TraceMsg(uMsg));
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogSR::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogSR::OnTimer(TNotifyUI& msg)
{
}

void CDialogSR::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogSR::InitWindow()
{
    ASSERT(m_pParent);

    //::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW |SWP_NOSIZE);
    //CenterWindow();
}

//各窗口的初始化在此完成
void CDialogSR::OnPrepare(TNotifyUI& msg)
{
    ::SetTimer(GetHWND(), SR_TIMER_ID, 1000, NULL);
}

void CDialogSR::Notify(TNotifyUI& msg)
{
    CString sType = msg.sType.GetData();

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
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

LRESULT CDialogSR::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    switch (uMsg)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case SR_TIMER_ID:
            {
                int v;
                bHandled = TRUE;
                if (m_pParent->m_AiEngine.IsCallbackQuit())
                {
                    ::KillTimer(GetHWND(), SR_TIMER_ID);
                    Close();
                    bHandled = FALSE;
                    break;
                }
                CProgressUI* pPgrs = xFindControl(CProgressUI, _T("srprgrs"));
                v = pPgrs->GetValue();
                v++;
                if (v > 10)
                {
                    v = 0;
                }
                pPgrs->SetValue(v);
            }
        	break;
        default:
            break;
        }
    	break;
    default:
        break;
    }
	return 0;
}

LPCTSTR CDialogSR::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

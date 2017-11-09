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
#include "loading.h"
#include "main_frame.h"

CDialogLoading::CDialogLoading(MainFrame* pParent) : m_pParent(pParent)
{}

CDialogLoading::~CDialogLoading()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogLoading::GetWindowClassName() const
{
	return _T("RIADialogLoadingWindow");
}

CControlUI* CDialogLoading::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogLoading::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogLoading::GetSkinFile()
{
	return _T("loading.xml");
}

CDuiString CDialogLoading::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogLoading::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogLoading::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogLoading::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogLoading::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogLoading::OnTimer(TNotifyUI& msg)
{
}

void CDialogLoading::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogLoading::InitWindow()
{
    ASSERT(m_pParent);

    ::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW |SWP_NOSIZE);
    CenterWindow();
}

//各窗口的初始化在此完成
void CDialogLoading::OnPrepare(TNotifyUI& msg)
{
}

void CDialogLoading::Notify(TNotifyUI& msg)
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

LRESULT CDialogLoading::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDialogLoading::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

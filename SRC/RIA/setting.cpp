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
#include "setting.h"
#include "readingbarsetup.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"

CAppSetting::CAppSetting(MainFrame* pParent) : m_pParent(pParent)
{}

CAppSetting::~CAppSetting()
{
	//PostQuitMessage(0);
}

LPCTSTR CAppSetting::GetWindowClassName() const
{
	return _T("RIAAppSettingWindow");
}

CControlUI* CAppSetting::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CAppSetting::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CAppSetting::GetSkinFile()
{
	return _T("setting.xml");
}

CDuiString CAppSetting::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CAppSetting::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CAppSetting::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAppSetting::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CAppSetting::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CAppSetting::OnTimer(TNotifyUI& msg)
{
}

void CAppSetting::OnExit(TNotifyUI& msg)
{
	Close();
}

void CAppSetting::InitWindow()
{
    ASSERT(m_pParent);
}

extern CString g_strVersion;
extern CString g_strCopyright;

//各窗口的初始化在此完成
void CAppSetting::OnPrepare(TNotifyUI& msg)
{
    CComboUI* pCombo;
    CCheckBoxUI* pChk;
    CLabelUI* pLbl;
    CString strText;

    pLbl = xFindControl(CLabelUI, _T("appsettingnametxt"));
    strText.LoadString(AFX_IDS_APP_TITLE);
    pLbl->SetText(strText);

    pLbl = xFindControl(CLabelUI, _T("appsettingversiontxt"));
    strText.Format(_T("版本：V%s"), g_strVersion);
    pLbl->SetText(strText);

    pLbl = xFindControl(CLabelUI, _T("appsettingcopyrighttxt"));
    pLbl->SetText(g_strCopyright);

    pLbl = xFindControl(CLabelUI, _T("appsettingsntxt"));
    strText.Format(_T("产品序列号：%s"), m_pParent->RegKeyGetValue(REG_SerialNumber));
    pLbl->SetText(strText);

    pLbl = xFindControl(CLabelUI, _T("appsettingdbvertxt"));
    strText.Format(_T("数据库版本：%s"), m_pParent->ReadSetup(_T("Version")));
    pLbl->SetText(strText);

    pCombo = xFindControl(CComboUI, _T("appsettingintervalcmb"));
    int x = _tstoi(m_pParent->RegKeyGetValue(REG_Updateinterval));
    TRACE(_T("REG_Updateinterval:%d, count:%d\n"), x, pCombo->GetCount());
    if (x >= pCombo->GetCount())
    {
        x = 1;
    }
    pCombo->SelectItem(x);

    pChk = xFindControl(CCheckBoxUI, _T("appsettingautoupdatechk"));
    pChk->SetCheck(m_pParent->RegKeyGetValue(REG_Updateautocheck) == _T("1") ? true : false);
}

void CAppSetting::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CComboUI* pCombo;
    CCheckBoxUI* pChk;
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
        else if (strSender == _T("appsettingdefaultbtn"))
        {
            pChk = xFindControl(CCheckBoxUI, _T("appsettingautoupdatechk"));
            pChk->SetCheck(true);
            pCombo = xFindControl(CComboUI, _T("appsettingintervalcmb"));
            pCombo->SelectItem(1);
        }
        else if (strSender == _T("appsettingokbtn"))
        {
            pChk = xFindControl(CCheckBoxUI, _T("appsettingautoupdatechk"));
            m_pParent->RegKeySetValue(REG_Updateautocheck, pChk->GetCheck() ? _T("1") : _T("0"));
            pCombo = xFindControl(CComboUI, _T("appsettingintervalcmb"));
            strTmp.Format(_T("%d"), pCombo->GetCurSel());
            m_pParent->RegKeySetValue(REG_Updateinterval, strTmp);
            OnExit(msg);
        }
        else if (strSender == _T("appsettingclearbtn"))
        {
            strTmp.Format(_T("确定要清除历史成绩吗？"));
            if (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                m_pParent->StudyExecuteSql(_T("delete * from 成绩记录"));
            }
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        if (strSender == _T("appsettingautoupdatechk"))
        {
            pChk = static_cast<CCheckBoxUI*>(msg.pSender);
            pCombo = xFindControl(CComboUI, _T("appsettingintervalcmb"));
            pCombo->SetEnabled(pChk->GetCheck());
            //int x = _tstoi(m_pParent->RegKeyGetValue(REG_Updateinterval));
            //TRACE(_T("REG_Updateinterval:%d, count:%d\n"), x, pCombo->GetCount());
            //if (x >= pCombo->GetCount())
            //{
            //    x = 1;
            //}
            //pCombo->SelectItem(x);
        }
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

LRESULT CAppSetting::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CAppSetting::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

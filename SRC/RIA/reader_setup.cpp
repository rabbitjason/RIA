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
#include "main_frame.h"

CReaderSetup::CReaderSetup(MainFrame* pParent) : m_pParent(pParent)
{}

CReaderSetup::~CReaderSetup()
{
	//PostQuitMessage(0);
}

LPCTSTR CReaderSetup::GetWindowClassName() const
{
	return _T("RIAReaderSetupWindow");
}

CControlUI* CReaderSetup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CReaderSetup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CReaderSetup::GetSkinFile()
{
	return _T("readcfg.xml");
}

CDuiString CReaderSetup::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CReaderSetup::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CReaderSetup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CReaderSetup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CReaderSetup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CReaderSetup::OnTimer(TNotifyUI& msg)
{
}

void CReaderSetup::OnExit(TNotifyUI& msg)
{
	Close();
}

void CReaderSetup::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CReaderSetup::OnPrepare(TNotifyUI& msg)
{
    //CString strSpeaker = m_pParent->ReadSetup(_T("Lesson_Speaker"));
    CString strChangeSpeaker = m_pParent->ReadSetup(_T("Lesson_ChangeSpeaker"));
    CString strSecondSpeaker = m_pParent->ReadSetup(_T("Lesson_SecondSpeaker"));
    //CString strSpeed = m_pParent->ReadSetup(_T("Lesson_Speed"));
    CString strLevel = m_pParent->ReadSetup(_T("Lesson_Level"));

    CListLabelElementUI* pElement;
    CComboUI* pCombo;
    CCheckBoxUI* pChk;

    m_pParent->InitVoiceRateControl(&m_PaintManager, _T("Lesson_Speaker"), _T("Lesson_Speed"));

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("readercfgreader2cmb")));
    pElement = new CListLabelElementUI;
    pElement->SetText(MAN_VOICE);
    pCombo->Add(pElement);
    pElement = new CListLabelElementUI;
    pElement->SetText(WOMAN_VOICE);
    pCombo->Add(pElement);
    pCombo->SelectItem((strSecondSpeaker == MAN_VOICE) ? 0 : 1);

    pChk = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("readercfgchangechk")));
    pChk->SetCheck(strChangeSpeaker == _T("1"));

    int s = _tstoi(strLevel);
    if ((s < -10) ||
        (s > 10) )
    {
        s = 0;
        strLevel.Format(_T("%d"), s);
    }
    CSliderUI* pSlid = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("readercfglevel")));
    pSlid->SetValue(s);
    CTextUI* pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("readercfgleveltxt")));
    pTxt->SetText(strLevel);
}

void CReaderSetup::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CSliderUI* pSlider;
    CTextUI* pTxt;
    CComboUI* pCombo;
    CCheckBoxUI* pChk;
    CString strTmp;
    CListLabelElementUI* pEle;

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
        else if (strSender == _T("readercfgdefaultbtn"))
        {
            m_pParent->DefaultVoiceRateControl(&m_PaintManager);
            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("readercfgreader2cmb")));
            pCombo->SelectItem(0);

            pChk = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("readercfgchangechk")));
            pChk->SetCheck(false);

            pSlider = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("readercfglevel")));
            pSlider->SetValue(3);
            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("readercfgleveltxt")));
            pTxt->SetText(_T("3"));
        }
        else if (strSender == _T("readercfgokbtn"))
        {
            m_pParent->SaveVoiceRate(&m_PaintManager, _T("Lesson_Speaker"), _T("Lesson_Speed"));

            pChk = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("readercfgchangechk")));
            m_pParent->SaveSetup(_T("Lesson_ChangeSpeaker"), pChk->GetCheck() ? _T("1") : _T("0"));

            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("readercfgreader2cmb")));
            pEle = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
            m_pParent->SaveSetup(_T("Lesson_SecondSpeaker"), pEle->GetText().GetData());

            pSlider = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("readercfglevel")));
            m_pParent->SaveSetup(_T("Lesson_Level"), pSlider->GetValue());

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
        if (strSender == _T("readercfglevel"))
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("readercfgleveltxt")));
            pTxt->SetText(strTmp);
        }
        else if (_T("listenspeed") == strSender)
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            CLabelUI* pLbl = xFindControl(CLabelUI, _T("listenspeedtxt"));
            pLbl->SetText(strTmp);
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CReaderSetup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CReaderSetup::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

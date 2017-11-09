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
#include "translateword.h"
#include "main_frame.h"

CTranslateWord::CTranslateWord(MainFrame* pParent) : m_pParent(pParent)
{}

CTranslateWord::~CTranslateWord()
{
	//PostQuitMessage(0);
}

LPCTSTR CTranslateWord::GetWindowClassName() const
{
	return _T("RIATranslateWordWindow");
}

CControlUI* CTranslateWord::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CTranslateWord::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
    m_pParent->StopSpeak();
	delete this;
}

CDuiString CTranslateWord::GetSkinFile()
{
	return _T("readtrans.xml");
}

CDuiString CTranslateWord::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CTranslateWord::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CTranslateWord::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CTranslateWord::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CTranslateWord::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CTranslateWord::OnTimer(TNotifyUI& msg)
{
}

void CTranslateWord::OnExit(TNotifyUI& msg)
{
	Close();
}

void CTranslateWord::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CTranslateWord::OnPrepare(TNotifyUI& msg)
{
    CLabelUI* pLbl;
    CRichEditUI* pRedt;
    CButtonUI* pBtn;
    CString strTmp;

    bool bFound = false;
    do 
    {
        if (m_strWord.IsEmpty())
        {
            strTmp = _T("没有要查询的单词。");
            break;
        }
        m_pParent->SetVoiceRateBySetting(_T("Lesson_Speaker"), _T("Lesson_Speed"));

        m_strWord.Trim();
        m_strWord.Replace(_T("'"), _T("''"));
        _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("Select * from 单词 where 单词='%s'"),
            m_strWord);
        if (pRecdPtr == NULL)
        {
            strTmp = _T("词典中没有您查询的单词。");
            break;
        }

        pLbl = xFindControl(CLabelUI, _T("readtranwordtxt"));
        pLbl->SetText(GetCollect(pRecdPtr, _T("单词")));

        pLbl = xFindControl(CLabelUI, _T("readtranymboltxt"));
        pLbl->SetText(GetCollect(pRecdPtr, _T("音标")));

        pLbl = xFindControl(CLabelUI, _T("readtranParaphrasetxt"));
        pLbl->SetText(GetCollect(pRecdPtr, _T("释义")));

        pRedt = xFindControl(CRichEditUI, _T("readtranenglishsampletxt"));
        strTmp = GetCollect(pRecdPtr, _T("英文例句"));
        pRedt->SetText(strTmp);
        pBtn = xFindControl(CButtonUI, _T("readtranspeakesamplebtn"));
        pBtn->SetVisible(!strTmp.IsEmpty());

        pRedt = xFindControl(CRichEditUI, _T("readtranchinesesampletxt"));
        pRedt->SetText(GetCollect(pRecdPtr, _T("中文例句")));

        bFound = true;
    } while (false);

    if (!bFound)
    {
        CHorizontalLayoutUI *pUI = xFindControl(CHorizontalLayoutUI, _T("readtranerrhlayout"));
        pLbl = xFindControl(CLabelUI, _T("readtranerrortxt"));
        pLbl->SetText(strTmp);
        pUI->SetVisible(true);

        pLbl = xFindControl(CLabelUI, _T("readtranwordtxt"));
        pLbl->SetVisible(false);

        pLbl = xFindControl(CLabelUI, _T("readtranymboltxt"));
        pLbl->SetVisible(false);

        pLbl = xFindControl(CLabelUI, _T("readtranParaphrasetxt"));
        pLbl->SetVisible(false);

        pRedt = xFindControl(CRichEditUI, _T("readtranenglishsampletxt"));
        pRedt->SetVisible(false);

        pRedt = xFindControl(CRichEditUI, _T("readtranchinesesampletxt"));
        pRedt->SetVisible(false);

        pBtn = xFindControl(CButtonUI, _T("readtranpeakewordbtn"));
        pBtn->SetVisible(false);

        pBtn = xFindControl(CButtonUI, _T("readtranspeakesamplebtn"));
        pBtn->SetVisible(false);
    }
}

void CTranslateWord::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();

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
        else if (strSender == _T("readtranpeakewordbtn"))
        {
            SpeakIt(_T("readtranwordtxt"));
        }
        else if (strSender == _T("readtranspeakesamplebtn"))
        {
            SpeakIt(_T("readtranenglishsampletxt"));
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

LRESULT CTranslateWord::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CTranslateWord::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

HRESULT CTranslateWord::SpeakIt(LPCTSTR strCtrl)
{
    CString strTmp;
    CControlUI *pCtrl = xFindControl(CControlUI, strCtrl);

    strTmp = pCtrl->GetText();
    if (!strTmp.IsEmpty())
    {
        m_pParent->StopSpeak();
        return m_pParent->Speak(strTmp);
    }
    return S_OK;
}

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
#include "wordtovoice.h"
#include "main_frame.h"

CWordToVoice::CWordToVoice(MainFrame* pParent) : m_pParent(pParent)
{}

CWordToVoice::~CWordToVoice()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordToVoice::GetWindowClassName() const
{
	return _T("RIAWordToVoiceWindow");
}

CControlUI* CWordToVoice::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordToVoice::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
    m_pParent->StopSpeak();
	delete this;
}

CDuiString CWordToVoice::GetSkinFile()
{
	return _T("wordtovoice.xml");
}

CDuiString CWordToVoice::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordToVoice::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordToVoice::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordToVoice::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordToVoice::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordToVoice::OnTimer(TNotifyUI& msg)
{
}

void CWordToVoice::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordToVoice::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CWordToVoice::OnPrepare(TNotifyUI& msg)
{
    m_bExported = false;
    InitControls(msg);
}

void CWordToVoice::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CSliderUI* pSlider;
    CComboUI* pCombo;
    //CCheckBoxUI* pChk;
    CLabelUI* pLbl;
    //CButtonUI* pButton;
    //int i;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if ((strSender == kCloseButtonName) ||
            (strSender == kReturnButtonName) ||
            (strSender == kSubReturnButtonName) ||
            (strSender == _T("dlgreturnbtn"))
            )
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
        else if (strSender == _T("wordvoicesubmit"))
        {
            if (m_bExported)
            {
                strTmp = m_strPathFile.Left(m_strPathFile.ReverseFind(_T('\\')));
                HINSTANCE hRet = ::ShellExecute(NULL, _T("explore"), strTmp, NULL, NULL, SW_MAXIMIZE);
                TRACE(_T("ShellExecute return:%d\n"), hRet);
                Close();
                return;
            }

            pCombo = xFindControl(CComboUI, _T("wordvoicefromcmb"));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("当前没有任何可以供制作音频的单词。"));
                return;
            }
            strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
            pCombo = xFindControl(CComboUI, _T("wordvoicetocmb"));
            if (strTmp.CompareNoCase(pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData()) > 0)
            {
                MyAfxMessageBox(_T("起始位置的单词必须比截至位置的单词靠前。"));
                return;
            }

            DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_NONETWORKBUTTON |
                OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
            CFileDialog cfg(FALSE, _T("mp3"), NULL, dwFlags, _T("mp3文件(*.mp3)|*.mp3||"), CWnd::FromHandle(GetHWND()));
            OPENFILENAME& ofn = cfg.GetOFN();
            TCHAR tbuf[4096];
            tbuf[0] = _T('\0');
            ofn.lpstrFile = tbuf;
            ofn.nMaxFile = _countof(tbuf);
            ofn.hwndOwner = GetHWND();

            if (cfg.DoModal() != IDOK)
            {
                return;
            }

            m_strPathFile = cfg.GetPathName();

            ExportMp3();
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
            pLbl = xFindControl(CLabelUI, _T("listenspeedtxt"));
            pLbl->SetText(strTmp);
        }
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

LRESULT CWordToVoice::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordToVoice::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CWordToVoice::InitControls(TNotifyUI& msg)
{
    CComboUI* pCombo;
    CString strSql, strTmp;
    CListLabelElementUI* pElement;

    m_pParent->InitVoiceRateControl(&m_PaintManager, _T("Word_Speaker"), _T("Word_Speed"));

    //CString strwordtype = m_pParent->ReadSetup(_T("word_type"));
    CString strnewbook = m_pParent->ReadSetup(_T("word_nwb"));
    CString strGId = m_pParent->ReadSetup(_T("word_grade"));
    CString strBId = m_pParent->ReadSetup(_T("word_book"));
    CString strUId = m_pParent->ReadSetup(_T("word_unit"));
    //TRACE(_T("grade:%s, book:%s, unit:%s\n"), strGId, strBId, strUId);

    if (strGId == _T("0"))
    {
        strSql.Format(_T("Select distinct 单词 from 单词 where Id in (Select 生词 from 生词 where 生词本=%s)"),
            strnewbook);
    }
    else
    {
        strSql.Format(_T("Select 单词 from 单词 where 年级=%s"),
            strGId);
        if (strBId != _T("0"))
        {
            strTmp.Format(_T(" and 教材=%s"),
                strBId);
            strSql += strTmp;
        }
        if (strUId != _T("0"))
        {
            strTmp.Format(_T(" and 课文=%s"),
                strUId);
            strSql += strTmp;
        }
    }
    strSql += _T(" Order by 单词");

    pCombo = xFindControl(CComboUI, _T("wordvoicefromcmb"));
    CComboUI *pCombTo = xFindControl(CComboUI, _T("wordvoicetocmb"));
    pCombo->RemoveAll();
    pCombTo->RemoveAll();
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), strSql);
    if (!pRecdPtr)
    {
        pCombo->SetEnabled(false);
        pCombTo->SetEnabled(false);
        return;
    }
    while(!pRecdPtr->GetadoEOF())
    {
        strTmp = GetCollect(pRecdPtr, _T("单词"));

        pElement = new CListLabelElementUI;
        pElement->SetText(strTmp);
        pElement->SetUserData(strTmp);
        pCombo->Add(pElement);

        pElement = new CListLabelElementUI;
        pElement->SetText(strTmp);
        pElement->SetUserData(strTmp);
        pCombTo->Add(pElement);

        pRecdPtr->MoveNext();
    }
    if (pCombo->GetCount() > 0)
    {
        pCombo->SelectItem(0);
        pCombTo->SelectItem(pCombTo->GetCount() - 1);
    }
}

void CWordToVoice::ExportMp3()
{
    CComboUI *pCombo;
    CCheckBoxUI* pChk;
    CString strFromWord, strToWord, strTopNumber;
    int nRepeatTime, nStopSeconds, i, j;
    bool bSpellWord, bReadSample;
    CString strSql, strTmp, strSelect = _T("*");

    CString strnewbook = m_pParent->ReadSetup(_T("word_nwb"));
    CString strGId = m_pParent->ReadSetup(_T("word_grade"));
    CString strBId = m_pParent->ReadSetup(_T("word_book"));
    CString strUId = m_pParent->ReadSetup(_T("word_unit"));

    pChk = xFindControl(CCheckBoxUI, _T("wordvoiceonlytopchk"));
    if (pChk->GetCheck())
    {
        pCombo = xFindControl(CComboUI, _T("wordvoiceonlytopcmb"));
        strSelect.Format(_T("top %s *"), pCombo->GetItemAt(pCombo->GetCurSel())->GetText().GetData());
    }

    if (strGId == _T("0"))
    {
        strSql.Format(_T("Select %s from 词典 where Id in (Select 生词 from 生词 where 生词本=%s)"),
            strSelect, strnewbook);
    }
    else
    {
        strSql.Format(_T("Select %s from 单词 where 年级=%s"),
            strSelect, strGId);
        if (strBId != _T("0"))
        {
            strTmp.Format(_T(" and 教材=%s"),
                strBId);
            strSql += strTmp;
        }
        if (strUId != _T("0"))
        {
            strTmp.Format(_T(" and 课文=%s"),
                strUId);
            strSql += strTmp;
        }
    }

    pCombo = xFindControl(CComboUI, _T("wordvoicefromcmb"));
    strFromWord = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    strFromWord.Replace(_T("'"), _T("''"));

    pCombo = xFindControl(CComboUI, _T("wordvoicetocmb"));
    strToWord = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    strToWord.Replace(_T("'"), _T("''"));

    strTmp.Format(_T(" and 单词>='%s' and 单词<='%s'"),
        strFromWord, strToWord);
    strSql += strTmp;

    strSql += _T(" Order by 单词");
    TRACE(_T("sql:%s\n"), strSql);
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), strSql);
    if (pRecdPtr == NULL)
    {
        MyAfxMessageBox(_T("当前没有选择任何单词。"));
        return;
    }

    pCombo = xFindControl(CComboUI,  _T("wordvoicerepeatcmb"));
    nRepeatTime = _tstoi(pCombo->GetItemAt(pCombo->GetCurSel())->GetText().GetData());

    pCombo = xFindControl(CComboUI,  _T("wordvoicecombospacecmb"));
    nStopSeconds = _tstoi(pCombo->GetItemAt(pCombo->GetCurSel())->GetText().GetData()) * 2;

    pChk = xFindControl(CCheckBoxUI, _T("wordvoicespellchk"));
    bSpellWord = pChk->GetCheck();

    pChk = xFindControl(CCheckBoxUI, _T("wordvoicespeaksamplechk"));
    bReadSample = pChk->GetCheck();

    m_pParent->SetVoiceRateBySetting(_T("Word_Speaker"), _T("Word_Speed"));
    //delete the file always
    ::DeleteFile(m_strPathFile);
    CString strWavFile, strTmp2;
    strWavFile = m_strPathFile.Left(m_strPathFile.GetLength() - 3);
    strWavFile += _T("wav");
    ::DeleteFile(strWavFile);

    //TRACE(_T("m_strPathFile:%s, strWavFile:%s\n"), m_strPathFile, strWavFile);
    m_pParent->SpeakBindToFile(strWavFile);
    DuiLib::CWaitCursor cw;
    while(!pRecdPtr->GetadoEOF())
    {
        for (i = 1; i <= nRepeatTime; i++)
        {
            strTmp = GetCollect(pRecdPtr, _T("单词"));
            //TRACE(_T("speak %s\n"), strTmp);
            m_pParent->Speak(strTmp, SPF_DEFAULT);
            if (bSpellWord)
            {
                strTmp2.Format(_T("<spell>%s</spell>"), strTmp);
                m_pParent->Speak(strTmp2, SPF_DEFAULT);
            }
            m_pParent->Speak(strTmp, SPF_DEFAULT);
            m_pParent->Speak(_T("."), SPF_DEFAULT);
            if (bReadSample)
            {
                m_pParent->Speak(GetCollect(pRecdPtr, _T("英文例句")));
                m_pParent->Speak(_T("."), SPF_DEFAULT);
            }
            for (j = 0; j <= nStopSeconds; j++)
            {
                m_pParent->Speak(_T("."), SPF_DEFAULT);
            }
        }
        pRecdPtr->MoveNext();
    }
    m_pParent->SpeakRestore();

    m_pParent->hlpWave2Mp3(strWavFile, m_strPathFile);

    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("wwordvoiceprogtxt"));
    strTmp.Format(_T("音频制作完成，保存路径为：%s。"),
        m_strPathFile);
    pREdt->SetText(strTmp);

    CButtonUI* pBtn = xFindControl(CButtonUI, _T("wordvoicesubmit"));
    pBtn->SetText(_T("打开"));
    m_bExported = true;
}

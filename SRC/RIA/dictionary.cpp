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
#include "wordaddnew.h"
#include "dictionary.h"
#include "main_frame.h"

CDictionary::CDictionary(MainFrame* pParent) : m_pParent(pParent)
{}

CDictionary::~CDictionary()
{
	//PostQuitMessage(0);
}

LPCTSTR CDictionary::GetWindowClassName() const
{
	return _T("RIADictionaryWindow");
}

CControlUI* CDictionary::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDictionary::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDictionary::GetSkinFile()
{
	return _T("dict.xml");
}

CDuiString CDictionary::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDictionary::GetResourceType() const
{
	return SKIN_TYPE;
}

LRESULT CDictionary::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
    if( wParam == SC_CLOSE ) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT CDictionary::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDictionary::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDictionary::OnTimer(TNotifyUI& msg)
{
}

void CDictionary::OnExit(TNotifyUI& msg)
{
    m_pParent->StopSpeak();
	Close();
}

void CDictionary::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CDictionary::OnPrepare(TNotifyUI& msg)
{
    InitDictControls(msg);
}

void CDictionary::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CTextUI* pTxt;
    CString strTmp;
    CListUI* pList;
    CRichEditUI* pRichTxt;

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
            m_pParent->StopSpeak();
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
        else if (strSender == _T("dicspeakewordbtn"))
        {
            m_pParent->StopSpeak();
            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("dicwordtxt")));
            m_pParent->Speak(pTxt->GetText().GetData());
        }
        else if (strSender == _T("dicspeakesamplebtn"))
        {
            m_pParent->StopSpeak();
            pRichTxt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("dicenglishsampletxt")));
            m_pParent->Speak(pRichTxt->GetText().GetData());
        }
        else if (_T("dicaddnewbtn") == strSender)
        {
            pList = xFindControl(CListUI, _T("dicwordlist"));
            if (pList->GetCount() > 0)
            {
                CWordAddnew* pWnd = new CWordAddnew(m_pParent);
                if (pWnd)
                {
                    //保存单词的（在词典中的）ID
                    pWnd->m_nNewWord = pList->GetItemAt(pList->GetCurSel())->GetTag();
                    m_pParent->OpenModalWnd(pWnd, g_SkinInfo[sWordAddNewSkin].title);
                }
            }
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        if (strSender == _T("dicwordlist"))
        {
            pList = static_cast<CListUI*>(msg.pSender);
            InitDictWord(pList->GetCurSel());
        }
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (_T("dickeywordedt") == strSender)
        {
            InitDictControls(msg);
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CDictionary::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDictionary::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CDictionary::InitDictWord(int nIndex)
{
    CListUI* pList = xFindControl(CListUI, _T("dicwordlist"));

    CTextUI* pWordText = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("dicwordtxt")));
    CTextUI* pSymbolText = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("dicsymboltxt")));
    CRichEditUI* pParaText = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("dicParaphrasetxt")));
    CRichEditUI* pEnText = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("dicenglishsampletxt")));
    CRichEditUI* pCnText = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("dicchinesesampletxt")));
    CControlUI* pPicWord = m_PaintManager.FindControl(_T("dicwordpic"));
    CButtonUI* pWordBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("dicspeakewordbtn")));
    CButtonUI* pSampleBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("dicspeakesamplebtn")));

    pWordText->SetVisible(false);
    pSymbolText->SetVisible(false);
    pParaText->SetVisible(false);
    pEnText->SetVisible(false);
    pCnText->SetVisible(false);
    pPicWord->SetVisible(false);
    pWordBtn->SetVisible(false);
    pSampleBtn->SetVisible(false);

    if (pList->GetCount() > 0)
    {
        int nId = pList->GetItemAt(nIndex)->GetTag();
        _RecordsetPtr pWordPtr = m_pParent->GetTablesBySql(_T("Select * from 词典 where Id=%d"),
            nId);
        if (pWordPtr)
        {
            pWordText->SetText(GetCollect(pWordPtr, _T("单词")));
            pWordText->SetVisible(pWordText->GetText().IsEmpty() ? false : true);
            pWordBtn->SetVisible(pWordText->GetText().IsEmpty() ? false : true);

            pSymbolText->SetText(GetCollect(pWordPtr, _T("音标")));
            pSymbolText->SetVisible(pSymbolText->GetText().IsEmpty() ? false : true);

            pParaText->SetText(GetCollect(pWordPtr, _T("释义")));
            pParaText->SetVisible(pParaText->GetText().IsEmpty() ? false : true);

            pEnText->SetText(GetCollect(pWordPtr, _T("英文例句")));
            pEnText->SetVisible(pEnText->GetText().IsEmpty() ? false : true);
            pSampleBtn->SetVisible(pEnText->GetText().IsEmpty() ? false : true);

            pCnText->SetText(GetCollect(pWordPtr, _T("中文例句")));
            pCnText->SetVisible(pCnText->GetText().IsEmpty() ? false : true);

            //pPicWord->SetVisible(false);
            CString strPic = GetCollect(pWordPtr, _T("图片"));
            if (!strPic.IsEmpty())
            {
                CString strPicPath;
                strPicPath.Format(_T("%sData\\Picture\\%s"),
                    CPaintManagerUI::GetInstancePath().GetData(),
                    (LPCTSTR)strPic);
                CFileFind finder;
                if (finder.FindFile(strPicPath))
                {
                    pPicWord->SetBkImage(strPicPath);
                    // 不显示图片 [2016-1-25 Arthur]
                    pPicWord->SetVisible(true);
                }
                finder.Close();
            }
        }
    }
}

void CDictionary::InitDictControls(TNotifyUI& msg)
{
    CEditUI* pEdit;
    CListUI* pList;
    CListLabelElementUI* pItem;
    CString strSql, strTmp;

    pEdit = xFindControl(CEditUI, _T("dickeywordedt"));
    strTmp = pEdit->GetText();
    strTmp.Trim();
    if (strTmp.IsEmpty())
    {
        strSql = _T("Select Id, 单词 from 词典 Order by 单词");
    }
    else
    {
        strTmp.Replace(_T("'"), _T("''"));
        strSql.Format(_T("Select Id, 单词 from 词典 where 单词 like '%%%s%%' Order by 单词"),
            strTmp);
    }

    TRACE(_T("query:%s\n"), strSql);
    _RecordsetPtr pWordsSetPtr = m_pParent->GetTablesBySql(_T("%s"), strSql);

    pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("dicwordlist")));
    pList->RemoveAll();
    LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");
    if (pWordsSetPtr != NULL)
    {
        while(!pWordsSetPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            //指定高度和宽度，能够提高效率
            pItem->ApplyAttributeList(strAttr);
            strTmp = GetCollect(pWordsSetPtr, _T("Id"));
            pItem->SetTag(_tstoi(strTmp));
            strTmp = GetCollect(pWordsSetPtr, _T("单词"));
            pItem->SetText(strTmp);
            pList->Add(pItem);

            pWordsSetPtr->MoveNext();
        }
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(0);
    }
    //InitDictWord(0);
}

//LPCTSTR CDictionary::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
//{
//    TCHAR szBuf[1024] = {0};
//    switch (iSubItem)
//    {
//    case 0:
//        {
//            StringCchPrintf(szBuf, _countof(szBuf), m_ListDicWords[iIndex]);
//        }
//        break;
//    default:
//        break;
//    }
//
//    pControl->SetUserData(szBuf);
//    return pControl->GetUserData();
//}

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
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "wordmgtword.h"
#include "main_frame.h"

LPCTSTR CWordMgtword::m_strAttr = _T("textpadding=\"23,3,0,0\" align=\"left\" font=\"1\" \
normalimage=\"file='Image/checkbox_unchk.png' dest='0,0,23,23'\" \
selectedimage=\"file='Image/checkbox_chk.png' dest='0,0,23,23'\"");

CWordMgtword::CWordMgtword(MainFrame* pParent) : m_pParent(pParent)
{
    m_arSelWord.reserve(64*1204);
    m_arAllWord.reserve(64*1024);
}

CWordMgtword::~CWordMgtword()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordMgtword::GetWindowClassName() const
{
	return _T("RIAWordMgtWordWindow");
}

CControlUI* CWordMgtword::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordMgtword::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordMgtword::GetSkinFile()
{
	return _T("wordmngword.xml");
}

CDuiString CWordMgtword::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordMgtword::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordMgtword::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordMgtword::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordMgtword::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordMgtword::OnTimer(TNotifyUI& msg)
{
}

void CWordMgtword::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordMgtword::InitWindow()
{
    ASSERT(m_pParent);
    ASSERT(!m_strBId.IsEmpty() && !m_strGId.IsEmpty() && !m_strUId.IsEmpty());
}

//各窗口的初始化在此完成
void CWordMgtword::OnPrepare(TNotifyUI& msg)
{
    ::CWaitCursor watc;
    InitControls(msg);
}

void CWordMgtword::Notify(TNotifyUI& msg)
{
    TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CListUI* pList;
    CEditUI* pEdt;
    //CButtonUI* pBtn;
    //CListTextElementUI* pItem;
    CListContainerElementUI* pContain;
    //CCheckBoxUI* pChk;
    CTextUI* pTxt;
    ArrayWords::const_iterator pw;
    int i, n;
    UINT nId;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        if (strSender == kCloseButtonName)
		{
            //OnExit(msg);
            Close(IDCANCEL);
		}
        else if (strSender == kSubReturnButtonName ||
            strSender == kReturnButtonName)
		{
            //OnExit(msg);
            Close(IDOK);
		}
        else if (kMinButtonName == strSender)
        {
#if defined(UNDER_CE)
            ::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
        }
        else if (strSender == _T("wordmngwordokbtn"))
        {
            if (mySave())
            {
                MyAfxMessageBox(_T("单词数据保存成功。"));
            }
            //Close(IDOK);
        }
        else if (strSender == _T("wordmngwordaddbtn"))
        {
            pList = xFindControl(CListUI, _T("wordmngwordalllist"));
            //CListUI* pSelList = xFindControl(CListUIm _T("wordmngwordselectedlist"));
            //int nCur = 0;
            //if (pSelList->GetCount() > 0)
            //{
            //   nCur = pSelList->GetCurSel();
            //}
            n = pList->GetCount();
            for (i = 0; i < n; i++)
            {
                pContain = (CListContainerElementUI*)pList->GetItemAt(i);
                pTxt = (CTextUI*)pContain->GetItemAt(0);
                if (!pContain->IsEnabled())
                {
                    //already in
                    continue;
                }
                if (pContain->IsSelected())
                {
                    nId = pContain->GetTag();
                    Add2SelWord(nId, pTxt->GetText().GetData(), true);
                    pContain->SetEnabled(false);
                    pContain->GetItemAt(0)->SetEnabled(false);
                    pContain->GetItemAt(1)->SetEnabled(false);
                    TRACE(_T("select word:%s, id:%d\n"), pTxt->GetText().GetData(), nId);
                }
#if 0
                pChk = (CCheckBoxUI*)pItem->GetItemAt(0);
                if (pChk->IsSelected())
                {
                    nId = pItem->GetTag();
                    Add2SelWord(nId, pChk->GetText().GetData(), true);
                    pChk->SetEnabled(false);
                    pTxt = (CTextUI*)pItem->GetItemAt(1);
                    pTxt->SetEnabled(false);
                    pItem->SetEnabled(false);

                    //strTmp.Format(_T("insert into 课文单词(年级,教材,课文,单词) values(%s,%s,%s,%lu)"),
                    //    m_strGId, m_strBId, m_strUId, nId);
                    //m_pParent->ExecuteSql(_T("%s"), strTmp);
                }
#endif
            }
        }
        else if (strSender == _T("wordmngworddelbtn"))
        {
            typedef vector<CControlUI*> _removelst;
            _removelst rlst;
            pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
            n = pList->GetCount();
            for (i = 0; i < n; i++)
            {
                pContain = (CListContainerElementUI*)pList->GetItemAt(i);
                pTxt = (CTextUI*)pContain->GetItemAt(0);
                if (pContain->IsSelected())
                {
                    //从已选中删除，并清除待选单词中的状态
                    rlst.push_back(pContain);
                    if (!m_arSelWord[i].bNew)
                    {
                        m_arWordRemoved.push_back(m_arSelWord[i]);
                    }
                }
#if 0
                pChk = (CCheckBoxUI*)pItem->GetItemAt(0);
                if (pChk->IsSelected())
                {
                    //从已选中删除，并清除待选单词中的状态
                    rlst.push_back(pItem);
                    if (!m_arSelWord[i].bNew)
                    {
                        m_arWordRemoved.push_back(m_arSelWord[n]);
                    }
                }
#endif
            }
            _removelst::const_iterator pr;
            CListUI* pAllList = xFindControl(CListUI, _T("wordmngwordalllist"));
            for (pr = rlst.begin(); pr != rlst.end(); ++pr)
            {
                pContain = (CListContainerElementUI*)*pr;
                pTxt = (CTextUI*)pContain->GetItemAt(0);
                nId = pContain->GetTag();
                pw = find(m_arAllWord.begin(), m_arAllWord.end(), nId);
                if (pw != m_arAllWord.end())
                {
                    pContain = (CListContainerElementUI*)pAllList->GetItemAt(pw - m_arAllWord.begin());
                    //pTxt = (CTextUI*)pContain->GetItemAt(0);
#if 0
                    pChk = (CCheckBoxUI*)pItem->GetItemAt(0);
                    pChk->SetEnabled(true);
                    pChk->SetCheck(false);
                    
                    pTxt = (CTextUI*)pItem->GetItemAt(1);
                    pTxt->SetEnabled(true);
#endif
                    pContain->SetEnabled(true);
                    pContain->GetItemAt(0)->SetEnabled(true);
                    pContain->GetItemAt(1)->SetEnabled(true);
                    pContain->Select(false);
                }
#ifdef _DEBUG
                else
                {
                    MyAfxMessageBox(_T("单词ID %d(%s) 不在词典中！"), nId, pTxt->GetText().GetData());
                }
#endif
                pw = find(m_arSelWord.begin(), m_arSelWord.end(), nId);
                ASSERT(pw != m_arSelWord.end());
                TRACE(_T("erase %s\n"), pw->strWord);
                m_arSelWord.erase(pw);
                //m_arSelWord.erase(remove(m_arSelWord.begin(), m_arSelWord.end(), nId), m_arSelWord.end());
                pList->Remove(*pr);

                //strTmp.Format(_T("delete from 课文单词 where 年级=%s and 教材=%s and 课文=%s and 单词=%lu"),
                //    m_strGId, m_strBId, m_strUId, n);
                //m_pParent->ExecuteSql(_T("%s"), strTmp);
            }
        }
        else if (_T("wordmngwordupbtn") == strSender)
        {
            pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
            n = pList->GetCount();
            //如果多选了则不移动
            int nSum = 0;
            for (i = 0; i < n; i++)
            {
                pContain = (CListContainerElementUI*)pList->GetItemAt(i);
                if (pContain->IsSelected())
                {
                    nSum++;
                    if (nSum > 1)
                    {
                        break;
                    }
                }
            }
            if (nSum == 1)
            {
                myUp(pList->GetCurSel());
            }
        }
        else if (_T("wordmngworddownbtn") == strSender)
        {
            pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
            n = pList->GetCount();
            //如果多选了则不移动
            int nSum = 0;
            for (i = 0; i < n; i++)
            {
                pContain = (CListContainerElementUI*)pList->GetItemAt(i);
                if (pContain->IsSelected())
                {
                    nSum++;
                    if (nSum > 1)
                    {
                        break;
                    }
                }
            }
            if (nSum == 1)
            {
                myDown(pList->GetCurSel());
            }
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
#if 0
        pChk = static_cast<CCheckBoxUI*>(msg.pSender);
        pItem = (CListContainerElementUI*)pChk->GetParent();    //should be Item, if it's checkbox
        if (pItem)
        {
            pList = static_cast<CListUI*>(pItem->GetParent());
            if (pList)
            {
                pList = (CListUI*)pList->GetParent();
                if (pList == xFindControl(CListUI, _T("wordmngwordselectedlist")) ||
                    pList == xFindControl(CListUI, _T("wordmngwordalllist")) )
                {
                    pList->SelectItem(pList->GetItemIndex(pItem));
                }
            }
        }
#endif
    }
    else if (sType == DUI_MSGTYPE_ITEMCLICK)
    {
#if 0
        pItem = (CListTextElementUI*)msg.pSender;
        if (pItem)
        {
            pList = (CListUI*)pItem->GetParent();   //it's ListHeader
            if (pList)
            {
                pList = (CListUI*)pList->GetParent();
                if (pList == xFindControl(CListUI, _T("wordmngwordalllist")) ||
                    pList == xFindControl(CListUI, _T("wordmngwordselectedlist")))
                {
#define SHIFTED 0x8000
                    BOOL bCtrl = (GetKeyState(VK_CONTROL) & SHIFTED);
                    BOOL bShift = (GetKeyState(VK_SHIFT) & SHIFTED);
                    //pChk = (CCheckBoxUI*)pItem->GetItemAt(0);
                    //pChk->SetCheck(!pChk->IsSelected());
                    TRACE(_T("%s clicked\n"), pItem->GetText(0));
                    pList->SelectItem(pList->GetItemIndex(pItem), true);
                    //pItem->Select(true);
                    //pItem = (CListTextElementUI*)pList->GetItemAt(pList->GetItemIndex(pItem) + 1);
                    //pItem->Select(true);
                    pList->SelectItem(pList->GetItemIndex(pItem) + 1);
                }
            }
        }
#endif
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (_T("wordmngwordfilteredt") == strSender)
        {
            pEdt = static_cast<CEditUI*>(msg.pSender);
            strTmp = pEdt->GetText().GetData();
            strTmp.Trim();
            InitWordList(strTmp);
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CWordMgtword::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordMgtword::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CWordMgtword::InitControls(TNotifyUI& msg)
{
    CListUI* pList;
    CTextUI* pTxt;
    //CListContainerElementUI* pItem;
    //CCheckBoxUI* pChk;
    CString strTmp, strText;
    UINT    nId;

    strTmp.Format(_T("Select 名称 from 年级 where Id=%s"),
        m_strGId);
    strText = m_pParent->GetSingleField(strTmp) + _T(">");
    strTmp.Format(_T("Select 名称 from 教材 where Id=%s and 年级=%s"),
        m_strBId, m_strGId);
    strText += m_pParent->GetSingleField(strTmp) + _T(">");
        strTmp.Format(_T("Select 名称 from 课文 where Id=%s and 教材=%s and 年级=%s"),
            m_strUId, m_strBId, m_strGId);
    strText += m_pParent->GetSingleField(strTmp);
    pTxt = xFindControl(CTextUI, _T("wordmngwordlocationtxt"));
    pTxt->SetText(strText);

    strTmp.Format(_T("Select Id, 顺序, 单词, 释义 from 单词 where 课文=%s and 教材=%s and 年级=%s Order by 顺序,单词"),
        m_strUId, m_strBId, m_strGId);
    _RecordsetPtr pWordPtr = m_pParent->GetTablesBySql(_T("%s"), strTmp);
    pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
    pList->RemoveAll();
    m_arSelWord.clear();

    if (pWordPtr != NULL)
    {
        CString strId;

        while(!pWordPtr->GetadoEOF())
        {
            strId = GetCollect(pWordPtr, _T("Id"));
            nId = _tstol(strId);
            Add2SelWord(nId, GetCollect(pWordPtr, _T("单词")));
            
            pWordPtr->MoveNext();
        }
    }

    InitWordList();
}

void CWordMgtword::InitWordList(LPCTSTR strFilter/* =NULL */)
{
    CString strTmp = strFilter;
    _RecordsetPtr pWordPtr;
    CListUI* pList;
    //CListContainerElementUI* pItem;
    //CCheckBoxUI *pChk;
    //CTextUI* pTxt;
    UINT    nId;

    if (strTmp.IsEmpty())
    {
        strTmp = _T("Select Id, 单词, 释义 from 词典 Order by 单词");
    }
    else
    {
        CString strX = strTmp;
        strX.Replace(_T("'"), _T("''"));
        strTmp.Format(_T("Select Id, 单词, 释义 from 词典 where 单词 like '%%%s%%' Order by 单词"),
            strX);
    }

    pWordPtr = m_pParent->GetTablesBySql(_T("%s"), strTmp);
    pList = xFindControl(CListUI, _T("wordmngwordalllist"));
    pList->RemoveAll();
    m_arAllWord.clear();

    if (pWordPtr != NULL)
    {
        CString strId;

        while(!pWordPtr->GetadoEOF())
        {
            strId = GetCollect(pWordPtr, _T("Id"));
            nId = _tstol(strId);
            Add2AllWord(nId, GetCollect(pWordPtr, _T("单词")), GetCollect(pWordPtr, _T("释义")));
            pWordPtr->MoveNext();
        }
    }
}

void CWordMgtword::Add2SelWord(UINT nId, LPCTSTR strWord, bool bNew/* = false*/, int nPos/*=-1*/)
{
    CListUI* pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
    //CListTextElementUI* pItem;
    CListContainerElementUI* pContain;
    //CCheckBoxUI* pChk;
    CTextUI* pTxt;

    tagWord tWord;
    tWord.nId = nId;
    tWord.bNew = bNew;
    tWord.strWord = strWord;
    if (nPos >= 0)
    {
        m_arSelWord.insert(m_arSelWord.begin() + nPos, tWord);
    }
    else
    {
        m_arSelWord.push_back(tWord);
    }
    pContain = new CListContainerElementUI;
    pContain->SetAttribute(_T("height"), _T("25"));
    pTxt = new CTextUI;
    pTxt->ApplyAttributeList(_T("font=\"1\""));
    pTxt->SetText(strWord);
    pContain->Add(pTxt);
    pContain->SetTag(nId);

    if (nPos >= 0)
    {
        pList->AddAt(pContain, nPos);
    }
    else
    {
        pList->Add(pContain);
    }
}

void CWordMgtword::Add2AllWord(UINT nId, LPCTSTR strWord, LPCTSTR strTrans)
{
    CListUI* pList = xFindControl(CListUI, _T("wordmngwordalllist"));
    //CListTextElementUI* pItem;
    CListContainerElementUI* pContain;
    //CCheckBoxUI* pChk;
    CTextUI* pTxt;

    tagWord tWord;
    tWord.nId = nId;
    tWord.strWord = strWord;
    m_arAllWord.push_back(tWord);
    bool bEnabled = (find(m_arSelWord.begin(), m_arSelWord.end(), tWord) == m_arSelWord.end()) ?
        true : false;

    pContain = new CListContainerElementUI;
    pContain->SetAttribute(_T("height"), _T("25"));
    pTxt = new CTextUI;
    pTxt->ApplyAttributeList(_T("font=\"1\""));
    pTxt->SetText(strWord);
    pContain->Add(pTxt);
    pTxt = new CTextUI;
    pTxt->ApplyAttributeList(_T("font=\"1\""));
    pTxt->SetText(strTrans);
    pContain->Add(pTxt);
    pContain->SetTag(nId);
    pContain->SetEnabled(bEnabled);
    pContain->GetItemAt(0)->SetEnabled(bEnabled);
    pContain->GetItemAt(1)->SetEnabled(bEnabled);

    //pTxt = new CTextUI;
    //pTxt->ApplyAttributeList(_T("font=\"1\""));
    //pTxt->SetText(strTrans);
    //pTxt->SetEnabled(bEnabled);
    //pItem->Add(pTxt);

    pList->Add(pContain);
}

BOOL CWordMgtword::mySave()
{
    BOOL bRet = FALSE;
    CString strSql;
    ArrayWords::iterator pw;

    do 
    {
        for (pw = m_arWordRemoved.begin(); pw != m_arWordRemoved.end(); ++pw)
        {
            m_pParent->ExecuteSql(_T("delete from 课文单词 where 年级=%s and 教材=%s and 课文=%s and 单词=%lu"),
                    m_strGId, m_strBId, m_strUId, pw->nId);
        }
        m_arWordRemoved.clear();

        UINT nSeq = 1;
        for (pw = m_arSelWord.begin(); pw != m_arSelWord.end(); ++pw, ++nSeq)
        {
            if (pw->bNew)
            {
                strSql.Format(_T("insert into 课文单词(年级,教材,课文,顺序,单词) values(%s,%s,%s,%lu,%lu)"),
                    m_strGId, m_strBId, m_strUId, nSeq, pw->nId);
            }
            else
            {
                strSql.Format(_T("update 课文单词 set 顺序=%lu where 年级=%s and 教材=%s and 课文=%s and 单词=%lu"),
                    nSeq, m_strGId, m_strBId, m_strUId, pw->nId);
            }
            m_pParent->ExecuteSql(_T("%s"), strSql);
            pw->bNew = false;
            //pw->bModified = false;
        }

        bRet = TRUE;
    } while (0);

    return bRet;
}

void CWordMgtword::myUp(int nItem)
{
    CListUI* pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
    //CListTextElementUI* pItem;
    CListContainerElementUI* pContain;
    CTextUI* pTxt;

    if (nItem <= 0)
    {
        return;
    }

    swap(m_arSelWord[nItem], m_arSelWord[nItem-1]);

    int nAfter = nItem - 1;
    pContain = (CListContainerElementUI*)pList->GetItemAt(nItem);
    pTxt = (CTextUI*)pContain->GetItemAt(0);
    UINT nId = pContain->GetTag();
    //CCheckBoxUI* pChk = (CCheckBoxUI*)pItem->GetItemAt(0);
    //CString strWord = pChk->GetText().GetData();
    CString strWord = pTxt->GetText().GetData();
    
    pContain = new CListContainerElementUI;
    pContain->SetAttribute(_T("height"), _T("25"));
    pTxt = new CTextUI;
    pTxt->ApplyAttributeList(_T("font=\"1\""));
    pTxt->SetText(strWord);
    pContain->SetTag(nId);
    pContain->Add(pTxt);

    pList->RemoveAt(nItem);
    pList->AddAt(pContain, nAfter);
    pList->SelectItem(nAfter);
    pList->SetPos(pList->GetPos());
    pList->EnsureVisible(nAfter);
}

void CWordMgtword::myDown(int nItem)
{
    CListUI* pList = xFindControl(CListUI, _T("wordmngwordselectedlist"));
    //CListTextElementUI* pItem;
    CTextUI* pTxt;
    CListContainerElementUI* pContain;
    if (nItem >= pList->GetCount() - 1)
    {
        return;
    }

    swap(m_arSelWord[nItem], m_arSelWord[nItem+1]);

    int nAfter = nItem + 1;
    pContain = (CListContainerElementUI*)pList->GetItemAt(nItem);
    pTxt = (CTextUI*)pContain->GetItemAt(0);
    tagWord tWord;
    tWord.nId = pContain->GetTag();
    tWord.strWord = pTxt->GetText().GetData();

    pContain = new CListContainerElementUI;
    pContain->SetAttribute(_T("height"), _T("25"));
    pTxt = new CTextUI;
    pTxt->ApplyAttributeList(_T("font=\"1\""));
    pTxt->SetText(tWord.strWord);
    pContain->SetTag(tWord.nId);
    pContain->Add(pTxt);

    pList->RemoveAt(nItem);
    pList->AddAt(pContain, nAfter);
    pList->SelectItem(nAfter);
    pList->SetPos(pList->GetPos());
    pList->EnsureVisible(nAfter);
}
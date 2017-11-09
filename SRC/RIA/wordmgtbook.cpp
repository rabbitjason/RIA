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
#include "RIA.h"
#include "wordmgtbook.h"
#include "DlgInputName.h"
#include "wordmgtword.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"

static
LPCTSTR g_WordListName[] =
{
    _T("wordbookgradelist"),
    _T("wordbooktextbooklist"),
    _T("wordbooktextlist")
};

static
LPCTSTR g_WordEditName[] =
{
    _T("wordbookgradeedit"),
    _T("wordbookbookedit"),
    _T("wordbooktextedit")
};

static
LPCTSTR g_WordInputName[] =
{
    _T("新年级"),
    _T("新教材"),
    _T("新课文")
};

LPCTSTR CWordMgtBook::g_strAttr = _T("height=\"26\" width=\"9999\"");

CWordMgtBook::CWordMgtBook(MainFrame* pParent) : m_pParent(pParent)
{
    m_pCurEdit = NULL;
    m_pCurGradeDat = NULL;
    m_pCurBookDat = NULL;
    m_pCurUnitDat = NULL;
    m_pCurDbDat = NULL;
    m_nCurType = -1;
    m_nCurItem = -1;
    m_bModified = false;
}

CWordMgtBook::~CWordMgtBook()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordMgtBook::GetWindowClassName() const
{
	return _T("RIAWordMgtBookWindow");
}

CControlUI* CWordMgtBook::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordMgtBook::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordMgtBook::GetSkinFile()
{
	return _T("wordmngbook.xml");
}

CDuiString CWordMgtBook::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordMgtBook::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordMgtBook::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordMgtBook::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordMgtBook::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordMgtBook::OnTimer(TNotifyUI& msg)
{
}

void CWordMgtBook::Close(UINT nRet /* = IDOK */)
{
    CListUI* pList;
    CString strId;

    pList = xFindControl(CListUI, g_WordListName[0]);
    if (pList->GetCount() > 0)
    {
        strId = pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData();
    }
    else
    {
        //should never
        strId = _T("0");
    }
    m_pParent->SaveSetup(_T("wordmgt_grade"), strId);

    pList = xFindControl(CListUI, g_WordListName[1]);
    if (pList->GetCount() > 0)
    {
        strId = pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData();
    }
    else
    {
        //should never
        strId = _T("0");
    }
    m_pParent->SaveSetup(_T("wordmgt_book"), strId);

    pList = xFindControl(CListUI, g_WordListName[2]);
    if (pList->GetCount() > 0)
    {
        strId = pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData();
    }
    else
    {
        //should never
        strId = _T("0");
    }
    m_pParent->SaveSetup(_T("wordmgt_unit"), strId);
    __super::Close(nRet);
}

void CWordMgtBook::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordMgtBook::InitWindow()
{
    ASSERT(m_pParent);
    m_bModified = false;
}

//各窗口的初始化在此完成
void CWordMgtBook::OnPrepare(TNotifyUI& msg)
{
    CString strTmp;
    UINT    nId;
    _RecordsetPtr pWordsSetPtr = m_pParent->GetTablesBySql(_T("%s"), _T("Select * from 年级 Order by 顺序"));

    m_arGrade.clear();
    m_nMaxGradeId = 0;

    if (pWordsSetPtr != NULL)
    {
        while(!pWordsSetPtr->GetadoEOF())
        {
            tagGrade g;
            g.strId = GetCollect(pWordsSetPtr, _T("Id"));
            g.strName = GetCollect(pWordsSetPtr, _T("名称"));
            g.strSeq = GetCollect(pWordsSetPtr, _T("顺序"));
            nId = _tstol(g.strId);
            if (m_nMaxGradeId < nId)
            {
                m_nMaxGradeId = nId;
            }

            _RecordsetPtr pBookPtr = m_pParent->GetTablesBySql(_T("Select * from 教材 where 年级=%s Order by 顺序"),
                g.strId);
            if (pBookPtr != NULL)
            {
                while(!pBookPtr->GetadoEOF())
                {
                    tagBOOK b;
                    b.strId = GetCollect(pBookPtr, _T("Id"));
                    b.strName = GetCollect(pBookPtr, _T("名称"));
                    b.strSeq = GetCollect(pBookPtr, _T("顺序"));
                    b.strOrder = GetCollect(pBookPtr, _T("排序"));
                    nId = _tstol(b.strId);
                    if (g.nMaxBookId < nId)
                    {
                        g.nMaxBookId = nId;
                    }

                    _RecordsetPtr pUnitPtr = m_pParent->GetTablesBySql(_T("Select * from 课文 where 教材=%s and 年级=%s Order by 顺序,Id"),
                        b.strId, g.strId);
                    if (NULL != pUnitPtr)
                    {
                        while(!pUnitPtr->GetadoEOF())
                        {
                            tagUNIT u;
                            u.strId = GetCollect(pUnitPtr, _T("Id"));
                            u.strName = GetCollect(pUnitPtr, _T("名称"));
                            u.strSeq = GetCollect(pUnitPtr, _T("顺序"));
                            nId = _tstol(u.strId);
                            if (b.nMaxUnitId < nId)
                            {
                                b.nMaxUnitId = nId;
                            }

                            b.arUnit.push_back(u);

                            pUnitPtr->MoveNext();
                        }
                    }

                    g.arBooks.push_back(b);

                    pBookPtr->MoveNext();
                }
            }
            m_arGrade.push_back(g);
            pWordsSetPtr->MoveNext();
        }
    }

    InitControls(msg);
}

void CWordMgtBook::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CListUI* pList;
    //CEditUI* pEdt;
    //CListLabelElementUI* pItem;
    ArrayBOOK::iterator ptrBook;
    ArrayUnit::iterator ptrUnit;
    int n;

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
        //年级
        else if (strSender == _T("wordbookgradedelbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                strTmp.Format(_T("确定要删除年级 %s 吗？\r\n%s 对应的教材与课文都将会被删除。"),
                    m_arGrade[n].strName, m_arGrade[n].strName);
                if (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    if (!m_arGrade[n].bNew)
                    {
                        m_arGradeRemoved.push_back(m_arGrade[n]);
                    }

                    if (_tstol(m_arGrade[n].strId) == m_nMaxGradeId)
                    {
                        m_nMaxGradeId--;
                    }
                    m_arGrade.erase(m_arGrade.begin() + n);
                    pList->RemoveAt(n);
                    m_bModified = true;
                }
            }
        }
        else if (strSender == _T("wordbookgradeaddbtn"))
        {
#if 1
            myAddModifyItem(0, -1);
#else
            pList = xFindControl(CListUI, _T("wordbookgradelist"));
            pEdt = xFindControl(CEditUI, _T("wordbookgradeedit"));
            
            tagGrade tgGrad;
            tgGrad.bNew = true;
            m_arGrade.push_back(tgGrad);
            pItem = new CListLabelElementUI;
            pItem->SetText(_T("新年级"));
            pList->Add(pItem);
            n = pList->GetCount() - 1;
            pList->SelectItem(n);
            myEditItem(0, n, pEdt);
#endif
        }
        else if (strSender == _T("wordbookgradeupbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                ItemUp(pList, n, m_arGrade);
            }
        }
        else if (strSender == _T("wordbookgradedownbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                ItemDown(pList, n, m_arGrade);
            }
        }
        //教材
        else if (strSender == _T("wordbookbookdelbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                tagGrade& tgGrade = m_arGrade[pList->GetCurSel()];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    n = pList->GetCurSel();
                    strTmp.Format(_T("确定要删除教材 %s 吗？\r\n%s 对应的课文都将会被删除。"),
                        tgGrade.arBooks[n].strName, tgGrade.arBooks[n].strName);
                    if (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES)
                    {
                        if (!tgGrade.arBooks[n].bNew)
                        {
                            tgGrade.arBooksRemoved.push_back(tgGrade.arBooks[n]);
                        }
                        if (_tstol(tgGrade.arBooks[n].strId) == tgGrade.nMaxBookId)
                        {
                            tgGrade.nMaxBookId--;
                        }
                        tgGrade.arBooks.erase(tgGrade.arBooks.begin() + n);
                        pList->RemoveAt(n);
                        m_bModified = true;
                    }
                }
            }
        }
        else if (strSender == _T("wordbookbookaddbtn"))
        {
#if 1
            myAddModifyItem(1, -1);
#else
            pList = xFindControl(CListUI, _T("wordbookgradelist"));
            n = pList->GetCurSel();
            tagGrade& tg = m_arGrade[n];
            pList = xFindControl(CListUI, _T("wordbooktextbooklist"));
            n = pList->GetCurSel();

            pEdt = xFindControl(CEditUI, _T("wordbookbookedit"));
            tagBOOK tgBook;
            tgBook.bNew = true;
            tg.arBooks.push_back(tgBook);
            pItem = new CListLabelElementUI;
            pItem->SetText(_T("新教材"));
            pList->Add(pItem);
            n = pList->GetCount() - 1;
            pList->SelectItem(n);
            myEditItem(1, n, pEdt);
#endif
        }
        else if (strSender == _T("wordbookbookupbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                tagGrade& tgGrade = m_arGrade[n];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    n = pList->GetCurSel();
                    ItemUp(pList, n, tgGrade.arBooks);
                }
            }
        }
        else if (strSender == _T("wordbookbookdownbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                tagGrade& tgGrade = m_arGrade[n];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    n = pList->GetCurSel();
                    ItemDown(pList, n, tgGrade.arBooks);
                }
            }
        }
        //课本
        else if (strSender == _T("wordbooktextdelbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                tagGrade& arGrade = m_arGrade[pList->GetCurSel()];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    tagBOOK& tagBooks = arGrade.arBooks[pList->GetCurSel()];
                    pList = xFindControl(CListUI, g_WordListName[2]);
                    if (pList->GetCount() > 0)
                    {
                        n = pList->GetCurSel();
                        strTmp.Format(_T("确定要删除课本 %s 吗？"), tagBooks.arUnit[n].strName);
                        if (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES)
                        {
                            if (!tagBooks.arUnit[n].bNew)
                            {
                                tagBooks.arUnitRemoved.push_back(tagBooks.arUnit[n]);
                            }
                            if (_tstol(tagBooks.arUnit[n].strId) == tagBooks.nMaxUnitId)
                            {
                                tagBooks.nMaxUnitId--;
                            }
                            tagBooks.arUnit.erase(tagBooks.arUnit.begin() + n);
                            pList->RemoveAt(n);
                            m_bModified = true;
                        }
                    }
                }
            }
        }
        else if (strSender == _T("wordbooktextaddbtn"))
        {
#if 1
            myAddModifyItem(2, -1);
#else
            pList = xFindControl(CListUI, _T("wordbookgradelist"));
            n = pList->GetCurSel();
            tagGrade& tg = m_arGrade[n];
            pList = xFindControl(CListUI, _T("wordbooktextbooklist"));
            n = pList->GetCurSel();
            tagBOOK& tb = tg.arBooks[n];
            pList = xFindControl(CListUI, _T("wordbooktextlist"));
            n = pList->GetCurSel();

            pEdt = xFindControl(CEditUI, _T("wordbooktextedit"));
            tagUNIT tgUnit;
            tgUnit.bNew = true;
            tb.arUnit.push_back(tgUnit);
            pItem = new CListLabelElementUI;
            pItem->SetText(_T("新课文"));
            pList->Add(pItem);
            n = pList->GetCount() - 1;
            pList->SelectItem(n);
            myEditItem(2, n, pEdt);
#endif
        }
        else if (strSender == _T("wordbooktextupbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                tagGrade& tgGrade = m_arGrade[n];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    n = pList->GetCurSel();
                    tagBOOK& tgBook = tgGrade.arBooks[n];
                    pList = xFindControl(CListUI, g_WordListName[2]);
                    if (pList->GetCount() > 0)
                    {
                        n = pList->GetCurSel();
                        ItemUp(pList, n, tgBook.arUnit);
                    }
                }
            }
        }
        else if (strSender == _T("wordbooktextdownbtn"))
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            if (pList->GetCount() > 0)
            {
                n = pList->GetCurSel();
                tagGrade& tgGrade = m_arGrade[n];
                pList = xFindControl(CListUI, g_WordListName[1]);
                if (pList->GetCount() > 0)
                {
                    n = pList->GetCurSel();
                    tagBOOK& tgBook = tgGrade.arBooks[n];
                    pList = xFindControl(CListUI, g_WordListName[2]);
                    if (pList->GetCount() > 0)
                    {
                        n = pList->GetCurSel();
                        ItemDown(pList, n, tgBook.arUnit);
                    }
                }
            }
        }
        //
        else if (strSender == _T("wordbookcfgwordbtn"))
        {
            CString strID[3];
            if (m_bModified)
            {
                if (AfxMessageBox(_T("进行单词编辑需要保存年级教材表。现在就保存吗？"),
                        MB_YESNO | MB_ICONQUESTION) != IDYES)
                {
                    return;
                }
                if (!myCheckAll())
                {
                    return;
                }
                mySaveAll();
            }

            for (n = 0; n < _countof(g_WordListName); n++)
            {
                pList = xFindControl(CListUI, g_WordListName[n]);
                ASSERT(pList->GetCount() > 0);
                strID[n] = pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData();
            }

            CWordMgtword* pWW = new CWordMgtword(m_pParent);
            pWW->m_strGId = strID[0];
            pWW->m_strBId = strID[1];
            pWW->m_strUId = strID[2];
            ShowWindow(false);
            n = m_pParent->OpenModalWnd(pWW, _T("单词编辑"));
            if (IDCANCEL == n)
            {
                Close(IDCANCEL);
            }
            else
            {
                ShowWindow(true);
            }
        }
        else if (strSender == _T("wordbookokbtn"))
        {
            if (myCheckAll())
            {
                if (mySaveAll())
                {
                    //Close(IDOK);
                }
            }
        }
	}
    else if (sType == DUI_MSGTYPE_ITEMACTIVATE)
    {
        POINT pt = m_PaintManager.GetMousePos();
        CControlUI* pCtrl = m_PaintManager.FindControl(pt);
        if (pCtrl)
        {
            pCtrl = pCtrl->GetParent();
            if (pCtrl)
            {
                pCtrl = pCtrl->GetParent();
                if (pCtrl)
                {
                    strTmp = pCtrl->GetName().GetData();
                    int nType = -1;
                    if (g_WordListName[0] == strTmp)
                    {
                        nType = 0;
                    }
                    else if (g_WordListName[1] == strTmp)
                    {
                        nType = 1;
                    }
                    else if (g_WordListName[2] == strTmp)
                    {
                        nType = 2;
                    }

                    //pEdt = xFindControl(CEditUI, g_WordEditName[nType]);
                    pList = static_cast<CListUI*>(pCtrl);
                    n = pList->GetCurSel();
                    //pItem = (CListLabelElementUI*)pList->GetItemAt(n);
                    //myEditItem(nType, n, pEdt);
                    myAddModifyItem(nType, n);
                }
            }
        }
    }
    else if (sType == DUI_MSGTYPE_RETURN)
    {
        if (_T("wordbookgradeedit") == strSender ||
            _T("wordbookbookedit") == strSender ||
            _T("wordbooktextedit") == strSender)
        {
            //myUpdateItem();
        }
    }
    else if (sType == DUI_MSGTYPE_KILLFOCUS)
    {
        if (_T("wordbookgradeedit") == strSender ||
            _T("wordbookbookedit") == strSender ||
            _T("wordbooktextedit") == strSender)
        {
            //myUpdateItem();
        }
    }
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        pList = static_cast<CListUI*>(msg.pSender);
        if (strSender == g_WordListName[0])
        {
            InitBookList(pList->GetCurSel());
        }
        else if (strSender == g_WordListName[1])
        {
            CListUI* plstGrade = xFindControl(CListUI, g_WordListName[0]);
            InitUnitList(plstGrade->GetCurSel(), pList->GetCurSel());
        }
        else if (strSender == g_WordListName[2])
        {
        }
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
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

LRESULT CWordMgtBook::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordMgtBook::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

//static LPCTSTR g_strAttr = _T("height=\"26\" width=\"9999\"");

void CWordMgtBook::InitControls(TNotifyUI& msg)
{
    CString strTmp;
    CListLabelElementUI* pItem;
    ArrayGrade::const_iterator p;
    CListUI* pList = xFindControl(CListUI, g_WordListName[0]);
    CEditUI* pEdt;

    pEdt = xFindControl(CEditUI, _T("wordbookgradeedit"));
    if (pEdt)
    {
        pEdt->SetVisible(false);
    }
    pEdt = xFindControl(CEditUI, _T("wordbookbookedit"));
    if (pEdt)
    {
        pEdt->SetVisible(false);
    }
    pEdt = xFindControl(CEditUI, _T("wordbooktextedit"));
    if (pEdt)
    {
        pEdt->SetVisible(false);
    }

    CString strGrade = m_pParent->ReadSetup(_T("wordmgt_grade"));
    CString strBook = m_pParent->ReadSetup(_T("wordmgt_book"));
    CString strUnit = m_pParent->ReadSetup(_T("wordmgt_unit"));

    pList->RemoveAll();
    int nSel = 0, nIdx = 0;
    for (p = m_arGrade.begin(); p != m_arGrade.end(); ++p)
    {
        pItem = new CListLabelElementUI;
        //指定高度和宽度，能够提高效率
        pItem->ApplyAttributeList(g_strAttr);
        pItem->SetText(p->strName);
        pItem->SetUserData(p->strId);
        pList->Add(pItem);
        if (strGrade == p->strId)
        {
            nSel = nIdx;
        }
        nIdx++;
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(nSel);
        pList->SetPos(pList->GetPos());
        pList->EnsureVisible(nSel);
    }

    InitBookList(nSel, strBook);
    pList = xFindControl(CListUI, g_WordListName[1]);
    InitUnitList(nSel, pList->GetCurSel(), strUnit);
}

void CWordMgtBook::InitBookList(UINT nGradeIndex, LPCTSTR lpstrBook/* =NULL */)
{
    CString strTmp;
    CListLabelElementUI* pItem;
    ArrayBOOK::const_iterator p;
    const ArrayBOOK& arBooks = m_arGrade[nGradeIndex].arBooks;
    CListUI* pList = xFindControl(CListUI, g_WordListName[1]);
    pList->RemoveAll();

    //教材可能为空（如新增教材情况下），清除
    CListUI* pUnit = xFindControl(CListUI, g_WordListName[2]);
    pUnit->RemoveAll();
    int nSel = 0, nIdx = 0;
    for (p = arBooks.begin(); p != arBooks.end(); ++p)
    {
        pItem = new CListLabelElementUI;
        //指定高度和宽度，能够提高效率
        pItem->ApplyAttributeList(g_strAttr);
        pItem->SetText(p->strName);
        pItem->SetUserData(p->strId);
        pList->Add(pItem);
        if (lpstrBook && (p->strId == lpstrBook))
        {
            nSel = nIdx;
        }
        nIdx++;
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(nSel);
        pList->SetPos(pList->GetPos());
        pList->EnsureVisible(nSel);
    }
}

void CWordMgtBook::InitUnitList(UINT nGradeId, UINT nBookId, LPCTSTR lpstrUnit/* =NULL */)
{
    CString strTmp;
    CListLabelElementUI* pItem;
    ArrayUnit::const_iterator p;
    const ArrayUnit& arUnits = m_arGrade[nGradeId].arBooks[nBookId].arUnit;
    CListUI* pList = xFindControl(CListUI, g_WordListName[2]);
    
    pList->RemoveAll();
    int nSel = 0, nIdx = 0;
    for (p = arUnits.begin(); p != arUnits.end(); ++p)
    {
        pItem = new CListLabelElementUI;
        //指定高度和宽度，能够提高效率
        pItem->ApplyAttributeList(g_strAttr);
        pItem->SetText(p->strName);
        pItem->SetUserData(p->strId);
        pList->Add(pItem);
        if (lpstrUnit && (p->strId == lpstrUnit))
        {
            nSel = nIdx;
        }
        nIdx++;
    }

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(nSel);
        pList->SetPos(pList->GetPos());
        pList->EnsureVisible(nSel);
    }
}

//void CWordMgtBook::myUpdateItem()
//{
//    CString strName;
//    CListUI* pList;
//    CListUI* pCurList = xFindControl(CListUI, g_WordListName[m_nCurType]);;
//    int n;
//    ASSERT(m_pCurEdit);
//
//    if (m_pCurEdit->IsVisible())
//    {
//        strName = m_pCurEdit->GetText();
//        if (strName.IsEmpty())
//        {
//            AfxMessageBox(_T("名字不能为空。"));
//            pCurList->SelectItem(m_nCurItem);
//            m_pCurEdit->SetFocus();
//            m_pCurEdit->SetSel(0, 0);
//            return;
//        }
//
//        if (0 == m_nCurType)
//        {
//            ArrayGrade::const_iterator pg;
//            for (pg = m_arGrade.begin(); pg != m_arGrade.end(); ++pg)
//            {
//                if (m_pCurGradeDat == &(*pg))
//                {
//                    continue;
//                }
//                if (pg->strName == strName)
//                {
//                    AfxMessageBox(_T("名字重复。"));
//                    pCurList->SelectItem(m_nCurItem);
//                    m_pCurEdit->SetFocus();
//                    m_pCurEdit->SetSel(0, 0);
//                    return;
//                }
//            }
//            m_pCurGradeDat->bModified = true;
//            m_pCurGradeDat->strName = strName;
//        }
//        else if (1 == m_nCurType)
//        {
//            pList = xFindControl(CListUI, _T("wordbookgradelist"));
//            n = pList->GetCurSel();
//            tagGrade& tg = m_arGrade[n];
//
//            ArrayBOOK::const_iterator pb;
//            for (pb = tg.arBooks.begin(); pb != tg.arBooks.end(); ++pb)
//            {
//                if (&(*pb) == m_pCurBookDat)
//                {
//                    continue;
//                }
//                if (pb->strName == strName)
//                {
//                    AfxMessageBox(_T("名字重复。"));
//                    pCurList->SelectItem(m_nCurItem);
//                    m_pCurEdit->SetFocus();
//                    m_pCurEdit->SetSel(0, 0);
//                    return;
//                }
//            }
//
//            m_pCurBookDat->bModified = true;
//            m_pCurBookDat->strName = strName;
//        }
//        else if (2 == m_nCurType)
//        {
//            pList = xFindControl(CListUI, _T("wordbookgradelist"));
//            n = pList->GetCurSel();
//            tagGrade& tg = m_arGrade[n];
//            pList = xFindControl(CListUI, _T("wordbooktextbooklist"));
//            n = pList->GetCurSel();
//            tagBOOK& tb = tg.arBooks[n];
//
//            ArrayUnit::const_iterator pu;
//            for (pu = tb.arUnit.begin(); pu != tb.arUnit.end(); ++pu)
//            {
//                if (m_pCurUnitDat == &(*pu))
//                {
//                    continue;
//                }
//                if (pu->strName == strName)
//                {
//                    AfxMessageBox(_T("名字重复。"));
//                    pCurList->SelectItem(m_nCurItem);
//                    m_pCurEdit->SetFocus();
//                    m_pCurEdit->SetSel(0, 0);
//                    return;
//                }
//            }
//
//            m_pCurUnitDat->bModified = true;
//            m_pCurUnitDat->strName = strName;
//        }
//        pCurList->SetEnabled(true);
//        CListLabelElementUI* pItem = (CListLabelElementUI*)pCurList->GetItemAt(m_nCurItem);
//        pItem->SetText(strName);
//        m_pCurEdit->SetVisible(false);
//    }
//}

void CWordMgtBook::myEditItem(int nType, int nItem, CEditUI* pEdit)
{
    int n;
    CListUI* pList;
    CListLabelElementUI* pItem;
    RECT rctPos;

    if (0 == nType)
    {
        m_pCurGradeDat = &m_arGrade[nItem];
    }
    else if (1 == nType)
    {
        pList = xFindControl(CListUI, g_WordListName[0]);
        n = pList->GetCurSel();
        m_pCurBookDat = &m_arGrade[n].arBooks[nItem];
    }
    else if (2 == nType)
    {
        pList = xFindControl(CListUI, g_WordListName[0]);
        n = pList->GetCurSel();
        tagGrade& tg = m_arGrade[n];
        pList = xFindControl(CListUI, g_WordListName[1]);
        n = pList->GetCurSel();
        tagBOOK& tb = tg.arBooks[n];

        m_pCurUnitDat = &tb.arUnit[nItem];
    }
#ifdef _DEBUG
    else
    {
        ASSERT(FALSE);
    }
#endif

    pList = xFindControl(CListUI, g_WordListName[nType]);
    //调此函数，计算新加Item的Pos
    pList->SetPos(pList->GetPos());
    pList->EnsureVisible(nItem);
    pList->SetEnabled(false);
    pItem = (CListLabelElementUI*)pList->GetItemAt(nItem);
    rctPos = pItem->GetPos();
    pEdit->SetPos(rctPos);
    pEdit->SetText(pItem->GetText().GetData());
    pEdit->SetVisible(true);
    pEdit->SetFocus();
    pEdit->SetSel(0, 0);
    //pEdt->SetSelAll();

    m_pCurEdit = pEdit;
    m_nCurType = nType;
    m_nCurItem = nItem;
}

void CWordMgtBook::myAddModifyItem(int nType, int nItem)
{
    CListUI* pCurList = xFindControl(CListUI, g_WordListName[nType]);
    CListUI* pList;
    CListLabelElementUI* pItem;
    int n;

    m_nCurType = nType;
    m_nCurItem = nItem;
    if (nItem >= 0)
    {
        if (0 == nType)
        {
            m_pCurDbDat = &m_arGrade[nItem];
        }
        else if (1 == nType)
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            n = pList->GetCurSel();
            m_pCurDbDat = &m_arGrade[n].arBooks[nItem];
        }
        else if (2 == nType)
        {
            pList = xFindControl(CListUI, g_WordListName[0]);
            n = pList->GetCurSel();
            tagGrade& tg = m_arGrade[n];
            pList = xFindControl(CListUI, g_WordListName[1]);
            n = pList->GetCurSel();
            tagBOOK& tb = tg.arBooks[n];

            m_pCurDbDat = &tb.arUnit[nItem];
        }
    }
    else
    {
        m_pCurDbDat = NULL;
    }

    CDlgInputName dlgname(this, CWnd::FromHandle(m_PaintManager.GetPaintWindow()));
    if (nItem >= 0)
    {
        pItem = (CListLabelElementUI*)pCurList->GetItemAt(nItem);
        dlgname.m_strInputName = pItem->GetText().GetData();
    }
    else
    {
        dlgname.m_strInputName = g_WordInputName[nType];
    }
    if (dlgname.DoModal() == IDOK)
    {
        if (nItem >= 0) //修改
        {
            m_pCurDbDat->bModified = true;
            m_pCurDbDat->strName = dlgname.m_strInputName;
            pItem = (CListLabelElementUI*)pCurList->GetItemAt(nItem);
            pItem->SetText(dlgname.m_strInputName);
        }
        else        //添加
        {
            CString strId;
            int nCur = pCurList->GetCurSel();
            if (0 == nType)
            {
                tagGrade tgGrad;
                tgGrad.bNew = true;
                tgGrad.strName = dlgname.m_strInputName;
                m_nMaxGradeId++;
                tgGrad.strId.Format(_T("%lu"), m_nMaxGradeId);
                strId = tgGrad.strId;
                if (nCur < 0)
                {
                    m_arGrade.push_back(tgGrad);
                }
                else
                {
                    m_arGrade.insert(m_arGrade.begin() + nCur, tgGrad);
                }
            }
            else if (1 == nType)
            {
                pList = xFindControl(CListUI, g_WordListName[0]);
                n = pList->GetCurSel();
                tagBOOK tb;
                tb.bNew = true;
                tb.strName = dlgname.m_strInputName;
                m_arGrade[n].nMaxBookId++;
                tb.strId.Format(_T("%lu"), m_arGrade[n].nMaxBookId);
                strId = tb.strId;
                if (nCur < 0)
                {
                    m_arGrade[n].arBooks.push_back(tb);
                }
                else
                {
                    m_arGrade[n].arBooks.insert(m_arGrade[n].arBooks.begin() + nCur, tb);
                }
            }
            else if (2 == nType)
            {
                pList = xFindControl(CListUI, g_WordListName[0]);
                n = pList->GetCurSel();
                tagGrade& tg = m_arGrade[n];
                pList = xFindControl(CListUI, g_WordListName[1]);
                n = pList->GetCurSel();
                tagBOOK& tb = tg.arBooks[n];

                tagUNIT tu;
                tu.bNew = true;
                tu.strName = dlgname.m_strInputName;
                tb.nMaxUnitId++;
                tu.strId.Format(_T("%lu"), tb.nMaxUnitId);
                strId = tu.strId;
                if (nCur < 0)
                {
                    tb.arUnit.push_back(tu);
                }
                else
                {
                    tb.arUnit.insert(tb.arUnit.begin() + nCur, tu);
                }
            }

            pItem = new CListLabelElementUI;
            pItem->SetText(dlgname.m_strInputName);
            pItem->SetUserData(strId);
            if (nCur < 0)
            {
                pCurList->Add(pItem);
                nCur = 0;
            }
            else
            {
                pCurList->AddAt(pItem, nCur);
            }
            pCurList->SetPos(pCurList->GetPos());
            pCurList->SelectItem(nCur);
            pCurList->EnsureVisible(nCur);
        }

        m_bModified = true;
    }
}

BOOL CWordMgtBook::myCheckName(LPCTSTR strName)
{
    BOOL bOK = TRUE;
    CListUI* pList;
    int n;

    if (0 == m_nCurType)
    {
        ArrayGrade::const_iterator pg;
        for (pg = m_arGrade.begin(); pg != m_arGrade.end(); ++pg)
        {
            if (m_pCurDbDat == &(*pg))
            {
                continue;
            }
            if (pg->strName == strName)
            {
                return FALSE;
            }
        }
    }
    else if (1 == m_nCurType)
    {
        pList = xFindControl(CListUI, g_WordListName[0]);
        n = pList->GetCurSel();
        tagGrade& tg = m_arGrade[n];

        ArrayBOOK::const_iterator pb;
        for (pb = tg.arBooks.begin(); pb != tg.arBooks.end(); ++pb)
        {
            if (&(*pb) == m_pCurDbDat)
            {
                continue;
            }
            if (pb->strName == strName)
            {
                return FALSE;
            }
        }
    }
    else if (2 == m_nCurType)
    {
        pList = xFindControl(CListUI, g_WordListName[0]);
        n = pList->GetCurSel();
        tagGrade& tg = m_arGrade[n];
        pList = xFindControl(CListUI, g_WordListName[1]);
        n = pList->GetCurSel();
        tagBOOK& tb = tg.arBooks[n];

        ArrayUnit::const_iterator pu;
        for (pu = tb.arUnit.begin(); pu != tb.arUnit.end(); ++pu)
        {
            if (m_pCurDbDat == &(*pu))
            {
                continue;
            }
            if (pu->strName == strName)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CWordMgtBook::mySaveAll()
{
    ::CWaitCursor waitit;

    CString strSql, strTmp;
    ArrayGrade::iterator pg;
    ArrayBOOK::iterator pb;
    ArrayUnit::iterator pu;
    //_RecordsetPtr pRecdPtr;
    BOOL bRet = FALSE;

    do 
    {
        if (!m_bModified)
        {
            bRet = TRUE;
            break;
        }

        for (pg = m_arGradeRemoved.begin(); pg != m_arGradeRemoved.end(); ++pg)
        {
            m_pParent->ExecuteSql(_T("delete from 年级 where Id=%s"),
                pg->strId);
            //删除教材/课文
            m_pParent->ExecuteSql(_T("delete from 教材 where 年级=%s"),
                pg->strId);
#if 1
            m_pParent->ExecuteSql(_T("delete from 课文 where 年级=%s"),
                pg->strId);
#else
            pRecdPtr = m_pParent->GetTablesBySql(_T("select Id from 教材 where 年级=%s"),
                pg->strId);
            if (pRecdPtr)
            {
                while (!pRecdPtr->GetadoEOF())
                {
                    m_pParent->ExecuteSql(_T("delete from 课文 where 年级=%s and 教材=%s"),
                        pg->strId, GetCollect(pRecdPtr, _T("Id")));
                    pRecdPtr->MoveNext();
                }
                pRecdPtr->Close();
                pRecdPtr.Release();
            }
#endif
            //课文单词表
            m_pParent->ExecuteSql(_T("delete from 课文单词 where 年级=%s"),
                pg->strId);
        }
        m_arGradeRemoved.clear();

        UINT nSeq = 1;
        for (pg = m_arGrade.begin(); pg != m_arGrade.end(); ++pg, ++nSeq)
        {
            strTmp = pg->strName;
            strTmp.Replace(_T("'"), _T("''"));
            //保存年级
            if (pg->bNew)
            {
                strSql.Format(_T("INSERT INTO 年级(Id,名称,顺序) Values (%s,'%s',%lu)"),
                    pg->strId, strTmp, nSeq);
            }
            else/* if (pg->bModified)*/
            {
                strSql.Format(_T("Update 年级 set 名称='%s', 顺序=%lu where Id=%s"),
                    strTmp, nSeq, pg->strId);
            }
            m_pParent->ExecuteSql(_T("%s"), strSql);
            pg->bNew = false;
            pg->bModified = false;

            //保存年级下的教材
            for (pb = pg->arBooksRemoved.begin(); pb != pg->arBooksRemoved.end(); ++pb)
            {
                m_pParent->ExecuteSql(_T("delete from 教材 where Id=%s and 年级=%s"),
                    pb->strId, pg->strId);
                m_pParent->ExecuteSql(_T("delete from 课文 where 教材=%s and 年级=%s"),
                    pb->strId, pg->strId);
                //课文单词表
                m_pParent->ExecuteSql(_T("delete from 课文单词 where 年级=%s and 教材=%s"),
                    pg->strId, pb->strId);
            }
            pg->arBooksRemoved.clear();

            UINT nSeqB = 1;
            for (pb = pg->arBooks.begin(); pb != pg->arBooks.end(); ++pb, ++nSeqB)
            {
                strTmp = pb->strName;
                strTmp.Replace(_T("'"), _T("''"));
                if (pb->bNew)
                {
                    strSql.Format(_T("INSERT INTO 教材(Id,年级,名称,顺序) Values (%s,%s,'%s',%lu)"),
                        pb->strId, pg->strId, strTmp, nSeqB);
                }
                else/* if (pb->bModified)*/
                {
                    strSql.Format(_T("Update 教材 set 名称='%s',顺序=%lu where Id=%s and 年级=%s"),
                        strTmp, nSeqB, pb->strId, pg->strId);

                }
                m_pParent->ExecuteSql(_T("%s"), strSql);
                pb->bNew = false;
                pb->bModified = false;

                //保存教材下的课文
                for (pu = pb->arUnitRemoved.begin(); pu != pb->arUnitRemoved.end(); ++pu)
                {
                    m_pParent->ExecuteSql(_T("delete from 课文 where Id=%s and 年级=%s and 教材=%s"),
                        pu->strId, pg->strId, pb->strId);
                    //把课文单词表中，属于此年级、教材和课文的单词也删除
                    m_pParent->ExecuteSql(_T("delete from 课文单词 where 年级=%s and 教材=%s and 课文=%s"),
                        pg->strId, pb->strId, pu->strId);
                }
                pb->arUnitRemoved.clear();

                UINT nSeqU = 1;
                for (pu = pb->arUnit.begin(); pu != pb->arUnit.end(); ++pu, ++nSeqU)
                {
                    strTmp = pu->strName;
                    strTmp.Replace(_T("'"), _T("''"));
                    if (pu->bNew)
                    {
                        strSql.Format(_T("INSERT INTO 课文(Id,名称,年级,教材,顺序) Values (%s,'%s',%s,%s,%lu)"),
                                         pu->strId, strTmp, pg->strId, pb->strId, nSeqU);
                    }
                    else/* if (pb->bModified)*/
                    {
                        strSql.Format(_T("Update 课文 set 名称='%s',顺序=%lu where Id=%s and 年级=%s and 教材=%s"),
                            strTmp, nSeqU, pu->strId, pg->strId, pb->strId);

                    }
                    m_pParent->ExecuteSql(_T("%s"), strSql);
                    pu->bNew = false;
                    pu->bModified = false;
                }
            }
        }

        bRet = TRUE;
        m_bModified = false;
    } while (0);

    AfxMessageBox(_T("数据保存成功。"));
    return TRUE;
}

BOOL CWordMgtBook::myCheckAll()
{
    BOOL bRet = FALSE;

    do 
    {
        CString strSql, strTmp;
        ArrayGrade::const_iterator pg;
        ArrayBOOK::const_iterator pb;
        ArrayUnit::const_iterator pu;
        CListUI* pList;

        for (pg = m_arGrade.begin(); pg != m_arGrade.end(); ++pg)
        {
            if (pg->arBooks.size() <= 0)
            {
                MyAfxMessageBox(_T("年级 %s 的教材为空，请添加教材。"),
                    pg->strName);
                pList = xFindControl(CListUI, g_WordListName[0]);
                pList->SelectItem(pg - m_arGrade.begin());
                break;
            }

            //年级下的教材
            bool bbad = false;
            for (pb = pg->arBooks.begin(); pb != pg->arBooks.end(); ++pb)
            {
                if (pb->arUnit.size() <= 0)
                {
                    MyAfxMessageBox(_T("%s>%s的课文为空，请添加课文。"),
                        pg->strName, pb->strName);
                    pList = xFindControl(CListUI, g_WordListName[0]);
                    pList->SelectItem(pg - m_arGrade.begin());
                    pList = xFindControl(CListUI, g_WordListName[1]);
                    pList->SelectItem(pb - pg->arBooks.begin());
                    bbad = true;
                }
            }
            if (bbad)
            {
                break;
            }
        }
        if (pg != m_arGrade.end())
        {
            break;
        }
        bRet = TRUE;

    } while (0);

    return bRet;
}

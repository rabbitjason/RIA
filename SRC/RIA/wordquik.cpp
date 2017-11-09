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
#include "wordaddnew.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "wordquik.h"
#include "main_frame.h"

CWordQuikselect::CWordQuikselect(MainFrame* pParent) : m_pParent(pParent), m_bInMsg(false)
{}

CWordQuikselect::~CWordQuikselect()
{
	//PostQuitMessage(0);
}

LPCTSTR CWordQuikselect::GetWindowClassName() const
{
	return _T("RIAWordQuikselectWindow");
}

CControlUI* CWordQuikselect::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CWordQuikselect::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CWordQuikselect::GetSkinFile()
{
	return _T("wordquikselect.xml");
}

CDuiString CWordQuikselect::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CWordQuikselect::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CWordQuikselect::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CWordQuikselect::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordQuikselect::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWordQuikselect::OnTimer(TNotifyUI& msg)
{
}

void CWordQuikselect::OnExit(TNotifyUI& msg)
{
	Close();
}

void CWordQuikselect::InitWindow()
{
    ASSERT(m_pParent);
}

//各窗口的初始化在此完成
void CWordQuikselect::OnPrepare(TNotifyUI& msg)
{
    InitControls(msg);
}

void CWordQuikselect::Notify(TNotifyUI& msg)
{
    TRACE(_T("CWordQuikselect,type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CString strTmp;
    CListUI* pList;
    //CEditUI* pEdt;
    //CButtonUI* pBtn;
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
//    CListLabelElementUI* pElement;
    CListContainerElementUI* pItem;
    CCheckBoxUI* pChk;
    int i, n;

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
        else if (strSender == _T("wordquikokbtn"))
        {
            pCombo = xFindControl(CComboUI, _T("wordquikgradecmb"));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可用的年级。"));
                return;
            }
            if (pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData() != _T("0"))
            {
                m_pParent->SaveSetup(_T("word_type"), 0);

                m_pParent->SaveSetup(_T("word_grade"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordquiktextbookcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的教材。"));
                    return;
                }
                m_pParent->SaveSetup(_T("word_book"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordquiktextcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的课文。"));
                    return;
                }
                m_pParent->SaveSetup(_T("word_unit"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                m_pParent->SaveSetup(_T("word_nwb"), 0);

                /*
                word_group的保存规则：
                以逗号分隔，第一个保存是否全部（1:全部，0:部分）；
                后面：每10个一组，如果显示则为1，否则为0
                */
                pList = xFindControl(CListUI, _T("wordquikgrouplist"));
                n = pList->GetCount();
                if (n > 0)
                {
                    //CString strSort = m_pParent->ReadSetup(_T("word_sort"));
                    pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(0));
                    pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                    if (pChk->GetCheck())
                    {
                        strTmp = _T("1");
                    }
                    else
                    {
                        strTmp = _T("0");
                        for (i = 1; i < n; i++)
                        {
                            pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
                            pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                            //if (_T("0") == strSort)
                            //{
                            //    if (pChk->GetCheck())
                            //    {
                            //        strTmp += _T(",");
                            //        strTmp += pChk->GetUserData().GetData();
                            //    }
                            //}
                            //else
                            {
                                strTmp += _T(",");
                                strTmp += pChk->GetCheck() ? _T("1") : _T("0");
                            }
                        }
                    }
                    TRACE(_T("save word_group:%s\n"), strTmp);
                    m_pParent->SaveSetup(_T("word_group"), strTmp);

                    pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
                    m_pParent->SaveSetup(_T("word_sort"), pCombo->IsEnabled() ? pCombo->GetCurSel() : 0);
                }
            }
            else
            {
                m_pParent->SaveSetup(_T("word_type"), 1);

                m_pParent->SaveSetup(_T("word_grade"), 0);
                m_pParent->SaveSetup(_T("word_book"), 0);
                m_pParent->SaveSetup(_T("word_unit"), 0);
                m_pParent->SaveSetup(_T("word_group"), _T("1"));
                m_pParent->SaveSetup(_T("word_sort"), 0);

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordquiktextbookcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的生词本。"));
                    return;
                }
                m_pParent->SaveSetup(_T("word_nwb"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData());
            }
            Close(IDOK);
        }
        //else if (_T("wordquikallbtn") == strSender)
        //{
        //    pList = xFindControl(CListUI, _T("wordquikgrouplist"));
        //    for (i = 1; i < pList->GetCount(); ++i)
        //    {
        //        pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
        //        pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
        //        pChk->SetCheck(true);
        //    }
        //}
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        pList = xFindControl(CListUI, _T("wordquikgrouplist"));
        //TRACE(_T("list:%p\n"), pList);
        pChk = static_cast<CCheckBoxUI*>(msg.pSender);

        //比较安全的方法
        CControlUI* p = pChk->GetParent();
        if (p)      //ListContainerElement
        {
            //TRACE(_T("pChk->GetParent():%p\n"), p);
            p = p->GetParent(); //CListUI
            //TRACE(_T("pChk->GetParent()->GetParent():%p\n"), p);
            if (p)
            {
                p = p->GetParent(); //CListUI
                //TRACE(_T("pChk->GetParent()->GetParent()->GetParent():%p\n"), p);
                if (p == pList)
                {
                    int nItem = pList->GetItemIndex(pChk->GetParent());

                    //如果是全选
                    bool bChk;
                    if (0 == nItem)
                    {
                        if (!m_bInMsg)
                        {
                            m_bInMsg = true;
                            bChk = pChk->GetCheck();
                            for (i = 1; i < pList->GetCount(); ++i)
                            {
                                pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
                                pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                                pChk->SetCheck(bChk);
                            }
                            pList->SelectItem(nItem);
                            m_bInMsg = false;
                        }
                    }
                    else
                    {
                        if (!m_bInMsg)
                        {
                            m_bInMsg = true;
                            if (pChk->GetCheck())
                            {
                                bChk = true;
                                for (i = 1; i < pList->GetCount(); ++i)
                                {
                                    pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
                                    pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                                    if (!pChk->GetCheck())
                                    {
                                        bChk = false;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                bChk = false;
                            }
                            pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(0));
                            pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
                            pChk->SetCheck(bChk);
                            pList->SelectItem(nItem);
                            m_bInMsg = false;
                        }
                    }
                }
            }
        }
        //发送消息的是CheckBox，ChekBox被ListContainerElement包含
        //i = pList->GetItemIndex(pChk->GetParent());
        //TRACE(_T("sender:%p, parent:%p, item index:%d\n"), pItem, pItem->GetParent(), i);
        //应该是List接收不到Click，但是CheckBox接收到，并发送selectchanged消息
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        if (strSender == _T("wordquikgradecmb"))
        {
            InitWordBookCombo();   
        }
        else if (strSender == _T("wordquiktextbookcmb"))
        {
            pCombo = static_cast<CComboUI*>(msg.pSender);
            TRACE(_T("pCombo->GetItemAt(pCombo->GetCurSel())->GetTag():%d\n"), pCombo->GetItemAt(pCombo->GetCurSel())->GetTag());
            bool bEnabled = (pCombo->GetItemAt(pCombo->GetCurSel())->GetTag() == 1) ? false : true;
            pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
            pCombo->SetEnabled(bEnabled);
            InitWordUnitCombo();
        }
        else if (_T("wordquiktextcmb") == strSender)
        {
            InitWordGroupList();
        }
        else if (_T("wordsetupsortcmb") == strSender)
        {
            InitWordGroupList();
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

LRESULT CWordQuikselect::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CWordQuikselect::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CWordQuikselect::InitControls(TNotifyUI& msg)
{
    //CString strGrade = m_pParent->ReadSetup(_T("word_grade"));
    //CString strBook = m_pParent->ReadSetup(_T("word_book"));
    //CString strUnit = m_pParent->ReadSetup(_T("word_unit"));
    CString strGroup = m_pParent->ReadSetup(_T("word_group"));
    CString strWordSort = m_pParent->ReadSetup(_T("word_sort"));

    m_bFreshGrpList = false;
    int i = _tstoi(strWordSort);
    CComboUI* pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
    if (pCombo)
    {
        if (i >= pCombo->GetCount())
        {
            i = 0;
        }
        pCombo->SelectItem(i);
    }
    InitWordGradeCombo(m_strGrade);
    InitWordBookCombo(m_strBook);
    InitWordUnitCombo(m_strUnit);
    m_bFreshGrpList = true;
    InitWordGroupList(strGroup);
}

void CWordQuikselect::InitWordGradeCombo(LPCTSTR lpstrGrade /* = NULL */)
{
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;

    pCombo = xFindControl(CComboUI, _T("wordquikgradecmb"));
    pCombo->RemoveAll();
    pElement = new CListLabelElementUI;
    pElement->SetText(_T("生词本"));
    pElement->SetUserData(_T("0")); //Id
    pCombo->Add(pElement);
    pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), _T("Select Id, 名称 from 年级 Order by 顺序"));
    if (pRecdPtr == NULL)
    {
        pCombo->SelectItem(0);
        return;
    }
    int nSel = 0, idx = 1;  //注意，idx从1开始，因为有“生词本”
    CString strId;
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
        strId = GetCollect(pRecdPtr, _T("Id"));
        pElement->SetUserData(strId);
        if (lpstrGrade && (strId == lpstrGrade))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        pRecdPtr->MoveNext();
        idx++;
    }
    pCombo->SelectItem(nSel);
}

void CWordQuikselect::InitWordBookCombo(LPCTSTR lpstrBook /* = NULL */)
{
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    CString strSql, strTmp;
    bool bCheck;

    pCombo = xFindControl(CComboUI, _T("wordquikgradecmb"));
    strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    if (strTmp == _T("0"))
    {
        strSql = _T("Select * from 生词本");
        bCheck = false;
    }
    else
    {
        strSql.Format(_T("Select * from 教材 where 年级=%s Order by 顺序"),
            strTmp);
        bCheck = true;
    }

    pCombo = xFindControl(CComboUI, _T("wordquiktextbookcmb"));
    pCombo->RemoveAll();
    pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), strSql);
    int nSel = 0, idx = 0;
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pElement = new CListLabelElementUI;
            pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pElement->SetUserData(strTmp);
            if (lpstrBook && (strTmp == lpstrBook))
            {
                nSel = idx;
            }
            if (bCheck)
            {
                strTmp = GetCollect(pRecdPtr, _T("排序"));
                //TRACE(_T("名称:%s,排序:%s\n"),
                //    GetCollect(pRecdPtr, _T("名称")),
                //    strTmp);
                if (strTmp.IsEmpty() ||
                    (_T("0") == strTmp))
                {
                    pElement->SetTag(0);
                }
                else
                {
                    pElement->SetTag(1);
                }
            }
            pCombo->Add(pElement);
            pRecdPtr->MoveNext();
            idx++;
        }
    }

    if (pCombo->GetCount() > 0)
    {
        pCombo->SelectItem(nSel);
    }
}

void CWordQuikselect::InitWordUnitCombo(LPCTSTR lpstrUnit /* = NULL */)
{
    _RecordsetPtr pRecdPtr;
    CComboUI *pComboGrade, *pComboBook, *pComboUnit;
    CListLabelElementUI* pElement;
    CString strGID, strBID;

    pComboUnit = xFindControl(CComboUI, _T("wordquiktextcmb"));
    pComboUnit->RemoveAll();

    pElement = new CListLabelElementUI;
    pElement->SetText(_T("全部"));
    pElement->SetUserData(_T("0"));
    pComboUnit->Add(pElement);
    pComboUnit->SelectItem(0);

    pComboGrade = xFindControl(CComboUI, _T("wordquikgradecmb"));
    strGID = pComboGrade->GetItemAt(pComboGrade->GetCurSel())->GetUserData();
    if (strGID == _T("0"))
    {
        return;
    }

    pComboBook = xFindControl(CComboUI, _T("wordquiktextbookcmb"));
    strBID = pComboBook->GetItemAt(pComboBook->GetCurSel())->GetUserData();

    if (strBID == _T("0"))
    {
        return;
    }

    pRecdPtr = m_pParent->GetTablesBySql(_T("Select Id, 名称 from 课文 where 年级=%s and 教材=%s Order by Id"),
        strGID, strBID);
    int nSel = 0, idx = 1;  //注意，idx从1开始
    CString strId;
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pElement = new CListLabelElementUI;
            pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
            strId = GetCollect(pRecdPtr, _T("Id"));
            pElement->SetUserData(strId);
            if (lpstrUnit && (strId == lpstrUnit))
            {
                nSel = idx;
            }
            pComboUnit->Add(pElement);
            pRecdPtr->MoveNext();
            idx++;
        }
    }
    if (pComboUnit->GetCount() > 0)
    {
        pComboUnit->SelectItem(nSel);
    }
}

void CWordQuikselect::InitWordGroupList(LPCTSTR lpstrGroup /* = NULL */)
{
    if (!m_bFreshGrpList)
    {
        return;
    }
    CString strGID, strBID, strUID;
    int nSort;
    CString strTmp;
    CString strGrade = m_pParent->ReadSetup(_T("word_grade"));
    CString strBook = m_pParent->ReadSetup(_T("word_book"));
    CString strUnit = m_pParent->ReadSetup(_T("word_unit"));
    //CString strGroup = m_pParent->ReadSetup(_T("word_group"));
    CListUI* pList = xFindControl(CListUI, _T("wordquikgrouplist"));
    pList->RemoveAll();

    CComboUI* pCombo = xFindControl(CComboUI, _T("wordquikgradecmb"));
    CControlUI* pSort = xFindControl(CControlUI, _T("wordquiksortlayout"));
    strGID = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    if (_T("0") == strGID)
    {
        //pList->SetEnabled(false);
        pList->SetVisible(false);

        pSort->SetVisible(false);
        return;
    }
    pSort->SetVisible(true);
    pList->SetVisible(true);

    pCombo = xFindControl(CComboUI, _T("wordquiktextbookcmb"));
    strBID = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData();
    pCombo = xFindControl(CComboUI, _T("wordquiktextcmb"));
    strUID = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData();

    //strSort = m_pParent->ReadSetup(_T("word_sort"));
    pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
    nSort = pCombo->GetCurSel();
    if (nSort < 0 || !pCombo->IsEnabled())
    {
        nSort = 0;
    }

    if (_T("0") == strUID)
    {
        strTmp.Format(_T("select * from 单词 where 年级=%s and 教材=%s"),
            strGID, strBID);
    }
    else
    {
        strTmp.Format(_T("select * from 单词 where 年级=%s and 教材=%s and 课文=%s"),
            strGID, strBID, strUID);
    }

    if (0 == nSort)     //按照字母次序
    {
        strTmp += _T(" Order by 单词");
    }
    else
    {
        strTmp += _T(" Order by 顺序");
    }

    SplitResultVector sptGroup;
    //default select all
    sptGroup.push_back(_T("1"));
    if (lpstrGroup)
    {
        SplitString(TString(lpstrGroup), _T(","), sptGroup, false);
    }

    CListContainerElementUI* pItem;
    CCheckBoxUI* pChk;
    LPCTSTR strAttr = _T("textpadding=\"23,3,0,0\" align=\"left\" font=\"1\" normalimage=\"file='Image/checkbox_unchk.png' dest='0,0,23,23'\" selectedimage=\"file='Image/checkbox_chk.png' dest='0,0,23,23'\"");

    _RecordsetPtr pSetPtr = m_pParent->GetTablesBySql(_T("%s"), strTmp);
    if (pSetPtr != NULL)
    {
        CString strStart, strEnd, strPre;
        int nAll = GetCollectCount(pSetPtr);
        int n = nAll / 10, i;
        if (nAll % 10)
        {
            n++;
        }

        pItem = new CListContainerElementUI;
        pItem->SetAttribute(_T("height"), _T("25"));
        pChk = new CCheckBoxUI;
        pChk->ApplyAttributeList(strAttr);
        strTmp.Format(_T("全选（共%d组，%d个单词）"),
            n, nAll);
        pChk->SetText(strTmp);
        pChk->SetUserData(_T("0"));
        pItem->Add(pChk);
        pList->Add(pItem);

        //if (1 == nSort)
        //{
        //    while(!pSetPtr->GetadoEOF())
        //    {
        //        pItem = new CListContainerElementUI;
        //        pItem->SetAttribute(_T("height"), _T("23"));
        //        pChk = new CCheckBoxUI;
        //        pChk->ApplyAttributeList(strAttr);
        //        strTmp = GetCollect(pSetPtr, _T("名称"));
        //        pChk->SetText(strTmp);
        //        pChk->SetUserData(GetCollect(pSetPtr, _T("Id")));
        //        pItem->Add(pChk);
        //        //TRACE(_T("add item:%p,%p\n"), pItem, pChk);

        //        pList->Add(pItem);

        //        if ( (sptGroup[0]) == _T("0") &&
        //            (find(sptGroup.begin() + 1, sptGroup.end(), TString(pChk->GetUserData().GetData())) != sptGroup.end()) )
        //        {
        //            pChk->SetCheck(true);
        //        }

        //        pSetPtr->MoveNext();
        //    }
        //}
        //else
        {
            TRACE(_T("all word:%d, so groups:%d, in db:%d\n"), nAll, n, sptGroup.size());
            for (i = 1; i <= n; i++)
            {
                pItem = new CListContainerElementUI;
                pItem->SetAttribute(_T("height"), _T("25"));
                pChk = new CCheckBoxUI;
                pChk->ApplyAttributeList(strAttr);
                strStart = GetCollect(pSetPtr, _T("单词"));
                int xi;
                for (xi = 0; (xi < 10-1) && !pSetPtr->GetadoEOF(); xi++)
                {
                    strPre = GetCollect(pSetPtr, _T("单词"));
                    pSetPtr->MoveNext();
                }
                if (!pSetPtr->GetadoEOF())
                {
                    strEnd = GetCollect(pSetPtr, _T("单词"));
                    pSetPtr->MoveNext();
                }
                else
                {
                    //已经到末尾
                    strEnd = strPre;
                }
                strTmp.Format(_T("第%d组:%s-%s"),
                    i, strStart, strEnd);
                pChk->SetText(strTmp);
                strTmp.Format(_T("%d"), i);
                pChk->SetUserData(strTmp);
                pItem->Add(pChk);
                //TRACE(_T("%d, group select:%s\n"), i, ((int)sptGroup.size() > i) ? sptGroup[i].c_str() : _T("out range!"));

                if ( (sptGroup[0]) == _T("0") &&
                    ((int)sptGroup.size() > i) &&
                    (sptGroup[i] != _T("0")) )
                {
                    pChk->SetCheck(true);
                }

                pList->Add(pItem);
            }
        }
        if ( (strGrade != strGID) ||
            (strBook != strBID) ||
            (strUnit != strUID) ||
            (sptGroup[0] == _T("1")))
        {
            pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(0));
            pChk = static_cast<CCheckBoxUI*>(pItem->GetItemAt(0));
            pChk->SetCheck(true);
        }
    }

}

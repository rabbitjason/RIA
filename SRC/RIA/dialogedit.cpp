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
#include "dialogedit.h"
#include "main_frame.h"

CDialogEdit::CDialogEdit(MainFrame* pParent) : m_pParent(pParent)
{}

CDialogEdit::~CDialogEdit()
{
	//PostQuitMessage(0);
}

LPCTSTR CDialogEdit::GetWindowClassName() const
{
	return _T("RIADialogEditWindow");
}

CControlUI* CDialogEdit::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogEdit::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CDialogEdit::GetSkinFile()
{
	return _T("dialogedit.xml");
}

CDuiString CDialogEdit::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE CDialogEdit::GetResourceType() const
{
	return SKIN_TYPE;
}

//LRESULT CDialogEdit::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDialogEdit::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogEdit::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDialogEdit::OnTimer(TNotifyUI& msg)
{
}

void CDialogEdit::OnExit(TNotifyUI& msg)
{
	Close();
}

void CDialogEdit::InitWindow()
{
    ASSERT(m_pParent);
    ASSERT(m_nType == DIALOG_FOLLOWME || m_nType == DIALOG_TALK);

    if (m_nType == DIALOG_FOLLOWME)
    {
        m_strWndName = _T("编辑跟我读");
        m_strSqlName = _T("跟读场景");
        m_strSqlGrpName = _T("跟读分组");
        m_strSqlCtntName = _T("跟读内容");
    }
    else
    {
        m_strWndName = _T("编辑人机对话");
        m_strSqlName = _T("人机对话场景");
        m_strSqlGrpName = _T("人机对话分组");
        m_strSqlCtntName = _T("人机对话内容");
    }
    
}

//各窗口的初始化在此完成
void CDialogEdit::OnPrepare(TNotifyUI& msg)
{
    CString strTmp;
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneENedt"));

    DWORD dwMask = pREdt->GetEventMask();
    dwMask |= (ENM_CHANGE | ENM_KEYEVENTS);
    pREdt->SetEventMask(dwMask);

    pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneCNedt"));

    dwMask = pREdt->GetEventMask();
    dwMask |= (ENM_CHANGE | ENM_KEYEVENTS);
    pREdt->SetEventMask(dwMask);

    ::SetWindowText(GetHWND(), m_strWndName);

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("dlgedtnametxt"));
    strTmp.Format(_T("%s：自定义场景"),
        m_nType == 1 ? _T("跟我读") : _T("人机对话"));
    pLbl->SetText(strTmp);

    m_strCustomSenceId = m_pParent->GetSingleField(_T("select Id from %s where 名称='自定义场景'"),
        m_strSqlName);

    InitCtrls();
}

void CDialogEdit::Notify(TNotifyUI& msg)
{
    TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    //CSliderUI* pSlider;
    //CTextUI* pTxt;
    CString strTmp;
    CRichEditUI* pREdt;
    CHorizontalLayoutUI* pLay;
    CComboUI* pCombo;
    CListUI* pList;
    CEditUI* pEdt;
    int i;

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
        //else if (strSender == kDictButtonName)
        //{
        //    CDictionary *pdlgdic = new CDictionary(m_pParent);
        //    m_pParent->StopSpeak();
        //    m_pParent->OpenModalWnd(pdlgdic, g_SkinInfo[sDictSkin].title);
        //}
        else if (_T("dlgedtsavebtn") == strSender)
        {
            pEdt = xFindControl(CEditUI, _T("dlgedtsceneedt"));
            pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneENedt"));
            if (pEdt->GetText().IsEmpty() && pREdt->GetText().IsEmpty())
            {
                AfxMessageBox(_T("请输入会话名称。"));
                pEdt->SetFocus();
            }
            else if (!pREdt->GetText().IsEmpty() && IncludeChinese(pREdt->GetText().GetData()))
            {
                AfxMessageBox(_T("英文内容中包含除英文以外的字符。"));
                pREdt->SetFocus();
            }
            else
            {
                if (pEdt->GetText().IsEmpty() &&
                    !pREdt->GetText().IsEmpty())
                {
                    SplitResultVector sptText;
                    SplitString(TString(pREdt->GetText().GetData()), _T("\r\n"), sptText);
                    pEdt->SetText(sptText[0].c_str());
                }

                pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
                if (pCombo->GetCurSel() >= 0)
                {
                    strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
                    TRACE(_T("selected id:%s, name:%s\n"), strTmp, pCombo->GetItemAt(pCombo->GetCurSel())->GetText().GetData());
                    m_pParent->ExecuteSql(_T("Delete from %s where 分组=%s"),
                        m_strSqlCtntName, strTmp);
                    m_pParent->ExecuteSql(_T("Delete from %s where Id=%s"),
                        m_strSqlGrpName, strTmp);
                }

                CString str = pEdt->GetText();
                str.Trim();
                str.Replace(_T("'"), _T("''"));

                strTmp = m_pParent->GetSingleField(_T("select count(0) from %s where 名称='%s' and 场景=%s"),
                                        m_strSqlGrpName, str, m_strCustomSenceId);
                if (strTmp.IsEmpty() || (strTmp == _T("0")))
                {
                    strTmp = m_pParent->GetSingleField(_T("select max(顺序) from %s"),
                        m_strSqlGrpName).Trim();
                    if (strTmp.IsEmpty())
                    {
                        strTmp = _T("0");
                    }
                    int num = _tstoi(strTmp) + 1;
                    BOOL bOK = m_pParent->ExecuteSql(_T("Insert into %s(场景,名称,顺序) Values(%s, '%s', %d)"),
                                            m_strSqlGrpName, m_strCustomSenceId, str, num);
                    if (bOK)
                    {
                        SplitResultVector sptSpokenEN, sptSpokenCN;
                        
                        pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneENedt"));
                        strTmp = pREdt->GetText();
                        strTmp.Trim();
                        strTmp.Replace(_T("'"), _T("''"));
                        SplitString(TString(strTmp), _T("\r"), sptSpokenEN);
                        
                        //TRACE(_T("EN size:%d\n"), sptSpokenEN.size());
                        //SplitResultVector::const_iterator p;
                        //for (p = sptSpokenEN.begin(); p != sptSpokenEN.end(); ++p)
                        //{
                        //    TRACE(_T("%s\n"), p->c_str());
                        //}
                        //TRACE(_T("\n"));

                        pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneCNedt"));
                        strTmp = pREdt->GetText();
                        strTmp.Trim();
                        strTmp.Replace(_T("'"), _T("''"));
                        SplitString(TString(strTmp), _T("\r"), sptSpokenCN);
                        
                        //TRACE(_T("CN size:%d\n"), sptSpokenEN.size());
                        //for (p = sptSpokenCN.begin(); p != sptSpokenCN.end(); ++p)
                        //{
                        //    TRACE(_T("%s\n"), p->c_str());
                        //}
                        //TRACE(_T("\n"));

                        num = max(sptSpokenEN.size(), sptSpokenCN.size());

                        strTmp = m_pParent->GetSingleField(_T("select max(Id) from %s"), m_strSqlGrpName).Trim();
                        CString str5, str6;
                        for (i = 0; i < num; i++)
                        {
                            str5.Empty();
                            str6.Empty();
                            if (i < (int)sptSpokenCN.size())
                            {
                                str5 = sptSpokenCN[i].c_str();
                                str5.Trim();
                            }
                            if (i < (int)sptSpokenEN.size())
                            {
                                str6 = sptSpokenEN[i].c_str();
                                str6.Trim();
                            }
                            if (!str5.IsEmpty() || !str6.IsEmpty())
                            {
                                if (DIALOG_TALK == m_nType)
                                {
                                    m_pParent->ExecuteSql(_T("Insert into %s(内容,中文,分组) Values('%s', '%s',%s)"),
                                        m_strSqlCtntName, str6, str5, strTmp);
                                }
                                else
                                {
                                    m_pParent->ExecuteSql(_T("Insert into %s(内容,中文,分组,顺序) Values('%s', '%s',%s,%d)"),
                                        m_strSqlCtntName, str6, str5, strTmp, i);
                                }
                            }
                        }
                        MyAfxMessageBox(_T("%s"), _T("保存成功。"));
                        InitCtrls();
                    }
                }
                else
                {
                    MyAfxMessageBox(_T("会话 [%s] 已存在，请勿重复加入。"),
                        pEdt->GetText().GetData());
                }
            }
        }
        else if (_T("dlgedtdelbtn") == strSender)
        {
            pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
            if (pCombo->GetCount() <= 0)
            {
                //_T("没有可以删除的会话。")
            }
            else if (pCombo->GetCurSel() < 0)
            {
                //_T("没有指定删除的会话。");
            }
            else
            {
                if (IDOK == AfxMessageBox(_T("是否确定删除会话？"),
                                MB_OKCANCEL | MB_ICONQUESTION))
                {
                    strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
                    m_pParent->ExecuteSql(_T("Delete from %s where 分组=%s"),
                        m_strSqlCtntName,
                        strTmp);
                    m_pParent->ExecuteSql(_T("Delete from %s where Id=%s"),
                        m_strSqlGrpName,
                        strTmp);

                    InitCtrls();
                }
            }
        }
	}
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        if (_T("dlgedtscenecmb") == strSender)
        {
            GetContent();
            pCombo = static_cast<CComboUI*>(msg.pSender);
            pEdt = xFindControl(CEditUI, _T("dlgedtsceneedt"));
            int nSelectedId = pCombo->GetCurSel();
            strTmp = pCombo->GetItemAt(nSelectedId)->GetText();
            TRACE(_T("select:%s\n"), strTmp);
            pEdt->SetText(strTmp);
            strTmp = pCombo->GetItemAt(nSelectedId)->GetUserData();
            pList = xFindControl(CListUI, _T("dlgedtscenelist"));
            if ( (pList->GetCount() > 0) &&
                ( (pList->GetCurSel() < 0) ||
                    (pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData() != strTmp)) )
            {
                for (i = 0; i < pList->GetCount(); i++)
                {
                    if (pList->GetItemAt(i)->GetUserData().GetData() == strTmp)
                    {
                        pList->SelectItem(i);
                        break;
                    }
                }
            }
        }
        else if (_T("dlgedtscenelist") == strSender)
        {
            pList = static_cast<CListUI*>(msg.pSender);
            strTmp = pList->GetItemAt(pList->GetCurSel())->GetUserData();
            pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
            if ( (pCombo->GetCount() > 0) &&
                ( (pCombo->GetCurSel() < 0) ||
                    (pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData() != strTmp)) )
            {
                for (i = 0; i < pCombo->GetCount(); i++)
                {
                    if (pCombo->GetItemAt(i)->GetUserData().GetData() == strTmp)
                    {
                        pCombo->SelectItem(i);
                        break;
                    }
                }
            }
        }
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
    }
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (_T("dlgedtsceneENedt") == strSender)
        {
            pREdt = static_cast<CRichEditUI*>(msg.pSender);
            pLay = xFindControl(CHorizontalLayoutUI, _T("dlgedtMsgEng"));
            pLay->SetVisible(false);
            strTmp = pREdt->GetText();
            strTmp.Trim();
            if (strTmp.IsEmpty())
            {
                pLay->SetVisible(true);
            }
            if (IncludeChinese(strTmp))
            {
                MyAfxMessageBox(_T("%s"), _T("请在英文状态下输入英文内容。"));
                pREdt->SetFocus();
            }
        }
        else if (_T("dlgedtsceneCNedt") == strSender)
        {
            pREdt = static_cast<CRichEditUI*>(msg.pSender);
            pLay = xFindControl(CHorizontalLayoutUI, _T("dlgedtMsgChi"));
            pLay->SetVisible(false);
            strTmp = pREdt->GetText();
            strTmp.Trim();
            if (strTmp.IsEmpty())
            {
                pLay->SetVisible(true);
            }
        }
    }
	else if (sType, DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

    return;
}

LRESULT CDialogEdit::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LPCTSTR CDialogEdit::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return m_pParent->GetCurrentResourceID();
}

void CDialogEdit::GetContent()
{
    CString strTmp;
    CComboUI* pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
    CListUI* pList = xFindControl(CListUI, _T("dlgedtcontentlist"));
    
    strTmp.Format(_T("and 分组=%s"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("Select * from %s where (内容 IS NOT NULL) and (内容 <> '') %s"),
        m_strSqlCtntName,
        strTmp);

    pList->RemoveAll();
    CListLabelElementUI* pItem;
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("内容")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pItem->SetUserData(strTmp);
            pList->Add(pItem);

            pRecdPtr->MoveNext();
        }
    }

    GetContentTxt();
}

void CDialogEdit::GetContentTxt()
{
    CString strTmp, strEN = _T(""), strCN = _T(""), strSql;
    CComboUI* pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
    CListUI* pList = xFindControl(CListUI, _T("dlgedtcontentlist"));
    CRichEditUI *pREdtEN = xFindControl(CRichEditUI, _T("dlgedtsceneENedt"));
    CRichEditUI *pREdtCN = xFindControl(CRichEditUI, _T("dlgedtsceneCNedt"));

    strTmp.Format(_T("where 分组=%s"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
    if (DIALOG_TALK == m_nType)
    {
        strSql.Format(_T("Select * from %s %s"),
            m_strSqlCtntName,
            strTmp);
    }
    else
    {
        strSql.Format(_T("Select * from %s %s order by 顺序"),
            m_strSqlCtntName,
            strTmp);
    }
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("%s"), strSql);
    
    pREdtCN->SetText(_T(""));
    pREdtEN->SetText(_T(""));

    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            strEN += GetCollect(pRecdPtr, _T("内容"));
            strEN += _T("|");

            strCN += GetCollect(pRecdPtr, _T("中文"));
            strCN += _T("|");

            pRecdPtr->MoveNext();
        }
    }

    if (!strEN.IsEmpty())
    {
        strEN.TrimRight(_T('|'));
        strEN.Replace(_T("|"), _T("\r\n"));
        pREdtEN->SetText(strEN);
    }
    if (!strCN.IsEmpty())
    {
        strCN.TrimRight(_T('|'));
        strCN.Replace(_T("|"), _T("\r\n"));
        pREdtCN->SetText(strCN);
    }

}

void CDialogEdit::InitCtrls()
{
    CListLabelElementUI* pItem;
    CString strTmp;
    CComboUI* pCombo = xFindControl(CComboUI, _T("dlgedtscenecmb"));
    CListUI* pList = xFindControl(CListUI, _T("dlgedtscenelist"));
    //CEditUI* pEdt = xFindControl(CEditUI, _T("dlgedtsceneedt"));

    pCombo->RemoveAll();
    pList->RemoveAll();
    //pEdt->SetText(_T(""));

    if (m_strCustomSenceId.IsEmpty())
    {
        return;
    }
    _RecordsetPtr pRecdPtr = m_pParent->GetTablesBySql(_T("Select * from %s where 场景=%s order by 顺序 desc"),
        m_strSqlGrpName, m_strCustomSenceId);
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pItem->SetUserData(strTmp);
            pCombo->Add(pItem);

            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pItem->SetUserData(strTmp);
            pList->Add(pItem);

            pRecdPtr->MoveNext();
        }
        pCombo->SelectItem(-1);
    }

    CEditUI* pEdt = xFindControl(CEditUI, _T("dlgedtsceneedt"));
    pEdt->SetText(_T(""));

    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneENedt"));
    pREdt->SetText(_T(""));

    pREdt = xFindControl(CRichEditUI, _T("dlgedtsceneCNedt"));
    pREdt->SetText(_T(""));

}
//
// main_frame.cpp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#include "stdafx.h"
#include "reader_setup.h"
#include "readingbarsetup.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "listen_voice.h"
#include "dictionary.h"
#include "wordaddnew.h"
#include "wordtovoice.h"
#include "translateword.h"
#include "translateall.h"
#include "SplitSentence.h"
#include "talkcount.h"
#include "listenresult.h"
#include "dialogedit.h"
#include "dialoglist.h"
#include "dialogsetup.h"
#include "dialogwrittingsetup.h"
#include "loading.h"
#include "sr.h"
#include "MFCDlgSR.h"
#include "setting.h"
#include "wordquik.h"
#include "DlgPaper.h"
#include "wordmgtbook.h"
#include "main_frame.h"

//const TCHAR* const kTitleControlName = _T("apptitle");

//主界面按钮
const TCHAR* const kWordButtonName       = _T("wordbtn");
const TCHAR* const kGrammarButtonName    = _T("Grammarbtn");
const TCHAR* const kReadingbarButtonName = _T("readingbarbtn");
const TCHAR* const kReadButtonName       = _T("readbtn");
const TCHAR* const kSymbolButtonName     = _T("symbolbtn");
const TCHAR* const kReportButtonName     = _T("reportbtn");
const TCHAR* const kDialogButtonName     = _T("dialogbtn");
const TCHAR* const kJoyButtonName        = _T("joybtn");
const TCHAR* const kContactButtonName    = _T("contactbtn");
const TCHAR* const kListenButtonName     = _T("listenbtn");
const TCHAR* const kRecordButtonName     = _T("recordbtn");
const TCHAR* const kHelpButtonName       = _T("helpbtn");
const TCHAR* const kPaperButtonName       = _T("paperbtn");

//听力设置界面按钮
const TCHAR* const kListenCfgDefaultButtonName  = _T("listendefaultbtn");
const TCHAR* const kListenCfgOKButtonName       = _T("listenoktbtn");


//配置表中的项目
//const TCHAR* const kDBConfigListenSpeed = _T("Listen_Speed");

SkinInfo g_SkinInfo[] =
{
    {_T("main.xml"), _T("全记牢智能英语")},
    {_T("wordselect.xml"), _T("学单词设置")},
    {_T("word.xml"), _T("学单词")},
    {_T("wordsetup.xml"), _T("学单词设置")},
    {_T("wordexam.xml"), _T("练单词")},
    {_T("wordexamsetup.xml"), _T("练单词设置")},
    {_T("wordexamresult.xml"), _T("练单词结果")},
    {_T("wordaddnew.xml"), _T("生词本")},
    {_T("wordmngnew.xml"), _T("生词本")},
    {_T("wordtovoice.xml"), _T("单词转声音文件")},
    {_T("read.xml"), _T("朗读")},
    {_T("readcfg.xml"), _T("朗读设置")},
    {_T("readstat.xml"), _T("发音统计")},
    {_T("readtrans.xml"), _T("词典")},
    {_T("readcntext.xml"), _T("全文翻译")},
    {_T("dialog.xml"), _T("对话")},
    {_T("dialogselect.xml"), _T("对话")},
    {_T("dialogedit.xml"), _T("对话")},
    {_T("dialogsetup.xml"), _T("对话设置")},
    {_T("dialoglist.xml"), _T("跟读列表")},
    {_T("listen.xml"), _T("听力训练")},
    {_T("listenselect.xml"), _T("听力设置")},
    {_T("listensetup.xml"), _T("练听力设置")},
    {_T("listenresult.xml"), _T("听力结果")},
    {_T("Grammar.xml"), _T("学语法")},
    {_T("GrammarResult.xml"), _T("练语法结果")},
    {_T("symbol.xml"), _T("音标")},
    {_T("joy.xml"), _T("音乐")},
    {_T("record.xml"), _T("录音机")},
    {_T("readingbar.xml"), _T("迷你朗读条")},
    {_T("readingbarcfg.xml"), _T("迷你朗读条设置")},
    {_T("report.xml"), _T("成绩报告")},
    {_T("report_detail.xml"), _T("成绩报告")},
    {_T("loading.xml"), NULL},
    {_T("exportwave.xml"), NULL},
    {_T("dict.xml"), _T("词典")},
    {_T("register.xml"), _T("注册")},
    {_T("paperselect.xml"), _T("单元测试")},
};

void CMyWndUI::DoInit()
{
    BOOL bOk;
    do 
    {
        m_hWnd = NULL;
        if (m_strClassName == MSCHART_CLASS_NAME)
        {
            ASSERT(m_pChart == NULL);
            m_pChart = new CMSChart;
            bOk = m_pChart->Create(NULL, m_dwStyle, CRect(0, 0, 0, 0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 
                0, NULL, FALSE, CRIAApp::m_bstrChartLicense);
            if (!bOk)
            {
                //MyAfxMessageBox(_T("%s"), _T("创建MsChart控件失败，请确认控件已正确注册。"));
                TRACE(_T("%s"), _T("创建MsChart控件失败，请确认控件已正确注册。\n"));
                delete m_pChart;
                m_pChart = NULL;
                break;
            }
            m_pChart->SetChartType(VtChChartType2dBar);
            m_pChart->SetDrawMode(VtChDrawModeBlit);
            m_pChart->SetAllowSelections(FALSE);
            m_pChart->SetRandomFill(FALSE);
            m_pChart->SetDrawMode(VtChDrawModeBlit);

            m_pChart->SetShowLegend(TRUE);
            CVcLegend& cLegn = m_pChart->GetLegend();
            cLegn.GetLocation().SetVisible(TRUE);
            cLegn.GetLocation().SetLocationType(VtChLocationTypeRight);

            m_hWnd = m_pChart->GetSafeHwnd();
        }
        else if (m_strClassName == MSWMP_CLASS_NAME)
        {
            //should use child class...
            ASSERT(m_pWMP == NULL);
            m_pWMP = new CWMPPlayer4;
            bOk = m_pWMP->Create(NULL, m_dwStyle, CRect(0,0,0,0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 
                USER_WMP_WMV_ID, NULL, FALSE, CRIAApp::m_bstrWMPLicense);
            if (!bOk)
            {
                //MyAfxMessageBox(_T("%s"), _T("创建视频播放接口失败，请确认控件已正确注册。"));
                TRACE(_T("%s"), _T("创建视频播放接口失败，请确认控件已正确注册。\n"));
                delete m_pWMP;
                m_pWMP = NULL;
                break;
            }
            //TRUE:全屏时有右键菜单；FALSE:无
            m_pWMP->put_enableContextMenu(FALSE);
            //TRUE：双击可全屏
            m_pWMP->put_enabled(TRUE);
            //"invisible":隐藏所有；"none":只显示视频或者音频效果；"mini":MINI模式；"full":完整模式；"custom":自定义效果 
            m_pWMP->put_uiMode(WMP_MODE);
            m_pWMP->put_windowlessVideo(TRUE);
            //CWMPSettings wmpSettings= m_pWMP->get_settings();

            m_hWnd = m_pWMP->GetSafeHwnd();
        }
        else
        {
            m_hWnd = ::CreateWindowEx(0, m_strClassName, _T(""),
                m_dwStyle, 0, 0, 0, 0,
                m_PaintManager.GetPaintWindow(),
                NULL, AfxGetInstanceHandle(), NULL);
        }
    } while (false);
    
    //Attach(hWnd);
    __super::DoInit();
}

static LPCTSTR g_wordexam_opt[] =
{
    _T("wordexamanwer1opt"),
    _T("wordexamanwer2opt"),
    _T("wordexamanwer3opt"),
    _T("wordexamanwer4opt")
};

static LPCTSTR g_wordexam_type_chk[]=
{
    _T("wordexamciyiselectdancichk"),
    _T("wordexamciyiwritedancichk"),
    _T("wordexamyinbiaoselectdancichk"),
    _T("wordexamyinbiaowritedancichk"),
    _T("wordexamfayinwritedancichk"),
    _T("wordexamfayinselectdancichk"),
    _T("wordexamdanciselectciyichk"),
    _T("wordexamdanciselectyinbiaochk")
};

static LPCTSTR g_grammar_opt[] =
{
    _T("gramexamanswer1opt"),
    _T("gramexamanswer2opt"),
    _T("gramexamanswer3opt"),
    _T("gramexamanswer4opt")
};

static LPCTSTR g_ReportDetailBtns[] =
{
    _T("reportdancibtn"),
    _T("reportyuedubtn"),
    _T("reportkouyubtn"),
    _T("reportinglibtn"),
    _T("reporyufabtn")
};

static LPCTSTR g_listen_opt[] =
{
    _T("listenquestion1"),
    _T("listenquestion2"),
    _T("listenquestion3"),
    _T("listenquestion4")
};

static LPCTSTR g_listen_pic_opt[] =
{
    _T("listenpicquestion1"),
    _T("listenpicquestion2"),
    _T("listenpicquestion3"),
    _T("listenpicquestion4")
};

static LPCTSTR g_dialog_select_opt[] =
{
    _T("dlgfollowmeopt"),
    _T("dlgtalkopt"),
    _T("dlgscenetalkopt"),
    _T("dlgwrittingopt")
};

static LPCTSTR g_dialog_opt[] =
{
    _T("dialogfollowmeopt"),
    _T("dialogtalkopt"),
    _T("dialogscenetalkopt"),
    _T("dialogwrttingopt")
};

static LPCTSTR g_dialog_writting_opt[] =
{
    _T("dialogwrttingtype0"),
    _T("dialogwrttingtype1"),
    _T("dialogwrttingtype2")
};

static LPCTSTR g_register_opt[] =
{
    _T("regstep1opt"),
    _T("regstep2opt"),
    _T("regstep3opt"),
    _T("regstep4opt")
};

static LPCTSTR g_register_type_opt[] =
{
    _T("regtypeonlineopt"),
    _T("regtypeqrcodeopt"),
    _T("regtypetelephoneopt")
};

static
int IsOption(LPCTSTR arOption[], int nNumber, const TNotifyUI& msg)
{
    int i;
    for (i = 0; i < nNumber; i++)
    {
        if (msg.pSender->GetName() == arOption[i])
        {
            return i+1;
        }
    }
    return 0;
}

static
int hlpAppedText(CRichEditUI* pREdt, LPCTSTR lpstrText)
{
    if ((pREdt->GetText().GetLength() > 0) &&
        (pREdt->GetText(GT_USECRLF).Right(1) != _T("\n")) )
    {
        pREdt->AppendText(_T("\n"));
    }
    int i = pREdt->AppendText(lpstrText);
    pREdt->SetFocus();
    return i;
}

static
void hlpRichEditEnsureVisible(CRichEditUI* pREdt)
{
    long lStart, lEnd;
    pREdt->GetSel(lStart, lEnd);
    //TRACE(_T("lStart:%d, lEnd:%d\n"), lStart, lEnd);
    POINT cr_pt;
    ::GetCaretPos(&cr_pt);
    TRACE(_T("read:Caret x:%d, y:%d\n"), cr_pt.x, cr_pt.y);
    while (cr_pt.y < 0)   //if out of range, invisible
    {
        long lLine = pREdt->LineFromChar(lStart), lFirst;
        LRESULT lr = 0;
        pREdt->TxSendMessage(EM_GETFIRSTVISIBLELINE, 0, 0, &lr);
        lFirst = (long)lr;
        pREdt->LineScroll((lLine > lFirst) ? 1 : -1);
        ::GetCaretPos(&cr_pt);
        TRACE(_T("Caret y:%d\n"), cr_pt.y);
    }
}

locale MainFrame::m_loc;
static SkinMap InitSkinMap()
{
    SkinMap tmp;
    tmp.insert(SkinMap::value_type(_T("skin1mnu"), IDR_ZIPRES));
    tmp.insert(SkinMap::value_type(_T("skin2mnu"), IDR_ZIPRES2));
    tmp.insert(SkinMap::value_type(_T("skin3mnu"), IDR_ZIPRES3));
#if SKIN==SKN_SMART
    //tmp.insert(SkinMap::value_type(_T("skin4mnu"), IDR_ZIPRES4));
#endif

    return tmp;
}

SkinMap MainFrame::m_SkinInfo = InitSkinMap();

MainFrame::MainFrame() : m_nSkinId(sMainSkin)
, m_cpVoice(NULL)
, m_bGramBackFromResult(false)
, m_pJoyWMPCur(NULL)
, m_pchrFileBuffer(NULL)
, m_dwFileBufferSize(4096*100)
, m_bListenReview(false)
, m_nDialogSelectPageId(DIALOG_FOLLOWME)
//, m_bRecognizerInput(false)
, m_bHoverActived(false)
, m_hRegKey(NULL)
, m_bInitOK(false)
, m_nResourceId(IDR_ZIPRES)
, m_bRegistered(false)
, m_bMaterialInstalled(false)
//, m_bAICompleted(false)
, m_hSubWnd(NULL)
{
    m_strCommonWaveFile.Format(_T("%sData\\%s"), CPaintManagerUI::GetInstancePath().GetData(),  READ_FOLLOW_WAVE_FILE);
    m_strRegisterQRFile.Format(_T("%sData\\%s"), CPaintManagerUI::GetInstancePath().GetData(),  REG_QR_FILE);
#if SKIN==SKN_WIN8
    m_strRegKey = REG_PATH_RIA_QJL;
#else
    m_strRegKey = REG_PATH_RIA;
#endif

    DWORD dwErr = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
        m_strRegKey,
        0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_READ | KEY_WRITE | KEY_WOW64_32KEY, NULL,
        &m_hRegKey, NULL);
    if (ERROR_SUCCESS != dwErr)
    {
        TRACE(_T("RegCreateKeyEx with %s failed:%d\n"), m_strRegKey, dwErr);
        MyAfxMessageBox(_T("注册表访问失败：%s"), MyFormatMessage(dwErr));
        AfxThrowUserException();
        return;
    }
    //读取皮肤信息
    CDuiString strSkin;
    WORD wId;
    strSkin = RegKeyGetValue(_T("SkinName"));
    if (strSkin.IsEmpty())
    {
        strSkin = _T("skin1mnu");
    }
    
    SkinMap::const_iterator p = m_SkinInfo.find(strSkin);
    if (p == m_SkinInfo.end())
    {
        //注册表信息有错误？
        m_SkinMenuCheckInfo.insert(map<CDuiString,bool>::value_type(_T("skin1mnu"), true));
        wId = IDR_ZIPRES;
    }
    else
    {
        m_SkinMenuCheckInfo.insert(map<CDuiString,bool>::value_type(strSkin, true));
        wId = p->second;
    }
	
    SetCurrentResourceID(wId);

    CString strMaterial = RegKeyGetValue(_T("Material"));
    if (!strMaterial.IsEmpty() &&
        (_tstoi(strMaterial) >= 1) )
    {
        m_bMaterialInstalled = true;
    }

    m_loc = locale::classic();
    TRACE("loc name:%s\n", m_loc.name().c_str());
}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("RIAGuiWindow");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
    TRACE("%s\n", __FUNCTION__);
    TRACE(_T("crete class:%s\n"), pstrClass);
    CString strName = pstrClass;
    CMyWndUI  *pUI = NULL;
    CString strClassName = _T("");
    DWORD dwStyle = WS_VISIBLE | WS_CHILD;

    if (strName ==_T("MySlider"))
    {
        strClassName = TRACKBAR_CLASS;
        dwStyle |= (TBS_AUTOTICKS | TBS_HORZ | TBS_TOP);
    }
    else if (strName == _T("MyCombo"))
    {
        strClassName = WC_COMBOBOX;
        dwStyle |= (CBS_DROPDOWNLIST);
    }
    else if (strName == _T("MyEdit"))
    {
        strClassName = WC_EDIT;
        dwStyle |= (ES_AUTOVSCROLL | ES_MULTILINE);
    }
    else if (strName == _T("MsChart"))
    {
        strClassName = MSCHART_CLASS_NAME;
    }
    else if (strName == _T("MSWMP"))
    {
        strClassName = MSWMP_CLASS_NAME;
    }
    else
    {
        return NULL;
    }

    pUI  = new CMyWndUI(strClassName, dwStyle, m_PaintManager);
    m_AllMyWnd.push_back(pUI);

	return pUI;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);

    if (m_pchrFileBuffer)
    {
        delete[] m_pchrFileBuffer;
        m_pchrFileBuffer = NULL;
    }

    if (m_hRegKey)
    {
        ::RegCloseKey(m_hRegKey);
        m_hRegKey = NULL;
    }

    if (m_hAiEngine)
    {
        m_AiEngine.aiengine_delete(m_hAiEngine);
    }
    m_AiEngine.Release();

	delete this;
}

CDuiString MainFrame::GetSkinFile()
{
	return g_SkinInfo[m_nSkinId].skin;
}

CDuiString MainFrame::GetSkinFolder()
{
	return SKIN_FOLDER;
}

UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return SKIN_TYPE;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
    if( wParam == SC_CLOSE ) {
        Close();
        //::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //DUITRACE(_T("recv %s"), DUI__TraceMsg(uMsg));
    //CMyWndUI *pUI;
    switch(uMsg)
    {
    case WM_COMMAND:
        //TRACE(_T("wParam:0x%x, lParam:0x%x\n"), wParam, lParam);
        //CMyWndUI  *pUI = static_cast<CMyWndUI*>(m_PaintManager.FindControl(_T("readercmb")));
        //CPaintManagerUI::SendNotify(pUI,)
        break;
    case WM_HSCROLL:
        TRACE(_T("WM_HSCROLL, wParam:0x%x, lParam:0x%x\n"), wParam, lParam);
        break;
    case WM_PAINT:
        break;
    case WM_ACTIVATE:
        {
            //TRACE(_T("recv WM_ACTIVATE msg, wParam:0x%x, lParam:%d\n"),
            //    wParam, lParam);
            //if ((HANDLE)lParam == GetHWND())
            {
                WORD wAct = LOWORD(wParam);
                if (wAct == WA_ACTIVE ||
                    wAct == WA_CLICKACTIVE)
                {
                    m_bHoverActived = false;
                }
                else
                {
                    m_bHoverActived = true;
                }
            }
        }
        break;
    case WM_NCHITTEST:
        if (m_bHoverActived)
        {
            TRACKMOUSEEVENT te = {0};
            te.cbSize = sizeof(TRACKMOUSEEVENT);
            te.dwFlags = TME_HOVER | TME_NONCLIENT;
            te.hwndTrack = GetHWND();
            te.dwHoverTime = 10;//HOVER_DEFAULT;
            ::TrackMouseEvent(&te);
        }
        break;
    case WM_NCMOUSEHOVER:
    case WM_MOUSEHOVER:
        //TRACE(_T("recv 0x%x msg, wParam:0x%x, x_pt:%d, y_pt:%d\n"),
        //    uMsg, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        if (sReadingbarSkin == m_nSkinId)
        {
            if (m_bHoverActived)
            {
                ::SendInput(_countof(m_ReadBarCtrlC), m_ReadBarCtrlC, sizeof(INPUT));
                m_bHoverActived = false;
            }
        }
        break;
    case WM_CLOSE:
        {
            TRACE(_T("WM_CLOSE, m_hSubWnd:%p\n"), m_hSubWnd);
            if (m_hAiEngine && !m_AiEngine.IsCallbackQuit())
            {
                TRACE(_T("still AI, don't close\n"));
                return 0;
            }
            //if (m_hSubWnd)
            //{
            //    //::SendMessage(m_hSubWnd, WM_SETFOCUS, (WPARAM)GetHWND(), 0);
            //    //::SetActiveWindow(m_hSubWnd);
            //    //::SendMessage(m_hSubWnd, WM_CLOSE, 0, 0);
            //    TRACE(_T("still sub wnd, don't close\n"));
            //    return 0;
            //}
        }
        break;
    case WM_DESTROY:
        {
            TRACE(_T("WM_DESTROY\n"));
        }
        break;
    case WM_NOTIFY:
        //RichEdit may send notify
        //richedit不发送Notify...
        //if (m_nSkinId == sReadSkin)
        //{
        //    MSGFILTER* pMsg = (MSGFILTER*)lParam;
        //    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        //    TRACE(_T("notify code:%d, hwndFrom:%p, GetManager()->GetPaintWindow():%p\n"),
        //        pMsg->nmhdr.code, pMsg->nmhdr.hwndFrom, pREdt->GetManager()->GetPaintWindow());
        //    if ((EN_MSGFILTER == pMsg->nmhdr.code) &&
        //        (pREdt->GetManager()->GetPaintWindow() == pMsg->nmhdr.hwndFrom))
        //    {
        //        ReadKeyMouseEvent(pMsg);
        //    }
        //}
        break;
    case WM_KEYDOWN:
        if ((m_nSkinId == sReadSkin) &&
             (READ_TYPE_READ == m_nReadType))
        {
            ReadKeyDown(wParam, lParam);
        }
       break;
    case MM_MIXM_LINE_CHANGE:
        {
            TRACE(_T("recv MM_MIXM_LINE_CHANGE, wParam:%p, lParam:%d\n"), wParam, lParam);
            m_WaveRecorder.OnLineChange(wParam, lParam);
        }
        break;
    case MM_MIXM_CONTROL_CHANGE:
        {
            TRACE(_T("recv MM_MIXM_CONTROL_CHANGE, wParam:%p, lParam:%d\n"), wParam, lParam);
            m_WaveRecorder.OnControlChange(wParam, lParam);
        }
        break;
    case WM_DEVICECHANGE:
        {
            TRACE(_T("recv WM_DEVICECHANGE, wParam:%d\n"), wParam);
            m_WaveRecorder.OnDevChanged(wParam, lParam);
        }
        break;
    default:
        break;
    }
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MainFrame::OnTimer(TNotifyUI& msg)
{
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

BOOL MainFrame::MyReadAsciiFile(LPCTSTR strPath, CString& strContent)
{
    CFile fl;
    CFileException fe;

    if (!fl.Open(strPath, CFile::modeRead | CFile::shareDenyNone, &fe))
    {
        return FALSE;
    }
    DWORD dwSz = DWORD(fl.GetLength());
    if (m_dwFileBufferSize < dwSz)
    {
        m_dwFileBufferSize = dwSz;
        delete[] m_pchrFileBuffer;
        m_pchrFileBuffer = new char[m_dwFileBufferSize];
    }
    UINT xr = fl.Read(m_pchrFileBuffer, m_dwFileBufferSize);
    m_pchrFileBuffer[xr] = '\0';
    strContent = CA2T(m_pchrFileBuffer);
    //myA2T(strContent.GetBufferSetLength(xr), xr, m_pchrFileBuffer);
    //strContent.ReleaseBuffer();

    fl.Close();
    return TRUE;
}

BOOL MainFrame::MyWriteAsciiFile(LPCTSTR strPath, LPCTSTR strContent)
{
    CFile fl;
    CFileException fe;

    if (!fl.Open(strPath, CFile::modeWrite | CFile::shareDenyNone, &fe))
    {
        return FALSE;
    }
    CT2A pchrFileBuffer(strContent);//myT2A(m_pchrFileBuffer, m_dwFileBufferSize, strContent);
    //m_pchrFileBuffer[lstrlen(strContent)] = '\0';
    fl.Write(&pchrFileBuffer.m_szBuffer[0], lstrlenA(pchrFileBuffer));

    fl.Close();
    return TRUE;
}

void MainFrame::GetSentencePosition(__in LPCTSTR strCtrlName, __in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition)
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, strCtrlName);
    GetSentencePosition(pREdt, nCurrentPos, intFromPosition, intToPosition);
}

void MainFrame::GetSentencePosition(__in CRichEditUI* pREdt, __in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition)
{
    CString strContent = pREdt->GetText().GetData();
    int nIdx, n_count = strContent.GetLength();

    //读入后，\n被转换成\r?
    static CString READ_N_POS = _T("\r\n");

    intFromPosition = intToPosition = 0;
    //TCHAR xbuf[1024];
    //memcpy(xbuf, strContent.GetBuffer(), 200);
    //strContent.ReleaseBuffer();
    //xbuf[200] = _T('\0');

    do 
    {
        if (strContent.IsEmpty())
        {
            break;
        }
        if (nCurrentPos < 0 ||
            nCurrentPos >= n_count)
        {
            break;
        }

        //往前数，碰到READ_POS中的字符为止
        nIdx = nCurrentPos;
        while(nIdx >= 0)
        {
            if (READ_N_POS.Find(strContent[nIdx]) >= 0)
            {
                intFromPosition = nIdx;
                break;
            }
            nIdx--;
        }
        //if (intFromPosition != 0)
        {
            //跳过连续的\r,\n
            while((intFromPosition < n_count) &&
                (READ_N_POS.Find(strContent[intFromPosition]) >= 0) )
            {
                intFromPosition++;
            }
        }

        //往后数
        nIdx = intFromPosition+1;
        while(nIdx < n_count)
        {
            if (READ_N_POS.Find(strContent[nIdx]) >= 0)
            {
                intToPosition = nIdx;
                break;
            }
            nIdx++;
        }
        if (intToPosition == 0)
        {
            intToPosition = n_count;
        }
        //else
        //{
        //    if ((intToPosition > n_count) ||
        //        (intToPosition <= intFromPosition))
        //    {
        //        intToPosition = n_count;
        //    }
        //}
    } while (FALSE);
}

void MainFrame::NoticePrompt()
{
    CString strTmp;
    strTmp.Format(_T("%sSystem\\Sound\\ding.wav"), CPaintManagerUI::GetInstancePath());
    m_mciWave.PlayWait(GetHWND(), strTmp);
}

void MainFrame::SetRichEditSelColor(CRichEditUI* pREdt, COLORREF rgb)
{
    CHARFORMAT2 cf;
    cf.cbSize = sizeof(cf);
    pREdt->GetSelectionCharFormat(cf);
    cf.crTextColor = rgb;
    cf.dwMask |= CFM_COLOR;
    pREdt->SetSelectionCharFormat(cf);
}

void MainFrame::ShowSymbolStatistic(const CString& strWord, const CString& strSymbol)
{
    CTalkCount* pWnd = new CTalkCount(this);
    pWnd->m_strWord = strWord;
    pWnd->m_strSymbol = strSymbol;
    OpenModalWnd(pWnd, _T("发音统计"));
}

void MainFrame::ShowLoading()
{
    ShowWindow(false);
    m_pDlgLoading->ShowWindow(true);
    m_pDlgLoading->CenterWindow();
}

void MainFrame::HideLoading()
{
    m_pDlgLoading->ShowWindow(false);
    ShowWindow(true);
}

DWORD MainFrame::InstallVoice()
{
    CString strPath;

    strPath.Format(_T("%s%s"),
        CPaintManagerUI::GetInstancePath().GetData(),
        _T("Voice32.reg"));

    CFileFind finder;
    if (!finder.FindFile(strPath))
    {
        MyAfxMessageBox(_T("缺少必须的文件。请确认您的软件是否正确安装。"));
        return ERROR_FILE_NOT_FOUND;
    }
    finder.Close();

    CStdioFile cfreg;
    CFileException fe;
    //string sbuf;

    CString sTmp;
    MyReadAsciiFile(strPath, sTmp);

    strPath = CPaintManagerUI::GetInstancePath();
    /*去掉末尾的\*/
    strPath.TrimRight(_T('\\'));
    strPath.Replace(_T("\\"), _T("\\\\"));
    sTmp.Replace(_T("{Path}"), strPath);
    //sbuf.resize(sTmp.GetLength());
    //myT2A(&sbuf[0], sbuf.size(), sTmp);
    //sbuf = (LPSTR)CT2A(sTmp);

    strPath.Format(_T("%s%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("voicetmp32.reg"));
    ::DeleteFile(strPath);

    cfreg.Open(strPath, CFile::modeWrite | CFile::modeCreate | CFile::typeText, &fe);
    cfreg.WriteString(sTmp);
    cfreg.Close();

    //HINSTANCE hRet = ::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_HIDE);
    //TRACE(_T("ShellExecute return:%d\n"), hRet);

    return ERROR_SUCCESS;
}

bool MainFrame::CheckVoice(LPCWSTR wstrMyVoice, LPCWSTR wstrSysVoice, ISpObjectToken* pToken)
{
    bool bFound = false;
    HRESULT Hr;

    do 
    {
        if (lstrcmpW(wstrMyVoice, wstrSysVoice) != 0)
        {
            break;
        }
        try
        {
            CComPtr<ISpObjectToken> pOldToken;
            USHORT usVol = 0;
            Hr = m_cpVoice->GetVoice(&pOldToken);
            if (FAILED(Hr))
            {
                TRACE(_T("GetVoice failed here:0x%08x\n"), Hr);
                break;
            }
            Hr = m_cpVoice->GetVolume(&usVol);
            if (FAILED(Hr))
            {
                TRACE(_T("GetVolume failed here:0x%08x\n"), Hr);
                break;
            }
            Hr = m_cpVoice->SetVoice(pToken);
            if (FAILED(Hr))
            {
                TRACE(_T("SetVoice failed here:0x%08x\n"), Hr);
                break;
            }
            Hr = m_cpVoice->SetVolume(0);
            if (FAILED(Hr))
            {
                TRACE(_T("SetVolume failed here:0x%08x\n"), Hr);
                break;
            }
            StopSpeak();
            Hr = m_cpVoice->SetVolume(0);
            if (FAILED(Hr))
            {
                TRACE(_T("SetVolume 0 here:0x%08x\n"), Hr);
                break;
            }
            SpeakAndWait(_T(""));
            Hr = m_cpVoice->SetVoice(pOldToken);
            if (FAILED(Hr))
            {
                TRACE(_T("SetVoice pOldToken here:0x%08x\n"), Hr);
                break;
            }
            Hr = m_cpVoice->SetVolume(usVol);
            if (FAILED(Hr))
            {
                TRACE(_T("SetVolume usVol here:0x%08x\n"), Hr);
                break;
            }
            if (pOldToken)
            {
                pOldToken.Release();
            }
            TRACE("test voice %S OK\n", wstrSysVoice);
            bFound = true;
        }
        catch (...)
        {
            TRACE("failed to speak with %S\n", wstrSysVoice);
            bFound = false;
        }
    } while (FALSE);

    return bFound;
}

CString MainFrame::RegKeyGetValue(LPCTSTR strName)
{
    ASSERT(m_hRegKey);
    TCHAR   strValue[1024] = {_T('\0')};
    DWORD   dwLen = 1024, dwTyp = REG_SZ;
    ::RegQueryValueEx(m_hRegKey, strName, 0, &dwTyp, (LPBYTE)strValue, &dwLen);
    return CString(strValue);
}

void MainFrame::RegKeySetValue(LPCTSTR strName, LPCTSTR strValue)
{
    ASSERT(m_hRegKey);
    ::RegSetValueEx(m_hRegKey, strName, 0,
        REG_SZ, (const BYTE*)strValue, (lstrlen(strValue) + 1) * sizeof(TCHAR));
}

//////////////////////////////////////////////////////////////////////////
//注册函数
void MainFrame::OnPrepareRegister()
{
    m_nRegisterIdx = 1;

    memset(m_chRegBuffer, 0, sizeof(m_chRegBuffer));
    m_pNetReg->GetCoumputerID(m_chRegBuffer);
    m_strRegisterComputerId = CA2T(m_chRegBuffer).m_psz;
    if (m_strRegisterComputerId.IsEmpty())
    {
        AfxMessageBox(_T("获取机器码失败，软件无法运行。"));
        Close();
        return;
    }

    InitRegisterPage(m_nRegisterIdx);
}

extern CString g_strVersion;

void MainFrame::RegisterNextClicked()
{
    CEditUI* pEdt;
    CLabelUI* pLbl;
    CControlUI* pCtrl;

    if (1 == m_nRegisterIdx)
    {
    }
    else if (2 == m_nRegisterIdx)
    {
    }
    else if (3 == m_nRegisterIdx)
    {
        pEdt = xFindControl(CEditUI, _T("regteledt"));
        if (pEdt->GetText().IsEmpty())
        {
            AfxMessageBox(_T("请输入手机号。"));
            pEdt->SetFocus();
            return;
        }
        m_strRegisterMobile = pEdt->GetText();

        pEdt = xFindControl(CEditUI, _T("regsnedt"));
        if (pEdt->GetText().IsEmpty())
        {
            AfxMessageBox(_T("请输入序列号。"));
            pEdt->SetFocus();
            return;
        }
        m_strRegisterSN = pEdt->GetText();
        m_strRegisterSN.MakeUpper();

        int nFlag = 0;
        m_pNetReg->IsNetLess(&nFlag);
        if (1 == nFlag)
        {
            CT2A strSerialNumber(m_strRegisterSN);
            CT2A strMobile(m_strRegisterMobile);
            CT2A strComputerId(m_strRegisterComputerId);

            memset(m_chRegBuffer, '\0', sizeof(m_chRegBuffer));
            int nLen = _countof(m_chRegBuffer);
            HRESULT Hr;
            //注册，显示注册结果
            Hr = m_pNetReg->NetRegMember(_ProCode, _ProKey, (LPSTR)strSerialNumber, (LPSTR)strMobile, strComputerId, m_chRegBuffer, &nLen);
            dump_hex(_T("NetRegMember return"), m_chRegBuffer, lstrlenA(m_chRegBuffer));
            if (lstrlenA(m_chRegBuffer) == 0)
            {
                StringCchCopyA(m_chRegBuffer, _countof(m_chRegBuffer), "2");
            }
            if (/*lstrcmpA(m_chRegBuffer, "1")*/m_chRegBuffer[0] != '1')
            {
                CString lpInfo;
                string xstr[2] = {"\0"};
                xstr[0] = m_chRegBuffer[0];
                int eno = atoi(xstr->c_str());
                switch(eno)
                {
                case 0:
                    lpInfo = _T("商家密钥错误。");
                    break;
                case 2:
                    lpInfo = _T("注册失败。");
                    break;
                case 3:
                    lpInfo = _T("当天IP注册次数过多。");
                    break;
                case 4:
                    lpInfo = _T("序列号错误。");
                    break;
                case 5:
                    lpInfo = _T("序列号使用超过次数。");
                    break;
                case 6:
                    lpInfo = _T("数据不能为空。");
                    break;
                default:
                    lpInfo.Format(_T("未知错误:%d。"), eno);
                    break;
                }
                pLbl = xFindControl(CLabelUI, _T("regsresult"));
                pLbl->SetText(lpInfo);
                return;
            }
            RegisterSuccessed();
            return;
        }
        //否则进入下一步
    }
    else if (4 == m_nRegisterIdx)
    {
    }

    m_nRegisterIdx++;
    InitRegisterPage(m_nRegisterIdx);

    if (2 == m_nRegisterIdx)
    {
    }
    else if (3 == m_nRegisterIdx)
    {
        pLbl = xFindControl(CLabelUI, _T("regsresult"));
        pLbl->SetText(_T(""));

        pEdt = xFindControl(CEditUI, _T("regteledt"));
        pEdt->SetFocus();
    }
    else if (4 == m_nRegisterIdx)
    {
        pLbl = xFindControl(CLabelUI, _T("regauthresult"));
        pLbl->SetText(_T(""));

        pEdt = xFindControl(CEditUI, _T("regauthcode"));
        pEdt->SetText(_T(""));

        //CT2A strSerialNumber(m_strRegisterSN);
        //CT2A strMobile(m_strRegisterMobile);
        //CT2A strComputerId(m_strRegisterComputerId);

        //生成二维码
        RegisterQRCode();
        pCtrl = xFindControl(CControlUI, _T("regstep4qrcode"));
        pCtrl->SetBkImage(m_strRegisterQRFile);

        //电话
        pLbl = xFindControl(CLabelUI, _T("regstep4tel"));
        pLbl->SetText(m_strRegisterMobile);

        //序列号
        pLbl = xFindControl(CLabelUI, _T("regstep4sn"));
        pLbl->SetText(m_strRegisterSN);

        //机器码
        pLbl = xFindControl(CLabelUI, _T("regstep4id"));
        pLbl->SetText(m_strRegisterComputerId);

        pEdt = xFindControl(CEditUI, _T("regauthcode"));
        pEdt->SetFocus();
    }
}

void MainFrame::RegisterOkClick()
{
    //检查验证码
    CEditUI* pEdt = xFindControl(CEditUI, _T("regauthcode"));
    if (pEdt->GetText().IsEmpty())
    {
        AfxMessageBox(_T("请输入验证码。"));
        pEdt->SetFocus();
        return;
    }

    CT2A strSerialNumber(m_strRegisterSN);
    CT2A strMobile(m_strRegisterMobile);
    CT2A strComputerId(m_strRegisterComputerId);

    memset(m_chRegBuffer, '\0', sizeof(m_chRegBuffer));
    HRESULT Hr = m_pNetReg->ValiDationCode(_ProCode, _ProKey, strSerialNumber, strMobile, strComputerId, m_chRegBuffer);
    TRACE("local auth code:%s\n", m_chRegBuffer);
    CString x = pEdt->GetText().GetData();
    x.MakeUpper();
    if (x.CompareNoCase(CA2T(m_chRegBuffer)) == 0)
    {
        RegisterSuccessed();
    }
    else
    {
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("regauthresult"));
        pLbl->SetText(_T("验证码错误。"));
    }
}

void MainFrame::RegisterSuccessed()
{
    //AfxMessageBox(_T("注册成功！请重新运行程序。"),
    AfxMessageBox(_T("注册成功！"),
        MB_OK | MB_ICONINFORMATION);

    RegKeySetValue(REG_Mobile, m_strRegisterMobile);
    m_strRegisterMobile.Empty();
    RegKeySetValue(REG_ComputerId, m_strRegisterComputerId);
    m_strRegisterComputerId.Empty();
    RegKeySetValue(REG_Version, g_strVersion);
    RegKeySetValue(REG_SerialNumber, m_strRegisterSN);
    m_strRegisterSN.Empty();
    RegKeySetValue(REG_ProCode, CA2T(_ProCode));
    RegKeySetValue(REG_ProKey, CA2T(_ProKey));

    //Close(IDCANCEL);
    m_bRegistered = true;
    TNotifyUI msg;
    msg.pSender = xFindControl(CControlUI, kCloseButtonName);
    msg.sType = DUI_MSGTYPE_CLICK;
    m_PaintManager.SendNotify(msg);
}

void MainFrame::RegisterPreClicked()
{
    m_nRegisterIdx--;
    InitRegisterPage(m_nRegisterIdx);
}

void MainFrame::RegisterTabSelected()
{
    ;
}

void MainFrame::InitRegisterPage(int nIndex)
{
    ASSERT(nIndex >= 1 && nIndex <= 4);

    CTabLayoutUI* pTab = xFindControl(CTabLayoutUI, _T("regtab"));
    pTab->SelectItem(nIndex - 1);
}

void MainFrame::RegisterQRCode()
{
    CT2A strSerialNumber(m_strRegisterSN);
    CT2A strMobile(m_strRegisterMobile);
    CT2A strComputerId(m_strRegisterComputerId);

    int nWid = 0, nHeight = 0;
    memset(m_chRegBuffer, '\0', sizeof(m_chRegBuffer));
    int nLen = _countof(m_chRegBuffer);
    HRESULT Hr = m_pNetReg->DiMentionCode(_ProCode, _ProKey, strSerialNumber, strMobile, strComputerId, m_chRegBuffer, &nWid, &nHeight);
    TRACE("width:%d, height:%d\n", nWid, nHeight);

    DWORD *pDest = new DWORD[nWid * nHeight];
    int x, y;
    for(y=0; y<nHeight; y++)
    {
        for(x=0; x<nWid; x++)
        {
            if(m_chRegBuffer[y*nWid+x]!=0)
                pDest[y*nWid+x]=RGB(255,255,255);
            else
                pDest[y*nWid+x]=RGB(0,0,0);
        }
    }
    BITMAPINFO bmi;
    ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = nWid;
    bmi.bmiHeader.biHeight = -nHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = nWid * nHeight * 4;
    Bitmap *pbtbmp = Bitmap::FromBITMAPINFO(&bmi, pDest);
    CLSID bmpClsid;
    GetEncoderClsid(L"image/png", &bmpClsid);
    pbtbmp->Save(CT2W(m_strRegisterQRFile), &bmpClsid);
    delete[] pDest;
    delete pbtbmp;
}

BOOL MainFrame::CheckUpdate(BOOL bRun, const CString& strExe)
{
    BOOL bRt = TRUE;
    CString strTmp, strUpdate;

    do 
    {
        if (bRun)
        {
            break;
        }
        //总是检查Loader的更新
        CFileFind finder;
        CString strPath, strInst = CPaintManagerUI::GetInstancePath().GetData(), strDest;
        strPath = strInst + _T("System\\Update\\Loader\\*.*");
        BOOL bWorking = finder.FindFile(strPath);
        while(bWorking)
        {
            bWorking = finder.FindNextFile();
            if (finder.IsDots())
            {
                continue;
            }
            if (finder.IsDirectory())
            {
                continue;
            }
            strDest = strInst + finder.GetFileName();
            ::MoveFileEx(finder.GetFilePath(), strDest, MOVEFILE_REPLACE_EXISTING);
        }
        finder.Close();
        //不检查更新
        strUpdate = RegKeyGetValue(REG_Updateautocheck);
        if (strUpdate == _T("0"))
        {
            break;
        }
        strTmp = RegKeyGetValue(REG_Updateinterval);
        LONGLONG ullIntv;
        switch (_tstoi(strTmp))
        {
        case 0:         //每天
            ullIntv = 24 * 60 * 60ULL;
            break;
        case 1:         //每周
            ullIntv = 7 * 24 * 60 * 60ULL;
            break;
        case 2:         //每月
            ullIntv = 30 * 7 *24 * 60 * 60ULL;
            break;
        case 3:         //启动时
            ullIntv = 0;
            break;
        default:
            ullIntv = 7 * 24 * 60 * 60ULL;
            break;
        }
        strTmp = RegKeyGetValue(REG_Updatelasttime);
        CTime tnow(CTime::GetCurrentTime());
        CTime tlast((time_t)_tstoi64(strTmp));
        LONGLONG lll = CTimeSpan(tnow - tlast).GetTotalSeconds();
        if ( (lll >= 0) &&
            (lll < ullIntv) )
        {
            break;
        }
        strTmp.Format(_T("%I64d"), (LONGLONG)tnow.GetTime());
        RegKeySetValue(REG_Updatelasttime, strTmp);

#if SKIN==SKN_WIN8
        CString strType = _T("win8");
#else
        CString strType = _T("smart");
#endif
        CString strLoader, strParam;
        strParam.Format(_T("/version=%s /type=%s /procode=%s /prokey=%s /caller=\"%s\""),
            g_strVersion, strType, (LPTSTR)CA2T(_ProCode), (LPTSTR)CA2T(_ProKey), strExe);
        strLoader.Format(_T("%s%s"), CPaintManagerUI::GetInstancePath().GetData(), LoaderName);
        TRACE(_T("strLoader:%s, param:%s\n"), strLoader, strParam);
#if 1
        SHELLEXECUTEINFO si = {0};
        si.cbSize = sizeof(si);
        //si.fMask = SEE_MASK_NO_CONSOLE;
        si.lpFile = (LPCTSTR)strLoader;
        si.lpParameters = (LPCTSTR)strParam;
        si.lpDirectory = _T(".");
        //si.lpVerb = _T("runas");
        si.lpVerb = _T("open");
        si.nShow = SW_SHOW;
        if (!::ShellExecuteEx(&si))
        {
            DWORD dwErr = GetLastError();
            MyAfxMessageBox(_T("程序无法正常启动：%s"), MyFormatMessage(dwErr));
            bRt = FALSE;
            break;
        }
#else
        int e = (int)::ShellExecute(NULL, _T("open"), strLoader, strParam, _T("."), SW_HIDE);
        TRACE(_T("ShellExecute return:%d\n"), e);
        if (e <= 32)
        {
            MyAfxMessageBox(_T("程序无法正常启动，错误码：%d"), e);
            bRt = FALSE;
            break;
        }
#endif

        bRt = FALSE;

    } while (false);

    return bRt;
}

bool MainFrame::CheckMaterial()
{
    if (!m_bMaterialInstalled)
    {
        MyAfxMessageBox(_T("%s"), _T("没有可用的素材。请下载并安装素材包。"));
        TNotifyUI tmsg;
        tmsg.pSender = xFindControl(CButtonUI, kCloseButtonName);
        tmsg.sType = DUI_MSGTYPE_CLICK;
        m_PaintManager.SendNotify(tmsg);
    }
    return m_bMaterialInstalled;
}

void MainFrame::OnPrepareMain()
{
    ASSERT(m_pNetReg);

    if (m_bRegistered)
    {
        return;
    }

    CT2A strMobile(RegKeyGetValue(REG_Mobile));
    CT2A strComputerId(RegKeyGetValue(REG_ComputerId));
    //CT2A strVersion(g_strVersion);
    CT2A strSerialNumber(RegKeyGetValue(REG_SerialNumber));
    CString strCode = RegKeyGetValue(REG_ProCode);
    CString strKey = RegKeyGetValue(REG_ProKey);

    //static char strProCode[] = "BS";
    //static char strProKey[] = "BS00001523561";

    int nFlag = 0, nLen;
    bool bRegistered = false;
    HRESULT Hr;

    do 
    {
        if (strCode.IsEmpty() ||
            strKey.IsEmpty() ||
            lstrlenA((LPCSTR)strComputerId) == 0 ||
            lstrlenA((LPCSTR)strSerialNumber) == 0 ||
            lstrlenA((LPCSTR)strMobile) == 0)
        {
            TRACE("something empty, need to register\n");
            break;
        }

        if (strCode != (LPTSTR)CA2T(_ProCode))
        {
            TRACE(_T("bad ProCode:%s, should be:%s\n"), (LPTSTR)CA2T(_ProCode), strCode);
            break;
        }
        if (strKey != (LPTSTR)CA2T(_ProKey))
        {
            TRACE(_T("bad ProKey:%s, should be:%s\n"), (LPTSTR)CA2T(_ProKey), strKey);
            break;
        }

        try
        {
            //电话、序列号、ComputerID生成UserKey
            memset(m_chRegBuffer, '\0', sizeof(m_chRegBuffer));
            Hr = m_pNetReg->UserKeys(strMobile, strSerialNumber, strComputerId, m_chRegBuffer);

            //验证是否注册
            nFlag = 0;
            Hr = m_pNetReg->LoginValidator(strMobile, strSerialNumber, strComputerId, m_chRegBuffer, &nFlag);
            if (!nFlag)
            {
                TRACE("LoginValidator failed\n");
                break;
            }

            Hr = m_pNetReg->IsNetLess(&nFlag);
            if (!nFlag)
            {
                bRegistered = true;
                break;
            }

            memset(m_chRegBuffer, '\0', sizeof(m_chRegBuffer));
            //ComputerID与序列号检查
            nLen = _countof(m_chRegBuffer);
            Hr = m_pNetReg->SntoComputerID(strSerialNumber, strComputerId, m_chRegBuffer, &nLen);
            if (m_chRegBuffer[0] != '1')
            {
                TRACE("SntoComputerID failed:%s\n", m_chRegBuffer);
                break;
            }

            //检查是否有新版本
            //不检查升级，单独的进程完成
            //CheckUpdate();

            RegKeySetValue(REG_Version, g_strVersion);

            bRegistered = true;
        }
        catch (CException* e)
        {
            e->Delete();
        }
        catch (_com_error& e)
        {
            TRACE(_T("com error:%s\n"), e.ErrorMessage());
            UNREFERENCED_PARAMETER(e);
        }
    } while (0);

    m_bRegistered = bRegistered;

    if (!bRegistered)
    {
        OpenChildModalWnd(sRegisterSkin);
    }
}

int MainFrame::GetAiEngineSN()
{
    CString strSerialSpeech = RegKeyGetValue(_T("SpeechSerial"));
    //d83d-906b-0740-6eb7-f593
    if (strSerialSpeech.GetLength() == 0x18)
    {
        TRACE(_T("we already have it:%s\n"), strSerialSpeech);
        return 0;
    }
    //if (strSerialSpeech.IsEmpty())
    //6b65-c92e-bb6f-cc63-1a5b
    int ai = 0;
    if (strSerialSpeech.IsEmpty() || (strSerialSpeech.GetLength() != 24))
    {
        char cstrSN[1024] = {'\0',};
        Json::Reader jreader;
        Json::Value jroot;
        Json::Value jsn;

        StringCchPrintfA(cstrSN, _countof(cstrSN), "{\"appKey\": \"%s\",\"secretKey\": \"%s\"}", m_AiEngine.m_appKey, m_AiEngine.m_secretKey);
        do 
        {
            ai = m_AiEngine.aiengine_opt(NULL, CAiEngine::AIENGINE_GET_SERIAL_NUMBER, (LPBYTE)cstrSN, sizeof(cstrSN));
            TRACE(_T("aiengine_opt return:%d, %s\n"), ai, (LPTSTR)CA2T(cstrSN));
            //aiengine_opt return:45, {"Get serial number error, Network abnormal"}
            //aiengine_opt return:76, {"serialNumber":"4b8c-2938-7012-215b-9728","tips":"deviceId already exists"}

            TRACE("get sn:%s\n", cstrSN);
            if (!jreader.parse(cstrSN, jroot))
            {
                TRACE("sn parse error:%s\n", jreader.getFormatedErrorMessages().c_str());
                ai = AfxMessageBox(_T("启用语音识别引擎失败：格式错误，请确认能够访问互联网。\r\n如果忽略此错误评分功能将不可用。"),
                    MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_DEFBUTTON2);
            }
            else
            {
                jsn = jroot["serialNumber"];
                if (jsn.isNull())
                {
                    ai = AfxMessageBox(_T("启用语音识别引擎失败：无SN值，请确认能够访问互联网。\r\n如果忽略此错误评分功能将不可用。"),
                        MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_DEFBUTTON2);
                }
                else
                {
                    ai = 0;
                    TRACE("get sn result:%s\n", jsn.asCString());
                    RegKeySetValue(REG_SpeechSerial, CA2T(jsn.asCString()));

                    Json::Value jpro = jroot["provision"];
                    if (!jpro.isNull())
                    {
                        TRACE(_T("3月体验版\n"));
                        //试用/体验版本
                        CString strFile= CPaintManagerUI::GetInstancePath().GetData();
                        strFile += _T("assets\\aiengine.provision");
                        CFile hf;
                        CFileException fe;
                        if (!hf.Open(strFile, CFile::modeCreate | CFile::modeWrite, &fe))
                        {
                            ai = AfxMessageBox(_T("启用语音识别引擎失败（3月体验版），请确认能够访问互联网。\r\n如果忽略此错误评分功能将不可用。"),
                                MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_DEFBUTTON2);
                        }
                        else
                        {
                            BYTE decode[1024];
                            int decodeLen = 1024;
                            BOOL bDecRet = Base64Decode(jpro.asString().c_str(), jpro.asString().size(), decode, &decodeLen);
                            TRACE(_T("Base64Decode return:%s, len%d\n"), bDecRet ? _T("TRUE") : _T("FALSE"), decodeLen);
                            hf.Write(decode, decodeLen);
                            hf.Close();
                        }
                    }
#ifdef _DEBUG
                    Json::Value jtips = jroot["tips"];
                    if (!jtips.isNull())
                    {
                        TRACE("get sn tips:%s\n", jtips.asCString());
                    }
#endif
                    break;
                }
            }
        } while (ai == IDRETRY);
    }

    return ai;

#if 0
    /*
    以下代码用于获取Speech序列号
    */
    string strTimeStamp = m_AiEngine.GetTimeStamp();
    string strID = m_AiEngine.GetDeviceId();
    string str5 = "?appKey=";
    str5 += m_AiEngine.m_appKey;
    str5 += "&timestamp=";
    str5 += strTimeStamp;
    str5 += "&deviceId=";
    str5 += strID;
    str5 += "&sig=";
    str5 += m_AiEngine.GetSAH(strTimeStamp, strID);
    string strBuilder = "POST /device";
    strBuilder += str5;
    strBuilder += " HTTP/1.1\r\n";
    strBuilder += "Host: auth.api.aispeech.com\r\n";
    strBuilder += "Connection: close\r\n";
    strBuilder += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";

    //ansi ==> unicode ==> utf8
    CA2W wstrBuilder(strBuilder.c_str());
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, wstrBuilder, -1, NULL, 0, NULL, NULL);
    strBuilder.clear();
    strBuilder.resize(nLen);
    nLen = ::WideCharToMultiByte(CP_UTF8, 0, wstrBuilder, -1, &strBuilder[0], nLen, NULL, NULL);

    SOCKET hSk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in speer = {0};
    speer.sin_family = AF_INET;
    speer.sin_port = htons(80);
    speer.sin_addr.s_addr = inet_addr("183.129.160.237");
    int x = connect(hSk, (struct sockaddr*)&speer, sizeof(speer));
    if (SOCKET_ERROR == x)
    {
        TRACE(_T("connect to server failed:%d\n"), WSAGetLastError());
        closesocket(hSk);
        return;
    }
    x = send(hSk, &strBuilder[0], nLen, 0);
    string strRecv;
    char rcvbuf[0x400];
    while((x = recv(hSk, rcvbuf, sizeof(rcvbuf), 0)) > 0)
    {
        rcvbuf[x] = '\0';
        strRecv += rcvbuf;
    }
    shutdown(hSk, SD_BOTH);
    closesocket(hSk);
    TRACE("send to server:%s\n", strBuilder.c_str());
    TRACE("recv from server(UFT8):%s\n", strRecv.c_str());

    /*HTTP/1.1 200 OK
Server: openresty/1.7.2.1
Date: Tue, 01 Sep 2015 08:50:17 GMT
      Content-Type: text/html; charset=utf-8
Connection: close
            X-Powered-By: PHP/5.4.16

        {"serialNumber":"d83d-906b-0740-6eb7-f593","tips":"a new deviceId"}

        or

        {"error":"not found, invalid appKey"}

        */
    Json::Reader jreader;
    Json::Value jroot;
    string::size_type nrn = strRecv.find('{');
    if (nrn == string::npos)
    {
        TRACE(_T("not find result\n"));
        return;
    }
    TRACE("json:%s\n", strRecv.substr(nrn).c_str());
    if (!jreader.parse(strRecv.substr(nrn), jroot))
    {
        TRACE("jreader.parse failed\n");
        return;
    }
    Json::Value jsn = jroot["serialNumber"];
    if (jsn.isNull())
    {
#ifdef _DEBUG
        Json::Value jerr = jroot["error"];
        if (!jerr.isNull())
        {
            TRACE("get sn failed:%s\n", jerr.asCString());
        }
#endif
    }
    else
    {
        RegKeySetValue(_T("SpeechSerial"), (LPCTSTR)CA2T(jsn.asCString()));
        TRACE(_T("we get SpeechSerial:%s\n"), (LPCTSTR)CA2T(jsn.asCString()));
    }
#ifdef _DEBUG
    Json::Value jtips = jroot["tips"];
    if (!jtips.isNull())
    {
        TRACE(_T("we get tips:%s\n"), (LPCTSTR)CA2T(jtips.asCString()));
    }
#endif

#endif
}

void MainFrame::InitWindow()
{
    //TRACE("%s\n", __FUNCTION__);
    BOOL bOK = FALSE;
    DWORD dwErr;
    CString strErr = _T("");

    m_hAiEngine = NULL;

    DuiLib::CWaitCursor cw;
    do 
    {
        m_pDlgLoading = new CDialogLoading(this);
        m_pDlgLoading->Create(GetHWND(), _T(""), 0, WS_EX_TOOLWINDOW);
        //m_pDlgLoading->ShowWindow(true);

        //m_pDlgSR = new CDialogSR(this);
        //m_pDlgSR->Create(GetHWND(), _T(""), 0, WS_EX_TOOLWINDOW);

        GetEncoderClsid(L"image/png", &m_clsidpng);

        HRESULT Hr = m_pNetReg.CreateInstance(__uuidof(Reg));
        TRACE("CreateInstance IReg return:0x%x\n", Hr);
        if (FAILED(Hr))
        {
            //REGDB_E_CLASSNOTREG??==(0x80040154L)
            //strErr = _T("程序出现严重错误，COM组件未正确注册，请确认程序正确安装且在安装过程中允许COM组件注册。");
            if (AfxMessageBox(_T("程序出现严重错误，COM组件未正确注册，请确认程序正确安装且在安装过程中允许COM组件注册。\r\n是否现在自动注册？"),
                    MB_YESNO | MB_ICONQUESTION) != IDYES)
            {
                break;
            }
            TCHAR sysPath[MAX_PATH] = {0};
            GetSystemDirectory(sysPath, _countof(sysPath));
            CString strDll;
            strDll.Format(_T("%s\\NYRlib.dll"), sysPath);
#if 1
            SHELLEXECUTEINFO si = {0};
            si.cbSize = sizeof(si);
            si.fMask = SEE_MASK_FLAG_NO_UI;
            si.lpFile = _T("regsvr32");
            si.lpParameters = (LPCTSTR)strDll;
            si.lpDirectory = _T(".");
            si.lpVerb = IsAdmin() ? _T("open") : _T("runas");
            si.nShow = SW_HIDE;
            if (!::ShellExecuteEx(&si))
            {
                dwErr = GetLastError();
                strErr.Format(_T("COM组件注册失败：%s"), MyFormatMessage(dwErr));
                break;
            }
            //wait it
            //if (si.hProcess)
            //{
            //    ::WaitForSingleObject(si.hProcess, INFINITE);
            //    ::CloseHandle(si.hProcess);
            //}
#else
            typedef HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);
            HMODULE hDll = ::LoadLibraryEx(strDll, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
            if (NULL == hDll)
            {
                break;
            }
            lpDllEntryPoint DllEntryPoint = (lpDllEntryPoint)GetProcAddress(hDll, "DllRegisterServer");
            if (NULL == DllEntryPoint)
            {
                ::FreeLibrary(hDll);
                break;
            }
            Hr = DllEntryPoint();
            ::FreeLibrary(hDll);
            if (FAILED(Hr))
            {
                break;
            }
#endif
            int iWait = 10, i;
            for (i = 0; i < iWait; i++)
            {
                Hr = m_pNetReg.CreateInstance(__uuidof(Reg));
                if (FAILED(Hr))
                {
                    Sleep(1000);
                }
            }
            if (FAILED(Hr))
            {
                TRACE(_T("m_pNetReg.CreateInstance still failed:0x%x\n"), Hr);
                dwErr = HRESULT_CODE(Hr);
                break;
            }
        }

        dwErr = m_ssEvalMgr.Open();

        //m_hAiEngine = NULL;
        //dwErr = m_AiEngine.LoadFunc();
#ifndef APP_DEMO 
//#if 0
        if (ERROR_SUCCESS != dwErr)
        {
            strErr.Format(_T("语音识别引擎加载失败：%s"), MyFormatMessage(dwErr));
            break;
        }

        m_ssEvalMgr.SetEvalCallback(this);

//        if (GetAiEngineSN() != 0)
//        {
//            break;
//        }
//
//        TRACE(_T("尝试采用新语音识别引擎。\n"));
//        char data[AI_DAT_LEN + 1] = {'\0'};
//        m_AiEngine.aiengine_opt(NULL, CAiEngine::AIENGINE_OPT_GET_VERSION, (LPBYTE)data, AI_DAT_LEN);
//        TRACE("ai version:%s\n", (const char*)data);
//        CString strTmp, strSerialSpeech = RegKeyGetValue(_T("SpeechSerial"));
//        //strTmp.Format(_T("{\"appKey\":\"%s\",\"secretKey\": \"%s\", \"provision\": \"./assets/aiengine.provision\", \"serialNumber\": \"%s\",\"native\": {\"en.sent.score\":{\"res\": \"./resource/eval/bin/eng.snt.splp.0.10\"}}}"),
//        //    (LPCTSTR)CA2T(m_AiEngine.m_appKey), (LPCTSTR)CA2T(m_AiEngine.m_secretKey), strSerialSpeech);
//        strTmp.Format(_T("\
//{\
//    \"appKey\": \"%s\",\
//    \"secretKey\": \"%s\",\
//    \"provision\": \"assets/aiengine.provision\",\
//    \"native\": {\
//        \"en.word.score\": {\
//            \"res\": \"resource/eval/bin/eng.wrd.g4.P2.0.4\"\
//        },\
//        \"en.sent.score\": {\
//            \"res\": \"resource/eval/bin/eng.snt.g4.P2.0.7\"\
//        },\
//        \"en.pred.exam\": {\
//            \"res\": \"resource/exam/bin/eng.pred.aux.P3.V4.11\"\
//        }\
//    }\
//}"),  (LPTSTR)CA2T(m_AiEngine.m_appKey), (LPTSTR)CA2T(m_AiEngine.m_secretKey));
//        TRACE(_T("AiEngine cfg:%s\n"), strTmp);
//        TRACE(_T("SpeechSerial:%s\n"), strSerialSpeech);
//        m_hAiEngine = m_AiEngine.aiengine_new(CT2A(strTmp));
//        TRACE(_T("aiengine_new return %p\n"), m_hAiEngine);
//        TRACE(_T("采用新语音识别引擎:%s\n"),
//            m_hAiEngine ? _T("成功") : _T("失败"));
//        if (NULL == m_hAiEngine)
//        {
//            strErr = _T("创建语音识别引擎实例失败。");
//#ifndef _DEBUG
//            break;
//#endif
//        }
#endif

        int i;
        SHORT vk = VK_INSERT;//VkKeyScan(_T('c')) & 0xff;
        memset(m_ReadBarCtrlC, 0, sizeof(m_ReadBarCtrlC));
        for (i = 0; i < _countof(m_ReadBarCtrlC); i++)
        {
            m_ReadBarCtrlC[i].type = INPUT_KEYBOARD;
            //???
            m_ReadBarCtrlC[i].ki.dwExtraInfo = GetMessageExtraInfo();
        }
        m_ReadBarCtrlC[0].ki.wVk = VK_CONTROL;
        m_ReadBarCtrlC[1].ki.wVk = vk;
        m_ReadBarCtrlC[2].ki.wVk = vk;
        m_ReadBarCtrlC[2].ki.dwFlags |= KEYEVENTF_KEYUP;
        m_ReadBarCtrlC[3].ki.wVk = VK_CONTROL;
        m_ReadBarCtrlC[3].ki.dwFlags |= KEYEVENTF_KEYUP;

        TRACKMOUSEEVENT te = {0};
        te.cbSize = sizeof(TRACKMOUSEEVENT);
        te.dwFlags = TME_HOVER | TME_NONCLIENT;
        te.hwndTrack = GetHWND();
        te.dwHoverTime = 10;//HOVER_DEFAULT;
        ::TrackMouseEvent(&te);

        m_pchrFileBuffer = new char[m_dwFileBufferSize];
        if (NULL == m_pchrFileBuffer)
        {
            strErr = _T("内存不足。");
            break;
        }
        CString strDb;
        strDb.Format(_T("%s\\db\\%s"), CPaintManagerUI::GetInstancePath().GetData(), g_strDBaseFile);
        if (!m_DbFile.Open(strDb))
        {
            break;
        }

        //HRESULT Hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_cpVoice);
        Hr = m_cpVoice.CoCreateInstance(CLSID_SpVoice);
        if( FAILED( Hr ) )
        {
            strErr.Format(_T("打开TTS_Voice失败！错误代码：0x%08x"), Hr);
            break;
        }

        Hr = m_cpVoice->SetNotifyWindowMessage(GetHWND(), WM_TTSAPPCUSTOMEVENT, 0, 0);
        // We're interested in all TTS events
        if( FAILED(Hr) )
        {
            strErr = _T("打开TTS_Notify失败！");
            break;
        }
        Hr = m_cpVoice->SetInterest( SPFEI_ALL_TTS_EVENTS, SPFEI_ALL_TTS_EVENTS );
        if( FAILED(Hr) )
        {
            strErr = _T("打开TTS_Interest失败！");
            break;
        }

        CComPtr<ISpObjectToken>        cpVoiceToken;
        CComPtr<IEnumSpObjectTokens>   cpEnum;
        try
        {
            Hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
            if (SUCCEEDED(Hr))
            {
                bool bWoman = false, bMan = false;
                while (cpEnum->Next(1, &cpVoiceToken, NULL) == S_OK)
                {
                    CSpDynamicString dstrDesc;
                    Hr = SpGetDescription(cpVoiceToken, &dstrDesc);
                    TRACE("voice:%S\n", dstrDesc.m_psz);
                    if (CheckVoice(L"Woman Voice", dstrDesc.m_psz, cpVoiceToken))
                    {
                        bWoman = true;
                        m_cpVoice->SetVoice(cpVoiceToken);
                    }
                    if (CheckVoice(L"Man Voice", dstrDesc.m_psz, cpVoiceToken))
                    {
                        bMan = true;
                        m_cpVoice->SetVoice(cpVoiceToken);
                    }
                    //we set it
                    LANGID lid;
                    Hr = SpGetLanguageFromToken(cpVoiceToken, &lid);
                    if (lid == 0x0409)
                    {
                        TRACE("we set voice to:%S\n", dstrDesc.m_psz);
                        m_cpVoice->SetVoice(cpVoiceToken);
                    }
                    cpVoiceToken.Release();
                }
                if (!(bWoman && bMan))
                {
                    TRACE(_T("语音库失败，安装语音库。\n"));
                    //InstallVoice();
#ifdef _DEBUG
                    m_cpVoice->GetVoice(&cpVoiceToken);
                    CSpDynamicString dstrDesc2;
                    SpGetDescription(cpVoiceToken, &dstrDesc2);
                    TRACE("default voice:%S\n", dstrDesc2.m_psz);
                    //m_cpVoice->SetVoice(cpVoiceToken);
                    cpVoiceToken.Release();
#endif
                }
                cpEnum.Release();
            }
        }
        catch (_com_error& e)
        {
            strErr.Format(_T("%s"), e.ErrorMessage());
            break;
        }
        catch (CMemoryException* e)
        {
            TRACE(_T("CMemoryException\n"));
            e->GetErrorMessage(strErr.GetBufferSetLength(1024), 1024);
            strErr.ReleaseBuffer();
        	e->Delete();
            break;
        }
        catch (CException* e)
        {
            TRACE(_T("CException\n"));
            e->GetErrorMessage(strErr.GetBufferSetLength(1024), 1024);
            strErr.ReleaseBuffer();
            e->Delete();
            break;
        }

#ifdef _DEBUG
        SpeakAndWait(_T("test voice, can you here then?"));
#endif

#if 1
        Hr = m_cpRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer);
        if (FAILED(Hr))
        {
            TRACE(_T("m_cpRecognizer.CoCreateInstance failed:0x%x\n"), Hr);
            strErr = _T("打开语音识别功能失败，程序无法运行。请确认安装并启用语音识别功能。");
            break;
        }
        SpGetDefaultTokenFromCategoryId(SPCAT_AUDIOIN, &cpVoiceToken);
        m_cpRecognizer->SetInput(cpVoiceToken, TRUE);
        cpVoiceToken.Release();

        //CComPtr<ISpAudio>            cpAudio;
        //SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
        //m_cpRecognizer->SetInput(cpVoiceToken, TRUE); //cpAudio.Release();

        //0x8004503a错误，如果未安装Speech
        Hr = m_cpRecognizer->CreateRecoContext(&m_cpRecoCtxt);
        //REGDB_E_CLASSNOTREG错误，如果未安装Speech
        //Hr = m_cpRecoCtxt.CoCreateInstance(CLSID_SpInProcRecoContext);
        if (FAILED(Hr))
        {
            TRACE(_T("CreateRecoContext failed:0x%x\n"), Hr);
            strErr = _T("打开语音识别功能失败，程序无法运行。请确认安装并启用语音识别功能。");
            break;
        }

        //Hr = m_cpRecoCtxt->SetNotifyWin32Event();
        m_cpRecoCtxt->SetNotifyWindowMessage(GetHWND(), WM_SRAPPCUSTOMEVENT, 0, 0);
        Hr = m_cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_END_SR_STREAM), SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_END_SR_STREAM));   //SPEI_HYPOTHESIS
        //Hr = m_cpRecoCtxt->SetInterest(SPFEI_ALL_SR_EVENTS, SPFEI_ALL_SR_EVENTS);
        Hr = m_cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL);
        Hr = m_cpRecoCtxt->CreateGrammar(1, &m_cpGrammar);
        if (FAILED(Hr))
        {
            TRACE(_T("CreateGrammar failed:0x%x\n"), Hr);
            strErr = _T("创建语法识别对象失败，程序无法运行。请确认安装并启用语音识别功能。");
            break;
        }
        Hr = m_cpGrammar->LoadDictation(NULL, SPLO_STATIC);
        //Hr = m_cpGrammar->SetDictationState(SPRS_ACTIVE);

        Hr = SpEnumTokens(SPCAT_RECOGNIZERS, NULL, NULL, &cpEnum);
        if (SUCCEEDED(Hr))
        {
#ifdef _DEBUG
            vector<wstring> vcReg;
#endif
            while (cpEnum->Next(1, &cpVoiceToken, NULL) == S_OK)
            {
                CSpDynamicString dstrDesc;
                Hr = SpGetDescription(cpVoiceToken, &dstrDesc);
                LANGID lid;
                Hr = SpGetLanguageFromToken(cpVoiceToken, &lid);
                TRACE("RECOGNIZERS:%S, LANGID:0x%x\n", dstrDesc.m_psz, (int)lid);
                //or, use PRIMARYLANGID?
                if ( (lid == 0x409) &&
                    (wcsstr(dstrDesc.m_psz, L"Developer Sample Engine") == NULL) )
                {
                    TRACE(_T("we select Recognizer:%s\n"), dstrDesc.m_psz);
                    m_cpRecognizer->SetRecognizer(cpVoiceToken);
                }
#ifdef _DEBUG
                vcReg.push_back(dstrDesc.m_psz);
#endif
                cpVoiceToken.Release();
            }
            cpEnum.Release();
#ifdef _DEBUG
            vector<wstring>::const_iterator p;
            TRACE(_T("RECOGNIZERS num:%d\n"), vcReg.size());
            for (p = vcReg.begin(); p != vcReg.end(); ++p)
            {
                TRACE("RECOGNIZERS:%S\n", p->c_str());
            }
#endif
        }
        //Hr = m_cpGrammar->SetDictationState(SPRS_ACTIVE);
#endif

        if (ERROR_SUCCESS != m_WaveRecorder.Open(GetHWND()))
        {
            //strErr = _T("系统没有检测到录音设备，本软件无法运行。");
            break;
        }
        TRACE(_T("wave in volume:%d\n"), m_WaveRecorder.GetWaveInVolume());

        if (ERROR_SUCCESS != m_mciWave.OpenDevice())
        {
            strErr = _T("系统没有检测到音频设备，本软件无法运行。");
            break;
        }

        // 推迟到娱乐功能初始化的时候创建 [2016-9-8 Arthur]
        //bOK = m_JoyWMPlayer4.Create(NULL, WS_CHILD, CRect(0,0,0,0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 
        //    USER_WMP_MP3_ID, NULL, FALSE, CRIAApp::m_bstrWMPLicense);
        //if (!bOK)
        //{
        //    strErr = _T("创建视频播放接口失败，本软件无法运行。");
        //    break;
        //}
        //m_pJoyWMPCur = &m_JoyWMPlayer4;
        //m_pJoyWMPCur->put_enableContextMenu(FALSE);
        //m_pJoyWMPCur->put_enabled(TRUE);
        //m_pJoyWMPCur->put_uiMode(_T("none"));
        //m_pJoyWMPCur->put_windowlessVideo(TRUE);
        //m_JoyWMPCurSettings = m_pJoyWMPCur->get_settings();
        //m_JoyWMPCurSettings.put_autoStart(FALSE);
        //m_JoyWMPCurCtl = m_pJoyWMPCur->get_controls();

        bOK = TRUE;
    } while (FALSE);

    if (!bOK)
    {
        m_bInitOK = false;
        if (!strErr.IsEmpty())
        {
            MyAfxMessageBox(_T("%s"), strErr);
        }
        Close(IDCANCEL);
        //SendMessage(WM_CLOSE, (WPARAM)IDCANCEL);
        return;
    }
    m_bInitOK = true;

    //it's 96
    //lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    //HDC hDC = GetDC(GetHWND());
    //TRACE("GetDeviceCaps(hDC, LOGPIXELSY)=%d\n", GetDeviceCaps(hDC, LOGPIXELSY));
}

void MainFrame::KillAllTimer()
{
    UINT i;
    for (i = TIMER_ID_MIN_VALUE; i < TIMER_ID_MAX_VALUE; i++)
    {
        ::KillTimer(GetHWND(), i);
    }
}

void MainFrame::SetVoiceRateByValue(LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed/* = NULL*/)
{
    CComPtr<ISpObjectToken>        cpVoiceToken;
    CComPtr<IEnumSpObjectTokens>   cpEnum;
    HRESULT Hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
    //WCHAR wbuf[1024];

    CT2W wbuf(lpstrSpeaker);// myT2W(wbuf, 1024, lpstrSpeaker);
    if (SUCCEEDED(Hr))
    {
        while (cpEnum->Next(1, &cpVoiceToken, NULL) == S_OK)
        {
            CSpDynamicString dstrDesc;
            Hr = SpGetDescription(cpVoiceToken, &dstrDesc);
            if (lstrcmpW(wbuf, dstrDesc.m_psz) == 0)
            {
                Hr = m_cpVoice->SetVoice(cpVoiceToken);
                if (FAILED(Hr))
                {
                    TRACE("SetVoice %S failed:0x%08x\n", dstrDesc, Hr);
                }
                break;
            }
            cpVoiceToken.Release();
        }
    }

    if (lpstrSpeed)
    {
        Hr = m_cpVoice->SetRate(_tstoi(lpstrSpeed));
        if (FAILED(Hr))
        {
            TRACE(_T("SetRate %s failed:0x%08x\n"), lpstrSpeed, Hr);
        }
    }

    return;
}

void MainFrame::SetVoiceRateBySetting(LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed/* = NULL*/)
{
    CString strVoice = ReadSetup(lpstrSpeaker);
    CString strSpeed;
    if (lpstrSpeed)
    {
        strSpeed = ReadSetup(lpstrSpeed);
    }
    SetVoiceRateByValue(strVoice, lpstrSpeed ? strSpeed : NULL);

    return;
}

LPCTSTR TraceSkin(emSkinId nSkinId)
{
#define MSGDEF(x) if(nSkinId==x) return _T(#x)
    MSGDEF(sMainSkin);
    MSGDEF(sWordSelectSkin);
    MSGDEF(sWordSkin);
    MSGDEF(sWordSetupSkin);
    MSGDEF(sWordExamSkin);
    MSGDEF(sWordExamSetupSkin);  
    MSGDEF(sWordExamResultSkin);
    MSGDEF(sWordAddNewSkin);
    MSGDEF(sWordManageNewSkin);   
    MSGDEF(sWordToVoiceSkin);
    MSGDEF(sReadSkin);
    MSGDEF(sReadCfgSkin);
    MSGDEF(sReadStatSkin);   
    MSGDEF(SReadTransSkin);
    MSGDEF(sReadCnTextSkin);
    MSGDEF(sDialogSkin);
    MSGDEF(sDialogSelectSkin);
    MSGDEF(sDialogEditSkin);
    MSGDEF(sDialogSetupSkin);
    MSGDEF(sDialogListSkin);
    MSGDEF(sListenSkin);  
    MSGDEF(sListenSelectSkin);
    MSGDEF(sListenSetupSkin);
    MSGDEF(sListenResultSkin);
    MSGDEF(sGrammarSkin);
    MSGDEF(sGrammarResultSkin);
    MSGDEF(sSymbolSkin);
    MSGDEF(sJoySkin);
    MSGDEF(sRecordSkin);   
    MSGDEF(sReadingbarSkin);
    MSGDEF(sReadingbarCfgSkin);
    MSGDEF(sReportSkin);
    MSGDEF(sReportDetailSkin);
    MSGDEF(sLoadingSkin);
    MSGDEF(sExportWaveSkin);
    MSGDEF(sDictSkin);
    MSGDEF(sPaperSelectSkin);
    static TCHAR szMsg[10];
    StringCchPrintf(szMsg, _countof(szMsg), _T("%d"), nSkinId);
    return szMsg;
}

//各窗口的初始化在此完成
void MainFrame::OnPrepare(TNotifyUI& msg)
{
    //TRACE("%s, id:%S\n", __FUNCTION__, TraceSkin(m_nSkinId));
//    int i;
    CString strTmp;
    //MyCStringArray strColl;
    //MyDbCStringArray strResult;
    //MyDbCStringArray::const_iterator pColl;

//    CComboUI* pCombo;
//    CListLabelElementUI* pElement;
//    CSliderUI* pSlider;
//    CTextUI* pText;
    COptionUI* pOpt;
    CEditUI* pEdit;
    CCheckBoxUI* pCheck;
//    CListUI* pList;
    //CButtonUI* pBtn;
    //CLabelUI* pLbl;

    if (!m_bInitOK)
    {
        return;
    }
    DuiLib::CWaitCursor cwc;
    ShowLoading();

    StopSpeak();
    StopWaveRecordReco();

    switch (m_nSkinId)
    {
    case sMainSkin:
        {
            OnPrepareMain();
        }
        break;
    case sRegisterSkin:
        OnPrepareRegister();
        break;
    case sDialogSkin:
        {
            OnDialogPrepare();
        }
        break;
    case sDialogSelectSkin:
        if (!CheckMaterial())
        {
            break;
        }
        ASSERT(m_nDialogSelectPageId >= DIALOG_FOLLOWME && m_nDialogSelectPageId <= DIALOG_WRITTING);
        pOpt = xFindControl(COptionUI, g_dialog_select_opt[m_nDialogSelectPageId-1]);
        pOpt->Selected(true);
        break;
    case sDialogEditSkin:
        ASSERT(FALSE);
        break;
    case sGrammarSkin:
        {
            if (!CheckMaterial())
            {
                break;
            }
            InitGrammarClassCmb();
            //如果控件不可见或disabled，则SelectItem返回失败。。。
            //先可见
            CTabLayoutUI* pTab = xFindControl(CTabLayoutUI, _T("tabgrammer"));
            pTab->SelectItem(1);
            InitGrammarExamSettingCtrls();
            pTab->SelectItem(2);
            InitGrammarExamCtrls();
            pTab->SelectItem(0);
            if (m_bGramBackFromResult)
            {
                pOpt = xFindControl(COptionUI, _T("gramexamopt"));
                pOpt->Selected(true);
            }
            else
            {
                pOpt = xFindControl(COptionUI, _T("gramlernopt"));
                pOpt->Selected(true);
            }
            m_bGramBackFromResult = false;

            m_tmGrammarLogFrom = CTime::GetCurrentTime();
        }
        break;
    case sGrammarResultSkin:
        {
            InitGrammarResultCtrls();
        }
        break;
    case sJoySkin:
        {
            if (!CheckMaterial())
            {
                break;
            }
            pOpt = xFindControl(COptionUI, _T("joymusicopt"));
            pOpt->Selected(true);
            //选择后，会调用此函数
            //InitJoyCtrls(0);
        }
        break;
    case sListenSkin:
        {
            m_tmListenLogFrom = CTime::GetCurrentTime();
            OnListenPrepare();
        }
        break;
    case sListenSelectSkin:
        {
            if (!CheckMaterial())
            {
                break;
            }
            InitVoiceRateControl(&m_PaintManager, _T("Listen_Speaker"), _T("Listen_Speed"));
            InitListenCfgControl();
        }
        break;
    case sReadSkin:
        {
            if (!CheckMaterial())
            {
                break;
            }
#ifdef APP_DEMO
            pOpt = xFindControl(COptionUI, _T("storyresiteopt"));
            pOpt->SetVisible(false);
#endif
            m_tmReadLogFrom = CTime::GetCurrentTime();
            m_nReadValue2 = 0;
            OnReadWndPrepare();            
        }
        break;
    case sReadCfgSkin:
        break;
    case sReadingbarSkin:
        {
            OnReadBarPrepare();
        }
        break;
    case sRecordSkin:
        break;
    case sReportSkin:
        {
            InitReportCtrls();
        }
        break;
    case sReportDetailSkin:
        {
            ASSERT( (m_nReportDetailItem >= 1) &&
                (m_nReportDetailItem <= 5) );
            InitReportDetailCtrls();
        }
        break;
    case sSymbolSkin:
        {
            InitSymbolCtrls();
        }
        break;
    case sWordSkin:
        {
            m_tmWordLogFrom = CTime::GetCurrentTime();
            OnWordPrepare();
        }
        break;
    case sWordSelectSkin:
        {
            if (!CheckMaterial())
            {
                break;
            }
            InitWordCfgControl();
        }
        break;
    case sWordExamSkin:
        {
            CString strGrade = ReadSetup(_T("word_grade"));
            CString strBook = ReadSetup(_T("word_book"));
            CString strUnit = ReadSetup(_T("word_unit"));
            InitWordExamGradeCombo(strGrade);
            InitWordExamBookCombo(strBook);
            InitWordExamUnitCombo(strUnit);

            pOpt = xFindControl(COptionUI, _T("wordexampracticeopt"));
            pOpt->Selected(true);

            m_tmWordLogFrom = CTime::GetCurrentTime();
            InitWordExamStatus();
        }
        break;
    case sWordExamResultSkin:
        {
            InitWordExamResultCtrls();
        }
        break;
    case sDictSkin:
        {
            //单独窗口
            ASSERT(FALSE);
        }
        break;
    case sWordSetupSkin:
        {
            InitWordSetupControls();
        }
        break;
    case sWordExamSetupSkin:
        {
            int num;
            strTmp = ReadSetup(_T("WordExam_QuestionNumber"));
            if (!IsNumeric(strTmp))
            {
                num = 20;
            }
            else
            {
                num = _tstoi(strTmp);
            }
            if ((num <= 0) || (num > 99))
            {
                num = 20;
            }
            strTmp.Format(_T("%d"), num);
            pEdit = xFindControl(CEditUI, _T("wordexmnumberedt"));
            pEdit->SetText(strTmp);

            strTmp = ReadSetup(_T("WordExam_AotuAddWrongWord"));
            pCheck = xFindControl(CCheckBoxUI, _T("wordexmautoaddchk"));
            pCheck->SetCheck(strTmp == _T("1"));

            InitVoiceRateControl(&m_PaintManager, _T("WordExam_Speaker"), _T("WordExam_Speed"));
        }
        break;
    case sPaperSelectSkin:
        {
            CString strGrade = ReadSetup(_T("paper_grade"));
            CString strBook = ReadSetup(_T("paper_book"));
            CString strUnit = ReadSetup(_T("paper_unit"));

            InitPaperCfgGradeCombo(strGrade);
            InitPaperCfgBookCombo(strBook);
            InitPaperCfgUnitCombo(strUnit);

            pOpt = xFindControl(COptionUI, _T("papercfgeasyopt"));
            pOpt->Selected(true);
        }
        break;
    default:
        break;
    }

    HideLoading();
}

//////////////////////////////////////////////////////////////////////////
//迷你朗读条函数

void MainFrame::OnReadBarPrepare()
{
    SetVoiceRateBySetting(_T("ReadingBar_Speaker"), _T("ReadingBar_Speed"));

    ::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW |SWP_NOSIZE);
    //CenterWindow();

    //是否需要清除clipboard的内容？
}

bool MainFrame::ReadBarGetText()
{
    MyGetClipboardText(GetHWND(), m_strReadbarContent);
    return m_strReadbarContent.empty() ? false : true;
}

void MainFrame::ReadBarSaveTextToSpeech(LPCTSTR strWaveFile)
{
    SpeakBindToFile(strWaveFile);
    SpeakAndWait(m_strReadbarContent.c_str());
    SpeakRestore();
}

HRESULT MainFrame::SpeakBindToFile(LPCTSTR strWavFile)
{
    HRESULT hr;
    CSpStreamFormat OriginalFmt;
    //WCHAR szWFileName[1024];

    do 
    {
        CT2W szWFileName(strWavFile);//myT2W(szWFileName, _countof(szWFileName), strWavFile);

        hr = m_cpVoice->GetOutputStream( &m_cpOldStream );
        if (hr == S_OK)
        {
            hr = OriginalFmt.AssignFormat(m_cpOldStream);
        }
        else
        {
            hr = E_FAIL;
            break;
        }
        // User SAPI helper function in sphelper.h to create a wav file
        if (SUCCEEDED(hr))
        {
            hr = SPBindToFile( szWFileName, SPFM_CREATE_ALWAYS,
                &m_cpWavStream, &OriginalFmt.FormatId(), OriginalFmt.WaveFormatExPtr() ); 
        }
        if( SUCCEEDED( hr ) )
        {
            // Set the voice's output to the wav file instead of the speakers
            hr = m_cpVoice->SetOutput(m_cpWavStream, TRUE);
        }
    } while (FALSE);

    return hr;
}

HRESULT MainFrame::SpeakAndWait(LPCWSTR lpwText, DWORD dwFlags/*=SPF_ASYNC*/)
{
    DuiLib::CWaitCursor cwait;
    Speak(lpwText, dwFlags);
    return m_cpVoice->WaitUntilDone( INFINITE );
}

void MainFrame::SpeakRestore()
{
    m_cpWavStream.Release();

    // Reset output
    m_cpVoice->SetOutput( m_cpOldStream, FALSE );
    m_cpOldStream.Release();
}

static
int speak_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
    TRACE(_T("exception code:0x%08x\n"), code);
    if (code == EXCEPTION_ACCESS_VIOLATION)
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
}

HRESULT MainFrame::Speak(LPCWSTR lpwText, DWORD dwFlags/* =SPF_ASYNC */)
{
    HRESULT Hr = S_OK;
    WCHAR wTxt[1024];

    StringCchCopyW(wTxt, _countof(wTxt), lpwText);
    SpeakCheckText(wTxt, _countof(wTxt));
    //TRACE("speak orig:%S, speak:%S\n", lpwText, wTxt);
    TRACE("speak:%S\n", wTxt);

    __try
    {
        Hr = m_cpVoice->Speak(wTxt, dwFlags, NULL);
    }
    __except(speak_filter(GetExceptionCode(), GetExceptionInformation()))
    {
        Hr = S_FALSE;
    }

    return Hr;
}

void MainFrame::hlpWave2Mp3(LPCTSTR strWave, LPCTSTR strMp3)
{
    CString strlame = CPaintManagerUI::GetInstancePath() + _T("lame.exe");
#if 1
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    ::GetStartupInfo(&si);

    CString strParam;
    strParam.Format(_T("\"%s\" -b128 --resample 22.05 -m j \"%s\" \"%s\""),
        strlame, strWave, strMp3);

    BOOL bOk = CreateProcess(NULL,
        strParam.GetBuffer(),
        NULL, NULL, FALSE,
        CREATE_NO_WINDOW,
        NULL, NULL, &si, &pi);
    strParam.ReleaseBuffer();
    if (bOk)
    {
        ::WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
    }
#ifdef _DEBUG
    else
    {
        TRACE(_T("create %s failed:0x%x\n"), strParam, GetLastError());
    }
#endif
    ::DeleteFile(strWave);
#else
    CString strParam;
    strParam.Format(_T("-b128 --resample 22.05 -m j \"%s\" \"%s\""),
        strWave, strMp3);
    HINSTANCE hRet = ::ShellExecute(NULL, _T("open"), strlame, strParam, NULL, SW_HIDE);
    TRACE(_T("ShellExecute return:%d\n"), hRet);
#endif
}

LPCTSTR MainFrame::GetCurrentResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return MAKEINTRESOURCE(m_nResourceId);
}

LPCTSTR MainFrame::SetCurrentResourceID(WORD nId)
{
    LPCTSTR x = GetCurrentResourceID();
    m_nResourceId = nId;

    static LPCTSTR s_SkinFoldName[] =
#if SKIN==SKN_WIN8
    {
        _T("缤纷年华"),
        _T("碧海蓝天"),
        _T("绿色希望")
    };
#else
    {
        _T("黑夜"),
        _T("湛蓝"),
        _T("湖蓝"),
        _T("澄青")
    };
#endif
    // 同时也设置ZIP file，SetResourcePath [2016-4-26 Arthur]
    ASSERT(nId - IDR_ZIPRES < _countof(s_SkinFoldName));
    switch (GetResourceType())
    {
    case UILIB_FILE:
        CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + GetSkinFolder() + _T("\\") + s_SkinFoldName[nId - IDR_ZIPRES]);
		
    	break;
    case UILIB_ZIP:
        m_strZipFileName = s_SkinFoldName[nId - IDR_ZIPRES];
        m_strZipFileName += _T(".zip");
        break;
    default:
        break;
    }
    
    return x;
}

CDuiString MainFrame::GetZIPFileName() const
{
    return m_strZipFileName;
}

int MainFrame::IsReportDetail(const TNotifyUI& msg)
{
    return IsOption(g_ReportDetailBtns, _countof(g_ReportDetailBtns), msg);
}

static LPCTSTR g_SymbolNames[14][6]=
{
    {_T("symqianyuanyin1btn"),          _T("symqianyuanyin2btn"),           _T("symqianyuanyin3btn"),           _T("symqianyuanyin4btn"),           _T("symqianyuanyin5btn"),           _T("symqianyuanyin6btn")},
    {_T("symzhongyuanyin1btn"),         _T("symzhongyuanyin2btn"),          _T("symzhongyuanyin3btn"),          _T("symzhongyuanyin4btn"),          _T("symzhongyuanyin5btn"),          _T("symzhongyuanyin6btn")},
    {_T("symhouyuanyin1btn"),           _T("symhouyuanyin2btn"),            _T("symhouyuanyin3btn"),            _T("symhouyuanyin4btn"),            _T("symhouyuanyin5btn"),            _T("symhouyuanyin6btn")},
    {_T("symkaiheshuangyuanyin1btn"),   _T("symkaiheshuangyuanyin2btn"),    _T("symkaiheshuangyuanyin3btn"),    _T("symkaiheshuangyuanyin4btn"),    _T("symkaiheshuangyuanyin5btn"),    _T("symkaiheshuangyuanyin6btn")},
    {_T("symjizhongshuangyuanyin1btn"), _T("symjizhongshuangyuanyin2btn"),  _T("symjizhongshuangyuanyin3btn"),  _T("symjizhongshuangyuanyin4btn"),  _T("symjizhongshuangyuanyin5btn"),  _T("symjizhongshuangyuanyin6btn")},
    {_T("symbaopoyin1btn"),             _T("symbaopoyin2btn"),              _T("symbaopoyin3btn"),              _T("symbaopoyin4btn"),              _T("symbaopoyin5btn"),              _T("symbaopoyin6btn")},
    {_T("symqingfuyin1btn"),            _T("symqingfuyin2btn"),             _T("symqingfuyin3btn"),             _T("symqingfuyin4btn"),             _T("symqingfuyin5btn"),             _T("symqingfuyin6btn")},
    {_T("symmocayin1btn"),              _T("symmocayin2btn"),               _T("symmocayin3btn"),               _T("symmocayin4btn"),               _T("symmocayin5btn"),               _T("symmocayin6btn")},
    {_T("symmoca2yin1btn"),             _T("symmoca2yin2btn"),              _T("symmoca2yin3btn"),              _T("symmoca2yin4btn"),              _T("symmoca2yin5btn"),              _T("symmoca2yin6btn")},
    {_T("sympocayin1btn"),              _T("sympocayin2btn"),               _T("sympocayin3btn"),               _T("sympocayin4btn"),               _T("sympocayin5btn"),               _T("sympocayin6btn")},
    {_T("sympoca2yin1btn"),             _T("sympoca2yin2btn"),              _T("sympoca2yin3btn"),              _T("sympoca2yin4btn"),              _T("sympoca2yin5btn"),              _T("sympoca2yin6btn")},
    {_T("symbiyin1btn"),                _T("symbiyin2btn"),                 _T("symbiyin3btn"),                 _T("symbiyin4btn"),                 _T("symbiyin5btn"),                 _T("symbiyin6btn")},
    {_T("symsheceyin1btn"),             _T("symsheceyin2btn"),              _T("symsheceyin3btn"),              _T("symsheceyin4btn"),              _T("symsheceyin5btn"),              _T("symsheceyin6btn")},
    {_T("symbanyuanyin1btn"),           _T("symbanyuanyin2btn"),            _T("symbanyuanyin3btn"),            _T("symbanyuanyin4btn"),            _T("symbanyuanyin5btn"),            _T("symbanyuanyin6btn")},
};

static LPCTSTR g_SymWord[5][4] =
{
    {_T("symword00btn"), _T("symword01btn"), _T("symword02btn"), _T("symword03btn")},
    {_T("symword10btn"), _T("symword11btn"), _T("symword12btn"), _T("symword13btn")},
    {_T("symword20btn"), _T("symword21btn"), _T("symword22btn"), _T("symword23btn")},
    {_T("symword30btn"), _T("symword31btn"), _T("symword32btn"), _T("symword33btn")},
    {_T("symword40btn"), _T("symword41btn"), _T("symword42btn"), _T("symword43btn")},
};

bool MainFrame::IsSymbolName(const TNotifyUI& msg)
{
    int i, j;
    for (i = 0; i < _countof(g_SymbolNames); i++)
    {
        for (j = 0; j < _countof(g_SymbolNames[i]); j++)
        {
            if (msg.pSender->GetName() == g_SymbolNames[i][j])
            {
                return true;
            }
        }
    }
    return false;
}

bool MainFrame::IsSymbolWord(const TNotifyUI& msg)
{
    int i, j;
    for (i = 0; i < _countof(g_SymWord); i++)
    {
        for (j = 0; j < _countof(g_SymWord[i]); j++)
        {
            if (msg.pSender->GetName() == g_SymWord[i][j])
            {
                return true;
            }
        }
    }
    return false;
}

void MainFrame::SymbolHideAllWord()
{
    int i, j;
    CButtonUI* pBtn;
    for (i = 0; i < _countof(g_SymWord); i++)
    {
        for (j = 0; j < _countof(g_SymWord[i]); j++)
        {
            pBtn = xFindControl(CButtonUI, g_SymWord[i][j]);
            pBtn->SetVisible(false);
        }
    }
}

bool MainFrame::OnSymbolWordNotify(void* event)
{
    CButtonUI* pNode;
    TNotifyUI* pMsg = static_cast<TNotifyUI*>(event);
    if (pMsg && (pMsg->sType == DUI_MSGTYPE_CLICK))
    {
        pNode = static_cast<CButtonUI*>(pMsg->pSender);
        MainFrame* pFrame = (MainFrame*)pNode->GetTag();
        pFrame->Speak(pNode->GetUserData().GetData());
    }

    return true;
}

/*控件须为float*/
static
void SetBkImageAutoSize(CControlUI* pCtrl, LPCTSTR pStrImage)
{
    CString strTmp;
    //WCHAR wBuf[1024];

    CT2W wBuf(pStrImage);//myT2W(wBuf, _countof(wBuf), pStrImage);
    //得到图片的尺寸
    Image tImg((LPCWSTR)wBuf);
    //设置控件大小
    //strTmp.Format(_T("width=\"%d\" height=\"%d\""), tImg.GetWidth(), tImg.GetHeight());
    //pCtrl->ApplyAttributeList(strTmp);
    RECT rct;
    rct = pCtrl->GetPos();
    rct.right = rct.left + tImg.GetWidth();
    rct.bottom = rct.top + tImg.GetHeight();
    pCtrl->SetPos(rct);
    pCtrl->SetBkImage(pStrImage);
}

static
void SetBtnImageAutoSize(CControlUI* pCtrl, LPCTSTR pStrImage)
{
    CString strTmp;
    //WCHAR wBuf[1024];

    CT2W wBuf(pStrImage); //myT2W(wBuf, _countof(wBuf), pStrImage);
    //得到图片的尺寸
    Image tImg((LPCWSTR)wBuf);
    //设置控件大小
    RECT rct;
    rct = pCtrl->GetPos();
    rct.right = rct.left + tImg.GetWidth();
    rct.bottom = rct.top + tImg.GetWidth();
    pCtrl->SetPos(rct);
    //strTmp.Format(_T("normalimage=\"%s\" hotimage=\"%s\" width=\"%d\" height=\"%d\""),
    //    pStrImage, pStrImage, tImg.GetWidth(), tImg.GetHeight());
    strTmp.Format(_T("normalimage=\"%s\" hotimage=\"%s\" disabledimage=\"%s\""),
        pStrImage, pStrImage, pStrImage);
    pCtrl->ApplyAttributeList(strTmp);
}

void MainFrame::InitSymbolSymbol(LPCTSTR strId)
{
    if ( (NULL == strId) ||
        (lstrlen(strId) == 0) )
    {
        return;
    }

    CButtonUI* pBtn = xFindControl(CButtonUI, _T("symspeakwordbtn"));
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("symbolmemoedt"));
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("symbolsymtext"));
    CControlUI* pPic = xFindControl(CControlUI, _T("symbolpic"));
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 音标列表 where Id=%s"),
        strId);
    CString strTmp;
    int i, j;

    pBtn->SetVisible(false);
    pREdt->SetVisible(false);
    pLbl->SetVisible(false);
    pPic->SetVisible(false);
    if (pRecdPtr)
    {
        pBtn->SetUserData(GetCollect(pRecdPtr, _T("声音")));
        pBtn->SetVisible(true);
        pREdt->SetText(GetCollect(pRecdPtr, _T("发音要诀")));
        pREdt->SetVisible(true);
        strTmp.Format(_T("[%s]"), GetCollect(pRecdPtr, _T("音标")));
        pLbl->SetText(strTmp);
        pLbl->SetVisible(true);
        CString strPic = GetCollect(pRecdPtr, _T("口型图"));
        if (!strPic.IsEmpty())
        {
            CString strPicPath;
            strPicPath.Format(_T("%sData\\Picture\\%s"),
                CPaintManagerUI::GetInstancePath().GetData(),
                (LPCTSTR)strPic);
            CFileFind finder;
            if (finder.FindFile(strPicPath))
            {
                //pPic->SetBkImage(strPicPath);
                SetBkImageAutoSize(pPic, strPicPath);
                pPic->SetVisible(true);
            }
            finder.Close();
        }
    }

    pRecdPtr = GetTablesBySql(_T("Select * from 音标单词 where 音标=%s"),
        strId);
    if (pRecdPtr == NULL)
    {
        return;
    }
#if 1
    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("symwordlayout"));
    CHorizontalLayoutUI* pHLay = NULL;
    pVLay->RemoveAll();
    i = 0, j = 0;
    CString strPath;
    while(!pRecdPtr->GetadoEOF())
    {
        strPath.Format(_T("%sData\\Picture\\%s"),
            CPaintManagerUI::GetInstancePath().GetData(),
            (LPCTSTR)GetCollect(pRecdPtr, _T("图片")));
        CFileFind finder;
        if (!finder.FindFile(strPath))
        {
            TRACE(_T("%s not found!\n"), strPath);
            finder.Close();
            continue;
        }
        finder.Close();
        if (i % 4 == 0)
        {
            pHLay = new CHorizontalLayoutUI;
            j = 0;
            if (i > 0)
            {
                strTmp.Format(_T("padding=\"0,10,0,0\""));
                pHLay->ApplyAttributeList(strTmp);
            }
            pVLay->Add(pHLay);
        }

        pBtn = new CButtonUI;
        Image img(strPath);
        if (img.GetHeight() > (UINT)j)
        {
            j = img.GetHeight();
            strTmp.Format(_T("height=\"%d\""), j);
            pHLay->ApplyAttributeList(strTmp);
        }
        strTmp.Format(_T("normalimage=\"%s\" hotimage=\"%s\" disabledimage=\"%s\" width=\"%d\" height=\"%d\""),
            strPath, strPath, strPath, img.GetWidth(), img.GetHeight());
        if (i % 4 != 0)
        {
            strTmp += _T(" padding=\"10,0,0,0\"");
        }
        pBtn->ApplyAttributeList(strTmp);
        pBtn->SetUserData(GetCollect(pRecdPtr, _T("单词")));
        pBtn->SetTag((UINT_PTR)this);
        pBtn->OnNotify += OnSymbolWordNotify;
        pHLay->Add(pBtn);

        pRecdPtr->MoveNext();
        i++;
    }
#else
    SymbolHideAllWord();
    i = 0, j = 0;
    while(!pRecdPtr->GetadoEOF())
    {
        ASSERT(j < _countof(g_SymWord[0]));
        ASSERT(i < _countof(g_SymWord));
        pBtn = xFindControl(CButtonUI, g_SymWord[i][j]);
        strTmp.Format(_T("%sData\\Picture\\%s"),
            CPaintManagerUI::GetInstancePath().GetData(),
            (LPCTSTR)GetCollect(pRecdPtr, _T("图片")));
        CFileFind finder;
        //pBtn->SetBkImage(strTmp);
        CString strT;
        strT.Format(_T("normalimage=\"%s\" hotimage=\"%s\" disabledimage=\"%s\""),
            strTmp, strTmp, strTmp);
        pBtn->ApplyAttributeList(strT);
        RECT rct = pBtn->GetPos();
        Image img(strTmp);
        rct.right = rct.left + img.GetWidth();
        rct.bottom = rct.top + img.GetHeight();
        pBtn->SetPos(rct);
        //SetBtnImageAutoSize(pBtn, strTmp);
        pBtn->SetVisible(true);
        pBtn->SetUserData(GetCollect(pRecdPtr, _T("单词")));

        j++;
        if ( (j % _countof(g_SymWord[0])) == 0 )
        {
            j = 0;
            i++;
        }
        pRecdPtr->MoveNext();
    }
#endif
}

void MainFrame::InitSymbolCtrls()
{
    int i, j, count = 14;
    _RecordsetPtr pRecdPtr;
    CButtonUI* pBtn;
    CString strTmp;

    for (i = 1; i <= 14; i++)
    {
        pRecdPtr = GetTablesBySql(_T("Select Id, 音标 from 音标列表 where 小类=%d"), i);
        if (pRecdPtr == NULL)
        {
            continue;
        }
        j = 0;
        while(!pRecdPtr->GetadoEOF())
        {
            ASSERT(j < _countof(g_SymbolNames[0]));
            pBtn = xFindControl(CButtonUI, g_SymbolNames[i-1][j]);
            strTmp.Format(_T("[%s]"), GetCollect(pRecdPtr, _T("音标")));
            pBtn->SetVisible(true);
            pBtn->SetText(strTmp);
            pBtn->SetUserData(GetCollect(pRecdPtr, _T("Id")));

            j++;
            pRecdPtr->MoveNext();
        }
    }

    InitSymbolSymbol(GetSingleField(_T("Select top 1 Id from 音标列表 Order by Id")));
}

void MainFrame::Notify(TNotifyUI& msg)
{
    //TRACE(_T("type:%s, sender:%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
    int i;
    CString sType = msg.sType;
    CString strSender = msg.pSender->GetName();
    CComboUI* pCombo;
    COptionUI* pOpt;
    CButtonUI* pButton;
    CSliderUI* pSlider;
    CProgressUI* pPrgrs;
    CTabLayoutUI* pTab;
    CTextUI* pTxt;
    CString strTmp;
    CEditUI* pEdit;
    CCheckBoxUI* pCheckBox;
    //CListLabelElementUI* pItem;
    CRichEditUI* pREdt;
    //CTreeViewUI* pTreeView;
    //CTreeNodeUI* pTreeNode;
    //CListUI* pList;
    CLabelUI* pLbl;

	if (sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare(msg);
        BringWindowToTop(GetHWND());
	}
	else if (sType == DUI_MSGTYPE_CLICK)
	{
        //公共按钮处理
        if ((strSender == kCloseButtonName) ||
            (strSender == kReturnButtonName) ||
            (strSender == kSubReturnButtonName) ||
            (strSender == _T("dlgreturnbtn")) )
		{
            if (m_nSkinId == sMainSkin ||
                (m_nSkinId == sRegisterSkin && !m_bRegistered))
            {
                //OnExit(msg);
                //m_JoyWMPlayer4.DestroyWindow();
                Close(IDCANCEL);
            }
            else
            {
                //KillAllTimer();
                StopSpeak();
                m_JoyWMPCurCtl.stop();
                m_strCloseFlag = CF_CLOSE;
                //m_WaveRecorder.StopRecord();
                StopWaveRecordReco();
                //SpeechRecognition_Stop();

                if (m_pReadTextDoc)
                {
                    m_pReadTextDoc.Release();
                    m_pReadTextDoc = NULL;
                }

                //记录成绩
                int nValue1, nValue2;
                if (sGrammarSkin == m_nSkinId)
                {
                    nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmGrammarLogFrom).GetTotalMinutes();
                    if (nValue1 > 0)
                    {
                        StudyLog(STUDY_TYPE_GRAMMAR, LOG_TYPE_TIME, nValue1, 0);
                    }
                }
                else if (sListenSkin == m_nSkinId)
                {
                    nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmListenLogFrom).GetTotalMinutes();
                    if (nValue1 > 0)
                    {
                        StudyLog(STUDY_TYPE_LISTEN, LOG_TYPE_TIME, nValue1, 0);
                    }
                }
                else if (sDialogSkin == m_nSkinId)
                {
                    nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmDialogLogFrom).GetTotalMinutes();
                    StudyLog(STUDY_TYPE_DIALOG, LOG_TYPE_TIME, nValue1, 0);
                    if (m_nDialogSelectPageId != DIALOG_WRITTING)
                    {
                        if (m_nDialogValue1 > 0)
                        {
                            nValue2 = (m_nDialogValue2 + 50) / 100;
                            StudyLog(STUDY_TYPE_DIALOG, LOG_TYPE_ACHIEV, m_nDialogValue1, nValue2);
                        }
                    }
                }
                else if (sWordSkin == m_nSkinId)
                {
                    nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmWordLogFrom).GetTotalMinutes();
                    if (nValue1 > 0)
                    {
                        TRACE(_T("word time:%d\n"), nValue1);
                        StudyLog(STUDY_TYPE_WORD, LOG_TYPE_TIME, nValue1, 0);
                    }
                }
                //???
                else if (sReadSkin == m_nSkinId)
                {
                    nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmReadLogFrom).GetTotalMinutes();
                    if (nValue1 > 0)
                    {
                        StudyLog(STUDY_TYPE_READ, LOG_TYPE_TIME, nValue1, 0);
                    }
                }
                
                if (m_nSkinId == sWordSetupSkin)
                {
                    OpenChildModalWnd(sWordSkin);
                }
                else if (m_nSkinId == sWordExamSetupSkin)
                {
                    OpenChildModalWnd(sWordExamSkin);
                }
                else if (m_nSkinId == sReportDetailSkin)
                {
                    OpenChildModalWnd(sReportSkin);
                }
                else
                    OpenChildModalWnd(sMainSkin);
            }
		}
		else if (strSender == kMinButtonName)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
        //////////////////////////////////////////////////////////////////////////
        //主界面处理
        else if (strSender == _T("mainsettingbtn"))
        {
            CAppSetting* appset = new CAppSetting(this);
            OpenModalWnd(appset, _T("全局设置"));
        }
		else if (strSender == kHelpButtonName)
		{
            CDuiString strHelp = CPaintManagerUI::GetInstancePath() + _T("System\\Help\\OnlineHelp.htm");
            ::ShellExecute(NULL, _T("open"), strHelp, NULL, NULL, SW_SHOWMAXIMIZED);
		}
        else if (strSender == kContactButtonName)
        {
            ::ShellExecute(NULL, _T("open"), _T("http://www.nyedu.com.cn"), NULL, NULL, SW_SHOWMAXIMIZED);
        }
        else if (strSender == kReadButtonName)
        {
            OpenChildModalWnd(sReadSkin);
        }
        else if (strSender == kWordButtonName)
        {
            //OpenChildModalWnd(sWordSelectSkin);
            
            //ShellExecute(GetHWND(), _T("open"), _T("WordExer.exe"),NULL,NULL,SW_SHOWNORMAL);
            PROCESS_INFORMATION pi;  
            STARTUPINFO si;  
            memset(&si,0,sizeof(si));  
            si.cb = sizeof(si);  
            si.wShowWindow = SW_SHOW;  
            si.dwFlags = STARTF_USESHOWWINDOW;
            bool fRet = CreateProcess(_T("WordExer.dll"),NULL,NULL,FALSE ,NULL,NULL,NULL,NULL,&si,&pi); 
        }
        else if (strSender == kDialogButtonName)
        {
            OpenChildModalWnd(sDialogSelectSkin);
        }
        else if (strSender == kListenButtonName)
        { 
            OpenChildModalWnd(sListenSelectSkin);
        }
        else if (strSender == kGrammarButtonName)
        {
            OpenChildModalWnd(sGrammarSkin);
        }
        else if (strSender == kSymbolButtonName)
        {
            OpenChildModalWnd(sSymbolSkin);
        }
        else if (strSender == kJoyButtonName)
        {
//#ifdef APP_DEMO
//            //禁止功能
//            MyAfxMessageBox(_T("%s"), DEMO_TIP);
//            return;
//            // end
//#endif

            OpenChildModalWnd(sJoySkin);
        }
        else if (strSender == kRecordButtonName)
        {
            OpenChildModalWnd(sRecordSkin);
        }
        else if (strSender == kReadingbarButtonName)
        {
            OpenChildModalWnd(sReadingbarSkin);
        }
        else if (strSender == kReportButtonName)
        {
#ifdef APP_DEMO
            //禁止功能
            MyAfxMessageBox(_T("%s"), DEMO_TIP);
            return;
            // end
#endif
            OpenChildModalWnd(sReportSkin);
        }
        else if (kPaperButtonName == strSender)
        {
            OpenChildModalWnd(sPaperSelectSkin);
        }
        else if (strSender == kDictButtonName)
        {
            CDictionary *pdlgdic = new CDictionary(this);
            StopWaveRecordReco();
            WordStopAutoPlay();
            OpenModalWnd(pdlgdic, g_SkinInfo[sDictSkin].title);
        }
        else if (strSender == _T("mainskinmenubtn"))
        {
            CMenuWnd* pMenu = new CMenuWnd();
            DuiLib::CPoint point = msg.ptMouse;
            ClientToScreen(GetHWND(), &point);
            pMenu->Init(NULL, _T("changeskin.xml"), point, &m_PaintManager, &m_SkinMenuCheckInfo);
        }
        //////////////////////////////////////////////////////////////////////////
        //注册界面
        else if (_T("regstep4pre") == strSender ||
            _T("regstep3pre") == strSender)
        {
            RegisterPreClicked();
        }
        else if (_T("regstep1next") == strSender ||
            _T("regstep2next") == strSender ||
            _T("regstep3next") == strSender)
        {
            RegisterNextClicked();
        }
        else if (_T("regstep4okbtn") == strSender)
        {
            RegisterOkClick();
        }
        //////////////////////////////////////////////////////////////////////////
        //朗读界面、听写界面等设置按钮处理处理
        else if (strSender == kSetButtonName)
        {
            WindowImplBase* pWnd = NULL;
            LPCTSTR lpTitle = NULL;

            StopSpeak();
            StopWaveRecordReco();
            switch(m_nSkinId)
            {
            case sReadSkin:
                pWnd = new CReaderSetup(this);
                lpTitle = g_SkinInfo[sReadCfgSkin].title;
                break;
            case sListenSkin:
                pWnd = new CListenVoiceSetup(this);
                lpTitle = g_SkinInfo[sListenSetupSkin].title;
                break;
            case sWordSkin:
                WordStopAutoPlay();

                OpenChildModalWnd(sWordSetupSkin);
                break;
            case sWordExamSkin:
                OpenChildModalWnd(sWordExamSetupSkin);
                break;
            case sDialogSkin:
                {
                    if (m_nDialogSelectPageId == DIALOG_WRITTING)
                    {
                        pWnd = new CDialogWrittingSetup(this);
                    }
                    else
                    {
                        pWnd = new CDialogSetup(this);
                        CDialogSetup* pD = static_cast<CDialogSetup*>(pWnd);
                        pD->m_nType = m_nDialogSelectPageId;
                    }
                    lpTitle = g_SkinInfo[sDialogSetupSkin].title;
                }
                break;
            default:
                break;
            }
            if (pWnd && (IDOK == OpenModalWnd(pWnd, lpTitle)))
            {
                switch(m_nSkinId)
                {
                case sReadSkin:
                    if (m_pReadTextDoc)
                    {
                        m_pReadTextDoc.Release();
                        m_pReadTextDoc = NULL;
                    }
                    OnReadWndPrepare();
                    break;
                case sListenSkin:
                    OnListenPrepare();
                    break;
                case sDialogSkin:
                    OnDialogPrepare();
                    break;
                default:
                    break;
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //听力设置界面处理
        //默认
        else if (_T("listendefaultbtn") == strSender)
        {
            DefaultVoiceRateControl(&m_PaintManager);

            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("gradecmb")));
            pCombo->SelectItem(3);
            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb")));
            pCombo->SelectItem(2);
            pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("listentypezhineng")));
            pOpt->Selected(true);
        }
        //////////////////////////////////////////////////////////////////////////
        //听力设置和听力界面，都在此处理
        else if (_T("listenokbtn") == strSender)
        {
            ListenSubmitClick();
        }
        //////////////////////////////////////////////////////////////////////////
        //练听力界面
        else if (_T("listenstartbtn") == strSender)
        {
            ListenStartClick();
        }
        else if (_T("listenreviewbtn") == strSender)
        {
            StopSpeak();
            ListenSpeakVoice();
        }
        else if (_T("listenviewtextbtn") == strSender)
        {
            pREdt = xFindControl(CRichEditUI, _T("listenansweredt"));
            pREdt->SetVisible(true);
            pREdt->SetText(GetCollect(m_pListenQuestionPtr, _T("原文")));
        }
        else if (_T("listenagainbtn") == strSender)
        {
            OpenChildModalWnd(sListenSelectSkin);
        }
        else if (_T("listenextbtn") == strSender)
        {
            pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
            pLbl->SetText(_T(""));
            m_nListenQuestionIdx++;
            m_pListenQuestionPtr->MoveNext();
            InitListenQuestion(true);
        }
        //////////////////////////////////////////////////////////////////////////
        //录音机界面处理
        else if (strSender == _T("recordstart"))
        {
            SYSTEMTIME st;
            ::DeleteFile(m_strRecordFile);
            srand((unsigned)time(NULL));
            ::GetLocalTime(&st);
            m_strRecordFile.Format(_T("%s%s\\%04d%02d%02d%02d%02d%02d%03d_%d%d.wav"),
                CPaintManagerUI::GetInstancePath().GetData(), _T("Data"),
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                rand(), rand());
            if (ERROR_SUCCESS == m_WaveRecorder.StartRecord(m_strRecordFile))
            {
                pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("recordtimetxt")));
                pTxt->SetText(_T("00:00"));
                pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("recordplaytimetxt")));
                pTxt->SetText(_T("00:00"));

                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstart")));
                pButton->SetEnabled(false);
                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstop")));
                pButton->SetEnabled(true);

                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordplaybtn")));
                pButton->SetEnabled(false);
                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstopbtn")));
                pButton->SetEnabled(false);
                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordsavebtn")));
                pButton->SetEnabled(false);

                pPrgrs = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("recordplayprgs")));
                pPrgrs->SetValue(0);

                m_tmRecorderStartTime = CTime::GetCurrentTime();
                m_dwRecordSeconds = 0;
                ::KillTimer(GetHWND(), TIMER_ID_RECORD_PLAY);
                ::SetTimer(GetHWND(), TIMER_ID_RECORD, 1000, NULL);
            }
        }
        else if (strSender == _T("recordstop"))
        {
            m_WaveRecorder.StopRecord();
            ::KillTimer(GetHWND(), TIMER_ID_RECORD);

            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstart")));
            pButton->SetEnabled(true);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstop")));
            pButton->SetEnabled(false);

            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordplaybtn")));
            pButton->SetEnabled(true);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstopbtn")));
            pButton->SetEnabled(false);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordsavebtn")));
            pButton->SetEnabled(false);

            ::KillTimer(GetHWND(), TIMER_ID_RECORD_PLAY);
        }
        else if (strSender == _T("recordplaybtn"))
        {
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstart")));
            pButton->SetEnabled(false);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstop")));
            pButton->SetEnabled(false);

            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordplaybtn")));
            pButton->SetEnabled(false);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstopbtn")));
            pButton->SetEnabled(true);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordsavebtn")));
            pButton->SetEnabled(true);

            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("recordplaytimetxt")));
            pTxt->SetText(_T("00:00"));
            m_tmRecorderStartTime = CTime::GetCurrentTime();
            pPrgrs = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("recordplayprgs")));
            pPrgrs->SetValue(0);
            pPrgrs->SetMaxValue(m_dwRecordSeconds);
            m_mciWave.Play(GetHWND(), m_strRecordFile);
            ::SetTimer(GetHWND(), TIMER_ID_RECORD_PLAY, 1000, NULL);
        }
        else if (strSender == _T("recordstopbtn"))
        {
            m_mciWave.Stop();
            ::KillTimer(GetHWND(), TIMER_ID_RECORD_PLAY);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstart")));
            pButton->SetEnabled(true);

            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordplaybtn")));
            pButton->SetEnabled(true);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstopbtn")));
            pButton->SetEnabled(false);
        }
        else if (strSender == _T("recordsavebtn"))
        {
            CFileDialog dlgmp3(FALSE, _T("mp3"), NULL,
                OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
                _T("mp3文件(*.mp3)|*.mp3||"), CWnd::FromHandle(GetHWND()));
            dlgmp3.m_ofn.hwndOwner = GetHWND();
            OPENFILENAME& ofn = dlgmp3.GetOFN();
            TCHAR tbuf[4096];
            tbuf[0] = _T('\0');
            ofn.lpstrFile = tbuf;
            ofn.nMaxFile = _countof(tbuf);
            if (dlgmp3.DoModal() == IDOK)
            {
                hlpWave2Mp3(m_strRecordFile, dlgmp3.GetPathName());
                MyAfxMessageBox(_T("保存成功。"));
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //迷你朗读条处理
        else if (strSender == _T("readminiplaybtn"))
        {
            ReadBarGetText();
            if (m_strReadbarContent.empty())
            {
                MyAfxMessageBox(_T("没有可以播放的内容。"));
            }
            else
            {
                StopSpeak();
                pButton = static_cast<CButtonUI*>(msg.pSender);
                pButton->SetEnabled(false);
                pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("readministopbtn")));
                pButton->SetEnabled(true);

                Speak(m_strReadbarContent.c_str());
            }
        }
        else if (strSender == _T("readministopbtn"))
        {
            StopSpeak();
            pButton = static_cast<CButtonUI*>(msg.pSender);
            pButton->SetEnabled(false);
            pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("readminiplaybtn")));
            pButton->SetEnabled(true);
        }
        else if (strSender == _T("readminimp3btn"))
        {
            CFileDialog dlgmp3(FALSE, _T("mp3"), NULL,
                OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
                _T("mp3文件(*.mp3)|*.mp3||"), CWnd::FromHandle(GetHWND()));
            dlgmp3.m_ofn.hwndOwner = GetHWND();
            OPENFILENAME& ofn = dlgmp3.GetOFN();
            TCHAR tbuf[4096];
            tbuf[0] = _T('\0');
            ofn.lpstrFile = tbuf;
            ofn.nMaxFile = _countof(tbuf);
            if (dlgmp3.DoModal() == IDOK)
            {
                CString strPN = dlgmp3.GetPathName();
                CString strMp3 = strPN;
                CString strWave = strPN.TrimRight(_T("mp3")) + _T("wav");
                ReadBarSaveTextToSpeech(strWave);
                hlpWave2Mp3(strWave, strMp3);
                ::DeleteFile(strWave);
            }
        }
        else if (strSender == _T("readminisetbtn"))
        {
            if (IDOK == OpenModalWnd(new CReadingbarSetup(this), g_SkinInfo[sReadingbarCfgSkin].title))
            {
                OnReadBarPrepare();
            }
        }
        ///////////////////////////////////////////////////////////////////////////
        //记单词选择界面处理
        else if (strSender == _T("wordcfgokbtn"))
        {
            pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("wordcfgbookopt")));
            if (pOpt->IsSelected())
            {
                SaveSetup(_T("word_type"), 0);

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfggradecmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的年级。"));
                    return;
                }
                SaveSetup(_T("word_grade"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextbookcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的教材。"));
                    return;
                }
                SaveSetup(_T("word_book"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的课文。"));
                    return;
                }
                SaveSetup(_T("word_unit"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

                SaveSetup(_T("word_nwb"), 0);
                SaveSetup(_T("word_group"), _T("1"));
            }
            else
            {
                SaveSetup(_T("word_type"), 1);

                SaveSetup(_T("word_grade"), 0);
                SaveSetup(_T("word_book"), 0);
                SaveSetup(_T("word_unit"), 0);
                SaveSetup(_T("word_group"), _T("1"));

                pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgnewwordcmb")));
                if (pCombo->GetCount() <= 0)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可用的生词本。"));
                    return;
                }
                SaveSetup(_T("word_nwb"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData());
            }

            pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("wordcfglernopt")));
            OpenChildModalWnd(pOpt->IsSelected() ? sWordSkin : sWordExamSkin);
        }
        else if (_T("wordcfgmgtbtn") == strSender)
        {
            CWordMgtBook* pMgtBook = new CWordMgtBook(this);
            ShowWindow(false);
            i = OpenModalWnd(pMgtBook, _T("课本单词管理"));
            if (IDCANCEL == i)
            {
                TNotifyUI msg;
                msg.pSender = xFindControl(CControlUI, kCloseButtonName);
                msg.sType = DUI_MSGTYPE_CLICK;
                m_PaintManager.SendNotify(msg);
                ShowWindow(true);
            }
            else
            {
                ShowWindow(true);
                InitWordCfgControl();
            }
        }
        //记单词界面处理
        else if (strSender == _T("wordchangebtn"))
        {
            WordChangeBtnClicked();
        }
        else if (strSender == _T("wordspeakwordbtn"))
        {
            pTxt = xFindControl(CTextUI, _T("wordnametxt"));
            if (pTxt->GetText().GetLength() > 0)
            {
                m_bWordAutoRead = false;
                pButton = xFindControl(CButtonUI, _T("wordautoplaybtn"));
                pButton->SetText(_T("自动播放"));
                StopSpeak();
                Speak(pTxt->GetText().GetData());
            }
        }
        else if (strSender == _T("wordspeaksamplebtn"))
        {
            pREdt = xFindControl(CRichEditUI, _T("wordensampletxt"));
            if (pREdt->GetText().GetLength() > 0)
            {
                m_bWordAutoRead = false;
                pButton = xFindControl(CButtonUI, _T("wordautoplaybtn"));
                pButton->SetText(_T("自动播放"));
                StopSpeak();
                Speak(pREdt->GetText().GetData());
            }
        }
        else if (strSender == _T("wordautoplaybtn"))
        {
            WordAutoPlayClicked();
        }
        else if (strSender == _T("wordaddnewbtn"))
        {
            WordAddNewClicked();
        }
        else if (strSender == _T("wordmp3btn"))
        {
            WordMp3Clicked();
        }
        else if (_T("wordfollowwordbtn") == strSender)
        {
            WordFollowWordClicked();
        }
        else if (_T("wordfollowexamplebtn") == strSender)
        {
            WordFollowExampleClicked();
        }
        //////////////////////////////////////////////////////////////////////////
        //记单词设置界面
        else if (strSender == _T("wordsetupdefaultbtn"))
        {
            DefaultVoiceRateControl(&m_PaintManager);

            pCombo = xFindControl(CComboUI, _T("wordsetuptimescmb"));
            pCombo->SelectItem(0);

            pCombo = xFindControl(CComboUI, _T("wordsetupintervalcmb"));
            pCombo->SelectItem(0);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetupspellchk"));
            pCheckBox->SetCheck(false);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetupspeaksamplechk"));
            pCheckBox->SetCheck(false);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetuphidewordchk"));
            pCheckBox->SetCheck(false);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetuphideparachk"));
            pCheckBox->SetCheck(false);

            //pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
            //pCombo->SelectItem(0);
        }
        else if (strSender == _T("wordsetupokbtn"))
        {
            pCombo = xFindControl(CComboUI, _T("wordsetuptimescmb"));
            SaveSetup(_T("Word_RepeatTime"), pCombo->GetCurSel() + 1);

            pCombo = xFindControl(CComboUI, _T("wordsetupintervalcmb"));
            SaveSetup(_T("Word_SpaceTime"), pCombo->GetCurSel() + 1);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetupspellchk"));
            SaveSetup(_T("Word_Spell"), pCheckBox->GetCheck() ? 1 : 0);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetupspeaksamplechk"));
            SaveSetup(_T("Word_SpeakSample"), pCheckBox->GetCheck() ? 1 : 0);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetuphidewordchk"));
            SaveSetup(_T("Word_HideWord"), pCheckBox->GetCheck() ? 1 : 0);

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordsetuphideparachk"));
            SaveSetup(_T("Word_HideParaphrase"), pCheckBox->GetCheck() ? 1 : 0);

            SaveVoiceRate(&m_PaintManager, _T("Word_Speaker"), _T("Word_Speed"));

            //pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
            //SaveSetup(_T("word_sort"), pCombo->GetCurSel());

            OpenChildModalWnd(sWordSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词设置界面
        else if (strSender == _T("wordexmsetupdefaultbtn"))
        {
            DefaultVoiceRateControl(&m_PaintManager);
            pEdit = xFindControl(CEditUI, _T("wordexmnumberedt"));
            pEdit->SetText(_T("20"));

            pCheckBox = xFindControl(CCheckBoxUI, _T("wordexmautoaddchk"));
            pCheckBox->SetCheck(false);
        }
        else if (strSender == _T("wordexmsetupokbtn"))
        {
            pEdit = xFindControl(CEditUI, _T("wordexmnumberedt"));
            strTmp = pEdit->GetText().GetData();
            if (!IsNumeric(strTmp))
            {
                MyAfxMessageBox(_T("请输入数字。"));
                pEdit->SetFocus();
                return;
            }
            int xnum = _tstoi(strTmp);
            if ( (xnum <=0) || (xnum > 99) )
            {
                MyAfxMessageBox(_T("请输入0到100之间的数字。"));
                pEdit->SetFocus();
                return;
            }
            SaveSetup(_T("WordExam_QuestionNumber"), strTmp);

            SaveVoiceRate(&m_PaintManager, _T("WordExam_Speaker"), _T("WordExam_Speed"));
            
            pCheckBox = xFindControl(CCheckBoxUI, _T("wordexmautoaddchk"));
            SaveSetup(_T("WordExam_AotuAddWrongWord"), pCheckBox->GetCheck() ? _T("1") : _T("0"));

            OpenChildModalWnd(sWordExamSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词界面
        else if (strSender == _T("wordexamchangebtn"))
        {
            WordExamChangeBtnClicked();
        }
        else if (strSender == _T("wordexamspeakbtn"))
        {
            WordExamSpeakWord();
        }
        else if (strSender == _T("wordexamsubmitbtn"))
        {
            WordExamSubmitClicked();
        }
        else if (strSender == _T("wordexamokbtn"))
        {
            WordExamOKClicked();
        }
        else if (strSender == _T("wordexamaddnewbtn"))
        {
            WordExamAddWordClicked();
        }
        else if (strSender == _T("wordexamanswerbtn"))
        {
            WordExamAnswerClicked();
        }
        else if (strSender == _T("wordexamallchk"))
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            //此时是当前的状态，还未变化
            //TRACE(_T("state:%s\n"), pCheckBox->GetCheck() ? _T("checked") : _T("Unchecked"));
            WordExamSelectAlltype(!pCheckBox->GetCheck());
        }
        else if (i = IsWordExamTypeCheck(msg))
        {
            WordExamTypeClicked(i);
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词结果界面处理
        else if (strSender == _T("wordresultaddbtn"))
        {
            WordExamResultAddWrongWord();
        }
        else if (strSender == _T("wordexamresultagainbtn"))
        {
            OpenChildModalWnd(sWordExamSkin);
        }
        else if (_T("wordresultspeakbtn") == strSender)
        {
            pLbl = xFindControl(CLabelUI, _T("wordresultwordtxt"));
            Speak(pLbl->GetText().GetData());
        }
        //////////////////////////////////////////////////////////////////////////
        //学语法界面处理
        else if (strSender == _T("grambeforebtn"))
        {
            pCombo = xFindControl(CComboUI, _T("gramlistcmb"));
            if (pCombo->GetCurSel() > 0)
            {
                pCombo->SelectItem(pCombo->GetCurSel() - 1);
            }
        }
        else if (strSender == _T("gramnextbtn"))
        {
            pCombo = xFindControl(CComboUI, _T("gramlistcmb"));
            if (pCombo->GetCurSel() < pCombo->GetCount() - 1)
            {
                pCombo->SelectItem(pCombo->GetCurSel() + 1);
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //练语法设置界面
        else if (strSender == _T("gramsettingokbtn"))
        {
            pTab = xFindControl(CTabLayoutUI, _T("tabgrammer"));
            pTab->SelectItem(2);
            ::SetWindowText(GetHWND(), _T("练语法"));
            pCombo = xFindControl(CComboUI, _T("gramsettingclasscmb"));
            m_nGramLastClass = pCombo->GetCurSel();
            pCheckBox = xFindControl(CCheckBoxUI, _T("gramsettingbaseopt"));
            m_nGramLastLvel = pCheckBox->GetCheck() ? 0 : 1;
            InitGrammarExamCtrls();
        }
        //////////////////////////////////////////////////////////////////////////
        //练语法界面
        else if (strSender == _T("gramexamcancelbtn"))
        {
            pTab = xFindControl(CTabLayoutUI, _T("tabgrammer"));
            pTab->SelectItem(1);
            InitGrammarExamSettingCtrls();
        }
        else if (strSender == _T("gramexamanswerbtn"))
        {
            CString strAnswer;
            CString strTip;
            CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("gramexamresultedt"));

            strTmp = GetCollect(m_pGrammarRecdPtr, _T("答案"));
            strAnswer = strTmp.Left(1);
            strTip = strTmp.Right(strTmp.GetLength()-2);
            strTip.Trim();

            strTmp.Format(_T("本题答案：%s"),
                strAnswer);
            strTmp += _T("\n");
            strTmp += strTip;
            pREdt->SetText(strTmp);
        }
        else if (strSender == _T("gramexamnextbtn"))
        {
            GrammarNextClicked();
        }
        //////////////////////////////////////////////////////////////////////////
        //练语法结果界面
        else if (strSender == _T("gramresultagainbtn"))
        {
            m_bGramBackFromResult = true;
            OpenChildModalWnd(sGrammarSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //成绩报告界面
        else if ( (i = IsReportDetail(msg)) )
        {
            m_nReportDetailItem = i;
            OpenChildModalWnd(sReportDetailSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //音标界面
        else if (strSender == _T("symspeakwordbtn"))
        {
            strTmp.Format(_T("%sData\\Picture\\%s"),
                CPaintManagerUI::GetInstancePath().GetData(),
                (LPCTSTR)msg.pSender->GetUserData().GetData());
            CFileFind finder;
            if (finder.FindFile(strTmp))
            {
                finder.Close();
                m_mciWave.PlayWait(GetHWND(), strTmp);
                //m_mciWave.Play(GetHWND(), strTmp);
            }
        }
        else if (IsSymbolName(msg))
        {
            InitSymbolSymbol(msg.pSender->GetUserData().GetData());
        }
        else if (IsSymbolWord(msg))
        {
            Speak(msg.pSender->GetUserData().GetData());
        }
        //////////////////////////////////////////////////////////////////////////
        //娱乐界面
        if (strSender == _T("joystartbtn"))
        {
            JoyStartClicked();
        }
        if (strSender == _T("joyendbtn"))
        {
            JoyEndClicked();
        }
        if (strSender == _T("joystopbtn"))
        {
            JoyStopClicked();
        }
        else if (strSender == _T("joyplaybtn"))
        {
            JoyPlayClicked();
        }
        else if (strSender == _T("joyopenbtn"))
        {
            JoyOpenClicked();
        }
        else if (strSender == _T("joyaddbtn"))
        {
            JoyAddClicked();
        }
        else if (strSender == _T("joydelbtn"))
        {
            JoyDelClicked();
        }
        //////////////////////////////////////////////////////////////////////////
        //朗读界面处理
        else if (m_nSkinId == sReadSkin)
        {
            OnReadWndClicked(msg);
        }
        //////////////////////////////////////////////////////////////////////////
        //对话设置界面处理
        else if (_T("dlgfollowmeeditbtn") == strSender)
        {
            CDialogEdit* pdlg = new CDialogEdit(this);
            pdlg->m_nType = DIALOG_FOLLOWME;
            OpenModalWnd(pdlg, g_SkinInfo[sDialogEditSkin].title);
            InitDialogSelectStatus();
        }
        else if (_T("dlgtalkeditbtn") == strSender)
        {
            CDialogEdit* pdlg = new CDialogEdit(this);
            pdlg->m_nType = DIALOG_TALK;
            OpenModalWnd(pdlg, g_SkinInfo[sDialogEditSkin].title);
            InitDialogSelectStatus();
        }
        else if (_T("dlgokbtn") == strSender)
        {
            DialogSelectSubmit();
        }
        //////////////////////////////////////////////////////////////////////////
        //对话界面处理
        //跟我读
        else if (_T("dialogfollowleftbtn") == strSender)
        {
            DialogFollowLeftClicked();
        }
        else if (_T("dialogfollowstartbtn") == strSender)
        {
            DialogFollowStartClicked();
        }
        else if (_T("dialogfollowstopbtn") == strSender)
        {
            DialogFollowStopClicked();
        }
        else if (_T("dialogfollowrightbtn") == strSender)
        {
            DialogFollowRightClicked();
        }
        else if (_T("dialogfollowstatisbtn") == strSender)
        {
            DialogFollowStatisClicked();
        }
        else if (_T("dialogfollowlistbtn") == strSender)
        {
            DialogFollowListClicked();
        }
        else if (_T("dialogfollowreplaybtn") == strSender)
        {
            DialogFollowReplayClicked();
        }
        //人机对话
        else if (_T("dialogtalkspeakerbtn") == strSender)
        {
            DialogTalkSpeakClicked();
        }
        else if (_T("dialogtalkstartbtn") == strSender)
        {
            DialogTalkStartClicked();
        }
        else if (_T("dialogtalkstopbtn") == strSender)
        {
            DialogTalkStopClicked();
        }
        else if (_T("dialogtalkskipbtn") == strSender)
        {
            DialogTalkSkipClicked();
        }
        else if (_T("dialogtalklistbtn") == strSender)
        {
            DialogTalkListClicked();
        }
        else if (_T("dialogtalkstatisbtn") == strSender)
        {
            DialogTalkStaticClicked();
        }
        else if (_T("dialogtalkreplaybtn") == strSender)
        {
            m_mciWave.Stop();
            m_mciWave.PlayWait(GetHWND(), m_strCommonWaveFile);
        }
        //情景对话
        else if (_T("dialogscenetalkstartbtn") == strSender)
        {
            DialogSceneTalkStartClicked();
        }
        else if (_T("dialogscenetalkstopbtn") == strSender)
        {
            DialogSceneTalkStopClicked();
        }
        else if (_T("dialogscenetalkskipbtn") == strSender)
        {
            DialogSceneTalkSkipClicked();
        }
        else if (_T("dialogscenetalkstatisbtn") == strSender)
        {
            DialogSceneTalkSatisClicked();
        }
        //口头作文
        else if (_T("dialogwrittingrightbtn") == strSender)
        {
            DialogWrittingNext();
        }
        else if (_T("dialogwrittingstartbtn") == strSender)
        {
            DialogWrittingStart();
        }
        else if (_T("dialogwrittingsourcebtn") == strSender)
        {
            DialogWrittingSource();
        }
        else if (_T("dialogwrttingreadstartbtn") == strSender)
        {
            DialogWrittingStartRead();
        }
        else if (_T("dialogwrttingbeginspeakbtn") == strSender)
        {
            DialogWrittingBeginSpeak();
        }
        else if (_T("dialogwrttingstopspeakbtn") == strSender)
        {
            DialogWrittingStopSpeak();
        }
        //////////////////////////////////////////////////////////////////////////
        //单元测试界面
        else if (_T("papercfgokbtn") == strSender)
        {
            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("papercfggradecmb")));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可用的年级。"));
                return;
            }
            SaveSetup(_T("paper_grade"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("papercfgtextbookcmb")));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可用的教材。"));
                return;
            }
            SaveSetup(_T("paper_book"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

            pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("papercfgtextcmb")));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可用的课文。"));
                return;
            }
            SaveSetup(_T("paper_unit"), pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

            pOpt = xFindControl(COptionUI, _T("papercfgeasyopt"));
            CDlgPaper dlgpaper;
            if (!dlgpaper.PrepareHtml(this, pOpt->IsSelected() ? 1 : 2))
            {
                return;
            }
            ShowWindow(false);
            PaperPrepareImage();
            dlgpaper.DoModal();
            PaperCleanImage();
            ShowWindow(true);
            //TNotifyUI msg;
            //msg.pSender = xFindControl(CButtonUI, kReturnButtonName);
            //msg.sType = DUI_MSGTYPE_CLICK;
            //m_PaintManager.SendNotify(msg);
        }
        else if (_T("papercfgcheckbtn") == strSender)
        {
            ::CWaitCursor waitit;
            PaperCheckit();
        }
    }
    else if (sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        //////////////////////////////////////////////////////////////////////////
        //听力设置界面处理
        //题型
        if (strSender == _T("listentypezhineng"))
        {
            pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("listencfgtab"))); 
            //听力和听力设置界面，共用代码，pTab可能为空（听力界面无此控件）
            pTab ? pTab->SelectItem(0) : ListenTypeCfg(0);
        }
        else if (strSender == _T("listentypedanxiang"))
        {
            pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("listencfgtab")));
            pTab ? pTab->SelectItem(1) : ListenTypeCfg(1);
        }
        else if (strSender == _T("listentypequanzhen"))
        {
            pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("listencfgtab"))); 
            pTab ? pTab->SelectItem(2) : ListenTypeCfg(2);
        }
        else if (strSender == _T("listentypecuotiku"))
        {
            pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("listencfgtab")));
            pTab ? pTab->SelectItem(3) : ListenTypeCfg(3);
        }
        //全真试题checkbox
        else if (_T("listendanjuchk") == strSender)
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listendanjuedt")));
            pEdit->SetEnabled(pCheckBox->GetCheck());
        }
        else if (_T("listenduihuachk") == strSender)
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listenduihuaedt")));
            pEdit->SetEnabled(pCheckBox->GetCheck());
        }
        else if (_T("listenduanwenchk") == strSender)
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listenduanwenedt")));
            pEdit->SetEnabled(pCheckBox->GetCheck());
        }
        else if (_T("listentupianchk") == strSender)
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listentupianedt")));
            pEdit->SetEnabled(pCheckBox->GetCheck());
        }
        //////////////////////////////////////////////////////////////////////////
        //听力界面
        else if (i = IsListenOpt(msg))
        {
            ListenOptionClicked(i);
        }
        else if (i = IsListenPicOpt(msg))
        {
            ListenPicOptionClicked(i);
        }
        //////////////////////////////////////////////////////////////////////////
        //记单词界面
        else if (_T("wordcfgbookopt") == strSender)
        {
            WordSetCfgControl(true);
        }
        else if (_T("wordcfgnewwordopt") == strSender)
        {
            WordSetCfgControl(false);
        }
        else if (strSender == _T("wordpracticeopt"))
        {
            OpenChildModalWnd(sWordExamSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //记单词设置界面
        else if (strSender == _T("wordsetuphidewordchk"))
        {
        }
        else if (strSender == _T("wordsetuphideparachk"))
        {
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词界面
        else if (i = IsWordExamOpt(msg))
        {
            WordExamOptionClicked(i);
        }
        else if (strSender == _T("wordexamlernopt"))
        {
            OpenChildModalWnd(sWordSelectSkin);
        }
        //////////////////////////////////////////////////////////////////////////
        //学语法界面
        else if (strSender == _T("gramlernopt"))
        {
            pTab = xFindControl(CTabLayoutUI, _T("tabgrammer"));
            pTab->SelectItem(0);
            ::SetWindowText(GetHWND(), _T("学语法"));
        }
        else if (strSender == _T("gramexamopt"))
        {
            pTab = xFindControl(CTabLayoutUI, _T("tabgrammer"));
            if (m_bGramBackFromResult)
            {
                pTab->SelectItem(2);
                ::SetWindowText(GetHWND(), _T("练语法"));
            }
            else
            {
                pTab->SelectItem(1);
                ::SetWindowText(GetHWND(), _T("设置"));
                int nValue1 = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmGrammarLogFrom).GetTotalMinutes();
                if (nValue1 > 0)
                {
                    StudyLog(STUDY_TYPE_GRAMMAR, LOG_TYPE_TIME, nValue1, 0);
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //练语法界面
        else if (i = IsGrammarOpt(msg))
        {
            GrammarOptionClicked(i);
        }
        else if (strSender == _T("joymusicopt"))
        {
            //尽量不要在XML文件中，Option控件添加selected="true"
            //否则导致后面的控件还未创建，而Notify已经发送出来，此时pCtrl为NULL
            //pCtrl = xFindControl(CHorizontalLayoutUI, _T("joyplaycontrol"));
            //pCtrl->SetVisible(true);
            //pButton = xFindControl(CButtonUI, _T("joyaddbtn"));
            //pButton->SetVisible(true);
            //pButton = xFindControl(CButtonUI, _T("joydelbtn"));
            //pButton->SetVisible(true);
            ::KillTimer(GetHWND(), TIMER_ID_JOY_SONG);
            m_JoyWMPCurCtl.stop();
            pTab = xFindControl(CTabLayoutUI, _T("tabjoy"));
            pTab->SelectItem(0);
            InitJoyCtrls(0);
        }
        else if (strSender == _T("joyvedioopt"))
        {
            ::KillTimer(GetHWND(), TIMER_ID_JOY_SONG);
            m_JoyWMPCurCtl.stop();
            pTab = xFindControl(CTabLayoutUI, _T("tabjoy"));
            pTab->SelectItem(1);
            InitJoyCtrls(1);
        }
        else if (strSender == _T("joygameopt"))
        {
            ::KillTimer(GetHWND(), TIMER_ID_JOY_SONG);
            m_JoyWMPCurCtl.stop();
            pTab = xFindControl(CTabLayoutUI, _T("tabjoy"));
            pTab->SelectItem(2);
            InitJoyCtrls(2);
        }
        else if (strSender == _T("joyvoicechk"))
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            if (pCheckBox->GetCheck())
            {
                //m_lJoyVolumn = m_JoyWMPCurSettings.get_volume();
                //m_JoyWMPCurSettings.put_volume(0);
                m_JoyWMPCurSettings.put_mute(TRUE);
            }
            else
            {
                //m_JoyWMPCurSettings.put_volume(m_lJoyVolumn);
                m_JoyWMPCurSettings.put_mute(FALSE);
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //朗读界面处理
        else if (strSender == _T("storyreadopt"))
        {
            if (m_nReadType != READ_TYPE_READ)
            {
                pTab = xFindControl(CTabLayoutUI, _T("tabread"));
                pTab->SelectItem(0);
                InitReadCtrls(READ_TYPE_READ);
            }
        }
        else if (strSender == _T("storyfollowopt"))
        {
            if (m_nReadType != READ_TYPE_FOLLOW)
            {
                pTab = xFindControl(CTabLayoutUI, _T("tabread"));
                pTab->SelectItem(0);
                InitReadCtrls(READ_TYPE_FOLLOW);
            }
        }
        else if (strSender == _T("storydictationopt"))
        {
            if (READ_TYPE_DICTA != m_nReadType)
            {
                pTab = xFindControl(CTabLayoutUI, _T("tabread"));
                pTab->SelectItem(1);
                InitReadCtrls(READ_TYPE_DICTA);
            }
        }
        else if (strSender == _T("storyresiteopt"))
        {
            if (READ_TYPE_RECITE != m_nReadType)
            {
                pTab = xFindControl(CTabLayoutUI, _T("tabread"));
                pTab->SelectItem(2);
                InitReadCtrls(READ_TYPE_RECITE);
            }
        }
        else if (strSender == _T("readassitantbtn"))
        {
            pCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
            m_bReadHelper = pCheckBox->GetCheck();
        }
        //////////////////////////////////////////////////////////////////////////
        //对话设置界面处理
        else if (i = IsDialogSelectOpt(msg))
        {
            m_nDialogSelectPageId = i;
            InitDialogSelectStatus();
        }
        else if (_T("dlgwritehuatiopt") == strSender)
        {
            m_nDialogWrittingType = DLG_WRT_HUATI;
        }
        else if (_T("dlgwritekantuopt") == strSender)
        {
            m_nDialogWrittingType = DLG_WRT_KANTU;
        }
        else if (_T("dlgwritelangduopt") == strSender)
        {
            m_nDialogWrittingType = DLG_WRT_LANGDU;
        }
        //////////////////////////////////////////////////////////////////////////
        //对话界面处理
        else if (i = IsDialogOpt(msg))
        {
            DialogOptClicked(i);
        }
        else if (i = IsDialogWrittingOpt(msg))
        {
            m_nDialogWrittingQuestion = 0;
            DialogWrittingOptClicked(i-1);
        }
    }
    else if (sType == DUI_MSGTYPE_ITEMSELECT)
    {
        //////////////////////////////////////////////////////////////////////////
        //记单词设置处理
        if (strSender == _T("wordcfggradecmb"))
        {
            InitWordCfgBookCombo();
        }
        else if (strSender == _T("wordcfgtextbookcmb"))
        {
            InitWordCfgUnitCombo();
        }
        //////////////////////////////////////////////////////////////////////////
        //记单词处理
        else if (strSender == _T("wordgradecmb"))
        {
            InitWordBookCombo();
            //WordChangeAddNewWordBtn();
        }
        else if (strSender == _T("wordtextbookcmb"))
        {
            InitWordUnitCombo();
        }
        else if (strSender == _T("wordlist"))
        {
            WordChangeWordParaphrase();
            CControlUI* pCtrl = xFindControl(CControlUI, _T("wordfollowprogvlayout"));
            pCtrl->SetVisible(false);
            pCtrl = xFindControl(CControlUI, _T("wordScoreHlayout"));
            pCtrl->SetVisible(false);
            if (m_bWordAutoRead)
            {
                WordSpeakCurretnWordAuto();
            }
            //else if (m_bWordIsClickWordList)
            //{
            //    m_bWordIsClickWordList = false;
            //    WordSpeakCurrentWord();
            //}
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词界面
        else if (strSender == _T("wordexamgradecmb"))
        {
            InitWordExamBookCombo();
        }
        else if (strSender == _T("wordexamtextbookcmb"))
        {
            InitWordExamUnitCombo();
        }
        //////////////////////////////////////////////////////////////////////////
        //练单词结果界面
        else if (strSender == _T("wordresultlist"))
        {
            WordExamResultQuestionDetail();
        }
        //////////////////////////////////////////////////////////////////////////
        //学语法界面
        else if (strSender == _T("gramclasscmb"))
        {
            InitGrammarListCmb();
        }
        else if (strSender == _T("gramlistcmb"))
        {
            InitGrammarContent();
        }
        //////////////////////////////////////////////////////////////////////////
        //学语法结果界面
        else if (strSender == _T("gramresultlist"))
        {
            GrammarResultQuestionDetail();
        }
        //////////////////////////////////////////////////////////////////////////
        //娱乐界面
        else if (strSender == _T("joysonglist"))
        {
            InitJoyContent();
        }
        //////////////////////////////////////////////////////////////////////////
        //朗读处理
        else if (strSender == _T("readstorylist"))
        {
            InitReadContent();
        }
        //////////////////////////////////////////////////////////////////////////
        //对话选择界面处理
        else if (_T("dlgfollowmecmb") == strSender)
        {
            DialogSelectFollowmeCmbChanged();
        }
        else if (_T("dlgtalkcmb1") == strSender)
        {
            DialogSelectTalkCmbChanged();
        }
        //////////////////////////////////////////////////////////////////////////
        //单元测试界面处理
        else if (_T("papercfggradecmb") == strSender)
        {
            InitPaperCfgBookCombo();
        }
        else if (_T("papercfgtextbookcmb") == strSender)
        {
            InitPaperCfgUnitCombo();
        }
    }
    else if (sType == DUI_MSGTYPE_ITEMCLICK)
    {
        if (m_nSkinId == sWordSkin)
        {
            //pList = xFindControl(CListUI, _T("wordlist"));
            //TRACE(_T("index:%d, userdata:%s\n"), pList->GetCurSel(), pList->GetItemAt(pList->GetCurSel())->GetUserData().GetData());
            //TRACE(_T("parent:%s\n"), msg.pSender->GetParent()->GetName().GetData());
            WordStopAutoPlay();

            StopSpeak();
            //m_bWordIsClickWordList = true;
            WordSpeakCurrentWord();
        }
    }
    else if (sType == DUI_MSGTYPE_VALUECHANGED)
    {
        //////////////////////////////////////////////////////////////////////////
        //听力设置界面处理
        if (strSender == _T("listenspeed"))
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            strTmp.Format(_T("%d"), pSlider->GetValue());
            pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("listenspeedtxt")));
            pTxt->SetText(strTmp);
        }
        //////////////////////////////////////////////////////////////////////////
        //娱乐界面处理
        else if (strSender == _T("joyprogress"))
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            m_JoyWMPCurCtl.put_currentPosition(pSlider->GetValue() * m_dbJoyDuration / 100);
        }
        else if (strSender == _T("joyvolumn"))
        {
            pSlider = static_cast<CSliderUI*>(msg.pSender);
            m_JoyWMPCurSettings.put_volume(pSlider->GetValue());
        }
        //////////////////////////////////////////////////////////////////////////
        //对话界面处理
    }
    else if (sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (strSender == _T("readcontenttxt"))
        {
            //不需要，自动设置
            //pREdt = static_cast<CRichEditUI*>(msg.pSender);
            //pREdt->SetModify(true);
        }
    }
	else if (sType == DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}
    else if (sType == DUI_MSGTYPE_RETURN)
    {
        if (strSender == _T("wordexamansweredt"))
        {
            pButton = xFindControl(CButtonUI, _T("wordexamsubmitbtn"));
            //如果Edit有焦点，不会隐藏。。。
            pButton->SetFocus();
            WordExamSubmitClicked();
        }
    }
    else if (sType == DUI_MSGTYPE_TABSELECT)
    {
    }

    return;
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CButtonUI* pBtn;
    CTextUI* pTxt;
    CListUI* pList;
    CSliderUI* pSilder;
    CProgressUI* pPrg;
    CLabelUI* pLbl;
    CTabLayoutUI* pTab;
    COptionUI* pOpt;
    CRichEditUI* pREdt;
    CControlUI* pCtrl;
    CComboUI* pCmb;
    int i, n;
    CString strTmp;

    switch (uMsg)
    {
    case WM_TTSAPPCUSTOMEVENT:
        {
            bHandled = TRUE;
            CSpEvent        event;  // helper class in sphelper.h for events that releases any 
            // allocated memory in it's destructor - SAFER than SPEVENT
            SPVOICESTATUS   Stat;
            WPARAM          nStart;
            LPARAM          nEnd;
            HRESULT         hr = S_OK;

            while( event.GetFrom(m_cpVoice) == S_OK )
            {
                //TRACE(_T("eEventId:%d\n"), (int)event.eEventId);
                switch( event.eEventId )
                {
                case SPEI_START_INPUT_STREAM:
                    break; 

                case SPEI_END_INPUT_STREAM:
                    m_cpVoice->GetStatus(&Stat, NULL);
                    TRACE(_T("recv SPEI_END_INPUT_STREAM, dwRunningState:%d\n"), Stat.dwRunningState);
                    //点击停止导致发送的End_Stream，什么也不干
                    if ((Stat.dwRunningState & SPRS_IS_SPEAKING) == 0)
                    {
                        TRACE(_T("StopSpeak?\n"));
                        break;
                    }
                    if (m_nSkinId == sReadingbarSkin)
                    {                        
                        pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("readministopbtn")));
                        m_PaintManager.SendNotify(pBtn, DUI_MSGTYPE_CLICK);
                    }
                    else if (m_nSkinId == sWordSkin)
                    {
                        if (m_bWordAutoRead)
                        {
                            m_nWordSpeakCount++;
                            if (m_nWordSpeakCount == m_nWordModeBase)
                            {
                                m_nWordSpeakCount = 0;
                                pList = xFindControl(CListUI, _T("wordlist"));
                                i = pList->GetCurSel();
                                n = pList->GetCount();
                                if (i < n - 1)
                                {
                                    pList->SelectItem(i+1);
                                }
                                else
                                {
                                    //pBtn = xFindControl(CButtonUI, _T("wordautoplaybtn"));
                                    //pBtn->SetText(_T("自动播放"));
                                    WordStopAutoPlay();
                                }
                            }
                        }
                    }
                    else if (m_nSkinId == sReadSkin)
                    {
                        if ((m_nReadType == READ_TYPE_DICTA) || !m_bReadPlaying)
                        {
                            break;
                        }
                        //切换speaker
                        if (m_nReadType == READ_TYPE_READ)
                        {
                            if (m_bReadChangeSpeaker)
                            {
                                m_nReadCurrentSpeaker++;
                                m_nReadCurrentSpeaker %= 2;
                                SetVoiceRateByValue(m_nReadCurrentSpeaker ? m_strReadSecondSpeaker : m_strReadSpeaker,
                                    m_strReadSpeed);
                            }
                        }
                        if (READ_TYPE_FOLLOW == m_nReadType)
                        {
                            pPrg = xFindControl(CProgressUI, _T("readfollowprg"));
                            ULONGLONG intLastSeconds = CTimeSpan(CTime::GetCurrentTime() - m_tmReadSpokenFrom).GetTotalSeconds() + 2;
                            intLastSeconds *= 10;
                            pPrg->SetMaxValue((int)intLastSeconds);
                            NoticePrompt();
                            ::SetTimer(GetHWND(), TIMER_ID_READ_FOLLOW, 100, NULL);
                            SpeechRecognition_Start();
                        }
                        else if (READ_TYPE_READ == m_nReadType)
                        {
                            ReadSpeakNext_();
                        }
                        else if (READ_TYPE_RECITE == m_nReadType)
                        {
                            pCmb = xFindControl(CComboUI, _T("recitemancmb"));
                            i = pCmb->GetCurSel();
                            //如果是背诵第二句，在朗读完后启动录音
                            if (RECITE_2nd == i/* ||
                                RECITE_All == i*/ &&
                                m_bReadReciting)
                            {
                                if (m_nReadCurrentSentence < m_sptReadSentences.size())
                                {
                                    m_nReadCurrentSentence++;
                                    ReadStartRecite_();
                                }
                            }
                            else    //一次流程结束
                            {
                                bool nEnabled = (m_nReadCurrentSentence > m_sptReadSentences.size() - 1) ? false : true;
                                pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
                                pBtn->SetEnabled(nEnabled);
                                pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
                                pBtn->SetEnabled(nEnabled);
                                m_bReadReciting = false;
                            }
                        }
                    }
                    else if (sDialogSkin == m_nSkinId)
                    {
                        bool bContinue = false;
                        switch (m_nDialogSelectPageId)
                        {
                        case DIALOG_FOLLOWME:
                            {
                                if (!m_bDialogplaying)
                                {
                                    break;
                                }
                                pPrg = xFindControl(CProgressUI, _T("dialogfollowprgrs"));
                                if (m_bDialogCountTime)
                                {
                                    int nSec = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmDialogSpeakFrom).GetTotalSeconds();
                                    nSec *= 2;
                                    nSec += 1;
                                    nSec *= 10;
                                    pPrg->SetMaxValue(nSec);
                                    i = nSec;
                                }
                                else
                                {
                                    i = pPrg->GetMaxValue();
                                }
                                pPrg->SetValue(0);
                                pPrg->SetVisible(true);
                                pLbl = xFindControl(CLabelUI, _T("dialogfollownoticetxt"));
                                strTmp.Format(_T("开始录音。请在%d秒内完成朗读。"),
                                    i / 10);
                                pLbl->SetText(strTmp);
                                pLbl->SetVisible(true);
                                bContinue = true;
                            }
                        	break;
                        case DIALOG_TALK:
                            {
                                if (!m_bDialogplaying)
                                {
                                    break;
                                }
                                TRACE(_T("m_nDialogSpokenIndex:%d, m_nDialogAllCount:%d\n"),
                                    m_nDialogSpokenIndex, m_nDialogAllCount);
                                if ( (m_nDialogSpokenIndex >= (m_nDialogAllCount - 1)) ||
                                    m_pDialogSpokenPtr->GetadoEOF())
                                {
                                    DialogTalkEndOf();
                                    //MyAfxMessageBox(_T("%s"), _T("本组对话练习结束。"));
                                    break;
                                }
                                m_nDialogSpokenIndex++;
                                m_pDialogSpokenPtr->MoveNext();

                                i = 120;
                                //pREdt = xFindControl(CRichEditUI, _T("dialogtalkanswerentxt"));
                                //strTmp = pREdt->GetText();
                                //strTmp.Trim();
                                strTmp = DialogTalkGetContent();
                                if (!strTmp.IsEmpty())
                                {
                                    i = GetSentenceTime(strTmp);
                                }
                                DialogTalkSetAnswerText(i);

                                pCtrl = xFindControl(CControlUI, _T("dialogtalkquestionlayout"));
                                pCtrl->SetVisible(false);
                                pCtrl = xFindControl(CControlUI, _T("dialogtalkanswerlayout"));
                                pCtrl->SetVisible(true);
                                bContinue = true;
                            }
                            break;
                        case DIALOG_SCENETALK:
                            {
                                if (!m_bDialogplaying)
                                {
                                    break;
                                }
                                int xn = DialogSceneTalkTextSeconds(_T("回复1"));
                                i = DialogSceneTalkTextSeconds(_T("回复2"));
                                if (i > xn)
                                {
                                    xn = i;
                                }
                                i = DialogSceneTalkTextSeconds(_T("回复3"));
                                if (i > xn)
                                {
                                    xn = i;
                                }
                                //i = max(DialogSceneTalkTextSeconds(_T("回复1")),
                                //    max(DialogSceneTalkTextSeconds(_T("回复2")),
                                //            DialogSceneTalkTextSeconds(_T("回复3"))));
                                xn *= 10;
                                pPrg = xFindControl(CProgressUI, _T("dialogscenetalkprgrs"));
                                pPrg->SetMaxValue(xn);
                                pPrg->SetValue(0);
                                pCtrl = xFindControl(CControlUI, _T("dialogscenetalkanswerlayout"));
                                pCtrl->SetVisible(true);
                                pCtrl = xFindControl(CControlUI, _T("dialogscenetalknoticevlayout"));
                                pCtrl->SetVisible(true);

                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkansweren1txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复1")));
                                pREdt->SetTextColor(DUI_BLACK);
                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkanswercn1txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复1中文")));
                                pREdt->SetTextColor(DUI_BLACK);

                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkansweren2txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复2")));
                                pREdt->SetTextColor(DUI_BLACK);
                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkanswercn2txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复2中文")));
                                pREdt->SetTextColor(DUI_BLACK);

                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkansweren3txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复3")));
                                pREdt->SetTextColor(DUI_BLACK);
                                pREdt = xFindControl(CRichEditUI, _T("dialogtalkanswercn3txt"));
                                pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("回复3中文")));
                                pREdt->SetTextColor(DUI_BLACK);
                                bContinue = true;
                            }
                            break;
                        case DIALOG_WRITTING:
                            if (m_nDialogWrittingType == DLG_WRT_LANGDU)
                            {
                                if (!m_bDialogWrittingReading)
                                {
                                    break;
                                }
                                pPrg = xFindControl(CProgressUI, _T("dialogwrttingreadprgrs"));
                                m_nDialogWrittingLastSeconds = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmDialogSpeakFrom).GetTotalSeconds() + 2;
                                m_nDialogWrittingLastSeconds *= 10;
                                pPrg->SetMaxValue(m_nDialogWrittingLastSeconds);
                                //NoticePrompt();
                                pLbl = xFindControl(CLabelUI, _T("dialogwrttingreadnoticetxt"));
                                strTmp.Format(_T("录音倒计时 %d 秒。"),
                                    m_nDialogWrittingLastSeconds / 10);
                                pLbl->SetText(strTmp);
                            }
                            else if (!m_bDialogWrittingPlaying)
                            {
                                break;
                            }
                            bContinue = true;
                            break;
                        default:
                            break;
                        }
                        if (bContinue)
                        {
                            NoticePrompt();
                            ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE, 100, NULL);
                            SpeechRecognition_Start();
                        }
                    }
                    break;     

                case SPEI_VOICE_CHANGE:
                    break;

                case SPEI_TTS_BOOKMARK:
                    break;

                case SPEI_WORD_BOUNDARY:
                    hr = m_cpVoice->GetStatus( &Stat, NULL );
                    if( FAILED( hr ) )
                    {
                        TRACE(_T("Voice GetStatus error\r\n"));
                        break;
                    }

                    // Highlight word
                    if (sReadSkin == m_nSkinId)
                    {
                        if (m_bReadPlaying &&
                            (READ_TYPE_READ == m_nReadType) ||
                            (READ_TYPE_FOLLOW == m_nReadType))
                        {
                            nStart = Stat.ulInputWordPos + m_nReadCurrentCursor;
                            nEnd = nStart + Stat.ulInputWordLen;



                            CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
                            pREdt->SetSel(nStart, nEnd);

                            //EnSureVisible...
                            hlpRichEditEnsureVisible(pREdt);
                            //::CPoint cr_pt(0, 0);
                            //::GetCaretPos(&cr_pt);
                            //TRACE(_T("Caret x:%d, y:%d\n"), cr_pt.x, cr_pt.y);
                            //while (cr_pt.y < 0)   //if out of range, invisible
                            //{
                            //    long lLine = pREdt->LineFromChar(nStart), lFirst;
                            //    LRESULT lr = 0;
                            //    pREdt->TxSendMessage(EM_GETFIRSTVISIBLELINE, 0, 0, &lr);
                            //    lFirst = (long)lr;
                            //    pREdt->LineScroll((lLine > lFirst) ? 1 : -1);
                            //    ::GetCaretPos(&cr_pt);
                            //    TRACE(_T("Caret y:%d\n"), cr_pt.y);
                            //}
                        }
                    }
                    break;

                case SPEI_PHONEME:
                    break;

                case SPEI_VISEME:
                    // Get the current mouth viseme position and map it to one of the 
                    // 7 mouth bitmaps. 
                    break;

                case SPEI_SENTENCE_BOUNDARY:
                    break;

                case SPEI_TTS_AUDIO_LEVEL:
                    break;

                case SPEI_TTS_PRIVATE:
                    break;

                default:
                    TRACE( _T("Unknown message:%d\n"), event.eEventId );
                    break;
                }
            }
        }
        break;
    case WM_SRAPPCUSTOMEVENT:
        {
            bHandled = TRUE;
            CSpEvent        event;  // helper class in sphelper.h for events that releases any 
            // allocated memory in it's destructor - SAFER than SPEVENT
            //SPVOICESTATUS   Stat;
            //WPARAM          nStart;
            //LPARAM          nEnd;
            HRESULT         hr = S_OK;

            while( event.GetFrom(m_cpRecoCtxt) == S_OK )
            {
                TRACE(_T("m_cpRecognizer, eEventId:%d\n"), event.eEventId);
                switch(event.eEventId)
                {
                case SPEI_END_SR_STREAM:
                    m_cpGrammar->SetDictationState(SPRS_INACTIVE);
                    SREndStream(event);
                    break;
                case SPEI_RECOGNITION:
                    {
                        //if (sReadSkin == m_nSkinId)
                        {
                            //m_cpGrammar->SetDictationState(SPRS_INACTIVE);
                            SRRecognition(event);
                        }
                    }
                    break;
                case SPEI_SOUND_START:
                    break;
                case SPEI_SOUND_END:
                    break;
                case SPEI_PHRASE_START:
                    break;
                case SPEI_HYPOTHESIS:
                    break;
                case SPEI_SR_BOOKMARK:
                    break;
                case SPEI_PROPERTY_NUM_CHANGE:
                    break;
                case SPEI_PROPERTY_STRING_CHANGE:
                    break;
                case SPEI_FALSE_RECOGNITION:
                    break;
                case SPEI_INTERFERENCE:
                    break;
                case SPEI_REQUEST_UI:
                    break;
                case SPEI_RECO_STATE_CHANGE:
                    break;
                case SPEI_ADAPTATION:
                    break;
                case SPEI_START_SR_STREAM:
                    break;
                case SPEI_RECO_OTHER_CONTEXT:
                    break;
                case SPEI_SR_AUDIO_LEVEL:
                    break;
                //case SPEI_SR_RETAINEDAUDIO:
                //    break;
                default:
                    break;
                }
            }
        }
        break;
    case WM_SRENDSTREAM:
        bHandled = TRUE;
        {
            if (sReadSkin == m_nSkinId)
            {
                if (READ_TYPE_FOLLOW == m_nReadType)
                {
                    if (m_strCloseFlag == CF_CLOSE)
                    {
                        TRACE(_T("CloseFlag:%s\n"), m_strCloseFlag);
                        break;
                    }

                    i = SpeechRecognitionCompare(m_strReadSpokenText, strTmp);
                    if (0 == i)
                    {
                        i = SpeechSpokenScoreToLevel();
                    }
                    else
                    {
                        AfxMessageBox(strTmp);
                    }

                    //

                    pLbl = xFindControl(CLabelUI, _T("readScore"));
                    if (IsNumeric(m_strRecognitionCompare[8]))
                    {
                        //pLbl->SetText(m_strRecognitionCompare[8]);
                    }
                    else
                    {
                        //srand((unsigned int)time(NULL));
                        //strTmp.Format(_T("%d"),  (int)((double)rand() / (RAND_MAX + 1) * 30));
                        //TRACE(_T("没有成绩，假分：%s\n"),
                        //    strTmp);
                        //pLbl->SetText(strTmp);
                        m_strRecognitionCompare[8] = _T("0");
                    }
                    pLbl->SetText(m_strRecognitionCompare[8]);

                    CHorizontalLayoutUI* pHo = xFindControl(CHorizontalLayoutUI, _T("readScoreHlayout"));
                    // 基础版本不显示得分 [2016-1-16 Arthur]
#ifdef APP_DEMO
                    pHo->SetVisible(false);
#else
                    pHo->SetVisible(true);
#endif

                    //m_strReadUnknowWord += m_strRecognitionCompare[7] + _T(",");
                    //m_strReadUnknowSymbol += m_strRecognitionCompare[11] + _T(",");
                    m_strReadUnknowWord = m_strRecognitionCompare[7];
                    m_strReadUnknowSymbol = m_strRecognitionCompare[11];

                    pBtn = xFindControl(CButtonUI, _T("storyplaybackbtn"));
                    pBtn->SetVisible(true);

                    //Sleep(900);
                    //Sleep(900);
                    //Sleep(900);
                    //Sleep(900);
                    //Sleep(900);

                    //ReadSpeakNext_();
                    ::SetTimer(GetHWND(), TIMER_ID_READ_NEXT, TIMER_INTERVAL_NEXT, NULL);
                }
                else if (READ_TYPE_RECITE == m_nReadType)
                {
                    if (m_strCloseFlag == CF_CLOSE)
                    {
                        TRACE(_T("CloseFlag:%s\n"), m_strCloseFlag);
                        break;
                    }

                    pCmb = xFindControl(CComboUI, _T("recitemancmb"));
                    int rtt = pCmb->GetCurSel();
                    CString strAllText;
                    //if (RECITE_All == rtt)
                    //{
                    //    pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
                    //    strAllText = pREdt->GetText(GT_USECRLF);
                    //    rtt = ct_pred;
                    //}
                    //else
                    {
                        ASSERT(m_nReadCurrentSentence < m_sptReadSentences.size());
                        strAllText = m_sptReadSentences[m_nReadCurrentSentence].c_str();
                        rtt = ct_sent;
                    }
                    i = SpeechRecognitionCompare(strAllText, strTmp, (MainFrame::SRCoreType)rtt);
                    if (0 == i)
                    {
                        i = SpeechSpokenScoreToLevel();
                    }
                    else
                    {
                        AfxMessageBox(strTmp);
                    }

                    pREdt = xFindControl(CRichEditUI, _T("recitetxt"));

                    if (!IsNumeric(m_strRecognitionCompare[8]))
                    {
                        m_strRecognitionCompare[8] = _T("0");
                    }
                    int iScore = _tstoi(m_strRecognitionCompare[8]);
                    Speak(strAllText);
                    //if (iScore < 70)
                    {
                        MyBadWordVector::const_iterator pBad;
                        int s, e;
                        if (ct_pred == rtt)
                        {
                            for (pBad = m_aReadbadWord.begin(); pBad != m_aReadbadWord.end(); ++pBad)
                            {
                                s = pREdt->SetSel(-1, -1);
                                SetRichEditSelColor(pREdt, RGB(0, 0, 0));
                                hlpAppedText(pREdt, pBad->m_strWord);
                                e = pREdt->SetSel(-1, -1);
                                //hlpRichEditEnsureVisible(pREdt);
                                //TRACE(_T("s:%d,e:%d\n"), s, e);
                                if (pBad->m_nScore < 70)
                                {
                                    pREdt->SetSel(s, e);
                                    SetRichEditSelColor(pREdt, RGB(255, 0, 0));
                                }
                            }
                        }
                        else
                        {
                            int nIdx = pREdt->SetSel(-1, -1);
                            int nOff = 0;
                            hlpAppedText(pREdt, strAllText);
                            int nEnd = pREdt->SetSel(-1, -1);
                            //hlpRichEditEnsureVisible(pREdt);
                            //SpeechRecognitionCompare函数会调用CheckRefText
                            CAiEngine::CheckRefText(strAllText);
                            strAllText.MakeLower();
                            TRACE(_T("strAllText:%s\n"), strAllText);

                            for (pBad = m_aReadbadWord.begin(); pBad != m_aReadbadWord.end(); ++pBad)
                            {
                                int nx = strAllText.Find(pBad->m_strWord, nOff);
                                //TRACE(_T("word:%s, score:%d, offset:%d\n"), pBad->m_strWord, pBad->m_nScore, nx);
                                if (nx < 0)
                                {
                                    //what?!
                                    continue;
                                }
                                //update
                                nOff = nx + pBad->m_strWord.GetLength();
                                //TRACE(_T("nOff:%d, pBad->m_strWord.GetLength():%d\n"), nOff, pBad->m_strWord.GetLength());
                                if (pBad->m_nScore < 70)
                                {
                                    s = nIdx + nx;
                                    e = nIdx + nx + pBad->m_strWord.GetLength() + 1;
                                    if (e > nEnd)
                                    {
                                        TRACE(_T("warning, e=%d > nEnd=%d!\n"), e, nEnd);
                                        e = nEnd;
                                    }
                                    pREdt->SetSel(s, e);
                                    SetRichEditSelColor(pREdt, RGB(255, 0, 0));
                                }
                            }
                        }
                    }
                    //pREdt->SetSelNone();
                    pREdt->SetSel(-1, -1);
                    SetRichEditSelColor(pREdt, RGB(0, 0, 0));
                    strTmp.Format(_T("==========本次得分：%d\n"),
                        iScore);
                    hlpAppedText(pREdt, strTmp);
                    pREdt->SetSel(-1, -1);
                    //hlpRichEditEnsureVisible(pREdt);

                    pCmb = xFindControl(CComboUI, _T("recitemancmb"));
                    //如果是背诵第一句，在录完音打分后后启动朗读
                    i = pCmb->GetCurSel();
                    TRACE(_T("m_nReadCurrentSentence:%d, m_sptReadSentences.size():%d\n"),
                        m_nReadCurrentSentence, m_sptReadSentences.size());
                    if (RECITE_1st == i)
                    {
                        if (m_nReadCurrentSentence < m_sptReadSentences.size() - 1)
                        {
                            m_nReadCurrentSentence++;
                            hlpAppedText(pREdt, m_sptReadSentences[m_nReadCurrentSentence].c_str());
                            Speak(m_sptReadSentences[m_nReadCurrentSentence].c_str());
                        }
                        else
                        {
                            pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
                            pBtn->SetEnabled(false);
                            pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
                            pBtn->SetEnabled(false);
                        }
                    }
                    //背诵偶数句，全文背诵,一次流程结束
                    else
                    {
                        bool bEnabled = (m_nReadCurrentSentence >= (m_sptReadSentences.size() - 1)) ? false : true;
                        pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
                        pBtn->SetEnabled(bEnabled);
                        pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
                        pBtn->SetEnabled(bEnabled);
                        m_bReadReciting = false;
                    }

                    //if (m_nReadCurrentSentence < m_sptReadSentences.size() - 1)
                    //{
                    //    pCmb = xFindControl(CComboUI, _T("recitemancmb"));
                    //    m_nReadCurrentSentence++;
                    //    i = pCmb->GetCurSel();
                    //    if (2 == i)
                    //    {
                    //        ReadStartRecite_();
                    //    }
                    //    else
                    //    {
                    //        Speak(m_sptReadSentences[m_nReadCurrentSentence].c_str());
                    //    }
                    //}
                }
            }
            else if (sDialogSkin == m_nSkinId)
            {
                switch (m_nDialogSelectPageId)
                {
                case DIALOG_FOLLOWME:
                    {
                        int num = 0;
                        int num2 = SpeechRecognitionCompare(GetCollect(m_pDialogSpokenPtr, _T("内容")), strTmp);
                        if (0 == num2)
                        {
                            num = SpeechSpokenScoreToLevel();
                        }
                        else
                        {
                            AfxMessageBox(strTmp);
                        }
                        /*
                        Select Case num
                        Case 0
                        Me.lblSpokenResult.Image = Image.FromFile((Me.imgPath & "Level2.png"))
                        Exit Select
                        Case 1
                        Me.lblSpokenResult.Image = Image.FromFile((Me.imgPath & "Level1.png"))
                        Exit Select
                        Case 2, 3
                        Me.lblSpokenResult.Image = Image.FromFile((Me.imgPath & "Level0.png"))
                        Exit Select
                        End Select
                        */
                        pBtn = xFindControl(CButtonUI, _T("dialogfollowreplaybtn"));
                        pBtn->SetVisible(true);
                        pCtrl = xFindControl(CControlUI, _T("dialogfollowfunlayout"));
                        // 基础版本不显示得分 [2016-1-16 Arthur]
#ifdef APP_DEMO
                        pCtrl->SetVisible(false);
#else
                        pCtrl->SetVisible(true);
#endif

                        if (!IsNumeric(m_strRecognitionCompare[8]))
                        {
                            m_strRecognitionCompare[8] = _T("0");
                        }

                        pLbl = xFindControl(CLabelUI, _T("dialogfollowScoretxt"));
                        pLbl->SetText(m_strRecognitionCompare[8]);

                        //m_strDialogUnknowWord += m_strRecognitionCompare[7] + _T(",");
                        //m_strDialogUnknowSymbol += m_strRecognitionCompare[11] + _T(",");
                        m_strDialogUnknowWord = m_strRecognitionCompare[7];
                        m_strDialogUnknowSymbol = m_strRecognitionCompare[11];

                        m_nDialogValue1++;
                        m_nDialogValue2 += _tstoi(m_strRecognitionCompare[8]);

                        if (/*num2 != 0 ||*/
                            num == 0)
                        {
#ifndef APP_DEMO
                            m_bDialogCountTime = false;
                            StopSpeak();
                            Speak(_T("I beg your pardon?"));
#endif
                        }

                        //pBtn = xFindControl(CButtonUI, _T("dialogfollowstartbtn"));
                        //pBtn->SetEnabled(true);
                        //pBtn = xFindControl(CButtonUI, _T("dialogfollowleftbtn"));
                        //pBtn->SetEnabled(true);
                        //pBtn = xFindControl(CButtonUI, _T("dialogfollowrightbtn"));
                        //pBtn->SetEnabled(true);
                    }
                    break;
                case DIALOG_TALK:
                    {
                        int num = 0;
                        int num2 = SpeechRecognitionCompare(DialogTalkGetContent(), strTmp);
                        if (0 == num2)
                        {
                            num = SpeechSpokenScoreToLevel();
                        }
                        else
                        {
                            AfxMessageBox(strTmp);
                        }

                        if (!IsNumeric(m_strRecognitionCompare[8]))
                        {
                            m_strRecognitionCompare[8] = _T("0");
                        }
                        pCtrl = xFindControl(CControlUI, _T("dialogtalkscorelayout"));
                        // 基础版本不显示得分 [2016-1-16 Arthur]
#ifdef APP_DEMO
                        pCtrl->SetVisible(false);
#else
                        pCtrl->SetVisible(true);
#endif
                        pLbl = xFindControl(CLabelUI, _T("dialogtalkscoretxt"));
                        pLbl->SetText(m_strRecognitionCompare[8]);
                        //m_strDialogUnknowWord += m_strRecognitionCompare[7] + _T(",");
                        //m_strDialogUnknowSymbol += m_strRecognitionCompare[11] + _T(",");
                        m_strDialogUnknowWord = m_strRecognitionCompare[7];
                        m_strDialogUnknowSymbol = m_strRecognitionCompare[11];

                        m_nDialogValue1++;
                        m_nDialogValue2 += _tstoi(m_strRecognitionCompare[8]);
                        ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT, TIMER_INTERVAL_NEXT, NULL);
                    }
                    break;
                case DIALOG_SCENETALK:
                    {
                        int num[3], nmax = 0;
                        static LPCTSTR s_ANSWER[] =
                        {
                            _T("回复1"),
                            _T("回复2"),
                            _T("回复3")
                        };
                        static LPCTSTR s_CTRL_EN[] =
                        {
                            _T("dialogtalkansweren1txt"),
                            _T("dialogtalkansweren2txt"),
                            _T("dialogtalkansweren3txt")
                        };
                        static LPCTSTR s_CTRL_CN[] =
                        {
                            _T("dialogtalkanswercn1txt"),
                            _T("dialogtalkanswercn2txt"),
                            _T("dialogtalkanswercn3txt")
                        };
                        CRichEditUI* pCtrl2;
                        for (i = 0; i < 3; i++)
                        {
                            SpeechRecognitionCompare(GetCollect(m_pDialogSpokenPtr, s_ANSWER[i]), strTmp);
                            num[i] = _tstoi(m_strRecognitionCompare[8]);
                            if (num[i] > nmax)
                            {
                                nmax = num[i];
                                pREdt = xFindControl(CRichEditUI, s_CTRL_EN[i]);
                                pCtrl2 = xFindControl(CRichEditUI, s_CTRL_CN[i]);
                            }
                        }

                        if (nmax > 0)
                        {
                            pREdt->SetTextColor(DUI_RGB(178,34,34));
                            pCtrl2->SetTextColor(DUI_RGB(178,34,34));
                            //m_strDialogUnknowWord += m_strRecognitionCompare[7] + _T(",");
                            //m_strDialogUnknowSymbol += m_strRecognitionCompare[11] + _T(",");
                            m_strDialogUnknowWord = m_strRecognitionCompare[7];
                            m_strDialogUnknowSymbol = m_strRecognitionCompare[11];
                            m_nDialogValue1++;
                            m_nDialogValue2 += nmax;
                            ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT, TIMER_INTERVAL_NEXT, NULL);
                        }
                        else
                        {
                            StopSpeak();
                            Speak(_T("I beg your pardon?"));
                        }
                    }
                    break;
                case DIALOG_WRITTING:
                    if (m_nDialogWrittingType == DLG_WRT_LANGDU)
                    {
                        i = SpeechRecognitionCompare(m_strDialogSpokenText, strTmp, ct_pred);
                        if (0 == i)
                        {
                            i = SpeechSpokenScoreToLevel();
                        }
                        else
                        {
                            AfxMessageBox(strTmp);
                        }
                        if (IsNumeric(m_strRecognitionCompare[8]))
                        {
                            //strTmp.Format(_T("本句得分：%s\r\n\r\n"),
                            //    m_strRecognitionCompare[8]);
                        }
                        else
                        {
                            //srand((unsigned int)time(NULL));
                            //strTmp.Format(_T("本句得分：%d\r\n\r\n"),
                            //    (int)((double)rand() / (RAND_MAX + 1) * 30));
                            //TRACE(_T("没有成绩，假分：%s\n"),
                            //    strTmp);
                            m_strRecognitionCompare[8] = _T("0");
                        }
                        strTmp.Format(_T("本句得分：%s\r\n\r\n"),
                            m_strRecognitionCompare[8]);
                        pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
#ifdef APP_DEMO
                        pREdt->SetText(CString(pREdt->GetText()) + _T("\r\n—————————————————————————\r\n"));

#else
                        pREdt->SetText(CString(pREdt->GetText()) + _T("\r\n—————————————————————————\r\n") + 
                            strTmp);
#endif
                    }
                    else// if ()
                    {
                        i = SpeechRecognitionCompare(m_sptDialogWrittingAnswer[m_nDialogSpokenIndex].c_str(), strTmp);
                        if (0 == i)
                        {
                            i = SpeechSpokenScoreToLevel();
                        }
                        else
                        {
                            AfxMessageBox(strTmp);
                        }
                        if (IsNumeric(m_strRecognitionCompare[8]))
                        {
                            //strTmp.Format(_T("本句得分：%s\r\n\r\n"),
                            //    m_strRecognitionCompare[8]);
                        }
                        else
                        {
                            //srand((unsigned int)time(NULL));
                            //strTmp.Format(_T("本句得分：%d\r\n\r\n"),
                            //    (int)((double)rand() / (RAND_MAX + 1) * 30));
                            //TRACE(_T("没有成绩，假分：%s\n"),
                            //    strTmp);
                            m_strRecognitionCompare[8] = _T("0");
                        }
                        strTmp.Format(_T("本句得分：%s\r\n\r\n"),
                            m_strRecognitionCompare[8]);
                        pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
#ifdef APP_DEMO
                        pREdt->SetText(CString(pREdt->GetText()) + m_sptDialogWrittingAnswer[m_nDialogSpokenIndex].c_str() + _T("\r\n—————————————————————————\r\n"));
#else
                        pREdt->SetText(CString(pREdt->GetText()) + m_sptDialogWrittingAnswer[m_nDialogSpokenIndex].c_str() + _T("\r\n—————————————————————————\r\n") + 
                            strTmp);
#endif
                        i = pREdt->GetText().GetLength();
                        pREdt->SetSel(i, i);

                        pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
                        pREdt->SetTextColor(DUI_BLACK);

                        m_nDialogSpokenIndex++;
                    }
                    ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT, TIMER_INTERVAL_NEXT, NULL);
                    break;
                default:
                    break;
                }
            }
            else if (sWordSkin == m_nSkinId)
            {
                CString strWordText;
                SRCoreType st;
                if (m_bWordFollowWord)
                {
                    pTxt = xFindControl(CTextUI, _T("wordnametxt"));
                    strWordText = pTxt->GetText();
                    st = ct_word;
                }
                else
                {
                    pREdt = xFindControl(CRichEditUI, _T("wordensampletxt"));
                    strWordText = pREdt->GetText();
                    st = ct_sent;
                }
                i = SpeechRecognitionCompare(strWordText, strTmp, st);
                if (0 == i)
                {
                    i = SpeechSpokenScoreToLevel();
                }
                else
                {
                    AfxMessageBox(strTmp);
                }

                pCtrl = xFindControl(CControlUI, _T("wordScoreHlayout"));
                pCtrl->SetVisible(true);

                pLbl = xFindControl(CLabelUI, _T("wordScore"));
                if (IsNumeric(m_strRecognitionCompare[8]))
                {
                    //pLbl->SetText(m_strRecognitionCompare[8]);
                }
                else
                {
                    //srand((unsigned int)time(NULL));
                    //strTmp.Format(_T("%d"),  (int)((double)rand() / (RAND_MAX + 1) * 30));
                    //TRACE(_T("没有成绩，假分：%s\n"),
                    //    strTmp);
                    //pLbl->SetText(strTmp);
                    m_strRecognitionCompare[8] = _T("0");
                }
                pLbl->SetText(m_strRecognitionCompare[8]);

                MyBadWordVector::const_iterator p;
                if (m_bWordFollowWord)
                {
                    pREdt = xFindControl(CRichEditUI, _T("wordsymboltxt"));
                    strTmp = pREdt->GetText();
                    i = strTmp.Find(_T('/'));
                    n = strTmp.ReverseFind(_T('/'));
                    pREdt->SetSel(i+1, n);
                    SetRichEditSelColor(pREdt, RGB(255,0,0));
                    
                    TRACE(_T("all symbol:%s\n"), strTmp);
                    CString strSym;
                    i = 0;
                    for (p = m_aReadbadWord.begin(); p != m_aReadbadWord.end(); ++p)
                    {
                        strSym = m_AiEngine.GetSpeechChar(p->m_strWord);
                        n = strTmp.Find(strSym, i);
                        TRACE(_T("symbol:%s, pos:%d, len:%d\n"), strSym, n, strSym.GetLength());
                        i = n + strSym.GetLength();
                        if ((n >= 0) && p->m_nScore >= 70)
                        {
                            pREdt->SetSel(n, n+strSym.GetLength()+1);
                            SetRichEditSelColor(pREdt, RGB(0,0,255));                            
                        }
                    }
                }
                else
                {
                    pREdt = xFindControl(CRichEditUI, _T("wordensampletxt"));
                    strWordText = pREdt->GetText();
                    strWordText.MakeLower();
                    i = 0;
                    for (p = m_aReadbadWord.begin(); p != m_aReadbadWord.end(); ++p)
                    {
                        n = strWordText.Find(p->m_strWord, i);
                        TRACE(_T("word:%s, pos:%d, len:%d, Score:%d\n"),
                            p->m_strWord, n, p->m_strWord.GetLength(), p->m_nScore);
                        i = n + p->m_strWord.GetLength();
                        pREdt->SetSel(n, n+p->m_strWord.GetLength()+1);
                        if (p->m_nScore < 70)
                        {
                            SetRichEditSelColor(pREdt, RGB(255,0,0));
                        }
                        else
                        {
                            SetRichEditSelColor(pREdt, RGB(0,0,0));
                        }
                    }
                }
            }
        }
        break;
    case WM_TIMER:
        bHandled = TRUE;
        switch (wParam)
        {
        //case TIMER_ID_RECOINPUT:
        //    SpeechRecognition_Start();
        //    //if (!m_bRecognizerInput)
        //    //{
        //    //    ::KillTimer(GetHWND(), TIMER_ID_RECOINPUT);
        //    //    SpeechRecognition_Start_();
        //    //}
        //    break;
        case TIMER_ID_RECORD:
            {
                pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("recordtimetxt")));
                CTimeSpan tspan = CTime::GetCurrentTime() - m_tmRecorderStartTime;
                m_dwRecordSeconds = (DWORD)tspan.GetTotalSeconds();
                pTxt->SetText(tspan.Format(_T("%M:%S")));
            }
            break;
        case TIMER_ID_RECORD_PLAY:
            {
                CTimeSpan tspan = CTime::GetCurrentTime() - m_tmRecorderStartTime;
                DWORD val = (DWORD)tspan.GetTotalSeconds();
                if (val <= m_dwRecordSeconds)
                {
                    pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("recordplaytimetxt")));
                    pTxt->SetText(tspan.Format(_T("%M:%S")));
                    pPrg = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("recordplayprgs")));
                    pPrg->SetValue(val);
                }
                else
                {
                    ::KillTimer(GetHWND(), TIMER_ID_RECORD_PLAY);
                }
            }
            break;
        case TIMER_ID_JOY_SONG:
            {
                TRACE(_T("m_pJoyWMPCur->get_playState():%d\n"), m_pJoyWMPCur->get_playState());
                if ( (m_pJoyWMPCur->get_playState() != wmppsStopped) ||
                    (m_pJoyWMPCur->get_playState() != wmppsUndefined) )
                {
                    pLbl = xFindControl(CLabelUI, _T("joyplayprogtxt"));
                    pSilder = xFindControl(CSliderUI, _T("joyprogress"));
                    strTmp.Format(_T("%s / %s"),
                        m_JoyWMPCurCtl.get_currentPositionString(),
                        m_strJoyDuration);
                    pLbl->SetText(strTmp);
                    double dbCur = m_JoyWMPCurCtl.get_currentPosition();
                    i = (int)(dbCur * 100 / m_dbJoyDuration);
                    pSilder->SetValue(i);
                }
                if (m_pJoyWMPCur->get_playState() == wmppsStopped)
                {
                    //JoyStopClicked();
                    m_PaintManager.SendNotify(xFindControl(CControlUI, _T("joystopbtn")), DUI_MSGTYPE_CLICK);
                    //::KillTimer(GetHWND(), TIMER_ID_JOY_SONG);
                }
            }
            break;
        case TIMER_ID_WORD_EXAM:
            {
                ::KillTimer(GetHWND(), TIMER_ID_WORD_EXAM);
                m_nWordExamQuestions++;
                m_pWordExamRecdPtr->MoveNext();
                InitWordExamQuestion();
            }
            break;
        case TIMER_ID_READ_FOLLOW:
            {
                pPrg = xFindControl(CProgressUI, _T("readfollowprg"));
                n = pPrg->GetValue();
                if (n < pPrg->GetMaxValue())
                {
                    n++;
                    pPrg->SetValue(n);
                }
                else
                {
                    ::KillTimer(GetHWND(), TIMER_ID_READ_FOLLOW);
                    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("readprogvlayout"));
                    pVLay->SetVisible(false);
                    SpeechRecognition_Stop();
                }
            }
            break;
        case TIMER_ID_READ_NEXT:
            {
                ::KillTimer(GetHWND(), TIMER_ID_READ_NEXT);
                //修改为“完成后增加“嘀”声提示，并且只能点击“开始”才能开始下一句”
                //ReadSpeakNext_();
                NoticePrompt();
                pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
                pREdt->SetTextColor(DUI_BLACK);
                pREdt->SetSel(m_nReadToPostion + 1, m_nReadToPostion + 1);
                TNotifyUI msg;
                msg.pSender = xFindControl(CControlUI, _T("storystartbtn"));
                msg.sType = DUI_MSGTYPE_CLICK;
                m_PaintManager.SendNotify(msg);
            }
            break;
        case TIMER_ID_READ_RECITE:
            {
                pPrg = xFindControl(CProgressUI, _T("reciteprg"));
                n = pPrg->GetValue();
                if (n < pPrg->GetMaxValue())
                {
                    n++;
                    pPrg->SetValue(n);
                }
                else
                {
                    ::KillTimer(GetHWND(), TIMER_ID_READ_RECITE);
                    SpeechRecognition_Stop();
                }
            }
            break;
        case TIMER_ID_READ_RECITETIP:
            {
                pREdt = xFindControl(CRichEditUI, _T("recitetxt"));
                if (m_nReadReciteTipeCount > 3)
                {
                    ::KillTimer(GetHWND(), TIMER_ID_READ_RECITETIP);
                    //pREdt->SetText(m_strReadReciteTextBak);
                    n = pREdt->SetSel(-1, -1);
                    pREdt->SetSel(m_nReadTipS, n);
                    pREdt->ReplaceSel(_T(""), false);
                    pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
                    pBtn->SetEnabled(true);
                    pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
                    pBtn->SetEnabled(true);
                }
                else
                {
                    m_nReadReciteTipeCount++;
                    if (m_nReadReciteTipeCount % 2)
                    {
                        n = pREdt->SetSel(-1, -1);
                        pREdt->SetSel(m_nReadTipS, n);
                        pREdt->ReplaceSel(_T(""), false);
                        //pREdt->SetText(m_strReadReciteTextBak);
                    }
                    else
                    {
                        pCmb = xFindControl(CComboUI, _T("recitemancmb"));
                        i = pCmb->GetCurSel();
                        int of = RECITE_2nd == i ? 2 : 1;
                        m_nReadTipS = pREdt->SetSel(-1, -1);
                        //SetRichEditSelColor(pREdt, RGB(0, 0, 0));
                        hlpAppedText(pREdt, m_sptReadSentences[m_nReadCurrentSentence + of].c_str());
                        //pREdt->SetText(m_strReadReciteTextBak + m_sptReadSentences[m_nReadCurrentSentence + 1].c_str());
                    }
                }
            }
            break;
        case TIMER_ID_LISTEN_NEXT:
            ::KillTimer(GetHWND(), TIMER_ID_LISTEN_NEXT);
            {
                pTab = xFindControl(CTabLayoutUI, _T("listentab"));
                i = pTab->GetCurSel();
                //TRACE(_T("tab current select:%d\n"), i);
                if (i != 2) //不是多个小题
                {
                    if (!m_bListenReview)
                    {
                        CString key = GetCollect(m_pListenSmallQuestionPtr, _T("Id"));
                        LPCTSTR* pArray = i ? g_listen_pic_opt : g_listen_opt;
                        for (i = 0; i < 4; i++)
                        {
                            pOpt = xFindControl(COptionUI, pArray[i]);
                            if (pOpt->IsSelected())
                            {
                                strTmp.Format(_T("%c"), _T('A') + i);
                                m_ListenDic.insert(CString_Pair(key, strTmp));
                                break;
                            }
                        }
                    }
                    StopSpeak();
                    pREdt = xFindControl(CRichEditUI, _T("listenansweredt"));
                    pREdt->SetVisible(false);
                }

                //for (i = 0; i < 4; i++)
                //{
                //    pOpt = xFindControl(COptionUI, g_listen_opt[i]);
                //    pOpt->SetEnabled(true);
                //    pOpt->Selected(false);
                //}
                //for (i = 0; i < 4; i++)
                //{
                //    pOpt = xFindControl(COptionUI, g_listen_pic_opt[i]);
                //    pOpt->SetEnabled(true);
                //    pOpt->Selected(false);
                //}

                pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
                pLbl->SetText(_T(""));
                pLbl = xFindControl(CLabelUI, _T("listenoktxt"));
                strTmp.Format(_T("%d"), m_nListenTrueNumber);
                pLbl->SetText(strTmp);
                m_nListenQuestionIdx++;
                m_pListenQuestionPtr->MoveNext();
                InitListenQuestion(true);
            }
            break;
        case TIMER_ID_DLG_WRITE:
            {
                static LPCTSTR s_PROG_NAME[] =
                {
                    _T("dialogfollowprgrs"),
                    _T("dialogtalkanswerprgrs"),
                    _T("dialogscenetalkprgrs")
                };
                static LPCTSTR s_LABEL_NAME[] =
                {
                    _T("dialogfollownoticetxt"),
                    _T("dialogtalkanswernoticetxt"),
                    _T("dialogscenetalknoticetxt")
                };
                CString strSrcText;
                switch (m_nDialogSelectPageId)
                {
                case DIALOG_FOLLOWME:
                case DIALOG_TALK:
                case DIALOG_SCENETALK:
                    {
                        pPrg = xFindControl(CProgressUI, s_PROG_NAME[m_nDialogSelectPageId - 1]);
                        pLbl = xFindControl(CLabelUI, s_LABEL_NAME[m_nDialogSelectPageId - 1]);
                        i = pPrg->GetValue();
                        if (i < pPrg->GetMaxValue())
                        {
                            i++;
                            pPrg->SetValue(i);
                            i = (pPrg->GetMaxValue() - i) / 10;
                            if (i != 0)
                            {
                                strTmp.Format(_T("开始录音。请在%d秒内完成朗读。"),
                                    i);
                            }
                            else
                            {
                                strTmp = _T("");
                            }
                            pLbl->SetText(strTmp);
                            return 0;
                        }
                    }
                    break;
                case DIALOG_WRITTING:
                    {
                        if (m_nDialogWrittingType == DLG_WRT_LANGDU)
                        {
                            pPrg = xFindControl(CProgressUI, _T("dialogwrttingreadprgrs"));
                            pLbl = xFindControl(CLabelUI, _T("dialogwrttingreadnoticetxt"));
                            i = pPrg->GetValue();
                            if (i < m_nDialogWrittingLastSeconds)
                            {
                                i++;
                                pPrg->SetValue(i);
                                strTmp.Format(_T("录音倒计时 %d 秒。"),
                                    (m_nDialogWrittingLastSeconds - i) / 10);
                                pLbl->SetText(strTmp);
                                return 0;
                            }
                            pPrg->SetValue(0);
                            pLbl->SetText(_T("本句录音结束。"));
                        }
                        else// if ()
                        {
                            pPrg = xFindControl(CProgressUI, _T("dialogwrittingprgrs"));
                            pLbl = xFindControl(CLabelUI, _T("dialogwrittingrcdtimetxt"));
                            i = pPrg->GetValue();
                            if (i < pPrg->GetMaxValue())
                            {
                                i++;
                                pPrg->SetValue(i);
                                strTmp.Format(_T("%d"), i);
                                pLbl->SetText(strTmp);
                                return 0;
                            }
                            pLbl = xFindControl(CLabelUI, _T("dialogwrittingnoticetxt"));
                            pLbl->SetText(_T("录音停止"));
                        }
                    }
                	break;
                default:
                    return 0;
                }
                ASSERT(pPrg && pLbl);
                pPrg->SetVisible(false);
                pLbl->SetText(_T(""));
                ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
                SpeechRecognition_Stop();
            }
            break;
        case TIMER_ID_DLG_WRITE_NEXT:
            ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT);
            switch (m_nDialogSelectPageId)
            {
            case DIALOG_FOLLOWME:
            	break;
            case DIALOG_TALK:
                if ( (m_nDialogSpokenIndex >= (m_nDialogAllCount - 1)) ||
                    m_pDialogSpokenPtr->GetadoEOF())
                {
                    DialogTalkEndOf();
                }
                else
                {
                    m_nDialogSpokenIndex++;
                    m_pDialogSpokenPtr->MoveNext();
                    DialogSpeackCurrentSentence();
                }
                break;
            case DIALOG_SCENETALK:
                {
                    if (m_nDialogSpokenIndex >= m_nDialogAllCount - 1)
                    {
                        DialogSceneTalkEndOf();
                        break;
                    }
                    m_nDialogSpokenIndex++;
                    m_pDialogSpokenPtr->MoveNext();
                    DialogSpeackCurrentSentence();
                }
                break;
            case DIALOG_WRITTING:
                {
                    if (m_nDialogWrittingType == DLG_WRT_LANGDU)
                    {
                        DialogWrittingSpeakNext_();
                    }
                    else// if ()
                    {
                        DialogWrittingStartNext_();
                    }
                }
                break;
            default:
                break;
            }
            break;
        case TIMER_ID_WORD_FOLLOW_WORD:
        case TIMER_ID_WORD_FOLLOW_EXAMPLE:
            {
                pPrg = xFindControl(CProgressUI, _T("wordfollowprg"));
                n = pPrg->GetValue();
                if (n < pPrg->GetMaxValue())
                {
                    n++;
                    pPrg->SetValue(n);
                }
                else
                {
                    ::KillTimer(GetHWND(), wParam);
                    SpeechRecognition_Stop();
                }
            }
            break;
        default:
            bHandled = FALSE;
            break;
        }
        break;
    case MM_MCINOTIFY:
        {
            //bHandled = TRUE;
            WORD wFlag = (WORD)wParam;
            //MCI_NOTIFY_SUCCESSFUL
            TRACE(_T("recv MM_MCINOTIFY, wParam:%d\n"), wFlag);
            if (MCI_NOTIFY_SUCCESSFUL != wFlag)
            {
                break;
            }
            m_mciWave.Stop();

            if (m_nSkinId == sRecordSkin)
            {
                pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstart")));
                pBtn->SetEnabled(true);
                pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstop")));
                pBtn->SetEnabled(false);

                pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordplaybtn")));
                pBtn->SetEnabled(true);
                pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordstopbtn")));
                pBtn->SetEnabled(false);
                //pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("recordsavebtn")));
                //pBtn->SetEnabled(false);  
            }
        }
        break;
    case WM_MENUCLICK:
        {
            bHandled = TRUE;
            CDuiString *strMenuName = (CDuiString*)wParam;
            BOOL bChecked = (BOOL)lParam;		 

            if ( *strMenuName == _T("readsavemenu")) 
            {
                TNotifyUI msg;
                msg.pSender = xFindControl(CControlUI, _T("readsavebtn"));
                ASSERT(msg.pSender);
                msg.sType = DUI_MSGTYPE_CLICK;
                m_PaintManager.SendNotify(msg);
            }
            else if (*strMenuName == _T("readsaveasmnu"))
            {
                if (ReadChangedSave_(2) == 0)
                {
                    break;
                }
                if (m_strReadDocumentPath.IsEmpty())
                {
                    m_nReadDocument++;
                }
            }
            else if (*strMenuName == _T("readlettermnu"))
            {
                m_bReadSpeakLetter = bChecked ? true : false;
            }
            else if (*strMenuName == _T("readwordmnu"))
            {
                m_bReadSpeakWord = bChecked ? true : false;
            }
            else if (*strMenuName == _T("readsentencemnu"))
            {
                m_bReadSpeakSentence = bChecked ? true : false;
            }
            else if (*strMenuName == _T("skin1mnu") ||
                *strMenuName == _T("skin2mnu") ||
                *strMenuName == _T("skin3mnu") ||
                *strMenuName == _T("skin4mnu"))
            {
                do 
                {
                    if (!bChecked)
                    {
                        TRACE(_T("unchecked %s\n"), *strMenuName);
                        break;
                    }
                    map<CDuiString,bool>::iterator pinfo;
                    //其它全部uncheck
                    for (pinfo = m_SkinMenuCheckInfo.begin(); pinfo != m_SkinMenuCheckInfo.end(); ++pinfo)
                    {
                        if (pinfo->first != *strMenuName)
                        {
                            pinfo->second = false;
                        }
                    }

                    SkinMap::const_iterator p = m_SkinInfo.find(*strMenuName);
                    ASSERT(p != m_SkinInfo.end());
                    SetCurrentResourceID(p->second);

                    switch (GetResourceType())
                    {
                    case UILIB_FILE:
                        break;
                    case UILIB_ZIP:
                        m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
                    	break;
                    case UILIB_RESOURCE:
                        break;
                    case UILIB_ZIPRESOURCE:
                        {
                            //切换UILIB_ZIPRESOURCE类型的皮肤
                            //WindowImplBase在OnCreate函数加载ZIPRES到m_lpResourceZIPBuffer，
                            //并调用m_PaintManager.SetResourceZip()
                            //但SetResourceZip函数只可设置一次，因此修改SetResourceZip函数，去掉if( m_pStrResourceZip == _T("membuffer") )的判断
                            HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
                            if( hResource == NULL )
                            {
                                TRACE(_T("FindResource failed:%s\n"), MyFormatMessage(GetLastError()));
                                break;
                            }
                            DWORD dwSize = 0;
                            HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
                            if( hGlobal == NULL ) 
                            {
                                TRACE(_T("LoadResource failed:%s\n"), MyFormatMessage(GetLastError()));
#if defined(WIN32) && !defined(UNDER_CE)
                                ::FreeResource(hResource);
#endif
                                break;
                            }
                            dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
                            if( dwSize == 0 )
                            {
#if defined(WIN32) && !defined(UNDER_CE)
                                ::FreeResource(hResource);
#endif
                                break;
                            }

                            ReleaseZIPBuffer();

                            m_lpResourceZIPBuffer = new BYTE[ dwSize ];
                            if (m_lpResourceZIPBuffer != NULL)
                            {
                                ::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
                            }
#if defined(WIN32) && !defined(UNDER_CE)
                            ::FreeResource(hResource);
#endif
                            m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
                        }
                        break;
                    default:
                        break;
                    }

                    m_PaintManager.ReloadSkin();

                    RegKeySetValue(_T("SkinName"), *strMenuName);
                } while (false);
            }
            delete strMenuName;
        }
        break;
    case WM_WAVEINSTATE:
        {
            if (wParam == DEVICE_STATE_UNPLUGGED)
            {
                //AfxMessageBox(_T("检测到麦克风被拔出，部分功能如评分将不能正常工作。\r\n如需正常使用，请将麦克风插入。"),
                //    MB_OK | MB_ICONINFORMATION);
            }
            else if (wParam == DEVICE_STATE_ACTIVE)
            {
                //AfxMessageBox(_T("检测到麦克风插入，全部功能恢复正常使用。"),
                //    MB_OK | MB_ICONINFORMATION);
            }
            m_WaveRecorder.OnStateChange((DWORD)wParam);
        }
        break;
    default:
        break;
    }
	return 0;
}

LPCTSTR MainFrame::GetResourceID() const
{
    //return MAKEINTRESOURCE(IDR_ZIPRES);
    return GetCurrentResourceID();
}

void MainFrame::OpenChildModalWnd(emSkinId nSkinId)
{
    //we should destroy all my controls
    MyWndContainer::const_iterator p;
    for (p = m_AllMyWnd.begin(); p != m_AllMyWnd.end(); ++p)
    {
        ::DestroyWindow(**p);
        //NOTE: we do NOT need to delete, CPaintManagerUI will do it when call AttachDialog
        //delete *p;
    }
    m_AllMyWnd.clear();

    KillAllTimer();

    ASSERT(nSkinId < _countof(g_SkinInfo));
    //TCHAR tbuf[1024];
    //::GetWindowText(this->GetHWND(), tbuf, 1024);
    //TRACE(_T("before, name:%s, font count:%d\n"), tbuf, m_PaintManager.GetCustomFontCount());
    //we don't remove all font, since we only add default font in main.xml
    //m_PaintManager.RemoveAllFonts();
    m_PaintManager.RemoveAllImages();
    m_PaintManager.RemoveAllOptionGroups();
    m_PaintManager.RemoveAllTimers();
    //we must remove all default attribute, because we'll add new default in new xml
    //maybe it's bad idea
    //m_PaintManager.RemoveAllDefaultAttributeList();

    m_nSkinId = nSkinId;
    CDialogBuilder builder;
    CString str = m_PaintManager.GetInstancePath();
    //always UILIB_ZIPRESOURCE, so type is NULL
    CControlUI* pRoot = builder.Create(g_SkinInfo[nSkinId].skin, NULL, this, &m_PaintManager);
    ASSERT(pRoot);
    m_PaintManager.AttachDialog(pRoot);
    if (g_SkinInfo[nSkinId].title)
    {
        ::SetWindowText(m_hWnd, g_SkinInfo[nSkinId].title);
    }
    SIZE newsz = m_PaintManager.GetInitSize();
    ::SetWindowPos(GetHWND(), HWND_TOP, 0, 0, newsz.cx, newsz.cy, SWP_NOMOVE);
    CenterWindow();
    //::GetWindowText(this->GetHWND(), tbuf, 1024);
    //TRACE(_T("after, name:%s, font count:%d\n"), tbuf, m_PaintManager.GetCustomFontCount());
    //m_PaintManager.NeedUpdate();
}

void MainFrame::StopWaveRecordReco()
{
    m_cpGrammar->SetDictationState(SPRS_INACTIVE);
    m_WaveRecorder.StopRecord();
    m_mciWave.Stop();
}

CString MainFrame::ReadSetup(LPCTSTR strParam)
{
    return m_DbFile.GetSetup(strParam);
}

BOOL MainFrame::SaveSetup(LPCTSTR strParam, LPCTSTR strValue)
{
    if (0 == strValue)
    {
        ASSERT(0);
        return m_DbFile.SetSetup(strParam, _T("0"));
    }
    return m_DbFile.SetSetup(strParam, strValue);
}

void MainFrame::InitVoiceRateControl(CPaintManagerUI *pPaintManager, LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed)
{
    CString strSpeaker = ReadSetup(lpstrSpeaker);
    CString strSpeed = ReadSetup(lpstrSpeed);

    CComboUI* pCombo = static_cast<CComboUI*>(pPaintManager->FindControl(_T("readercmb")));
    ASSERT(pCombo);
    pCombo->RemoveAll();
    CListLabelElementUI* pElement = new CListLabelElementUI;
    pElement->SetText(MAN_VOICE); 
    pCombo->Add(pElement);

    pElement = new CListLabelElementUI;
    pElement->SetText(WOMAN_VOICE); 
    pCombo->Add(pElement);

    pCombo->SelectItem((strSpeaker == MAN_VOICE) ? 0 : 1);

    //////////////////////////////////////////////////////////////////////////
    //语音速度
    //CMyWndUI *pMyUI = static_cast<CMyWndUI*>(m_PaintManager.FindControl(_T("listenspeed")));
    //::SendMessage(*pMyUI, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-10, 10));
    //::SendMessage(*pMyUI, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
    CSliderUI* pSlider = static_cast<CSliderUI*>(pPaintManager->FindControl(_T("listenspeed")));
    int i = _tstoi(strSpeed);
    CTextUI* pText;
    if ((i < -10) || (i > 10))
    {
        i = 0;
        strSpeed.Format(_T("%d"), i);
    }
    pSlider->SetValue(i);
    pText = static_cast<CTextUI*>(pPaintManager->FindControl(_T("listenspeedtxt")));
    pText->SetText(strSpeed);
}

void MainFrame::DefaultVoiceRateControl(CPaintManagerUI *pPaintManager)
{
    CComboUI* pCombo = static_cast<CComboUI*>(pPaintManager->FindControl(_T("readercmb")));
    pCombo->SelectItem(0);
    CSliderUI* pSlider = static_cast<CSliderUI*>(pPaintManager->FindControl(_T("listenspeed")));
    pSlider->SetValue(0);
    CTextUI* pTxt = static_cast<CTextUI*>(pPaintManager->FindControl(_T("listenspeedtxt")));
    pTxt->SetText(_T("0"));
}

void MainFrame::SaveVoiceRate(CPaintManagerUI *pPaintManager, LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed)
{
    CListLabelElementUI* pItem;
    CComboUI* pCombo = static_cast<CComboUI*>(pPaintManager->FindControl(_T("readercmb")));
    //注意听力界面和听力设置界面都用此函数，但听力界面没有朗读者控件，需判断
    if (pCombo)
    {
        pItem = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
        SaveSetup(lpstrSpeaker, pItem->GetText().GetData());
    }

    CSliderUI* pSlider = static_cast<CSliderUI*>(pPaintManager->FindControl(_T("listenspeed")));
    if (pSlider)
    {
        SaveSetup(lpstrSpeed, pSlider->GetValue());
    }
}

static LPCTSTR g_StudyTypeString[] =
{
    _T("单词"),
    _T("阅读"),
    _T("口语"),
    _T("听力"),
    _T("语法"),
};

BOOL MainFrame::StudyLog(int intStudyType, int intLogType, int intValue1, int intValue2)
{
    // 基础版本不记录成绩 [2016-1-16 Arthur]
#ifdef APP_DEMO
    return TRUE;
#endif
    intStudyType--;
    if (intStudyType >= _countof(g_StudyTypeString) ||
        intStudyType < 0)
    {
        return FALSE;
    }

    CString strSql, strTmp;
    CString strNow = CTime::GetCurrentTime().Format(_T("%Y-%m-%d"));

    strTmp = StudyGetSingleField(_T("Select top 1 Id from 成绩记录 where 日期=#%s#"),
        strNow);
    if (strTmp.IsEmpty())
    {
        StudyExecuteSql(_T("Insert into 成绩记录(日期) Values(#%s#)"),
            strNow);
    }
    
    strTmp = g_StudyTypeString[intStudyType];
    if (intLogType != LOG_TYPE_TIME)
    {
        strSql.Format(_T("Update 成绩记录 Set %s答题数=%s答题数+%d, %s准确数=%s准确数+%d, %s学习成绩=(%s准确数+%d)*100\\(%s答题数+%d) where 日期=#%s#"),
            strTmp, strTmp, intValue1, strTmp, strTmp, intValue2, strTmp, strTmp, intValue2, strTmp, intValue1, strNow);
    }
    else
    {
        strSql.Format(_T("Update 成绩记录 Set %s学习时间=%s学习时间+%d where 日期=#%s#"),
            strTmp, strTmp, intValue1, strNow);
    }

    //TRACE("run here:%s,%d\n%S\n", __FUNCTION__, __LINE__, (LPCTSTR)strSql);
    return StudyExecuteSql(_T("%s"), strSql);
}

int MainFrame::OpenModalWnd(WindowImplBase* pWnd, LPCTSTR lpsTitle/*=NULL*/)
{
    int nRet = IDCANCEL;
    do 
    {
        if (NULL == pWnd)
        {
            break;
        }
        HWND hWnd = pWnd->Create(GetHWND(), lpsTitle, UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW);
        if (NULL == hWnd)
        {
            break;
        }
        m_hSubWnd = pWnd->GetHWND();
        pWnd->CenterWindow();
        nRet = pWnd->ShowModal();
        m_hSubWnd = NULL;
    } while (FALSE);

    return nRet;
}

CString MainFrame::GetSingleField(LPCTSTR lpStrSql, ...)
{
    if (!lpStrSql)
    {
        return _T("");
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, lpStrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), lpStrSql, va);
    va_end(va);

    return m_DbFile.GetSingleField(OutText);
}

_RecordsetPtr MainFrame::GetTablesBySql(LPCTSTR pstrSql, ...)
{
    if (!pstrSql)
    {
        return NULL;
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, pstrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), pstrSql, va);
    va_end(va);

    return m_DbFile.GetTablesBySql(OutText);
}

BOOL MainFrame::ExecuteSql(LPCTSTR pstrSql, ...)
{
    if (!pstrSql)
    {
        return FALSE;
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, pstrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), pstrSql, va);
    va_end(va);

    BOOL bRet;
    __try
    {
        bRet = m_DbFile.ExecuteSql(OutText);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        bRet = FALSE;
    }

    return bRet;
}

CString MainFrame::StudyGetSingleField(LPCTSTR lpStrSql, ...)
{
    if (!lpStrSql)
    {
        return _T("");
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, lpStrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), lpStrSql, va);
    va_end(va);

    return m_DbFile.GetSingleField(OutText, true);
}

_RecordsetPtr MainFrame::StudyGetTablesBySql(LPCTSTR pstrSql, ...)
{
    if (!pstrSql)
    {
        return NULL;
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, pstrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), pstrSql, va);
    va_end(va);

    return m_DbFile.GetTablesBySql(OutText, true);
}

BOOL MainFrame::StudyExecuteSql(LPCTSTR pstrSql, ...)
{
    if (!pstrSql)
    {
        return FALSE;
    }
    va_list va;
    TCHAR OutText[4096];

    va_start(va, pstrSql);
    //
    // Truncation is acceptable.
    //
    StringCbVPrintf(OutText, sizeof(OutText), pstrSql, va);
    va_end(va);

    BOOL bRet;
    __try
    {
        bRet = m_DbFile.ExecuteSql(OutText, true);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        bRet = FALSE;
    }

    return bRet;
}

//////////////////////////////////////////////////////////////////////////
//练听力
void MainFrame::OnListenPrepare()
{
    CString strTmp;
    CButtonUI* pBtn;
    CLabelUI* pLbl;
    COptionUI* pOpt;

    //m_bListenReview = false;
    //m_nListenTrueNumber = 0;
    if (m_bListenReview)
    {
        pBtn = xFindControl(CButtonUI, _T("listenstartbtn"));
        pBtn->SetEnabled(false);
    }
    pBtn = xFindControl(CButtonUI, _T("listenreviewbtn"));
    pBtn->SetVisible(m_bListenReview ? true : false);
    pBtn = xFindControl(CButtonUI, _T("listenviewtextbtn"));
    pBtn->SetVisible(m_bListenReview ? true : false);
    pBtn = xFindControl(CButtonUI, _T("listenextbtn"));
    pBtn->SetVisible(m_bListenReview ? true : false);
    pBtn = xFindControl(CButtonUI, _T("listenagainbtn"));
    pBtn->SetVisible(false);

    //m_tmListenLogFrom = CTime::GetCurrentTime();
    if (InitListenStatus())
    {
        InitListenQuestion(m_bListenReview);
        if (m_bListenReview)
        {
            pLbl = xFindControl(CLabelUI, _T("listenoktxt"));
            strTmp.Format(_T("%d"), m_nListenTrueNumber);
            pLbl->SetText(strTmp);
        }
        CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("listenoptionvlay"));
        pVLay->SetEnabled(false);

        pVLay = xFindControl(CVerticalLayoutUI, _T("listenvlaysmallquest"));
        pVLay->SetEnabled(false);

        int i;
        for (i = 0; i < 4; i++)
        {
            pOpt = xFindControl(COptionUI, g_listen_pic_opt[i]);
            pOpt->SetEnabled(false);
        }
    }
}

bool MainFrame::InitListenStatus()
{
    bool bRet = false;
    int i;
    CString strTmp;
    CTextUI* pText;

    CString strGrade = ReadSetup(_T("Listen_Grade"));
    CString strQNum = ReadSetup(_T("Listen_QuestionNumber"));
    CString strQType = ReadSetup(_T("Listen_QuestionType"));

    do 
    {
        SetVoiceRateBySetting(_T("Listen_Speaker"), _T("Listen_Speed"));

        InitListenCfgControl();

        CString strNum[4] = {_T("0"),_T("0"),_T("0"),_T("0")};
        SplitResultVector strQumResult;
        TString tstr = strQNum;
        int n = SplitString(tstr, _T(","), strQumResult);
        for (i = 0; i < (int)strQumResult.size(); i++)
        {
            if (i > 3)
            {
                break;
            }
            strNum[i] = strQumResult[i].c_str();
        }

        srand((unsigned)time(NULL));
        int qsum = 0;
        CString strSql;
        if (_T("1") == strQType)    //智能出题
        {
            for (i = 0; i < 4; i++)
            {
                qsum += _tstoi(strNum[i]);
            }
            if (0 == qsum)
            {
                qsum = 10;
            }
            //Select top 10 * from 听力习题 where 年级=", Conversions.ToString(Me.intGrade), " and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+", Conversions.ToString(CSng((VBMath.Rnd * 1000!))), "))" 
            strSql.Format(_T("Select top %d * from 听力习题 where 年级=%s and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+%d))"),
                qsum, strGrade, SQLRand());
        }
        else if (_T("2") == strQType)   //单项训练
        {
            int t = 0;
            if (_T("0") != strNum[0])
            {
                qsum = _tstoi(strNum[0]);
                t = 1;
            }
            else if (_T("0") != strNum[1])
            {
                qsum = _tstoi(strNum[1]);
                t = 2;
            }
            else if (_T("0") != strNum[2])
            {
                qsum = _tstoi(strNum[2]);
                t = 3;
            }
            else if (_T("0") != strNum[3])
            {
                qsum = _tstoi(strNum[3]);
                t = 4;
            }
            else
            {
                qsum = 10;
                t = 0;
            }
            //"Select top ", strArray(0), " * from 听力习题 where 年级=", Conversions.ToString(Me.intGrade), " and 类型=1 and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+", Conversions.ToString(CSng((VBMath.Rnd * 1000!))), "))"
            if (t)
            {
                strSql.Format(_T("select top %d * from 听力习题 where 年级=%s and 类型=%d and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+%d))"),
                    qsum, strGrade, t, SQLRand());
            }
            else
            {
                strSql.Format(_T("select top %d * from 听力习题 where 年级=%s and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+%d))"),
                    qsum, strGrade, SQLRand());
            }
        }
        else if (_T("3") == strQType)   //全真试题
        {
            if ((_T("0") == strNum[0]) &&
                (_T("0") == strNum[1]) &&
                (_T("0") == strNum[2]) &&
                (_T("0") == strNum[3]) )
            {
                strNum[0] = strNum[1] = strNum[2] = strNum[3] = _T("2");
            }
            strSql.Empty();
            for (i = 0; i < 4; i++)
            {
                qsum = _tstoi(strNum[i]);
                if (qsum > 0)
                {
                    if (strSql.IsEmpty())
                    {
                        strSql.Format(_T("select top %d * from 听力习题 where 年级=%s and 类型=%d and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+%d))"),
                            qsum, strGrade, i+1, SQLRand());
                    }
                    else
                    {
                        strTmp.Format(_T(" union (select top %d * from 听力习题 where 年级=%s and 类型=%d Order by Rnd(-(Id+%d)))"),
                            qsum, strGrade, i+1, SQLRand());
                        strSql += strTmp;
                    }
                }
            }
        }
        else if (_T("4") == strQType)
        {
            qsum = _tstoi(strNum[0]);
            if (qsum == 0)
            {
                qsum = 10;
            }
            strSql.Format(_T("Select top %d * from (Select 听力习题.Id As Id,年级,教材,课文,原文,类型 from 听力习题,(SELECT Id from 听力习题 where Id in (select 习题 from 听力错题) union SELECT  distinct(习题) as Id from 听力小题 where Id in (select 习题 from 听力错题)) as 错题列表 where 年级=%s and 听力习题.Id=错题列表.Id) Order by Rnd(-(Id+%d))"),
                qsum, strGrade, SQLRand());
        }
        else
        {
            ASSERT(0);
            break;
        }
        //
        if (m_bListenReview)
        {
            strSql.Format(_T("Select * from 听力习题 where Id in (%s)"),
                m_strListenReviewQuestion);
        }

        CDbFile::CloseRecord(m_pListenSmallQuestionPtr);
        if (m_bListenReview)
        {
            ASSERT(m_pListenQuestionPtr);
            m_pListenQuestionPtr->MoveFirst();
        }
        else
        {
            CDbFile::CloseRecord(m_pListenQuestionPtr);
            TRACE(_T("sql:%s\n"), strSql);
            m_pListenQuestionPtr = GetTablesBySql(_T("%s"), strSql);
        }
        if (m_pListenQuestionPtr != NULL)
        {
            m_nListenQuestionIdx = 0;
            m_nListenSmallQuestionIdx = 0;
            m_pListenSmallQuestionPtr = GetTablesBySql(_T("Select * from 听力小题 where 习题=%s"),
                GetCollect(m_pListenQuestionPtr, _T("Id")));
        }
        else
        {
            MyAfxMessageBox(_T("本年级暂时没有可供练习的题目，请选择其他年级。"));
            ListenNoQuestion();
            //OnExit(msg);
            break;
        }


        strTmp.Empty();
        m_pListenQuestionPtr->MoveFirst();
        while(!m_pListenQuestionPtr->GetadoEOF())
        {
            strTmp += GetCollect(m_pListenQuestionPtr, _T("Id"));
            strTmp += _T(",");
            m_pListenQuestionPtr->MoveNext();
        }
        m_pListenQuestionPtr->MoveFirst();
        strTmp.TrimRight(_T(','));
        SplitResultVector sptTmp;
        SplitString(TString(strTmp), _T(","), sptTmp);

        m_nListenAllSmallQuestion = GetCollectCount(m_pListenSmallQuestionPtr);
        m_nListenQuestionNumber = m_nListenAllSmallQuestion;
        m_pListenSmallQuestionPtr->MoveFirst();

        int allsmall = _tstoi(GetSingleField(_T("Select Count(Id) from 听力小题 where 习题 in (%s)"),
                strTmp));

        CLabelUI* pLbl = xFindControl(CLabelUI, _T("listenallcounttxt"));
        m_nListenAllQuestion = sptTmp.size();
        strTmp.Format(_T("%d"), m_nListenAllQuestion);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("listenallsmalltxt"));
        strTmp.Format(_T("%d"), allsmall);
        pLbl->SetText(strTmp);

        pText = xFindControl(CTextUI, _T("listennavi"));
        strTmp.Format(_T("%s听力练习"),
            GetSingleField(_T("Select 名称 from 年级 where Id=%s"), strGrade));
        pText->SetText(strTmp);

        CRichEditUI* pEdit = xFindControl(CRichEditUI, _T("listenansweredt"));
        pEdit->SetVisible(false);

        bRet = true;
    } while (FALSE);

    return bRet;
}

void MainFrame::InitListenQuestion(bool bPlayVoice)
{
    CTabLayoutUI* pTab;
    CLabelUI* pLbl;
    CString strTmp;
    COptionUI* pOpt;
    CButtonUI* pBtn;
    CRichEditUI* pREdt;
    int i;

    if (m_nListenQuestionIdx < m_nListenAllQuestion)
    {
        if (GetCollect(m_pListenSmallQuestionPtr, _T("习题")) != GetCollect(m_pListenQuestionPtr, _T("Id")))
        {
            CDbFile::CloseRecord(m_pListenSmallQuestionPtr);
            m_pListenSmallQuestionPtr = GetTablesBySql(_T("Select * from 听力小题 where 习题=%s"),
                GetCollect(m_pListenQuestionPtr, _T("Id")));
            m_nListenSmallQuestionIdx = 0;
            m_nListenAllSmallQuestion = GetCollectCount(m_pListenSmallQuestionPtr);
            m_nListenQuestionNumber += m_nListenAllSmallQuestion;
            m_pListenSmallQuestionPtr->MoveFirst();
        }
        strTmp.Format(_T("%d"), m_nListenQuestionIdx + 1);
        pLbl = xFindControl(CLabelUI, _T("listenallcurrenttxt"));
        pLbl->SetText(strTmp);

        CString strResult, strStuanswer;
        if (m_bListenReview)
        {
            strStuanswer = GetCollect(m_pListenSmallQuestionPtr, _T("答案"));
            strStuanswer.Trim(_T("\r\n"));

            strTmp = GetCollect(m_pListenSmallQuestionPtr, _T("Id"));
            MyDictionary::const_iterator p;
//#ifdef _DEBUG
//            {
//                TRACE(_T("find %s in dic:\n"), strTmp);
//                for (p = m_ListenDic.begin(); p != m_ListenDic.end(); ++p)
//                {
//                    TRACE(_T("key:%s, value:%s\n"), p->first, p->second);
//                }
//            }
//#endif
            p = m_ListenDic.find(strTmp);
            if (p != m_ListenDic.end())
            {
                strResult = p->second;
            }
        }

        pTab = xFindControl(CTabLayoutUI, _T("listentab"));
        strTmp = GetCollect(m_pListenQuestionPtr, _T("类型"));
        //单句,对话,短文,图片
        if (strTmp == _T("4"))
        {
            //图片
            pTab->SelectItem(1);

            CString strPath;
            CControlUI* pCtrl;
            CFileFind finder;

            for (i = 1; i <= 4; i++)
            {
                pOpt = xFindControl(COptionUI, g_listen_pic_opt[i-1]);
                strTmp.Format(_T("listenpic%d"), i);
                pCtrl = xFindControl(CControlUI, strTmp);
                strTmp.Format(_T("图%d"), i);
                strPath.Format(_T("%sData\\Picture\\%s"),
                    CPaintManagerUI::GetInstancePath().GetData(),
                    GetCollect(m_pListenSmallQuestionPtr, strTmp));
                if (finder.FindFile(strPath))
                {
                    pCtrl->SetBkImage(strPath);
                    pOpt->SetVisible(true);
                    strTmp.Format(_T("选项%d"), i);
                    pOpt->SetText(GetCollect(m_pListenSmallQuestionPtr, strTmp));
                    pOpt->Selected(false);
                    pOpt->SetEnabled(true);
                    pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
                }
                else
                {
                    pCtrl->SetBkImage(_T(""));
                    pOpt->SetVisible(false);
                }
                finder.Close();
                if (m_bListenReview)
                {
                    pOpt->SetEnabled(false);
                    strTmp.Format(_T("%c"), _T('A') + i - 1);
                    if (strStuanswer == strTmp)
                    {
                        pOpt->SetEnabled(true);
                        pOpt->SetTextColor(DUI_RGB(255,0,0));
                    }
                    if (strResult == strTmp)
                    {
                        pOpt->Selected(true);
                    }
                }
            }
        }
        else
        {
            pTab->SelectItem(0);

            pLbl = xFindControl(CLabelUI, _T("listencurrentquestxt"));
            pLbl->SetText(GetCollect(m_pListenSmallQuestionPtr, _T("问题")));

            for (i = 1; i <= 4; i++)
            {
                pOpt = xFindControl(COptionUI, g_listen_opt[i-1]);
                strTmp.Format(_T("选项%d"), i);
                strTmp = GetCollect(m_pListenSmallQuestionPtr, strTmp);
                if (strTmp.IsEmpty())
                {
                    pOpt->SetVisible(false);
                }
                else
                {
                    pOpt->SetVisible(true);
                    pOpt->SetText(strTmp);
                    pOpt->Selected(false);
                    pOpt->SetEnabled(true);
                    //pOpt->SetTextColor(DEFAULT_TEXT_COLOR);
                    pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
                }
                if (m_bListenReview)
                {
                    pOpt->SetEnabled(false);
                    strTmp.Format(_T("%c"), _T('A') + i - 1);
                    if (strStuanswer == strTmp)
                    {
                        pOpt->SetEnabled(true);
                        pOpt->SetTextColor(DUI_RGB(255,0,0));
                    }
                    if (strResult == strTmp)
                    {
                        pOpt->Selected(true);
                    }
                }
            }
        }

        if (m_bListenReview)
        {
            strResult.MakeUpper();
            if (strStuanswer == strResult)
            {
                strTmp = _T("回答正确。");
            }
            else
            {
                strTmp.Format(_T("回答错误。正确答案：%s，您的答案：%s"),
                    strStuanswer, strResult);
            }
            pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
            pLbl->SetText(strTmp);

            pREdt = xFindControl(CRichEditUI, _T("listenansweredt"));
            pREdt->SetVisible(false);
        }
        ListenNextQuestion(/*strResult, strStuanswer*/);

        StopSpeak();
        if (bPlayVoice)
        {
            ListenSpeakVoice();
        }
    }
    else
    {
        if (m_bListenReview)
        {
            pBtn = xFindControl(CButtonUI, _T("listenagainbtn"));
            pBtn->SetVisible(true);
            pBtn = xFindControl(CButtonUI, _T("listenreviewbtn"));
            pBtn->SetEnabled(false);
            pBtn = xFindControl(CButtonUI, _T("listenviewtextbtn"));
            pBtn->SetEnabled(false);
            pBtn = xFindControl(CButtonUI, _T("listenextbtn"));
            pBtn->SetEnabled(false);
            pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
            pLbl->SetText(_T(""));
            pREdt = xFindControl(CRichEditUI, _T("listenansweredt"));
            pREdt->SetVisible(false);
            pTab = xFindControl(CTabLayoutUI, _T("listentab"));
            pTab->SetVisible(false);
            StopSpeak();
            MyAfxMessageBox(_T("复习试卷完毕。"));
        }
        else
        {
            CListenResult* pListenresult = new CListenResult(this);
            pListenresult->m_nNumber = m_nListenQuestionNumber;
            pListenresult->m_nTrueNumber = m_nListenTrueNumber;
            pListenresult->m_strQuestion = m_strListenQuestion;
            i = OpenModalWnd(pListenresult, g_SkinInfo[sListenResultSkin].title);
            //返回IDCANCEL表明复习
            if (IDCANCEL == i)
            {
                m_bListenReview = true;
                //不可如此，待返回时pListenresult已释放
                //m_strListenReviewQuestion = pListenresult->m_strReviewQuestion;
                
                OpenChildModalWnd(sListenSkin);
            }
            //再次练习
            else if (IDRETRY == i)
            {
                OpenChildModalWnd(sListenSelectSkin);
            }
            else
            {
                ListenNoQuestion();
            }
        }
    }
}

void MainFrame::ListenNextQuestion(/*const CString& strResult, const CString& strStuanswer*/)
{
    if (m_nListenAllSmallQuestion <= 1)
    {
        return;
    }
    CString strResult;
    CString strStuanswer;
    CString strTmp, str = _T(""), str2 = _T("");
    COptionUI* pOpt;
    CLabelUI* pLbl;
    int i, idx;
    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("listenvlaysmallquest"));
    CTabLayoutUI* pTab = xFindControl(CTabLayoutUI, _T("listentab"));
    LPCTSTR strLblAttr = _T("font=\"1\" height=\"30\" disabledtextcolor=\"0xffffffff\" textcolor=\"0xffffffff\"");
    LPCTSTR strOptAttr = _T("font=\"1\" height=\"28\" textpadding=\"28,0,0,0\" align=\"left\" normalimage=\"file='Image/radio2.png' dest='0,0,28,28'\" selectedimage=\"file='Image/radio1.png' dest='0,0,28,28'\" disabledtextcolor=\"0xffffffff\" textcolor=\"0xffffffff\"");
    
    pTab->SelectItem(2);
    pVLay->RemoveAll();

    m_ListenOptionVector.clear();

    strTmp.Format(_T("本大题共 %d 个小题："),
        m_nListenAllSmallQuestion);
    pLbl = xFindControl(CLabelUI, _T("listensmallcurrenttxt"));
    pLbl->SetText(strTmp);

    idx = 0;
    while(!m_pListenSmallQuestionPtr->GetadoEOF())
    {
        pLbl = new CLabelUI;
        pLbl->ApplyAttributeList(strLblAttr);
        strTmp.Format(_T("%d.%s"), idx+1,
            GetCollect(m_pListenSmallQuestionPtr, _T("问题")));
        pLbl->SetText(strTmp);
        pVLay->Add(pLbl);
        
        if (m_bListenReview)
        {
            strStuanswer = GetCollect(m_pListenSmallQuestionPtr, _T("答案"));
            strStuanswer.Trim(_T("\r\n"));
            strStuanswer.Trim();
            strStuanswer.MakeUpper();
            MyDictionary::const_iterator p = m_ListenDic.find(GetCollect(m_pListenSmallQuestionPtr, _T("Id")));
            if (p != m_ListenDic.end())
            {
                strResult = p->second;
            }
        }

        str += strStuanswer + _T(" ");
        str2 += strResult + _T(" ");

        MyOptions opts;
        for (i = 1; i <= 4; i++)
        {
            strTmp.Format(_T("选项%d"), i);
            strTmp = GetCollect(m_pListenSmallQuestionPtr, strTmp);
            strTmp.Trim();
            if (strTmp.IsEmpty())
            {
                continue;
            }
            pOpt = new COptionUI;
            pOpt->ApplyAttributeList(strOptAttr);
            pOpt->SetText(strTmp);
            strTmp.Format(_T("listensmallgroup%d"), idx);
            pOpt->SetAttribute(_T("group"), strTmp);
            //pOpt->SetTextColor(DUI_BLACK);
            pOpt->Selected(false);
            pOpt->SetEnabled(true);
            if (m_bListenReview)
            {
                pOpt->SetEnabled(false);
                strTmp.Format(_T("%c"), _T('A') + i - 1);
                if (strStuanswer == strTmp)
                {
                    pOpt->SetEnabled(true);
                    pOpt->SetTextColor(DUI_RGB(255,0,0));
                }
                if (strResult == strTmp)
                {
                    pOpt->Selected(true);
                }
            }
            pOpt->OnNotify += MakeDelegate(&MainFrame::OnListenSmallNotify);
            pOpt->SetTag((UINT_PTR)this);
            //Id | A
            strTmp.Format(_T("%s|%c"), GetCollect(m_pListenSmallQuestionPtr, _T("Id")), _T('A') + i - 1);
            pOpt->SetUserData(strTmp);
            pVLay->Add(pOpt);
            opts.push_back(pOpt);
        }
        m_ListenOptionVector.push_back(opts);
        m_pListenSmallQuestionPtr->MoveNext();
        idx++;
    }
    SIZE szPos;
    szPos.cx = szPos.cy = 0;
    pVLay->SetScrollPos(szPos);
    m_pListenSmallQuestionPtr->MoveFirst();

    if (m_bListenReview)
    {
        str.MakeUpper();
        str2.MakeUpper();
        if (str == str2)
        {
            strTmp = _T("回答正确。");
        }
        else
        {
            strTmp.Format(_T("回答错误。正确答案：%s，您的答案：%s"),
                str, str2);
        }
        pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
        pLbl->SetText(strTmp);
    }
}

void MainFrame::ListenSpeakVoice()
{
    CString strTmp;
    CString strText = GetCollect(m_pListenQuestionPtr, _T("原文"));
    if (GetCollect(m_pListenQuestionPtr, _T("类型")) == _T("2"))
    {
        //TRACE(_T("原文：%s\n"), strText);
        strText.Remove(_T('\n'));
        strText.Replace(_T("\r"), _T("^"));
        //TRACE(_T("原文after：%s\n"), strText);
        SplitResultVector sptText;
        SplitString(TString(strText), _T("^"), sptText);
        //TRACE(_T("number:%d\n"), sptText.size());
        SplitResultVector::const_iterator p;
        for (p = sptText.begin(); p != sptText.end(); ++p)
        {
            strTmp = p->c_str();
            //TRACE(_T("speak:%s\n"), strTmp);
            if ( (strTmp.Find(_T("Man:")) == 0) ||
                (strTmp.Find(_T("M:")) == 0) )
            {
                SetVoiceRateByValue(MAN_VOICE);
                
                strTmp.TrimLeft(_T("Man:"));
                strTmp.TrimLeft(_T("M:"));

                Speak(strTmp);
            }
            else if ( (strTmp.Find(_T("Woman:")) == 0) ||
                (strTmp.Find(_T("W:")) == 0) )
            {
                SetVoiceRateByValue(WOMAN_VOICE);

                strTmp.TrimLeft(_T("Woman:"));
                strTmp.TrimLeft(_T("W:"));

                Speak(strTmp);
            }
            else if (strTmp.Find(_T("Question:")) == 0)
            {
                SetVoiceRateBySetting(_T("Listen_Speaker"));
                Speak(strTmp);
            }
            else
            {
                if (p > sptText.begin())
                {
                    SplitResultVector::const_iterator p2 = p - 1;
                    strTmp = p2->c_str();
                    if ( (strTmp.Find(_T("Man:")) == 0) ||
                        (strTmp.Find(_T("M:")) == 0) )
                    {
                        SetVoiceRateByValue(MAN_VOICE);

                        strTmp = p->c_str();
                        strTmp.TrimLeft(_T("Man:"));
                        strTmp.TrimLeft(_T("M:"));
                    }
                    else if ( (strTmp.Find(_T("Woman:")) == 0) ||
                        (strTmp.Find(_T("W:")) == 0) )
                    {
                        SetVoiceRateByValue(WOMAN_VOICE);

                        strTmp = p->c_str();
                        strTmp.TrimLeft(_T("Woman:"));
                        strTmp.TrimLeft(_T("W:"));
                    }
                    else
                    {
                        strTmp = p->c_str();
                        SetVoiceRateBySetting(_T("Listen_Speaker"));
                    }
                    Speak(strTmp);
                }
                else
                {
                    SetVoiceRateBySetting(_T("Listen_Speaker"));
                    Speak(p->c_str());
                }
            }
        }
    }
    else
    {
        SetVoiceRateBySetting(_T("Listen_Speaker"));
        Speak(strText);
    }
}

bool MainFrame::OnListenSmallNotify(void* event)
{
    TNotifyUI* pMsg = static_cast<TNotifyUI*>(event);
    if (pMsg->sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        COptionUI* pOpt = static_cast<COptionUI*>(pMsg->pSender);
        MainFrame* pFrame = (MainFrame*)pOpt->GetTag();
        ASSERT(pFrame);
        if (!pFrame->m_bListenReview)
        {
            if (pOpt->IsSelected())
            {
                //遍历，看是否将小题都选择了
                MyOptionVector::const_iterator p;
                int nChk = 0;
                for (p = pFrame->m_ListenOptionVector.begin(); p != pFrame->m_ListenOptionVector.end(); ++p)
                {
                    MyOptions::const_iterator p2;
                    for (p2 = p->begin(); p2 != p->end(); ++p2)
                    {
                        if ((*p2)->IsSelected())
                        {
                            nChk++;
                            break;
                        }
                    }
                }
                if (nChk >= (int)pFrame->m_ListenOptionVector.size())
                {
                    pFrame->ListenGetSmallText();
                }
            }
        }
    }

    return true;
}

int MainFrame::IsListenOpt(const TNotifyUI& msg)
{
    return IsOption(g_listen_opt, _countof(g_listen_opt), msg);
}

int MainFrame::IsListenPicOpt(const TNotifyUI& msg)
{
    return IsOption(g_listen_pic_opt, _countof(g_listen_pic_opt), msg);
}

void MainFrame::ListenTheOptionClicked_(int nIndx, bool bIsPic)
{
    if (m_bListenReview)
    {
        return;
    }

    ASSERT(nIndx >= 1 && nIndx <= 4);
    int i, n = bIsPic ? _countof(g_listen_pic_opt) : _countof(g_listen_opt);
    COptionUI* pOpt;
    LPCTSTR* pArray = bIsPic ? g_listen_pic_opt : g_listen_opt;

    for (i = 0; i < n; i++)
    {
        pOpt = xFindControl(COptionUI, pArray[i]);
        pOpt->SetEnabled(false);
    }
    //pOpt = xFindControl(COptionUI, pArray[nIndx-1]);
    //pOpt->SetEnabled(true);
    ListenGetText(nIndx, bIsPic);
}

void MainFrame::ListenOptionClicked(int nIndx)
{
    ListenTheOptionClicked_(nIndx, false);
}

void MainFrame::ListenPicOptionClicked(int nIndx)
{
    ListenTheOptionClicked_(nIndx, true);
}

void MainFrame::ListenGetText(int nIndx, bool bIsPic)
{
    static LPCTSTR listen_answer[] =
    {
        _T("A"),
        _T("B"),
        _T("C"),
        _T("D")
    };
    CString strAnswer = listen_answer[nIndx-1];

    CString strText, strWave;
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
    CString str2 = GetCollect(m_pListenSmallQuestionPtr, _T("答案"));
    str2.Trim(_T("\r\n"));
    str2.MakeUpper();

    if (str2.CompareNoCase(strAnswer) == 0)
    {
        strText = _T("回答正确。");
        m_nListenTrueNumber++;
        strWave.Format(_T("%sSystem\\Sound\\examRight.wav"), CPaintManagerUI::GetInstancePath().GetData());
        m_strListenQuestion += GetCollect(m_pListenQuestionPtr, _T("Id"));
        m_strListenQuestion += _T(",1;");

        COptionUI* pOpt;
        LPCTSTR* pArray = bIsPic ? g_listen_pic_opt : g_listen_opt;
        pOpt = xFindControl(COptionUI, pArray[nIndx-1]);
        //disabled状态
        //pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->SetDisabledTextColor(DUI_RGB(255,0,0));
    }
    else
    {
        strText.Format(_T("回答错误。答案是：%s"),
            str2);
        strWave.Format(_T("%sSystem\\Sound\\examWrong.wav"), CPaintManagerUI::GetInstancePath().GetData());
        m_strListenQuestion += GetCollect(m_pListenQuestionPtr, _T("Id"));
        m_strListenQuestion += _T(",0;");
    }

    pLbl->SetText(strText);
    m_mciWave.PlayWait(GetHWND(), strWave);

    ::SetTimer(GetHWND(), TIMER_ID_LISTEN_NEXT, TIMER_INTERVAL_NEXT, NULL);
}

void MainFrame::ListenGetSmallText()
{
    MyOptionVector::const_iterator p;
    MyOptions::const_iterator p2;
    CString str7 = _T("K"), key, str = _T(""), str3 = _T("");
    CString strTmp;
    SplitResultVector sptKetAnsw;
    CRichEditUI* pREdt;

    StopSpeak();

    for (p = m_ListenOptionVector.begin(); p != m_ListenOptionVector.end(); ++p)
    {
        for (p2 = p->begin(); p2 != p->end(); ++p2)
        {
            (*p2)->SetEnabled(false);
            if ((*p2)->IsSelected())
            {
                (*p2)->SetEnabled(true);

                strTmp = (*p2)->GetUserData();
                SplitString(TString(strTmp), _T("|"), sptKetAnsw);
                ASSERT(sptKetAnsw.size() == 2);
                key = sptKetAnsw[0].c_str();
                str3 += key;
                str3 += _T("|");

                str7 = sptKetAnsw[1].c_str();
                str += str7;
                str += _T("|");
                m_ListenDic.insert(CString_Pair(key, str7));
            }
        }
    }

    pREdt = xFindControl(CRichEditUI, _T("listenansweredt"));
    pREdt->SetVisible(false);

    SplitResultVector sptArray2, sptArray;
    SplitString(TString(str3), _T("|"), sptArray2);
    SplitString(TString(str), _T("|"), sptArray);
    CString str11 = GetCollect(m_pListenQuestionPtr, _T("原文"));
    CString str6 = _T("");

    ASSERT(sptArray.size() == sptArray2.size());
    int i, n = (int)sptArray2.size();
    for (i = 0; i < n; i++)
    {
        if (sptArray2[i].empty())
        {
            continue;
        }
        _RecordsetPtr pRecdPtr = GetTablesBySql(_T("SELECT Id, 习题, 问题, 选项1, 选项2, 选项3, 选项4, 答案 FROM (听力小题) WHERE (Id = %s)"),
            sptArray2[i].c_str());
        strTmp = GetCollect(pRecdPtr, _T("答案"));
        strTmp.Trim(_T("\r\n"));
        strTmp.Trim();
        strTmp.MakeUpper();
        str6 += strTmp;
        str6 += _T("|");

        if (strTmp == sptArray[i].c_str())
        {
            m_nListenTrueNumber++;
            m_strListenQuestion += GetCollect(pRecdPtr, _T("Id"));
            m_strListenQuestion += _T(",1;");
        }
        else
        {
            m_strListenQuestion += GetCollect(pRecdPtr, _T("Id"));
            m_strListenQuestion += _T(",0;");
        }
    }

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("listenanswerresulttxt"));
    str.Trim(_T('|'));
    str6.Trim(_T('|'));
    if (str.CompareNoCase(str6) == 0)
    {
        pLbl->SetText(_T("回答正确。"));
        strTmp.Format(_T("%sSystem\\Sound\\examRight.wav"), CPaintManagerUI::GetInstancePath().GetData());
    }
    else
    {
        str6.Replace(_T('|'), _T(' '));
        strTmp.Format(_T("回答有误。正确答案：%s"),
            str6);
        pLbl->SetText(strTmp);
        strTmp.Format(_T("%sSystem\\Sound\\examWrong.wav"), CPaintManagerUI::GetInstancePath().GetData());
    }
    m_mciWave.PlayWait(GetHWND(), strTmp);
    ::SetTimer(GetHWND(), TIMER_ID_LISTEN_NEXT, TIMER_INTERVAL_NEXT, NULL);
}

void MainFrame::ListenSubmitClick()
{
    CComboUI* pCombo;
    CListLabelElementUI* pItem;
    COptionUI* pOpt;
    CCheckBoxUI* pCheckBox;
    CEditUI* pEdit;
    CButtonUI* pButton;
    CLabelUI* pLbl;

    int i;
    CString strTmp;

    SaveVoiceRate(&m_PaintManager, _T("Listen_Speaker"), _T("Listen_Speed"));

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("gradecmb")));
    pItem = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
    SaveSetup(_T("Listen_Grade"), pItem->GetUserData().GetData());

    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("listentypezhineng")));
    if (pOpt->IsSelected())
    {
        pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb")));
        pItem = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
        i = _tstoi(pItem->GetText().GetData());
        strTmp.Format(_T("%d,%d,%d,%d"), i / 4, i / 4, i / 4, i - i / 4 * 3);
        SaveSetup(_T("Listen_QuestionNumber"), strTmp);
        SaveSetup(_T("Listen_QuestionType"), _T("1"));
    }
    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("listentypedanxiang")));
    if (pOpt->IsSelected())
    {
        SaveSetup(_T("Listen_QuestionType"), _T("2"));
        CString strNum[4] = {_T("0"), _T("0"), _T("0"), _T("0")};
        pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("listendanxiangcmb")));
        i = pCombo->GetCurSel();
        ASSERT(i < 4);
        pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb")));
        if (pCombo->GetCurSel() >= 0)
        {
            pItem = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
            strNum[i] = pItem->GetText();
        }
        strTmp.Format(_T("%s,%s,%s,%s"), strNum[0], strNum[1], strNum[2], strNum[3]);
        SaveSetup(_T("Listen_QuestionNumber"), strTmp);
    }
    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("listentypequanzhen")));
    if (pOpt->IsSelected())
    {
        SaveSetup(_T("Listen_QuestionType"), _T("3"));

        strTmp.Empty();
        static CString sQuanzhenChk[]=
        {
            _T("listendanjuchk"),
            _T("listenduihuachk"),
            _T("listenduanwenchk"),
            _T("listentupianchk"),
        };
        static CString sQuanzhenEdt[]=
        {
            _T("listendanjuedt"),
            _T("listenduihuaedt"),
            _T("listenduanwenedt"),
            _T("listentupianedt"),
        };
        CString strn;
        for (i = 0; i < 4; i++)
        {
            pCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sQuanzhenChk[i]));
            if (pCheckBox->GetCheck())
            {
                pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(sQuanzhenEdt[i]));
                strn = pEdit->GetText().GetData();
                if (!IsNumeric(strn))
                {
                    MyAfxMessageBox(_T("请输入正确的数量。"));
                    pEdit->SetFocus();
                    return;
                }
                if (_tstoi(strn) > 20)
                {
                    MyAfxMessageBox(_T("一种类型最大的题目数不得超过20。"));
                    pEdit->SetFocus();
                    return;
                }
                strTmp += pEdit->GetText().GetData();
            }
            else
            {
                strTmp += _T("0");
            }
            if (i != 3)
            {
                strTmp += _T(",");
            }
        }
        if (strTmp == _T("0,0,0,0"))
        {
            MyAfxMessageBox(_T("请至少选择一种类型的题目，并输入数量。"));
            return;
        }
        SaveSetup(_T("Listen_QuestionNumber"), strTmp);
    }
    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("listentypecuotiku")));
    if (pOpt->IsSelected())
    {
        SaveSetup(_T("Listen_QuestionType"), _T("4"));
        pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb")));
        pItem = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(pCombo->GetCurSel()));
        SaveSetup(_T("Listen_QuestionNumber"), pItem->GetText().GetData());
    }

    //选择界面或者确定按钮，从此地方进入“练听力”界面，在此进行必要的初始化
    m_bListenReview = false;
    m_nListenTrueNumber = 0;
    m_ListenDic.clear();
    m_strListenQuestion.Empty();
    if (m_nSkinId == sListenSelectSkin)
    {
        OpenChildModalWnd(sListenSkin);
    }
    else
    {
        pLbl = xFindControl(CLabelUI, _T("listenoktxt"));
        pLbl->SetText(_T("0"));
        pButton = xFindControl(CButtonUI, _T("listenstartbtn"));
        pButton->SetEnabled(true);
        pButton = xFindControl(CButtonUI, _T("listenreviewbtn"));
        pButton->SetVisible(false);
        pButton = xFindControl(CButtonUI, _T("listenviewtextbtn"));
        pButton->SetVisible(false);
        pButton = xFindControl(CButtonUI, _T("listenextbtn"));
        pButton->SetVisible(false);
        pButton = xFindControl(CButtonUI, _T("listenagainbtn"));
        pButton->SetVisible(false);

        MyAfxMessageBox(_T("保存设置成功。正在生成习题。"));
        if (InitListenStatus())
        {
            InitListenQuestion(false);
            CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("listenoptionvlay"));
            pVLay->SetEnabled(false);

            pVLay = xFindControl(CVerticalLayoutUI, _T("listenvlaysmallquest"));
            pVLay->SetEnabled(false);

            for (i = 0; i < 4; i++)
            {
                pOpt = xFindControl(COptionUI, g_listen_pic_opt[i]);
                pOpt->SetEnabled(false);
            }
        }
    }
}

void MainFrame::ListenNoQuestion()
{
    m_PaintManager.SendNotify(xFindControl(CControlUI, _T("closebtn")), DUI_MSGTYPE_CLICK);
}

void MainFrame::ListenStartClick()
{
    CButtonUI* pBnt = xFindControl(CButtonUI, _T("listenstartbtn"));
    pBnt->SetEnabled(false);

    pBnt = xFindControl(CButtonUI, _T("listenviewtextbtn"));
    pBnt->SetVisible(true);

    pBnt = xFindControl(CButtonUI, _T("listenreviewbtn"));
    pBnt->SetVisible(true);

    pBnt = xFindControl(CButtonUI, _T("listenextbtn"));
    pBnt->SetVisible(m_bListenReview ? true : false);

    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("listenoptionvlay"));
    pVLay->SetEnabled(true);

    pVLay = xFindControl(CVerticalLayoutUI, _T("listenvlaysmallquest"));
    pVLay->SetEnabled(true);

    int i;
    COptionUI* pOpt;
    for (i = 0; i < _countof(g_listen_pic_opt); i++)
    {
        pOpt = xFindControl(COptionUI, g_listen_pic_opt[i]);
        pOpt->SetEnabled(true);
    }

    InitListenQuestion(true);
}

void MainFrame::ListenTypeCfg(int nType)
{
    int i;
    bool bEnabled[3];
    CComboUI *pCombNum = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb"))),
        *pCombDX = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("listendanxiangcmb")));
    CCheckBoxUI* pChk[4];
    CEditUI* pEdt[4];

    pChk[0] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("listendanjuchk")));
    pChk[1] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("listenduihuachk")));
    pChk[2] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("listenduanwenchk")));
    pChk[3] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("listentupianchk")));

    pEdt[0] = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listendanjuedt")));
    pEdt[1] = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listenduihuaedt")));
    pEdt[2] = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listenduanwenedt")));
    pEdt[3] = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("listentupianedt")));

    switch (nType)
    {
    case 0:
    case 3:
        bEnabled[0] = true;
        bEnabled[1] = false;
        bEnabled[2] = false;
        break;
    case 1:
        bEnabled[0] = true;
        bEnabled[1] = true;
        bEnabled[2] = false;
        break;
    case 2:
        bEnabled[0] = false;
        bEnabled[1] = true;
        bEnabled[2] = true;
        break;
    default:
        ASSERT(0);
        return;
    }

    pCombNum->SetEnabled(bEnabled[0]);
    pCombDX->SetEnabled(bEnabled[1]);
    for (i = 0; i < 4; i++)
    {
        pChk[i]->SetEnabled(bEnabled[2]);
        pEdt[i]->SetEnabled(bEnabled[2]);
    }
    return;
}

void MainFrame::InitListenCfgControl()
{
    int i;
    CString strTmp;

    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    COptionUI* pOpt;
    CEditUI* pEdit;

    CString strGrade = ReadSetup(_T("Listen_Grade"));
    CString strQNum = ReadSetup(_T("Listen_QuestionNumber"));
    CString strQType = ReadSetup(_T("Listen_QuestionType"));

    //////////////////////////////////////////////////////////////////////////
    //年级，在配置表中，按照ID保存
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 年级 Order by 顺序"));
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("gradecmb")));
    pCombo->RemoveAll();
    int idx = 0, nselect = 0;
    while(!pRecdPtr->GetadoEOF())
    {
        // 跳过“必修一、二、三”，因为没有题目 [2016-1-8 Arthur]
        strTmp = GetCollect(pRecdPtr, _T("名称"));
        if (strTmp == _T("必修一") ||       //id==18
            strTmp == _T("必修二") ||       //id==19
            strTmp == _T("必修三"))         //id==24
        {
            pRecdPtr->MoveNext();
            continue;
        }
        pElement = new CListLabelElementUI;
        pElement->SetText(strTmp);
        strTmp = GetCollect(pRecdPtr, _T("Id"));
        if (strTmp == strGrade)
        {
            nselect = idx;
        }
        pElement->SetUserData(strTmp);
        pCombo->Add(pElement);
        pRecdPtr->MoveNext();
        idx++;
    }
    pCombo->SelectItem(nselect);

    //////////////////////////////////////////////////////////////////////////
    //题型
    //智能（1），单项（2），全真（3），错题库（4）
    //选“全真”时候，在配置表中按 单句,对话,短文,图片 这样的次序保存Listen_QuestionNumber
    static const TCHAR* const strTypeNameConfig[] =
    {
        _T("listentypezhineng"),
        _T("listentypedanxiang"),
        _T("listentypequanzhen"),
        _T("listentypecuotiku")
    };
    i = _tstoi(strQType);
    //for safer
    if (i > _countof(strTypeNameConfig))
    {
        i = 1;
    }
    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(strTypeNameConfig[i - 1]));
    pOpt->Selected(true);

    //全真试题
    static const TCHAR* const strQuesNumber[] =
    {
        _T("listendanjuedt"),
        _T("listenduihuaedt"),
        _T("listenduanwenedt"),
        _T("listentupianedt")
    };
    static CString sQuanzhenChk[]=
    {
        _T("listendanjuchk"),
        _T("listenduihuachk"),
        _T("listenduanwenchk"),
        _T("listentupianchk"),
    };

    CString strNum[4] = {_T("0"),_T("0"),_T("0"),_T("0")};
    SplitResultVector strQumResult;
    TString tstr = strQNum;
    int n = SplitString(tstr, _T(","), strQumResult);
    for (i = 0; i < (int)strQumResult.size(); i++)
    {
        if (i > 3)
        {
            break;
        }
        strNum[i] = strQumResult[i].c_str();
    }
    for (i = 0; i < 4; i++)
    {
        pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(strQuesNumber[i]));
        pEdit->SetText(strNum[i]);
        CCheckBoxUI* pChk = xFindControl(CCheckBoxUI, sQuanzhenChk[i]);
        pChk->SetCheck(_tstoi(strNum[i]) > 0 ? true : false);
    }

    //////////////////////////////////////////////////////////////////////////
    //题量
    nselect = 2;
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("subjectnumcmb")));
    if (strQType == _T("1") || strQType == _T("2"))
    {
        idx = _tstoi(strNum[0]) + _tstoi(strNum[1]) + _tstoi(strNum[2]) + _tstoi(strNum[3]);
    }
    else if (strQType == _T("3"))
    {
        idx = 0;
    }
    else if (strQType == _T("4"))
    {
        idx = _tstoi(strQumResult[0].c_str());
    }
#ifdef _DEBUG
    else
    {
        ASSERT(FALSE);
    }
#endif
    for (i = 0; i < pCombo->GetCount(); i++)
    {
        pElement = static_cast<CListLabelElementUI*>(pCombo->GetItemAt(i));
        if (idx == _tstoi(pElement->GetText().GetData()))
        {
            nselect = i;
        }
    }
    pCombo->SelectItem(nselect);

    idx = 0;
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("listendanxiangcmb")));
    for (i = 0; i < (int)strQumResult.size(); i++)
    {
        if (lstrcmp(strQumResult[i].c_str(), _T("0")) != 0)
        {
            idx = i;
            break;
        }
    }
    pCombo->SelectItem(idx);
}

//////////////////////////////////////////////////////////////////////////
//单词
void MainFrame::WordSetCfgControl(bool bIsBook)
{
    CComboUI* pCmbGrade = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfggradecmb")));
    CComboUI* pCmbBook = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextbookcmb")));
    CComboUI* pCmbText = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextcmb")));
    CComboUI* pCmbNew = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgnewwordcmb")));

    /*
    注意，xml中option控件不要预先设置selected="true"属性，会导致在builder.create的时候就发送notify，此时
    wordcfggradecmb等控件应该还未创建
    或者，在此判断是否为NULL
    */
    if (pCmbGrade)
    {
        pCmbGrade->SetEnabled(bIsBook);
    }
    if (pCmbBook)
    {
        pCmbBook->SetEnabled(bIsBook);
    }
    if (pCmbText)
    {
        pCmbText->SetEnabled(bIsBook);
    }
    if (pCmbNew)
    {
        pCmbNew->SetEnabled(!bIsBook);
    }
}

void MainFrame::InitWordCfgControl()
{
    CString strTmp;
    COptionUI* pOpt;

    CString strType = ReadSetup(_T("word_type"));
    CString strGrade = ReadSetup(_T("word_grade"));
    CString strBook = ReadSetup(_T("word_book"));
    CString strUnit = ReadSetup(_T("word_unit"));
    CString strnewword = ReadSetup(_T("word_nwb"));

    InitWordCfgGradeCombo(strGrade);
    InitWordCfgBookCombo(strBook);
    InitWordCfgUnitCombo(strUnit);

    InitWordCfgNewWordCombo(strnewword);

    strTmp = (strType == _T("0")) ? _T("wordcfgbookopt") : _T("wordcfgnewwordopt");
    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(strTmp));
    pOpt->Selected(true);

    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("wordcfglernopt")));
    pOpt->Selected(true);
}

void MainFrame::InitWordCfgGradeCombo(LPCTSTR lpstrGrade/* =NULL */)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;

    pGradePtr = GetTablesBySql(_T("%s"), _T("Select * from 年级 Order by 顺序"));
    idx = 0, nSel = 0;
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfggradecmb")));
    pCombo->RemoveAll();
    if (pGradePtr == NULL)
    {
        return;
    }
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pGradePtr, _T("名称")));
        strTmp = GetCollect(pGradePtr, _T("Id"));
        pElement->SetUserData(strTmp);
        if (lpstrGrade && (strTmp == lpstrGrade))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);
}

void MainFrame::InitWordCfgBookCombo(LPCTSTR lpstrBook/* =NULL */)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextbookcmb")));
    pCombo->RemoveAll();

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfggradecmb")));
    if (pCombo->GetCount() <= 0)
    {
        return;
    }
    strTmp.Format(_T("Select * from 教材 where 年级=%s Order by 顺序"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
    pGradePtr = GetTablesBySql(_T("%s"), strTmp);
    if (pGradePtr == NULL)
    {
        return;
    }
    pCombo = xFindControl(CComboUI, _T("wordcfgtextbookcmb"));
    idx = 0, nSel = 0;
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pGradePtr, _T("名称")));
        strTmp = GetCollect(pGradePtr, _T("Id"));
        pElement->SetUserData(strTmp);
        if (lpstrBook && (strTmp == lpstrBook))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);
}

void MainFrame::InitWordCfgUnitCombo(LPCTSTR lpstrUnit/* =NULL */)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;
    CString gid, bid;

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextcmb")));
    pCombo->RemoveAll();

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfggradecmb")));
    if (pCombo->GetCount() <= 0)
    {
        return;
    }
    gid = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextbookcmb")));
    if (pCombo->GetCount() <= 0)
    {
        return;
    }
    bid = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();

    pGradePtr = GetTablesBySql(_T("Select Id, 名称 from 课文 where 教材=%s and 年级=%s Order by Id"),
        bid, gid);
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgtextcmb")));
    idx = 1, nSel = 0;
    pElement = new CListLabelElementUI;
    pElement->SetText(_T("全部"));
    pElement->SetUserData(_T("0"));
    pCombo->Add(pElement);
    if (pGradePtr == NULL)
    {
        pCombo->SelectItem(0);
        return;
    }
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pGradePtr, _T("名称")));
        strTmp = GetCollect(pGradePtr, _T("Id"));
        pElement->SetUserData(strTmp);
        if (lpstrUnit && (strTmp == lpstrUnit))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);
}

void MainFrame::InitWordCfgNewWordCombo(LPCTSTR lpstrNewWord/* =NULL */)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;

    pGradePtr = GetTablesBySql(_T("%s"), _T("Select * from 生词本"));
    //empty?
    if (NULL == pGradePtr)
    {
        return;
    }
    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("wordcfgnewwordcmb")));
    pCombo->RemoveAll();
    idx = 0, nSel = 0;
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        strTmp = GetCollect(pGradePtr, _T("名称"));
        pElement->SetText(strTmp);
        pElement->SetUserData(GetCollect(pGradePtr, _T("Id")));
        if (lpstrNewWord && (strTmp == lpstrNewWord))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);
}

void MainFrame::InitWordStatus()
{
    CTextUI* pWordTxt;
    //CComboUI* pCombo;
    //CString strRepeatTime = ReadSetup(_T("Word_RepeatTime"));
    //CString strSpaceTime = ReadSetup(_T("Word_SpaceTime"));
    //CString strSpell = ReadSetup(_T("Word_Spell"));
    //CString strSpeakSample = ReadSetup(_T("Word_SpeakSample"));
    CString strHideWord = ReadSetup(_T("Word_HideWord"));
    CString strHideParaphrase = ReadSetup(_T("Word_HideParaphrase"));
    //CString strSpeaker = ReadSetup(_T("Word_Speaker"));
    //CString strSpeed = ReadSetup(_T("Word_Speed"));

    SetVoiceRateBySetting(_T("Word_Speaker"), _T("Word_Speed"));

    CControlUI* pCtrl = xFindControl(CControlUI, _T("wordfollowprogvlayout"));
    if (pCtrl)
    {
        pCtrl->SetVisible(false);
    }
    
    pCtrl = xFindControl(CControlUI, _T("wordScoreHlayout"));
    if (pCtrl)
    {
        pCtrl->SetVisible(false);
    }
    
    CString strGrade = ReadSetup(_T("word_grade"));
    CString strBook = ReadSetup(_T("word_book"));
    CString strUnit = ReadSetup(_T("word_unit"));
    CString strGroup = ReadSetup(_T("word_group"));
    TRACE(_T("strGroup in db:%s\n"), strGroup);
    if (strGroup.IsEmpty())
    {
        strGroup = _T("1");
    }
    CString strNewBook = ReadSetup(_T("word_nwb"));
    CString strSort = ReadSetup(_T("word_sort"));
    if (strSort.IsEmpty())
    {
        strSort = _T("0");
    }
    CString strText, strTmp, strTmp2;
    //TRACE(_T("strGrade:%s, book:%s, unit:%s\n"), strGrade, strBook, strUnit);

    if (strGrade == _T("0"))
    {
        strTmp2.Format(_T("Select 名称 from 生词本 where Id=%s"),
            strNewBook);       
        strText.Format(_T("生词本>%s"),
            GetSingleField(strTmp2));
    }
    else
    {
        strTmp2.Format(_T("Select 名称 from 年级 where Id=%s"),
            strGrade);
        strText = GetSingleField(strTmp2) + _T(">");
        strTmp2.Format(_T("Select 名称 from 教材 where Id=%s and 年级=%s"),
            strBook, strGrade);
        strText += GetSingleField(strTmp2) + _T(">");
        if (strUnit != _T("0"))
        {
            strTmp2.Format(_T("Select 名称 from 课文 where Id=%s and 教材=%s and 年级=%s"),
                strUnit, strBook, strGrade);
            strText += GetSingleField(strTmp2);
            //strText += GetSingleField(strTmp2) + _T(">");
            //if (strGroup != _T("1"))
            //{
            //    strTmp2.Format(_T("Select 名称 from 小组 where Id=%s and 课文=%s and 教材=%s and 年级=%s"),
            //        strGroup, strUnit, strBook, strGrade);
            //    strText += GetSingleField(strTmp2);
            //}
            //else
            //{
            //    strText += _T("全部");
            //}
        }
        else
        {
            strText += _T("全部");
        }
    }
    pWordTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("wordlocationtxt")));
    pWordTxt->SetText(strText);

    if (strGrade == _T("0"))
    {
        strTmp.Format(_T("Select Id, 单词 + '\t\t' + 释义 as 单词显示 from 词典 where Id in (Select 生词 from 生词 where 生词本=%s)"), 
            strNewBook);
    }
    else
    {
        if (strUnit == _T("0"))
        {
            strTmp.Format(_T("Select Id, 顺序, 单词 + '\t\t' + 释义 as 单词显示 from 单词 where 教材=%s and 年级=%s"),
                strBook, strGrade);
        }
        else
        {
            strTmp.Format(_T("Select Id, 顺序, 单词 + '\t\t' + 释义 as 单词显示 from 单词 where 课文=%s and 教材=%s and 年级=%s"),
                strUnit, strBook, strGrade);
        }
    }
    if (strHideWord == _T("1"))
    {
        strTmp.Replace(_T("单词 + "), _T(""));
    }
    if (strHideParaphrase == _T("1"))
    {
        strTmp.Replace(_T(" + 释义"), _T(""));
    }

    if (strGrade == _T("0"))
    {
        strTmp += _T(" Order by 单词");
    }
    else
    {
        strTmp += (_T("0") == strSort) ? _T(" Order by 单词") : _T(" Order by 顺序");
    }

    TRACE(_T("GetTablesBySql:%s\n"), strTmp);
    _RecordsetPtr pWordPtr = GetTablesBySql(_T("%s"), strTmp);
    CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("wordlist")));
    pList->RemoveAll();
    //也可考虑使用CListContainerElementUI
    CListTextElementUI* pItem;
    int x;
    //LPCTSTR strAttr = _T("height=\"24\"");

    SplitResultVector sptGroup;
    //default select all
    SplitString(TString(strGroup), _T(","), sptGroup, false);
    //for safer
    if (sptGroup.size() <= 0)
    {
        sptGroup.clear();
        sptGroup.push_back(TString(_T("1")));
    }

    if (pWordPtr != NULL)
    {
        CString strId;

        if (_T("0") == sptGroup[0])
        {
            int xi;
            int idx = 0;
            while(!pWordPtr->GetadoEOF())
            {
                idx++;
                if ( (idx < (int)sptGroup.size()) &&
                    (_T("0") == sptGroup[idx]) )
                {
                    //跳过10条记录
                    for (xi = 0; (xi < 10) && !pWordPtr->GetadoEOF(); ++xi)
                    {
                        pWordPtr->MoveNext();
                    }
                    //continue;
                }
                else
                {
                    for (xi = 0; (xi < 10) && !pWordPtr->GetadoEOF(); ++xi)
                    {
                        strId = GetCollect(pWordPtr, _T("Id"));

                        pItem = new CListTextElementUI;
                        pItem->SetTag(_tstoi(strId));
                        pList->Add(pItem);

                        strTmp = GetCollect(pWordPtr, _T("单词显示"));
                        x = strTmp.Find(_T('\t'));
                        pItem->SetText(0, strTmp.Left(x));
                        strTmp2 = strTmp.Mid(x, strTmp.GetLength() - x);
                        strTmp2.TrimLeft(_T('\t'));
                        pItem->SetText(1, strTmp2);

                        pWordPtr->MoveNext();
                    }
                }
            }
        }
        else
        {
            while(!pWordPtr->GetadoEOF())
            {
                strId = GetCollect(pWordPtr, _T("Id"));
                pItem = new CListTextElementUI;
                pItem->SetTag(_tstoi(strId));
                pList->Add(pItem);

                strTmp = GetCollect(pWordPtr, _T("单词显示"));
                //TRACE(_T("Id:%s, 单词显示:%s, 顺序:%s\n"),
                //    strId, strTmp, GetCollect(pWordPtr, _T("顺序")));
                x = strTmp.Find(_T('\t'));
                pItem->SetText(0, strTmp.Left(x));
                strTmp2 = strTmp.Mid(x, strTmp.GetLength() - x);
                strTmp2.TrimLeft(_T('\t'));
                pItem->SetText(1, strTmp2);

                pWordPtr->MoveNext();
            }
        }
#if 0
        while(!pWordPtr->GetadoEOF())
        {
            strId = GetCollect(pWordPtr, _T("Id"));
            if (_T("0") == sptGroup[0])     //如果不是全选
            {
                //if (_T("0") == strSort)     //如果是安小组进行分类
                //{
                //    if (find(sptGroup.begin() + 1, sptGroup.end(), TString(strId)) == sptGroup.end())
                //    {
                //        pWordPtr->MoveNext();
                //        continue;
                //    }
                //}
                //else
                {
                    int xi;
                    if (_T("0") == sptGroup[idx++])
                    {
                        //跳过10条记录
                        for (xi = 0; (xi < 10) && !pWordPtr->GetadoEOF(); ++xi)
                        {
                            pWordPtr->MoveNext();
                        }
                        //continue;
                    }
                    else
                    {
                        for (xi = 0; (xi < 10) && !pWordPtr->GetadoEOF(); ++xi)
                        {
                            pItem = new CListTextElementUI;
                            pItem->SetTag(_tstoi(strId));
                            pList->Add(pItem);

                            strTmp = GetCollect(pWordPtr, _T("单词显示"));
                            x = strTmp.Find(_T('\t'));
                            pItem->SetText(0, strTmp.Left(x));
                            strTmp2 = strTmp.Mid(x, strTmp.GetLength() - x);
                            strTmp2.TrimLeft(_T('\t'));
                            pItem->SetText(1, strTmp2);

                            pWordPtr->MoveNext();
                        }
                    }
                }
            }
            //pItem = new CListTextElementUI;
            //pItem->SetTag(_tstoi(strId));
            //pList->Add(pItem);

            //strTmp = GetCollect(pWordPtr, _T("单词显示"));
            //x = strTmp.Find(_T('\t'));
            //pItem->SetText(0, strTmp.Left(x));
            //strTmp2 = strTmp.Mid(x, strTmp.GetLength() - x);
            //strTmp2.TrimLeft(_T('\t'));
            //pItem->SetText(1, strTmp2);

            //pWordPtr->MoveNext();
        }
#endif
    }

    CButtonUI* pWordBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("wordspeakwordbtn")));
    CButtonUI* pSampleBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("wordspeaksamplebtn")));

    CTextUI* pWordText = xFindControl(CTextUI, _T("wordnametxt"));
    CRichEditUI* pSymbolText = xFindControl(CRichEditUI, _T("wordsymboltxt"));
    CRichEditUI* pParaText = xFindControl(CRichEditUI, _T("wordParaphrasetxt"));
    CRichEditUI* pEnText = xFindControl(CRichEditUI, _T("wordensampletxt"));
    CRichEditUI* pCnText = xFindControl(CRichEditUI, _T("wordcnsampletxt"));
    CControlUI* pPicWord = xFindControl(CControlUI, _T("wordpicwordbtn"));

    x = pList->GetCount();
    if (x > 0)
    {
        pWordBtn->SetVisible(true);
        pSampleBtn->SetVisible(true);
        pWordText->SetVisible(true);
        pSymbolText->SetVisible(true);
        pParaText->SetVisible(true);
        pEnText->SetVisible(true);
        pCnText->SetVisible(true);

        pList->SelectItem(0);
    }
    else
    {
        pWordBtn->SetVisible(false);
        pSampleBtn->SetVisible(false);
        pWordText->SetVisible(false);
        pSymbolText->SetVisible(false);
        pParaText->SetVisible(false);
        pEnText->SetVisible(false);
        pCnText->SetVisible(false);
        pPicWord->SetVisible(false);
    }

    pWordTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("wordnametxt")));
    CRichEditUI* pEnSampleTxt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("wordensampletxt")));
    if (strHideWord == _T("1"))
    {
        ////pWordTxt->SetBkColor(DUI_WHITE);
        //pWordTxt->SetTextColor(DUI_WHITE);
        ////pEnSampleTxt->SetBkColor(DUI_RGB(0xe8,0xe8,0xe8));
        //pEnSampleTxt->SetTextColor(DUI_WHITE);

        pWordTxt->SetVisible(false);
        pEnSampleTxt->SetVisible(false);
    }
    //else
    //{
    //    ////pWordTxt->SetBkColor(DUI_WHITE);
    //    //pWordTxt->SetTextColor(DUI_BLACK);
    //    ////pEnSampleTxt->SetBkColor(DUI_WHITE);
    //    //pEnSampleTxt->SetTextColor(DUI_BLACK);

    //    pWordTxt->SetVisible(true);
    //    pEnSampleTxt->SetVisible(true);
    //}

    CRichEditUI* pRichTxt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("wordParaphrasetxt")));
    pEnSampleTxt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("wordcnsampletxt")));
    if (strHideParaphrase == _T("1"))
    {
        ////pRichTxt->SetBkColor(DUI_RGB(0xe8,0xe8,0xe8));
        //pRichTxt->SetTextColor(DUI_WHITE);
        ////pEnSampleTxt->SetBkColor(DUI_RGB(0xe8,0xe8,0xe8));
        //pEnSampleTxt->SetTextColor(DUI_WHITE);

        pRichTxt->SetVisible(false);
        pEnSampleTxt->SetVisible(false);
    }
    //else
    //{
    //    ////pRichTxt->SetBkColor(DUI_WHITE);
    //    //pRichTxt->SetTextColor(DUI_BLACK);
    //    ////pEnSampleTxt->SetBkColor(DUI_WHITE);
    //    //pEnSampleTxt->SetTextColor(DUI_BLACK);

    //    pRichTxt->SetVisible(true);
    //    pEnSampleTxt->SetVisible(true);
    //}

    WordUpdateWordCount();
    WordChangeAddNewWordBtn();
}

void MainFrame::WordChangeAddNewWordBtn()
{
    //CComboUI* pCombo = xFindControl(CComboUI, _T("wordgradecmb"));
    //CString strGrade = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    CString strGrade = ReadSetup(_T("word_grade"));
    CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("wordaddnewbtn")));
    pBtn->SetText((strGrade == _T("0")) ? _T("删除生词") : _T("加入生词本"));
}

void MainFrame::WordUpdateWordCount()
{
    CString strTmp;
    int n;
    CTextUI* pCountTxt = xFindControl(CTextUI, _T("wordalltxt"));
    CTextUI* pNowTxt = xFindControl(CTextUI, _T("wordnowtxt"));
    CListUI* pList = xFindControl(CListUI, _T("wordlist"));

    n = pList->GetCount();
    strTmp.Format(_T("%d"), n);
    pCountTxt->SetText(strTmp);
    
    if (n > 0)
    {
        n = pList->GetCurSel() + 1;
    }
    else
    {
        n = 0;
    }
    strTmp.Format(_T("%d"), n);
    pNowTxt->SetText(strTmp);
}

void MainFrame::InitWordGradeCombo(LPCTSTR lpstrGrade/*=NULL*/)
{
#if 1
#else
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;

    pCombo = xFindControl(CComboUI, _T("wordgradecmb"));
    pCombo->RemoveAll();
    pElement = new CListLabelElementUI;
    pElement->SetText(_T("生词本"));
    pElement->SetUserData(_T("0")); //Id
    pCombo->Add(pElement);
    pRecdPtr = GetTablesBySql(_T("Select Id, 名称 from 年级 Order by 顺序"));
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
#endif
}

void MainFrame::InitWordBookCombo(LPCTSTR lpstrBook/*=NULL*/)
{
#if 1
#else
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    CString strSql, strTmp;

    pCombo = xFindControl(CComboUI, _T("wordgradecmb"));
    strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    if (strTmp == _T("0"))
    {
        strSql = _T("Select * from 生词本");
    }
    else
    {
        strSql.Format(_T("Select * from 教材 where 年级=%s Order by 顺序"),
            strTmp);
    }

    pCombo = xFindControl(CComboUI, _T("wordtextbookcmb"));
    pCombo->RemoveAll();
    pRecdPtr = GetTablesBySql(_T("%s"), strSql);
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
            pCombo->Add(pElement);
            pRecdPtr->MoveNext();
            idx++;
        }
    }

    if (pCombo->GetCount() > 0)
    {
        pCombo->SelectItem(nSel);
    }
#endif
}

void MainFrame::InitWordUnitCombo(LPCTSTR lpstrUnit/*=NULL*/)
{
#if 1
#else
    _RecordsetPtr pRecdPtr;
    CComboUI *pComboGrade, *pComboBook, *pComboUnit;
    CListLabelElementUI* pElement;
    CString strGID, strBID;

    pComboUnit = xFindControl(CComboUI, _T("wordbookcmb"));
    pComboUnit->RemoveAll();

    pElement = new CListLabelElementUI;
    pElement->SetText(_T("全部"));
    pElement->SetUserData(_T("0"));
    pComboUnit->Add(pElement);
    pComboUnit->SelectItem(0);

    pComboGrade = xFindControl(CComboUI, _T("wordgradecmb"));
    strGID = pComboGrade->GetItemAt(pComboGrade->GetCurSel())->GetUserData();
    if (strGID == _T("0"))
    {
        return;
    }

    pComboBook = xFindControl(CComboUI, _T("wordtextbookcmb"));
    strBID = pComboBook->GetItemAt(pComboBook->GetCurSel())->GetUserData();

    if (strBID == _T("0"))
    {
        return;
    }

    pRecdPtr = GetTablesBySql(_T("Select Id, 名称 from 课文 where 年级=%s and 教材=%s Order by Id"),
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
#endif
}

void MainFrame::WordChangeWordParaphrase()
{
    CListUI* pList = xFindControl(CListUI, _T("wordlist"));
    int count = pList->GetCount();

    //CButtonUI* pWordBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("wordspeakwordbtn")));
    //CButtonUI* pSampleBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("wordspeaksamplebtn")));

    if (0 >= count)
    {
        return;
    }

    int nIndex = pList->GetCurSel();

    CTextUI* pWordText = xFindControl(CTextUI, _T("wordnametxt"));
    CRichEditUI* pSymbolText = xFindControl(CRichEditUI, _T("wordsymboltxt"));
    //CRichEditUI* pSymbolFollowText = xFindControl(CRichEditUI, _T("wordsymbolfollowtxt"));
    CRichEditUI* pParaText = xFindControl(CRichEditUI, _T("wordParaphrasetxt"));
    CRichEditUI* pEnText = xFindControl(CRichEditUI, _T("wordensampletxt"));
    CRichEditUI* pCnText = xFindControl(CRichEditUI, _T("wordcnsampletxt"));
    CControlUI* pPicWord = xFindControl(CControlUI, _T("wordpicwordbtn"));
    CButtonUI* pSpkEnBtn = xFindControl(CButtonUI, _T("wordspeaksamplebtn"));

    int nId = pList->GetItemAt(nIndex)->GetTag();

    _RecordsetPtr pWordPtr = GetTablesBySql(_T("Select * from 单词 where Id=%d"),
        nId);
    CString strWord = GetCollect(pWordPtr, _T("单词"));
    pWordText->SetText(strWord);
    TRACE(_T("set symbol:%s\n"), GetCollect(pWordPtr, _T("音标")));
    pSymbolText->SetText(GetCollect(pWordPtr, _T("音标")));
    //pSymbolText->SetSelNone();
    //pSymbolFollowText->SetText(_T(""));
    pParaText->SetText(GetCollect(pWordPtr, _T("释义")));
    pEnText->SetText(GetCollect(pWordPtr, _T("英文例句")));
    pCnText->SetText(GetCollect(pWordPtr, _T("中文例句")));

    pSpkEnBtn->SetVisible(pEnText->GetText().IsEmpty() ? false : true);

    pPicWord->SetVisible(false);
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
            finder.Close();
        }
    }

    WordUpdateWordCount();
}

void MainFrame::WordAddNewClicked()
{
    CButtonUI* pButton;
    CListUI* pList;

    m_bWordAutoRead = false;
    pButton = xFindControl(CButtonUI, _T("wordautoplaybtn"));
    if (pButton->GetText() == _T("停止播放"))
    {
        pButton->SetText(_T("自动播放"));
        m_bWordIsPlayOn = false;
    }
    StopSpeak();
    StopWaveRecordReco();
    pButton = xFindControl(CButtonUI, _T("wordaddnewbtn"));
    pList = xFindControl(CListUI, _T("wordlist"));
    if (pButton->GetText() == _T("删除生词"))
    {
        if (pList->GetCount() > 0)
        {
            ExecuteSql(_T("delete from 生词 where 生词本=%s and 生词=%d"),
                ReadSetup(_T("word_nwb")), pList->GetItemAt(pList->GetCurSel())->GetTag());
            InitWordStatus();
        }
    }
    else
    {
        //OpenModalWnd(new CWordAddnew(this), g_SkinInfo[sWordAddNewSkin].title);
        if (pList->GetCount() > 0)
        {
            CWordAddnew* pWnd = new CWordAddnew(this);
            if (pWnd)
            {
                pWnd->m_nNewWord = pList->GetItemAt(pList->GetCurSel())->GetTag();
                OpenModalWnd(pWnd, g_SkinInfo[sWordAddNewSkin].title);
            }
            InitWordStatus();
        }
    }
}

void MainFrame::WordStopAutoPlay()
{
    CButtonUI* pButton = xFindControl(CButtonUI, _T("wordautoplaybtn"));

    m_bWordIsPlayOn = false;
    m_bWordAutoRead = false;
    StopSpeak();
    if (pButton)
    {
        pButton->SetText(_T("自动播放"));
    }
}

void MainFrame::WordAutoPlayClicked()
{
    int i;
    CButtonUI* pButton;
    CListUI* pList = xFindControl(CListUI, _T("wordlist"));

    if (pList->GetCount() > 0)
    {
        pButton = xFindControl(CButtonUI, _T("wordautoplaybtn"));
        if (m_bWordIsPlayOn)
        {
            WordStopAutoPlay();
        }
        else
        {
            CString strRepeatTime = ReadSetup(_T("Word_RepeatTime"));
            CString strSpell = ReadSetup(_T("Word_Spell"));
            CString strSpeakSample = ReadSetup(_T("Word_SpeakSample"));

            m_bWordIsPlayOn = true;
            //auto play word
            m_bWordAutoRead = true;
            i = _tstoi(strRepeatTime);
            if ( (strSpeakSample != _T("1")) &&
                (strSpell != _T("1")) )
            {
                m_nWordModeBase = 2 * i;
            }
            else if ( (strSpeakSample == _T("1")) &&
                (strSpell != _T("1")) )
            {
                m_nWordModeBase = 3 * i;
            }
            else if ( (strSpeakSample != _T("1")) &&
                (strSpell == _T("1")) )
            {
                m_nWordModeBase = 4 * i;
            }
            else if ( (strSpeakSample == _T("1")) &&
                (strSpell == _T("1")) )
            {
                m_nWordModeBase = 5 * i;
            }
            pButton->SetText(_T("停止播放"));
            StopSpeak();
            StopWaveRecordReco();
            m_nWordSpeakCount = 0;
            WordSpeakCurretnWordAuto();
        }
    }
}

void MainFrame::WordMp3Clicked()
{
    StopSpeak();
    StopWaveRecordReco();
    CWordToVoice *pwv = new CWordToVoice(this);
    OpenModalWnd(pwv, g_SkinInfo[sWordToVoiceSkin].title);
}

void MainFrame::WordChangeBtnClicked()
{
#if 1
    StopSpeak();
    StopWaveRecordReco();
    CWordQuikselect* pQuik = new CWordQuikselect(this);
    pQuik->m_strGrade = ReadSetup(_T("word_grade"));
    pQuik->m_strBook = ReadSetup(_T("word_book"));
    pQuik->m_strUnit = ReadSetup(_T("word_unit"));
    if (OpenModalWnd(pQuik, _T("单词选择")) == IDOK)
    {
        InitWordStatus();
    }
#else
    CComboUI* pCmbGrade = xFindControl(CComboUI, _T("wordgradecmb"));
    CComboUI* pCmbBook = xFindControl(CComboUI, _T("wordtextbookcmb"));
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("wordautoplaybtn"));
    CString strTmp;

    m_bWordAutoRead = false;
    if (m_bWordIsPlayOn/*pBtn->GetText() == _T("停止播放")*/)
    {
        pBtn->SetText(_T("自动播放"));
        m_bWordIsPlayOn = false;
    }
    StopSpeak();

    strTmp = pCmbGrade->GetItemAt(pCmbGrade->GetCurSel())->GetUserData();
    if ( (strTmp == _T("0")) &&
        (pCmbBook->GetCount() <= 0) )
    {
        MyAfxMessageBox(_T("您还没有创建生词本。"));
        return;
    }

    SaveSetup(_T("word_grade"), strTmp);
    if (_T("0") == strTmp)
    {
        SaveSetup(_T("word_type"), _T("1"));
        SaveSetup(_T("word_nwb"), pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData().GetData());
    }
    else
    {
        //SaveSetup(_T("word_grade"), strTmp);
        SaveSetup(_T("word_type"), _T("0"));
        SaveSetup(_T("word_book"), pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData().GetData());
        pCmbBook = xFindControl(CComboUI, _T("wordbookcmb"));
        if (pCmbBook->GetCurSel() > 0)
        {
            SaveSetup(_T("word_unit"), pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData().GetData());
        }
        else
        {
            SaveSetup(_T("word_unit"), _T("0"));
        }
    }

    InitWordStatus();
#endif
}

HRESULT MainFrame::WordSpeakCurretnWordAuto()
{
    CTextUI* pText = xFindControl(CTextUI, _T("wordnametxt"));    
    CString strText = pText->GetText().GetData(), strTmp;

    if (strText.GetLength() <= 0)
    {
        return S_OK;
    }

    CString strRepeatTime = ReadSetup(_T("Word_RepeatTime"));
    CString strSpell = ReadSetup(_T("Word_Spell"));
    CString strSpeakSample = ReadSetup(_T("Word_SpeakSample"));
    CString strSpeed = ReadSetup(_T("Word_Speed"));
    CString strSpace = ReadSetup(_T("Word_SpaceTime"));

    int s = _tstoi(strSpace);
    if (s < 1 ||
        s > 3)
    {
        //SaveSetup(_T("Word_SpaceTime"), 1);
        s = 1;
    }

    int nRT = _tstoi(strRepeatTime), i;
    for (i = 0; i < nRT; i++)
    {
        Speak(strText);
        if (strSpell == _T("1"))
        {
            strTmp.Format(_T("<spell>%s</spell>"), CString(strText).MakeUpper());
            Speak(strTmp, SPF_IS_XML | SPF_ASYNC);
            m_cpVoice->SetRate(_tstoi(strSpeed));
            Speak(strText);
        }
        if (strSpeakSample == _T("1"))
        {
            CRichEditUI* pSample = xFindControl(CRichEditUI, _T("wordensampletxt"));
            CString strSample = pSample->GetText().GetData();
            Speak(strSample);
        }
        //Speak(L"<silence msec=\"8000\" />", SPF_IS_XML | SPF_ASYNC);

        strSpace.Format(_T("<silence msec=\"%d\" />"), s * 1000);
        Speak(strSpace, SPF_IS_XML | SPF_ASYNC);
        //Sleep(s * 1000);
    }

    return S_OK;
}

HRESULT MainFrame::WordSpeakCurrentWord()
{
    CString strSpell = ReadSetup(_T("Word_Spell"));
    CString strSpeakSample = ReadSetup(_T("Word_SpeakSample"));
    CString strSpeed = ReadSetup(_T("Word_Speed"));

    CTextUI* pText = xFindControl(CTextUI, _T("wordnametxt"));
    CString strText = pText->GetText().GetData();
    CString strTmp;

    Speak(strText);

    if (strSpell == _T("1"))
    {
        strTmp.Format(_T("<spell>%s</spell>"), CString(strText).MakeUpper());
        Speak(strTmp, SPF_ASYNC | SPF_IS_XML);
        m_cpVoice->SetRate(_tstoi(strSpeed));
        Speak(strText);
    }
    if (strSpeakSample == _T("1"))
    {
        CRichEditUI* pSample = xFindControl(CRichEditUI, _T("wordensampletxt"));
        CString strSample = pSample->GetText().GetData();
        Speak(strSample);
    }

    return S_OK;
}

void MainFrame::InitWordSetupControls()
{
    CComboUI* pCombo;
    CCheckBoxUI* pChk;
    int i;

    CString strRep = ReadSetup(_T("Word_RepeatTime"));
    CString strSpace = ReadSetup(_T("Word_SpaceTime"));
    CString strSpell = ReadSetup(_T("Word_Spell"));
    CString strSpeakSample = ReadSetup(_T("Word_SpeakSample"));
    CString strHideWord = ReadSetup(_T("Word_HideWord"));
    CString strHidePara = ReadSetup(_T("Word_HideParaphrase"));
    //CString strWordSort = ReadSetup(_T("word_sort"));
    
    InitVoiceRateControl(&m_PaintManager, _T("Word_Speaker"), _T("Word_Speed"));

    i = _tstoi(strRep);
    if ( (i < 1) || (i > 3) )
    {
        i = 1;
    }
    pCombo = xFindControl(CComboUI, _T("wordsetuptimescmb"));
    pCombo->SelectItem(i-1);

    i = _tstoi(strSpace);
    if ( (i < 1) || (i > 3) )
    {
        i = 1;
    }
    pCombo = xFindControl(CComboUI, _T("wordsetupintervalcmb"));
    pCombo->SelectItem(i-1);

    pChk = xFindControl(CCheckBoxUI, _T("wordsetupspellchk"));
    pChk->SetCheck(strSpell == _T("1"));

    pChk = xFindControl(CCheckBoxUI, _T("wordsetupspeaksamplechk"));
    pChk->SetCheck(strSpeakSample == _T("1"));

    pChk = xFindControl(CCheckBoxUI, _T("wordsetuphidewordchk"));
    pChk->SetCheck(strHideWord == _T("1"));

    pChk = xFindControl(CCheckBoxUI, _T("wordsetuphideparachk"));
    pChk->SetCheck(strHidePara == _T("1"));

    //i = _tstoi(strWordSort);
    //pCombo = xFindControl(CComboUI, _T("wordsetupsortcmb"));
    //if (pCombo)
    //{
    //    if (i >= pCombo->GetCount())
    //    {
    //        i = 0;
    //    }
    //    pCombo->SelectItem(i);
    //}
}

void MainFrame::WordFollowWordClicked()
{
    CControlUI* pCtl = xFindControl(CControlUI, _T("wordfollowprogvlayout"));
    CProgressUI* pPrg = xFindControl(CProgressUI, _T("wordfollowprg"));
    CTextUI* pTxt = xFindControl(CTextUI, _T("wordnametxt"));
    CRichEditUI* pSym = xFindControl(CRichEditUI, _T("wordsymboltxt"));
    CString strWord = pTxt->GetText();

    //pSymfollow->SetText(_T(""));
    pSym->SetSelNone();
    WordStopAutoPlay();
    if (!strWord.IsEmpty())
    {
        int i = GetSentenceTime(strWord);
        pPrg->SetMaxValue(i);
        pPrg->SetValue(0);
        pCtl->SetVisible(true);
        m_bWordFollowWord = true;
        //StopSpeak();
        StopWaveRecordReco();
        NoticePrompt();
        ::SetTimer(GetHWND(), TIMER_ID_WORD_FOLLOW_WORD, 100, NULL);
        SpeechRecognition_Start();
    }
}

void MainFrame::WordFollowExampleClicked()
{
    CControlUI* pCtl = xFindControl(CControlUI, _T("wordfollowprogvlayout"));
    CProgressUI* pPrg = xFindControl(CProgressUI, _T("wordfollowprg"));
    CRichEditUI* pTxt = xFindControl(CRichEditUI, _T("wordensampletxt"));
    CRichEditUI* pSym = xFindControl(CRichEditUI, _T("wordsymboltxt"));
    CString strWord = pTxt->GetText();

    //pSymfollow->SetText(_T(""));
    pSym->SetSelNone();
    WordStopAutoPlay();
    if (!strWord.IsEmpty())
    {
        int i = GetSentenceTime(strWord);
        pPrg->SetMaxValue(i);
        pPrg->SetValue(0);
        pCtl->SetVisible(true);
        m_bWordFollowWord = false;
        //StopSpeak();
        StopWaveRecordReco();
        NoticePrompt();
        ::SetTimer(GetHWND(), TIMER_ID_WORD_FOLLOW_EXAMPLE, 100, NULL);
        SpeechRecognition_Start();
    }
}

//////////////////////////////////////////////////////////////////////////
//练语法函数

void MainFrame::InitGrammarClassCmb()
{
    CComboUI* pCombo;
    _RecordsetPtr pRecdPtr;
    CListLabelElementUI* pElement;

    pCombo = xFindControl(CComboUI, _T("gramclasscmb"));
    pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 语法类别 Order by Id"));
    pCombo->RemoveAll();
    if (pRecdPtr == NULL)
    {
        return;
    }
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
        pElement->SetUserData(GetCollect(pRecdPtr, _T("Id")));
        pCombo->Add(pElement);
        pRecdPtr->MoveNext();
    }
    pCombo->SelectItem(0);
}

void MainFrame::InitGrammarListCmb()
{
    CComboUI* pComboClass = xFindControl(CComboUI, _T("gramclasscmb"));
    CComboUI* pCombo = xFindControl(CComboUI, _T("gramlistcmb"));
    _RecordsetPtr pRecdPtr;
    CListLabelElementUI* pElement;

    pCombo->RemoveAll();
    if (pComboClass->GetCount() <= 0)
    {
        return;
    }
    pRecdPtr = GetTablesBySql(_T("Select * from 语法 where 类别=%s"),
        pComboClass->GetItemAt(pComboClass->GetCurSel())->GetUserData().GetData());
    if (pRecdPtr == NULL)
    {
        return;
    }
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
        pElement->SetUserData(GetCollect(pRecdPtr, _T("Id")));
        pCombo->Add(pElement);
        pRecdPtr->MoveNext();
    }
    pCombo->SelectItem(0);
}

void MainFrame::InitGrammarContent()
{
    CRichEditUI* pREdit = xFindControl(CRichEditUI, _T("gramcontentedt"));
    CComboUI* pComboClass = xFindControl(CComboUI, _T("gramclasscmb"));
    CComboUI* pCombo = xFindControl(CComboUI, _T("gramlistcmb"));

    pREdit->SetText(_T(""));
    CString strTmp = GetSingleField(_T("Select 内容 from 语法 where Id=%s and 类别=%s"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData(),
        pComboClass->GetItemAt(pComboClass->GetCurSel())->GetUserData().GetData());
    pREdit->SetText(strTmp);
}

void MainFrame::InitGrammarExamSettingCtrls()
{
    CComboUI* pCombo = xFindControl(CComboUI, _T("gramsettingclasscmb"));
    _RecordsetPtr pRecdPtr;
    CListLabelElementUI* pElement;

    pCombo->RemoveAll();
    //pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 语法类别 where 名称<>'句子的种类'"));
    pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 语法类别"));
    if (pRecdPtr == NULL)
    {
        return;
    }
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
        pElement->SetUserData(GetCollect(pRecdPtr, _T("Id")));
        pCombo->Add(pElement);
        pRecdPtr->MoveNext();
    }

    int x = 0;
    CCheckBoxUI* pChk = xFindControl(CCheckBoxUI, _T("gramsettingbaseopt"));
    if (m_bGramBackFromResult)
    {
        x = m_nGramLastClass;
        pChk = xFindControl(CCheckBoxUI, m_nGramLastLvel ? _T("gramsettinghardopt") : _T("gramsettingbaseopt"));
    }
    pCombo->SelectItem(x);
    pChk->SetCheck(true);
}

void MainFrame::InitGrammarExamCtrls()
{
    CString strNum = ReadSetup(_T("GrammarExam_QuestionNumber"));
    CString strQuestionType = ReadSetup(_T("GrammarExam_QuestionType"));
    CString strQuestionContent = ReadSetup(_T("GrammarExam_QuestionContent"));
    CString strSql, strTmp;
    CComboUI* pCombo = xFindControl(CComboUI, _T("gramsettingclasscmb"));
    CCheckBoxUI* pChk = xFindControl(CCheckBoxUI, _T("gramsettingbaseopt"));
    TRACE(_T("难度：%d\n"), pChk->GetCheck() ? 0 : 1);
    int nHard = pChk->GetCheck() ? 0 : 1;

    m_strGramAnswerResult.Empty();

    strSql.Format(_T("Select top %d * FROM 语法习题 where 类别=%s and 难度=%d"),
        strNum,
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData(),
        nHard);
    if (!strQuestionType.IsEmpty())
    {
        strTmp.Format(_T(" and 题型 in (%s)"),
            strQuestionType);
        strSql += strTmp;
    }
    if (!strQuestionContent.IsEmpty())
    {
        strTmp.Format(_T(" and 类别 in (%s)"),
            strQuestionContent);
        strSql += strTmp;
    }
    srand((unsigned int)time(NULL));
    nHard = SQLRand();
    //如果是从语法结果界面返回，保持原来难度和排序不变
    if (m_bGramBackFromResult)
    {
        nHard = m_nGramLastRand;
    }
    else
    {
        m_nGramLastRand = nHard;
    }
    strTmp.Format(_T(" Order by Rnd(-(Id+%d))"), nHard);
    //strSql += strTmp;

    CButtonUI* pNextBtn = xFindControl(CButtonUI, _T("gramexamnextbtn"));
    CButtonUI* pAnswerBtn = xFindControl(CButtonUI, _T("gramexamanswerbtn"));
    CButtonUI* pCancelBtn = xFindControl(CButtonUI, _T("gramexamcancelbtn"));

    CDbFile::CloseRecord(m_pGrammarRecdPtr);
    m_pGrammarRecdPtr = GetTablesBySql(_T("%s"), strSql);
    m_nGrammarQuestions = -1;
    if (m_pGrammarRecdPtr == NULL)
    {
        MyAfxMessageBox(_T("根据您的设置没有找到可供练习的题目。请重新设置。"));
        pNextBtn->SetEnabled(false);
        pAnswerBtn->SetEnabled(false);
        pCancelBtn->SetEnabled(false);
        m_nGrammarQuestionsNumbers = 0;
    }
    else
    {
        m_nGrammarQuestions = 0;
        pNextBtn->SetEnabled(true);
        pAnswerBtn->SetEnabled(true);
        pCancelBtn->SetEnabled(true);
        m_nGrammarQuestionsNumbers = GetCollectCount(m_pGrammarRecdPtr);
    }

    CLabelUI* pAll = xFindControl(CLabelUI, _T("gramexamalltxt"));
    strTmp.Format(_T("%d"), m_nGrammarQuestionsNumbers);
    pAll->SetText(strTmp);

    InitGrammarExamQuestion();
}

void MainFrame::InitGrammarExamQuestion()
{
    CString strTmp;
    CRichEditUI* pREdt;
    CLabelUI* pCount = xFindControl(CLabelUI, _T("gramexamcurrenttxt"));
    strTmp.Format(_T("%d"), m_nGrammarQuestions + 1);
    pCount->SetText(strTmp);

    ASSERT(m_nGrammarQuestions < m_nGrammarQuestionsNumbers);
    if (m_nGrammarQuestions >= 0)
    {
        pREdt = xFindControl(CRichEditUI, _T("gramexamquestionedt"));
        pREdt->SetText(GetCollect(m_pGrammarRecdPtr, _T("题目")));

        if (GetCollect(m_pGrammarRecdPtr, _T("题型")) == _T("0"))
        {
            COptionUI* pOpt;
            pOpt = xFindControl(COptionUI, _T("gramexamanswer1opt"));
            pOpt->SetText(GetCollect(m_pGrammarRecdPtr, _T("选项1")));
            pOpt->Selected(false);
            pOpt->SetEnabled(true);
            pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);

            pOpt = xFindControl(COptionUI, _T("gramexamanswer2opt"));
            pOpt->SetText(GetCollect(m_pGrammarRecdPtr, _T("选项2")));
            pOpt->Selected(false);
            pOpt->SetEnabled(true);
            pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);

            pOpt = xFindControl(COptionUI, _T("gramexamanswer3opt"));
            pOpt->SetText(GetCollect(m_pGrammarRecdPtr, _T("选项3")));
            pOpt->Selected(false);
            pOpt->SetEnabled(true);
            pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);

            pOpt = xFindControl(COptionUI, _T("gramexamanswer4opt"));
            pOpt->SetText(GetCollect(m_pGrammarRecdPtr, _T("选项4")));
            pOpt->Selected(false);
            pOpt->SetEnabled(true);
            pOpt->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
        }
    }

    pREdt = xFindControl(CRichEditUI, _T("gramexamresultedt"));
    pREdt->SetText(_T(""));
}

int MainFrame::IsGrammarOpt(const TNotifyUI& msg)
{
    return IsOption(g_grammar_opt, _countof(g_grammar_opt), msg);
}

void MainFrame::GrammarOptionClicked(int nIndx)
{
    ASSERT(nIndx >= 1 && nIndx <= 4);
    int i;
    COptionUI* pOpt;

    for (i = 0; i < _countof(g_grammar_opt); i++)
    {
        pOpt = xFindControl(COptionUI, g_grammar_opt[i]);
        pOpt->SetEnabled(false);
        //pOpt->SetTextColor(DUI_WHITE);
    }
    
    GrammarCheckAnswer(nIndx);
}

void MainFrame::GrammarCheckAnswer(int nIndx)
{
    ASSERT(nIndx >= 1 && nIndx <= 4);
    //D. not...until为固定搭配，意为“直到...才”。此句意为：直到完成他的家庭作业，他才上床睡觉。
    CString strTmp;
    CString strAnswer;
    CString strTip;
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("gramexamresultedt"));

    strTmp = GetCollect(m_pGrammarRecdPtr, _T("答案"));
    strAnswer = strTmp.Left(1);
    strTip = strTmp.Right(strTmp.GetLength()-2);
    strTip.Trim();

    static LPCTSTR grammar_answer[] =
    {
        _T("A"),
        _T("B"),
        _T("C"),
        _T("D")
    };

    if (grammar_answer[nIndx - 1] == strAnswer)
    {
        strTmp = (_T("回答正确！"));
        COptionUI* pOpt = xFindControl(COptionUI, g_grammar_opt[nIndx-1]);
        //此时处于disabled状态
        pOpt->SetDisabledTextColor(DUI_RGB(255,0,0));
    }
    else
    {
        strTmp.Format(_T("回答错误！正确答案是：%s"),
            strAnswer);
    }
    strTmp += _T("\n");
    strTmp += strTip;
    pREdt->SetText(strTmp);
}

void MainFrame::GrammarNextClicked()
{
    CString strAnswered = _T("");
    CString strTmp;
    CRichEditUI* pREdit = xFindControl(CRichEditUI, _T("gramexamresultedt"));
    COptionUI* pOpt = xFindControl(COptionUI, _T("gramexamanswer1opt"));
    if (pOpt->IsSelected())
    {
        strAnswered = _T("A");
    }
    pOpt->Selected(false);
    pOpt->SetEnabled(true);

    pOpt = xFindControl(COptionUI, _T("gramexamanswer2opt"));
    if (pOpt->IsSelected())
    {
        strAnswered = _T("B");
    }
    pOpt->Selected(false);
    pOpt->SetEnabled(true);

    pOpt = xFindControl(COptionUI, _T("gramexamanswer3opt"));
    if (pOpt->IsSelected())
    {
        strAnswered = _T("C");
    }
    pOpt->Selected(false);
    pOpt->SetEnabled(true);

    pOpt = xFindControl(COptionUI, _T("gramexamanswer4opt"));
    if (pOpt->IsSelected())
    {
        strAnswered = _T("D");
    }
    pOpt->Selected(false);
    pOpt->SetEnabled(true);

    if (strAnswered.IsEmpty())
    {
        pREdit->SetText(_T("请答题后再进入下一题。"));
        return;
    }

    strTmp = pREdit->GetText();
    bool bOK = (strTmp.Find(_T("回答正确")) >= 0);
    strTmp.Format(_T("%s,%d,%s;"),
        GetCollect(m_pGrammarRecdPtr, _T("Id")),
        bOK ? 1 : 0,
        strAnswered);
    m_strGramAnswerResult += strTmp;
    //TRACE(_T("strtmp:%s, all:%s\n"), strTmp, m_strGramAnswerResult);

    if (m_nGrammarQuestions < m_nGrammarQuestionsNumbers - 1)
    {
        m_pGrammarRecdPtr->MoveNext();
        m_nGrammarQuestions++;
        InitGrammarExamQuestion();
    }
    else
    {
        //去掉末尾的;符号
        m_strGramAnswerResult.TrimRight(_T(';'));
        //显示练习结果界面
        OpenChildModalWnd(sGrammarResultSkin);
    }
}

void MainFrame::InitGrammarResultCtrls()
{
    CString strTmp;
    SplitResultVector strResultOne;
    //SplitString返回的是;的个数
    SplitString(TString(m_strGramAnswerResult), _T(";"), m_strGramResultAll);
    int count = m_strGramResultAll.size(), i, j;
    CListUI *pList = xFindControl(CListUI, _T("gramresultlist"));
    CListLabelElementUI *pItem;
    int intTrueQuestion = 0;
    CEditUI* pRat = xFindControl(CEditUI, _T("gramresultpercentedt"));
    CEditUI* pQCount = xFindControl(CEditUI, _T("gramresultalledt"));
    CEditUI* pQTrue = xFindControl(CEditUI, _T("gramresultrightedt"));
    CControlUI* pLbl = xFindControl(CControlUI, _T("answerctl"));

    pList->RemoveAll();
    //LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");
    //Id,1,A
    for (i = 0; i < count; i++)
    {
        j = SplitString(m_strGramResultAll[i].c_str(), _T(","), strResultOne) + 1;
        ASSERT(j == 3);
        strTmp.Format(_T("第%d题  %c"),
            i + 1, (strResultOne[1] == _T("1")) ? _T('√') : _T('×'));

        pItem = new CListLabelElementUI;
        pList->Add(pItem);
        pItem->SetText(strTmp);
        pItem->SetTag(_tstoi(strResultOne[0].c_str()));

        if (strResultOne[1] == _T("1"))
        {
            intTrueQuestion++;
        }
    }
    
    float fRate = intTrueQuestion * 100.0f / count;
    strTmp.Format(_T("%d%%"), (int)fRate);
    pRat->SetText(strTmp);

    strTmp.Format(_T("%d"), count);
    pQCount->SetText(strTmp);

    strTmp.Format(_T("%d"), intTrueQuestion);
    pQTrue->SetText(strTmp);

    strTmp.Format(_T("%s"), (fRate >= 60.0f) ? _T("Image/AnswerRight.png") : _T("Image/AnswerWrong.png"));
    pLbl->SetBkImage(strTmp);

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(0);
    }

    StudyLog(STUDY_TYPE_GRAMMAR, LOG_TYPE_ACHIEV, count, intTrueQuestion);
}

void MainFrame::GrammarResultQuestionDetail()
{
    CString strTmp, strAnswer;
    COptionUI* pOpt;
    CListUI *pList = xFindControl(CListUI, _T("gramresultlist"));
    CRichEditUI* pREdit = xFindControl(CRichEditUI, _T("gramresultedt"));
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("gramresultanswertxt"));
    int nIndx = pList->GetCurSel();

    ASSERT(nIndx < (int)m_strGramResultAll.size());

    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 语法习题 where Id=%d"),
        pList->GetItemAt(nIndx)->GetTag());
    if (pRecdPtr == NULL)
    {
        return;
    }

    pREdit->SetText(GetCollect(pRecdPtr, _T("题目")));

    //题型。。。
    //GetCollect(pRecdPtr, _T("题型"));

    //答案
    strTmp = GetCollect(pRecdPtr, _T("答案"));
    strAnswer = strTmp.Left(1);

    pOpt = xFindControl(COptionUI, _T("gramresultanswer1opt"));
    pOpt->SetText(GetCollect(pRecdPtr, _T("选项1")));
    pOpt->Selected(false);
    if (strAnswer == _T("A"))
    {
        pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->Selected(true);
    }
    else
    {
        pOpt->SetTextColor(DUI_WHITE);
    }

    pOpt = xFindControl(COptionUI, _T("gramresultanswer2opt"));
    pOpt->SetText(GetCollect(pRecdPtr, _T("选项2")));
    pOpt->Selected(false);
    if (strAnswer == _T("B"))
    {
        pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->Selected(true);
    }
    else
    {
        pOpt->SetTextColor(DUI_WHITE);
    }

    pOpt = xFindControl(COptionUI, _T("gramresultanswer3opt"));
    pOpt->SetText(GetCollect(pRecdPtr, _T("选项3")));
    pOpt->Selected(false);
    if (strAnswer == _T("C"))
    {
        pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->Selected(true);
    }
    else
    {
        pOpt->SetTextColor(DUI_WHITE);
    }

    pOpt = xFindControl(COptionUI, _T("gramresultanswer4opt"));
    pOpt->SetText(GetCollect(pRecdPtr, _T("选项4")));
    pOpt->Selected(false);
    if (strAnswer == _T("D"))
    {
        pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->Selected(true);
    }
    else
    {
        pOpt->SetTextColor(DUI_WHITE);
    }

    SplitResultVector strResultOne;
    SplitString(m_strGramResultAll[nIndx], _T(","), strResultOne);
    strTmp.Format(_T("您的答案是：%s"),
        strResultOne[2].c_str());
    pLbl->SetText(strTmp);
}

//////////////////////////////////////////////////////////////////////////
//成绩报告

//和g_ReportDetailBtns对应
static COLORREF g_col_color[] =
{
    //单词颜色
    RGB(255,0,0),
    //阅读,
    RGB(0,255,0),
    //口语,
    RGB(0,0,255),
    //听力,
    RGB(255,255,0),
    //语法,
    RGB(255,0,255)
};

static LPCTSTR g_ReportItemNames[] =
{
    _T("单词"),
    _T("阅读"),
    _T("口语"),
    _T("听力"),
    _T("语法")
};

static LPCTSTR g_ReportResultNames[] =
{
    _T("单词学习成绩"),
    _T("阅读学习成绩"),
    _T("口语学习成绩"),
    _T("听力学习成绩"),
    _T("语法学习成绩")
};

static LPCTSTR g_ReportTimeNames[] =
{
    _T("单词学习时间"),
    _T("阅读学习时间"),
    _T("口语学习时间"),
    _T("听力学习时间"),
    _T("语法学习时间")
};

static LPCTSTR g_ReportTrueNames[] =
{
    _T("单词准确数"),
    _T("阅读准确数"),
    _T("口语准确数"),
    _T("听力准确数"),
    _T("语法准确数")
};

static LPCTSTR g_ReportCountNames[] =
{
    _T("单词答题数"),
    _T("阅读答题数"),
    _T("口语答题数"),
    _T("听力答题数"),
    _T("语法答题数")
};

void MainFrame::InitReportMsChart(CMSChart* pChart, bool bIsTime)
{
    ASSERT(pChart);

    try
    {
        pChart->SetChartType(VtChChartType2dBar);
        pChart->SetAllowSelections(FALSE);
        pChart->SetRandomFill(FALSE);
        pChart->SetDrawMode(VtChDrawModeBlit);

        // 改变背景色
        //m_Chart.GetBackdrop().GetFill().SetStyle(VtFillStyleBrush);
        //m_Chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(GetRValue(m_rgbBkColor), GetGValue(m_rgbBkColor), GetBValue(m_rgbBkColor));

        VARIANT var;
        CVcAxis& cAxis = pChart->GetPlot().GetAxis(VtChAxisIdY, var);
        //CVcValueScale& cvScale = cAxis.GetValueScale();
        //cvScale.SetAuto(FALSE); // 不自动标注Y轴刻度
        cAxis.GetAxisScale().SetHide(TRUE);
        //隐藏第2个Y轴
        pChart->GetPlot().GetAxis(VtChAxisIdY2, var).GetAxisScale().SetHide(TRUE);

        pChart->SetColumnCount(5);
        pChart->SetRowCount(3);

        int i, j;
        for(i = 1; i <= 5; i++)
        {
            pChart->SetColumn(i);
            pChart->SetColumnLabel(bIsTime ? g_ReportTimeNames[i-1] : g_ReportResultNames[i-1]);

            CVcSeries& vItem = pChart->GetPlot().GetSeriesCollection().GetItem(i);
            vItem.Select();
            //CVcPen& vPen = vItem.GetPen();
            //// 线色
            //vPen.GetVtColor().Set(GetRValue(g_col_color[i-1]), GetGValue(g_col_color[i-1]), GetBValue(g_col_color[i-1]));

            CVcDataPoint& vDatPoint = vItem.GetDataPoints().GetItem(-1);
            vDatPoint.GetBrush().GetFillColor().Set(GetRValue(g_col_color[i-1]), GetGValue(g_col_color[i-1]), GetBValue(g_col_color[i-1]));

            for (j = 1; j <=3; j++)
            {
                pChart->SetRow(j);
                pChart->SetRowLabel(_T(""));
                pChart->SetData(_T(""));
            }
        }
    }
    catch (CException* e)
    {
        e->ReportError();
    }
}

void MainFrame::InitReportCtrls()
{
    int i, j, count;
    CMyWndUI* pTimeX = xFindControl(CMyWndUI, _T("reporttimeatx"));
    CMyWndUI* pArchvX = xFindControl(CMyWndUI, _T("reportarchvatx"));
    _RecordsetPtr pRecdPtr;
    CButtonUI* pBtn;
    CMSChart *pTimeChart = pTimeX->GetMSChart();
    CMSChart *pArchvChart = pArchvX->GetMSChart();

    if (!pTimeChart || !pArchvChart)
    {
        MyAfxMessageBox(_T("%s"), _T("创建MsChart控件失败，请确认控件已正确注册。"));
        TNotifyUI msg;
        msg.pSender = xFindControl(CButtonUI, kReturnButtonName);
        msg.sType = DUI_MSGTYPE_CLICK;
        m_PaintManager.SendNotify(msg);
        TRACE(_T("MsChart控件为NULL。\n"));
        return;
    }
    InitReportMsChart(pTimeChart, true);
    InitReportMsChart(pArchvChart, false);

    pRecdPtr = StudyGetTablesBySql(_T("%s"), _T("Select top 3 日期, 单词学习时间, 阅读学习时间, 口语学习时间, 听力学习时间, 语法学习时间 from 成绩记录 Order by 日期 desc"));
    if (!pRecdPtr)
    {
        return;
    }
    //GetCollectCount有可能大于3，比如两个相同的日期?
    count = min(3, GetCollectCount(pRecdPtr));

    for (i = 1; i <= 5; i++)
    {
        pTimeChart->SetColumn(i);
        //pTimeChart->SetColumnLabel(GetColumnName(pRecdPtr, i));
        if(!pRecdPtr->GetBOF())
            pRecdPtr->MoveFirst();
        for (j = 1; j <= count; j++)
        {
            pTimeChart->SetRow(j);
            pTimeChart->SetRowLabel(GetCollect(pRecdPtr, (long)0));
            pTimeChart->SetData(GetCollect(pRecdPtr, i));
            pRecdPtr->MoveNext();
        }
    }
    CDbFile::CloseRecord(pRecdPtr);

    pRecdPtr = StudyGetTablesBySql(_T("%s"), _T("Select top 3 日期, 单词学习成绩, 阅读学习成绩, 口语学习成绩, 听力学习成绩, 语法学习成绩 from 成绩记录 Order by 日期 desc"));
    if (!pRecdPtr)
    {
        return;
    }
    count = min(3, GetCollectCount(pRecdPtr));
    
    pBtn = xFindControl(CButtonUI, _T("reportdancitxt"));
    pBtn->SetText(GetCollect(pRecdPtr, _T("单词学习成绩")));
    pBtn = xFindControl(CButtonUI, _T("reportyuedutxt"));
    pBtn->SetText(GetCollect(pRecdPtr, _T("阅读学习成绩")));
    pBtn = xFindControl(CButtonUI, _T("reportkouyutxt"));
    pBtn->SetText(GetCollect(pRecdPtr, _T("口语学习成绩")));
    pBtn = xFindControl(CButtonUI, _T("reporttinglitxt"));
    pBtn->SetText(GetCollect(pRecdPtr, _T("听力学习成绩")));
    pBtn = xFindControl(CButtonUI, _T("reportyufatxt"));
    pBtn->SetText(GetCollect(pRecdPtr, _T("语法学习成绩")));

    for (i = 1; i <= 5; i++)
    {
        pArchvChart->SetColumn(i);
        //pArchvChart->SetColumnLabel(GetColumnName(pRecdPtr, i));
        if(!pRecdPtr->GetBOF())
            pRecdPtr->MoveFirst();
        for (j = 1; j <= count; j++)
        {
            pArchvChart->SetRow(j);
            pArchvChart->SetRowLabel(GetCollect(pRecdPtr, (long)0));
            pArchvChart->SetData(GetCollect(pRecdPtr, i));
            pRecdPtr->MoveNext();
        }
    }
}

void MainFrame::InitReportDetailCtrls()
{
    int i, nIdx = m_nReportDetailItem - 1, count;
    CMyWndUI* pArchvX = xFindControl(CMyWndUI, _T("reportdetailatx"));
    _RecordsetPtr pRecdPtr;
    CLabelUI* pLbl;
    CMSChart *pArchvChart = pArchvX->GetMSChart();

    if (!pArchvChart)
    {
        return;
    }

    try
    {
        pArchvChart->SetChartType(VtChChartType2dBar);
        pArchvChart->SetAllowSelections(FALSE);
        pArchvChart->SetRandomFill(FALSE);
        pArchvChart->SetDrawMode(VtChDrawModeBlit);

        pArchvChart->SetColumnCount(1);
        pArchvChart->SetRowCount(7);

        pArchvChart->SetColumn(1);
        pArchvChart->SetColumnLabel(g_ReportResultNames[nIdx]);
        for (i = 1; i <= 7; i++)
        {
            pArchvChart->SetRow(i);
            pArchvChart->SetRowLabel(_T(""));
            pArchvChart->SetData(_T(""));
        }

        CVcSeries& vItem = pArchvChart->GetPlot().GetSeriesCollection().GetItem(1);
        //CVcPen& vPen = vItem.GetPen();
        //// 线色
        //vPen.GetVtColor().Set(GetRValue(g_col_color[nIdx]), GetGValue(g_col_color[nIdx]), GetBValue(g_col_color[nIdx]));
        CVcDataPoint& vDatPoint = vItem.GetDataPoints().GetItem(-1);
        vDatPoint.GetBrush().GetFillColor().Set(GetRValue(g_col_color[nIdx]), GetGValue(g_col_color[nIdx]), GetBValue(g_col_color[nIdx]));

        pRecdPtr = StudyGetTablesBySql(_T("Select top 7 日期, %s from 成绩记录 Order by 日期 desc"),
            g_ReportResultNames[nIdx]);
        if (!pRecdPtr)
        {
            return;
        }

        count = min(GetCollectCount(pRecdPtr), 7);
        
        pArchvChart->SetColumn(1);
        //pArchvChart->SetColumnLabel(GetColumnName(pRecdPtr, 1));
        for(i = 1; i <= count; i++)
        {
            pArchvChart->SetRow(i);
            pArchvChart->SetRowLabel(GetCollect(pRecdPtr, (long)0));
            pArchvChart->SetData(GetCollect(pRecdPtr, (long)1));

            pRecdPtr->MoveNext();
        }
        CDbFile::CloseRecord(pRecdPtr);

        pRecdPtr = StudyGetTablesBySql(_T("%s"), _T("Select top 1 * from 成绩记录 Order by 日期 desc"));
        if (!pRecdPtr)
        {
            return;
        }
        CString strItem, strTmp;
        //时间，题数，成绩，准确数
        CString strInfo[4];
        strItem = g_ReportItemNames[nIdx];
        strInfo[0] = GetCollect(pRecdPtr, g_ReportTimeNames[nIdx]);
        strInfo[1] = GetCollect(pRecdPtr, g_ReportCountNames[nIdx]);
        strInfo[2] = GetCollect(pRecdPtr, g_ReportResultNames[nIdx]);
        strInfo[3] = GetCollect(pRecdPtr, g_ReportTrueNames[nIdx]);

        pLbl = xFindControl(CLabelUI, _T("reportdetailitemtxt"));
        strTmp.Format(_T("详细信息  > %s"),
            strItem);
        pLbl->SetText(strTmp);
        //时间，题数，成绩，准确数
        pLbl = xFindControl(CLabelUI, _T("reportdetailtimetxt"));
        strTmp.Format(_T("学习时间（分钟）：%s"),
            strInfo[0]);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("reportdetailnumbertxt"));
        strTmp.Format(_T("答题数：%s"),
            strInfo[1]);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("reportdetailscoretxt"));
        strTmp.Format(_T("成绩：%s"),
            strInfo[2]);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("reportdetailrighttxt"));
        strTmp.Format(_T("正确数：%s"),
            strInfo[3]);
        pLbl->SetText(strTmp);
    }
    catch (CException* e)
    {
        e->ReportError();
    }
}

//////////////////////////////////////////////////////////////////////////
//娱乐函数

static LPCTSTR g_Joy_db_name[] =
{
    _T("音乐"),
    _T("视频"),
    _T("游戏")
};

void MainFrame::InitJoyList(int nIdx)
{
    CListLabelElementUI* pItem;
    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from %s"), g_Joy_db_name[nIdx]);

    pList->RemoveAll();
    // 暂时禁止游戏 [2016-1-25 Arthur]
    //if (2 == nIdx)
    //{
    //    CDbFile::CloseRecord(pRecdPtr);
    //    pRecdPtr = NULL;
    //}
    //LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");
    LPCTSTR strAttr = _T("height=\"26\"");
    if (pRecdPtr != NULL)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            //指定高度和宽度，能够提高效率
            pItem->ApplyAttributeList(strAttr);
            pItem->SetTag(_tstoi(GetCollect(pRecdPtr, _T("Id"))));
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            pList->Add(pItem);

            pRecdPtr->MoveNext();
        }
    }

    //CScrollBarUI* pBar = pList->GetHorizontalScrollBar();
    //TRACE(_T("HorizontalScrollBar visible:%s\n"), pBar->IsVisible() ? _T("true") : _T("false"));
    //pBar->SetVisible(true);

    if (pList->GetCount() <= 0)
    {
        CButtonUI* pButton;
        pButton = xFindControl(CButtonUI, _T("joystartbtn"));
        pButton->SetEnabled(false);
        pButton = xFindControl(CButtonUI, _T("joyendbtn"));
        pButton->SetEnabled(false);
        pButton = xFindControl(CButtonUI, _T("joystopbtn"));
        pButton->SetEnabled(false);
        pButton = xFindControl(CButtonUI, _T("joyplaybtn"));
        pButton->SetEnabled(false);
    }
}

void MainFrame::InitJoyCtrls(UINT nIdx)
{
    ASSERT(nIdx < 3);
    
    m_nJoyType = nIdx;

    ::SetWindowText(GetHWND(), g_Joy_db_name[nIdx]);

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("joysonglisttxt"));
    CHorizontalLayoutUI* pCtrl;
    CButtonUI* pButton;
    bool bEnabled;
    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    CSliderUI* pSlider = xFindControl(CSliderUI, _T("joyvolumn"));

    static bool b_enabled[] =
    {
        true,
        true,
        false
    };
    static LPCTSTR lbl_name[] =
    {
        _T("歌曲列表"),
        _T("视频列表"),
        _T("游戏列表")
    };

    if (m_nJoyType == JOY_TYPE_SONG)
    {
        //m_pJoyWMPCur = &m_JoyWMPlayer4;
        CMyWndUI* pp = xFindControl(CMyWndUI, _T("joyvedioatx"));
        ASSERT(pp && pp->GetMSWMP());
        m_pJoyWMPCur = pp->GetMSWMP();
        if (NULL == m_pJoyWMPCur)
        {
            MyAfxMessageBox(_T("%s"), _T("创建视频播放接口失败，请确认Windows Media Player控件已正确注册。"));
            TNotifyUI msg;
            msg.pSender = xFindControl(CButtonUI, kCloseButtonName);
            msg.sType = DUI_MSGTYPE_CLICK;
            m_PaintManager.SendNotify(msg);
            return;
        }

        m_pJoyWMPCur->put_uiMode(_T("none"));

        m_JoyWMPCurCtl.ReleaseDispatch();
        m_JoyWMPCurCtl = m_pJoyWMPCur->get_controls();

        m_JoyWMPCurSettings.ReleaseDispatch();
        m_JoyWMPCurSettings = m_pJoyWMPCur->get_settings();

        pSlider->SetValue(m_JoyWMPCurSettings.get_volume());
        //m_lJoyVolumn = m_JoyWMPSettings.get_volume();

    }
    else if (m_nJoyType == JOY_TYPE_VEDIO)
    {
        CMyWndUI* pp = xFindControl(CMyWndUI, _T("joyvedioatx"));
        ASSERT(pp && pp->GetMSWMP());
        m_pJoyWMPCur = pp->GetMSWMP();
        if (NULL == m_pJoyWMPCur)
        {
            MyAfxMessageBox(_T("%s"), _T("创建视频播放接口失败，请确认Windows Media Player控件已正确注册。"));
            TNotifyUI msg;
            msg.pSender = xFindControl(CButtonUI, kCloseButtonName);
            msg.sType = DUI_MSGTYPE_CLICK;
            m_PaintManager.SendNotify(msg);
            return;
        }

        m_JoyWMPCurSettings.ReleaseDispatch();
        m_JoyWMPCurSettings = m_pJoyWMPCur->get_settings();
        m_JoyWMPCurCtl.ReleaseDispatch();
        m_JoyWMPCurCtl = m_pJoyWMPCur->get_controls();

        pSlider->SetValue(m_JoyWMPCurSettings.get_volume());
    }

    pButton = xFindControl(CButtonUI, _T("joyopenbtn"));
    pButton->SetText(m_nJoyType == JOY_TYPE_GAME ? _T("开始游戏") : _T("打开"));

    pLbl->SetText(lbl_name[m_nJoyType]);

    bEnabled = b_enabled[m_nJoyType];
    pCtrl = xFindControl(CHorizontalLayoutUI, _T("joyplaycontrol"));
    pCtrl->SetVisible(bEnabled);
    pButton = xFindControl(CButtonUI, _T("joyaddbtn"));
    pButton->SetVisible(bEnabled);
    pButton = xFindControl(CButtonUI, _T("joydelbtn"));
    pButton->SetVisible(bEnabled);

    InitJoyList(m_nJoyType);

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(0);
    }
    else
    {
        //pButton = xFindControl(CButtonUI, _T("joystartbtn"));
        //pButton->SetEnabled(false);
        //pButton = xFindControl(CButtonUI, _T("joyendbtn"));
        //pButton->SetEnabled(false);
        //pButton = xFindControl(CButtonUI, _T("joystopbtn"));
        //pButton->SetEnabled(false);
        //pButton = xFindControl(CButtonUI, _T("joyplaybtn"));
        //pButton->SetEnabled(false);
    }
}

//去掉歌词中，以[]包围的字符
//Regex.Replace(strLrcText, "\[.*\]", "")
//并去掉前面的\r\n字符
static
CString LRC_replace(CString& strLrcText)
{
    int l, r;

    //[]必须配对
    l = strLrcText.Find(_T('['));
    if (l < 0)
    {
        strLrcText.TrimLeft(_T("\r\n"));
        return strLrcText.TrimRight();
    }
    //有可能在[前面,从[后面开始查找
    r = strLrcText.Find(_T(']'), l);
    if (r > l)
    {
        strLrcText.Delete(l, r-l+1);
        return LRC_replace(strLrcText.TrimLeft(_T("\r\n")));
    }
    strLrcText.TrimLeft(_T("\r\n"));
    return strLrcText.TrimRight();
}

void MainFrame::JoyPlaySong(CString& strPath)
{
    CString strTmp;
    CLabelUI* pLblStatus = xFindControl(CLabelUI, _T("joyplaystatustxt"));
    CLabelUI* pLblProg = xFindControl(CLabelUI, _T("joyplayprogtxt"));

    CFileFind finder;
    if (finder.FindFile(strPath))
    {
        finder.Close();
        m_JoyWMPCurCtl.stop();

        JoyPaneStop();

        CWMPMedia nmedia = (CWMPMedia)m_pJoyWMPCur->newMedia(strPath);
        m_strJoyDuration = nmedia.get_durationString();
        m_dbJoyDuration = nmedia.get_duration();
        strTmp.Format(_T("%s / %s"),
            _T("00:00"), m_strJoyDuration);
        pLblProg->SetText(strTmp);
        pLblStatus->SetText(_T("正在播放"));

        CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("joysonglyricedt"));
        pREdt->SetText(_T(""));
        if (m_nJoyType == JOY_TYPE_SONG) //歌曲
        {
            strTmp = strPath.Left(strPath.GetLength()-4);
            strTmp += _T(".lrc");
            if (finder.FindFile(strTmp))
            {
                finder.Close();
                CString tbuf;
                MyReadAsciiFile(strTmp, tbuf);
                pREdt->SetText(LRC_replace(tbuf));
                SIZE sz;
                sz.cx = 0;
                sz.cy = 0;
                pREdt->SetScrollPos(sz);
            }
        }

        m_pJoyWMPCur->put_URL(strPath);
        CCheckBoxUI* pChk = xFindControl(CCheckBoxUI, _T("joyvoicechk"));
        m_JoyWMPCurSettings.put_mute(pChk->GetCheck() ? TRUE : FALSE);
        m_JoyWMPCurCtl.play();

        ::SetTimer(GetHWND(), TIMER_ID_JOY_SONG, 1000, NULL);
        JoyPanePause();
    }
}

void MainFrame::InitJoyContent()
{
    ASSERT(m_pJoyWMPCur);

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("joysongnametxt"));
    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    CString strLabel, strPath, strTmp;
    int s = pList->GetCurSel();
    CButtonUI* pStartBtn = xFindControl(CButtonUI, _T("joystartbtn"));
    CButtonUI* pEndBtn = xFindControl(CButtonUI, _T("joyendbtn"));
    //CButtonUI* pPlayBtn = xFindControl(CButtonUI, _T("joyplaybtn"));

    //never!
    ASSERT(pList->GetCount() > 0);
    //if (pList->GetCount() <= 0)
    //{
    //    pPlayBtn->SetEnabled(false);
    //    pEndBtn->SetEnabled(false);
    //    pStartBtn->SetEnabled(false);
    //}
    pEndBtn->SetEnabled(true);
    pStartBtn->SetEnabled(true);
    if (s == pList->GetCount() - 1)
    {
        pEndBtn->SetEnabled(false);
    }
    if (s == 0)
    {
        pStartBtn->SetEnabled(false);
    }

    switch (m_nJoyType)
    {
    case JOY_TYPE_SONG:
        {
            strLabel.Format(_T("歌曲名称：%s"),
                pList->GetItemAt(s)->GetText().GetData());

            strTmp = GetSingleField(_T("Select 路径 from 音乐 where Id=%d"),
                pList->GetItemAt(s)->GetTag());
            if (strTmp.Find(_T("\\")) < 0)
            {
                strPath.Format(_T("%sData\\Music\\%s"),
                    CPaintManagerUI::GetInstancePath().GetData(),
                    (LPCTSTR)strTmp);
            }
            else
            {
                strPath = strTmp;
            }
            JoyPlaySong(strPath);
        }
    	break;
    case JOY_TYPE_VEDIO:
        {
            strLabel.Format(_T("视频播放：%s"),
                pList->GetItemAt(s)->GetText().GetData());

            strTmp = GetSingleField(_T("Select 路径 from 视频 where Id=%d"),
                pList->GetItemAt(s)->GetTag());
            if (strTmp.Find(_T("\\")) < 0)
            {
                strPath.Format(_T("%sData\\Video\\%s"),
                    CPaintManagerUI::GetInstancePath().GetData(),
                    (LPCTSTR)strTmp);
            }
            else
            {
                strPath = strTmp;
            }
            JoyPlaySong(strPath);
        }
        break;
    case JOY_TYPE_GAME:
        {
            strLabel.Format(_T("当前游戏：%s"),
                pList->GetItemAt(s)->GetText().GetData());

            _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 游戏 where Id=%d"),
                pList->GetItemAt(s)->GetTag());
            
            CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("joygameedt"));
            pREdt->SetText(GetCollect(pRecdPtr, _T("介绍")));

            CControlUI* pCtrl = xFindControl(CControlUI, _T("joygamepic"));
            pCtrl->SetVisible(false);
            strPath.Format(_T("%sData\\Game\\%s"),
                CPaintManagerUI::GetInstancePath().GetData(),
                (LPCTSTR)GetCollect(pRecdPtr, _T("图片")));
            CFileFind finder;
            if (finder.FindFile(strPath))
            {
                finder.Close();
                pCtrl->SetBkImage(strPath);
                pCtrl->SetVisible(true);
            }
        }
        break;
    default:
        return;
    }

    pLbl->SetText(strLabel);
}

void MainFrame::JoyPanePause()
{
    CButtonUI* pPlay = xFindControl(CButtonUI, _T("joyplaybtn"));
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("joystopbtn"));

    pPlay->ApplyAttributeList(_T("normalimage=\"Image/playpause.png\" hotimage=\"Image/playpause.png\""));
    //pPlay->SetAttribute(_T("normalimage"), _T("Image/playpause.png"));
    pBtn->SetEnabled(true);
}

void MainFrame::JoyPanePlay()
{
    CButtonUI* pPlay = xFindControl(CButtonUI, _T("joyplaybtn"));
    pPlay->ApplyAttributeList(_T("normalimage=\"file='Image/playon.png' dest='0,0,13,16'\" hotimage=\"file='Image/playon.png' dest='0,0,13,16'\""));
    //pPlay->SetAttribute(_T("normalimage"), _T("file='Image/playon.png' dest='0,0,13,16'"));
}

void MainFrame::JoyPaneStop()
{
    CButtonUI* pPlay = xFindControl(CButtonUI, _T("joyplaybtn"));
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("joystopbtn"));
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("joyplaystatustxt"));
    CLabelUI* pLblProg = xFindControl(CLabelUI, _T("joyplayprogtxt"));

    pPlay->ApplyAttributeList(_T("normalimage=\"file='Image/playon.png' dest='0,0,13,16'\" hotimage=\"file='Image/playon.png' dest='0,0,13,16'\""));
//    pPlay->SetAttribute(_T("normalimage"), _T("file='Image/playon.png' dest='0,0,13,16'"));
    pBtn->SetEnabled(false);
    pLbl->SetText(_T(""));
    pLblProg->SetText(_T(""));
}

void MainFrame::JoyPlayClicked()
{
    //CButtonUI* pPlay = xFindControl(CButtonUI, _T("joyplaybtn"));
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("joyplaystatustxt"));
//    TRACE(_T("m_pJoyWMPPlayer->get_playState()=%d\n"), 
//        m_pJoyWMPSong->get_playState());
    if (m_pJoyWMPCur->get_playState() == wmppsPlaying)
    {
        m_JoyWMPCurCtl.pause();
        //暂停
        pLbl->SetText(_T("暂停"));
        JoyPanePlay();
    }
    else
    {
        m_JoyWMPCurCtl.play();
        //正在播放
        pLbl->SetText(_T("正在播放"));
        JoyPanePause();
    }
}

void MainFrame::JoyStopClicked()
{
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("joyplaystatustxt"));
    CLabelUI* pLblProg = xFindControl(CLabelUI, _T("joyplayprogtxt"));
    CSliderUI* pPrg = xFindControl(CSliderUI, _T("joyprogress"));
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("joystopbtn"));

    m_JoyWMPCurCtl.stop();
    pLbl->SetText(_T(""));
    pLblProg->SetText(_T(""));
    pPrg->SetValue(0);
    pBtn->SetEnabled(false);

    JoyPanePlay();
    //::KillTimer(GetHWND(), TIMER_ID_JOY_SONG);
}

void MainFrame::JoyStartClicked()
{
    int s;
    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    if (pList->GetCount() <= 0)
    {
        return;
    }

    s = pList->GetCurSel();
    if (s > 0)
    {
        s--;
        pList->SelectItem(s);
    }
}

void MainFrame::JoyEndClicked()
{
    int s;
    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    if (pList->GetCount() <= 0)
    {
        return;
    }

    s = pList->GetCurSel();
    if (s < pList->GetCount() - 1)
    {
        s++;
        pList->SelectItem(s);
    }
}

void MainFrame::JoyOpenGame()
{
    CString strPath;
    CListUI* pList = xFindControl(CListUI,  _T("joysonglist"));

    if (pList->GetCount() <= 0)
    {
        return;
    }
    strPath.Format(_T("%sData\\Game\\%s"),
        CPaintManagerUI::GetInstancePath().GetData(),
        GetSingleField(_T("Select 文件 from 游戏 where Id=%d"),
            pList->GetItemAt(pList->GetCurSel())->GetTag()));

    CFileFind finder;
    if (finder.FindFile(strPath))
    {
        finder.Close();
        
        CString strlame = CPaintManagerUI::GetInstancePath() + _T("FlashPlayer.exe");
        CString strParam;
        strParam.Format(_T("\"%s\""),
            strPath);
        HINSTANCE hRet = ::ShellExecute(NULL, _T("open"), strlame, strParam, NULL, SW_SHOW);
        TRACE(_T("ShellExecute return:%d\n"), hRet);
    }
    else
    {
        MyAfxMessageBox(_T("指定的游戏不存在。"));
    }
}

void MainFrame::JoyOpenClicked()
{
    if (m_nJoyType == JOY_TYPE_GAME)
    {
        JoyOpenGame();
        return;
    }
    JoyhlpAddSongs(true);
}

void MainFrame::JoyhlpAddSongs(bool bIsOpen)
{
    ASSERT( (m_nJoyType == JOY_TYPE_SONG) ||
        (m_nJoyType == JOY_TYPE_VEDIO) );
    CString strPath;
    CString strTitle, strAuthor, strName;
    DWORD dwFlags = OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON |
        OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    static LPCTSTR joy_filter[]=
    {
        _T("mp3文件(*.mp3)|*.mp3||"),
        _T("媒体文件(*.asf;*.wma;*.wmv;*.avi;*.mpeg;*.mpg)|*.asf;*.wma;*.wmv;*.avi;*.mpeg;*.mpg||")
    };
    CFileDialog cfg(TRUE, NULL, NULL, dwFlags, joy_filter[m_nJoyType], CWnd::FromHandle(GetHWND()));
    OPENFILENAME& ofn = cfg.GetOFN();
    TCHAR tbuf[4096 * 20];
    tbuf[0] = _T('\0');
    ofn.lpstrFile = tbuf;
    ofn.nMaxFile = _countof(tbuf);
    ofn.hwndOwner = GetHWND();

    if (cfg.DoModal() != IDOK)
    {
        return;
    }

    if (bIsOpen)
    {
        if (AfxMessageBox(_T("添加新内容将会删除已有的旧内容，继续吗？"),
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
        {
            return;
        }
        TRACE(_T("Delete from %s\n"), g_Joy_db_name[m_nJoyType]);
        ExecuteSql(_T("Delete from %s"), g_Joy_db_name[m_nJoyType]);
    }

    POSITION pos = cfg.GetStartPosition();
    while(pos != NULL)
    {
        strPath = cfg.GetNextPathName(pos);
        CWMPMedia nm = static_cast<CWMPMedia>(m_pJoyWMPCur->newMedia(strPath));
        strTitle = nm.getItemInfo(_T("Title"));
        strAuthor = nm.getItemInfo(_T("Author"));
        if (!strAuthor.IsEmpty())
        {
            if (!strTitle.IsEmpty())
            {
                strName.Format(_T("%s - %s"), strAuthor, strTitle);
            }
            else
            {
                strName = strAuthor;
            }
        }
        else if (!strTitle.IsEmpty())
        {
            strName = strTitle;
        }
        else
        {
            strName = strPath;
            int x = strPath.ReverseFind(_T('\\'));
            if (x > 0)
            {
                strName = strPath.Right(strPath.GetLength() - x);
            }
        }

        TRACE(_T("Insert into %s(名称, 路径) Values('%s', '%s')\n"),
            g_Joy_db_name[m_nJoyType], strName, strPath);
        ExecuteSql(_T("Insert into %s(名称, 路径) Values('%s', '%s')"),
            g_Joy_db_name[m_nJoyType], strName, strPath);
    }
    InitJoyList(m_nJoyType);
}

void MainFrame::JoyAddClicked()
{
    JoyhlpAddSongs(false);
}

void MainFrame::JoyDelClicked()
{
    ASSERT( (m_nJoyType == JOY_TYPE_SONG) ||
        (m_nJoyType == JOY_TYPE_VEDIO) );

    CListUI* pList = xFindControl(CListUI, _T("joysonglist"));
    if (pList->GetCount() <= 0)
    {
        return;
    }

    if (pList->GetCurSel() < 0)
    {
        return;
    }
    ExecuteSql(_T("Delete from %s where Id=%d"),
        g_Joy_db_name[m_nJoyType], pList->GetItemAt(pList->GetCurSel())->GetTag());

    InitJoyList(m_nJoyType);
}

void MainFrame::InitWordExamQuestion()
{
    if (m_nWordExamQuestions >= m_nWordExamQuestionsNumbers)
    {
        m_strWordExamAnswerResult.TrimRight(_T(';'));
        OpenChildModalWnd(sWordExamResultSkin);
        return;
    }
    CString strTmp;

    CString strwordtype = ReadSetup(_T("word_type"));
    CString strnewbook = ReadSetup(_T("word_nwb"));
    CString strGId = ReadSetup(_T("word_grade"));
    CString strBId = ReadSetup(_T("word_book"));
    CString strUId = ReadSetup(_T("word_unit"));

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("wordexamcurrenttxt"));
    strTmp.Format(_T("共%d道题，当前第%d道题。"),
        m_nWordExamQuestionsNumbers, m_nWordExamQuestions+1);
    pLbl->SetText(strTmp);

    pLbl = xFindControl(CLabelUI, _T("wordexamanswertxt"));
    pLbl->SetText(_T(""));

    CHorizontalLayoutUI *pWordHLay, *pSpeakHLay;
    pWordHLay = xFindControl(CHorizontalLayoutUI, _T("wordexamwordhlayout"));
    pWordHLay->SetVisible(false);

    pSpeakHLay = xFindControl(CHorizontalLayoutUI, _T("wordspeakhlayout"));
    pSpeakHLay->SetVisible(false);

    CVerticalLayoutUI* pSelectVlay = xFindControl(CVerticalLayoutUI, _T("wordexamselectvlayout"));
    pSelectVlay->SetVisible(false);

    CVerticalLayoutUI* pTextVlay = xFindControl(CVerticalLayoutUI, _T("wordexamtextvlayout"));
    pTextVlay->SetVisible(false);

    CEditUI* pEdt = xFindControl(CEditUI, _T("wordexamansweredt"));
    pEdt->SetText(_T(""));

    CRichEditUI* pQuesLbl = xFindControl(CRichEditUI, _T("wordexamwordtxt"));
    pQuesLbl->SetFont(0);
    COptionUI* pOpt1 = xFindControl(COptionUI, _T("wordexamanwer1opt"));
    pOpt1->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
    pOpt1->SetFont(1);
    COptionUI* pOpt2 = xFindControl(COptionUI, _T("wordexamanwer2opt"));
    pOpt2->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
    pOpt2->SetFont(1);
    COptionUI* pOpt3 = xFindControl(COptionUI, _T("wordexamanwer3opt"));
    pOpt3->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
    pOpt3->SetFont(1);
    COptionUI* pOpt4 = xFindControl(COptionUI, _T("wordexamanwer4opt"));
    pOpt4->SetDisabledTextColor(DEFAULT_TEXT_COLOR);
    pOpt4->SetFont(1);
    pLbl = xFindControl(CLabelUI, _T("wordexamtypetxt"));
    int nType = _tstoi(GetCollect(m_pWordExamRecdPtr, _T("题型")));
    bool bNeedOpt = false;
    switch (nType)
    {
    case 0:
        pLbl->SetText(_T("* 根据词义选单词"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 1:
        pLbl->SetText(_T("* 根据词义写单词"));
        pWordHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        pEdt->SetFocus();
        break;
    case 2:
        pLbl->SetText(_T("* 根据音标选单词"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        pQuesLbl->SetFont(5);
        bNeedOpt = true;
        break;
    case 3:
        pLbl->SetText(_T("* 根据音标写单词"));
        pWordHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        pEdt->SetFocus();
        pQuesLbl->SetFont(5);
        break;
    case 4:
        pLbl->SetText(_T("* 根据发音写单词"));
        pSpeakHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        pEdt->SetFocus();
        break;
    case 5:
        pLbl->SetText(_T("* 根据发音选单词"));
        pSpeakHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 6:
        pLbl->SetText(_T("* 根据单词选词义"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 7:
        pLbl->SetText(_T("* 根据单词选音标"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        pOpt1->SetFont(3);
        pOpt2->SetFont(3);
        pOpt3->SetFont(3);
        pOpt4->SetFont(3);
        bNeedOpt = true;
        break;
    default:
        break;
    }
    if (nType != 4 && nType != 5)
    {
        pQuesLbl->SetText(GetCollect(m_pWordExamRecdPtr, _T("题目")));
    }
    if (bNeedOpt)
    {
        pOpt1->SetText(GetCollect(m_pWordExamRecdPtr, _T("选项1")));
        pOpt1->Selected(false);
        pOpt1->SetEnabled(true);
        pOpt2->SetText(GetCollect(m_pWordExamRecdPtr, _T("选项2")));
        pOpt2->Selected(false);
        pOpt2->SetEnabled(true);
        pOpt3->SetText(GetCollect(m_pWordExamRecdPtr, _T("选项3")));
        pOpt3->Selected(false);
        pOpt3->SetEnabled(true);
        pOpt4->SetText(GetCollect(m_pWordExamRecdPtr, _T("选项4")));
        pOpt4->Selected(false);
        pOpt4->SetEnabled(true);
    }
}

void MainFrame::OnWordPrepare()
{
    COptionUI* pOpt;

    m_bWordAutoRead = false;
    m_bWordIsPlayOn = false;
    m_nWordModeBase = 0;
    m_nWordSpeakCount = 0;
    //m_bWordIsClickWordList = false;

    CString strGrade = ReadSetup(_T("word_grade"));
    CString strBook = ReadSetup(_T("word_book"));
    CString strUnit = ReadSetup(_T("word_unit"));
    InitWordGradeCombo(strGrade);
    InitWordBookCombo(strBook);
    InitWordUnitCombo(strUnit);

    pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("wordlernopt")));
    pOpt->Selected(true);

    // 基础版本禁止此功能 [2016-10-17 Arthur]
#ifdef APP_DEMO
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("wordfollowwordbtn"));
    pBtn->SetVisible(false);
    pBtn = xFindControl(CButtonUI, _T("wordfollowexamplebtn"));
    pBtn->SetVisible(false);
#endif
    //CRichEditUI* pSymbolText = xFindControl(CRichEditUI, _T("wordsymboltxt"));
    //pSymbolText->SetMouseEnabled(false);
    InitWordStatus();
}

void MainFrame::InitWordExamStatus()
{
    CString strCheck, strSql, strTmp;
    CCheckBoxUI* pChk;
    int QNum = 20;
    CString strWordExam_QuestionNumber = ReadSetup(_T("WordExam_QuestionNumber"));
    CString strType0 = ReadSetup(_T("WordExam_QuestionType0"));
    CString strType1 = ReadSetup(_T("WordExam_QuestionType1"));
    CString strType2 = ReadSetup(_T("WordExam_QuestionType2"));
    CString strType3 = ReadSetup(_T("WordExam_QuestionType3"));
    CString strType4 = ReadSetup(_T("WordExam_QuestionType4"));
    CString strType5 = ReadSetup(_T("WordExam_QuestionType5"));
    CString strType6 = ReadSetup(_T("WordExam_QuestionType6"));
    CString strType7 = ReadSetup(_T("WordExam_QuestionType7"));

    SetVoiceRateBySetting(_T("WordExam_Speaker"), _T("WordExam_Speed"));

    strCheck = _T("");
#define WORDEXAM_TYPE(type, name, val) do { pChk = xFindControl(CCheckBoxUI, name); pChk->SetCheck(false);  \
    if (type == _T("1")) { strCheck += val; pChk->SetCheck(true); } } while (0)

    WORDEXAM_TYPE(strType0, _T("wordexamciyiselectdancichk"), _T("0,"));
    WORDEXAM_TYPE(strType1, _T("wordexamciyiwritedancichk"), _T("1,"));
    WORDEXAM_TYPE(strType2, _T("wordexamyinbiaoselectdancichk"), _T("2,"));
    WORDEXAM_TYPE(strType3, _T("wordexamyinbiaowritedancichk"), _T("3,"));
    WORDEXAM_TYPE(strType4, _T("wordexamfayinwritedancichk"), _T("4,"));
    WORDEXAM_TYPE(strType5, _T("wordexamfayinselectdancichk"), _T("5,"));
    WORDEXAM_TYPE(strType6, _T("wordexamdanciselectciyichk"), _T("6,"));
    WORDEXAM_TYPE(strType7, _T("wordexamdanciselectyinbiaochk"), _T("7,"));

#undef WORDEXAM_TYPE

    if (!strCheck.IsEmpty())
    {
        strCheck.TrimRight(_T(','));
    }

    //更改“全选”的状态
    WordExamTypeClicked(0);

    if (IsNumeric(strWordExam_QuestionNumber))
    {
        int x = _tstoi(strWordExam_QuestionNumber);
        if ((x > 0) && (x <= 99))
        {
            QNum = x;
        }
    }

    CString strwordtype = ReadSetup(_T("word_type"));
    CString strnewbook = ReadSetup(_T("word_nwb"));
    CString strGId = ReadSetup(_T("word_grade"));
    CString strBId = ReadSetup(_T("word_book"));
    CString strUId = ReadSetup(_T("word_unit"));
    if (_T("1") == strwordtype)
    {
        strSql.Format(_T("Select Top %d * from 单词习题 where 单词 in (Select 生词 from 生词 where 生词本=%s)"),
            QNum, strnewbook);
    }
    else if (_T("0") != strUId)
    {
        strSql.Format(_T("Select Top %d * from 单词习题 where 单词 in (Select Id from 单词 where 年级=%s and 教材=%s and 课文=%s)"),
            QNum, strGId, strBId, strUId);
    }
    else
    {
        strSql.Format(_T("Select Top %d * from 单词习题 where 单词 in (Select Id from 单词 where 年级=%s and 教材=%s)"),
            QNum, strGId, strBId);
    }

    if (!strCheck.IsEmpty())
    {
        strTmp.Format(_T(" and 题型 in (%s)"),
            strCheck);
        strSql += strTmp;
    }
    srand((unsigned)time(NULL));
    strTmp.Format(_T(" and 题目 is not null and 题目 <> '' Order by Rnd(-(Id+%d))"),
        SQLRand());
    strSql += strTmp;
    CDbFile::CloseRecord(m_pWordExamRecdPtr);
    m_pWordExamRecdPtr = GetTablesBySql(_T("%s"), strSql);
    if (m_pWordExamRecdPtr == NULL)
    {
        MyAfxMessageBox(_T("根据您选择的课文，没有找到题目。请重新选择。"));
        TNotifyUI msg;
        msg.pSender = xFindControl(CControlUI, kReturnButtonName);
        msg.sType = DUI_MSGTYPE_CLICK;
        m_PaintManager.SendNotify(msg);
        //Notify(msg);
        return;
    }
    m_nWordExamQuestions = 0;
    m_nWordExamQuestionsNumbers = GetCollectCount(m_pWordExamRecdPtr);
    m_strWordExamAnswerResult.Empty();

    CLabelUI* pLbl = xFindControl(CLabelUI, _T("wordexamlocationtxt"));
    if (_T("0") == strGId)
    {
        strTmp.Format(_T("生词本>%s"),
            GetSingleField(_T("Select 名称 from 生词本 where Id=%s"),
            strBId));
    }
    else
    {
        strTmp.Format(_T("%s>%s"),
            GetSingleField(_T("Select 名称 from 年级 where Id=%s"),
            strGId),
            GetSingleField(_T("Select 名称 from 教材 where Id=%s and 年级=%s"),
            strBId, strGId));
        if (_T("0") == strUId)
        {
            strTmp += _T(">全部");
        }
        else
        {
            strTmp += _T(">");
            strTmp += GetSingleField(_T("Select 名称 from 课文 where Id=%s and 教材=%s and 年级=%s"),
                strUId, strBId, strGId);
        }
    }
    pLbl->SetText(strTmp);

    InitWordExamQuestion();
}

void MainFrame::InitWordExamGradeCombo(LPCTSTR lpstrGrade/*=NULL*/)
{
#if 0
#else
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;

    pCombo = xFindControl(CComboUI, _T("wordexamgradecmb"));
    pCombo->RemoveAll();
    pRecdPtr = GetTablesBySql(_T("%s"), _T("Select Id, 名称 from 年级 Order by 顺序"));
    pElement = new CListLabelElementUI;
    pElement->SetText(_T("生词本"));
    pElement->SetUserData(_T("0")); //Id
    pCombo->Add(pElement);
    if (pRecdPtr == NULL)
    {
        pCombo->SelectItem(0);
        return;
    }
    int nSel = 0, idx = 1;  //注意，idx从1开始
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
#endif
}

void MainFrame::InitWordExamBookCombo(LPCTSTR lpstrBook/*=NULL*/)
{
#if 0
#else
    _RecordsetPtr pRecdPtr;
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    CString strSql, strTmp;

    pCombo = xFindControl(CComboUI, _T("wordexamgradecmb"));
    strTmp = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
    if (strTmp == _T("0"))
    {
        strSql = _T("Select * from 生词本");
    }
    else
    {
        strSql.Format(_T("Select * from 教材 where 年级=%s Order by 顺序"),
            strTmp);
    }

    pCombo = xFindControl(CComboUI, _T("wordexamtextbookcmb"));
    pCombo->RemoveAll();
    pRecdPtr = GetTablesBySql(_T("%s"), strSql);
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
            pCombo->Add(pElement);
            pRecdPtr->MoveNext();
            idx++;
        }
    }
    
    if (pCombo->GetCount() > 0)
    {
        pCombo->SelectItem(nSel);
    }
#endif
}

void MainFrame::InitWordExamUnitCombo(LPCTSTR lpstrUnit/*=NULL*/)
{
#if 0
#else
    _RecordsetPtr pRecdPtr;
    CComboUI *pComboGrade, *pComboBook, *pComboUnit;
    CListLabelElementUI* pElement;
    CString strGID, strBID;

    pComboUnit = xFindControl(CComboUI, _T("wordexambookcmb"));
    pComboUnit->RemoveAll();

    pElement = new CListLabelElementUI;
    pElement->SetText(_T("全部"));
    pElement->SetUserData(_T("0"));
    pComboUnit->Add(pElement);
    pComboUnit->SelectItem(0);

    pComboGrade = xFindControl(CComboUI, _T("wordexamgradecmb"));
    strGID = pComboGrade->GetItemAt(pComboGrade->GetCurSel())->GetUserData();
    if (strGID == _T("0"))
    {
        return;
    }

    pComboBook = xFindControl(CComboUI, _T("wordexamtextbookcmb"));
    strBID = pComboBook->GetItemAt(pComboBook->GetCurSel())->GetUserData();
    if (strBID == _T("0"))
    {
        return;
    }

    pRecdPtr = GetTablesBySql(_T("Select Id, 名称 from 课文 where 年级=%s and 教材=%s Order by Id"),
        strGID, strBID);
    if (pRecdPtr == NULL)
    {
        pComboUnit->SelectItem(0);
        return;
    }
    int nSel = 0, idx = 1;  //注意，idx从1开始
    CString strId;
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("名称")));
        strId = GetCollect(pRecdPtr, _T("Id"));
        pElement->SetUserData(strId);
        if (lpstrUnit && (strId == lpstrUnit))
        {
            nSel = idx++;
        }
        pComboUnit->Add(pElement);
        pRecdPtr->MoveNext();
        idx++;
    }
    pComboUnit->SelectItem(nSel);
#endif
}

void MainFrame::WordExamChangeBtnClicked()
{
#if 0
#else
    CComboUI* pComboGrd = xFindControl(CComboUI, _T("wordexamgradecmb"));
    CComboUI* pCmbBook = xFindControl(CComboUI, _T("wordexamtextbookcmb"));
    CComboUI* pCmbUnit = xFindControl(CComboUI, _T("wordexambookcmb"));
    CString strGId, strBId, strUid, strTmp;

    strGId = pComboGrd->GetItemAt(pComboGrd->GetCurSel())->GetUserData();
    strBId = pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData();
    if (pCmbUnit->GetCount() > 0)
    {
        strUid = pCmbUnit->GetItemAt(pCmbUnit->GetCurSel())->GetUserData();
    }

    if (_T("0") == strGId)
    {
        if (pCmbBook->GetCount() <= 0)
        {
            MyAfxMessageBox(_T("您当前没有创建生词本。"));
            return;
        }
        strTmp = GetSingleField(_T("Select top 1 Id from 单词习题 where 单词 in (Select 生词 from 生词 where 生词本=%s)"),
            pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData());
    }
    else
    {
        strBId = pCmbBook->GetItemAt(pCmbBook->GetCurSel())->GetUserData();
        strUid = pCmbUnit->GetItemAt(pCmbUnit->GetCurSel())->GetUserData();
        if (_T("0") != strUid)
        {
            strTmp = GetSingleField(_T("Select top 1 Id from 单词习题 where 单词 in (Select Id from 单词 where 年级=%s and 教材=%s and 课文=%s)"),
                strGId, strBId, strUid);
        }
        else
        {
            strTmp = GetSingleField(_T("Select top 1 Id from 单词习题 where 单词 in (Select Id from 单词 where 年级=%s and 教材=%s)"),
                strGId, strBId);
        }
    }
    if (strTmp.IsEmpty())
    {
        MyAfxMessageBox(_T("根据您的选择没有可以练习的习题，请重新选择。"));
        return;
    }

    SaveSetup(_T("word_grade"), strGId);
    if (_T("0") == strGId)
    {
        SaveSetup(_T("word_type"), _T("1"));
        SaveSetup(_T("word_nwb"), strBId);
    }
    else
    {
        SaveSetup(_T("word_type"), _T("0"));
        SaveSetup(_T("word_book"), strBId);
        SaveSetup(_T("word_unit"), strUid);
    }

    InitWordExamStatus();
#endif
}

HRESULT MainFrame::WordExamSpeakWord()
{
    return Speak(GetCollect(m_pWordExamRecdPtr, _T("题目")));
}

void MainFrame::WordExamAnswerClicked()
{
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("wordexamanswertxt"));
    CString strTmp;

    strTmp.Format(_T("本题答案是：%s"),
        GetCollect(m_pWordExamRecdPtr, _T("答案")));
    pLbl->SetText(strTmp);
}

int MainFrame::IsWordExamTypeCheck(const TNotifyUI& msg)
{
    return IsOption(g_wordexam_type_chk, _countof(g_wordexam_type_chk), msg);
}

void MainFrame::WordExamTypeClicked(int nIdx)
{
    int i;
    DWORD dwChecked = 0;
    CCheckBoxUI* pChk;

    for (i = 0 ; i < _countof(g_wordexam_type_chk); i++)
    {
        pChk = xFindControl(CCheckBoxUI, g_wordexam_type_chk[i]);
        if (nIdx == i + 1)
        {
            if (!pChk->GetCheck())
            {
                dwChecked |= (0x1 << i);
            }
        }
        else if (pChk->GetCheck())
        {
            dwChecked |= (0x1 << i);
        }
    }
    pChk = xFindControl(CCheckBoxUI, _T("wordexamallchk"));
    pChk->SetCheck(dwChecked == 0xff);
}

void MainFrame::WordExamOKClicked()
{
    int i;
    CCheckBoxUI* pChk;
    CString strTmp;
    bool bSaved = false;

    for (i = 0 ; i < _countof(g_wordexam_type_chk); i++)
    {
        pChk = xFindControl(CCheckBoxUI, g_wordexam_type_chk[i]);
        if (pChk->GetCheck())
        {
            bSaved = true;
            break;
        }
    }
    if (!bSaved)
    {
        MyAfxMessageBox(_T("请至少选择一种题型。"));
        return;
    }
    for (i = 0 ; i < _countof(g_wordexam_type_chk); i++)
    {
        strTmp.Format(_T("WordExam_QuestionType%d"), i);
        pChk = xFindControl(CCheckBoxUI, g_wordexam_type_chk[i]);
        SaveSetup(strTmp, pChk->GetCheck() ? _T("1") : _T("0"));
    }

    MyAfxMessageBox(_T("保存设置成功。开始生成习题。"));
    InitWordExamStatus();
}

void MainFrame::WordExamAddWordClicked()
{
    CWordAddnew* pWnd = new CWordAddnew(this);
    //CComboUI* pCombo = xFindControl(CComboUI, _T("wordexamgradecmb"));
    if (pWnd)
    {
        pWnd->m_nNewWord = _tstoi(GetCollect(m_pWordExamRecdPtr, _T("单词")));
        HWND hWnd = pWnd->Create(GetHWND(), g_SkinInfo[sWordAddNewSkin].title, UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW);
        if (hWnd)
        {
            pWnd->CenterWindow();
            pWnd->ShowModal();
        }
    }
}

bool MainFrame::WordExamGetText(const CString& strAnswer)
{
    bool bOK = false;
    CString strTmp;
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("wordexamanswertxt"));

    if (strAnswer.CompareNoCase(GetCollect(m_pWordExamRecdPtr, _T("答案"))) == 0)
    {
        pLbl->SetText(_T("回答正确。"));
        strTmp.Format(_T("%sSystem\\Sound\\examRight.wav"),
            m_PaintManager.GetInstancePath());
        m_mciWave.PlayWait(GetHWND(), strTmp);
        strTmp.Format(_T("%s,1,%s;"),
            GetCollect(m_pWordExamRecdPtr, _T("Id")),
            strAnswer);
        m_strWordExamAnswerResult += strTmp;
        bOK = true;
    }
    else
    {
        strTmp.Format(_T("回答错误。答案是：%s"),
            GetCollect(m_pWordExamRecdPtr, _T("答案")));
        pLbl->SetText(strTmp);
        strTmp.Format(_T("%s\\System\\Sound\\examWrong.wav"),
            m_PaintManager.GetInstancePath());
        m_mciWave.PlayWait(GetHWND(), strTmp);
        strTmp.Format(_T("%s,0,%s;"),
            GetCollect(m_pWordExamRecdPtr, _T("Id")),
            strAnswer);
        m_strWordExamAnswerResult += strTmp;
    }
    ::SetTimer(GetHWND(), TIMER_ID_WORD_EXAM, 2000, NULL);

    return bOK;
}

void MainFrame::WordExamSubmitClicked()
{
    CEditUI* pEdt = xFindControl(CEditUI, _T("wordexamansweredt"));
    CString strAnswer;
    
    strAnswer = pEdt->GetText();
    strAnswer.Trim();

    WordExamGetText(strAnswer);
}

void MainFrame::WordExamSelectAlltype(bool bSelected)
{
    int i;
    CCheckBoxUI* pChk;

    for (i = 0; i < _countof(g_wordexam_type_chk); i++)
    {
        pChk = xFindControl(CCheckBoxUI, g_wordexam_type_chk[i]);
        pChk->SetCheck(bSelected);
    }
}

int MainFrame::IsWordExamOpt(const TNotifyUI& msg)
{
    return IsOption(g_wordexam_opt, _countof(g_wordexam_opt), msg);
}

void MainFrame::WordExamOptionClicked(int nIndx)
{
    ASSERT(nIndx >= 1 && nIndx <= 4);
    int i;
    COptionUI* pOpt;

    for (i = 0; i < _countof(g_wordexam_opt); i++)
    {
        pOpt = xFindControl(COptionUI, g_wordexam_opt[i]);
        pOpt->SetEnabled(false);
    }
    pOpt = xFindControl(COptionUI, g_wordexam_opt[nIndx-1]);
    //pOpt->SetEnabled(true);
    static LPCTSTR wordexam_answer[] =
    {
        _T("A"),
        _T("B"),
        _T("C"),
        _T("D")
    };
    CString strAnswer = wordexam_answer[nIndx - 1];
    bool bOK = WordExamGetText(strAnswer);
    if (bOK)
    {
        //pOpt->SetTextColor(DUI_RGB(255,0,0));
        pOpt->SetDisabledTextColor(DUI_RGB(255,0,0));
    }
}

void MainFrame::InitWordExamResultCtrls()
{
    CString strTmp;
    SplitResultVector strResultOne;
    //SplitString返回的是;的个数
    //TRACE(_T("m_strWordExamAnswerResult=%s\n"), m_strWordExamAnswerResult);
    SplitString(TString(m_strWordExamAnswerResult), _T(";"), m_strWordExamResultAll);
    int count =  m_strWordExamResultAll.size(), i, j;
    CListUI *pList = xFindControl(CListUI, _T("wordresultlist"));
    CListLabelElementUI *pItem;
    int intTrueQuestion = 0;
    CEditUI* pRat = xFindControl(CEditUI, _T("wordresultpercentedt"));
    CEditUI* pQCount = xFindControl(CEditUI, _T("wordresultalledt"));
    CEditUI* pQTrue = xFindControl(CEditUI, _T("wordresultrightedt"));
    CControlUI* pLbl = xFindControl(CControlUI, _T("wordresultnswerctl"));    

    pList->RemoveAll();
    //LPCTSTR strAttr = _T("height=\"26\" width=\"9999\"");
    //Id,1,A
    for (i = 0; i < count; i++)
    {
        j = SplitString(m_strWordExamResultAll[i], _T(","), strResultOne) + 1;
        ASSERT(j == 3);
        strTmp.Format(_T("第%d题  %c"),
            i + 1, (strResultOne[1] == _T("1")) ? _T('√') : _T('×'));

        pItem = new CListLabelElementUI;
        pList->Add(pItem);
        pItem->SetText(strTmp);
        pItem->SetTag(_tstoi(strResultOne[0].c_str()));

        if (strResultOne[1] == _T("1"))
        {
            intTrueQuestion++;
        }
    }

    int nValue = (int)CTimeSpan(CTime::GetCurrentTime() - m_tmWordLogFrom).GetTotalMinutes();
    StudyLog(STUDY_TYPE_WORD, LOG_TYPE_TIME, nValue, 0);
    StudyLog(STUDY_TYPE_WORD, LOG_TYPE_ACHIEV, count, intTrueQuestion);

    float fRate = intTrueQuestion * 100.0f / count;
    strTmp.Format(_T("%d%%"), (int)fRate);
    pRat->SetText(strTmp);

    strTmp.Format(_T("%d"), count);
    pQCount->SetText(strTmp);

    strTmp.Format(_T("%d"), intTrueQuestion);
    pQTrue->SetText(strTmp);

    strTmp.Format(_T("%s"), (fRate >= 60.0f) ? _T("Image/AnswerRight.png") : _T("Image/AnswerWrong.png"));
    pLbl->SetBkImage(strTmp);

    if (pList->GetCount() > 0)
    {
        pList->SelectItem(0);
    }

    CComboUI* pCombo = xFindControl(CComboUI, _T("wordresultbookcmb"));
    pCombo->RemoveAll();
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 生词本"));
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            pItem->SetUserData(GetCollect(pRecdPtr, _T("Id")));
            pCombo->Add(pItem);
            pRecdPtr->MoveNext();
        }
    }
    
    if (pCombo->GetCount() > 0)
    {
        pCombo->SelectItem(0);
    }
}

void MainFrame::WordExamResultQuestionDetail()
{
    CHorizontalLayoutUI *pWordHLay, *pSpeakHLay;
    pWordHLay = xFindControl(CHorizontalLayoutUI, _T("wordresultwordhlayout"));
    pWordHLay->SetVisible(false);

    pSpeakHLay = xFindControl(CHorizontalLayoutUI, _T("wordresultpeakhlayout"));
    pSpeakHLay->SetVisible(false);

    CVerticalLayoutUI* pSelectVlay = xFindControl(CVerticalLayoutUI, _T("wordresultselectvlayout"));
    pSelectVlay->SetVisible(false);

    CVerticalLayoutUI* pTextVlay = xFindControl(CVerticalLayoutUI, _T("wordexamtextvlayout"));
    pTextVlay->SetVisible(false);

    CEditUI* pEdt = xFindControl(CEditUI, _T("wordresultansweredt"));
    pEdt->SetText(_T(""));

    CListUI *pList = xFindControl(CListUI, _T("wordresultlist"));
    int nIndx = pList->GetCurSel();

    CLabelUI* pQuesLbl = xFindControl(CLabelUI, _T("wordresultwordtxt"));
    pQuesLbl->SetFont(0);
    COptionUI* pOpt1 = xFindControl(COptionUI, _T("wordresultanwer1opt"));
    pOpt1->SetFont(1);
    COptionUI* pOpt2 = xFindControl(COptionUI, _T("wordresultanwer2opt"));
    pOpt2->SetFont(1);
    COptionUI* pOpt3 = xFindControl(COptionUI, _T("wordresultanwer3opt"));
    pOpt3->SetFont(1);
    COptionUI* pOpt4 = xFindControl(COptionUI, _T("wordresultanwer4opt"));
    pOpt4->SetFont(1);
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("wordresulttypetxt"));
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 单词习题 where Id=%d"),
        pList->GetItemAt(nIndx)->GetTag());
    int nType = _tstoi(GetCollect(pRecdPtr, _T("题型")));
    bool bNeedOpt = false;
    switch (nType)
    {
    case 0:
        pLbl->SetText(_T("根据词义选单词"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 1:
        pLbl->SetText(_T("根据词义写单词"));
        pWordHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        break;
    case 2:
        pLbl->SetText(_T("根据音标选单词"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        pQuesLbl->SetFont(5);
        bNeedOpt = true;
        break;
    case 3:
        pLbl->SetText(_T("根据音标写单词"));
        pWordHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        pQuesLbl->SetFont(5);
        break;
    case 4:
        pLbl->SetText(_T("根据发音写单词"));
        pSpeakHLay->SetVisible(true);
        pTextVlay->SetVisible(true);
        break;
    case 5:
        pLbl->SetText(_T("根据发音选单词"));
        pSpeakHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 6:
        pLbl->SetText(_T("根据单词选词义"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        bNeedOpt = true;
        break;
    case 7:
        pLbl->SetText(_T("根据单词选音标"));
        pWordHLay->SetVisible(true);
        pSelectVlay->SetVisible(true);
        pOpt1->SetFont(3);
        pOpt2->SetFont(3);
        pOpt3->SetFont(3);
        pOpt4->SetFont(3);
        bNeedOpt = true;
        break;
    default:
        break;
    }
    CString strAnswer = GetCollect(pRecdPtr, _T("答案"));
    //if (nType != 4 && nType != 5)
    {
        pQuesLbl->SetText(GetCollect(pRecdPtr, _T("题目")));
    }
    if (bNeedOpt)
    {
        pOpt1->SetText(GetCollect(pRecdPtr, _T("选项1")));
        pOpt1->SetEnabled(false);
        if (strAnswer == _T("A"))
        {
            pOpt1->Selected(true);
            pOpt1->SetTextColor(DUI_RGB(255,0,0));
        }

        pOpt2->SetText(GetCollect(pRecdPtr, _T("选项2")));
        pOpt2->SetEnabled(false);
        if (strAnswer == _T("B"))
        {
            pOpt2->Selected(true);
            pOpt2->SetTextColor(DUI_RGB(255,0,0));
        }

        pOpt3->SetText(GetCollect(pRecdPtr, _T("选项3")));
        pOpt3->SetEnabled(false);
        if (strAnswer == _T("C"))
        {
            pOpt3->Selected(true);
            pOpt3->SetTextColor(DUI_RGB(255,0,0));
        }

        pOpt4->SetText(GetCollect(pRecdPtr, _T("选项4")));
        pOpt4->SetEnabled(false);
        if (strAnswer == _T("D"))
        {
            pOpt4->Selected(true);
            pOpt4->SetTextColor(DUI_RGB(255,0,0));
        }
    }
    else
    {
        pEdt->SetText(strAnswer);
    }

    SplitResultVector strResultOne;
    SplitString(m_strWordExamResultAll[nIndx], _T(","), strResultOne);
    strAnswer = strResultOne[2].c_str();
    if (strAnswer.IsEmpty())
    {
        strAnswer = _T("未答题");
    }
    CString strTmp;
    strTmp.Format(_T("您的答案是：%s"),
        strAnswer);
    CRichEditUI* pRLbl = xFindControl(CRichEditUI, _T("wordresultanswertxt"));
    pRLbl->SetText(strTmp);
}

void MainFrame::WordExamResultAddWrongWord()
{
    CComboUI* pCombo = xFindControl(CComboUI, _T("wordresultbookcmb"));
    CListUI* pList = xFindControl(CListUI, _T("wordresultlist"));
    int nIndx;

    if (pCombo->GetCount() <= 0)
    {
        MyAfxMessageBox(_T("您尚未创建生词本。"));
        return;
    }

    nIndx = pList->GetCurSel();
    ASSERT(nIndx >= 0);

    SplitResultVector strResultOne;
    SplitString(m_strWordExamResultAll[nIndx].c_str(), _T(","), strResultOne);

    CString strWord = GetSingleField(_T("Select 单词 from 单词习题 where Id=%s"),
        strResultOne[0].c_str());
    CString strTmp = GetSingleField(_T("Select Id from 生词 where 生词本=%s and 生词=%s"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData(), strWord);
    if (strTmp.IsEmpty())
    {
        BOOL bOK = ExecuteSql(_T("Insert into 生词(生词, 生词本) Values(%s, %s)"),
            strWord, pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
        if (bOK)
        {
            MyAfxMessageBox(_T("添加成功。"));
        }
    }
    else
    {
        MyAfxMessageBox(_T("该生词已存在，请勿重复加入生词。"));
    }
}

//////////////////////////////////////////////////////////////////////////
//朗读函数
static
void MyDeleteFile(LPCTSTR strPath, BOOL bRecursive = TRUE)
{
    CFileFind finder;
    BOOL bWorking = finder.FindFile(strPath);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
        if (finder.IsDots())
        {
            continue;
        }
        if (finder.IsDirectory())
        {
            if (bRecursive)
            {
                CString strTmp;
                strTmp.Format(_T("%s\\*.*"), finder.GetFilePath());
                MyDeleteFile(strTmp);
            }
            continue;
        }
        if (!::DeleteFile(finder.GetFilePath()))
        {
            TRACE(_T("DeleteFile(%s) failed%d\n"), finder.GetFilePath(), GetLastError());
        }
    }
    finder.Close();
}

void MainFrame::ReadDelMaterialFiles_(LPCTSTR strPath)
{
    CString strTmp;
#if 1
    strTmp.Format(_T("%sData\\ReadingMaterial\\%s\\*.*"), CPaintManagerUI::GetInstancePath().GetData(), strPath);
    MyDeleteFile(strTmp);
#else
    strTmp.Format(_T("%sData\\ReadingMaterial\\%s\\*.*\0"), CPaintManagerUI::GetInstancePath().GetData(), strPath);
    SHFILEOPSTRUCT ss;
    memset(&ss, 0, sizeof(ss));
    ss.hwnd = GetHWND();
    ss.wFunc = FO_DELETE;
    ss.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_FILESONLY;
#ifndef _DEBUG
    ss.fFlags |= FOF_NOERRORUI;
#endif
    ss.pFrom = (LPCTSTR)strTmp;
    int x = SHFileOperation(&ss);
    TRACE(_T("SHFileOperation return:%d, fAnyOperationsAborted:%s, Path:%s\n"),
        x, ss.fAnyOperationsAborted ? _T("TRUE") : _T("FALSE"), strTmp);
#endif
    strTmp.Format(_T("%sData\\ReadingMaterial\\%s"), CPaintManagerUI::GetInstancePath().GetData(), strPath);
    if (!::RemoveDirectory(strTmp))
    {
        TRACE(_T("::RemoveDirectory(%s) failed:%d\n"), strTmp, GetLastError());
    }
}

int MainFrame::ReadChangedSave_(int bSaveAs)
{
    int nRet = 0;
    CString strTmp, strPath;
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

    do 
    {
        if (!bSaveAs)
        {
            long pBool = tomFalse;
            m_pReadTextDoc->GetSaved(&pBool);
            if (!pREdt->GetModify() &&
                (pBool == tomTrue) )
            {
                nRet = 1;
                break;
            }
            if (AfxMessageBox(_T("当前文档已经更改，是否需要保存？"),
                    MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
            {
                nRet = 1;
                pREdt->SetModify(false);
                m_pReadTextDoc->SetSaved(tomTrue);
                break;
            }
        }


        strPath = m_strReadDocumentPath;
        if (strPath.IsEmpty() || (bSaveAs == 2))
        {
            strTmp.Format(_T("Document %d.txt"), m_nReadDocument);
            //::MessageBox(GetHWND(), _T("rr"), _T("d"), MB_OK);
            CFileDialog dlgmp3(FALSE, _T("txt"), NULL,
                OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
                _T("文本文件 (*.txt)|*.txt|RTF文件 (*.rtf)|*.rtf||"),
                CWnd::FromHandle(GetHWND()));
            OPENFILENAME& ofn = dlgmp3.GetOFN();
            TCHAR tbuf[4096];
            StringCchPrintf(tbuf, _countof(tbuf), strTmp);
            ofn.lpstrFile = tbuf;
            ofn.nMaxFile = _countof(tbuf);
            ofn.hwndOwner = GetHWND();
            if (dlgmp3.DoModal() != IDOK)
            {
                break;
            }
            strPath = dlgmp3.GetPathName();
        }
        long lFlag = tomCreateAlways;
        if (strPath.Right(3).CompareNoCase(_T("txt")) == 0)
        {
            //MyWriteFile(strPath, pREdt->GetText().GetData());
            lFlag |= tomText;
        }
        else
        {
            ASSERT(m_pReadTextDoc);
            lFlag |= tomRTF;
            //m_pReadTextDoc->Save(COleVariant(strPath), tomCreateAlways | tomRTF, 0);
        }
        TRACE(_T("save %s\n"), strPath);
        m_pReadTextDoc->Save(&COleVariant(strPath), lFlag, 0);
        pREdt->SetModify(false);

        nRet = 1;
    } while (FALSE);

    return nRet;
}

void MainFrame::ReadGetWordPosition_(__in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition)
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
    CString strContent = pREdt->GetText().GetData();
    int nIdx, n_count = strContent.GetLength();

    static CString READ_POS = _T("?!,.\"- ;");

    intFromPosition = intToPosition = 0;

    do 
    {
        if (strContent.IsEmpty())
        {
            break;
        }
        if (nCurrentPos < 0 ||
            nCurrentPos >= n_count)
        {
            break;
        }

        //往前数，碰到READ_POS中的字符为止
        nIdx = nCurrentPos - 1;
        while(nIdx > 0)
        {
            if (READ_POS.Find(strContent[nIdx]) >= 0)
            {
                intFromPosition = nIdx;
                break;
            }
            nIdx--;
        }
        if (intFromPosition != 0)
        {
            intFromPosition++;
        }

        //往后数
        nIdx = nCurrentPos;
        while(nIdx < n_count)
        {
            if (READ_POS.Find(strContent[nIdx]) >= 0)
            {
                intToPosition = nIdx;
                break;
            }
            nIdx++;
        }
        if (intToPosition == 0)
        {
            intToPosition = n_count;
        }

    } while (FALSE);
}

void MainFrame::ReadGetSentencePosition_(__in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition)
{
    GetSentencePosition(_T("readcontenttxt"), nCurrentPos, intFromPosition, intToPosition);
}

void MainFrame::ReadStartSpeak_(int nFrom, int nTo)
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
    
    if (nFrom != nTo)
    {
        CProgressUI* pPrg = xFindControl(CProgressUI, _T("readfollowprg"));
        pPrg->SetValue(0);

        CButtonUI* pBtn = xFindControl(CButtonUI, _T("storyplaybackbtn"));
        pBtn->SetVisible(false);

        CHorizontalLayoutUI* pHo = xFindControl(CHorizontalLayoutUI, _T("readScoreHlayout"));
        pHo->SetVisible(false);

        m_nReadCurrentCursor = nFrom;
        m_nReadToPostion = nTo;

        pREdt->SetSel(nFrom, nTo);
        SetRichEditSelColor(pREdt, RGB(255,0,0));

        m_strReadSpokenText = pREdt->GetSelText();
        pREdt->SetSelNone();
        m_tmReadSpokenFrom = CTime::GetCurrentTime();
        Speak(m_strReadSpokenText);
    }
}

void MainFrame::ReadUpdateHelpList_(_RecordsetPtr& pRecdPtr)
{
    CString strTmp;
    CListLabelElementUI* pItem;
    CListUI* pList = xFindControl(CListUI, _T("readhlplistlist"));
    pList->RemoveAll();

    if (pRecdPtr == NULL)
    {
        return;
    }

    LPCTSTR strAttr = _T("height=\"16\" width=\"9999\"");
    int idx = 1;
    while(!pRecdPtr->GetadoEOF())
    {
        pItem = new CListLabelElementUI;
        //指定高度和宽度，能够提高效率
        pItem->ApplyAttributeList(strAttr);
        pItem->SetUserData(GetCollect(pRecdPtr, (long)0));
        strTmp.Format(_T("%d.%s"), idx % 10, GetCollect(pRecdPtr, (long)0));
        pItem->SetText(strTmp);
        pList->Add(pItem);

        pRecdPtr->MoveNext();

        idx++;
    }
}

bool MainFrame::ReadUpdateHelpList(CString& strKeyWord)
{
    bool bFound = false;
    m_strReadHelpKeyWord = strKeyWord;

    do 
    {
        m_strReadHelpKeyWord.Replace(_T("'"), _T("''"));
        _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%'"),
            m_strReadHelpKeyWord);

        if (NULL == pRecdPtr)
        {
            break;
        }

        ReadUpdateHelpList_(pRecdPtr);

        bFound = true;
    } while (FALSE);

    return bFound;
}

void MainFrame::ReadReLayoutHelp()
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
    CContainerUI* pHelp = xFindControl(CContainerUI, _T("readwordhelp"));
    long lStart, lEnd;
    pREdt->GetSel(lStart, lEnd);
    DuiLib::CPoint pt = pREdt->GetCharPos(lStart);
    CRect rct = pREdt->GetPos();
    CRect rcthlp = pHelp->GetPos();
    int nWidth = rcthlp.Width();
    int nHeight = rcthlp.Height();
    int cx = pt.x;
    int cy = pt.y;

    int padl, padt;

    if (cx + nWidth < rct.right)
    {
        padl = pt.x - rct.left + 10;
    }
    else
    {
        padl = pt.x - rct.left - nWidth - 10;
    }

    //if (nHeight + 0x95 - cy > rct.Height())
    if (cy + nHeight < rct.bottom)
    {
        padt = cy - rct.top;
    }
    else
    {
        padt = cy - rct.top - nHeight +10;
    }

    CString strTmp;
    strTmp.Format(_T("padding=\"%d,%d,0,0\""), padl, padt);
    CContainerUI* pWordHelp = xFindControl(CContainerUI, _T("readwordhelp"));
    pWordHelp->ApplyAttributeList(strTmp);
}

void MainFrame::OnReadWndPrepare()
{
    //if (m_pReadTextDoc)
    //{
    //    m_pReadTextDoc.Release();
    //    m_pReadTextDoc = NULL;
    //}
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
    IRichEditOle* pOle = NULL;
    LRESULT lr;
    pREdt->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pOle, &lr);
    //pOle->QueryInterface(IID_ITextDocument, (LPVOID*)&m_pReadTextDoc);
    pOle->QueryInterface(__uuidof(ITextDocument), (LPVOID*)&m_pReadTextDoc);
    pOle->Release();
    TRACE(_T("m_pReadTextDoc:%p\n"), m_pReadTextDoc);

    DWORD dwMask = pREdt->GetEventMask();
    TRACE(_T("default mask:0x%x\n"), dwMask);
    dwMask |= (ENM_CHANGE | ENM_KEYEVENTS);
    pREdt->SetEventMask(dwMask);
    dwMask = pREdt->GetEventMask();
    TRACE(_T("after mask:0x%x\n"), dwMask);
    pREdt->SetAutoURLDetect(false);
    //pREdt->TxSendMessage(EM_GETEDITSTYLE, (WPARAM)0, (LPARAM)0, &lr);
    //TRACE(_T("before EM_SETEDITSTYLE:0x%x\n"), lr);
    //pREdt->TxSendMessage(EM_SETEDITSTYLE, (WPARAM)SES_NOIME, (LPARAM)SES_NOIME, &lr);
    //pREdt->TxSendMessage(EM_GETEDITSTYLE, (WPARAM)0, (LPARAM)0, &lr);
    //TRACE(_T("after EM_SETEDITSTYLE:0x%x\n"), lr);

    m_nReadDocument = 0;
    m_iReadFontSize = 0;
    m_bReadSpeakLetter = false;
    m_bReadSpeakWord = true;
    m_bReadSpeakSentence = true;
    m_bReadHelper = false;
    m_bReadExporting = false;
    m_bReadPlaying = false;
    m_bReadReciting = false;
    
    m_strReadHelpKeyWord.Empty();
    m_strReadSentences.Empty();
    m_nReadCurrentSentence = 0;
    m_sptReadSentences.clear();
    m_iReadFontSize = 0;

    m_strCloseFlag = CF_OPEN;
    m_strReadUnknowWord.Empty();
    m_strReadUnknowSymbol.Empty();

    CString strChangeSpeaker = ReadSetup(_T("Lesson_ChangeSpeaker"));
    m_bReadChangeSpeaker = _tstoi(strChangeSpeaker) ? true : false;
    m_strReadSecondSpeaker = ReadSetup(_T("Lesson_SecondSpeaker"));
    m_strReadSpeaker = ReadSetup(_T("Lesson_Speaker"));
    m_strReadLevel = ReadSetup(_T("Lesson_Level"));
    m_strReadSpeed = ReadSetup(_T("Lesson_Speed"));

    SetVoiceRateByValue(m_strReadSpeaker, m_strReadSpeed);

    //新建，认为此时处于未修改状态
    pREdt->SetModify(false);
    m_pReadTextDoc->SetSaved(tomTrue);

    m_nReadType = -1;
    COptionUI* pOpt = xFindControl(COptionUI, _T("storyreadopt"));
    pOpt->Selected(true);
    //选择后，会调用此函数
    ///InitReadCtrls(0);

    InitReadTreeView();
}

void MainFrame::ReadSendStopClick()
{
    if (m_bReadPlaying)
    {
        TNotifyUI msg;
        msg.pSender = xFindControl(CButtonUI, _T("storystartbtn"));
        msg.sType = DUI_MSGTYPE_CLICK;
        //Notify(msg);
        m_PaintManager.SendNotify(msg);
    }
}

void MainFrame::OnReadWndClicked(TNotifyUI& msg)
{
    CString strSender = msg.pSender->GetName().GetData();
    CString strTmp;
    CString strPath;
    CTreeViewUI* pView;
    CTreeNodeUI* pNode;
    CRichEditUI* pREdt;
    CButtonUI*  pBtn;
    int i;

    if (strSender == _T("storyaddbtn"))
    {
        BROWSEINFO bi;
        memset(&bi, 0, sizeof(bi));
        bi.hwndOwner = GetHWND();
        //BIF_RETURNFSANCESTORS, BIF_SHAREABLE, BIF_USENEWUI
        bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS; 
        TCHAR sPathBuf[MAX_PATH];
        bi.pszDisplayName = sPathBuf;
        bi.lpszTitle = _T("添加自定义素材");
        PIDLIST_ABSOLUTE pId = SHBrowseForFolder(&bi);
        if (pId)
        {
            if (!SHGetPathFromIDList(pId, sPathBuf))
            {
                return;
            }
            TRACE(_T("slelect folder:%s\n"), sPathBuf);
            strTmp = sPathBuf;
            strTmp.Remove(_T('\''));
            strPath = strTmp.Right(strTmp.GetLength() - strTmp.ReverseFind(_T('\\')) - 1);
            strTmp = GetSingleField(_T("Select Id from 自定义素材 where 路径='%s'"),
                strPath);
            if (!strTmp.IsEmpty())
            {
                MyAfxMessageBox(_T("[%s] 素材已经存在，请勿重复添加。"),
                    strPath);
                return;
            }
            strTmp.Format(_T("%sData\\ReadingMaterial\\%s"), CPaintManagerUI::GetInstancePath().GetData(), strPath);
            i = SHCreateDirectoryEx(GetHWND(), strTmp, NULL);
            if (ERROR_SUCCESS != i)
            {
                if (ERROR_FILE_EXISTS != i &&
                    ERROR_ALREADY_EXISTS != i)
                {
                    MyAfxMessageBox(_T("创建目录%s失败，原因码：%d"), i);
                    return;
                }
#ifdef _DEBUG
                else
                {
                    TRACE(_T("目录%s已存在。\n"), strTmp);
                }
#endif
            }
            i = 0;
            CFileFind finder;
            strTmp.Format(_T("%s\\*.txt"), sPathBuf);
            BOOL bWorking = finder.FindFile(strTmp);
            while(bWorking)
            {
                bWorking = finder.FindNextFile();

                if (finder.IsDots())
                {
                    continue;
                }
                if (finder.IsDirectory())
                {
                    TRACE(_T("文件夹中的文件夹:%s\n"),
                        finder.GetFilePath());
                    if (finder.GetFilePath() == sPathBuf)
                    {
                        continue;
                    }
                    //文件夹中不能有文件夹
                    TRACE(_T("文件夹中不能有文件夹:%s\n"),
                        finder.GetFilePath());
                    i = 0;
                    break;
                    //continue;
                }
                //if (finder.GetFileName().Right(4).CompareNoCase(_T(".txt")) == 0)
                {
                    strTmp.Format(_T("%sData\\ReadingMaterial\\%s\\%s"), 
                        CPaintManagerUI::GetInstancePath().GetData(), strPath, finder.GetFileName());
                    ::CopyFile(finder.GetFilePath(), strTmp, FALSE);
                    i++;
                }
            }
            finder.Close();
            if (i > 0)
            {
                ExecuteSql(_T("Insert into 自定义素材(路径) Values('%s')"), 
                    strPath);
                MyAfxMessageBox(_T("成功添加 %d 个素材文件。"),
                    i);
                InitReadTreeView();
            }
            else
            {
                MyAfxMessageBox(_T("添加素材 0 个，可能出错的原因如下：\r\n1.文件格式不正确，只允许txt格式。\r\n2.存在多层目录，文件夹中不能有文件夹。"));
                ReadDelMaterialFiles_(strPath);
            }
        }
    }
    else if (strSender == _T("storydelbtn"))
    {
        pView = xFindControl(CTreeViewUI, _T("readstorylist"));
        int count = pView->GetCount();
        for (i = 0; i < count; i++)
        {
            pNode = static_cast<CTreeNodeUI*>(pView->GetItemAt(i));
            if (pNode->GetTag() == 0)
            {
                //不是素材
                continue;
            }
            CCheckBoxUI* pChk = pNode->GetFolderButton();
            //未展开，即认为未选中
            if (!pChk->GetCheck())
            {
                continue;
            }
            strPath = pNode->GetUserData().GetData();
            i = _tstoi(GetSingleField(_T("select count(0) from 自定义素材 where 路径='%s'"),
                strPath));
            strTmp.Format(_T("确定要删除素材 [%s] 吗？"),
                strPath);
            if ((i > 0) &&
                (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES))
            {
                ExecuteSql(_T("Delete from 自定义素材 where 路径='%s'"),
                    strPath);
                ReadDelMaterialFiles_(strPath);
                InitReadTreeView();
            }
            //只有一个会选中，退出
            break;
        }
    }
    else if (strSender == _T("readnewbtn"))
    {
        ReadSendStopClick();
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        
        //取消操作
        if (ReadChangedSave_(0) == 0)
        {
            return;
        }

        pREdt->SetReadOnly(false);

        m_pReadTextDoc->New();
        m_nReadDocument++;
        
        //新建，认为此时处于未修改状态
        pREdt->SetModify(false);
        m_pReadTextDoc->SetSaved(tomTrue);

        //pREdt->SetText(_T(""));
        m_strReadDocumentPath.Empty();
        pBtn = xFindControl(CButtonUI, _T("readalltranslatebtn"));
        pBtn->SetVisible(false);
    }
    else if (strSender == _T("readopenbtn"))
    {
        ReadSendStopClick();
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

        if (ReadChangedSave_(0) == 0)
        {
            return;
        }

        //CFileDialog dlgmp3(TRUE, _T("txt"), NULL,
        //    OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
        //    _T("文本文件 (*.txt)|*.txt|RTF文件 (*.rtf)|*.rtf||"), CWnd::FromHandle(GetHWND()));
        CFileDialog dlgmp3(TRUE, _T("txt"), NULL,
            OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
            _T("文本文件 (*.txt)|*.txt||"), CWnd::FromHandle(GetHWND()));
        dlgmp3.m_ofn.hwndOwner = GetHWND();
        OPENFILENAME& ofn = dlgmp3.GetOFN();
        TCHAR tbuf[4096];
        tbuf[0] = _T('\0');
        ofn.lpstrFile = tbuf;
        ofn.nMaxFile = _countof(tbuf);
        if (IDOK != dlgmp3.DoModal())
        {
            return;
        }
        m_strReadDocumentPath = dlgmp3.GetPathName();
#if 0
        //If the tomPasteFile flag is not set in the Flags parameter,
        //the method first closes the current document after saving any unsaved changes.
        //i = tomOpenExisting | tomPasteFile;
        i = tomOpenExisting;
        if (dlgmp3.GetFileExt().CompareNoCase(_T("txt")) == 0)
        {
            //MyReadFile(m_strReadDocumentPath, strTmp);
            //pREdt->SetText(strTmp);
            i |= tomText;
        }
        else
        {
            //m_pReadTextDoc->Open(COleVariant(m_strReadDocumentPath), tomOpenExisting | tomRTF, 0);
            i |= tomRTF;
        }
        m_pReadTextDoc->Open(&COleVariant(m_strReadDocumentPath), i, 0);
#endif

        MyReadAsciiFile(m_strReadDocumentPath, strTmp);

        strTmp = CSplitSentence::GetSplitSentence(strTmp);
        m_strReadSentences = strTmp;
        strTmp.Replace(_T("<p>"), _T(""));
        strTmp.Replace(_T("<d>"), _T(""));
        strTmp.Replace(_T("|"), _T("\n"));
        pREdt->SetText(strTmp);
        pREdt->SetSel(0, 0);

        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSelNone();
        pREdt->SetFocus();

        pREdt->SetReadOnly(false);
        pREdt->SetModify(false);
        m_pReadTextDoc->SetSaved(tomTrue);

        m_nReadDocument++;
        pBtn = xFindControl(CButtonUI, _T("readalltranslatebtn"));
        pBtn->SetVisible(false);
    }
    else if (strSender == _T("readsavebtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

        if (pREdt->IsReadOnly())
        {
            MyAfxMessageBox(_T("当前显示的是课文内容，不能保存。"));
            return;
        }
        if (ReadChangedSave_(1) == 0)
        {
            return;
        }
        if (m_strReadDocumentPath.IsEmpty())
        {
            m_nReadDocument++;
        }
        pREdt->SetModify(false);
    }
    else if (strSender == _T("readsavemenubtn"))
    {
        CMenuWnd* pMenu = new CMenuWnd();
        DuiLib::CPoint point = msg.ptMouse;
        ClientToScreen(GetHWND(), &point);
        pMenu->Init(NULL, _T("readmenusave.xml"), point, &m_PaintManager);
    }
    else if (strSender == _T("readcopybtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        pREdt->Copy();
    }
    else if (strSender == _T("readpastebtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        //pREdt->Paste();
        CComPtr<ITextRange> cpTxtRange;
        long nStart, nEnd;
        pREdt->GetSel(nStart, nEnd);
        HRESULT Hr = m_pReadTextDoc->Range(nStart, nEnd, &cpTxtRange);
        if (SUCCEEDED(Hr))
        {
            cpTxtRange->Paste(NULL, 0);
        }
    }
    else if (strSender == _T("readundobtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        pREdt->Undo();
    }
    else if (strSender == _T("readredobtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        pREdt->Redo();
    }
    else if (strSender == _T("readtranslatebtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        int nFrom = 0, nTo = 0;

        long lStart, lEnd;
        pREdt->GetSel(lStart, lEnd);
        ReadGetWordPosition_(lStart, nFrom, nTo);

        if (nFrom >= nTo)
        {
            strTmp.Empty();
        }
        else
        {
            strTmp = pREdt->GetText().Mid(nFrom, nTo - nFrom).GetData();
        }

        CTranslateWord* pTWord = new CTranslateWord(this);
        pTWord->m_strWord = strTmp;
        OpenModalWnd(pTWord, g_SkinInfo[SReadTransSkin].title);
    }
    else if (strSender == _T("readreadbtn"))
    {
        //do nothing here
    }
    else if (strSender == _T("readreadmenubtn"))
    {
        CMenuWnd* pMenu = new CMenuWnd();
        DuiLib::CPoint point = msg.ptMouse;
        ClientToScreen(GetHWND(), &point);
        pMenu->Init(NULL, _T("readmenuread.xml"), point, &m_PaintManager, &m_ReadMenuCheckInfo);
    }
    else if (strSender == _T("readzoomoutbtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        TFontInfo* pFInfo = m_PaintManager.GetFontInfo(pREdt->GetFont());
        if (m_iReadFontSize == 0)
        {
            m_iReadFontSize = pFInfo->iSize;
        }
        m_iReadFontSize += 2;
        pREdt->SetFont(pFInfo->sFontName, m_iReadFontSize, pFInfo->bBold, pFInfo->bUnderline, pFInfo->bItalic);
        //pREdt->SetZoom(2, 1);
    }
    else if (strSender == _T("readzoominbtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        TFontInfo* pFInfo = m_PaintManager.GetFontInfo(pREdt->GetFont());
        if (m_iReadFontSize == 0)
        {
            m_iReadFontSize = pFInfo->iSize;
        }
        if (m_iReadFontSize > 2)
        {
            m_iReadFontSize -= 2;
        }
        pREdt->SetFont(pFInfo->sFontName, m_iReadFontSize, pFInfo->bBold, pFInfo->bUnderline, pFInfo->bItalic);
        //pREdt->SetZoom(1, 2);
    }
    else if (strSender == _T("readalltranslatebtn"))
    {
        if (m_strReadContentPath.IsEmpty())
        {
            return;
        }
        strPath = m_strReadContentPath;
        strPath.Replace(_T("英文.txt"), _T("中文.txt"));
        CFileFind finder;
        if (finder.FindFile(strPath))
        {
            CTranslateAll *pTwnd = new CTranslateAll(this);
            MyReadAsciiFile(strPath, pTwnd->m_strContent);
            OpenModalWnd(pTwnd, g_SkinInfo[sReadCnTextSkin].title);
        }
        finder.Close();
    }
    else if (strSender == _T("storystartbtn"))
    {
        //if (READ_TYPE_READ == m_nReadType)
        {
            //_T("正在导出音频，请等待导出完成。");
            pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
            pBtn = xFindControl(CButtonUI, _T("storystartbtn"));
            pREdt->SetFocus();

            CVerticalLayoutUI* pCtrl;
            if (m_bReadPlaying)
            {
                m_bReadPlaying = false;
                StopSpeak();
                StopWaveRecordReco();
                pBtn->SetText(_T("开  始"));
                pREdt->SetTextColor(DUI_BLACK);
                pREdt->SetSelNone();
                ::KillTimer(GetHWND(), TIMER_ID_READ_FOLLOW);
                ::KillTimer(GetHWND(), TIMER_ID_READ_NEXT);

                pCtrl = xFindControl(CVerticalLayoutUI, _T("readprogvlayout"));
                pCtrl->SetVisible(false);
            }
            else
            {
                m_bReadPlaying = true;
                pBtn->SetText(_T("停  止"));

                int nFrom = 0, nTo = 0;

                long lStart, lEnd;
                pREdt->GetSel(lStart, lEnd);
                if (lStart == pREdt->GetText().GetLength())
                {
                    lStart = 0;
                }
                ReadGetSentencePosition_(lStart, nFrom, nTo);

                ReadStartSpeak_(nFrom, nTo);

                if (READ_TYPE_FOLLOW == m_nReadType)
                {
                    pCtrl = xFindControl(CVerticalLayoutUI, _T("readprogvlayout"));
                    pCtrl->SetVisible(true);
                }
            }
        }
        //else
        //{
        //    ASSERT(m_nReadType == READ_TYPE_FLLOW);
        //    StopSpeak();
        //}
    }
    else if (strSender == _T("storystaticsbtn"))
    {
        if (m_nReadType == READ_TYPE_READ)
        {
            pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
            strTmp = pREdt->GetText();
            strTmp.Trim();
            if (strTmp.IsEmpty())
            {
                MyAfxMessageBox(_T("请输入要转为音频的文字。"));
                return;
            }
            CFileDialog dlgmp3(FALSE, _T("mp3"), NULL,
                OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
                _T("mp3文件(*.mp3)|*.mp3||"), CWnd::FromHandle(GetHWND()));
            dlgmp3.m_ofn.hwndOwner = GetHWND();
            OPENFILENAME& ofn = dlgmp3.GetOFN();
            TCHAR tbuf[4096];
            tbuf[0] = _T('\0');
            ofn.lpstrFile = tbuf;
            ofn.nMaxFile = _countof(tbuf);
            if (dlgmp3.DoModal() != IDOK)
            {
                return;
            }
            CString strMp3 = dlgmp3.GetPathName();
            ::DeleteFile(strMp3);
            CString strWav = dlgmp3.GetPathName().Left(strMp3.GetLength() - 3);
            strWav += _T("wav");
            ::DeleteFile(strWav);

            strTmp = pREdt->GetSelText();
            if (strTmp.IsEmpty())
            {
                strTmp = pREdt->GetText();
            }

            SplitResultVector sptToVoice;
            SplitString(TString(strTmp), _T("\r\n"), sptToVoice);
            SplitResultVector::const_iterator p;
            i = 0;
            SpeakBindToFile(strWav);
            for (p = sptToVoice.begin(); p != sptToVoice.end(); ++p)
            {
                if (p->empty())
                {
                    continue;
                }
                if (m_bReadChangeSpeaker)
                {
                    if ((i % 2) == 0)
                    {
                        SetVoiceRateByValue(m_strReadSpeaker, m_strReadSpeed);
                    }
                    else
                    {
                        SetVoiceRateByValue(m_strReadSecondSpeaker, m_strReadSpeed);
                    }
                    i++;
                }
                SpeakAndWait(p->c_str());
            }
            SpeakRestore();
            hlpWave2Mp3(strWav, strMp3);
            pBtn = xFindControl(CButtonUI, _T("readmsgbtn"));
            pBtn->SetUserData(strMp3);
            pBtn->SetVisible(true);
        }
        else
        {
            ReadSendStopClick();
            ShowSymbolStatistic(m_strReadUnknowWord, m_strReadUnknowSymbol);
        }
    }
    else if (strSender == _T("storyplaybackbtn"))
    {
        ReadSendStopClick();
        CProgressUI* pPrg = xFindControl(CProgressUI, _T("readfollowprg"));
        pPrg->SetValue(0);

        StopWaveRecordReco();
        m_mciWave.PlayWait(GetHWND(), m_strCommonWaveFile);
    }
    else if (strSender == _T("dictationfirstbtn"))
    {
        m_nReadCurrentSentence = 0;
        ReadSpeackCurrentSentence();
    }
    else if (strSender == _T("dictationleftbtn"))
    {
        if (m_nReadCurrentSentence > 0)
        {
            m_nReadCurrentSentence--;
            ReadSpeackCurrentSentence();
        }
    }
    else if (strSender == _T("dictationplaybtn"))
    {
        ReadSpeackCurrentSentence();
    }
    else if (strSender == _T("dictationrightbtn"))
    {
        if (m_nReadCurrentSentence < m_sptReadSentences.size() - 1)
        {
            m_nReadCurrentSentence++;
            ReadSpeackCurrentSentence();
        }
    }
    else if (strSender == _T("dictationlastbtn"))
    {
        m_nReadCurrentSentence = m_sptReadSentences.size() - 1;
        ReadSpeackCurrentSentence();
    }
    else if (strSender == _T("dictationsunmitbtn"))
    {
        ASSERT(m_nReadCurrentSentence < (int)m_sptReadSentences.size());
        pREdt = xFindControl(CRichEditUI, _T("dictationansweredt"));
        pREdt->SetSelNone();
        strTmp = pREdt->GetText();
        strTmp.Trim();
        CString sourceText;
        sourceText.Format(_T(" %s "), m_sptReadSentences[m_nReadCurrentSentence].c_str());
        pREdt = xFindControl(CRichEditUI, _T("dictationsouceedt"));
        pREdt->SetText(sourceText);

        int n = strTmp.GetLength(), startidx = 0, num = 0, num5 = 0, num6 = -1;
        MyWordVector mywords;

        while(num < n)
        {
            while(!isalpha(strTmp[num], MainFrame::m_loc) &&
                (num < n - 1) )
            {
                num++;
            }
            if (num == n - 1)
            {
                break;
            }
            startidx = num;
            int num2 = num + 1;
            while(isalpha(strTmp[num2], MainFrame::m_loc) &&
                (num2 < n - 1) )
            {
                num2++;
            }
            if (num2 == n - 1)
            {
                if (isalpha(strTmp[num2], MainFrame::m_loc))
                {
                    num5 = num2;
                }
                else
                {
                    num5 = num2 - 1;
                }
            }
            else
            {
                num5 = num2 - 1;
            }
            MyWord mw;
            mw.m_nWordId = num6 + 1;
            num6++;
            mw.m_nWordStart = startidx;
            mw.m_nWordEnd = num5;
            mw.m_strWordText = strTmp.Mid(startidx, num5 - startidx + 1);
            //将sourceText中m_strWordText的内容清为空格，并返回m_strWordText的位置
            mw.m_nWordMatchIndex = ReadCompareWord(sourceText, mw.m_strWordText);

            mywords.push_back(mw);
            if ((mw.m_nWordId != 0) && (num6 >= 1) &&
                (mywords[num6-1].m_nWordMatchIndex > mw.m_nWordMatchIndex) &&
                (mw.m_nWordMatchIndex != -1) )
            {
                MyWord mw2 = mywords[mw.m_nWordId - 1];
                sourceText = sourceText.Left(mw2.m_nWordMatchIndex) + mw2.m_strWordText + 
                    sourceText.Mid(mw2.m_nWordMatchIndex + mw2.m_strWordText.GetLength(), sourceText.GetLength() - 
                    mw2.m_nWordMatchIndex - mw2.m_strWordText.GetLength());
            }

            num = num5 + 1;
        }
        pREdt->SetSelAll();
        CHARFORMAT2 cf;
        pREdt->GetSelectionCharFormat(cf);
        SetRichEditSelColor(pREdt, RGB(255,0,0));

        cf.crTextColor = RGB(0,0,0);        //black
        cf.dwMask |= CFM_COLOR;
        MyWordVector::const_iterator p;
        strTmp = pREdt->GetText();
        for (p = mywords.begin(); p != mywords.end(); ++p)
        {
            if (p->m_nWordMatchIndex != -1)
            {
                pREdt->SetSel(p->m_nWordMatchIndex, p->m_nWordMatchIndex + p->m_strWordText.GetLength());
                pREdt->SetSelectionCharFormat(cf);
                //[text] = ([text].Substring(0, list.Item(num).WordMatchIndex) & Me.GetEmptyCharString(list.Item(num).WordText.Length) & [text].Substring((list.Item(num).WordMatchIndex + list.Item(num).WordText.Length), (([text].Length - list.Item(num).WordMatchIndex) - list.Item(num).WordText.Length)))
                CString strTmp2 = strTmp;
                strTmp = strTmp2.Left(p->m_nWordMatchIndex);
                for (i = 0; i < p->m_strWordText.GetLength(); i++)
                {
                    strTmp.Append(_T(" "));
                }
                strTmp += strTmp2.Mid(p->m_nWordMatchIndex + p->m_strWordText.GetLength(), strTmp2.GetLength() - p->m_nWordMatchIndex - p->m_strWordText.GetLength());
            }
        }
        sourceText = pREdt->GetText();
        n = sourceText.GetLength();
        for (i = 0; i < n; i++)
        {
            if (!isalpha(sourceText[i], MainFrame::m_loc))
            {
                pREdt->SetSel(i, i+1);
                pREdt->SetSelectionCharFormat(cf);
            }
        }
        int num4 = 0;
        for (p = mywords.begin(); p != mywords.end(); ++p)
        {
            if (p->m_nWordMatchIndex != -1)
            {
                num4++;
            }
        }
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("dictationinfotxt"));
        CString strInfo;
        if (num4 == 0)
        {
            strInfo = _T("回答错误。");
        }
        else
        {
            strInfo = _T("回答正确。");
            for (i = 0; i < strTmp.GetLength(); i++)
            {
                if (isalpha(strTmp[i], MainFrame::m_loc))
                {
                    strInfo = _T("部分正确,请参考原文。");
                    break;
                }
            }
        }
        pLbl->SetText(strInfo);
        pLbl->SetVisible(true);
    }
    else if (strSender == _T("dictationsoucebtn"))
    {
        pREdt = xFindControl(CRichEditUI, _T("dictationsouceedt"));
        strTmp = m_sptReadSentences[m_nReadCurrentSentence].c_str();
        strTmp.Trim();
        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetText(strTmp);
    }
    else if (strSender == _T("dictationtovoicebtn"))
    {
        if (m_sptReadSentences.size() == 0)
        {
            MyAfxMessageBox(_T("原文内容为空，无法转换。"));
            return;
        }
        CFileDialog dlgmp3(FALSE, _T("mp3"), NULL,
            OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NONETWORKBUTTON,
            _T("mp3文件(*.mp3)|*.mp3||"), CWnd::FromHandle(GetHWND()));
        dlgmp3.m_ofn.hwndOwner = GetHWND();
        OPENFILENAME& ofn = dlgmp3.GetOFN();
        TCHAR tbuf[4096];
        tbuf[0] = _T('\0');
        ofn.lpstrFile = tbuf;
        ofn.nMaxFile = _countof(tbuf);
        if (dlgmp3.DoModal() != IDOK)
        {
            return;
        }
        CString strMp3 = dlgmp3.GetPathName();
        ::DeleteFile(strMp3);
        CString strWav = dlgmp3.GetPathName().Left(strMp3.GetLength() - 3);
        strWav += _T("wav");
        ::DeleteFile(strWav);

        SpeakBindToFile(strWav);
        SplitResultVector::const_iterator p;
        for (p = m_sptReadSentences.begin(); p != m_sptReadSentences.end(); ++p)
        {
            if (p->empty())
            {
                continue;
            }
            //if (m_bReadChangeSpeaker)
            //{
            //    if ((i % 2) == 0)
            //    {
            //        SetVoiceRateByValue(m_strReadSpeaker, m_strReadSpeed);
            //    }
            //    else
            //    {
            //        SetVoiceRateByValue(m_strReadSecondSpeaker, m_strReadSpeed);
            //    }
            //    i++;
            //}
            SpeakAndWait(p->c_str());
        }
        SpeakRestore();
        hlpWave2Mp3(strWav, strMp3);
        MyAfxMessageBox(_T("保存成功。"));
    }
    else if (strSender == _T("readmsgbtn"))
    {
        pBtn = static_cast<CButtonUI*>(msg.pSender);
        strTmp = pBtn->GetUserData();
        if (strTmp.IsEmpty())
        {
            return;
        }
        HINSTANCE hRet = ::ShellExecute(NULL, _T("explore"), strTmp.Left(strTmp.ReverseFind(_T('\\'))), NULL, NULL, SW_MAXIMIZE);
        TRACE(_T("ShellExecute return:%d\n"), hRet);
        pBtn->SetVisible(false);
    }
    else if (strSender == _T("readhlpup"))
    {
        CListUI* pList = xFindControl(CListUI, _T("readhlplistlist"));
        if (pList->GetCount() >= 10)
        {
            //只会<= 10
            pList->SelectItem(0);
            strTmp = pList->GetItemAt(0)->GetUserData();
            _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%' and words<'%s'"),
                m_strReadHelpKeyWord, strTmp);
            if (pRecdPtr != NULL)
            {
                ReadUpdateHelpList_(pRecdPtr);
            }
        }
    }
    else if (strSender == _T("readhlpdown"))
    {
        CListUI* pList = xFindControl(CListUI, _T("readhlplistlist"));
        if (pList->GetCount() >= 10)
        {
            //只会<= 10
            pList->SelectItem(9);
            strTmp = pList->GetItemAt(9)->GetUserData();
            _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select top 10 words from CET6 where words like '%s%%' and words>'%s'"),
                m_strReadHelpKeyWord, strTmp);
            if (pRecdPtr != NULL)
            {
                ReadUpdateHelpList_(pRecdPtr);
            }
        }
    }
    else if (strSender == _T("readMaterialbtn"))
    {
        strTmp = ReadSetup(_T("MaterialURL"));
        if (strTmp.IsEmpty())
        {
            strTmp = _T("http://www.smartabctest.com");
        }
        ::ShellExecute(NULL, _T("open"), strTmp, NULL, NULL, SW_SHOWMAXIMIZED);
    }
    else if (strSender == _T("recitestartbtn"))
    {
        if (m_bReadPlaying)
        {
            m_bReadPlaying = false;
            pBtn = xFindControl(CButtonUI, _T("recitestartbtn"));
            pBtn->SetText(_T("开  始"));

            ReadStopRecite();
        }
        else
        {
            m_bReadPlaying = true;
            pBtn = xFindControl(CButtonUI, _T("recitestartbtn"));
            pBtn->SetText(_T("停  止"));

            ReadStartRecite();
        }

    }
    else if (strSender == _T("recitenextbtn"))
    {
        if (m_bReadPlaying)
        {
            CComboUI* pCmb = xFindControl(CComboUI, _T("recitemancmb"));
            //全文背诵没有提示和下一句
            //if (RECITE_All != pCmb->GetCurSel())
            {
                if (m_nReadCurrentSentence < m_sptReadSentences.size() - 1)
                {
                    m_nReadCurrentSentence++;
                    ReadStartRecite();
                }
            }
        }
    }
    else if (strSender == _T("recitetipsbtn"))
    {
        if (m_bReadPlaying)
        {
            CComboUI* pCmb = xFindControl(CComboUI, _T("recitemancmb"));
            //全文背诵没有提示和下一句
            //if (RECITE_All != pCmb->GetCurSel())
            {
                i = pCmb->GetCurSel();
                int of = RECITE_2nd == i ? 2 : 1;
                if (m_nReadCurrentSentence < m_sptReadSentences.size() - of)
                {
                    pREdt = xFindControl(CRichEditUI, _T("recitetxt"));
                    m_nReadTipS = pREdt->SetSel(-1, -1);
                    //SetRichEditSelColor(pREdt, RGB(0, 0, 0));
                    //m_strReadReciteTextBak = pREdt->GetText(GT_USECRLF).GetData();
                    m_nReadReciteTipeCount = 0;
                    pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
                    pBtn->SetEnabled(false);
                    pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
                    pBtn->SetEnabled(false);
                    //pREdt->SetText(m_strReadReciteTextBak + m_sptReadSentences[m_nReadCurrentSentence + 1].c_str());
                    //pREdt->AppendText(m_sptReadSentences[m_nReadCurrentSentence + of].c_str());
                    hlpAppedText(pREdt, m_sptReadSentences[m_nReadCurrentSentence + of].c_str());
                    ::SetTimer(GetHWND(), TIMER_ID_READ_RECITETIP, 1000, NULL);
                }
            }
        }
    }
}

void MainFrame::InitReadCtrls(int nIndex)
{
    ASSERT(nIndex >=READ_TYPE_READ && nIndex <= READ_TYPE_RECITE);
    m_nReadType = nIndex;

    CString strTmp;
    CButtonUI* pBtn;
    CLabelUI* pLbl;
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

    StopSpeak();
    m_bReadPlaying = false;

    CHorizontalLayoutUI* pHo = xFindControl(CHorizontalLayoutUI, _T("readScoreHlayout"));
    pHo->SetVisible(false);

    if (nIndex == READ_TYPE_READ ||
        nIndex == READ_TYPE_FOLLOW)
    {
        pBtn = xFindControl(CButtonUI, _T("storystartbtn"));
        pBtn->SetText(_T("开  始"));

        pBtn = xFindControl(CButtonUI, _T("storyplaybackbtn"));
        pBtn->SetVisible(false);

        pBtn = xFindControl(CButtonUI, _T("storystaticsbtn"));
        pBtn->SetText(nIndex ? _T("发音统计") : _T("转为音频"));
#ifdef APP_DEMO
        pBtn->SetVisible(nIndex ? false : true);
#endif

        pBtn = xFindControl(CButtonUI, _T("readmsgbtn"));
        pBtn->SetVisible(false);


        CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("readprogvlayout"));
        //pVLay->SetVisible(nIndex ? true : false);
        pVLay->SetVisible(false);
    }
    else if (nIndex == READ_TYPE_DICTA)
    {
        pLbl = xFindControl(CLabelUI, _T("dictationinfotxt"));
        pLbl->SetText(_T(""));
        CLabelUI* pLblCur = xFindControl(CLabelUI, _T("dictationcurrenttxt"));
        CLabelUI* pLblCount = xFindControl(CLabelUI, _T("dictationalltxt"));
        CHorizontalLayoutUI* pHLay = xFindControl(CHorizontalLayoutUI, _T("readdictationhlay"));

        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        
        strTmp = pREdt->GetText(GTL_USECRLF).GetData();
        TRACE(_T("readcontenttxt:%s\n"), strTmp);
        m_strReadSentences = CSplitSentence::GetSplitSentence(strTmp);
        
        //pREdt->SetSelAll();
        //pREdt->Copy();
        //pREdt->SetSelNone();
        //wstring wTmp;
        //MyGetClipboardText(GetHWND(), wTmp);
        //TRACE("readcontenttxt:%S\n", wTmp.c_str());
        //m_strReadSentences = CSplitSentence::GetSplitSentence((LPTSTR)CW2T(wTmp.c_str()));

        //strTmp = pREdt->GetText();
        //strTmp.Trim();
        //bool bShow = strTmp.IsEmpty() ? false : true;
        bool bShow = m_strReadSentences.IsEmpty() ? false : true;
        pHLay->SetVisible(bShow);
        pBtn = xFindControl(CButtonUI, _T("dictationsunmitbtn"));
        pBtn->SetVisible(bShow);
        pBtn = xFindControl(CButtonUI, _T("dictationsoucebtn"));
        pBtn->SetVisible(bShow);
        pBtn = xFindControl(CButtonUI, _T("dictationtovoicebtn"));
        pBtn->SetVisible(bShow);

        if (m_strReadSentences.IsEmpty())
        {
            MyAfxMessageBox(_T("%s"), _T("请打开素材或键入需听写的内容。"));
            pLblCur->SetText(_T("0"));
            pLblCount->SetText(_T("0"));
        }
        else
        {
            SplitString(TString(m_strReadSentences), _T("|"), m_sptReadSentences);
            m_nReadCurrentSentence = 0;
            pLblCur->SetText(_T("1"));
            strTmp.Format(_T("%d"), m_sptReadSentences.size());
            pLblCount->SetText(strTmp);
        }
    }
    else
    {
        pBtn = xFindControl(CButtonUI, _T("recitestartbtn"));
        pBtn->SetText(_T("开  始"));
        pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
        pBtn->SetEnabled(false);
        pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
        pBtn->SetEnabled(false);

        ReadStopRecite();

        //CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("reciteprogvlayout"));
        //pVLay->SetVisible(false);

        //CComboUI* pCmb = xFindControl(CComboUI, _T("recitemancmb"));
        //pCmb->SelectItem(1);

        pREdt = xFindControl(CRichEditUI, _T("recitetxt"));
        pREdt->SetText(_T(""));

        pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

        strTmp = pREdt->GetText(GTL_USECRLF).GetData();
        TRACE(_T("reite:readcontenttxt:%s\n"), strTmp);
        m_strReadSentences = CSplitSentence::GetSplitSentence(strTmp);

        if (m_strReadSentences.IsEmpty())
        {
            MyAfxMessageBox(_T("%s"), _T("请打开素材或键入需背诵的内容。"));
        }
        else
        {
            SplitString(TString(m_strReadSentences), _T("|"), m_sptReadSentences);
            m_nReadCurrentSentence = 0;
        }
    }
}

static
bool HasSubDir(LPCTSTR strPath)
{
    bool bHas = false;
    CFileFind finder;
    CString strTmp;
    strTmp.Format(_T("%s\\*.*"), strPath);
    BOOL bWork = finder.FindFile(strTmp);
    while(bWork)
    {
        bWork = finder.FindNextFile();
        if (finder.IsDots())
        {
            continue;
        }
        if (finder.IsDirectory())
        {
            bHas = true;
            break;
        }
    }
    finder.Close();

    return bHas;
}

void MainFrame::ReadTreeViewAddNode(LPCTSTR lpstrPath, CTreeNodeUI* pParent)
{
#if 0
    CString strPath = lpstrPath, strTmp;
    CFileFind finder;
    BOOL bWorking;
    CTreeNodeUI* pNode;

    if (!finder.FindFile(strPath))
    {
        finder.Close();
        return;
    }
    strPath += _T("\\*.*");
    bWorking = finder.FindFile(strPath);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDots())
        {
            continue;
        }

        if (finder.IsDirectory())
        {
            if (HasSubDir(finder.GetFilePath()))
            {
            }
        }
        if (!finder.IsDirectory())
        {
            //如果有子目录，忽略此目录下的文件
            if (HasSubDir(lpstrPath))
            {
                continue;
            }
            CTreeNodeUI* pSubNode = new CTreeNodeUI;
            pSubNode->SetAttribute(_T("height"), _T("21"));
            pSubNode->SetAttribute(_T("folderattr"), _T("width=\"0\""));
#if SKIN==SKN_WIN8
            pSubNode->SetAttribute(_T("itemtextcolor"), _T("0xffffffff"));
#endif
            strTmp = _T("font=\"1\"");
            pSubNode->SetAttribute(_T("itemattr"), strTmp);
            pSubNode->SetItemText(subfinder.GetFileName());
            //注意，素材的Tag为0
            pSubNode->SetTag(0);
            strTmp.Format(_T("%s\\英文.txt"),
                subfinder.GetFilePath());
            if (txtfind.FindFile(strTmp))
            {
                //素材保存全路径
                pSubNode->SetUserData(strTmp);
            }
            else
            {
                txtfind.Close();
            }
            txtfind.Close();
            pNode->Add(pSubNode);
        }

        strTmp = GetSingleField(_T("Select Id from 自定义素材 where 路径='%s'"),
            finder.GetFileName());
        if (!strTmp.IsEmpty())
        {
            TRACE(_T("[%s] 素材为自定义素材，延迟添加。\n"),
                finder.GetFileName());
            continue;
        }
        
        /*
        <TreeNode height="27" folderattr="text=&quot;B&quot; textpadding=&quot;10,0,0,0&quot; align=&quot;left&quot; width=&quot;238&quot; height=&quot;27&quot; normalimage=&quot;file='Image/paclose.png'&quot; selectedhotimage=&quot;Image/paopen2.png&quot; selectedimage=&quot;file='Image/paopen.png'&quot;">
        <TreeNode text="B1" height="21" folderattr="width=&quot;0&quot;"/>
        <TreeNode text="B1" height="21" folderattr="width=&quot;0&quot;"/>
        </TreeNode>
        */
        /*
        改变TreeNode控件中CheckBox控件的属性(folderattr)，来显示故事等,
        TreeNode包含CheckBox（item展开与收缩按钮，folderattr）、Label（item虚线，dotlineattr）
        CheckBox（item复选框，checkboxattr）、Option（item按钮，itemattr）等
        */
        CTreeNodeUI* pNode = new CTreeNodeUI;
        strTmp.Format(_T("text=\"%s\" textpadding=\"10,0,0,0\" align=\"left\" width=\"238\" height=\"27\" normalimage=\"file='Image/paclose.png'\" selectedhotimage=\"Image/paopen2.png\" selectedimage=\"file='Image/paopen.png'\""),
            finder.GetFileName());
#if SKIN==SKN_WIN8
        strTmp += _T(" textcolor=\"0xffffffff\" ");
#endif
        pNode->SetAttribute(_T("folderattr"), strTmp);
        pNode->SetAttribute(_T("height"), _T("27"));
        //素材分类，UserData保存名字
        pNode->SetUserData(finder.GetFileName());
        //注意，Tag不为0的是素材分类，记录MainFrame指针
        pNode->SetTag((UINT_PTR)this);

        CCheckBoxUI *pChk = pNode->GetFolderButton();
        pChk->SetUserData(finder.GetFileName());
        pChk->SetTag((UINT_PTR)this);
        pChk->OnNotify += MakeDelegate(&MainFrame::OnReadTreeViewNotify);

        if (HasSubDir(finder.GetFilePath()))
        {
            ReadTreeViewAddNode(finder.GetFilePath());
        }

        CFileFind subfinder;
        strTmp.Format(_T("%s\\*.*"), finder.GetFilePath());
        BOOL bSubWorking = subfinder.FindFile(strTmp);
        while(bSubWorking)
        {
            bSubWorking = subfinder.FindNextFile();

            if (subfinder.IsDots())
            {
                continue;
            }
            if (!subfinder.IsDirectory())
            {
                continue;
            }
            
        }
        pView->Add(pNode);
    }
    finder.Close();
#endif
}

void MainFrame::InitReadTreeView()
{
    CString strPath, strTmp;
    CTreeViewUI* pView = xFindControl(CTreeViewUI, _T("readstorylist"));
    CTreeNodeUI* pNode;
    CTreeNodeUI* pSubNode;

    pView->RemoveAll();

#if 0       //修改为：遍历ReadingMaterial下的文件夹
    strPath.Format(_T("%sData\\ReadingMaterial"), CPaintManagerUI::GetInstancePath());
    ReadTreeViewAddNode(strPath, NULL);
#else
    //遍历ReadingMaterial目录下的目录，分两层，第一层目录为分类名字，如““故事”，第二层为素材名字，下面为txt文件
    //自定义素材只有一层目录，下面为txt文件
    strPath.Format(_T("%sData\\ReadingMaterial"), CPaintManagerUI::GetInstancePath());
    CFileFind finder;
    BOOL bWorking;
    if (finder.FindFile(strPath))
    {
        finder.Close();

        strPath += _T("\\*.*");
        bWorking = finder.FindFile(strPath);
        while (bWorking)
        {
            bWorking = finder.FindNextFile();

            if (finder.IsDots())
            {
                continue;
            }
            if (!finder.IsDirectory())
            {
                continue;
            }

            strTmp = GetSingleField(_T("Select Id from 自定义素材 where 路径='%s'"),
                finder.GetFileName());
            if (!strTmp.IsEmpty())
            {
                TRACE(_T("[%s] 素材为自定义素材，延迟添加。\n"),
                    finder.GetFileName());
                continue;
            }
            if (!HasSubDir(finder.GetFilePath()))
            {
                continue;
            }
            /*
            <TreeNode height="27" folderattr="text=&quot;B&quot; textpadding=&quot;10,0,0,0&quot; align=&quot;left&quot; width=&quot;238&quot; height=&quot;27&quot; normalimage=&quot;file='Image/paclose.png'&quot; selectedhotimage=&quot;Image/paopen2.png&quot; selectedimage=&quot;file='Image/paopen.png'&quot;">
                <TreeNode text="B1" height="21" folderattr="width=&quot;0&quot;"/>
                <TreeNode text="B1" height="21" folderattr="width=&quot;0&quot;"/>
            </TreeNode>
            */
            /*
            改变TreeNode控件中CheckBox控件的属性(folderattr)，来显示故事等,
            TreeNode包含CheckBox（item展开与收缩按钮，folderattr）、Label（item虚线，dotlineattr）
            CheckBox（item复选框，checkboxattr）、Option（item按钮，itemattr）等
            */
            pNode = new CTreeNodeUI;
            strTmp.Format(_T("text=\"%s\" textpadding=\"10,0,0,0\" align=\"left\" width=\"238\" height=\"27\" normalimage=\"file='Image/paclose.png'\" selectedhotimage=\"Image/paopen2.png\" selectedimage=\"file='Image/paopen.png'\""),
                finder.GetFileName());
#if SKIN==SKN_WIN8
            //参见CCheckBoxUI的属性（CCheckBoxUI继承自COptionUI, COptionUI从CButtonUI）
            strTmp += _T(" textcolor=\"0xffffffff\" hottextcolor=\"0xff000000\" selectedtextcolor=\"0xff000000\"");
#endif
            pNode->SetAttribute(_T("folderattr"), strTmp);
            pNode->SetAttribute(_T("height"), _T("27"));
            //素材分类，UserData保存名字
            pNode->SetUserData(finder.GetFileName());
            //注意，Tag不为0的是素材分类，记录MainFrame指针
            pNode->SetTag((UINT_PTR)this);

            CCheckBoxUI *pChk = pNode->GetFolderButton();
            pChk->SetUserData(finder.GetFileName());
            pChk->SetTag((UINT_PTR)this);
            pChk->OnNotify += MakeDelegate(&MainFrame::OnReadTreeViewNotify);
            
            CFileFind subfinder;
            strTmp.Format(_T("%s\\*.*"), finder.GetFilePath());
            BOOL bSubWorking = subfinder.FindFile(strTmp);
            while(bSubWorking)
            {
                bSubWorking = subfinder.FindNextFile();

                if (subfinder.IsDots())
                {
                    continue;
                }
                if (!subfinder.IsDirectory())
                {
                    continue;
                }
                pSubNode = new CTreeNodeUI;
                pSubNode->SetAttribute(_T("height"), _T("21"));
                pSubNode->SetAttribute(_T("folderattr"), _T("width=\"0\""));
#if SKIN==SKN_WIN8
                pSubNode->SetAttribute(_T("itemtextcolor"), _T("0xffffffff"));
#endif
                strTmp = _T("font=\"1\"");
                pSubNode->SetAttribute(_T("itemattr"), strTmp);
                pSubNode->SetItemText(subfinder.GetFileName());
                //注意，素材的Tag为0
                pSubNode->SetTag(0);
                CFileFind txtfind;
                strTmp.Format(_T("%s\\英文.txt"),
                    subfinder.GetFilePath());
                if (txtfind.FindFile(strTmp))
                {
                    //素材保存全路径
                    pSubNode->SetUserData(strTmp);
                }
                txtfind.Close();
                pNode->Add(pSubNode);
            }
            pView->Add(pNode);
        }
        finder.Close();
    }

    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("%s"), _T("Select * from 自定义素材"));
    if (pRecdPtr != NULL)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            strTmp = GetCollect(pRecdPtr, _T("路径"));
            strPath.Format(_T("%sData\\ReadingMaterial\\%s"), CPaintManagerUI::GetInstancePath().GetData(), strTmp);
            pNode = new CTreeNodeUI;
            CString strAttr;
            strAttr.Format(_T("text=\"%s\" textpadding=\"10,0,0,0\" align=\"left\" width=\"238\" height=\"27\" normalimage=\"file='Image/paclose.png'\" selectedhotimage=\"Image/paopen2.png\" selectedimage=\"file='Image/paopen.png'\""),
                strTmp);
#if SKIN==SKN_WIN8
            strAttr += _T(" textcolor=\"0xffffffff\" ");
#endif
            pNode->SetAttribute(_T("folderattr"), strAttr);
            pNode->SetAttribute(_T("height"), _T("27"));
            pNode->SetUserData(strTmp);
            pNode->SetTag((UINT_PTR)this);

            CCheckBoxUI *pChk = pNode->GetFolderButton();
            pChk->SetUserData(strTmp);
            pChk->SetTag((UINT_PTR)this);
            pChk->OnNotify += MakeDelegate(&MainFrame::OnReadTreeViewNotify);

            strPath += _T("\\*.txt");
            bWorking = finder.FindFile(strPath);
            while(bWorking)
            {
                bWorking = finder.FindNextFile();

                if (finder.IsDots())
                {
                    continue;
                }
                if (finder.IsDirectory())
                {
                    continue;
                }
                //if (finder.GetFileName().Right(4).CompareNoCase(_T(".txt")) == 0)
                {
                    pSubNode = new CTreeNodeUI;
                    pSubNode->SetAttribute(_T("height"), _T("21"));
                    pSubNode->SetAttribute(_T("folderattr"), _T("width=\"0\""));
#if SKIN==SKN_WIN8
                    pSubNode->SetAttribute(_T("itemtextcolor"), _T("0xffffffff"));
#endif
                    pSubNode->SetItemText(finder.GetFileName());
                    pSubNode->SetAttribute(_T("itemattr"), _T("font=\"1\""));
                    pSubNode->SetUserData(finder.GetFilePath());
                    pSubNode->SetTag(0);
                    pNode->Add(pSubNode);
                }
            }
            finder.Close();

            pView->Add(pNode);
            pRecdPtr->MoveNext();
        }
    }
#endif
}

bool MainFrame::OnReadTreeViewNotify(void* event)
{
    CCheckBoxUI* pNode;
    TNotifyUI* pMsg = static_cast<TNotifyUI*>(event);
    if (pMsg->sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        pNode = static_cast<CCheckBoxUI*>(pMsg->pSender);
        MainFrame* pFrame = (MainFrame*)pNode->GetTag();

        if (pNode->GetCheck())
        {
            //收缩其它所有分类
            CTreeViewUI* pView = (CTreeViewUI*)pFrame->m_PaintManager.FindControl(_T("readstorylist"));
            //int i;
            //for (i = 0; i < pView->GetCount(); i++)
            //{
            //    CTreeNodeUI* pItem = (CTreeNodeUI*)pView->GetItemAt(i);
            //    if (pItem->GetTag() &&
            //        pItem->GetUserData() != pNode->GetUserData())
            //    {
            //        CCheckBoxUI* pChk = pItem->GetFolderButton();
            //        if (pChk->GetCheck())
            //        {
            //            pChk->SetCheck(false);
            //        }
            //    }
            //}

            pFrame->m_strReadParentName = pNode->GetUserData().GetData();

            //if (pFrame->m_nReadType != 2)
            {
                CLabelUI* pLbl = static_cast<CLabelUI*>(pFrame->m_PaintManager.FindControl(_T("storyreadnametxt")));
                pLbl->SetText(pNode->GetUserData().GetData());
            }

            ////显示/隐藏“全文翻译”按钮
            //CButtonUI* pBtn = static_cast<CButtonUI*>(pFrame->m_PaintManager.FindControl(_T("readalltranslatebtn")));
            //CString strTmp = pFrame->GetSingleField(_T("SELECT count(*) from 自定义素材 where 路径='%s'"),
            //    pNode->GetUserData().GetData());
            //pBtn->SetVisible(strTmp == _T("1") ? false : true);

        }
    }

    return true;
}

void MainFrame::InitReadContent()
{
    CTreeViewUI* pTreeView = xFindControl(CTreeViewUI, _T("readstorylist"));
    int i = pTreeView->GetCurSel();
    if (i < 0)
    {
        return;
    }

    CTreeNodeUI* pTreeNode = static_cast<CTreeNodeUI*>(pTreeView->GetItemAt(i));
    TRACE(_T("selected:%s\n"), pTreeNode->GetUserData().GetData());
    if (pTreeNode->GetTag() == 0)
    {
        ::KillTimer(GetHWND(), TIMER_ID_READ_FOLLOW);
        CString strTmp;
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("storyreadnametxt"));

        //CButtonUI* pBtn = xFindControl(CButtonUI, _T("storystartbtn"));
        //pBtn->SetText(_T("开  始"));
        //StopSpeak();
        ReadSendStopClick();

        strTmp.Format(_T("%s>%s"),
            m_strReadParentName, pTreeNode->GetItemText().GetData());
        pLbl->SetText(strTmp);

        CString strPath;
        CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        pREdt->SetText(_T(""));

        strPath = pTreeNode->GetUserData();

        //显示/隐藏“全文翻译”按钮
        //自定义格式：Data\\ReadingMaterial\\自定义\\文章.txt
        //预定义格式：Data\\ReadingMaterial\\故事\\阿尔吉农挽歌\\英文.txt
        CButtonUI* pBtn = xFindControl(CButtonUI, _T("readalltranslatebtn"));
        i = strPath.ReverseFind(_T('\\'));
        strTmp.Empty();
        if (i >= 0)
        {
            CString strTmp2 = strPath.Left(i);
            i = strTmp2.ReverseFind(_T('\\'));
            if (i >= 0)
            {
                strTmp = strTmp2.Right(strTmp2.GetLength() - i - 1);
            }
        }
        if (strTmp.IsEmpty())
        {
            strTmp = _T("0");
        }
        else
        {
            strTmp.Replace(_T("'"), _T("''"));
            strTmp = GetSingleField(_T("SELECT count(*) from 自定义素材 where 路径='%s'"),
                strTmp);
        }
        pBtn->SetVisible(strTmp == _T("1") ? false : true);

        CFileFind finder;
        if (!finder.FindFile(strPath))
        {
            return;
        }
        finder.Close();

        MyReadAsciiFile(strPath, strTmp);
        m_strReadContentPath = strPath;

        strTmp = CSplitSentence::GetSplitSentence(strTmp);
        m_strReadSentences = strTmp;
        strTmp.Replace(_T("<p>"), _T(""));
        strTmp.Replace(_T("<d>"), _T(""));
        strTmp.Replace(_T("|"), _T("\n"));
        pREdt->SetText(strTmp);
        pREdt->SetSel(0, 0);

        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSelNone();
        pREdt->SetFocus();

        pREdt->SetModify(false);
        m_pReadTextDoc->SetSaved(tomTrue);
        //pREdt->SetReadOnly(true);

        if (m_nReadType == READ_TYPE_DICTA)
        {
            pREdt = xFindControl(CRichEditUI, _T("dictationansweredt"));
            pREdt->SetText(_T(""));
            pREdt = xFindControl(CRichEditUI, _T("dictationsouceedt"));
            pREdt->SetText(_T(""));
            pREdt->SetTextColor(DUI_BLACK);

            InitReadCtrls(READ_TYPE_DICTA);
        }
        else if (READ_TYPE_RECITE == m_nReadType)
        {
            InitReadCtrls(READ_TYPE_RECITE);
        }
    }
}

void MainFrame::ReadSpeackCurrentSentence()
{
    ASSERT(m_sptReadSentences.size() > 0);
    ASSERT(m_nReadCurrentSentence >=0 && m_nReadCurrentSentence < m_sptReadSentences.size());
    CString strTmp;
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("dictationcurrenttxt"));
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dictationsouceedt"));
    StopSpeak();

    strTmp.Format(_T("%d"), m_nReadCurrentSentence+1);
    pLbl->SetText(strTmp);

    pLbl = xFindControl(CLabelUI, _T("dictationinfotxt"));
    pLbl->SetText(_T(""));

    pREdt->SetText(_T(""));

    pREdt = xFindControl(CRichEditUI, _T("dictationansweredt"));
    pREdt->SetText(_T(""));

    Speak(m_sptReadSentences[m_nReadCurrentSentence].c_str());
}

HRESULT MainFrame::ReadKeyMouseEvent(MSGFILTER* pMsg)
{
    //WM_LBUTTONDOWN, WM_KEYUP
    TRACE(_T("recv key id:%d\n"), pMsg->msg);

    return 0L;
}

CString MainFrame::ReadHelpGetSubString(TCHAR chars[], int num)
{
    long lStart, lEnd;
    CString strTmp;
    int startIndex, i, j;
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

    pREdt->GetSel(lStart, lEnd);
    strTmp = pREdt->GetText().Left(lStart);

    startIndex = 0;
    for (j = 0; j < num; j++)
    {
        i = strTmp.ReverseFind(chars[j]);
        if (i > startIndex)
        {
            startIndex = i;
        }
    }
    /*
    startIndex = strTmp.ReverseFind(_T(' '));
    i = strTmp.ReverseFind(_T(','));
    if (i > startIndex)
    {
        startIndex = i;
    }
    i = strTmp.ReverseFind(_T('.'));
    if (i > startIndex)
    {
        startIndex = i;
    }
    i = strTmp.ReverseFind(_T('?'));
    if (i > startIndex)
    {
        startIndex = i;
    }
    i = strTmp.ReverseFind(_T('!'));
    if (i > startIndex)
    {
        startIndex = i;
    }
    i = strTmp.ReverseFind(_T('\r'));
    if (i > startIndex)
    {
        startIndex = i;
    }

    if (startIndex < 0)
    {
        startIndex = 0;
    }
    */
    return strTmp.Mid(startIndex);
}

void MainFrame::ReadDoHelp(const CString& strValue, int vkValue)
{
    int opt;
    if (vkValue >= (int)'0' &&
        vkValue <= (int)'9')
    {
        opt = vkValue - (int)'0';
    }
    else if (vkValue >= VK_NUMPAD0 &&
        vkValue <= VK_NUMPAD9)
    {
        opt = vkValue - VK_NUMPAD9;
    }
    if (opt == 0)
    {
        opt = 10;
    }

    //CContainerUI* pReadHelpWnd = xFindControl(CContainerUI, _T("readwordhelp"));
    CListUI* pList = xFindControl(CListUI, _T("readhlplistlist"));
    if (pList->GetCount() >= opt)
    {
        pList->SelectItem(opt - 1);

#if 1
        //原版本已知bug：
        //1，建议词如果有大写（大小写不符合），则不会被清除，如按g时的第一个建议词G，会被直接附加到g后面
        //2、建议词中的重复字（词）会都被清除;如按g时的第二个建议词gage，会得到gae
        CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
        long lStart, lEnd;
        pREdt->GetSel(lStart, lEnd);
        //所以不可直接替换
        //CString strTmp = m_pReadHelpWnd->GetHelpString();
        //strTmp.Replace(strValue, _T(""));
        //直接跳过前面字符
        CString strTmp = pList->GetItemAt(pList->GetCurSel())->GetUserData();
        pREdt->InsertText(lStart, strTmp.Mid(strValue.GetLength()), true);

        INPUT ki[2];
        int i;
        SHORT vk = VK_BACK;
        memset(ki, 0, sizeof(ki));
        for (i = 0; i < 2; i++)
        {
            ki[i].type = INPUT_KEYBOARD;
            //???
            //ki[i].ki.dwExtraInfo = GetExceptionInformation();
        }
        ki[0].ki.wVk = vk;
        ki[1].ki.wVk = vk;
        ki[1].ki.dwFlags |= KEYEVENTF_KEYUP;
        ::SendInput(_countof(ki), ki, sizeof(INPUT));
#else
        INPUT ki[2];
        int i;
        SHORT vk = VK_BACK;
        memset(ki, 0, sizeof(ki));
        for (i = 0; i < 2; i++)
        {
            ki[i].type = INPUT_KEYBOARD;
            //???
            //ki[i].ki.dwExtraInfo = GetExceptionInformation();
        }
        ki[0].ki.wVk = vk;
        ki[1].ki.wVk = vk;
        ki[1].ki.dwFlags |= KEYEVENTF_KEYUP;
        ::SendInput(_countof(ki), ki, sizeof(INPUT));

        CString strTmp = m_pReadHelpWnd->GetHelpString();
        strTmp.Replace(strValue, _T(""));

#if defined(_UNICODE) || defined(UNICODE)
        UINT fmt = CF_UNICODETEXT;
#else
        UINT fmt = CF_TEXT;
#endif
        do 
        {
            if (!IsClipboardFormatAvailable(fmt))
            {
                break;
            }
            if (!::OpenClipboard(GetHWND()))
            {
                break;
            }

            ::EmptyClipboard();

            int sz = (strTmp.GetLength() + 1);
            HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, sz * sizeof(TCHAR)); 
            if (hglbCopy == NULL) 
            { 
                CloseClipboard(); 
                break;
            } 

            // Lock the handle and copy the text to the buffer. 

            LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
            memcpy(lptstrCopy, strTmp.GetBuffer(), sz * sizeof(TCHAR));
            strTmp.ReleaseBuffer();
            lptstrCopy[sz] = _T('\0');
            GlobalUnlock(hglbCopy); 

            // Place the handle on the clipboard. 
            SetClipboardData(fmt, hglbCopy);

            CloseClipboard();

            CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
            pREdt->Paste();
        }while(FALSE);
#endif
    }
}

void MainFrame::ReadKeyDown(WPARAM wParam, LPARAM lParam)
{
    //TCHAR kbuf[1024];
    //::GetKeyNameText(lParam, kbuf, 1024);
    //UINT uc = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
    //TCHAR tv = (TCHAR)(LOWORD(uc));
    //SHORT ss = GetKeyState(VK_SHIFT);
    //TRACE(_T("%d, press:%c, vkcode:%d, nametext:%s, shiftstate:%s\n"),
    //    uc, tv ? tv : '?', (int)wParam, kbuf, HIWORD(ss) ? _T("pressed") : _T("normal"));
    
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));
    //如果为只读，则不处理
    if (pREdt->IsReadOnly())
    {
        TRACE(_T("readcontenttxt is readonly.\n"));
        return;
    }
    TCHAR vKCode;
    int   vkValue = (int)wParam;
    // 按下shift、ctrl等，忽略
    if (vkValue == VK_SHIFT ||
        vkValue == VK_CONTROL)
    {
        return;
    }

    vKCode = (TCHAR)LOWORD(MapVirtualKey(wParam, MAPVK_VK_TO_CHAR));
    bool bShift = HIWORD(GetKeyState(VK_SHIFT)) ? true : false;
    
    CString strTmp, strSpeak;
    //long lStart, lEnd;

    static TCHAR chars1[] = {_T(' '), _T(','), _T('.'),  _T('?'), _T('!'), _T('\r')};
    static TCHAR chars2[] = {_T(','), _T('.'),  _T('?'), _T('!'), _T('\r')};

    if (m_bReadSpeakLetter && isalpha(vKCode, MainFrame::m_loc))
    {
        StopSpeak();
        strSpeak = vKCode;
        Speak(strSpeak);
    }

    if (m_bReadSpeakWord &&
        ( (vKCode == _T(' ')) ||
          (vkValue == VK_OEM_COMMA) ||
          (vkValue == VK_OEM_PERIOD) ||
          (bShift && (vkValue == VK_OEM_2)) ||  //'/?' for US
          (bShift && (vkValue == 0x31)) )     //0x31==>'1'
          )
    //if (m_bReadSpeakWord &&
    //    ( (vKCode == _T(' ')) ||
    //    (vKCode == _T(',')) ||
    //    (vKCode == _T('.')) ||
    //    (bShift && (vKCode == _T('/'))) ||  //press ?
    //    (bShift && (vKCode == _T('1'))) )   //press !
    //    )
    {
        strSpeak = ReadHelpGetSubString(chars1, _countof(chars1));
        if (!strSpeak.IsEmpty())
        {
            StopSpeak();
            Speak(strSpeak);
        }
    }

    if (m_bReadSpeakSentence &&
        ( (vkValue == VK_OEM_COMMA) ||
        (vkValue == VK_OEM_PERIOD) ||
        (bShift && (vkValue == VK_OEM_2)) ||
        (bShift && (vkValue == 0x31)) ||
        (vkValue == VK_RETURN) )
        )
    {
        strSpeak = ReadHelpGetSubString(chars2, _countof(chars2));
        if (!strSpeak.IsEmpty())
        {
            StopSpeak();
            Speak(strSpeak);
        }
    }

    CContainerUI* pReadHelpWnd = xFindControl(CContainerUI, _T("readwordhelp"));
    if (m_bReadHelper && 
        isalpha(vKCode, MainFrame::m_loc))
    {
        strSpeak = ReadHelpGetSubString(chars1, _countof(chars1));
        strSpeak.Trim();
        strSpeak += vKCode;
        if (strSpeak.IsEmpty())
        {
            pReadHelpWnd->SetVisible(false);
            pREdt->SetFocus();
        }
        else
        {
            if (ReadUpdateHelpList(strSpeak))
            {
                ReadReLayoutHelp();
                pReadHelpWnd->SetVisible(true);
                pREdt->SetFocus();
            }
            else
            {
                pReadHelpWnd->SetVisible(false);
            }
        }
    }
    else
    {
        CListUI* pList = xFindControl(CListUI, _T("readhlplistlist"));
        if (m_bReadHelper && pReadHelpWnd->IsVisible() &
            (pList->GetCount() > 0) &&
            ( (vkValue >= (int)'0' && vkValue <= (int)'9') || (vkValue >= VK_NUMPAD0 && vkValue <= VK_NUMPAD9) )
            )
        {
            strSpeak = ReadHelpGetSubString(chars1, _countof(chars1));
            strSpeak.Trim();
            if (strSpeak.IsEmpty())
            {
                return;
            }
            //vKCode还未显示
            //strSpeak.TrimRight(vKCode);
            ReadDoHelp(strSpeak, vkValue);
        }
        pReadHelpWnd->SetVisible(false);
    }
}

void MainFrame::SREndStream(CSpEvent& event)
{
    SendMessage(WM_SRENDSTREAM);
    //PostMessage(WM_SRENDSTREAM);
    //m_bRecognizerInput = false;
}

void MainFrame::ReadSpeakNext_()
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("readcontenttxt"));

    if (m_nReadToPostion < pREdt->GetText().GetLength())
    {
        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSel(m_nReadToPostion + 1, m_nReadToPostion + 1);
        int nFrom, nTo;
        ReadGetSentencePosition_(m_nReadToPostion  + 1, nFrom, nTo);
        ReadStartSpeak_(nFrom, nTo);
    }
    else
    {
        //stop
        TNotifyUI msg;
        msg.pSender = xFindControl(CControlUI, _T("storystartbtn"));
        msg.sType = DUI_MSGTYPE_CLICK;
        //Notify(msg);
        m_PaintManager.SendNotify(msg);
    }
}

DWORD MainFrame::SpeechRecognition_Start_()
{
    DWORD Hr = ERROR_SUCCESS;
    CString strTmp;

    do 
    {
        //m_cpGrammar->SetDictationState(SPRS_INACTIVE);
        BOOL bOK = ::DeleteFile(m_strCommonWaveFile);
#ifdef _DEBUG
        if (!bOK)
        {
            TRACE(_T("Delete %s failed:%s\n"), m_strCommonWaveFile, MyFormatMessage(GetLastError()));
        }
#endif
        Hr = m_WaveRecorder.StartRecord(m_strCommonWaveFile);
    } while (FALSE);

    return Hr;
}

DWORD MainFrame::SpeechRecognition_Start()
{
    DWORD Hr = ERROR_SUCCESS;
    CString strTmp;

    do 
    {
        //m_cpGrammar->SetDictationState(SPRS_INACTIVE);
        //StopWaveRecordReco();
        //BOOL bOK = ::DeleteFile(m_strCommonWaveFile);
        //if (!bOK)
        //{
        //    DWORD dwErr = GetLastError();
        //    TRACE(_T("Delete %s failed:0x%x\n"), m_strCommonWaveFile, dwErr);
        //    ////ERROR_SHARING_VIOLATION
        //    //if (ERROR_FILE_NOT_FOUND != dwErr)
        //    //{
        //    //    ::SetTimer(GetHWND(), TIMER_ID_RECOINPUT, 500, NULL);
        //    //    break;
        //    //}
        //}
        //for safer
        m_WaveRecorder.StopRecord();
        Hr = m_WaveRecorder.StartRecord(m_strCommonWaveFile);
    } while (FALSE);

    return Hr;

    //if (m_bRecognizerInput)
    //{
    //    ::SetTimer(GetHWND(), TIMER_ID_RECOINPUT, 500, NULL);
    //    return ERROR_SUCCESS;
    //}
    //else
    //{
    //    return SpeechRecognition_Start_();
    //}
}

HRESULT MainFrame::SpeechRecognition_Stop()
{
    StopWaveRecordReco();

    //if (m_strCloseFlag == CF_CLOSE)
    //{
    //    return S_OK;
    //}
    HRESULT Hr = S_OK;

    //if (m_hAiEngine)
    //{
    //    TRACE(_T("使用新语音识别引擎,do nothing here。\n"));
    //    SendMessage(WM_SRENDSTREAM);
    //    return Hr;
    //}

    if (m_ssEvalMgr.IsOpen())
    {
        TRACE(_T("使用新语音识别引擎,do nothing here。\n"));
        SendMessage(WM_SRENDSTREAM);
        return Hr;
    }
#if 1
    CString strTmp;
    CComPtr<ISpStream> cpStream;
    CSpStreamFormat Fmt;
    
    do 
    {
        Hr = cpStream.CoCreateInstance(CLSID_SpStream);
        if (FAILED(Hr))
        {
            break;
        }
        //see m_WaveRecorder.Open()...
        //新引擎，wav格式支持16KHz。SPSF_16kHz16BitMono
        Hr = Fmt.AssignFormat(SPSF_16kHz16BitMono);
        if (FAILED(Hr))
        {
            break;
        }
        //m_bRecognizerInput = true;
        cpStream->BindToFile(m_strCommonWaveFile, SPFM_OPEN_READONLY, &Fmt.FormatId(), Fmt.WaveFormatExPtr(), NULL);

        m_cpRecognizer->SetInput(cpStream, TRUE);

        int i;
        for (i = 0; i < _countof(m_strRecognitionCompare); i++)
        {
            m_strRecognitionCompare[i].Empty();
        }

        m_cpGrammar->SetDictationState(SPRS_ACTIVE);
    } while (FALSE);

    if (cpStream)
    {
        cpStream.Release();
    }
#endif
    return Hr;
}

void MainFrame::SRRecognition(CSpEvent& event)
{
    CComPtr<ISpRecoResult> cpResult = event.RecoResult();
    SPPHRASE               *pPhrase = NULL;
    SPPHRASE               *pAltPhrase = NULL;
    CComPtr<ISpPhraseAlt>  pcpAlter[MY_MAX_ALTERNATES];
    ULONG   ulCount;
    CSpDynamicString dstrText;
    HRESULT Hr;
    ULONG  i, j, k, l;
    CString strTmp;
    WCHAR w;

    if (!cpResult)
    {
        return;
    }
    do 
    {
        //for (i = 0; i < _countof(m_strRecognitionCompare); i++)
        //{
        //    m_strRecognitionCompare[i].Empty();
        //}

        Hr = cpResult->GetPhrase(&pPhrase);
        if (FAILED(Hr))
        {
            break;
        }
        Hr = cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
        if (FAILED(Hr))
        {
            break;
        }
        TRACE("I heard:%S\n", dstrText.m_psz);
        //myW2T(m_strRecognitionCompare[1], lstrlenW(dstrText.m_psz), dstrText.m_psz);
        m_strRecognitionCompare[1] += (LPCTSTR)CW2T(dstrText.m_psz);
        m_strRecognitionCompare[1] += _T(" ");

        Hr = cpResult->GetAlternates(0, SPPR_ALL_ELEMENTS, MY_MAX_ALTERNATES, (ISpPhraseAlt **)pcpAlter, &ulCount);
        if (FAILED(Hr))
        {
            break;
        }

        ULONG nNewCount = min(MY_MAX_ALTERNATES, ulCount);
        for (i = 0; i < nNewCount; i++)
        {
            dstrText.Clear();
            pcpAlter[i]->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
            m_strRecognitionCompare[i+2] += (LPCTSTR)CW2T(dstrText.m_psz);
            m_strRecognitionCompare[i+2] += _T(" ");
        }

        m_strRecognitionCompare[9].Empty();

        for (i = 0; i < pPhrase->Rule.ulCountOfElements; i++)
        {
            l = lstrlenW(pPhrase->pElements[i].pszPronunciation);
            for (j = 0; j < l; j++)
            {
                w = pPhrase->pElements[i].pszPronunciation[j];
                strTmp.Format(_T("%d,"), (int)w);
                m_strRecognitionCompare[9] += strTmp;
            }
        }

        for (i = 0; i < nNewCount; i++)
        {
            Hr = pcpAlter[i]->GetPhrase(&pAltPhrase);
            if (FAILED(Hr))
            {
                TRACE(_T("pcpAlter[%d]->GetPhrase failed:0x%x\n"), i, Hr);
                break;
            }
            for (k = 0; k < pAltPhrase->Rule.ulCountOfElements; k++)
            {
                l = lstrlenW(pAltPhrase->pElements[k].pszPronunciation);
                for (j = 0; j < l; j++)
                {
                    w = pAltPhrase->pElements[k].pszPronunciation[j];
                    strTmp.Format(_T("%d,"), (int)w);
                    m_strRecognitionCompare[9] += strTmp;
                }
            }
            ::CoTaskMemFree(pAltPhrase);
        }

        m_strRecognitionCompare[9].TrimRight(_T(','));
    } while (FALSE);

    if (pPhrase) ::CoTaskMemFree(pPhrase);

    cpResult.Release();
}

int MainFrame::SpeechRecognitionCompare(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType/* = ct_sent*/)
{
    int i;

#ifdef APP_DEMO
    m_strRecognitionCompare[8] = _T("0");
    return 0;
#endif

    //if (m_hAiEngine)
    //{
    //    TRACE(_T("使用新语音识别引擎\n"));
    //    return SpeechRecognitionCompare2(strSpokenText, strErrMsg, nCoreType);
    //}

    if (m_ssEvalMgr.IsOpen())
    {
        TRACE(_T("使用先声语音识别引擎\n"));
        return SingSoundPronEval(strSpokenText, strErrMsg, nCoreType);
    }

    CString strTmp, strLoclSrc;
    strErrMsg.Empty();

    if (strSpokenText.IsEmpty())
    {
        strErrMsg = _T("参考语句为空。");
        return 1;
    }

    for (i = 1; i <= MY_MAX_ALTERNATES+1; i++)
    {
        if (m_strRecognitionCompare[i].IsEmpty())
        {
            strErrMsg = _T("语音识别失败。");
            return 2;
        }
    }

    strLoclSrc = strSpokenText;
    strLoclSrc.Trim();
    strLoclSrc.MakeLower();
    strLoclSrc.Replace(_T('!'), _T(' '));
    strLoclSrc.Replace(_T(','), _T(' '));
    strLoclSrc.Replace(_T('.'), _T(' '));
    strLoclSrc.Replace(_T('?'), _T(' '));
    strLoclSrc.Trim();

    m_strRecognitionCompare[1].Trim();
    TRACE(_T("orig sentence:%s\n"), strLoclSrc);
    TRACE(_T("I heard sentence:%s\n"), m_strRecognitionCompare[1]);

    m_strRecognitionCompare[0] = strLoclSrc;
    while (m_strRecognitionCompare[0].Find(_T("  ")) >= (int)0)
    {
        m_strRecognitionCompare[0].Replace(_T("  "), _T(" "));
        m_strRecognitionCompare[0].Trim();
    }
    m_strRecognitionCompare[0].Trim();

    SplitResultVector strArray[7];
    SplitResultVector::const_iterator p;
    for (i = 0; i < _countof(strArray); i++)
    {
        strTmp = m_strRecognitionCompare[i].MakeLower();
        SplitString(TString(strTmp), _T(" "), strArray[i]);
    }

    /*
      原句和识别的句子（m_strRecognitionCompare[1]、Alternates（m_strRecognitionCompare[2~6]）逐个比较，
      将不可识别的单词放到m_strRecognitionCompare[7]
    */
    for (i = 1; i < _countof(strArray); i++)
    {
        m_strRecognitionCompare[7].Empty();
        for (p = strArray[0].begin(); p != strArray[0].end(); ++p)
        {
            if (find(strArray[i].begin(), strArray[i].end(), *p) == strArray[i].end())
            {
                m_strRecognitionCompare[7] += p->c_str();
                m_strRecognitionCompare[7] += _T(" ");
            }
        }

        m_strRecognitionCompare[7].Trim();
        SplitString(TString(m_strRecognitionCompare[7]), _T(" "), strArray[0]);
    }
    TRACE(_T("unknown word:%s\n"), m_strRecognitionCompare[7]);

    //计算识别率
    SplitString(TString(m_strRecognitionCompare[0]), _T(" "), strArray[0]);
    SplitString(TString(m_strRecognitionCompare[7]), _T(" "), strArray[1]);

    ASSERT(strArray[0].size() != 0);
    i = (int)((strArray[0].size() - strArray[1].size()) * 100.0f / strArray[0].size());
    m_strRecognitionCompare[8].Format(_T("%d"), i);
    TRACE(_T("all word:%d, unknown word:%d, so we get:%d\n"), strArray[0].size(), strArray[1].size(), i);

    //////////////////////////////////////////////////////////////////////////
    //以下计算音标识别

    m_strRecognitionCompare[10].Empty();

    SplitString(TString(strLoclSrc), _T(" "), strArray[0]);

    //获取单词的音标
    for (p = strArray[0].begin(); p != strArray[0].end(); ++p)
    {
        strTmp = p->c_str();
        strTmp.Replace(_T("'"), _T("''"));
        strTmp = GetSingleField(_T("Select 音标 from 单词 where 单词='%s'"),
                    strTmp);
        strTmp.Replace(_T("/"), _T(""));
        m_strRecognitionCompare[10] += strTmp + _T(" ");
    }

    m_strRecognitionCompare[10].Trim();
    strTmp = m_strRecognitionCompare[10];
    m_strRecognitionCompare[9].Trim();
    TRACE(_T("orig symbol:%s\n"), m_strRecognitionCompare[10]);
    if (m_strRecognitionCompare[9].IsEmpty())
    {
        m_strRecognitionCompare[11] = strTmp;
    }
    else
    {
        //如果找到音标，则在m_strRecognitionCompare[10]中将之清为空格" "
        _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select Symbol from 音标 where PhoneId in (%s) Order by Len(Symbol) desc"),
            m_strRecognitionCompare[9]);
        if (pRecdPtr)
        {
            while(!pRecdPtr->GetadoEOF())
            {
                strTmp.Replace(GetCollect(pRecdPtr, (long)0), _T(" "));

                pRecdPtr->MoveNext();
            }
            CDbFile::CloseRecord(pRecdPtr);
        }

        //此时strTmp剩下不能识别的音标
        m_strRecognitionCompare[11].Empty();
        pRecdPtr = GetTablesBySql(_T("%s"), _T("Select Symbol from 音标 Order by Len(Symbol) desc"));
        if (pRecdPtr)
        {
            while(!pRecdPtr->GetadoEOF())
            {
                if (strTmp.Find(GetCollect(pRecdPtr, (long)0)) >= (int)0)
                {
                    m_strRecognitionCompare[11] += GetCollect(pRecdPtr, (long)0) + _T(",");
                    strTmp.Replace(GetCollect(pRecdPtr, (long)0), _T(" "));
                }

                pRecdPtr->MoveNext();
            }
        }
        if (!m_strRecognitionCompare[11].IsEmpty())
        {
            m_strRecognitionCompare[11].TrimRight(_T(','));
        }
    }
    TRACE(_T("unknown symbol:%s\n"), m_strRecognitionCompare[11]);

    TRACE(_T("first num:%s\n"), m_strRecognitionCompare[8]);
    if (m_strRecognitionCompare[10].GetLength() > (int)0)
    {
        //音标识别率占75%
        i = (int)((m_strRecognitionCompare[10].GetLength() - m_strRecognitionCompare[11].GetLength()) * 1.0f / m_strRecognitionCompare[10].GetLength() * 75.0f)
            + (int)(_tstoi(m_strRecognitionCompare[8]) / 4.0f);
        m_strRecognitionCompare[8].Format(_T("%d"), i);
    }

    TRACE(_T("second num:%s\n"), m_strRecognitionCompare[8]);
    if (IsNumeric(m_strRecognitionCompare[8]))
    {
        i = abs(_tstoi(m_strRecognitionCompare[8]));
        if (i < 60)
        {
            i = (int)(i * 1.5f);
        }
        m_strRecognitionCompare[8].Format(_T("%d"), i);
    }
//    // 基础版本，评分总为0 [2016-1-15 Arthur]
//#ifdef APP_DEMO
//    m_strRecognitionCompare[8].Format(_T("%d"), 0);
//#endif
    return 0;
}

void MainFrame::OnEvalReply(const char *id, int nType, const void *lpMessage, int nSize)
{
    TRACE("type:%d, size:%d\n", nType, nSize);
    if (nType == SSOUND_MESSAGE_TYPE_JSON)
    {
        m_strRecognitionCompare[8].Format(_T("%d"), -1);
        m_strRecognitionCompare[7].Empty();
        m_strRecognitionCompare[11].Empty();

        do 
        {
            Json::Reader jreader;
            Json::Value jroot;
            //CA2W wMsg((LPCSTR)lpMessage);
            //int nLen = AtlUnicodeToUTF8(wMsg, -1, NULL, 0);//::WideCharToMultiByte(CP_UTF8, 0, wMsg, -1, NULL, 0, NULL, NULL);
            //string sJson;
            //sJson.resize(nLen);
            //nLen = AtlUnicodeToUTF8(wMsg, -1, &sJson[0], nLen);//::WideCharToMultiByte(CP_UTF8, 0, wMsg, -1, &sJson[0], nLen, NULL, NULL);
            string sJson = (LPCSTR)lpMessage;
//#ifdef _DEBUG
//            m_flog->WriteString(_T("Reftext:\n"));
//            m_flog->WriteString(pFrame->m_strAiRefText);
//            m_flog->WriteString(_T("\n"));
//            m_flog->WriteString(_T("Result:\n"));
//            m_flog->WriteString((LPTSTR)CA2T((LPCSTR)lpMessage));
//            m_flog->WriteString(_T("\n\n"));
//#endif
            TRACE("Json:%s\n", sJson.c_str());
            if (!jreader.parse(sJson.c_str(), jroot))
            {
                m_strAiErrMsg = _T("语音识别返回数据格式错误。");
                break;
            }
            //TRACE("version:%s,recordId:%s\n", jroot["version"].asCString(), jroot["tokenId"].asCString());
            Json::Value jresult = jroot["result"];
            if (jresult.isNull())
            {
                //{"errId": 70200, "error": "Network abnormal.", "tokenId": "565fd9ec02dbc2158c000001" }
                Json::Value jerrid = jroot["errId"];
                if (!jerrid.isNull())
                {
                    m_strRecognitionCompare[8].Format(_T("%d"), -2);
                    TRACE(_T("errorid:%d\n"), jerrid.asInt());
                }
                Json::Value jerror = jroot["error"];
#ifdef _DEBUG
                if (!jerror.isNull())
                {
                    TRACE("Json error:%s\n", jerror.asCString());
                }
#endif

                if (!jerrid.isNull() && !jerror.isNull())
                {
                    //UTF8==>unicode
                    m_strAiErrMsg.Format(_T("语音识别失败，错误码：%d，错误描述：%s。请联系客服人员。"),
                        jerrid.asInt(), (LPTSTR)CA2T(jerror.asCString()));
                    //if (jerrid.asInt() == 60015)
                    //{
                    //    m_strAiErrMsg = _T("语音识别失败，请重新启动程序。");
                    //    RegKeySetValue(REG_SpeechSerial, _T(""));
                    //    //并删除%ProgramData%/aiengine/udidinfo
                    //    TCHAR szPath[1024] = {_T('\0'),};
                    //    if (SUCCEEDED(::SHGetFolderPath(pFrame->GetHWND(),
                    //        CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath)))
                    //    {
                    //        ::PathAppend(szPath, _T("aiengine"));
                    //        TRACE(_T("sn failed, delete %s\n"), szPath);
                    //        MyRemoveDir(szPath);
                    //    }
                    //}
                }
                break;
            }
            //得分，在识别结果为0（成功情况下）
            Json::Value jOverall = jresult["overall"];
            if (jOverall.isNull())
            {
                m_strAiErrMsg = _T("语音识别失败，无结果。");
                break;
            }
            TRACE("\noverall:%d\n", jOverall.asInt());
            m_strRecognitionCompare[8].Format(_T("%d"), jOverall.asInt());
#ifdef _DEBUG
            //if (jOverall.asInt() < 50)
            {
                Json::Value jinfo = jresult["info"];
                if (!jinfo.isNull())
                {
                    TRACE("tipId:%d\n", jinfo["tipId"].asUInt());
                    if (jinfo["tipId"].asUInt() != 0)
                    {
                        TRACE(_T("tips:%s\n"), SSEvalMgr::GetErrMsg(jinfo["tipId"].asUInt()).c_str());
                        //break;
                    }
                }
            }
#endif

            Json::Value jdetails = jresult["details"];
            if (jdetails.isNull())
            {
                TRACE(_T("has no details!!\n"));
                break;
            }
            Json::UInt i = 0, count = jdetails.size();
            if (count <= 0)
            {
                TRACE(_T("has %d details!!\n"), count);
                break;
            }
            CString strWords;
            Json::Value ele;
            Json::Value score;
            Json::Value jchar;
            Json::Value jtext;
            m_aReadbadWord.clear();
            if (MainFrame::ct_pred == m_nAiCoreType)
            {
                for (i = 0; i < count; i++)
                {
                    ele = jdetails[i];
                    score = ele["score"];
                    jtext = ele["text"];
                    if (score.isNull() ||
                        jtext.isNull())
                    {
                        continue;
                    }
                    m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jtext.asCString())));
                }
            }
            else if (MainFrame::ct_sent == m_nAiCoreType)
            {
                for (i = 0; i < count; i++)
                {
                    //得分<=80情况下，单词未识别
                    ele = jdetails[i];
                    score = ele["score"];
                    jchar = ele["char"];
                    if (jchar.isNull() ||
                        score.isNull())
                    {
                        continue;
                    }
                    //TRACE("%d: score:%d, char:%s\n", i+1, ele["score"].asUInt(), ele["char"].asCString());
                    if (ele["score"].asUInt() <= 80)
                    {
                        strWords += (LPTSTR)CA2T(jchar.asCString());
                        strWords += _T(" ");
                        TRACE(_T("%s is bad:%d\n"), (LPTSTR)CA2T(jchar.asCString()), ele["score"].asUInt());
                    }
                    m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jchar.asCString())));
                }
                strWords.TrimRight(_T(' '));
                m_strRecognitionCompare[7] = strWords;
            }
            else if (MainFrame::ct_word == m_nAiCoreType)
            {
                i = 0;
                Json::Value ele = jdetails[i];
                Json::Value phone = ele["phone"];
                if (!phone.isNull())
                {
                    count = phone.size();
                    for (i = 0; i < count; i++)
                    {
                        score = phone[i]["score"];
                        jchar = phone[i]["char"];
                        if (jchar.isNull() ||
                            score.isNull())
                        {
                            continue;
                        }
                        m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jchar.asCString())));
                    }
                }
#ifdef _DEBUG
                else
                {
                    TRACE(_T("单词没有音素评分结果！\n"));
                }
#endif
            }
            else
            {
                ;
            }

#if 0       //只有单词才有音素的评分
            Json::Value jstatics = jresult["statics"];
            count = jstatics.size();
            strWords.Empty();
            for (i = 0; i < count; i++)
            {
                //得分<=80情况下，音标未识别
                Json::Value ele = jstatics[i];
                //TRACE("%d: score:%d, char:%s\n", i+1, ele["score"].asUInt(), ele["char"].asCString());
                if (ele["score"].asUInt() <= 80)
                {
                    strWords += (LPTSTR)CA2T(ele["char"].asCString());
                    strWords += _T(",");
                }
            }
            strWords.TrimRight(_T(','));
            pFrame->m_strRecognitionCompare[11] = strWords;
#endif
        } while (FALSE);

        //pFrame->m_bAICompleted = true;
        //SetEventState(TRUE);
        //TRACE(_T("callback out, set m_bAICompleted %d\n"), IsCallbackQuit());
    }
}

int MainFrame::SingSoundPronEval(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType /* = ct_sent */)
{
    int nRet = 0;
    CString strTmp, strSrc = strSpokenText;
    //CString strSpeechSN = RegKeyGetValue(REG_SpeechSerial);
    TCHAR szId[64] = {0};
    strErrMsg.Empty();
    do 
    {
        ASSERT(nCoreType < ct_max);
        if (strSrc.IsEmpty())
        {
            nRet = 1;
            strErrMsg = _T("参考语句为空。");
            break;
        }

        if (ct_pred == nCoreType)
        {
            nRet = m_ssEvalMgr.EvalPred( m_strCommonWaveFile, strSpokenText, szId );
        } else if (ct_sent == nCoreType)
        {
            nRet = m_ssEvalMgr.EvalSentence( m_strCommonWaveFile, strSpokenText, szId );
        } else if (ct_word == nCoreType)
        {
            nRet = m_ssEvalMgr.EvalWord( m_strCommonWaveFile, strSpokenText, szId );
        }

        if (-1 == nRet)
        {
            strErrMsg.Format(_T("打开录音文件失败：%s"), m_strCommonWaveFile.GetString());
        } else if (-2 == nRet)
        {
            strErrMsg = _T("无法完成语音识别，请重新启动程序。");
        }

        CMFCDlgSR DlgSr(this);
        DlgSr.DoModal();

        if (!m_strAiErrMsg.IsEmpty())
        {
            strErrMsg = m_strAiErrMsg;
            nRet = -3;
        }

    } while (FALSE);

    return nRet;
}

int MainFrame::SpeechRecognitionCompare2(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType/* = ct_sent*/)
{
    int nRt = 0;
    CString strTmp, strSrc = strSpokenText;
    CString strSpeechSN = RegKeyGetValue(REG_SpeechSerial);

    strErrMsg.Empty();
    m_strAiErrMsg.Empty();

    do 
    {
        ASSERT(nCoreType < ct_max);
        if (strSrc.IsEmpty())
        {
            nRt = 1;
            strErrMsg = _T("参考语句为空。");
            break;
        }
        if (strSpeechSN.IsEmpty())
        {
            nRt = 1;
            strErrMsg = _T("无法完成语音识别，请重新启动程序。");
            break;
        }
        //strSrc.Trim();
        //strSrc.Replace(_T("\""), _T(""));
        //strSrc.Replace(_T("\r"), _T(" "));
        TRACE(_T("before:%s\n"), strSrc);
        m_AiEngine.CheckRefText(strSrc);
        TRACE(_T("after:%s\n"), strSrc);
        m_strRecognitionCompare[0] = strSrc;
        CString strParam, strRefTxt;
        if (ct_pred == nCoreType)
        {
            strRefTxt.Format(_T("{\"qid\": \"PAPER-000021-QT-000031\",\"lm\": \"%s\"}"), strSrc);
        }
        else
        {
            strRefTxt.Format(_T("\"%s\""), strSrc);
        }
        //strParam.Format(_T("{\"coreProvideType\": \"native\", \"app\": {\"userId\": \"yx_demo\"}, \"audio\": {\"audioType\": \"wav\",\"sampleRate\": 16000,\"channel\": 1,\"sampleBytes\": 2}, \"request\": {\"coreType\": \"en.sent.score\",\"refText\":\"%s\"}}"),
        //    strSrc);
        //段落必须指定precision??否则参数错误
        static LPCTSTR SR_TypeStr[] =
        {
            _T("en.pred.exam"),
            _T("en.sent.score"),
            _T("en.word.score")
        };
        strParam.Format(_T("\
{\
    \"coreProvideType\": \"native\",\
    \"serialNumber\": \"%s\",\
    \"app\":\
    {\
        \"userId\": \"ny_demo\"\
    },\
    \"audio\":\
    {\
        \"audioType\": \"wav\",\
        \"sampleRate\": %d,\
        \"channel\": %d,\
        \"sampleBytes\": %d\
    },\
    \"request\":\
    {\
        \"coreType\": \"%s\",\
        \"refText\": %s,\
        \"rank\": 100,\
        \"precision\": 1,\
        \"rank\": 100\
    }\
}"),
strSpeechSN,
g_dwSamplesPerSec, g_wChannel, g_wBitsPerSample / 8,
SR_TypeStr[nCoreType],
strRefTxt);
        TRACE(_T("compare:%s\n"), strRefTxt);
        TRACE(_T("param:%s\n"), strParam);
        BYTE bID[AI_DAT_LEN+1] = {0};
        m_strAiRefText = strSrc;
        m_nAiCoreType = nCoreType;
        CFile fVoice;
        CFileException fe;
        if (!fVoice.Open(m_strCommonWaveFile, CFile::modeRead, &fe))
        {
            //fe.ReportError();
            TRACE(_T("open %s failed\n"), m_strCommonWaveFile);
            TCHAR szErr[1024];
            fe.GetErrorMessage(szErr, _countof(szErr));
            strErrMsg.Format(_T("打开录音文件失败：%s"), szErr);
            nRt = 2;
            break;
        }
        if (fVoice.GetLength() <= sizeof(WAVEHEADER))
        {
            //MyAfxMessageBox(_T("录音文件长度不正确：%d"),  (int)fVoice.GetLength());
            strErrMsg.Format(_T("录音文件长度不正确：%d"), (int)fVoice.GetLength());
            nRt = 2;
            fVoice.Close();
            break;
        }
        BYTE bDatap[0x1000];
        fVoice.Seek(sizeof(WAVEHEADER), CFile::begin);

        //m_bAICompleted = false;

        if (m_AiEngine.aiengine_start(m_hAiEngine, (LPSTR)CT2A(strParam), bID, &CAiEngine::aiengine_callback, (void*)this) != 0)
        {
            TRACE(_T("aiengine_start失败\n"));
            //AfxMessageBox(_T("启动语音识别失败，请检查程序是否安装正确并尝试重启程序。"));
            strErrMsg = _T("启动语音识别失败，请检查程序是否正确安装并尝试重启程序。");
            nRt = 2;
            fVoice.Close();
            break;
        }
        TRACE("id:%s\n", (LPCSTR)bID);

        UINT nR;
        do 
        {
            nR = fVoice.Read(bDatap, sizeof(bDatap));
        } while (nR > 0 && (m_AiEngine.aiengine_feed(m_hAiEngine, bDatap, nR) == 0) );
        fVoice.Close();
        m_AiEngine.aiengine_stop(m_hAiEngine);
        //wait result
        //while(!m_bAICompleted)
        //{
        //    Sleep(200);
        //}
        //CDialogSR* pDlgSR = new CDialogSR(this);
        //OpenModalWnd(pDlgSR, _T(""));
        CMFCDlgSR DlgSr(this);
        DlgSr.DoModal();

        if (!m_strAiErrMsg.IsEmpty())
        {
            strErrMsg = m_strAiErrMsg;
            nRt = 2;
        }
    } while (FALSE);
    
    return nRt;
}

int MainFrame::SpeechSpokenScoreToLevel()
{
    int s = _tstoi(m_strRecognitionCompare[8]);
    if (s < 10)
    {
        return 0;
    }
    if (s < 25)
    {
        return 1;
    }
    if (s < 50)
    {
        return 2;
    }
    return 3;
}

int MainFrame::ReadCompareWord(__inout CString& SourceText, const CString& WordText)
{
    int nPos = -1, i = 0, len = WordText.GetLength();
    //匹配WordText前后不是字母的"字符串"
    //Dim matchs As MatchCollection = New Regex(("([^a-zA-z])" & WordText & "([^a-zA-z])")).Matches(SourceText)
    while(1)
    {
        i = SourceText.Find(WordText, i);
        //没找到，或者是最前面的...
        if (i <= (int)0)
        {
            break;
        }
        //判断前后是否为[a-zA-z]
        if (!isalpha(SourceText[i-1], MainFrame::m_loc) &&
            !isalpha(SourceText[i+len], MainFrame::m_loc))
        {
            nPos = i;
            break;
        }
        i++;
    };

    if (nPos != -1)
    {
        CString strTmp = SourceText.Left(nPos);
        for (i = 0; i < len; i++)
        {
            strTmp += _T(" ");
        }
        strTmp += SourceText.Mid(nPos + len, SourceText.GetLength() - len - nPos);
        SourceText = strTmp;
    }

    return nPos;
}

void MainFrame::ReadStartRecite_()
{
    CProgressUI* pPrg = xFindControl(CProgressUI, _T("reciteprg"));
    CComboUI* pCombo = xFindControl(CComboUI, _T("recitemancmb"));
    CRichEditUI* pREdit = xFindControl(CRichEditUI, _T("readcontenttxt"));
    CString strAllText;// = pREdit->GetText();

    //if (RECITE_All != pCombo->GetCurSel())
    {
        strAllText.Empty();
        if (m_nReadCurrentSentence < m_sptReadSentences.size())
        {
            strAllText = m_sptReadSentences[m_nReadCurrentSentence].c_str();
        }
    }
    //else
    //{
    //    m_nReadCurrentSentence = m_sptReadSentences.size();
    //}

    if (!strAllText.IsEmpty())
    {
        int i = GetSentenceTime(strAllText);
        pPrg->SetMaxValue(i);
        pPrg->SetValue(0);
        NoticePrompt();
        ::SetTimer(GetHWND(), TIMER_ID_READ_RECITE, 100, NULL);
        SpeechRecognition_Start();
    }
}

void MainFrame::ReadStartRecite()
{
    CButtonUI* pBtn;
    CRichEditUI* pREdt;
    CComboUI* pCmb;
    int i;

    //if (!m_bReadPlaying)
    //{
    //    m_bReadPlaying = true;
    //    pBtn = xFindControl(CButtonUI, _T("recitestartbtn"));
    //    pBtn->SetText(_T("停  止"));
    //}

    //for safer
    if (m_nReadCurrentSentence >= m_sptReadSentences.size())
    {
        return;
    }

    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("reciteprogvlayout"));
    pVLay->SetVisible(true);
    CProgressUI* pPrg = xFindControl(CProgressUI, _T("reciteprg"));
    pPrg->SetValue(0);
    pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
    pBtn->SetEnabled(false);
    pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
    pBtn->SetEnabled(false);

    pCmb = xFindControl(CComboUI, _T("recitemancmb"));
    pCmb->SetEnabled(false);
    i = pCmb->GetCurSel();
    m_bReadReciting = true;
    if (RECITE_2nd == i)    //第二句
    {
        pREdt = xFindControl(CRichEditUI, _T("recitetxt"));
        hlpAppedText(pREdt, m_sptReadSentences[m_nReadCurrentSentence].c_str());
        Speak(m_sptReadSentences[m_nReadCurrentSentence].c_str());
    }
    else
    {
        ReadStartRecite_();
    }
    //m_tmReadSpokenFrom = CTime::GetCurrentTime();
}

void MainFrame::ReadStopRecite()
{
    //CButtonUI* pBtn;

    //if (m_bReadPlaying)
    //{
    //    m_bReadPlaying = false;
    //    pBtn = xFindControl(CButtonUI, _T("recitestartbtn"));
    //    pBtn->SetText(_T("开  始"));
    //}

    StopSpeak();
    StopWaveRecordReco();
    ::KillTimer(GetHWND(), TIMER_ID_READ_RECITE);

    CVerticalLayoutUI* pVLay = xFindControl(CVerticalLayoutUI, _T("reciteprogvlayout"));
    pVLay->SetVisible(false);
    CComboUI* pCmb = xFindControl(CComboUI, _T("recitemancmb"));
    pCmb->SetEnabled(true);
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("recitenextbtn"));
    pBtn->SetEnabled(false);
    pBtn = xFindControl(CButtonUI, _T("recitetipsbtn"));
    pBtn->SetEnabled(false);
    m_nReadCurrentSentence = 0;
}

//////////////////////////////////////////////////////////////////////////
//对话选择界面函数
void MainFrame::InitDialogSelectStatus()
{
    CComboUI* pCombo;
    _RecordsetPtr pRecdPtr;
    CListLabelElementUI* pItem;
    CString strSetup, strTmp;
    int nSel = 0, idx = 0, offset = m_nDialogSelectPageId - 1;

    DialogSelectExpandPage(m_nDialogSelectPageId);

    static LPCTSTR ds_cfg[] =
    {
        _T("Spoken_Scene"),
        _T("Talk_Scene"),
        _T("SceneTalk_Scene")
    };
    static LPCTSTR ds_ctl[] =
    {
        _T("dlgfollowmecmb"),
        _T("dlgtalkcmb1"),
        _T("dlgscenetalkcmb")
    };
    static LPCTSTR ds_sql[] =
    {
        _T("Select * from 跟读场景"),
        _T("Select * from 人机对话场景"),
        _T("Select * from 情景对话场景")
    };

    if (m_nDialogSelectPageId == 4)
    {
        //always select the first
        COptionUI* pOpt = xFindControl(COptionUI, _T("dlgwritehuatiopt"));
        pOpt->Selected(true);
    }
    else
    {
        pCombo = xFindControl(CComboUI, _T("dlgfollowmegrpcmb"));
        pCombo->SetVisible(false);

        strSetup = ReadSetup(ds_cfg[offset]);
        pCombo = xFindControl(CComboUI, ds_ctl[offset]);
        pCombo->RemoveAll();
        pRecdPtr = GetTablesBySql(_T("%s"), ds_sql[offset]);
        if (pRecdPtr)
        {
            while(!pRecdPtr->GetadoEOF())
            {
                pItem = new CListLabelElementUI;
                pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
                strTmp = GetCollect(pRecdPtr, _T("Id"));
                pItem->SetUserData(strTmp);
                if (strTmp == strSetup)
                {
                    nSel = idx;
                }
                pCombo->Add(pItem);

                pRecdPtr->MoveNext();
                idx++;
            }
            pCombo->SelectItem(nSel);
        }
        else
        {
            MyAfxMessageBox(_T("没有可供学习的场景。"));
            ListenNoQuestion();
        }
    }
}

void MainFrame::DialogSelectExpandPage(int nIdx)
{
    static LPCTSTR dialog_slect_pages[] =
    {
        _T("dlgfollowmelayout"),
        _T("dlgtalklayout"),
        _T("dlgscenetalklayout"),
        _T("dlgwrittinglayout"),
    };
    ASSERT(nIdx >= DIALOG_FOLLOWME && nIdx <= DIALOG_WRITTING);

    int i;
    CVerticalLayoutUI* pCtrl;
    
    for (i = 1; i <= _countof(dialog_slect_pages); i++)
    {
        pCtrl = xFindControl(CVerticalLayoutUI, dialog_slect_pages[i-1]);
        pCtrl->SetVisible((nIdx == i) ? true : false);
    }    
}

int MainFrame::IsDialogSelectOpt(const TNotifyUI& msg)
{
    return IsOption(g_dialog_select_opt, _countof(g_dialog_select_opt), msg);
}

void MainFrame::DialogSelectFollowmeCmbChanged()
{
    CString strId;
    CComboUI* pCombo = xFindControl(CComboUI, _T("dlgfollowmecmb"));
    strId = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData();
    CString strTmp = GetSingleField(_T("select 是否导入 from 跟读场景 where Id=%s"),
        strId);

    pCombo = xFindControl(CComboUI, _T("dlgfollowmegrpcmb"));
    if (strTmp == _T("1"))
    {
        pCombo->RemoveAll();

        CListLabelElementUI* pItem;
        _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 跟读分组 where 场景=%s order by 顺序"),
            strId);
        if (pRecdPtr)
        {
            while(!pRecdPtr->GetadoEOF())
            {
                pItem = new CListLabelElementUI;
                pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
                strTmp = GetCollect(pRecdPtr, _T("Id"));
                pItem->SetUserData(strTmp);
                pCombo->Add(pItem);

                pRecdPtr->MoveNext();
            }
            pCombo->SelectItem(0);
        }
        else
        {
            //奇怪的事情，不能在此弹出提示，否则OldDefWindProc为非法值
            //MyAfxMessageBox(_T("没有可供学习的句子。"));
            //MyAfxMessageBox(_T("%s"), _T("没有可供学习的句子。"));
            //::MessageBox(GetHWND(), _T("test"), _T("tt"), MB_OK);
        }
        pCombo->SetVisible(true);
    }
    else
    {
        pCombo->SetVisible(false);
    }
}

void MainFrame::DialogSelectTalkCmbChanged()
{
    CString strTmp;
    CComboUI* pCombo = xFindControl(CComboUI, _T("dlgtalkcmb1"));
    _RecordsetPtr pRecdPtr = GetTablesBySql(_T("Select * from 人机对话分组 where 场景=%s order by 名称"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());

    pCombo = xFindControl(CComboUI, _T("dlgtalkcmb2"));
    pCombo->RemoveAll();

    CListLabelElementUI* pItem;
    if (pRecdPtr)
    {
        while(!pRecdPtr->GetadoEOF())
        {
            pItem = new CListLabelElementUI;
            pItem->SetText(GetCollect(pRecdPtr, _T("名称")));
            strTmp = GetCollect(pRecdPtr, _T("Id"));
            pItem->SetUserData(strTmp);
            pCombo->Add(pItem);

            pRecdPtr->MoveNext();
        }
        pCombo->SelectItem(0);
    }
    else
    {
        //同上
        //MyAfxMessageBox(_T("没有可供学习的对话。"));
    }
}

void MainFrame::DialogSelectSubmit()
{
    CComboUI* pCombo;
    CComboUI* pCombo2;

    switch (m_nDialogSelectPageId)
    {
    case DIALOG_FOLLOWME:
        {
            pCombo = xFindControl(CComboUI, _T("dlgfollowmecmb"));
            if (pCombo->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可供学习的跟读场景。"));
                return;
            }

            m_strDialogScene = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
            SaveSetup(_T("Spoken_Scene"), m_strDialogScene);
            pCombo2 = xFindControl(CComboUI, _T("dlgfollowmegrpcmb"));
            if (pCombo2->IsVisible() &&
                pCombo2->GetCount() > 0 &&
                pCombo2->GetCurSel() >= 0)
            {
                m_strDialogGroup = pCombo2->GetItemAt(pCombo2->GetCurSel())->GetUserData();
            }
            else
            {
                m_strDialogGroup = _T("0");
            }
        }
    	break;
    case DIALOG_TALK:
        {
            pCombo = xFindControl(CComboUI, _T("dlgtalkcmb1"));
            pCombo2 = xFindControl(CComboUI, _T("dlgtalkcmb2"));
            if (pCombo->GetCount() <= 0 ||
                pCombo2->GetCount() <= 0)
            {
                MyAfxMessageBox(_T("%s"), _T("没有可供学习的跟读场景。"));
                return;
            }
            m_strDialogScene = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData();
            SaveSetup(_T("Talk_Scene"), m_strDialogScene);
            pCombo2 = xFindControl(CComboUI, _T("dlgtalkcmb2"));
            m_strDialogGroup = pCombo2->GetItemAt(pCombo2->GetCurSel())->GetUserData();
        }
        break;
    case DIALOG_SCENETALK:
        {
#ifdef APP_DEMO
            //  [2015-12-10 Arthur]
            MyAfxMessageBox(_T("%s"), DEMO_TIP);
            return;
            // end
#endif
            pCombo = xFindControl(CComboUI, _T("dlgscenetalkcmb"));
            m_strDialogScene = pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData();
            SaveSetup(_T("SceneTalk_Scene"), m_strDialogScene);
        }
        break;
    case DIALOG_WRITTING:
#ifdef APP_DEMO
        MyAfxMessageBox(_T("%s"), DEMO_TIP);
        return;
#endif
        break;
    default:
        ASSERT(0);
        return;
    }

    OpenChildModalWnd(sDialogSkin);
}

//////////////////////////////////////////////////////////////////////////
//对话函数
void MainFrame::OnDialogPrepare()
{
    static LPCTSTR dlg_title[] =
    {
        _T("跟我读"),
        _T("人机对话"),
        _T("情景对话"),
        _T("口头作文")
    };
    ::SetWindowText(GetHWND(), dlg_title[m_nDialogSelectPageId-1]);
    m_tmDialogLogFrom = CTime::GetCurrentTime();
    m_bDialogplaying = false;
    m_nDialogWrittingQuestion = 0;
    m_bDialogWrittingReading = false;
    m_bDialogWrittingPlaying = false;
    m_nDialogWrittingDefaultSecond = 90;
    m_nDialogValue1 = 0;
    m_nDialogValue2 = 0;
    m_strDialogUnknowSymbol.Empty();
    m_strDialogUnknowWord.Empty();
    InitDialogStatus();
}

void MainFrame::InitDialogStatus()
{
    ASSERT(m_nDialogSelectPageId >= DIALOG_FOLLOWME && m_nDialogSelectPageId <= DIALOG_WRITTING);

    CString strTmp;
    CVerticalLayoutUI* pLayout;
    CControlUI* pCtrl;
    CLabelUI* pLbl;
    COptionUI* pOpt = xFindControl(COptionUI, g_dialog_opt[m_nDialogSelectPageId-1]);

    pOpt->Selected(true);

    CTabLayoutUI* pTab = xFindControl(CTabLayoutUI,  _T("dialogtab"));
    pTab->SelectItem(m_nDialogSelectPageId-1);

    pLayout = xFindControl(CVerticalLayoutUI, _T("dialogMain"));
    pLayout->SetBkImage(m_nDialogSelectPageId == DIALOG_WRITTING ? _T("Image/MainBackground.png") : _T("Image/spokenbg.png"));

    static LPCTSTR s_ctrlScenes[] =
    {
        _T("dialogpicScene"),
        _T("dialogtalkpicScene"),
        _T("dialogscenetalkpicScene"),
    };
    static LPCTSTR s_ctrlPhotos[] =
    {
        __T("dialogpicSpeakerPhoto"),
        _T("dialogtalkpicSpeakerPhoto"),
        _T("dialogscenetalkpicSpeakerPhoto")
    };
    static LPCTSTR s_ctrlBackGround[] =
    {
        __T("dialogfollowBackGround"),
        _T("dialogtalkquestionlayout"),
        _T("dialogscenetalkquestionlayout")
    };
    static LPCTSTR s_ctrlBackGround2[] =
    {
        NULL,
        _T("dialogtalkanswerlayout"),
        _T("dialogscenetalkanswerlayout")
    };
    static LPCTSTR s_strErrMsgScene[] =
    {
        __T("没有可以跟读的场景。"),
        _T("没有可以对话的场景。"),
        _T("没有可以情景对话的场景。")
    };
    static LPCTSTR s_strErrMsgGrp[] =
    {
        __T("没有可以跟读的场景内容。"),
        _T("没有可以对话的语句。"),
        _T("没有可以情景对话的场景。")
    };
    static LPCTSTR s_strDlgBtnStatistic[] =
    {
        _T("dialogfollowstatisbtn"),
        _T("dialogtalkstatisbtn"),
        _T("dialogscenetalkstatisbtn")
    };

    int offset = m_nDialogSelectPageId - 1;

    SetVoiceRateBySetting(_T("Writing_Speaker"), _T("Writing_Speed"));
    strTmp = ReadSetup(_T("Writing_RecordLast"));
    if (IsNumeric(strTmp))
    {
        m_nDialogWrittingDefaultSecond = _tstoi(strTmp);
    }
    StopSpeak();
    StopWaveRecordReco();
    //SpeechRecognition_Stop();

    switch (m_nDialogSelectPageId)
    {
    case DIALOG_FOLLOWME:
    case DIALOG_TALK:
    case DIALOG_SCENETALK:
        {
#ifdef APP_DEMO
            CButtonUI* pBtn = xFindControl(CButtonUI, s_strDlgBtnStatistic[offset]);
            if (pBtn)
            {
                pBtn->SetVisible(false);
            }
#endif
            SetVoiceRateBySetting(s_strDlgSpeakers[offset], s_strDlgSpeeds[offset]);
            CString strScene = ReadSetup(s_strDlgScenes[offset]);
            CString strLevel = ReadSetup(s_strDlgSpeeds[offset]);

            if (!IsNumeric(strScene))
            {
                strScene = GetSingleField(_T("Select top 1 Id from %s"), s_strDlgSqlScenes[offset]);
                if (!IsNumeric(strScene))
                {
                    MyAfxMessageBox(_T("%s"), s_strErrMsgScene[offset]);
                    ListenNoQuestion();
                    return;
                }
            }

            CDbFile::CloseRecord(m_pDialogSpokenPtr);
            if (DIALOG_FOLLOWME == m_nDialogSelectPageId)
            {
                if (m_strDialogGroup == _T("0"))
                {
                    strTmp.Format(_T("Select * from %s where 场景=%s Order by 顺序"),
                        s_strDlgSqlCotents[offset], strScene);
                }
                else
                {
                    strTmp.Format(_T("Select * from %s where 分组=%s and (内容 IS NOT NULL) and (内容 <> '') Order by 顺序"),
                        s_strDlgSqlCotents[offset], m_strDialogGroup);
                }
            }
            else if (DIALOG_TALK == m_nDialogSelectPageId)
            {
                strTmp.Format(_T("Select * from %s where 分组=%s and (内容 IS NOT NULL) and (内容 <> '')"),
                    s_strDlgSqlCotents[offset], m_strDialogGroup);
            }
            else
            {
                strTmp.Format(_T("Select * from %s where 场景=%s Order by Id"),
                    s_strDlgSqlCotents[offset], strScene);
            }
            m_pDialogSpokenPtr = GetTablesBySql(_T("%s"), strTmp);
            if (!m_pDialogSpokenPtr)
            {
                MyAfxMessageBox(_T("%s"), s_strErrMsgGrp[offset]);
                ListenNoQuestion();
                return;
            }
            m_nDialogAllCount = GetCollectCount(m_pDialogSpokenPtr);

            m_nDialogSpokenIndex = 0;

            LPCTSTR lpCtrlName;

            lpCtrlName = s_ctrlBackGround[offset];
            if (lpCtrlName)
            {
                pLayout = xFindControl(CVerticalLayoutUI, lpCtrlName);
                pLayout->SetVisible(false);
            }
            lpCtrlName = s_ctrlBackGround2[offset];
            if (lpCtrlName)
            {
                pLayout = xFindControl(CVerticalLayoutUI, lpCtrlName);
                pLayout->SetVisible(false);
            }
            
            CString strPath;
            RECT rct, rctUp;
            lpCtrlName = s_ctrlScenes[offset];
            if (lpCtrlName)
            {
                strTmp = GetSingleField(_T("Select 图片 from %s where Id=%s"),
                    s_strDlgSqlScenes[offset], strScene);
                // 都用同一张图片作背景 [2016-1-25 Arthur]
                //strTmp = _T("201512610503732064392.jpg");
                strPath.Format(_T("%sData\\Picture\\%s"), CPaintManagerUI::GetInstancePath().GetData(), strTmp);
                CFileFind finder;
                pCtrl = xFindControl(CControlUI, lpCtrlName);
                if (!strTmp.IsEmpty() &&
                    finder.FindFile(strPath))
                {
                    pCtrl->SetBkImage(strPath);
                }
                finder.Close();
                rctUp = pCtrl->GetPos();
                int ht = pCtrl->GetHeight();

                pCtrl = xFindControl(CControlUI, s_ctrlPhotos[offset]);
                strTmp = ReadSetup(s_strDlgSpeakers[offset]);
                if (strTmp == MAN_VOICE)
                {
                    strPath.Format(_T("%sSystem\\Picture\\%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("SpeakerManBig.png"));
                }
                else
                {
                    strPath.Format(_T("%sSystem\\Picture\\%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("SpeakerWomanBig.png"));
                }
                SetBkImageAutoSize(pCtrl, strPath);
                //设置宽、高属性无效
                //pCtrl->SetAttribute(_T("pos"), strTmp);
#if SKIN==SKN_SMART
                rct.left = 15;
#else
                rct.left = 55;
#endif
                rct.right = rct.left + pCtrl->GetWidth();
                rct.top = rctUp.top + ht - pCtrl->GetHeight();
                rct.bottom = rct.top + pCtrl->GetHeight();
                //TRACE(_T("left:%d,top:%d,right:%d,bottom:%d\n"), rct.left, rct.top, rct.right, rct.bottom);
                pCtrl->SetPos(rct);
                pCtrl->SetVisible(false);
            }
            else
            {
                pCtrl = xFindControl(CHorizontalLayoutUI, _T("dialogscenetalkhlayout"));
                rctUp = pCtrl->GetPos();
                int ht = pCtrl->GetHeight();

                pCtrl = xFindControl(CControlUI, s_ctrlPhotos[offset]);
                strTmp = ReadSetup(s_strDlgSpeakers[offset]);
                if (strTmp == MAN_VOICE)
                {
                    strPath.Format(_T("%sSystem\\Picture\\%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("SpeakerManSmall.png"));
                }
                else
                {
                    strPath.Format(_T("%sSystem\\Picture\\%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("SpeakerWomanSmall.png"));
                }
                SetBkImageAutoSize(pCtrl, strPath);
                //设置宽、高属性无效
                //pCtrl->SetAttribute(_T("pos"), strTmp);
#if SKIN==SKN_SMART
                rct.left = 25;
#else
                rct.left = 65;
#endif
                rct.right = rct.left + pCtrl->GetWidth();
                rct.top = rctUp.top + 8 + 480 - pCtrl->GetHeight();
                rct.bottom = rct.top + pCtrl->GetHeight();
                pCtrl->SetPos(rct);
                pCtrl->SetVisible(false);
            }

            //////////////////////////////////////////////////////////////////////////
            //各自单独的初始化
            if (DIALOG_FOLLOWME == m_nDialogSelectPageId)
            {
                pLbl = xFindControl(CLabelUI, _T("dialogfollowcurrenttxt"));
                strTmp.Format(_T("%d"), m_nDialogSpokenIndex + 1);
                pLbl->SetText(strTmp);

                pLbl = xFindControl(CLabelUI, _T("dialogfollowcounttxt"));
                strTmp.Format(_T("%d"), m_nDialogAllCount);
                pLbl->SetText(strTmp);
            }
            else if (DIALOG_TALK == m_nDialogSelectPageId)
            {
                CHorizontalLayoutUI* pLayoutH = xFindControl(CHorizontalLayoutUI, _T("dialogtalkscorelayout"));
                pLayoutH->SetVisible(false);

                pLbl = xFindControl(CLabelUI, _T("dialogtalkscoretxt"));
                pLbl->SetText(_T("0"));

                pOpt = xFindControl(COptionUI, _T("dialogtalkmanbopt"));
                pOpt->Selected(true);
            }
            else
            {
                pLayout = xFindControl(CVerticalLayoutUI, _T("dialogscenetalknoticevlayout"));
                pLayout->SetVisible(false);
            }
        }
    	break;
    case DIALOG_WRITTING:
        {
            ASSERT(m_nDialogWrittingType >= DLG_WRT_HUATI && m_nDialogWrittingType <= DLG_WRT_LANGDU);

            SetVoiceRateBySetting(_T("Writing_Speaker"), _T("Writing_Speed"));

            CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
            pREdt->SetText(_T(""));
            pREdt->SetAutoURLDetect(false);
            CProgressUI* pPrgrs = xFindControl(CProgressUI, _T("dialogwrittingprgrs"));
            pPrgrs->SetValue(0);
            pPrgrs = xFindControl(CProgressUI, _T("dialogwrttingreadprgrs"));
            pPrgrs->SetValue(0);
            pLbl = xFindControl(CLabelUI, _T("dialogwrittingrcdtimetxt"));
            pLbl->SetText(_T("0"));
            pLbl = xFindControl(CLabelUI, _T("dialogwrttingreadnoticetxt"));
            pLbl->SetText(_T(""));

            pOpt = xFindControl(COptionUI, g_dialog_writting_opt[m_nDialogWrittingType]);
            pOpt->Selected(true);
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}

int MainFrame::IsDialogOpt(const TNotifyUI& msg)
{
    return IsOption(g_dialog_opt, _countof(g_dialog_opt), msg);
}

void MainFrame::DialogOptClicked(int nIdx)
{
    ASSERT(nIdx >= DIALOG_FOLLOWME && nIdx <= DIALOG_WRITTING);

    if (nIdx != m_nDialogSelectPageId)
    {
        DialogChangePage(nIdx);
    }
}

void MainFrame::DialogChangePage(int nIdx)
{
    CTimeSpan tmInterval(CTime::GetCurrentTime() - m_tmDialogLogFrom);
    int v = (int)tmInterval.GetTotalMinutes();
    StudyLog(STUDY_TYPE_DIALOG, LOG_TYPE_TIME, v, 0);
    if (DIALOG_WRITTING != m_nDialogSelectPageId)
    {
        if (m_nDialogValue1 > 0)
        {
            int v2 = (m_nDialogValue2 + 50) / 100;
            StudyLog(STUDY_TYPE_DIALOG, LOG_TYPE_ACHIEV, m_nDialogValue1, v2);
        }
    }
    m_nDialogSelectPageId = nIdx;
    StopSpeak();
    StopWaveRecordReco();
    OpenChildModalWnd(sDialogSelectSkin);
}

void MainFrame::DialogDbPtrMovePre()
{
    if (m_nDialogSpokenIndex <= 0)
    {
        return;
    }
    int nOld = m_nDialogSpokenIndex - 1;
    m_nDialogSpokenIndex = 0;
    m_pDialogSpokenPtr->MoveFirst();
    while(m_nDialogSpokenIndex < nOld)
    {
        m_nDialogSpokenIndex++;
        m_pDialogSpokenPtr->MoveNext();
    }
}

void MainFrame::DialogRecordCurrentSentence()
{
    CString strTmp;
    CControlUI* pCtrl;
    COptionUI* pOpt;
    //CRichEditUI* pREdt;
    //CProgressUI* pPrgrs;
    //CLabelUI* pLbl;

    pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
    pOpt->SetEnabled(false);
    pOpt = xFindControl(COptionUI, _T("dialogtalkmanbopt"));
    pOpt->SetEnabled(false);

    pCtrl = xFindControl(CControlUI, _T("dialogtalkquestionlayout"));
    pCtrl->SetVisible(false);
    pCtrl = xFindControl(CControlUI, _T("dialogtalkanswerlayout"));
    pCtrl->SetVisible(true);

    DialogTalkSetAnswerText(40);

    ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE, 100, NULL);
    SpeechRecognition_Start();
}

void MainFrame::DialogSpeackCurrentSentence()
{
    CString strTmp;
    CLabelUI* pLbl;
    CRichEditUI* pREdt;
    CButtonUI* pBtn;
    CProgressUI* pPrgrs;
    CControlUI* pCtrl;
    COptionUI* pOpt;

    m_bDialogplaying = false;
    StopSpeak();
    StopWaveRecordReco();

    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);

    if (m_nDialogSelectPageId == DIALOG_FOLLOWME)
    {
        pCtrl = xFindControl(CControlUI, _T("dialogfollowBackGround"));
        pCtrl->SetVisible(true);

        //pBtn = xFindControl(CButtonUI, _T("dialogfollowstartbtn"));
        //pBtn->SetEnabled(false);
        //pBtn = xFindControl(CButtonUI, _T("dialogfollowleftbtn"));
        //pBtn->SetEnabled(false);
        //pBtn = xFindControl(CButtonUI, _T("dialogfollowrightbtn"));
        //pBtn->SetEnabled(false);

        pPrgrs = xFindControl(CProgressUI, _T("dialogfollowprgrs"));
        pPrgrs->SetValue(0);
        pPrgrs->SetVisible(false);
        pCtrl = xFindControl(CControlUI, _T("dialogfollowfunlayout"));
        pCtrl->SetVisible(false);
        pBtn = xFindControl(CButtonUI, _T("dialogfollowreplaybtn"));
        pBtn->SetVisible(false);
        pLbl = xFindControl(CLabelUI, _T("dialogfollownoticetxt"));
        pLbl->SetVisible(false);

        pLbl = xFindControl(CLabelUI, _T("dialogfollowcurrenttxt"));
        strTmp.Format(_T("%d"), m_nDialogSpokenIndex + 1);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("dialogfollowcounttxt"));
        strTmp.Format(_T("%d"), m_nDialogAllCount);
        pLbl->SetText(strTmp);

        pREdt = xFindControl(CRichEditUI, _T("dialogfollowcntxt"));
        pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("中文")));

        strTmp = GetCollect(m_pDialogSpokenPtr, _T("内容"));
        pREdt = xFindControl(CRichEditUI, _T("dialogfollowentxt"));
        pREdt->SetText(strTmp);

        m_bDialogCountTime = true;
    }
    else if (DIALOG_TALK == m_nDialogSelectPageId)
    {
        pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
        pOpt->SetEnabled(false);

        pOpt = xFindControl(COptionUI, _T("dialogtalkmanbopt"));
        pOpt->SetEnabled(false);

        pCtrl = xFindControl(CControlUI, _T("dialogtalkquestionlayout"));
        pCtrl->SetVisible(true);
        pCtrl = xFindControl(CControlUI, _T("dialogtalkanswerlayout"));
        pCtrl->SetVisible(false);

        pCtrl = xFindControl(CControlUI, _T("dialogtalkscorelayout"));
        pCtrl->SetVisible(false);

        pCtrl = xFindControl(CControlUI, _T("dialogtalkanswerprgrs"));
        pCtrl->SetVisible(true);

        strTmp = DialogTalkSetRequestText();
    }
    else if (DIALOG_SCENETALK == m_nDialogSelectPageId)
    {
        pCtrl = xFindControl(CControlUI, _T("dialogscenetalkquestionlayout"));
        pCtrl->SetVisible(true);
        pCtrl = xFindControl(CControlUI, _T("dialogscenetalkanswerlayout"));
        pCtrl->SetVisible(false);
        pCtrl = xFindControl(CControlUI, _T("dialogscenetalknoticevlayout"));
        pCtrl->SetVisible(false);

        pREdt = xFindControl(CRichEditUI, _T("dialogscenetalkquestionentxt"));
        strTmp = GetCollect(m_pDialogSpokenPtr, _T("问题"));
        pREdt->SetText(strTmp);
        pREdt = xFindControl(CRichEditUI, _T("dialogscenetalkquestioncntxt"));
        pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("问题中文")));
    }
    else
    {
        return;
    }

    m_tmDialogSpeakFrom = CTime::GetCurrentTime();

    m_bDialogplaying = true;
    Speak(strTmp);
}

void MainFrame::DialogFollowLeftClicked()
{
    if (m_nDialogSpokenIndex > 0)
    {
        //m_nDialogSpokenIndex--;
        //m_pDialogSpokenPtr->MovePrevious();
        DialogDbPtrMovePre();
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("dialogfollowcurrenttxt"));
        CString strTmp;
        strTmp.Format(_T("%d"), m_nDialogSpokenIndex + 1);
        pLbl->SetText(strTmp);
        if (m_bDialogplaying)
        {
            DialogSpeackCurrentSentence();
        }
    }
}

void MainFrame::DialogFollowStartClicked()
{
    //if (m_bDialogplaying)
    //{
    //    return;
    //}
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogfollowstartbtn"));
    CControlUI* pCtrl = xFindControl(CControlUI, _T("dialogpicSpeakerPhoto"));
    
    //will be set in DialogSpeackCurrentSentence()
    //m_bDialogplaying = true;
    pBtn->SetEnabled(false);
    pCtrl->SetVisible(true);

    pCtrl = xFindControl(CControlUI, _T("dialogpicScene"));
    pCtrl->SetBkImage(_T("Image/spokenbg2.png"));

    DialogSpeackCurrentSentence();
}

void MainFrame::DialogFollowStopClicked()
{
    m_bDialogplaying = false;
    StopSpeak();
    StopWaveRecordReco();
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    CControlUI* pCtrl;

    pCtrl = xFindControl(CControlUI, _T("dialogfollowstartbtn"));
    pCtrl->SetEnabled(true);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowleftbtn"));
    pCtrl->SetEnabled(true);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowrightbtn"));
    pCtrl->SetEnabled(true);


    pCtrl = xFindControl(CControlUI, _T("dialogfollowentxt"));
    pCtrl->SetVisible(true);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowcntxt"));
    pCtrl->SetVisible(true);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowfunlayout"));
    pCtrl->SetVisible(false);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowprgrs"));
    pCtrl->SetVisible(false);
    pCtrl = xFindControl(CControlUI, _T("dialogfollownoticetxt"));
    pCtrl->SetVisible(false);
    pCtrl = xFindControl(CControlUI, _T("dialogfollowreplaybtn"));
    pCtrl->SetVisible(false);
}

void MainFrame::DialogFollowRightClicked()
{
    if (m_nDialogSpokenIndex < m_nDialogAllCount - 1)
    {
        m_pDialogSpokenPtr->MoveNext();
        m_nDialogSpokenIndex++;
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("dialogfollowcurrenttxt"));
        CString strTmp;
        strTmp.Format(_T("%d"), m_nDialogSpokenIndex + 1);
        pLbl->SetText(strTmp);
        //m_bDialogplaying = true;
        if (m_bDialogplaying)
        {
            DialogSpeackCurrentSentence();
        }
        return;
    }

    m_PaintManager.SendNotify(xFindControl(CControlUI, _T("dialogfollowstopbtn")), DUI_MSGTYPE_CLICK);
    MyAfxMessageBox(_T("%s"), _T("本组练习结束。"));    
}

void MainFrame::DialogFollowStatisClicked()
{
    //if (m_bDialogplaying)
    //{
    //    TNotifyUI msg;
    //    msg.pSender = xFindControl(CButtonUI, _T("dialogfollowstopbtn"));
    //    msg.sType = DUI_MSGTYPE_CLICK;
    //    //Notify(msg);
    //    m_PaintManager.SendNotify(msg);
    //}
    StopSpeak();
    StopWaveRecordReco();
    ShowSymbolStatistic(m_strDialogUnknowWord, m_strDialogUnknowSymbol);
}

void MainFrame::DialogFollowListClicked()
{
    StopSpeak();
    StopWaveRecordReco();
    CDialogList* pDlgLst = new CDialogList(this);
    pDlgLst->m_nCurrentIdx = m_nDialogSpokenIndex;
    if (IDOK == OpenModalWnd(pDlgLst, g_SkinInfo[sDialogListSkin].title))
    {
        CLabelUI* pLbl;
        CString strTmp;
        
        pLbl = xFindControl(CLabelUI, _T("dialogfollowcurrenttxt"));
        strTmp.Format(_T("%d"), m_nDialogSpokenIndex + 1);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("dialogfollowcounttxt"));
        strTmp.Format(_T("%d"), m_nDialogAllCount);
        pLbl->SetText(strTmp);

        CProgressUI* pPrgrs;
        pPrgrs = xFindControl(CProgressUI, _T("dialogfollowprgrs"));
        pPrgrs->SetValue(0);
        pPrgrs->SetVisible(false);
        CControlUI* pCtrl;
        pCtrl = xFindControl(CControlUI, _T("dialogfollowfunlayout"));
        pCtrl->SetVisible(false);

        CButtonUI* pBtn;
        pBtn = xFindControl(CButtonUI, _T("dialogfollowreplaybtn"));
        pBtn->SetVisible(false);
        pLbl = xFindControl(CLabelUI, _T("dialogfollownoticetxt"));
        pLbl->SetVisible(false);

        CRichEditUI* pREdt;
        pREdt = xFindControl(CRichEditUI, _T("dialogfollowcntxt"));
        pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("中文")));

        strTmp = GetCollect(m_pDialogSpokenPtr, _T("内容"));
        pREdt = xFindControl(CRichEditUI, _T("dialogfollowentxt"));
        pREdt->SetText(strTmp);
    }
}

void MainFrame::DialogFollowReplayClicked()
{
    m_mciWave.Stop();
    m_mciWave.PlayWait(GetHWND(), m_strCommonWaveFile);
}

void MainFrame::DialogTalkEndOf()
{
    COptionUI* pOpt;
    CButtonUI* pBtn;

    m_bDialogplaying = false;
    StopSpeak();
    StopWaveRecordReco();
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);

    pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
    pOpt->SetEnabled(true);
    pOpt = xFindControl(COptionUI, _T("dialogtalkmanbopt"));
    pOpt->SetEnabled(true);

    pBtn = xFindControl(CButtonUI, _T("dialogtalkstartbtn"));
    pBtn->SetEnabled(true);

    MyAfxMessageBox(_T("%s"), _T("本组对话练习结束。"));
}

CString MainFrame::DialogTalkGetContent()
{
    CString strTmp = GetCollect(m_pDialogSpokenPtr, _T("内容"));
    strTmp.TrimLeft(_T("\\"));
    strTmp.Replace(_T("："), _T(":"));
    SplitResultVector sptTmp;
    SplitString(TString(strTmp), _T(":"), sptTmp);
    if (sptTmp.size() > 1)
    {
        strTmp.Replace(CString(sptTmp[0].c_str()) + _T(":"), _T(""));
    }

    return strTmp;
}

CString MainFrame::DialogTalkSetText_(LPCTSTR pstrCtrlEN, LPCTSTR pstrCtrlCN)
{
    CString strTmp;
    CRichEditUI* pREdt;

    strTmp = DialogTalkGetContent();

    pREdt = xFindControl(CRichEditUI, pstrCtrlEN);
    pREdt->SetText(strTmp);

    pREdt = xFindControl(CRichEditUI, pstrCtrlCN);
    pREdt->SetText(GetCollect(m_pDialogSpokenPtr, _T("中文")));

    return strTmp;
}

CString MainFrame::DialogTalkSetRequestText()
{
    return DialogTalkSetText_(_T("dialogtalkquestionentxt"), _T("dialogtalkquestioncntxt"));
}

CString MainFrame::DialogTalkSetAnswerText(int nMaxValue)
{
    CControlUI* pCtrl;
    CProgressUI* pPrgrs;
    CLabelUI* pLbl;

    pPrgrs = xFindControl(CProgressUI, _T("dialogtalkanswerprgrs"));
    pPrgrs->SetMaxValue(nMaxValue);
    pPrgrs->SetValue(0);

    pLbl = xFindControl(CLabelUI, _T("dialogtalkanswernoticetxt"));
    //pLbl->SetVisible(false);
    pLbl->SetText(_T(""));

    pCtrl = xFindControl(CControlUI, _T("dialogtalkscorelayout"));
    pCtrl->SetVisible(false);

    return DialogTalkSetText_(_T("dialogtalkanswerentxt"), _T("dialogtalkanswercntxt"));
}

void MainFrame::DialogTalkSpeakClicked()
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogtalkanswerentxt"));
    CString strTmp = pREdt->GetText();

    strTmp.Trim();
    if (strTmp.IsEmpty())
    {
        return;
    }

    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    m_tmDialogSpeakFrom = CTime::GetCurrentTime();
    CProgressUI* pPrgrs = xFindControl(CProgressUI, _T("dialogtalkanswerprgrs"));
    pPrgrs->SetValue(0);
    DialogDbPtrMovePre();
    StopWaveRecordReco();
    StopSpeak();
    Speak(strTmp);
}

void MainFrame::DialogTalkStartClicked()
{
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogtalkstartbtn"));
    CControlUI* pCtrl = xFindControl(CControlUI, _T("dialogtalkpicSpeakerPhoto"));

    //m_bDialogplaying = true;
    pBtn->SetEnabled(false);
    pCtrl->SetVisible(true);

    pCtrl = xFindControl(CControlUI, _T("dialogtalkpicScene"));
    pCtrl->SetBkImage(_T("Image/spokenbg2.png"));

    m_nDialogSpokenIndex = 0;
    m_pDialogSpokenPtr->MoveFirst();

    COptionUI* pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
    if (pOpt->IsSelected())
    {
        DialogRecordCurrentSentence();
    }
    else
    {
        DialogSpeackCurrentSentence();
    }
}

void MainFrame::DialogTalkStopClicked()
{
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    m_bDialogplaying = false;
    StopSpeak();
    StopWaveRecordReco();
    //SpeechRecognition_Stop();

    COptionUI* pOpt;
    pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
    pOpt->SetEnabled(true);
    pOpt = xFindControl(COptionUI, _T("dialogtalkmanbopt"));
    pOpt->SetEnabled(true);

    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogtalkstartbtn"));
    pBtn->SetEnabled(true);

    m_nDialogSpokenIndex = 0;
    m_pDialogSpokenPtr->MoveFirst();
}

void MainFrame::DialogTalkSkipClicked()
{
    if (m_nDialogSpokenIndex >= m_nDialogAllCount - 1 ||
        m_pDialogSpokenPtr->GetadoEOF())
    {
        DialogTalkEndOf();
        return;
    }

    m_nDialogSpokenIndex++;
    m_pDialogSpokenPtr->MoveNext();
    if (!m_bDialogplaying)
    {
        return;
    }
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    StopSpeak();
    StopWaveRecordReco();
    COptionUI* pOpt = xFindControl(COptionUI, _T("dialogtalkmanaopt"));
    if (pOpt->IsSelected())
    {
        if (m_nDialogSpokenIndex % 2 == 0)
        {
            DialogRecordCurrentSentence();
        }
        else
        {
            DialogSpeackCurrentSentence();
        }
    }
    else if (m_nDialogSpokenIndex % 2 == 0)
    {
        DialogSpeackCurrentSentence();
    }
    else
    {
        DialogRecordCurrentSentence();
    }
}

void MainFrame::DialogTalkListClicked()
{
    //StopSpeak();
    DialogChangePage(m_nDialogSelectPageId);
}

void MainFrame::DialogTalkStaticClicked()
{
    ShowSymbolStatistic(m_strDialogUnknowWord, m_strDialogUnknowSymbol);
}

int MainFrame::DialogSceneTalkTextSeconds(LPCTSTR pstrSqlText)
{
    CString strTmp = GetCollect(m_pDialogSpokenPtr, pstrSqlText);
    return GetSentenceTime(strTmp);
}

void MainFrame::DialogSceneTalkStartShow(bool bShow)
{
    CControlUI* pCtrl;

    pCtrl = xFindControl(CControlUI, _T("dialogscenetalkquestionlayout"));
    pCtrl->SetVisible(bShow);
    pCtrl = xFindControl(CControlUI, _T("dialogscenetalkanswerlayout"));
    pCtrl->SetVisible(bShow);
    pCtrl = xFindControl(CControlUI, _T("dialogscenetalknoticevlayout"));
    pCtrl->SetVisible(bShow);
}

void MainFrame::DialogSceneTalkEndOf()
{
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    DialogSceneTalkStartShow(false);
    MyAfxMessageBox(_T("%s"), _T("本组对话练习结束。"));
}

void MainFrame::DialogSceneTalkStartClicked()
{
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogscenetalkstartbtn"));
    CControlUI* pCtrl = xFindControl(CControlUI, _T("dialogscenetalkpicSpeakerPhoto"));

    //m_bDialogplaying = true;
    pBtn->SetEnabled(false);
    pCtrl->SetVisible(true);

    pCtrl = xFindControl(CControlUI, _T("dialogtalkpicScene"));
    pCtrl->SetBkImage(_T("Image/spokenbg2.png"));

    //m_nDialogSpokenIndex = 0;
    //m_pDialogSpokenPtr->MoveFirst();

    DialogSceneTalkStartShow(true);

    DialogSpeackCurrentSentence();
}

void MainFrame::DialogSceneTalkStopClicked()
{
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogscenetalkstartbtn"));

    m_bDialogplaying = false;
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    StopSpeak();
    StopWaveRecordReco();

    pBtn->SetEnabled(true);

    DialogSceneTalkStartShow(false);
}

void MainFrame::DialogSceneTalkSkipClicked()
{
    if (m_nDialogSpokenIndex < m_nDialogAllCount - 1)
    {
        m_nDialogSpokenIndex++;
        m_pDialogSpokenPtr->MoveNext();
        if (m_bDialogplaying)
        {
            DialogSpeackCurrentSentence();
        }
        return;
    }
    DialogSceneTalkEndOf();
}

void MainFrame::DialogSceneTalkSatisClicked()
{
    ShowSymbolStatistic(m_strDialogUnknowWord, m_strDialogUnknowSymbol);
}

int MainFrame::IsDialogWrittingOpt(const TNotifyUI& msg)
{
    return IsOption(g_dialog_writting_opt, _countof(g_dialog_writting_opt), msg);
}

void MainFrame::DialogWrittingOptClicked(int nIdx)
{
    CRichEditUI* pREdt;
    CLabelUI* pLbl;
    CString strTmp;
    CControlUI* pCtrl;
    CProgressUI* pPrgrs;
    int i;

    ASSERT(nIdx >= DLG_WRT_HUATI && nIdx <= DLG_WRT_LANGDU);
    
    m_nDialogWrittingType = nIdx;

    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
    ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT);

    if (m_bDialogWrittingReading)
    {
        m_PaintManager.SendNotify(xFindControl(CControlUI, _T("dialogwrttingreadstartbtn")), DUI_MSGTYPE_CLICK);
    }

    if (m_bDialogWrittingPlaying)
    {
        m_PaintManager.SendNotify(xFindControl(CControlUI, _T("dialogwrittingstartbtn")), DUI_MSGTYPE_CLICK);
    }

    pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
    pREdt->SetText(_T(""));

    pPrgrs = xFindControl(CProgressUI, _T("dialogwrittingprgrs"));
    pLbl = xFindControl(CLabelUI, _T("dialogwrittingrcdtimetxt"));
    pPrgrs->SetValue(0);
    pLbl->SetText(_T("0"));

    static LPCTSTR ctrl_name[] =
    {
        _T("dialogwrttingpiclayout"),
        _T("dialogwrttingSpokenResultlayout"),
        _T("dialogwrttingreadlayout"),
        _T("dialogwrttingtypelayout")
    };
    bool bVisible[4];

    switch (nIdx)
    {
    case DLG_WRT_HUATI:     //话题描述
        bVisible[0] = false;
        bVisible[1] = false;
        bVisible[2] = false;
        bVisible[3] = true;
        break;
    case DLG_WRT_KANTU:     //看图说话
        bVisible[0] = true;
        bVisible[1] = false;
        bVisible[2] = false;
        bVisible[3] = true;
        break;
    case DLG_WRT_LANGDU:     //朗读短文
        bVisible[0] = false;
        bVisible[1] = false;
        bVisible[2] = true;
        bVisible[3] = false;
        break;
    default:
        return;
    }

    for (i = 0; i < _countof(ctrl_name); i++)
    {
        pCtrl = xFindControl(CControlUI, ctrl_name[i]);
        pCtrl->SetVisible(bVisible[i]);
    }

    switch (nIdx)
    {
    case DLG_WRT_HUATI:
    case DLG_WRT_KANTU:
        {
            _RecordsetPtr pRecdPtr;
            if (m_nDialogWrittingQuestion != 0)
            {
                pRecdPtr = GetTablesBySql(_T("Select top 1 * from 口头作文 where Id=%d"),
                    m_nDialogWrittingQuestion);
            }
            else
            {
                pRecdPtr = GetTablesBySql(_T("Select top 1 * from 口头作文 where 题型=%d Order by Id"),
                    nIdx);
                if (pRecdPtr)
                {
                    m_nDialogWrittingQuestion = _tstoi(GetCollect(pRecdPtr, _T("Id")));
                }
                else
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可以练习的题目。"));
                    ListenNoQuestion();
                    return;
                }
            }
            
            strTmp = GetCollect(pRecdPtr, _T("答案")).Trim();
            strTmp.Replace(_T("\r\n"), _T("|"));
            strTmp.Trim(_T("|"));
            while(strTmp.Find(_T("||")) >= 0)
            {
                strTmp.Replace(_T("||"), _T("|"));
            }
            SplitString(TString(strTmp), _T("|"), m_sptDialogWrittingAnswer);

            strTmp = GetCollect(pRecdPtr, _T("题目"));
            strTmp.Replace(_T("\r\n"), _T("|"));
            strTmp.Trim(_T("|"));
            while(strTmp.Find(_T("||")) >= 0)
            {
                strTmp.Replace(_T("||"), _T("|"));
            }
            SplitString(TString(strTmp), _T("|"), m_sptDialogWrittingSentence);
            m_nDialogSpokenIndex = 0;

            pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
            pREdt->SetText(GetCollect(pRecdPtr, _T("题目")));
            pREdt->SetTextColor(DUI_BLACK);
            pREdt->SetSel(0, 0);
            pREdt->SetSelNone();

            pLbl = xFindControl(CLabelUI, _T("dialogwrittingquesnoticetxt"));
            pLbl->SetText((nIdx == DLG_WRT_HUATI) ? _T("请根据中文逐句说出英文，电脑会自动评分。") : _T("请看图并根据中文逐句说出英文，电脑会自动评分。"));

            if (nIdx == DLG_WRT_KANTU)
            {
                strTmp.Format(_T("%sData\\Picture\\%s"),
                    CPaintManagerUI::GetInstancePath().GetData(),
                    GetCollect(pRecdPtr, _T("图片")));
                CFileFind finder;
                if (finder.FindFile(strTmp))
                {
                    CControlUI* pParent = xFindControl(CControlUI, _T("dialogwrttingpiclayout"));
                    pCtrl = xFindControl(CControlUI, _T("dialogwrttingpic"));
                    SetBkImageAutoSize(pCtrl, strTmp);
                    RECT rct = pCtrl->GetPos();
                    int wd = pCtrl->GetWidth(), ht = pCtrl->GetHeight();
                    RECT rctParent = pParent->GetPos();
                    int wdp = pParent->GetWidth(), htp = pParent->GetHeight();
                    rct.top = rctParent.top + (htp - ht) / 2;
                    rct.bottom = rct.top + ht;
                    rct.left = rctParent.left + (wdp - wd) / 2;
                    rct.right = rct.left + wd;
                    pCtrl->SetPos(rct);
                }
                finder.Close();
            }
        }
    	break;
    case DLG_WRT_LANGDU:
        {
            pLbl = xFindControl(CLabelUI, _T("dialogwrittingquesnoticetxt"));
            pLbl->SetText(_T("请跟着电脑逐句朗读下面的短文，电脑会自动评分。"));

            _RecordsetPtr pRecdPtr;
            if (m_nDialogWrittingQuestion == 0)
            {
                pRecdPtr = GetTablesBySql(_T("%s"), _T("Select top 1 * from 朗读短文 Order by Id"));
                if(!pRecdPtr)
                {
                    MyAfxMessageBox(_T("%s"), _T("没有可以练习的题目。"));
                    ListenNoQuestion();
                    return;
                }
                m_nDialogWrittingQuestion = _tstoi(GetCollect(pRecdPtr, _T("Id")));
            }
            else
            {
                pRecdPtr = GetTablesBySql(_T("Select top 1 * from 朗读短文 where Id=%d"),
                    m_nDialogWrittingQuestion);
            }

            pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
            pREdt->SetText(GetCollect(pRecdPtr, _T("内容")));
            pREdt->SetTextColor(DUI_BLACK);
            pREdt->SetSel(0, 0);
            pREdt->SetSelNone();
        }
        break;
    default:
        return;
    }
}

void MainFrame::DialogWrittingNext()
{
    CString strTmp;

    if (m_nDialogWrittingType == DLG_WRT_LANGDU)
    {
        strTmp.Format(_T("Select top 1 Id from 朗读短文 where Id>%d Order by Id"),
            m_nDialogWrittingQuestion);
        if (m_bDialogWrittingReading)
        {
            m_PaintManager.SendNotify(xFindControl(CControlUI, _T("dialogwrttingreadstartbtn")), DUI_MSGTYPE_CLICK);
        }
    }
    else
    {
        if (m_bDialogWrittingPlaying)
        {
            m_PaintManager.SendNotify(xFindControl(CControlUI, _T("dialogwrittingstartbtn")), DUI_MSGTYPE_CLICK);
        }

        strTmp.Format(_T("Select top 1 Id from 口头作文 where 题型=%d and Id>%d Order by Id"),
            m_nDialogWrittingType, m_nDialogWrittingQuestion);
    }

    CString strQues = GetSingleField(_T("%s"), strTmp);
    //TRACE(_T("value:%s\n"), strQues);
    if (strQues.IsEmpty())
    {
        MyAfxMessageBox(_T("%s"), _T("没有可以显示的下一题。"));
    }
    else
    {
        m_nDialogWrittingQuestion = _tstoi(strQues);
        //InitDialogStatus();
        DialogWrittingOptClicked(m_nDialogWrittingType);
    }
}

void MainFrame::DialogWrittingStart()
{
    CString strTmp;

    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogwrittingstartbtn"));
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
    CLabelUI* pLbl = xFindControl(CLabelUI, _T("dialogwrittingnoticetxt"));
    //CProgressUI* pPrgrs = xFindControl(CProgressUI, _T("dialogwrittingprgrs"));
//    CLabelUI* pLblTime = xFindControl(CLabelUI, _T("dialogwrittingrcdtimetxt"));

    if (m_bDialogWrittingPlaying)
    {
        m_bDialogWrittingPlaying = false;
        StopWaveRecordReco();
        StopSpeak();
        ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
        ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT);
        pBtn->SetText(_T("开始"));
        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSel(0, 0);
        pREdt->SetSelNone();
        pLbl->SetText(_T("录音停止"));
    }
    else
    {
        m_bDialogWrittingPlaying = true;
        pBtn->SetText(_T("停止"));

        pLbl->SetText(_T("开始录音"));

        StopSpeak();
        //SpeechRecognition_Stop();
        ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
        if (m_nDialogSpokenIndex == (int)m_sptDialogWrittingSentence.size())
        {
            m_nDialogSpokenIndex = 0;
        }
        DialogWrittingStartNext_();
    }
}

void MainFrame::DialogWrittingStartNext_()
{
    if (m_nDialogSpokenIndex == (int)m_sptDialogWrittingSentence.size())
    {
        m_PaintManager.SendNotify(xFindControl(COptionUI, _T("dialogwrittingstartbtn")), DUI_MSGTYPE_CLICK);
    }
    else
    {
        CString strTmp;
        int i;

        CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
        CProgressUI* pPrgrs = xFindControl(CProgressUI, _T("dialogwrittingprgrs"));
        CLabelUI* pLbl = xFindControl(CLabelUI, _T("dialogwrittingrcdtimetxt"));

        strTmp = pREdt->GetText();
        i = strTmp.Find(m_sptDialogWrittingSentence[m_nDialogSpokenIndex].c_str());
        pREdt->SetSel(i, i + m_sptDialogWrittingSentence[m_nDialogSpokenIndex].length());
        SetRichEditSelColor(pREdt, RGB(255,0,0));

        pPrgrs->SetValue(0);
        pPrgrs->SetMaxValue(m_nDialogWrittingDefaultSecond);
        strTmp.Format(_T("%d"), 0);
        pLbl->SetText(strTmp);

        pLbl = xFindControl(CLabelUI, _T("dialogwrittingnoticetxt"));
        pLbl->SetText(_T("开始录音"));

        m_tmDialogSpeakFrom = CTime::GetCurrentTime();
        SpeechRecognition_Start();
        ::SetTimer(GetHWND(), TIMER_ID_DLG_WRITE, 1000, NULL);
    }
}

void MainFrame::DialogWrittingSource()
{
    if (m_nDialogWrittingQuestion == 0)
    {
        MyAfxMessageBox(_T("%s"), _T("没有可以显示的范文。"));
        return;
    }
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSpokenResulttxt"));
    CControlUI* pCtrl = xFindControl(CControlUI, _T("dialogwrttingSpokenResultlayout"));

    pREdt->SetText(GetSingleField(_T("Select 答案 from 口头作文 where Id=%d"),
        m_nDialogWrittingQuestion));
    pCtrl->SetVisible(true);
}

void MainFrame::DialogWrittingStartRead()
{
    CButtonUI* pBtn = xFindControl(CButtonUI, _T("dialogwrttingreadstartbtn"));
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
    CString strTmp;
    
    if (m_bDialogWrittingReading)
    {
        m_bDialogWrittingReading = false;
        StopSpeak();
        StopWaveRecordReco();
        pBtn->SetText(_T("开始朗读"));
        ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE);
        ::KillTimer(GetHWND(), TIMER_ID_DLG_WRITE_NEXT);
        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSel(0, 0);
        pREdt->SetSelNone();
    }
    else
    {
        m_bDialogWrittingReading = true;
        pBtn->SetText(_T("停止朗读"));

        pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
        pREdt->SetText(_T(""));

        int nFrom = 0, nTo = 0;
        long lStart, lEnd;
        pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));
        pREdt->GetSel(lStart, lEnd);
        GetSentencePosition(pREdt, lStart, nFrom, nTo);

        DialogWrittingStartSpeak_(nFrom, nTo);        
    }
}

void MainFrame::DialogWrittingBeginSpeak()
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingSpokenResulttxt"));
    CString strTmp = pREdt->GetText();

    strTmp.Trim();
    if (!strTmp.IsEmpty())
    {
        Speak(strTmp);
    }
}

void MainFrame::DialogWrittingStopSpeak()
{
    StopSpeak();
}

void MainFrame::DialogWrittingSpeakNext_()
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));

    if (m_nDialogWrittingToPostion < pREdt->GetText().GetLength())
    {
        pREdt->SetTextColor(DUI_BLACK);
        pREdt->SetSel(m_nDialogWrittingToPostion + 1, m_nDialogWrittingToPostion + 1);
        int nFrom, nTo;
        GetSentencePosition(pREdt, m_nDialogWrittingToPostion  + 1, nFrom, nTo);
        DialogWrittingStartSpeak_(nFrom, nTo);
    }
    else
    {
        //stop
        TNotifyUI msg;
        msg.pSender = xFindControl(CControlUI, _T("dialogwrttingreadstartbtn"));
        msg.sType = DUI_MSGTYPE_CLICK;
        //Notify(msg);
        m_PaintManager.SendNotify(msg);
    }
}

void MainFrame::DialogWrittingStartSpeak_(int nFrom, int nTo)
{
    CRichEditUI* pREdt = xFindControl(CRichEditUI, _T("dialogwrttingrequesttxt"));

    if (nFrom != nTo)
    {
        m_nDialogWrittingToPostion = nTo;
        //m_nDialogWrittingCurrentCursor = nFrom;

        CProgressUI* pPrgs = xFindControl(CProgressUI, _T("dialogwrttingreadprgrs"));
        pPrgs->SetValue(0);

        pREdt->SetSel(nFrom, nTo);
        m_strDialogSpokenText = pREdt->GetSelText();
        SetRichEditSelColor(pREdt, RGB(255,0,0));
        pREdt->SetSelNone();

        CRichEditUI* pREdt2 = xFindControl(CRichEditUI, _T("dialogwrttingSampletxt"));
        CString strTmp = pREdt2->GetText();
        strTmp += m_strDialogSpokenText;
        pREdt2->SetText(strTmp);

        m_tmDialogSpeakFrom = CTime::GetCurrentTime();
        Speak(m_strDialogSpokenText);
    }
}

//////////////////////////////////////////////////////////////////////////
//单元测试函数
void MainFrame::InitPaperCfgGradeCombo(LPCTSTR lpstrGrade/*=NULL*/)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;

    pGradePtr = GetTablesBySql(_T("%s"), _T("Select * from 单元测试年级 Order by ID"));
    idx = 0, nSel = 0;
    pCombo = xFindControl(CComboUI, _T("papercfggradecmb"));
    pCombo->RemoveAll();
    if (pGradePtr == NULL)
    {
        return;
    }
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pGradePtr, _T("年级")));
        strTmp = GetCollect(pGradePtr, _T("ID"));
        pElement->SetUserData(strTmp);
        if (lpstrGrade && (strTmp == lpstrGrade))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);
}

void MainFrame::InitPaperCfgBookCombo(LPCTSTR lpstrBook/*=NULL*/)
{
    CComboUI* pCombo;
    CListLabelElementUI* pElement;
    int idx, nSel;
    _RecordsetPtr pGradePtr;
    CString strTmp;

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("papercfgtextbookcmb")));
    pCombo->RemoveAll();

    pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("papercfggradecmb")));
    if (pCombo->GetCount() <= 0)
    {
        return;
    }
    strTmp.Format(_T("Select * from 单元测试教材 where 年级ID=%s Order by ID"),
        pCombo->GetItemAt(pCombo->GetCurSel())->GetUserData().GetData());
    pGradePtr = GetTablesBySql(_T("%s"), strTmp);
    if (pGradePtr == NULL)
    {
        return;
    }
    pCombo = xFindControl(CComboUI, _T("papercfgtextbookcmb"));
    idx = 0, nSel = 0;
    while(!pGradePtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pGradePtr, _T("版本")));
        strTmp = GetCollect(pGradePtr, _T("ID"));
        pElement->SetUserData(strTmp);
        if (lpstrBook && (strTmp == lpstrBook))
        {
            nSel = idx;
        }
        pCombo->Add(pElement);
        idx++;
        pGradePtr->MoveNext();
    }
    pCombo->SelectItem(nSel);

}
void MainFrame::InitPaperCfgUnitCombo(LPCTSTR lpstrUnit/*=NULL*/)
{
    _RecordsetPtr pRecdPtr;
    CComboUI *pComboGrade, *pComboBook, *pComboUnit;
    CListLabelElementUI* pElement;
    CString strGID, strBID;

    pComboUnit = xFindControl(CComboUI, _T("papercfgtextcmb"));
    pComboUnit->RemoveAll();

    pComboGrade = xFindControl(CComboUI, _T("papercfggradecmb"));
    if (pComboGrade->GetCount() <= 0)
    {
        return;
    }
    strGID = pComboGrade->GetItemAt(pComboGrade->GetCurSel())->GetUserData();

    pComboBook = xFindControl(CComboUI, _T("papercfgtextbookcmb"));
    if (pComboBook->GetCount() <= 0)
    {
        return;
    }
    strBID = pComboBook->GetItemAt(pComboBook->GetCurSel())->GetUserData();

    pRecdPtr = GetTablesBySql(_T("Select * from 单元测试课文 where 版本ID=%s Order by ID"),
        strBID);
    if (pRecdPtr == NULL)
    {
        return;
    }
    int nSel = 0, idx = 0;
    CString strId;
    while(!pRecdPtr->GetadoEOF())
    {
        pElement = new CListLabelElementUI;
        pElement->SetText(GetCollect(pRecdPtr, _T("单元名称")));
        strId = GetCollect(pRecdPtr, _T("ID"));
        pElement->SetUserData(strId);
        if (lpstrUnit && (strId == lpstrUnit))
        {
            nSel = idx++;
        }
        pComboUnit->Add(pElement);
        pRecdPtr->MoveNext();
        idx++;
    }
    pComboUnit->SelectItem(nSel);
}

static LPCTSTR sg_PaperImage[] =
{
    _T("MainBackground.png"),
    _T("button1.png"),
    _T("button2.png"),
    _T("close1.png"),
    _T("close2.png"),
    _T("min1.png"),
    _T("min2.png"),
    _T("return1.png"),
    _T("return2.png"),
    _T("Title.png"),
#if SKIN==SKN_SMART
    _T("slogan.png"),
#endif
};

void MainFrame::PaperPrepareImage()
{
    int i;
    CString strTmp;

    for (i = 0; i < _countof(sg_PaperImage); i++)
    {
        strTmp = _T("Image/");
        strTmp += sg_PaperImage[i];
        Image* pImg = CMFCDlgSR::MyLoadImage(strTmp);
        if (pImg)
        {
            strTmp.Format(_T("%spaper/%s"),
                CPaintManagerUI::GetInstancePath().GetData(),
                sg_PaperImage[i]);
            pImg->Save(CT2W(strTmp).m_psz, &m_clsidpng);
            delete pImg;
        }
    }
}

void MainFrame::PaperCleanImage()
{
    int i;
    CString strTmp;

    for (i = 0; i < _countof(sg_PaperImage); i++)
    {
        strTmp.Format(_T("%spaper/%s"),
            CPaintManagerUI::GetInstancePath().GetData(),
            sg_PaperImage[i]);
        ::DeleteFile(strTmp);
    }
}

void MainFrame::PaperCheckit()
{
    _RecordsetPtr pGradePtr, pBookPtr, pUnitPtr, pPaperPtr;
    CString m_strText, m_strAnswer, m_strPoints;
    CString strTmp, strFile;
    int i, j;

    CStdioFile chkFile;
    CFileException fe;
    
    strFile.Format(_T("%sSystem\\试卷检查.txt"), CPaintManagerUI::GetInstancePath().GetData());
    if (!chkFile.Open(strFile, CFile::modeCreate | CFile::modeWrite, &fe))
    {
        fe.ReportError();
        return;
    }

    pGradePtr = GetTablesBySql(_T("%s"), _T("Select * from 单元测试年级 Order by ID"));

    while(!pGradePtr->GetadoEOF())
    {
        pBookPtr = GetTablesBySql(_T("Select * from 单元测试教材 where 年级ID=%s Order by ID"),
            GetCollect(pGradePtr, _T("ID")));
        if (NULL == pBookPtr)
        {
            pGradePtr->MoveNext();
            continue;
        }
        while(!pBookPtr->GetadoEOF())
        {
            pUnitPtr = GetTablesBySql(_T("Select * from 单元测试课文 where 版本ID=%s Order by ID"),
                GetCollect(pBookPtr, _T("ID")));
            if (NULL == pUnitPtr)
            {
                pBookPtr->MoveNext();
                continue;
            }
            while(!pUnitPtr->GetadoEOF())
            {
                SplitResultVector sptAnswer;
                SplitResultVector sptPoints;

                pPaperPtr = GetTablesBySql(_T("select * from 单元测试试卷 where 单元ID=%s and 难度=%d"),
                    GetCollect(pUnitPtr, _T("ID")), 1);
                if (pPaperPtr)
                {
                    m_strText = GetCollect(pPaperPtr, _T("题内容"));
                    HtmlDecode(m_strText);
                    m_strAnswer = GetCollect(pPaperPtr, _T("题答案"));
                    m_strPoints = GetCollect(pPaperPtr, _T("题分数"));


                    i = 0, j = 0;
                    do 
                    {
                        strTmp.Format(_T("inp[%d]"), i++);
                        j = m_strText.Find(strTmp, j);
                    } while (j >= 0);
                    i--;


                    //答案格式A-A-D-C-C-A-B-B-C-A-D-2-4-1-5
                    SplitString(TString(m_strAnswer), _T("-"), sptAnswer);
                    SplitString(TString(m_strPoints), _T("-"), sptPoints);

                    if (sptAnswer.size() != i ||
                        sptAnswer.size() != sptPoints.size() ||
                        sptPoints.size() != i)
                    {
                        strTmp.Format(_T("%s>%s>%s,难度:%s 错误，分数(%d,%s)、答案(%d,%s)、用户答题数(%d)不符！\n\n"),
                            GetCollect(pGradePtr, _T("年级")),
                            GetCollect(pBookPtr, _T("版本")),
                            GetCollect(pUnitPtr, _T("单元名称")),
                            _T("简单"),
                            sptPoints.size(), m_strPoints,
                            sptAnswer.size(), m_strAnswer,
                            i);
                        chkFile.WriteString(strTmp);
                    }

                    pPaperPtr->Close();
                    pPaperPtr.Release();
                }
                else
                {
                    strTmp.Format(_T("%s>%s>%s,难度:%s 没有试卷！\n\n"),
                        GetCollect(pGradePtr, _T("年级")),
                        GetCollect(pBookPtr, _T("版本")),
                        GetCollect(pUnitPtr, _T("单元名称")),
                        _T("简单"));
                    chkFile.WriteString(strTmp);
                }


                pPaperPtr = GetTablesBySql(_T("select * from 单元测试试卷 where 单元ID=%s and 难度=%d"),
                    GetCollect(pUnitPtr, _T("ID")), 2);
                if (pPaperPtr)
                {
                    m_strText = GetCollect(pPaperPtr, _T("题内容"));
                    HtmlDecode(m_strText);
                    m_strAnswer = GetCollect(pPaperPtr, _T("题答案"));
                    m_strPoints = GetCollect(pPaperPtr, _T("题分数"));

                    i = 0, j = 0;
                    do 
                    {
                        strTmp.Format(_T("inp[%d]"), i++);
                        j = m_strText.Find(strTmp, j);
                    } while (j >= 0);
                    i--;


                    //答案格式A-A-D-C-C-A-B-B-C-A-D-2-4-1-5
                    SplitString(TString(m_strAnswer), _T("-"), sptAnswer);
                    SplitString(TString(m_strPoints), _T("-"), sptPoints);

                    if (sptAnswer.size() != i ||
                        sptAnswer.size() != sptPoints.size() ||
                        sptPoints.size() != i)
                    {
                        strTmp.Format(_T("%s>%s>%s,难度:%s,错误，分数(%d,%s)、答案(%d,%s)、用户答题数(%d)不符！\n\n"),
                            GetCollect(pGradePtr, _T("年级")),
                            GetCollect(pBookPtr, _T("版本")),
                            GetCollect(pUnitPtr, _T("单元名称")),
                            _T("困难"),
                            sptPoints.size(), m_strPoints,
                            sptAnswer.size(), m_strAnswer,
                            i);
                        chkFile.WriteString(strTmp);
                    }

                    pPaperPtr->Close();
                    pPaperPtr.Release();
                }
                else
                {
                    strTmp.Format(_T("%s>%s>%s,难度:%s 没有试卷！\n\n"),
                        GetCollect(pGradePtr, _T("年级")),
                        GetCollect(pBookPtr, _T("版本")),
                        GetCollect(pUnitPtr, _T("单元名称")),
                        _T("困难"));
                    chkFile.WriteString(strTmp);
                }

                pUnitPtr->MoveNext();
            }

            pUnitPtr->Close();
            pUnitPtr.Release();

            pBookPtr->MoveNext();
        }

        pBookPtr->Close();
        pBookPtr.Release();

        pGradePtr->MoveNext();
    }

    chkFile.Close();

    strTmp.Format(_T("试卷检查完成，是否打开 %s 文件查看检查结果？"),
        strFile);
    if (AfxMessageBox(strTmp, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        ShellExecute(NULL, _T("open"), strFile, NULL, NULL, SW_SHOW);
    }
}

// DlgPaper.cpp : implementation file
//

#include "stdafx.h"
#include "RIA.h"
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"
#include "DlgPaper.h"

#define HTML_FILE   _T("paper/paper.html")

// CDlgPaper dialog

IMPLEMENT_DYNCREATE(CDlgPaper, CDHtmlDialog)

CDlgPaper::CDlgPaper(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlgPaper::IDD, 0, pParent)
{
}

CDlgPaper::~CDlgPaper()
{
}

void CDlgPaper::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);

    //DDX_DHtml_ElementInnerText(pDX, _T("location"), m_strCurrDir);
    DDX_DHtml_ElementInnerText(pDX, _T("papername"), m_strPaperName);
    DDX_DHtml_ElementInnerText(pDX, _T("level"), m_strLevel);
}

BEGIN_MESSAGE_MAP(CDlgPaper, CDHtmlDialog)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgPaper)
    DHTML_EVENT_ONCLICK(_T("cs_subject2_sub"), OnButtonSubmit)
    DHTML_EVENT_ONCLICK(_T("subject_return"), OnButtonReturn)
    DHTML_EVENT_ONCLICK(_T("subject_min"), OnButtonMin)
    DHTML_EVENT_ONCLICK(_T("subject_close"), OnButtonReturn)
END_DHTML_EVENT_MAP()

//此内容考虑从数据库中读取，便于升级或更新素材
static const TCHAR* const g_strHtmlHead =
_T("<HTML>\n\
<HEAD>\n\
    <link href=\"./%s\" rel=\"stylesheet\" type=\"text/css\"/>\n\
</HEAD>\n\
<BODY>\n\
    <div class=\"main\">\n\
        <div class=\"instructions\">\n\
            <img src=\"./Title.png\" width=\"%d\" height=\"%d\" />\n\
            %s\n\
            <img class=\"min_img\" src=\"./min1.png\" id=\"subject_min\" />\n\
            <img class=\"close_img\" src=\"./close1.png\" id=\"subject_close\" />\n\
        </div>\n\
        <div class=\"cs_con_2\" id=\"cs_main\">\n\
            <div class=\"cs_con_box\">\n\
                <h1 class=\"center\" id=\"papername\"></h1>\n\
                <h2 class=\"center\" id=\"level\"></h2>\n");

static const TCHAR* const g_strHtmlEnd =
_T("\n\
            </div>\n\
        </div>\n\
        <div class=\"cs_bottom_2\">\n\
            <input class=\"bt_input\" id=\"cs_subject2_sub\" type=\"button\" value=\"提 交\"/>\n\
        </div>\n\
        <div class=\"footer\">\n\
            %s\n\
        </div>\n\
    </div>\n\
</BODY>\n\
</HTML>");

BOOL CDlgPaper::OnInitDialog()
{
    CRect clientrect;
    GetClientRect(&clientrect);
    CRect windowrect;
    GetWindowRect(&windowrect);
    int width=1000+windowrect.Width()-clientrect.Width();
    int height=750+windowrect.Height()-clientrect.Height();
    SetWindowPos(NULL,0,0,width,height,SWP_NOMOVE | SWP_NOOWNERZORDER);

    //SetHostFlags(DOCHOSTUIFLAG_SCROLL_NO);
    SetWindowText(m_strCurrDir);
	__super::OnInitDialog();
    Navigate(m_strHtmlPath);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// CDlgPaper message handlers

HRESULT CDlgPaper::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved)
{
#ifdef _DEBUG
    return __super::ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
#else
    return S_OK;
#endif
}

typedef vector<CComPtr<IHTMLElement>> HTML_Span_Array;

HRESULT CDlgPaper::OnButtonSubmit(IHTMLElement* /*pElement*/)
{
    CComPtr<IHTMLElement> cpHtml = NULL;
    //CComPtr<IHTMLInputElement> cpHtml = NULL;
    CString strName, strAnswers = _T(""), strTmp;
    int i = 0;
    HRESULT Hr;

    /*
    <script>  
    $(".bt_c_d_2").click(function(){
    $("#open_div_box").hide();
    $(".zq").show();	 
    var bt_val=$("#cs_subject2_sub");
    var bt_val_2=$("#cs_subject2_sub_2");
    bt_val.css('display','none');
    bt_val_2.css('display','block');	

    $.post('__URL__/da_an',{},function(data){
    if(data.status==1){
    var daan=(data.daan).split('-');
    var error_daan=(data.error_daan).split('-');
    console.log(data.daan);
    console.log(data.error_daan);
    for(var i=0; i<(data.error_daan).length;i++){
    if(error_daan[i]==0){
    $('.zq').eq(i).html('<b class="check_red">╳&nbsp;&nbsp;&nbsp;</b>正确答案为:&nbsp;'+daan[i]);
    }else{
    $('.zq').eq(i).html('<b class="check_green">√</b>');
    }
    }

    }
    else{
    }

    },"json")
    });
    });
    </script>  

    */

    CComPtr<IHTMLDocument2> cpHtmlDoc;
    //CComPtr<IHTMLStyleSheetsCollection> cpSheetCol; 

    GetDHtmlDocument(&cpHtmlDoc);
    CComPtr<IHTMLElementCollection> cpEleColl;
    cpHtmlDoc->get_all(&cpEleColl);
    long ei, el;
    cpEleColl->get_length(&el);
    HTML_Span_Array allspan;
    for (ei = 0; ei < el; ei++)
    {
        CComPtr<IDispatch> cpDispatch;
        CComPtr<IHTMLElement> cpEle;
        cpEleColl->item(CVariant(ei), CVariant(ei), &cpDispatch);
        cpDispatch->QueryInterface(IID_IHTMLElement, (void**)&cpEle);
        CComBSTR sbstr;
        cpEle->get_className(&sbstr);
        TRACE(_T("className:%s\n"), CString(sbstr));
        if (sbstr == _T("zq"))
        {
            allspan.push_back(cpEle);
        }
    }

    do 
    {
        strName.Format(_T("inp[%d]"), i++);
        Hr = GetElement(strName, &cpHtml);
        //Hr = GetElementInterface(strName, IID_IHTMLInputElement, (void**)&cpHtml);
        if (SUCCEEDED(Hr))
        {
            CComVariant cvar;
            cpHtml->getAttribute(L"value", 0, &cvar);
            //CComBSTR bvar;
            //HRESULT vhr = cpHtml->get_value(&bvar);
            strTmp = cvar;
            strTmp.MakeUpper();
            strAnswers += strTmp + _T("-");

            cpHtml.Release();
        }
    } while (SUCCEEDED(Hr));

    strAnswers.Delete(strAnswers.GetLength() - 1);
//#ifdef _DEBUG
//    MyAfxMessageBox(_T("用户答题：%s，答案：%s，分数：%s"),
//        strAnswers, m_strAnswer, m_strPoints);
//#endif
    SplitResultVector sptUser, sptAnswer;
    SplitResultVector sptPoints;
    SplitResultVector::const_iterator p;

    //答案格式A-A-D-C-C-A-B-B-C-A-D-2-4-1-5
    SplitString(TString(strAnswers), _T("-"), sptUser);
    SplitString(TString(m_strAnswer), _T("-"), sptAnswer);

    vector<float> all_point;
    SplitString(TString(m_strPoints), _T("-"), sptPoints);
    for (p = sptPoints.begin(); p != sptPoints.end(); ++p)
    {
#if 1
        all_point.push_back((float)_tstof(p->c_str()));
#else
        int c, a, m;
        SplitResultVector sptP;
        SplitString(*p, _T("-"), sptP);
        m = 0;
        a = _tstoi(sptP[0].c_str());    //大题总分数
        if (sptP.size() == 2)
        {
            c = _tstoi(sptP[1].c_str());    //题数
        }
        else
        {
            c = 1;
        }
        if (c > 0)
        {
            m = a / c;
        }
        all_point.insert(all_point.end(), c, m);
#endif
    }

#ifdef _DEBUG
    if (sptAnswer.size() != allspan.size())
    {
        MyAfxMessageBox(_T("数据库错误，题数(%d)、Span(%d)不符！"),
            (int)sptAnswer.size(), (int)allspan.size());
    }
    if (sptAnswer.size() != all_point.size() ||
        sptAnswer.size() != sptUser.size() ||
        all_point.size() != sptUser.size())
    {
        MyAfxMessageBox(_T("数据库错误，分数(%d,%s)、答案(%d,%s)、用户答题数(%d,%s)不符！"),
            all_point.size(), m_strPoints,
            sptAnswer.size(), m_strAnswer,
            sptUser.size(), strAnswers);
    }
#endif

    //MyAfxMessageBox(_T("用户答题数：%d，答案数：%d，分数个数：%d"),
    //    sptUser.size(), sptAnswer.size(), all_point.size());

    i = 0;
    float sum = 0.0f;
    int nOK = 0;
    for (p = sptAnswer.begin(); p != sptAnswer.end(); ++p)
    {
        if (i < (int)sptUser.size() &&
            i < (int)all_point.size()) //防止数据库错误，题目数目不符合的情况
        {
            strTmp = (*p).c_str();
            if (strTmp.CompareNoCase(sptUser[i].c_str()) == 0)
            {
                sum += all_point[i];
                nOK++;
                strTmp.Format(_T("<b class=\"check_green\">√</b>"));
            }
            else
            {
                strTmp.Format(_T("<b class=\"check_red\">╳&nbsp;&nbsp;&nbsp;</b>正确答案为:&nbsp;%s"),
                    p->c_str());
            }
            CComPtr<IHTMLStyle> cpStyle;
            allspan[i]->get_style(&cpStyle);
            cpStyle->put_display(CComBSTR(_T("inline")));
            allspan[i]->put_innerHTML(CComBSTR(strTmp));
        }
        i++;
    }

    strTmp.Format(_T("共%d题，答对%d题。\r\n得分：%.0f"),
        sptAnswer.size(), nOK, sum);
    AfxMessageBox(strTmp, MB_OK | MB_ICONINFORMATION);


	return S_OK;
}

HRESULT CDlgPaper::OnButtonReturn(IHTMLElement *pElement)
{
    EndDialog(IDCANCEL);
    return S_OK;
}

HRESULT CDlgPaper::OnButtonMin(IHTMLElement *pElement)
{
    ShowWindow(SW_MINIMIZE);
    return S_OK;
}

void CDlgPaper::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
    CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);

    // TODO: Add your specialized code here and/or call the base class
}

BOOL CDlgPaper::PrepareHtml(MainFrame* pMain, int nLevel)
{
    BOOL bRet = FALSE;

    //根据数据库中html内容，生成html文件
    m_strHtmlPath = CPaintManagerUI::GetInstancePath().GetData();
    m_strHtmlPath += HTML_FILE;
    CStdioFile fHtmlFile;
    CFileException fe;

    do 
    {
        if (!fHtmlFile.Open(m_strHtmlPath, CFile::modeCreate | CFile::modeWrite, &fe))
        {
            fe.ReportError();
            break;
        }

        CString strGrade = pMain->ReadSetup(_T("paper_grade"));
        CString strBook = pMain->ReadSetup(_T("paper_book"));
        CString strUnit = pMain->ReadSetup(_T("paper_unit"));

        CString strSql;
        strSql.Format(_T("select 版本 from 单元测试教材 where ID=%s and 年级ID=%s"),
            strBook, strGrade);
        CString strBookName = pMain->GetSingleField(strSql);
        strSql.Format(_T("select 单元名称 from 单元测试课文 where ID=%s and 版本ID=%s"),
            strUnit, strBook);
        CString strUnitName = pMain->GetSingleField(strSql);

        _RecordsetPtr pRecdPtr;
        strSql.Format(_T("select * from 单元测试试卷 where 单元ID=%s and 难度=%d"),
            strUnit, nLevel);
        pRecdPtr = pMain->GetTablesBySql(_T("%s"), strSql);
        if (pRecdPtr == NULL)
        {
            AfxMessageBox(_T("没有试卷。"));
            break;
        }
        m_strText = GetCollect(pRecdPtr, _T("题内容"));
        HtmlDecode(m_strText);

        m_strAnswer = GetCollect(pRecdPtr, _T("题答案"));
        m_strPoints = GetCollect(pRecdPtr, _T("题分数"));

        m_strCurrDir.Format(_T("%s>%s>%s"),
            pMain->GetSingleField(_T("select 年级 from 单元测试年级 where ID=%s"), strGrade),
            strBookName,
            strUnitName);
        m_strPaperName.Format(_T("%s %s"), strBookName, strUnitName);
        m_strLevel = (nLevel == 2) ? _T("困难") : _T("简易");

        CString strHtml, strHtml1, strHtml2;
        int cx, cy;
#if SKIN==SKN_WIN8
        strHtml1 = _T("basis_2.css");
        strHtml2 = _T("");
        cx = 320;
        cy = 89;
#else
        strHtml1 = _T("basis_1.css");
        strHtml2 = _T("<img class=\"return_img\" src=\"./return1.png\" id=\"subject_return\" />");
        cx = 213;
        cy = 63;
#endif
        strHtml.Format(g_strHtmlHead,
            strHtml1, cx, cy, strHtml2);
        fHtmlFile.WriteString(strHtml);
        
        //写入试题
        fHtmlFile.WriteString(m_strText);

#if SKIN==SKN_WIN8
        strHtml2 = _T("<img src=\"./return1.png\" id=\"subject_return\" /> ");
#else
        strHtml2 = _T("<img src=\"./slogan.png\" />");
#endif
        strHtml.Format(g_strHtmlEnd, strHtml2);
        fHtmlFile.WriteString(strHtml);
        fHtmlFile.Close();

        bRet = TRUE;
    } while (false);
    
    return bRet;
}

void CDlgPaper::OnDestroy()
{
    CDHtmlDialog::OnDestroy();

    // TODO: Add your message handler code here
#ifndef _DEBUG
    ::DeleteFile(m_strHtmlPath);
#endif
}

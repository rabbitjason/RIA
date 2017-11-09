// DlgInputName.cpp : implementation file
//

#include "stdafx.h"
#include "RIA.h"
#include "wordmgtbook.h"
#include "DlgInputName.h"


// CDlgInputName dialog

IMPLEMENT_DYNAMIC(CDlgInputName, CDialog)

CDlgInputName::CDlgInputName(CWordMgtBook* pMyParent /*= NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputName::IDD, pParent)
    , m_strInputName(_T(""))
    , m_pMyParent(pMyParent)
{

}

CDlgInputName::~CDlgInputName()
{
}

void CDlgInputName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_INPUTNAME, m_strInputName);
    DDV_MaxChars(pDX, m_strInputName, 255);
    
    DDX_Control(pDX, IDC_EDT_INPUTNAME, m_edtInputName);
}


BEGIN_MESSAGE_MAP(CDlgInputName, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgInputName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputName message handlers

BOOL CDlgInputName::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputName::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    if (!UpdateData(TRUE))
    {
        return;
    }

    if (m_strInputName.IsEmpty())
    {
        AfxMessageBox(_T("名字不能为空。"));
        m_edtInputName.SetFocus();
        return;
    }
    //特殊字符
    static LPCTSTR s_BadString = _T("\"'_\\%[],;&<>?()@*$=+#");
    if (m_strInputName.FindOneOf(s_BadString) >= 0)
    {
        MyAfxMessageBox(_T("名字不能包含特殊字符：%s"), s_BadString);
        return;
    }

    if (!m_pMyParent->myCheckName(m_strInputName))
    {
        AfxMessageBox(_T("名字重复。"));
        m_edtInputName.SetFocus();
        return;
    }
    OnOK();
}

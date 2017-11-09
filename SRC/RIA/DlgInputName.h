#pragma once


// CDlgInputName dialog

class CDlgInputName : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputName)

public:
	CDlgInputName(CWordMgtBook* pMyParent = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInputName();

// Dialog Data
	enum { IDD = IDD_DLG_INPUTNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CEdit   m_edtInputName;
    CWordMgtBook*   m_pMyParent;
public:
    virtual BOOL OnInitDialog();
    CString m_strInputName;
    afx_msg void OnBnClickedOk();
};

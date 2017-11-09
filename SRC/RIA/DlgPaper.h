#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CDlgPaper dialog

class CDlgPaper : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlgPaper)

public:
	CDlgPaper(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPaper();
// Overrides
    HRESULT OnButtonSubmit(IHTMLElement *pElement);
    HRESULT OnButtonReturn(IHTMLElement *pElement);
    HRESULT OnButtonMin(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DLG_PAPER };

    BOOL PrepareHtml(MainFrame*  pParent, int nLevel);  

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
private:
    //MainFrame*  m_pParent;

    //�꼶
    CString m_strGrade;
    //�̲�����
    CString m_strPaperName;
    //�Ѷ�
    CString m_strLevel;
    //��������
    CString m_strText;
    //��
    CString m_strAnswer;
    //����
    CString m_strPoints;

    CString m_strCurrDir;

    CString m_strHtmlPath;
public:
    virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
    afx_msg void OnDestroy();
    HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved);
protected:
};

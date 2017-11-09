#pragma once


// CMFCDlgSR dialog

class CMFCDlgSR : public CDialog
{
	DECLARE_DYNAMIC(CMFCDlgSR)

public:
	CMFCDlgSR(MainFrame* pMyParent = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMFCDlgSR();

// Dialog Data
	enum { IDD = IDD_DLG_SR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    Image*  m_pBkImage;
    Image*  m_pPrgsBkImage;
    Image*  m_pPrgsFrImage;
    MainFrame*  m_pParent;
    int     m_nValue;
    CRect   m_Rct;
public:
    virtual BOOL OnInitDialog();
    static Image* MyLoadImage(LPCTSTR lpstImagePath);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
};

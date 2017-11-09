//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef DIALOG_EDIT_HPP
#define DIALOG_EDIT_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CDialogEdit : public WindowImplBase
{
public:

    CDialogEdit(MainFrame* pParent);
    ~CDialogEdit();
    int m_nType;

public:

    LPCTSTR GetWindowClassName() const;	
    virtual void OnFinalMessage(HWND hWnd);
    virtual void InitWindow();
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual CDuiString GetSkinFile();
    virtual CDuiString GetSkinFolder();
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    //virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LPCTSTR GetResourceID() const;

protected:	

    CDialogEdit(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    void InitCtrls();
    void GetContent();
    void GetContentTxt();

    MainFrame*  m_pParent;
    CString     m_strWndName;
    CString     m_strSqlName;
    CString     m_strSqlGrpName;
    CString     m_strSqlCtntName;
    CString     m_strCustomSenceId;
};

#endif // MAINFRAME_HPP
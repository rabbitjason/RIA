//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDQUIKSELECT_HPP
#define WORDQUIKSELECT_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordQuikselect : public WindowImplBase
{
public:

    CWordQuikselect(MainFrame* pParent);
    ~CWordQuikselect();

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

    CString m_strGrade;
    CString m_strBook;
    CString m_strUnit;
protected:	

    CWordQuikselect(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //³õÊ¼»¯¿Ø¼þ
    void InitControls(TNotifyUI& msg);
    void InitWordGradeCombo(LPCTSTR lpstrGrade = NULL);
    void InitWordBookCombo(LPCTSTR lpstrBook = NULL);
    void InitWordUnitCombo(LPCTSTR lpstrUnit = NULL);
    void InitWordGroupList(LPCTSTR lpstrGroup = NULL);
    MainFrame*  m_pParent;

    bool m_bFreshGrpList;
    bool m_bInMsg;
};

#endif // MAINFRAME_HPP
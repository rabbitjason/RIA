//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDNEWMANAGE_HPP
#define WORDNEWMANAGE_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordMgtnew : public WindowImplBase
{
public:

    CWordMgtnew(MainFrame* pParent);
    ~CWordMgtnew();
    //生词本
    int m_nBook;

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

    CWordMgtnew(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //初始化控件
    void InitControls(TNotifyUI& msg);
    MainFrame*  m_pParent;
};

#endif // MAINFRAME_HPP
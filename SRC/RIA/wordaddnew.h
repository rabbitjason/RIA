//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDADDNEW_HPP
#define WORDADDNEW_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordAddnew : public WindowImplBase
{
public:

    CWordAddnew(MainFrame* pParent);
    ~CWordAddnew();
    //new word id
    int m_nNewWord;

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

    //virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

protected:	

    CWordAddnew(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //³õÊ¼»¯¿Ø¼þ
    void InitControls(TNotifyUI& msg);
    //_RecordsetPtr m_pSetPtr;
    MainFrame*  m_pParent;
};

#endif // MAINFRAME_HPP
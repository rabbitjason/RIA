//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CDictionary : public WindowImplBase//, public IListCallbackUI
{
public:

    CDictionary(MainFrame* pParent);
    ~CDictionary();

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
    virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LPCTSTR GetResourceID() const;

    //virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

protected:	

    CDictionary(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //³õÊ¼»¯´Êµä¿Ø¼þ
    void InitDictControls(TNotifyUI& msg);
    void InitDictWord(int nIndex);
    //MyListText  m_ListDicWords;
    //_RecordsetPtr m_pWordsSetPtr;
    //MyDbCStringArray m_strAllWord;
    MainFrame*  m_pParent;

};

#endif // MAINFRAME_HPP
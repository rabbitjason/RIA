#pragma once
//#include <windows.h>
#include "ssound.h"
#include "IEvalSink.h"

#include <string>
#include <vector>
using namespace std;

class SSEvalMgr
{
public:
    typedef enum {
        E_TYPE_EVAL_WORD,
        E_TYPE_EVAL_SENT,
        E_TYPE_EVAL_PRED
    } EVALTYPE;

    typedef enum {
        E_STATE_BUSY,
        E_STATE_IDLE
    } EVAL_STATE;

public:
    SSEvalMgr(void);
    ~SSEvalMgr(void);

    int Open();
    int Close();
    
    BOOL IsOpen();

    int EvalSentence(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid);

    int EvalWord(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid);

    int EvalPred(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid);

    int EvalPrototype(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, EVALTYPE nType, LPTSTR sid);

    void SetEvalCallback(IEvalSink *pEvalSink);

    BOOL EvalIsCompleted();

    static wstring GetErrMsg(UINT nId);

public:
    struct ssound *m_pEngine;
    string m_strResult;
    HANDLE m_hRecvEvent;
    wstring m_wstrResult;
    vector<IEvalSink *> m_vecEvalSink;
    EVAL_STATE m_nState;


};

// RIA.h : RIA Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRIAApp:
// �йش����ʵ�֣������ RIA.cpp
//

class CRIAApp : public CWinApp
{
public:
	CRIAApp();
    static BSTR m_bstrChartLicense;
    static BSTR m_bstrWMPLicense;

// ��д
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

// ʵ��
protected:
    HANDLE m_hMutex;

public:
	DECLARE_MESSAGE_MAP()
protected:
    void GetApplicationVersion();
    ULONG_PTR m_gdiplusToken;
};

extern CRIAApp theApp;
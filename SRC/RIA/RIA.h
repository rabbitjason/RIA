// RIA.h : RIA 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CRIAApp:
// 有关此类的实现，请参阅 RIA.cpp
//

class CRIAApp : public CWinApp
{
public:
	CRIAApp();
    static BSTR m_bstrChartLicense;
    static BSTR m_bstrWMPLicense;

// 重写
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

// 实现
protected:
    HANDLE m_hMutex;

public:
	DECLARE_MESSAGE_MAP()
protected:
    void GetApplicationVersion();
    ULONG_PTR m_gdiplusToken;
};

extern CRIAApp theApp;
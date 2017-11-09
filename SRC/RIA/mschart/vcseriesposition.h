#if !defined(AFX_VCSERIESPOSITION_H__177E7B57_444A_4FAE_BC44_C99F9D88EC84__INCLUDED_)
#define AFX_VCSERIESPOSITION_H__177E7B57_444A_4FAE_BC44_C99F9D88EC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CVcSeriesPosition wrapper class

class CVcSeriesPosition : public COleDispatchDriver
{
public:
	CVcSeriesPosition() {}		// Calls COleDispatchDriver default constructor
	CVcSeriesPosition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcSeriesPosition(const CVcSeriesPosition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetExcluded();
	void SetExcluded(BOOL bNewValue);
	BOOL GetHidden();
	void SetHidden(BOOL bNewValue);
	short GetOrder();
	void SetOrder(short nNewValue);
	short GetStackOrder();
	void SetStackOrder(short nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCSERIESPOSITION_H__177E7B57_444A_4FAE_BC44_C99F9D88EC84__INCLUDED_)

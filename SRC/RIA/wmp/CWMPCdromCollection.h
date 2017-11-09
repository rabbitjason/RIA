// CWMPCdromCollection.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPCdromCollection

class CWMPCdromCollection : public COleDispatchDriver
{
public:
	CWMPCdromCollection() {}		// Calls COleDispatchDriver default constructor
	CWMPCdromCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPCdromCollection(const CWMPCdromCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	long get_count()
	{
		long result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Item(long lIndex)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, lIndex);
		return result;
	}
	LPDISPATCH getByDriveSpecifier(LPCTSTR bstrDriveSpecifier)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x12f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, bstrDriveSpecifier);
		return result;
	}


};

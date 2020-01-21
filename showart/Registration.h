// Registration.h: interface for the Registration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRATION_H__E94F6D9E_68C7_11D5_A311_000064657374__INCLUDED_)
#define AFX_REGISTRATION_H__E94F6D9E_68C7_11D5_A311_000064657374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Registration  
{
public:
	Registration();
	static void RecursiveDeleteKey(HKEY parentKey, HKEY key, CString keyName);
private:
	void RegisterImage();
	void RegisterList();
	void RecursiveDeleteKey(CString keyName);
	bool checkImageRegistration();
	bool checkListRegistration();
	CString docName;
	void SetShellKey(LPCTSTR key, LPCTSTR value);
	void registerApp();
	char buffer[MAX_PATH + 1];
};

#endif // !defined(AFX_REGISTRATION_H__E94F6D9E_68C7_11D5_A311_000064657374__INCLUDED_)

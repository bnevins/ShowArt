// Registration.cpp: implementation of the Registration class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Registration.h"
#include <afxpriv.h>


//////////////////////////////////////////////////////////////////////////////////

Registration::Registration()
{
	docName = "Showart.Document";
	registerApp();
	bool isReg = checkImageRegistration();

	/** infinite loop !!
	if(isReg == false)
	{
		if(IDYES == AfxMessageBox("JPEG not registered.  Shall I register it to Showart?", MB_YESNO))
		{
			RecursiveDeleteKey(".jpg");
			RegisterImage();
		}
	}
	bool isListReg = checkListRegistration();

	if(isListReg == false)
	{
		if(IDYES == AfxMessageBox(".lst not registered.  Shall I register it to Showart?", MB_YESNO))
		{
			RecursiveDeleteKey(".lst");
			RegisterList();
		}
	}
	*/

}

//////////////////////////////////////////////////////////////////////////////////

void Registration::registerApp()
{
	// always Register this stuff
	CString openCmd;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), openCmd);

	openCmd += " \"%1\"";

	CString command = docName + "\\shell\\open\\command";
	//CString ddeexec = docName + "\\shell\\open\\ddeexec";

	SetShellKey(docName,	"Showart Pix");
	//SetShellKey(ddeexec,	"[open(\"%1\")]");
	SetShellKey(command,	openCmd);
}

//////////////////////////////////////////////////////////////////////////////////

void Registration::SetShellKey(LPCTSTR key, LPCTSTR value)
{
	if (ERROR_SUCCESS != ::RegSetValue(HKEY_CLASSES_ROOT, key, REG_SZ,
			  value, lstrlen(value) * sizeof(TCHAR)))
	{
		TRACE("Warning: registration database update failed for key '%s'.\n",
			key);
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool Registration::checkImageRegistration()
{
	LONG	len = sizeof(buffer);
	
	::RegQueryValue(HKEY_CLASSES_ROOT, ".jpg", buffer, &len);

	if(lstrcmp(buffer, docName))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool Registration::checkListRegistration()
{
	LONG	len = sizeof(buffer);
	
	::RegQueryValue(HKEY_CLASSES_ROOT, ".lst", buffer, &len);

	if(lstrcmp(buffer, docName))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

void Registration::RecursiveDeleteKey(CString keyName)
{
	HKEY key = 0;

	if(ERROR_SUCCESS != ::RegOpenKey(HKEY_CLASSES_ROOT, keyName, &key))
		// doesn't exist.   No problem!
		return;

	RecursiveDeleteKey(HKEY_CLASSES_ROOT, key, keyName);
	::RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////////////

void Registration::RecursiveDeleteKey(HKEY parentKey, HKEY key, CString keyName)
{
	// first recursively delete all subkeys
	while(true)
	{
		HKEY subKey;
		char subKeyName[1024];

		if(ERROR_SUCCESS != ::RegEnumKey(key, 0, subKeyName, sizeof(subKeyName)))
			break;

		if(ERROR_SUCCESS != ::RegOpenKey(key, subKeyName, &subKey))
			return;

		RecursiveDeleteKey(key, subKey, subKeyName);
	}

	// now delete the key...

	::RegCloseKey(key);
	LONG result = ::RegDeleteKey(parentKey, keyName);

	if(result == ERROR_SUCCESS)
		TRACE("Deleted registry key successfully: %s\n", keyName);
	else
		TRACE("Couldn't Delete registry key: %s\n", keyName);
}

//////////////////////////////////////////////////////////////////////////////////

void Registration::RegisterImage()
{
	SetShellKey(".jpg", docName);
}

//////////////////////////////////////////////////////////////////////////////////

void Registration::RegisterList()
{
	SetShellKey(".lst", docName);
}

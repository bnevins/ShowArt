// prefer.cpp : implementation file
//

#include "stdafx.h"
#include "showart.h"
//#include ".\keycommands.h"
#include "Registration.h"
#include "dib_iter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
char	*KeyCommands::sectName				= "Current Scheme"; 
char	*KeyCommands::schemesSectName		= "Key Schemes"; 
const char *KeyCommands::defaultSchemeName	= "DefaultScheme";
const char *KeyCommands::locationName		= "location";
const char *KeyCommands::modeName			= "mode";
const CString KeyCommands::MOVE				= "Move";
const CString KeyCommands::LIST				= "List";
const CString KeyCommands::COPY				= "Copy";
const CString KeyCommands::INDEX			= "Index";
static CString noString;

static CString GetFullPathParentDirName(const CString& fname)
{
	// i.e. -- just hack off the filename

	int index;

	if((index = fname.ReverseFind('\\')) < 0)
		return fname; 
	
	return fname.Left(index);
}

const CString&	KeyCommands::GetKeyCommand(int whichKey)
{
	whichKey = KeyToIndex(whichKey);

	if(whichKey < 0)
		return noString;

	return cmds[whichKey];
}

CString	KeyCommands::GetExpandedKeyCommand(int whichKey)
{
	CString name = GetKeyCommand(whichKey);

	if(name.GetLength() && name[0] == '$')
	{
		CString expanded = rootName;
		expanded += '\\';
		expanded += name.Right(name.GetLength() - 1);
		name = expanded;
	}


	else if(name.GetLength() && name[0] == '%')
	{
		CString expanded = GetFullPathParentDirName(TheDibLib.Curr()->Name());
		expanded += '\\';
		expanded += name.Right(name.GetLength() - 1);
		name = expanded;
	}
	
	return name;
}
	

void KeyCommands::SetKeyCommand(int whichKey, const CString& cmd)
{
	whichKey = KeyToIndex(whichKey);

	if(whichKey < 0)
		return;

	cmds[whichKey] = cmd;
}

KeyCommands::Mode KeyCommands::GetKeyMode(int whichKey)
{
	whichKey = KeyToIndex(whichKey);

	if(whichKey < 0)
		return Move;

	return modes[whichKey];
}

const CString& KeyCommands::GetKeyModeString(int whichKey)
{
	return ModeToString(GetKeyMode(whichKey));
}

void KeyCommands::SetKeyMode(int whichKey, const CString& smode)
{
	whichKey = KeyToIndex(whichKey);
	Mode mode = StringToMode(smode);

	if(whichKey < 0)
		return;

	modes[whichKey] = mode;
}
void KeyCommands::Read()
{
	currentScheme = theApp.GetProfileString(schemesSectName, "CurrentScheme");
	CStringArray sects;
	sects.Add(schemesSectName);
	
	if(currentScheme.IsEmpty())
		currentScheme = defaultSchemeName;

	sects.Add(currentScheme);
	
	int i;

	for(i = 0; i < 26; i++)
	{
		ASSERT(sects.GetCount() == 2);
		CString regKey = (char)(i + 'A');
		sects.Add(regKey);
		cmds[i] = theApp.GetSubProfileString(sects, locationName);
		modes[i] = StringToMode(theApp.GetSubProfileString(sects, modeName, "Move"));
		sects.RemoveAt(2);
	}
	for(i = 0; i < 10; i++)
	{
		ASSERT(sects.GetCount() == 2);
		CString regKey = (char)(i + '0');
		sects.Add(regKey);
		cmds[i+26] = theApp.GetSubProfileString(sects, locationName);
		modes[i+26] = StringToMode(theApp.GetSubProfileString(sects, modeName, "Move"));
		sects.RemoveAt(2);
	}
	rootName = theApp.GetSubProfileString(sects, "RootName");
	useBatchFile = theApp.GetSubProfileBool(sects, "RootName", true);
}

void KeyCommands::Write()
{
	VERIFY(currentScheme.GetLength());
	CStringArray sects;
	sects.Add(schemesSectName);
	sects.Add(currentScheme);
	WriteScheme();
	ASSERT(sects.GetCount() == 2);

	int i;

	for(i = 0; i < 26; i++)
	{
		CString regKey = (char)(i + 'A');
		sects.Add(regKey);
		theApp.WriteSubProfileString(sects, locationName,	cmds[i]);
		theApp.WriteSubProfileString(sects, modeName,		ModeToString(modes[i]));
		sects.RemoveAt(2);
	}
	for(i = 0; i < 10; i++)
	{
		CString regKey = (char)(i + '0');
		sects.Add(regKey);
		theApp.WriteSubProfileString(sects, locationName,	cmds[i+26]);
		theApp.WriteSubProfileString(sects, modeName,		ModeToString(modes[i+26]));
		sects.RemoveAt(2);
	}
	 theApp.WriteSubProfileString(sects,	"RootName",			rootName);
	 theApp.WriteSubProfileBool(sects,		"UseBatchFile",		useBatchFile);
}

	

void KeyCommands::FillSchemesBox(CComboBox & box)
{
	HKEY hSecKey	= theApp.GetSectionKey(schemesSectName);
	char name[1024];

	if (hSecKey)
	{
		LONG ret = ERROR_SUCCESS;

		for(int i = 0;  ret == ERROR_SUCCESS ; i++)
		{
			ret = RegEnumKey(hSecKey, i, name, sizeof(name));
			if(ret == ERROR_SUCCESS)
				box.AddString(name);
		}
		RegCloseKey(hSecKey);
	}
}

void KeyCommands::WriteScheme()
{
	theApp.WriteProfileString(schemesSectName, "CurrentScheme", currentScheme);
}

int	KeyCommands::KeyToIndex(int keyChar)
{
	if(keyChar <= 'Z' && keyChar >= 'A')
		keyChar -= 'A';

	else if(keyChar <= '9' && keyChar >= '0')
	{
		keyChar -= '0';
		keyChar += 26;
	}

	if(keyChar < 0 || keyChar > 35)
		keyChar = -1;
	
	return keyChar;
}

const CString&	KeyCommands::ModeToString(KeyCommands::Mode mode)
{
	if(mode == Copy)
		return COPY;
	
	if(mode == Move)
		return MOVE;
	
	if(mode == Index)
		return INDEX;
	
	if(mode == List)
		return LIST;

	return MOVE;
}

KeyCommands::Mode KeyCommands::StringToMode(LPCTSTR str)
{
	if(!strcmp(str, "Copy"))
		return Copy;

	if(!strcmp(str, "Move"))
		return Move;

	if(!strcmp(str, "Index"))
		return Index;

	if(!strcmp(str, "List"))
		return List;
	
	return Move;
}

void KeyCommands::DeleteScheme(const CString& name)
{
	CString error	= "Couldn't delete Scheme: " + name;
	HKEY	hChild	= NULL;
	HKEY	hParent	= theApp.GetSectionKey(schemesSectName);

	if(hParent == NULL)
		AfxMessageBox(error, MB_ICONEXCLAMATION);

	if(ERROR_SUCCESS != ::RegOpenKeyEx(hParent, name, 0, KEY_ALL_ACCESS, &hChild))
		AfxMessageBox(error, MB_ICONEXCLAMATION);

	Registration::RecursiveDeleteKey(hParent, hChild, name);

	::RegCloseKey(hParent);
}

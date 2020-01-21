#pragma once

class KeyCommands
{
public:
	enum Mode {Copy, Move, Index, List };
	void WriteScheme();
	void FillSchemesBox(CComboBox& box);
	KeyCommands()	{}
	~KeyCommands()	{}
	const CString&	GetKeyCommand(int whichKey);
	CString			GetExpandedKeyCommand(int whichKey);
	void			SetKeyCommand(int whichKey, const CString& cmd);
	Mode			GetKeyMode(int whichKey);
	const CString&	GetKeyModeString(int whichKey);
	void			SetKeyMode(int whichKey, const CString& smode);
	CString&		KeyRootName()				{ return rootName;	}
	void			Read();
	void			Write();
	bool&			UseBatchFile()			{ return useBatchFile; }
	static const char *defaultSchemeName;
	static const char *locationName;
	static const char *modeName;
	CString			GetCurrentScheme()			{ return currentScheme; }
	void			SetCurrentScheme(const CString& scheme)			{ currentScheme = scheme; }
private:
	int			KeyToIndex(int keyChar);
	const CString&		ModeToString(Mode mode);
	Mode		StringToMode(LPCTSTR str);
	CString		currentScheme;
	CString		rootName;
	CString		cmds[36];
	Mode		modes[36];
	bool		useBatchFile;
	static char	*sectName;
	static char	*schemesSectName;
	static const CString MOVE;
	static const CString COPY;
	static const CString LIST;
	static const CString INDEX;
public:
	static void DeleteScheme(const CString& name);
};




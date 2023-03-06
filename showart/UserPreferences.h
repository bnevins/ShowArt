 
class UserPreferences
{
	static char	*sectName;
public:	
	BOOL	showErrorsInMessageBox;
	void	WriteScheme();
	void	FillSchemesBox(CComboBox& box);
	void	SetKeyScheme(const CString& scheme);
	CString	GetKeyScheme()		{ return keys.GetCurrentScheme(); }
	KeyCommands		keys;
	BOOL	defaultFitToWindow; 
	BOOL	debug;
	int		autoTime;
	int		jpegQuality;
	char	openDir[_MAX_DIR];
	char	saveDir[_MAX_DIR];
	int		m_sortType;
	int		m_sortDirection;
	int		superSkipNum;
	BOOL	useDefaultDir;
	BOOL	safeCopyMode;
	CString	defaultDir;
	BOOL	autoCopyPng;
	BOOL	openAllFilesInFolder;
	UserPreferences();
	~UserPreferences();
	void	Read();
	void 	Write();    
	const CString&	GetKeyCommand(int whichKey)						{ return	keys.GetKeyCommand(whichKey);		}
	CString			GetExpandedKeyCommand(int whichKey)				{ return	keys.GetExpandedKeyCommand(whichKey);	}
	void			SetKey(int whichKey, const CString& cmd, const CString& smode) { keys.SetKeyCommand(whichKey, cmd); keys.SetKeyMode(whichKey, smode);}
	void			SetKeyCommand(int whichKey, const CString& cmd)	{			keys.SetKeyCommand(whichKey, cmd);	}
	void			SetKeyMode(int whichKey, const CString& smode)	{			keys.SetKeyMode(whichKey, smode);	}
	KeyCommands::Mode	GetKeyMode(int whichKey)					{ return	keys.GetKeyMode(whichKey);			}
	const CString&	GetKeyModeString(int whichKey)					{ return	keys.GetKeyModeString(whichKey);			}
	CString&		KeyRootName()									{ return	keys.KeyRootName();	}
	bool&			UseBatchFile()									{ return	keys.UseBatchFile();	}
};

extern UserPreferences Prefs;

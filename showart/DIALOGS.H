// dialogs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PreferencesDialog dialog

class PreferencesDialog : public CDialog
{
public:
	PreferencesDialog(CWnd* pParent = NULL);
	enum { IDD = IDD_PREFERENCES };

	CSpinButtonCtrl	m_jpegQualitySpinner;
	//CButton			m_debugDisplayCtrl;
	CSpinButtonCtrl	m_spinner;
	BOOL			m_defaultFitToWindow;
	BOOL			m_debug;
	UINT			m_autoTime;
	BOOL			m_showErrorsInMessageBox;
	UINT			m_jpegQuality;
	int				m_sortType;
	int				m_sortDirection;
	BOOL			m_sortChanged;
	CString			defaultDir;
	BOOL			useDefaultDir;
	BOOL			safeCopyMode;
	BOOL			autoCopyPng;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

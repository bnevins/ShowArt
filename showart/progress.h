// progress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ProgressDlg dialog

class ProgressDlg : public CDialog
{
// Construction
public:
	ProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ProgressDlg)
	enum { IDD = IDD_PROGRESS };
	CStatic	passCtrl;
	CProgressCtrl	progCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ProgressDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

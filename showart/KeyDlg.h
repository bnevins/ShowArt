#include "afxwin.h"
#if !defined(AFX_KEYDLG_H__5086CCC1_CC26_11D0_8A9D_84967A000000__INCLUDED_)
#define AFX_KEYDLG_H__5086CCC1_CC26_11D0_8A9D_84967A000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KeyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KeyDlg dialog

class KeyDlg : public CDialog
{
// Construction
public:
	KeyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KeyDlg)
	enum { IDD = IDD_KEY_COMMANDS };
	CEdit	eNewScheme;
	CComboBox	comboSchemes;
	CString	eA;
	CString	eB;
	CString	eC;
	CString	eD;
	CString	eE;
	CString	eF;
	CString	eG;
	CString	eH;
	CString	eI;
	CString	eJ;
	CString	eK;
	CString	eL;
	CString	eM;
	CString	eN;
	CString	eO;
	CString	eP;
	CString	eQ;
	CString	eR;
	CString	eS;
	CString	eT;
	CString	eU;
	CString	eV;
	CString	eW;
	CString	eX;
	CString	eY;
	CString	eZ;
	CString	rootName;
	CString	e0;
	CString	e1;
	CString	e2;
	CString	e3;
	CString	e4;
	CString	e5;
	CString	e6;
	CString	e7;
	CString	e8;
	CString	e9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KeyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeSchemes();
	afx_msg void OnEditupdateSchemes();
	afx_msg void OnEditchangeSchemes();
	afx_msg void OnAddScheme();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitCombos();
	void InitEarlyFields();
	void InitEditFields();
	void InitModeCombos();
public:
	afx_msg void OnBnClickedSetall();
	CString modeA;
	CString modeB;
	CString modeC;
	CString modeD;
	CString modeE;
	CString modeF;
	CString modeG;
	CString modeH;
	CString modeI;
	CString modeJ;
	CString modeK;
	CString modeL;
	CString modeM;
	CString modeN;
	CString modeO;
	CString modeP;
	CString modeQ;
	CString modeR;
	CString modeS;
	CString modeT;
	CString modeU;
	CString modeV;
	CString modeW;
	CString modeX;
	CString modeY;
	CString modeZ;
	CString mode0;
	CString mode1;
	CString mode2;
	CString mode3;
	CString mode4;
	CString mode5;
	CString mode6;
	CString mode7;
	CString mode8;
	CString mode9;
	CComboBox globalModeCombo;
	afx_msg void OnDeleteScheme();
	BOOL useBatchFile;
	BOOL copyCurrentScheme;
	afx_msg void OnBnClickedClearAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDLG_H__5086CCC1_CC26_11D0_8A9D_84967A000000__INCLUDED_)

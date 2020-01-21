// KeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "showart.h"
#include "KeyDlg.h"
#include ".\keydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KeyDlg dialog


KeyDlg::KeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KeyDlg::IDD, pParent)
	, modeA(_T(""))
	, modeB(_T(""))
	, useBatchFile(FALSE)
	, copyCurrentScheme(FALSE)
{
	//{{AFX_DATA_INIT(KeyDlg)
	eA = _T("");
	eB = _T("");
	eC = _T("");
	eD = _T("");
	eE = _T("");
	eF = _T("");
	eG = _T("");
	eH = _T("");
	eI = _T("");
	eJ = _T("");
	eK = _T("");
	eL = _T("");
	eM = _T("");
	eN = _T("");
	eO = _T("");
	eP = _T("");
	eQ = _T("");
	eR = _T("");
	eS = _T("");
	eT = _T("");
	eU = _T("");
	eV = _T("");
	eW = _T("");
	eX = _T("");
	eY = _T("");
	eZ = _T("");
	rootName = _T("");
	e0 = _T("");
	e1 = _T("");
	e2 = _T("");
	e3 = _T("");
	e4 = _T("");
	e5 = _T("");
	e6 = _T("");
	e7 = _T("");
	e8 = _T("");
	e9 = _T("");
	//}}AFX_DATA_INIT
}


void KeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_SCHEME, eNewScheme);
	DDX_Control(pDX, IDC_SCHEMES, comboSchemes);
	DDX_Text(pDX, IDC_EDIT1, eA);
	DDX_Text(pDX, IDC_EDIT2, eB);
	DDX_Text(pDX, IDC_EDIT3, eC);
	DDX_Text(pDX, IDC_EDIT4, eD);
	DDX_Text(pDX, IDC_EDIT5, eE);
	DDX_Text(pDX, IDC_EDIT6, eF);
	DDX_Text(pDX, IDC_EDIT7, eG);
	DDX_Text(pDX, IDC_EDIT8, eH);
	DDX_Text(pDX, IDC_EDIT9, eI);
	DDX_Text(pDX, IDC_EDIT10, eJ);
	DDX_Text(pDX, IDC_EDIT11, eK);
	DDX_Text(pDX, IDC_EDIT12, eL);
	DDX_Text(pDX, IDC_EDIT13, eM);
	DDX_Text(pDX, IDC_EDIT14, eN);
	DDX_Text(pDX, IDC_EDIT15, eO);
	DDX_Text(pDX, IDC_EDIT16, eP);
	DDX_Text(pDX, IDC_EDIT17, eQ);
	DDX_Text(pDX, IDC_EDIT18, eR);
	DDX_Text(pDX, IDC_EDIT19, eS);
	DDX_Text(pDX, IDC_EDIT20, eT);
	DDX_Text(pDX, IDC_EDIT21, eU);
	DDX_Text(pDX, IDC_EDIT22, eV);
	DDX_Text(pDX, IDC_EDIT23, eW);
	DDX_Text(pDX, IDC_EDIT24, eX);
	DDX_Text(pDX, IDC_EDIT25, eY);
	DDX_Text(pDX, IDC_EDIT26, eZ);
	DDX_Text(pDX, IDC_EDITROOT, rootName);
	DDX_Text(pDX, IDC_EDIT27, e0);
	DDX_Text(pDX, IDC_EDIT28, e1);
	DDX_Text(pDX, IDC_EDIT29, e2);
	DDX_Text(pDX, IDC_EDIT30, e3);
	DDX_Text(pDX, IDC_EDIT31, e4);
	DDX_Text(pDX, IDC_EDIT32, e5);
	DDX_Text(pDX, IDC_EDIT33, e6);
	DDX_Text(pDX, IDC_EDIT34, e7);
	DDX_Text(pDX, IDC_EDIT35, e8);
	DDX_Text(pDX, IDC_EDIT36, e9);
	DDX_CBString(pDX, IDC_MODE1, modeA);
	DDX_CBString(pDX, IDC_MODE2, modeB);
	DDX_CBString(pDX, IDC_MODE3, modeC);
	DDX_CBString(pDX, IDC_MODE4, modeD);
	DDX_CBString(pDX, IDC_MODE5, modeE);
	DDX_CBString(pDX, IDC_MODE6, modeF);
	DDX_CBString(pDX, IDC_MODE7, modeG);
	DDX_CBString(pDX, IDC_MODE8, modeH);
	DDX_CBString(pDX, IDC_MODE9, modeI);
	DDX_CBString(pDX, IDC_MODE10, modeJ);
	DDX_CBString(pDX, IDC_MODE11, modeK);
	DDX_CBString(pDX, IDC_MODE12, modeL);
	DDX_CBString(pDX, IDC_MODE13, modeM);
	DDX_CBString(pDX, IDC_MODE14, modeN);
	DDX_CBString(pDX, IDC_MODE15, modeO);
	DDX_CBString(pDX, IDC_MODE16, modeP);
	DDX_CBString(pDX, IDC_MODE17, modeQ);
	DDX_CBString(pDX, IDC_MODE18, modeR);
	DDX_CBString(pDX, IDC_MODE19, modeS);
	DDX_CBString(pDX, IDC_MODE20, modeT);
	DDX_CBString(pDX, IDC_MODE21, modeU);
	DDX_CBString(pDX, IDC_MODE22, modeV);
	DDX_CBString(pDX, IDC_MODE23, modeW);
	DDX_CBString(pDX, IDC_MODE24, modeX);
	DDX_CBString(pDX, IDC_MODE25, modeY);
	DDX_CBString(pDX, IDC_MODE26, modeZ);
	DDX_CBString(pDX, IDC_MODE27, mode0);
	DDX_CBString(pDX, IDC_MODE28, mode1);
	DDX_CBString(pDX, IDC_MODE29, mode2);
	DDX_CBString(pDX, IDC_MODE30, mode3);
	DDX_CBString(pDX, IDC_MODE31, mode4);
	DDX_CBString(pDX, IDC_MODE32, mode5);
	DDX_CBString(pDX, IDC_MODE33, mode6);
	DDX_CBString(pDX, IDC_MODE34, mode7);
	DDX_CBString(pDX, IDC_MODE35, mode8);
	DDX_CBString(pDX, IDC_MODE36, mode9);
	DDX_Control(pDX, IDC_MODE37, globalModeCombo);
	DDX_Check(pDX, IDC_USE_BATCH_FILE, useBatchFile);
}


BEGIN_MESSAGE_MAP(KeyDlg, CDialog)
	//{{AFX_MSG_MAP(KeyDlg)
	ON_CBN_SELCHANGE(IDC_SCHEMES, OnSelchangeSchemes)
	ON_CBN_EDITUPDATE(IDC_SCHEMES, OnEditupdateSchemes)
	ON_CBN_EDITCHANGE(IDC_SCHEMES, OnEditchangeSchemes)
	ON_BN_CLICKED(IDC_ADD_SCHEME, OnAddScheme)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SetAll, OnBnClickedSetall)
	ON_BN_CLICKED(IDC_DELETE_SCHEME, OnDeleteScheme)
	ON_BN_CLICKED(IDC_CLEAR_ALL, &KeyDlg::OnBnClickedClearAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KeyDlg message handlers

BOOL KeyDlg::OnInitDialog() 
{
	InitEarlyFields();
	CDialog::OnInitDialog();
	InitCombos();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KeyDlg::OnOK() 
{
	CDialog::OnOK();

	//AddBase();
	int index = comboSchemes.GetCurSel();
	
	if(index >= 0)
	{
		CString scheme;
		comboSchemes.GetLBText(index, scheme);
		Prefs.SetKeyScheme(scheme);
	}

	Prefs.KeyRootName()		= rootName;
	Prefs.UseBatchFile()	= useBatchFile;
	Prefs.SetKey('A', eA, modeA);
	Prefs.SetKey('B', eB, modeB);
	Prefs.SetKey('C', eC, modeC);
	Prefs.SetKey('D', eD, modeD);
	Prefs.SetKey('E', eE, modeE);
	Prefs.SetKey('F', eF, modeF);
	Prefs.SetKey('G', eG, modeG);
	Prefs.SetKey('H', eH, modeH);
	Prefs.SetKey('I', eI, modeI);
	Prefs.SetKey('J', eJ, modeJ);
	Prefs.SetKey('K', eK, modeK);
	Prefs.SetKey('L', eL, modeL);
	Prefs.SetKey('M', eM, modeM);
	Prefs.SetKey('N', eN, modeN);
	Prefs.SetKey('O', eO, modeO);
	Prefs.SetKey('P', eP, modeP);
	Prefs.SetKey('Q', eQ, modeQ);
	Prefs.SetKey('R', eR, modeR);
	Prefs.SetKey('S', eS, modeS);
	Prefs.SetKey('T', eT, modeT);
	Prefs.SetKey('U', eU, modeU);
	Prefs.SetKey('V', eV, modeV);
	Prefs.SetKey('W', eW, modeW);
	Prefs.SetKey('X', eX, modeX);
	Prefs.SetKey('Y', eY, modeY);
	Prefs.SetKey('Z', eZ, modeZ);
	Prefs.SetKey('0', e0, mode0);
	Prefs.SetKey('1', e1, mode1);
	Prefs.SetKey('2', e2, mode2);
	Prefs.SetKey('3', e3, mode3);
	Prefs.SetKey('4', e4, mode4);
	Prefs.SetKey('5', e5, mode5);
	Prefs.SetKey('6', e6, mode6);
	Prefs.SetKey('7', e7, mode7);
	Prefs.SetKey('8', e8, mode8);
	Prefs.SetKey('9', e9, mode9);
	Prefs.Write();
}



void KeyDlg::InitEarlyFields()
{
	InitEditFields();
	InitModeCombos();
}

void KeyDlg::InitEditFields()
{
	// special!!
	eV = Prefs.GetKeyCommand('V');
	eS = Prefs.GetKeyCommand('S');
	eR = Prefs.GetKeyCommand('R');
	eG = Prefs.GetKeyCommand('G');
	eB = Prefs.GetKeyCommand('B');

	eA = Prefs.GetKeyCommand('A');
	eC = Prefs.GetKeyCommand('C');
	eD = Prefs.GetKeyCommand('D');
	eE = Prefs.GetKeyCommand('E');
	eF = Prefs.GetKeyCommand('F');
	eH = Prefs.GetKeyCommand('H');
	eI = Prefs.GetKeyCommand('I');
	eJ = Prefs.GetKeyCommand('J');
	eK = Prefs.GetKeyCommand('K');
	eL = Prefs.GetKeyCommand('L');
	eM = Prefs.GetKeyCommand('M');
	eN = Prefs.GetKeyCommand('N');
	eO = Prefs.GetKeyCommand('O');
	eP = Prefs.GetKeyCommand('P');
	eQ = Prefs.GetKeyCommand('Q');
	eT = Prefs.GetKeyCommand('T');
	eU = Prefs.GetKeyCommand('U');
	eW = Prefs.GetKeyCommand('W');
	eX = Prefs.GetKeyCommand('X');
	eY = Prefs.GetKeyCommand('Y');
	eZ = Prefs.GetKeyCommand('Z');
	e0 = Prefs.GetKeyCommand('0');
	e1 = Prefs.GetKeyCommand('1');
	e2 = Prefs.GetKeyCommand('2');
	e3 = Prefs.GetKeyCommand('3');
	e4 = Prefs.GetKeyCommand('4');
	e5 = Prefs.GetKeyCommand('5');
	e6 = Prefs.GetKeyCommand('6');
	e7 = Prefs.GetKeyCommand('7');
	e8 = Prefs.GetKeyCommand('8');
	e9 = Prefs.GetKeyCommand('9');
	rootName = Prefs.KeyRootName();
	useBatchFile = Prefs.UseBatchFile();
}

void KeyDlg::InitModeCombos()
{
	modeA = Prefs.GetKeyModeString('A');
	modeB = Prefs.GetKeyModeString('B');
	modeC = Prefs.GetKeyModeString('C');
	modeD = Prefs.GetKeyModeString('D');
	modeE = Prefs.GetKeyModeString('E');
	modeF = Prefs.GetKeyModeString('F');
	modeG = Prefs.GetKeyModeString('G');
	modeH = Prefs.GetKeyModeString('H');
	modeI = Prefs.GetKeyModeString('I');
	modeJ = Prefs.GetKeyModeString('J');
	modeK = Prefs.GetKeyModeString('K');
	modeL = Prefs.GetKeyModeString('L');
	modeM = Prefs.GetKeyModeString('M');
	modeN = Prefs.GetKeyModeString('N');
	modeO = Prefs.GetKeyModeString('O');
	modeP = Prefs.GetKeyModeString('P');
	modeQ = Prefs.GetKeyModeString('Q');
	modeR = Prefs.GetKeyModeString('R');
	modeS = Prefs.GetKeyModeString('S');
	modeT = Prefs.GetKeyModeString('T');
	modeU = Prefs.GetKeyModeString('U');
	modeV = Prefs.GetKeyModeString('V');
	modeW = Prefs.GetKeyModeString('W');
	modeX = Prefs.GetKeyModeString('X');
	modeY = Prefs.GetKeyModeString('Y');
	modeZ = Prefs.GetKeyModeString('Z');
	mode0 = Prefs.GetKeyModeString('0');
	mode1 = Prefs.GetKeyModeString('1');
	mode2 = Prefs.GetKeyModeString('2');
	mode3 = Prefs.GetKeyModeString('3');
	mode4 = Prefs.GetKeyModeString('4');
	mode5 = Prefs.GetKeyModeString('5');
	mode6 = Prefs.GetKeyModeString('6');
	mode7 = Prefs.GetKeyModeString('7');
	mode8 = Prefs.GetKeyModeString('8');
	mode9 = Prefs.GetKeyModeString('9');
}

void KeyDlg::InitCombos()
{
	Prefs.FillSchemesBox(comboSchemes);

	int index = comboSchemes.FindStringExact(-1, Prefs.GetKeyScheme());
	
	if(index == CB_ERR)
		index = 0;

	comboSchemes.SetCurSel(index);
	globalModeCombo.SetCurSel(0);
}

void KeyDlg::OnSelchangeSchemes() 
{
	int index = comboSchemes.GetCurSel();
	
	if(index >= 0)
	{
		CString scheme;
		comboSchemes.GetLBText(index, scheme);
		Prefs.SetKeyScheme(scheme);
		Prefs.WriteScheme();
	}
	else
	{
		Prefs.SetKeyScheme("");
	}

	// copyCurrentScheme is ONLY set when the button is pressed.  Always reset it to FALSE
		if(!copyCurrentScheme) {
			Prefs.Read();
			InitEditFields();
			UpdateData(FALSE);
		}
}

void KeyDlg::OnEditupdateSchemes() 
{
	// TODO: Add your control notification handler code here
	
}

void KeyDlg::OnEditchangeSchemes() 
{
	// TODO: Add your control notification handler code here
	
}

void KeyDlg::OnAddScheme() 
{
	copyCurrentScheme = TRUE;
	CString newName;
	eNewScheme.GetWindowText(newName);

	if(!newName.IsEmpty())
	{
		int index = comboSchemes.AddString(newName);

		if(index != CB_ERR)
		{
			comboSchemes.SetCurSel(index);
			OnSelchangeSchemes();
		}
	}
}

void KeyDlg::OnBnClickedSetall()
{
	CString m;
	int index = globalModeCombo.GetCurSel();

	if(index < 0)
		return;

	globalModeCombo.GetLBText(index, m);

	if(m.GetLength() < 1)
		return;

	modeA = m;
	modeB = m;
	modeC = m;
	modeD = m;
	modeE = m;
	modeF = m;
	modeG = m;
	modeH = m;
	modeI = m;
	modeJ = m;
	modeK = m;
	modeL = m;
	modeM = m;
	modeN = m;
	modeO = m;
	modeP = m;
	modeQ = m;
	modeR = m;
	modeS = m;
	modeT = m;
	modeU = m;
	modeV = m;
	modeW = m;
	modeX = m;
	modeY = m;
	modeZ = m;
	mode0 = m;
	mode1 = m;
	mode2 = m;
	mode3 = m;
	mode4 = m;
	mode5 = m;
	mode6 = m;
	mode7 = m;
	mode8 = m;
	mode9 = m;

	UpdateData(FALSE);
}

void KeyDlg::OnDeleteScheme()
{
	int index = comboSchemes.GetCurSel();
	
	if(index < 0)
		return;

	CString scheme;
	comboSchemes.GetLBText(index, scheme);
	comboSchemes.DeleteString(index);
	KeyCommands::DeleteScheme(scheme);	

	// check if there are no schemes...
	if(comboSchemes.GetCount() <= 0)
		comboSchemes.AddString(KeyCommands::defaultSchemeName);

	comboSchemes.SetCurSel(0);
	OnSelchangeSchemes();
}

void KeyDlg::OnBnClickedClearAll()
{
	// TODO: Add your control notification handler code here
}

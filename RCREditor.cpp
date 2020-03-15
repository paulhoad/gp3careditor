// RCREditor.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "RCREditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCREditor dialog


CRCREditor::CRCREditor(CWnd* pParent /*=NULL*/)
	: CDialog(CRCREditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCREditor)
	m_CockpitDistance = 0;
	m_OnBoardDistance = 0;
	m_ChaseDistance = 0;
	m_CameraDistance = 0;
	//}}AFX_DATA_INIT
}


void CRCREditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCREditor)
	DDX_Text(pDX, IDC_EDIT1, m_CockpitDistance);
	DDX_Text(pDX, IDC_EDIT2, m_OnBoardDistance);
	DDX_Text(pDX, IDC_EDIT3, m_ChaseDistance);
	DDX_Text(pDX, IDC_EDIT4, m_CameraDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCREditor, CDialog)
	//{{AFX_MSG_MAP(CRCREditor)
	ON_BN_CLICKED(IDC_BUTTON1, OnPatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCREditor message handlers

long GetPCDWord(FILE *fp)
{
        unsigned char tdata[4];
        long  PCword;
	
		fread(tdata,4,sizeof(char),fp);

		//tdata[0] = data[offset];
		//tdata[1] = data[offset+1];
		//tdata[2] = data[offset+2];
		//tdata[3] = data[offset+3];

		//return (MAKELONG(MAKEWORD(data[2],tdata[3]),MAKEWORD(data[0],tdata[1])));
		PCword = tdata[3]*256*256*256 +
                 tdata[2]*256*256 +
                 tdata[1]*256 +
                 tdata[0];
        return (signed long) PCword;

}

void WritePCDWord(FILE *fp,int value)
{
		unsigned char data[4];
	    char c1   = (value &  0x000000FF);
		char c2= ((value & 0x0000FF00)>>8);
		char c3 = ((value & 0x00FF0000)>>16);
		char c4 = ((value & 0xFF000000)>>24);

		data[0] = c1;
		data[1] = c2;
		data[2] = c3;
		data[3] = c4;

		fwrite(data,4,sizeof(char),fp);
}

BOOL CRCREditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();
	CString exePath;
	exePath.Format("%s\\gp3.exe",gp2Location);
	FILE *fp = fopen(exePath,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return false;
	}
	if (!fseek(fp,0x52ef68,SEEK_SET))
	{
		m_CockpitDistance = GetPCDWord(fp);
		m_OnBoardDistance = GetPCDWord(fp);
		m_ChaseDistance   = GetPCDWord(fp);
		m_CameraDistance  = GetPCDWord(fp);
	}

	fclose(fp);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCREditor::OnPatch() 
{
	UpdateData(TRUE);

	CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();
	CString exePath;
	exePath.Format("%s\\gp3.exe",gp2Location);
	FILE *fp = fopen(exePath,"rb+w");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return;
	}
	fseek(fp,0x52ef68,SEEK_SET);
	WritePCDWord(fp,m_CockpitDistance);
	WritePCDWord(fp,m_OnBoardDistance);
	WritePCDWord(fp,m_ChaseDistance);
	WritePCDWord(fp,m_CameraDistance);

	fclose(fp);

	OnOK();
}

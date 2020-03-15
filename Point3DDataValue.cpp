// Point3DDataValue.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "Point3DDataValue.h"
#include "Car.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPoint3DDataValue dialog


CPoint3DDataValue::CPoint3DDataValue(Object3D *obj,CWnd* pParent /*=NULL*/)
	: CDialog(CPoint3DDataValue::IDD, pParent),obj(obj)
{
	//{{AFX_DATA_INIT(CPoint3DDataValue)
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
	//}}AFX_DATA_INIT
}


void CPoint3DDataValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPoint3DDataValue)
	DDX_Control(pDX, IDC_SPIN3, m_SpinZ);
	DDX_Control(pDX, IDC_SPIN2, m_SpinY);
	DDX_Control(pDX, IDC_SPIN1, m_SpinX);
	DDX_Text(pDX, IDC_EDIT1, m_X);
	DDX_Text(pDX, IDC_EDIT2, m_Y);
	DDX_Text(pDX, IDC_EDIT3, m_Z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPoint3DDataValue, CDialog)
	//{{AFX_MSG_MAP(CPoint3DDataValue)
	ON_BN_CLICKED(IDC_X_SIGN, OnXSign)
	ON_BN_CLICKED(IDC_Y_SIGN, OnYSign)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON1, OnViewCenter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPoint3DDataValue message handlers

BOOL CPoint3DDataValue::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SpinX.SetRange(-32760,32760);
	m_SpinY.SetRange(-32760,32760);
	m_SpinZ.SetRange(-32760,32760);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPoint3DDataValue::OnXSign() 
{
}

void CPoint3DDataValue::OnYSign() 
{
}

void CPoint3DDataValue::OnChangeEdit4() 
{
}

void CPoint3DDataValue::OnChangeEdit5() 
{
}

void CPoint3DDataValue::OnSelchangeCombo3() 
{
}

void CPoint3DDataValue::OnSelchangeCombo4() 
{
}

void CPoint3DDataValue::UpdateXY()
{
}

void CPoint3DDataValue::OnViewCenter() 
{
	obj->origin.x = m_X;
	obj->origin.y = m_Y;
	obj->origin.z = m_Z;
	obj->pDoc->UpdateAllViews(NULL);
}

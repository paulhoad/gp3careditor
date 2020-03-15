// Point3DDataValue.h : header file
//
class Object3D;

/////////////////////////////////////////////////////////////////////////////
// CPoint3DDataValue dialog

class CPoint3DDataValue : public CDialog
{
// Construction
public:
	CPoint3DDataValue(Object3D *obj,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPoint3DDataValue)
	enum { IDD = IDD_DIALOG1 };
	CSpinButtonCtrl	m_SpinZ;
	CSpinButtonCtrl	m_SpinY;
	CSpinButtonCtrl	m_SpinX;
	int		m_X;
	int		m_Y;
	int		m_Z;
	//}}AFX_DATA

	Object3D *obj;

	void UpdateXY();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPoint3DDataValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPoint3DDataValue)
	virtual BOOL OnInitDialog();
	afx_msg void OnXSign();
	afx_msg void OnYSign();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnSelchangeCombo4();
	afx_msg void OnViewCenter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if !defined(AFX_RCREDITOR_H__3A68AFE3_9BA8_49FF_96E7_8BB532A0D79B__INCLUDED_)
#define AFX_RCREDITOR_H__3A68AFE3_9BA8_49FF_96E7_8BB532A0D79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCREditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCREditor dialog

class CRCREditor : public CDialog
{
// Construction
public:
	CRCREditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRCREditor)
	enum { IDD = IDD_RCR_IMAGES };
	int		m_CockpitDistance;
	int		m_OnBoardDistance;
	int		m_ChaseDistance;
	int		m_CameraDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCREditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRCREditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnPatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCREDITOR_H__3A68AFE3_9BA8_49FF_96E7_8BB532A0D79B__INCLUDED_)

#pragma once


// CTransCheckBox

class CTransCheckBox : public CButton
{
	DECLARE_DYNAMIC(CTransCheckBox)

public:
	CTransCheckBox();
	virtual ~CTransCheckBox();

	int m_BackColor;

protected:
	DECLARE_MESSAGE_MAP()
public:
	int Hello(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};



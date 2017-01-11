#pragma once


// CxEdit

#define TRANS_BACK -1

class CxEdit : public CEdit
{
	DECLARE_DYNAMIC(CxEdit)

	
public:
	CxEdit();
	virtual ~CxEdit();

	COLORREF    m_TextColor;
    COLORREF    m_BackColor;

private:
    CBrush      m_Brush;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnEnUpdate();
};



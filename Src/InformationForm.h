//InformationForm.h
#ifndef _INFORMATIONFORM_H
#define _INFORMATIONFORM_H
#include"resource.h"

class InformationForm : public CDialog {
public:
	enum { IDD = IDD_INFORMATIONFORM };
public:
	InformationForm(CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnClose();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};
#endif // _INFORMATIONFORM_H

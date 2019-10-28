//MessageForm.h
#ifndef _MESSAGEFORM_H
#define _MESSAGEFORM_H
#include "resource.h"


class MessageForm : public CDialog {
public:
	enum { IDD = IDD_MESSAGEFORM };
public:
	MessageForm(CWnd *parent = NULL);
	BOOL MessageForm::OnInitDialog();
private:
protected:
	afx_msg void OnOKButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()//∏≈≈©∑Œ
};


#endif // _MESSAGEFORM_H
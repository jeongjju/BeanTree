//MessageForm.cpp
#include "stdafx.h"
#include "MessageForm.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MessageForm, CDialog)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOKButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

MessageForm::MessageForm(CWnd *parent) : CDialog(MessageForm::IDD, parent)
{
	CDialog();
}

BOOL MessageForm::OnInitDialog()
{
	CDialog::OnInitDialog();
	//findButton.Create("Ã£±â", BS_DEFPUSHBUTTON, CRect(200, 100, 300, 150), this, 200);
	//CRect rect(85, 110, 180, 210);
	return FALSE;
}

void MessageForm::OnOKButtonClicked() {
	EndDialog(0);
}

void MessageForm::OnClose() {
	EndDialog(0);
}

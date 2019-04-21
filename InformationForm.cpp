//InformationForm.cpp
#include"InformationForm.h"
#include"MindMappingForm.h"
#include<afxdlgs.h>
#include<afxcmn.h>
BEGIN_MESSAGE_MAP(InformationForm,CDialog)
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB, OnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
InformationForm::InformationForm(CWnd *parent) :CDialog(InformationForm::IDD,parent)
{
}
BOOL InformationForm::OnInitDialog() {
	CDialog::OnInitDialog();
	((CTabCtrl*)GetDlgItem(IDC_TAB))->InsertItem(0,_T("프로그램 정보"));
	((CTabCtrl*)GetDlgItem(IDC_TAB))->InsertItem(1, _T("저작권 정보"));
	int nIndex = ((CTabCtrl*)GetDlgItem(IDC_TAB))->GetCurSel();
	if (nIndex == 0) {
		CFile file;
		if (!file.Open(_T("정보_프로그램 정보.txt"), CFile::modeRead)) {
			MessageBox(_T("파일을 열지 못했습니다."), _T("경고!"), MB_OK | MB_ICONHAND);
		}
		UINT length = (UINT)file.GetLength();
		/*size_t length = (size_t)file.GetLength() - 2;
		TCHAR (*text) = new TCHAR[length+sizeof(TCHAR)];
		memset(text, 0, length + sizeof(TCHAR));*/
		TCHAR (*text) = new TCHAR[length];
		file.Read(text, length);
		SetDlgItemText(IDC_STATIC,(LPCTSTR)text);
		delete[] text;
		file.Close();
	}
	return FALSE;
}



void InformationForm::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult) {
	int nIndex = ((CTabCtrl*)GetDlgItem(IDC_TAB))->GetCurSel();
	if (nIndex == 0) {
		CFile file;
		if (!file.Open(_T("정보_프로그램 정보.txt"), CFile::modeRead)) {
			MessageBox(_T("파일을 열지 못했습니다."), _T("경고!"), MB_OK | MB_ICONHAND);
		}
		UINT length = (UINT)file.GetLength();
		char (*text) = new char[length];
		file.Read(text, length);
		SetDlgItemText(IDC_STATIC,(LPCTSTR)text);
		delete[] text;
		file.Close();
	}
	if (nIndex == 1) {
		CFile file;
		if (!file.Open(_T("정보_저작권 정보.txt"), CFile::modeRead)) {
			MessageBox(_T("파일을 열지 못했습니다."), _T("경고!"), MB_OK | MB_ICONHAND);
		}
		UINT length = (UINT)file.GetLength();
		char *text = new char[length];
		file.Read(text, length);
		SetDlgItemText(IDC_STATIC, (LPCTSTR)text);
		delete[]text;
		file.Close();
	}
}
void InformationForm::OnClose() {
	EndDialog(0);
}

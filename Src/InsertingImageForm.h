//InsertingImageForm.h
#ifndef _INSERTINGIMAGEFORM_H
#define _INSERTINGIMAGEFORM_H

using namespace std;

class InsertingImageForm :public CFileDialog{

public:
	InsertingImageForm(CWnd *parent = NULL);
	void OnOpenButtonClicked();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP() // ∏≈≈©∑Œ
};
#endif // _INSERTINGIMAGEFORM_H


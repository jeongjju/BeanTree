//InsertingImageForm.cpp
#include"InsertingImageForm.h"
#include"MindMappingForm.h"
#include"MindMap.h"
#include"Frame.h"
#include"Image.h"
#include"TopicBox.h"
#include"FrameIterator.h"
#include<iostream>
#include<afxdlgs.h>
#include<atlimage.h>


BEGIN_MESSAGE_MAP(InsertingImageForm, CFileDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//LPCTSTR szFilter = _T("JPG File(*.jpg)|*.jpg|GIF File(*.gif)|*.gif|All File(*.*)|*.*|");
InsertingImageForm::InsertingImageForm(CWnd *parent) :CFileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JPG File(*.jpg)|*.jpg|GIF File(*.gif)|*.gif|All File(*.*)|*.*|"), NULL)
{
	CFileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JPG File(*.jpg)|*.jpg|GIF File(*.gif)|*.gif|All File(*.*)|*.*|"), NULL);
}

int InsertingImageForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFileDialog::OnCreate(lpCreateStruct);
	return 0;
}

// 열기 버튼을 클릭했을 때
void InsertingImageForm::OnOpenButtonClicked() {
	CString imagePath = this->GetPathName();
	CImage cImage;
	cImage.Load(imagePath);
	int imageWidth = cImage.GetWidth();
	int imageHeight = cImage.GetHeight();
	int x;
	int y;
	int width;
	int height;
	TCHAR(*topic) = 0;
	TopicBox *topicBox=0;
	Image *image = 0;
	CWinApp *cWinApp = AfxGetApp();
	CWnd *cWnd = cWinApp->GetMainWnd();
	MindMappingForm *mindMapForm = (MindMappingForm *)cWnd;
	Frame *current = mindMapForm->GetCurrent();
	int i = 0;
	while (i < current->GetLength()) {
		Shape *shape = current->GetChild(i);
		if (dynamic_cast<TopicBox*>(shape)) {
			topicBox = (TopicBox*)shape;
			x = topicBox->GetX();
			y = topicBox->GetY();
			width = topicBox->GetWidth();
			height = topicBox->GetHeight();
		}
		i++;
	}
	topicBox->Resize(x + (imageWidth * height) / imageHeight, y, width, height);
	i = 0;
	while (i < current->GetLength()) {
		Shape *shape = current->GetChild(i);
		if (dynamic_cast<TopicBox*>(shape)) {
			image = (Image*)shape;
		}
		i++;
	}
	image->Resize(x, y, (imageWidth*height) / imageHeight, height);
}

void InsertingImageForm::OnClose() {
}

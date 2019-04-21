#include"MindMapApp.h"
#include"MindMappingForm.h"

MindMapApp mindMapApp;
BOOL MindMapApp::InitInstance() {
	MindMappingForm *mindMappingForm = new MindMappingForm;
	
	mindMappingForm->Create(NULL, _T("ºóÆ®¸®"));
	mindMappingForm->ShowWindow(SW_SHOW);
	mindMappingForm->UpdateWindow();
	this->m_pMainWnd = mindMappingForm;
	return TRUE;
}
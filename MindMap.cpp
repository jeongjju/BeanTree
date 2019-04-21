//MindMap.cpp
#include"MindMap.h"
#include"Frame.h"
#include"TopicBox.h"
#include"Image.h"
#include"ConnectingLine.h"
#include"MindMapIterator.h"
MindMap::MindMap() {
	this->centerFrame = 0;
}
MindMap ::~MindMap() {
	if (this->centerFrame != 0) {
		delete this->centerFrame;
	}
}
MindMap::MindMap(const MindMap& source)
{
	if (source.centerFrame != 0) {
		InsertCenterFrame(source.centerFrame->GetX(), source.centerFrame->GetY(), source.centerFrame->GetWidth(), source.centerFrame->GetHeight(),source.centerFrame->GetEmphasize());
		int i = 0;
		Shape* shape;
		while (i < source.centerFrame->GetLength()) {
			shape = (const_cast<MindMap&>(source)).centerFrame->GetChild(i);
			this->centerFrame->Add(shape->Clone());
			i++;
		}
	}
}
MindMap&  MindMap:: operator=(const MindMap& source)
{
	if (this->centerFrame != 0) {
		delete this->centerFrame;
	}
	if (source.centerFrame != 0) {
		InsertCenterFrame(source.centerFrame->GetX(), source.centerFrame->GetY(), source.centerFrame->GetWidth(), source.centerFrame->GetHeight(),source.centerFrame->GetEmphasize());
		int i = 0;
		Shape* shape;
		while (i < source.centerFrame->GetLength()) {
			shape = (const_cast<MindMap&>(source)).centerFrame->GetChild(i);
			this->centerFrame->Add(shape->Clone());
			i++;
		}
	}
	return *this;

}
Frame* MindMap::InsertCenterFrame(int x, int y, int width, int height) {
	this->centerFrame = new Frame(x, y, width, height);
	return this->centerFrame;
}
Frame* MindMap::InsertCenterFrame(int x, int y, int width, int height, bool emphasize) {
	this->centerFrame = new Frame(x, y, width, height, emphasize);
	return this->centerFrame;
}
Iterator<Array<Frame*>>* MindMap::CreateIterator() {
	return new MindMapIterator(this);
}
Frame* MindMap::HighRankFrame(Frame* frame) {
	Frame *highRankFrame = 0;
	bool flag = false;
	Iterator<Array<Frame*>> *mindMapIterator = this->CreateIterator();
	for (mindMapIterator->First(); !mindMapIterator->IsDone(); mindMapIterator->Next()) {
		Array<Frame*> arrayFrame = mindMapIterator->Current();
		int length = arrayFrame.GetLength();
		int i = 0;
		while (i < length&&flag != true) {
			Frame *temp = arrayFrame.GetAt(i);
			int j = 0;
			int frameLength = temp->GetLength();
			while (j < frameLength) {
				if (temp->GetChild(j) == frame) {
					highRankFrame = temp;
					flag = true;
				}
				j++;
			}
			i++;
		}
	}
	return highRankFrame;
}

int MindMap::Save() {
	int underFrame = 0;
	int frameOrder = 0;
	int type = 0;
	FILE *file = fopen("MindMap.dat", "wb,ccs=UNICODE");
	if (file != 0) {
		if (this->centerFrame != 0) {
			fwrite(&frameOrder, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			int x = this->centerFrame->GetX();
			int y = this->centerFrame->GetY();
			int width = this->centerFrame->GetWidth();
			int height = this->centerFrame->GetHeight();
			int length = this->centerFrame->GetLength();
			int capacity = this->centerFrame->GetCapacity();
			bool emphasize = this->centerFrame->GetEmphasize();
			fwrite(&x, sizeof(int), 1, file);
			fwrite(&y, sizeof(int), 1, file);
			fwrite(&width,sizeof(int),1,file);
			fwrite(&height, sizeof(int), 1, file);
			fwrite(&length, sizeof(int), 1, file);
			fwrite(&capacity, sizeof(int), 1, file);
			fwrite(&emphasize, sizeof(bool), 1, file);
			underFrame = ++frameOrder;

			int i = 0;
			while (i < this->centerFrame->GetLength()) {
				Shape *shape = this->centerFrame->GetChild(i);
				if (dynamic_cast<Frame*>(shape)) {
					type = 0;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					int x = ((Frame*)shape)->GetX();
					int y = ((Frame*)shape)->GetY();
					int width = ((Frame*)shape)->GetWidth();
					int height = ((Frame*)shape)->GetHeight();
					int length = ((Frame*)shape)->GetLength();
					int capacity = ((Frame*)shape)->GetCapacity();
					bool emphasize = ((Frame*)shape)->GetEmphasize();

					fwrite(&x, sizeof(int), 1, file);
					fwrite(&y, sizeof(int), 1, file);
					fwrite(&width, sizeof(int), 1, file);
					fwrite(&height, sizeof(int), 1, file);
					fwrite(&length, sizeof(int), 1, file);
					fwrite(&capacity, sizeof(int), 1, file);
					fwrite(&emphasize, sizeof(bool), 1, file);
					frameOrder++;
					Node_Save(file, (Frame*)shape, &frameOrder);
				}
				else if (dynamic_cast<TopicBox*>(shape)) {
					type = 1;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					int x = ((TopicBox*)shape)->GetX();
					int y = ((TopicBox*)shape)->GetY();
					int width = ((TopicBox*)shape)->GetWidth();
					int height = ((TopicBox*)shape)->GetHeight();
					int length = ((TopicBox*)shape)->GetLength();
					int capacity = ((TopicBox*)shape)->GetCapacity();

					fwrite(&x, sizeof(int), 1, file);
					fwrite(&y, sizeof(int), 1, file);
					fwrite(&width, sizeof(int), 1, file);
					fwrite(&height, sizeof(int), 1, file);
					fwrite(&length, sizeof(int), 1, file);
					fwrite(&capacity, sizeof(int), 1, file);
					fwrite(((TopicBox*)shape)->GetTopic(), length, 1, file);
				}
				else if (dynamic_cast<Image*>(shape)) {
					type = 2;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					fwrite((Image*)shape, sizeof(Image), 1, file);
				}
				else if (dynamic_cast<ConnectingLine*>(shape)) {
					type = 3;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					fwrite((ConnectingLine*)shape, sizeof(ConnectingLine), 1, file);
				}
				i++;
			}
		}
		fclose(file);
	}
	return frameOrder;
}

int MindMap::SaveAs(string filePath) {
	int underFrame = 0;
	int frameOrder = 0;
	int type = 0;
	FILE *file;
	file = fopen(filePath.c_str(), "wb");
	if (file != 0) {
		if (this->centerFrame != 0) {
			fwrite(&frameOrder, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			int x = this->centerFrame->GetX();
			int y = this->centerFrame->GetY();
			int width = this->centerFrame->GetWidth();
			int height = this->centerFrame->GetHeight();
			int length = this->centerFrame->GetLength();
			int capacity = this->centerFrame->GetCapacity();
			bool emphasize = this->centerFrame->GetEmphasize();
			fwrite(&x, sizeof(int), 1, file);
			fwrite(&y, sizeof(int), 1, file);
			fwrite(&width, sizeof(int), 1, file);
			fwrite(&height, sizeof(int), 1, file);
			fwrite(&length, sizeof(int), 1, file);
			fwrite(&capacity, sizeof(int), 1, file);
			fwrite(&emphasize, sizeof(bool), 1, file);
			underFrame = ++frameOrder;

			int i = 0;
			while (i < this->centerFrame->GetLength()) {
				Shape *shape = this->centerFrame->GetChild(i);
				if (dynamic_cast<Frame*>(shape)) {
					type = 0;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					int x = ((Frame*)shape)->GetX();
					int y = ((Frame*)shape)->GetY();
					int width = ((Frame*)shape)->GetWidth();
					int height = ((Frame*)shape)->GetHeight();
					int length = ((Frame*)shape)->GetLength();
					int capacity = ((Frame*)shape)->GetCapacity();
					bool emphasize = ((Frame*)shape)->GetEmphasize();

					fwrite(&x, sizeof(int), 1, file);
					fwrite(&y, sizeof(int), 1, file);
					fwrite(&width, sizeof(int), 1, file);
					fwrite(&height, sizeof(int), 1, file);
					fwrite(&length, sizeof(int), 1, file);
					fwrite(&capacity, sizeof(int), 1, file);
					fwrite(&emphasize, sizeof(bool), 1, file);
					frameOrder++;
					Node_Save(file, (Frame*)shape, &frameOrder);
				}
				else if (dynamic_cast<TopicBox*>(shape)) {
					type = 1;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					int x = ((TopicBox*)shape)->GetX();
					int y = ((TopicBox*)shape)->GetY();
					int width = ((TopicBox*)shape)->GetWidth();
					int height = ((TopicBox*)shape)->GetHeight();
					int length = ((TopicBox*)shape)->GetLength();
					int capacity = ((TopicBox*)shape)->GetCapacity();

					fwrite(&x, sizeof(int), 1, file);
					fwrite(&y, sizeof(int), 1, file);
					fwrite(&width, sizeof(int), 1, file);
					fwrite(&height, sizeof(int), 1, file);
					fwrite(&length, sizeof(int), 1, file);
					fwrite(&capacity, sizeof(int), 1, file);
					fwrite(((TopicBox*)shape)->GetTopic(), length, 1, file);
				}
				else if (dynamic_cast<Image*>(shape)) {
					type = 2;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					fwrite((Image*)shape, sizeof(Image), 1, file);
				}
				else if (dynamic_cast<ConnectingLine*>(shape)) {
					type = 3;
					fwrite(&underFrame, sizeof(int), 1, file);
					fwrite(&type, sizeof(int), 1, file);
					fwrite((ConnectingLine*)shape, sizeof(ConnectingLine), 1, file);
				}
				i++;
			}
		}
		fclose(file);
	}
	return frameOrder;
}

void MindMap::Node_Save(FILE *file, Frame *frame, int *frameOrder) {
	int type;
	int underFrame = *frameOrder;
	int i = 0;
	while (i < frame->GetLength()) {
		Shape *shape = frame->GetChild(i);
		if (dynamic_cast<Frame*>(shape)) {
			type = 0;
			fwrite(&underFrame, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			int x = ((Frame*)shape)->GetX();
			int y = ((Frame*)shape)->GetY();
			int width = ((Frame*)shape)->GetWidth();
			int height = ((Frame*)shape)->GetHeight();
			int length = ((Frame*)shape)->GetLength();
			int capacity = ((Frame*)shape)->GetCapacity();
			bool emphasize = ((Frame*)shape)->GetEmphasize();
			fwrite(&x, sizeof(int), 1, file);
			fwrite(&y, sizeof(int), 1, file);
			fwrite(&width, sizeof(int), 1, file);
			fwrite(&height, sizeof(int), 1, file);
			fwrite(&length, sizeof(int), 1, file);
			fwrite(&capacity, sizeof(int), 1, file);
			fwrite(&emphasize, sizeof(bool), 1, file);
			(*frameOrder)++;
			Node_Save(file, (Frame*)shape, frameOrder);
		}
		else if (dynamic_cast<TopicBox*>(shape)) {
			type = 1;
			fwrite(&underFrame, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			int x = ((TopicBox*)shape)->GetX();
			int y = ((TopicBox*)shape)->GetY();
			int width = ((TopicBox*)shape)->GetWidth();
			int height = ((TopicBox*)shape)->GetHeight();
			int length = ((TopicBox*)shape)->GetLength();
			int capacity = ((TopicBox*)shape)->GetCapacity();

			fwrite(&x, sizeof(int), 1, file);
			fwrite(&y, sizeof(int), 1, file);
			fwrite(&width, sizeof(int), 1, file);
			fwrite(&height, sizeof(int), 1, file);
			fwrite(&length, sizeof(int), 1, file);
			fwrite(&capacity, sizeof(int), 1, file);
			fwrite(((TopicBox*)shape)->GetTopic(), length, 1, file);

		}
		else if (dynamic_cast<Image*>(shape)) {
			type = 2;
			fwrite(&underFrame, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			fwrite((Image*)shape, sizeof(Image), 1, file);
		}
		else if (dynamic_cast<ConnectingLine*>(shape)) {
			type = 3;
			fwrite(&underFrame, sizeof(int), 1, file);
			fwrite(&type, sizeof(int), 1, file);
			fwrite((ConnectingLine*)shape, sizeof(ConnectingLine), 1, file);
		}
		i++;
	}
}

int MindMap::Load(string filePath) {
	//1. 파일을 읽어온다.

	FILE *file = fopen(filePath.c_str(), "rb,ccs=UNICODE");
	int flag;
	int underFrame;
	int index = 0;
	Array<Frame*> indexes;

	
	flag = fread(&underFrame, sizeof(int), 1, file);
	if (!feof(file) && file != 0) {
		int type;
		int x;
		int y;
		int width;
		int height;
		int length;
		int capacity;
		bool emphasize;
		fread(&type, sizeof(int), 1, file);
		fread(&x, sizeof(int), 1, file);
		fread(&y, sizeof(int), 1, file);
		fread(&width, sizeof(int), 1, file);
		fread(&height, sizeof(int), 1, file);
		fread(&length, sizeof(int), 1, file);
		fread(&capacity, sizeof(int), 1, file);
		fread(&emphasize, sizeof(bool), 1, file);
		Frame *frame = this->InsertCenterFrame(x, y, width, height,emphasize);
		indexes.Store(index,frame);
		index++;
		Node_Load(file,&indexes,&index);
		fclose(file);
	}
	return index;
}
void MindMap::Node_Load(FILE *file, Array<Frame*> *indexes, int *index)
{
	int underFrame;
	int type;
	int flag;
	flag = fread(&underFrame, sizeof(int), 1, file);
	if (!feof(file) && flag != 0) {
		int x;
		int y;
		int width;
		int height;
		int length;
		int capacity;
		bool emphasize;
		Image image;
		ConnectingLine connectingLine;
		Frame *upperFrame;
		int frameIndex;
		fread(&type, sizeof(int), 1, file);
		switch (type)
		{
		case 0:
			fread(&x, sizeof(int), 1, file);
			fread(&y, sizeof(int), 1, file);
			fread(&width, sizeof(int), 1, file);
			fread(&height, sizeof(int), 1, file);
			fread(&length, sizeof(int), 1, file);
			fread(&capacity, sizeof(int), 1, file);
			fread(&emphasize, sizeof(bool), 1, file);
			upperFrame = indexes->GetAt(underFrame-1);
			frameIndex = upperFrame->Add(new Frame(x, y, width, height,emphasize));
			indexes->Store(*index, (Frame*)upperFrame->GetChild(frameIndex));
			(*index)++;
			break;
		case 1:
			int x;
			int y;
			int width;
			int height;
			int length;
			int capacity;
			TCHAR(*topic);
			fread(&x, sizeof(int), 1, file);
			fread(&y, sizeof(int), 1, file);
			fread(&width, sizeof(int), 1, file);
			fread(&height, sizeof(int), 1, file);
			fread(&length, sizeof(int), 1, file);
			fread(&capacity, sizeof(int), 1, file);
			topic = new TCHAR[length];
			fread(topic, length, 1, file);
			upperFrame = indexes->GetAt(underFrame - 1);
			upperFrame->Add(new TopicBox(x, y, width, height, topic));
			if (topic != 0) {
				delete[] topic;
			}
			break;
		case 2:
			fread(&image, sizeof(Image), 1, file);
			upperFrame = indexes->GetAt(underFrame - 1);
			upperFrame->Add(new Image(image.GetX(),image.GetY(),image.GetWidth(),image.GetHeight(),image.GetImagePath()));
			break;
		case 3:
			fread(&connectingLine, sizeof(ConnectingLine), 1, file);
			upperFrame = indexes->GetAt(underFrame - 1);
			upperFrame->Add(new ConnectingLine(connectingLine.GetX(),connectingLine.GetY(),connectingLine.GetChildX(),connectingLine.GetChildY()));
			break;
		default:
			break;
		}
		Node_Load(file, indexes, index);
	}
}
//#include"TopicBox.h"
//#include"Image.h"
//#include<iostream>
//int main(int argc, char *argv[]) {
	//MindMap mindMap;
	//Frame *centerFrame =mindMap.InsertCenterFrame(10, 20, 30, 40);
	//centerFrame->Add(new TopicBox);
	//Frame *frame1 = (Frame*)centerFrame->GetChild(centerFrame->Add(new Frame));
	//centerFrame->Add(new Image);
	//frame1->Add(new TopicBox);
	//mindMap.Save();
//	return 0;
//}
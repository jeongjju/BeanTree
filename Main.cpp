//#include<iostream>
//using namespace std;
//#include"Array.h"
//int CompareInt(void *one, void *other);
//int main(int argc, char *argv[]) {
//	Array<int>array(20);
//	int i = 0;
//	// 0 - 10
//	int index = array.Store(i++, 10);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	// 1 - 5
//	index = array.Store(i++, 5);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	// 2 - 8
//	index = array.Store(i++, 8);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	// 3 - 15
//	index = array.Store(i++, 15);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	// 4 - 3
//	index = array.Store(i++, 3);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	// 5 - 1
//	index = array.Store(i++, 1);
//	cout << " index " << index << " array 값 " << array[index] << endl;
//	array.BubbleSort(CompareInt);
//	cout << " ------------ 전체 출력 -----------" << endl;
//	int j = 0;
//	while (j < array.GetLength()) {
//		cout << " index " << j << " array 값 " << array[j] << endl;
//		j++;
//	}
//	return 0;
//}
//int CompareInt(void *one, void *other) {
//	int ret=0;
//	if (*(static_cast<int*>(one)) > *(static_cast<int*>(other))) {
//		ret = 1;
//	}
//	return ret;
//}
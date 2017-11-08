// Euler's algorithm.cpp: ���������� ����� ����� ��� ����������� ����������.
/**	\brief  ���������� ��������� ������
	\author �������� �.�.
	\date   17.04.2017 - ������
			29.04.2017 - ����� */

#include "stdafx.h"
#include <cstdio>
#include <conio.h>
#include "Matrix.h"

int main()
{
	int ans;
	cout << "	---Step 1---\nInput node amount, between 5 and 10.\n>> ";
	cin >> ans;
	Matrix* arr_list = new Matrix(ans);
	arr_list->CreateEulerGraff();
	if (!arr_list->IsCorrectGraff())
	{
		cout << "The created graph isn't euler.\n\n		---Step 1-1---\nPress any key to correct created graph.\n";
		system("pause");
		arr_list->CorrectGraff();
	}
	cout << "Euler graph created.\n	---Step 2---\nPress any key to make euler track.\n";
	system("pause");
	arr_list->MakeTrack();
	cout << "\nThat's it!\n";
	system("pause");
    return 0;
}
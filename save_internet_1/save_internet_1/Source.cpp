#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <conio.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include <locale.h>
using namespace std;
int c;
void encryption(int key[])
{
	int j = 0, a = 0;
	char* v1;
	ifstream ifs("Test.exe", ios::binary);
	size_t lSize = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024;
	ifs.seekg(0);
	c = 10 - lSize % 10;
	v1 = new char[lSize + c];
	char *buffer = new char[lSize + c];
	ifs.read(buffer, lSize);//—читывает заданное число байт из файла в пам€ть
	for (int i = 0; i < lSize + c; i++) {
		v1[i] = buffer[j + key[a] - 1];
		if (a > 8) {
			a = 0;
			j = j + 10;
		}
		else {
			a++;
		};
	}
	ofstream fout("Result.txt", ios_base::trunc | ios::binary);//trunc обрезает файл, чтобы он был опустошен
	for (int i = 0; i < (lSize + c); i++)
	{
	fout << v1[i];
	}
	delete[] buffer;
	fout.close();
}

void decryption(int key[]) {
	int j = 0, a = 0;
	char* v1;
	ifstream ifs("Result.txt", ios::binary);
	size_t lSize = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024;
	ifs.seekg(0);
	v1 = new char[lSize];
	char *buffer = new char[lSize];
	ifs.read(buffer, lSize);//—читывает заданное число байт из файла в пам€ть
	for (int i = 0; i < lSize; i++)
	{
		v1[j + key[a] - 1] = buffer[i];
		if (a > 8) {
			a = 0;
			j = j + 10;
		}
		else {
			a++;
		};
	}
	ofstream fout("New.txt", ios_base::trunc | ios::binary);//trunc обрезает файл, чтобы он был опустошен
	for (int i = 0; i < lSize - c; i++)
	{
		fout << v1[i];
	}
	delete[] buffer;
	fout.close();
}
void menu() {
	char ch_key;

	int c, key[10] = { 3, 9, 10, 5, 7, 1, 2, 8, 6, 4 };
	cout << "1 - Encrypt file.\n";
	cout << "2 - Decrypt file.\n";
	cout << "ESC - Exit.\n";
	switch ((ch_key = _getch()))
	{
	case '1':
		cout << "File Encryption...\n";
		encryption(key);
		cout << "Done.\n";
		system("pause");
		break;
	case '2':
		cout << "Decryption file...\n";
		decryption(key);
		cout << "Done.\n";
		system("pause");
		break;
	case 27:
		cout << "Exit the program....\n";
		exit(0);
		break;
	default:
		break;
	}
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	while (true) {
		system("cls");
		menu();
	}
	system("pause");
}
#include <fstream>
#include <conio.h>
#include <vector>
#include <iostream>
using namespace std;

void encryption(int key[]) {
	int val = 0, c = 0, d = 0, encr = 0;
	ifstream ifs("Test.exe", ios::binary);
	size_t size = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024;
	ifs.seekg(0);
	c = 4 - size % 4;
	ofstream fout("Result.txt", ios_base::trunc | ios::binary);
	for (int i = 0; i <= size / 4; i++) {
		char *buf = new char[block];
		ifs.read(buf, block);
		for (int j = 0; j < (block / 4); j++) {
			int *link = (j + (int*)buf);

		encr = 0;
			for (int i = 0; i < 32; i++) {
				val = 1 & (*link >> (31 - key[i]));
				if (val != 0) {
					encr = encr | (1 << (31 - i));
				}
				val = 0;
			}
			char buffer[4];
			for (int i = 1; i < 5; i++) {
				buffer[i - 1] = encr >> (32 - (i * 8));
			}
			for (int i = 0; i < 4; i++) {
				if (d < size + c) {
					fout << buffer[3 - i];
				}
				d++;
			}
		}
		delete[] buf;
	}
	fout << c;
}

void decryption(int key[]) {
	int val = 0, encr = 0, c = 0;
	char minus;
	ifstream ifs("Result.txt", ios::binary);
	size_t size = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024;
	ifs.seekg(size - 1);
	ifs.get(minus);
	int intminus = minus - '0';
	ifs.seekg(0);
	ofstream fout("New.txt", ios_base::trunc | ios::binary);
	for (int i = 0; i <= size / 4; i++) {
		char * buf = new char[block];
		ifs.read(buf, block);
		for (int j = 0; j < (block / 4); j++) {
			int *link = (j + (int*)buf);
			encr = 0;
			for (int i = 0; i < 32; i++) {
				val = 1 & (*link >> (31 - i));
				if (val != 0) {
					encr = encr | (1 << (31 - key[i]));
				}
				val = 0;
			}
			char buffer[4];
			for (int i = 1; i < 5; i++) {
				buffer[i - 1] = encr >> (32 - (i * 8));
			}
			for (int i = 0; i < 4; i++) {
				if (c < size - intminus - 1) {
					fout << buffer[3 - i];
				}
				c++;
			}
		}
		delete[] buf;
	}
}

void menu()
{
	char ch_key;
	int key[] = { 6, 29, 17, 3, 26, 13, 7, 0, 8, 15, 12, 19, 21, 5, 28, 16, 23, 24, 1, 2, 25, 30, 31, 10, 9 , 14, 27, 18, 22, 4, 11, 20};
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
	//setlocale(LC_ALL, "rus");

	while (true)
	{
		system("cls");
		menu();
	}
	system("pause");
}
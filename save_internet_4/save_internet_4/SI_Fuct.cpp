#include "SI.h"

void GetOffset(unsigned int * pOffset, const char * Buffer)
{
	*pOffset = 0; //��������� ��������
	*pOffset = *pOffset | Buffer[13];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[12];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[11];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[10];
}

void Check(const wchar_t * fileName)
{
	HANDLE hFile;
	char * Buffer;
	DWORD dwBytes;
	
	////�������� �� ������� ���������� ���������
	//��������� �����������
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(fileName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open file\n";
	
	//������ ���������� � ����� � 40 ���� ������
	Buffer = new char[100];
	ReadFile(hFile, Buffer, 100, &dwBytes, NULL);//���� 100 ������ ����
	
	unsigned int uiOffset;
	GetOffset(&uiOffset, Buffer);
	
	//������������ ����� �� �����������
	//�� �������� � 40 ����
	unsigned char cTag2[5] = { 0 };
	
	//��������� �������� ������ � ������������ �����
	char cImageByte;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			char cImageBit = 0;
			cImageByte = Buffer[uiOffset];
			cImageBit = cImageByte & cMask;
				cImageBit = cImageBit << 7;
			cTag2[i] = cTag2[i] >> 1;
			cTag2[i] = cTag2[i] | cImageBit;
			uiOffset++;
		}
	}
	char cTagTemp[5] = { cTag2[0], cTag2[1], cTag2[2], cTag2[3],
	cTag2[4] };//��� ��������� � ������
	if (strcmp(cTag, cTagTemp))
	{
		printf("No secret text here.\n");
	}
	else
		printf("Secret text.\n");
	CloseHandle(hFile);
	delete[] Buffer;
}
void Retrieve(const wchar_t * imageName, const wchar_t * textName)
{
	HANDLE hFile;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(imageName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open file\n";
	DWORD dwBytes = 0, BufSize = 0, dwBytes1 = 0;
	BufSize = GetFileSize(hFile, NULL);
	char * Buffer = new char[BufSize / sizeof(char)];
	ReadFile(hFile, Buffer, BufSize, &dwBytes, NULL);
	unsigned int uiOffset;
	GetOffset(&uiOffset, Buffer);
	char cTagTemp[5], cImageByte;
	unsigned char cTag2[5] = { 0 };
	
	//�������� �����
	for (int i = 0; i < 5; i++)
		cTag2[i] = 0;//���� ��� ��������
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			char cImageBit = 0;
			cImageByte = Buffer[uiOffset];
			cImageBit = cImageByte & cMask;
				cImageBit = cImageBit << 7;
			cTag2[i] = cTag2[i] >> 1;
			cTag2[i] = cTag2[i] | cImageBit;
			uiOffset++;
		}
	}
	for (int i = 0; i < 5; i++)
		cTagTemp[i] = cTag2[i];
	if (!strcmp(cTag, cTagTemp))//���� ����� ���� ����� ������	������ � ��������������� ���
	{
		//��������� ������
		unsigned char cTextSize[4] = { 0 };
		for (int i = 0; i < 4; i++)
		{
		for (int j = 0; j < 8; j++)
		{
		char cImageBit = 0;
		cImageByte = Buffer[uiOffset];
		cImageBit = cImageByte & cMask;
		cImageBit = cImageBit << 7;
		cTextSize[i] = cTextSize[i] >> 1;
		cTextSize[i] = cTextSize[i] | cImageBit;
		uiOffset++;
		}
		}
		//�������������� � int cTextSize
		int iTextSize = 0;
		for (int i = 3; i > -1; i--)
		{
		iTextSize = iTextSize << 8;
		iTextSize = iTextSize | cTextSize[i];
		}
		//������� ������ ��� �����
		char * Buffer2 = new char[iTextSize];
		//��������� �����
		for (int i = 0; i < iTextSize; i++)
		{
		unsigned char BufferTemp = 0;
		for (int j = 0; j < 8; j++)
		{
		char cImageBit = 0;
		cImageByte = Buffer[uiOffset];
		cImageBit = cImageByte & cMask;
		cImageBit = cImageBit << 7;
		BufferTemp = BufferTemp >> 1;
		BufferTemp = BufferTemp | cImageBit;
		uiOffset++;
		}
		Buffer2[i] = BufferTemp;
		}
		HANDLE hText;
		//������ � ����
		if (INVALID_HANDLE_VALUE == (hText = CreateFileW(textName,
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't create file\n";
		WriteFile(hText, Buffer2, iTextSize, &dwBytes1, NULL);
		CloseHandle(hText);
		delete[] Buffer2;
	}
	else
	{
		cout << "Nothing to retrieve.\n";
		exit(2);
	}
	CloseHandle(hFile);
	delete[] Buffer;
}
void Embed(const wchar_t * imageName, const wchar_t *	textName)
{
	HANDLE hFile;
	//HANDLE hFile1;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(imageName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open the image\n";
	DWORD dwBytes = 0, BufSize = 0, dwBytes1 = 0;
	BufSize = GetFileSize(hFile, NULL);
	char *Buffer = new char[BufSize / sizeof(char)];
	ReadFile(hFile, Buffer, BufSize, &dwBytes, NULL);
	
	//�������� ������
	HANDLE hText;
	if (INVALID_HANDLE_VALUE == (hText = CreateFileW(textName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open the text file\n";
	
	//���������� ������ � �����
		DWORD dwBytes2 = 0, BufSize2 = 0;
	BufSize2 = GetFileSize(hText, NULL);
	char *Buffer2 = new char[BufSize2 / sizeof(char)];
	ReadFile(hText, Buffer2, BufSize2, &dwBytes2, NULL);
	
	////������ ������ � �����������
	
	//��������� �������� ��������
	unsigned int uiOffset = 0;
	GetOffset(&uiOffset, Buffer);
	
	//��������� �������� ������ � ������������ �����
	char cImageByte;
	
	//��� ��������� ������
	char cTextByte = 0, cTextBit = 0;
	unsigned int uiTextOffset = 0;
	
	//�������� �������
	if (BufSize < (BufSize2 * 8) + 88)
	{
		cout << "Text do not fit into the image.\n";
		exit(3);
	}
	
	//������ ����� � �����������
	for (int i = 0; i < 5; i++)
	{
		char temp = cTag[i], tempbit = 0;
		for (char j = 0; j < 8; j++)
		{
			cImageByte = Buffer[uiOffset];
			//� ������ � ���� ���� ������
			tempbit = temp & cMask;
			temp = temp >> 1;
			if (cImageByte % 2 == 0)
				cImageByte = cImageByte | tempbit;
			else
			{
				cImageByte = cImageByte & cMask2;
				cImageByte = cImageByte | tempbit;
			}
			Buffer[uiOffset] = cImageByte;
			uiOffset++;
		}
	}
	
	//������ ������� ���������� ����� � �����������
	//32 ���� => 32 �����
	for (char j = 0; j < 32; j++)
	{
			//int temp = BufSize2;
			char tempbit = 0;
		cImageByte = Buffer[uiOffset];
		
		//� ������ � ���� ���� ������
		tempbit = BufSize2 & cMask;
		BufSize2 = BufSize2 >> 1;//��������� BufSize2
		if (cImageByte % 2 == 0)
			cImageByte = cImageByte | tempbit;
		else
		{
			cImageByte = cImageByte & cMask2;
			cImageByte = cImageByte | tempbit;
		}
		Buffer[uiOffset] = cImageByte;
		uiOffset++;
	}
	//�������������� BufSize2
	BufSize2 = GetFileSize(hText, NULL);
	//���� ������ ������ � ����������
	for (unsigned int i = 0; i < BufSize2; i++)
	{
		cTextByte = Buffer2[uiTextOffset];//���� ���� ������ ��	������ ���� �����������
			for (char j = 0; j < 8; j++)
			{
				//��������� ����� �����������
				cImageByte = Buffer[uiOffset];
				//� ������ � ���� ���� ������
				cTextBit = cTextByte & cMask;
				cTextByte = cTextByte >> 1;
				if (cImageByte % 2 == 0)
					cImageByte = cImageByte | cTextBit;
				else
				{
					cImageByte = cImageByte & cMask2;//��������	������� ��� ��� ���������� ������
						cImageByte = cImageByte | cTextBit;
				}
				Buffer[uiOffset] = cImageByte;
				uiOffset++;
			}
		uiTextOffset++;
	}
	//����� �����
	CloseHandle(hFile);
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(imageName,
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, TRUNCATE_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Writing in the file failed.\n";
	//������ � ����
	WriteFile(hFile, Buffer, BufSize, &dwBytes1, NULL);
	//�������� HANDLE
	CloseHandle(hText);
	CloseHandle(hFile);
	delete[] Buffer;
	delete[] Buffer2;
}
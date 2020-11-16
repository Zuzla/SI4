#include "SI.h"

void GetOffset(unsigned int * pOffset, const char * Buffer)
{
	*pOffset = 0; //Получение смещения
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
	
	////Проверка на наличие секретного сообщения
	//Открываем изображение
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(fileName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open file\n";
	
	//Читаем информацию о файле и 40 байт растра
	Buffer = new char[100];
	ReadFile(hFile, Buffer, 100, &dwBytes, NULL);//Берём 100 первых байт
	
	unsigned int uiOffset;
	GetOffset(&uiOffset, Buffer);
	
	//Формирование метки из изображения
	//от смещения — 40 байт
	unsigned char cTag2[5] = { 0 };
	
	//Получение значений цветов и формирование новых
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
	cTag2[4] };//для сравнение с меткой
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
	
	//Проверка метки
	for (int i = 0; i < 5; i++)
		cTag2[i] = 0;//Пока что обнуляем
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
	if (!strcmp(cTag, cTagTemp))//Если метка есть узнаём размер	текста и восстанавливаем его
	{
		//Считываем размер
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
		//Преобразование в int cTextSize
		int iTextSize = 0;
		for (int i = 3; i > -1; i--)
		{
		iTextSize = iTextSize << 8;
		iTextSize = iTextSize | cTextSize[i];
		}
		//Отводим память под текст
		char * Buffer2 = new char[iTextSize];
		//Считываем текст
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
		//Запись в файл
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
	
	//Открытие текста
	HANDLE hText;
	if (INVALID_HANDLE_VALUE == (hText = CreateFileW(textName,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open the text file\n";
	
	//Считывание текста в буфер
		DWORD dwBytes2 = 0, BufSize2 = 0;
	BufSize2 = GetFileSize(hText, NULL);
	char *Buffer2 = new char[BufSize2 / sizeof(char)];
	ReadFile(hText, Buffer2, BufSize2, &dwBytes2, NULL);
	
	////Запись текста в изображение
	
	//Получение значения смещения
	unsigned int uiOffset = 0;
	GetOffset(&uiOffset, Buffer);
	
	//Получение значений цветов и формирование новых
	char cImageByte;
	
	//Для обработки текста
	char cTextByte = 0, cTextBit = 0;
	unsigned int uiTextOffset = 0;
	
	//Проверка размера
	if (BufSize < (BufSize2 * 8) + 88)
	{
		cout << "Text do not fit into the image.\n";
		exit(3);
	}
	
	//Запись метки в изображение
	for (int i = 0; i < 5; i++)
	{
		char temp = cTag[i], tempbit = 0;
		for (char j = 0; j < 8; j++)
		{
			cImageByte = Buffer[uiOffset];
			//и запись в него бита текста
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
	
	//запись размера текстового файла в изображение
	//32 бита => 32 байта
	for (char j = 0; j < 32; j++)
	{
			//int temp = BufSize2;
			char tempbit = 0;
		cImageByte = Buffer[uiOffset];
		
		//и запись в него бита текста
		tempbit = BufSize2 & cMask;
		BufSize2 = BufSize2 >> 1;//изменение BufSize2
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
	//восстановление BufSize2
	BufSize2 = GetFileSize(hText, NULL);
	//Цикл записи текста в изображене
	for (unsigned int i = 0; i < BufSize2; i++)
	{
		cTextByte = Buffer2[uiTextOffset];//один байт текста на	восемь байт изображения
			for (char j = 0; j < 8; j++)
			{
				//Получение байта изображения
				cImageByte = Buffer[uiOffset];
				//и запись в него бита текста
				cTextBit = cTextByte & cMask;
				cTextByte = cTextByte >> 1;
				if (cImageByte % 2 == 0)
					cImageByte = cImageByte | cTextBit;
				else
				{
					cImageByte = cImageByte & cMask2;//обнуляем	младший бит для правильной записи
						cImageByte = cImageByte | cTextBit;
				}
				Buffer[uiOffset] = cImageByte;
				uiOffset++;
			}
		uiTextOffset++;
	}
	//Конец цикла
	CloseHandle(hFile);
	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(imageName,
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, TRUNCATE_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Writing in the file failed.\n";
	//Запись в файл
	WriteFile(hFile, Buffer, BufSize, &dwBytes1, NULL);
	//Закрытие HANDLE
	CloseHandle(hText);
	CloseHandle(hFile);
	delete[] Buffer;
	delete[] Buffer2;
}
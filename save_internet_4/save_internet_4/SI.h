#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

//��������� �������
void GetOffset(unsigned int *, const char *);
void Check(const wchar_t * fileName);
void Retrieve(const wchar_t * imageName, const wchar_t * textName);
void Embed(const wchar_t * imageName, const wchar_t * textName);

//���������
const char cMask = 0x01;//��� ����������� �� ������ ����
const char cMask2 = 0xFE;//��� ��������� �������� ���� ����� �����������
const char cTag[5] = "SI";


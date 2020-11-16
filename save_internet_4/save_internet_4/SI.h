#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

//Прототипы функций
void GetOffset(unsigned int *, const char *);
void Check(const wchar_t * fileName);
void Retrieve(const wchar_t * imageName, const wchar_t * textName);
void Embed(const wchar_t * imageName, const wchar_t * textName);

//Константы
const char cMask = 0x01;//Для выдёргивания из текста бита
const char cMask2 = 0xFE;//Для обнуления младшего бита байта изображения
const char cTag[5] = "SI";


// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

using namespace std;

TSet::TSet(int mp) :BitField(mp)
{
	MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) :BitField(s.BitField)
{
	MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) :BitField(bf)
{
	MaxPower = bf.GetLength();
}

TSet::operator TBitField() {
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
	if (!(*this == s))
	{
		BitField = s.BitField;
		MaxPower = s.MaxPower;
		return *this;
	}
	else return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
	return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
	return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
	TSet tField(std::max(MaxPower, s.MaxPower));
	tField.BitField = BitField | s.BitField;
	return tField;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	TSet tField(*this);
	tField.BitField.SetBit(Elem);
	return tField;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	TSet tField(*this);
	tField.BitField.ClrBit(Elem);
	return tField;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
	TSet tField(max(s.MaxPower, MaxPower));
	tField.BitField = BitField & s.BitField;
	return tField;
}

TSet TSet::operator~(void) // дополнение
{
	TSet tField(MaxPower);
	tField.BitField = ~BitField;
	return tField;
}

void TSet::InFile(std::string file_name)
{
	fstream fs;
	fs.open(file_name, fstream::in | fstream::out);
	if (!fs.is_open())
	{
		std::cout << "Error open file!" << std::endl;
	}
	else
	{
		fs << "{";
		for (int i = 0; i < BitField.GetLength(); i++)
		{
			if (BitField.GetBit(i))
			{
				if (i == BitField.GetLength() - 1)
				{
					fs << i;
				}
				else
				{
					fs << i;
					fs << ", ";
				}
			}
		}
		fs << "}";
		std::cout << "File opened and rewritten!" << std::endl;
	}
	fs.close();
}

void TSet::FromFile(std::string file_name)
{
	fstream fs;
	fs.open(file_name, fstream::in | fstream::out);
	if (!fs.is_open())
	{
		std::cout << "Error open file!" << std::endl;
	}
	else
	{
		std::string str;
		getline(fs, str);
		str = DelBadSymb(str);
		int NumOfDig = CountNumOfDig(str);
		std::string* arrStr = new std::string[NumOfDig];
		int* arrInt = new int[NumOfDig];
		StrToArrStr(str, arrStr);
		ArrStrToArrInt(arrStr, arrInt, NumOfDig);
		int Max = FindMaxElem(arrInt, NumOfDig);
		TSet res(Max + 1);
		for (int i = 0; i < NumOfDig; i++)
		{
			res.InsElem(arrInt[i]);
		}
		*this = res;
		delete[] arrStr;
		delete[] arrInt;
		std::cout << "File is open and read!" << std::endl;
	}
	fs.close();
}

void TSet::ChangeElements(int n, int new_elem)
{
	if (n == new_elem) {}			// Ecli nomer elementa = new element -> nichego ne delayem
	else {
		if ((n < 0) || (new_elem < 0 || new_elem > MaxPower))
		{
			throw std::out_of_range("Input error: invalide value of tset in ChangeElements");
		}
		else {
			int tmp = 0;
			int count = 0;
			for (int i = 0; count < n; i++)
			{
				if (BitField.GetBit(i))
				{
					tmp = i;
					count++;
				}
			}
			DelElem(tmp);
			InsElem(new_elem);
		}
	}
}

TSet TSet::getElements(int K)
{
	TSet res(MaxPower + 1);
	for (int i = 0; i < BitField.GetLength(); i++)
	{
		if (BitField.GetBit(i))
		{
			if (i % K == 0)
			{
				res.InsElem(i); // std::cout << i << " ";
			}
		}
	}
	return res;
}

// перегрузка ввода/вывода
istream& operator>>(istream& istr, TSet& s) // ввод
{
	string text;
	string tmp;
	getline(istr, tmp);
	int space = 1;
	for (int i = 0; i < tmp.length(); i++)
	{
		if (tmp[i] != '{' && tmp[i] != '}' && tmp[i] != ',')
		{
			text += tmp[i];									// Chistyui text
		}
		if (tmp[i] == ' ')
		{
			space++;
		}
	}

	string* delSpace = new string[space];
	int index = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] != ' ')
		{
			delSpace[index] += text[i];
		}
		else index++;
	}

	int* textConvertInt = new int[space];

	int maxElement = 0;
	for (int i = 0; i < space; i++)
	{
		textConvertInt[i] = StrToInt(delSpace[i]);
		if (textConvertInt[i] > maxElement)
		{
			maxElement = textConvertInt[i];
		}
	}

	TSet res(maxElement + 1);
	for (int i = 0; i < space; i++)
	{
		res.InsElem(textConvertInt[i]);
	}

	delete[] delSpace;
	delete[] textConvertInt;

	s = res;

	return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
	cout << "{";
	for (int i = 0; i < s.BitField.GetLength(); i++)
	{
		if (s.BitField.GetBit(i))
		{
			if (i == s.BitField.GetLength() - 1) 
			{
				ostr << i;
			}
			else
			{
				ostr << i;
				ostr << ", ";
			}
		}
	}
	cout << "}";

	return ostr;
}

std::string DelBadSymb(std::string str)
{
	std::string tmp = "";
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != '{' && str[i] != '}' && str[i] != ',')
		{
			tmp += str[i];
		}
	}
	return tmp;
}

int CountNumOfDig(const std::string& str)
{
	int result = 1;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
		{
			result += 1;
		}
	}
	return result;
}

void StrToArrStr(std::string str, std::string* arr)
{
	int count = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != ' ')
		{
			arr[count] += str[i];
		}
		else
			count++;
	}
}

int StrToInt(std::string str)
{
	int result = 0;
	for (int i = 0; i < str.length(); i++)
	{
		result = result * 10 + (str[i] - '0');
	}

	return result;
}

void ArrStrToArrInt(std::string* arrStr, int* arrInt, int len)
{
	for (int i = 0; i < len; i++)
	{
		arrInt[i] = StrToInt(arrStr[i]);
	}
}

int FindMaxElem(int* arr, int len)
{
	int max = 0;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	return max;
}
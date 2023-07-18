#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <Windows.h>

struct Record
{
	std::string _name;
	bool _sex = 0; // 0 - женский, 1 - мужской
	int _peopleCount = 0;
};

/*!
\brief функция для чтения файла с иностранными именами
\param[in] filename название файла
\param[out] вектор указателей на объекты Record
*/
std::vector<Record*> ReadForeignFile(const std::string& filename)
{
	std::vector<Record*> records;
	std::ifstream fin;

	fin.open(filename);

	if (!fin.is_open())
	{
		std::cout << "Файл не удалось открыть.\n";
		system("pause");
	}
	else
	{
		char sym;
		std::string word;
		int count = 0;										//> счётчик ";"
		bool isFirstLine = true;							//> переменная для организации пропуска первой строки
		
		Record* rec = new Record;
		while (fin.get(sym))
		{
			if (isFirstLine)
				while (fin.get(sym).get() != '\n');
			isFirstLine = false;
			if (sym == '"')									//> условие для обработки записей, которые имеют кавычки
			{
				char prevSym ='a';
				while (fin.get(sym))
				{
					if (sym == ';' && prevSym == '"')
						break;
					prevSym = sym;
				}
				
			}
			if (count == 6 && sym != '\n')					//> считывание до конца строки
				continue;
			else if (count == 6 && sym == '\n')				//> переход на новую строку в файле
			{
				count = 0;				
				continue;
			}
			if (sym == ';')
			{
				count++;
				if (count == 4)
				{
					if (word == "Female")
						rec->_sex = 0;
					else if (word == "Male")
						rec->_sex = 1;
					word = "";
				}
				else if(count == 6)
				{
					rec->_peopleCount = std::stoi(word);
					//std::cout << rec->_name << " " << rec->_sex << " " << rec->_peopleCount << std::endl;
					records.push_back(rec);
					rec = new Record;
				}
				else if (count == 3)
				{
					word = "";
				}
				continue;

			}
			else if (count == 1)
			{
				rec->_name += sym;
				continue;
			}
			else if (count == 3)
			{
				word += sym;
				continue;
			}
			else if (count == 5)
			{
				if (sym >= '0' && sym <= '9')
					word += sym;

				continue;
			}

		}
		if (records.back() != nullptr && records.back() != rec) //> защита от утечки памяти. Если последняя строчка не будет записана в вектор records
			delete rec;
	}
	

	fin.close();
	return std::move(records);
}

/*!
\brief функция для чтения файла с иностранными именами
\param[in] filename название файла
\param[out] вектор указателей на объекты Record
*/
std::vector<Record*> ReadRusFile(const std::string& filename)
{
	std::vector<Record*> records;
	std::ifstream fin;

	fin.open(filename);

	if (!fin.is_open())
	{
		std::cout << "Файл не удалось открыть.\n";
		system("pause");
	}
	else
	{
		/*
		std::string line;
		bool isFirst = true;
		int numLine = 19903;
		while (std::getline(fin, line)) {
			//std::cout << numLine++ << "\t";
			if (isFirst)
			{
				isFirst = false;
				continue;
			}
			Record* rec = new Record;
			std::string number;
			int count = 0;
			for (size_t i = 0; i < line.size(); i++)
			{
				if (line[i] == ';')
				{
					count++;
					continue;
				}

				if (count == 1)
				{
					if (line[i] != ';')
						rec->_name += line[i];
				}
				if (count == 2)
				{
					if (line[i] == 'Ж')
						rec->_sex = 0;
					else if (line[i] == 'M')
						rec->_sex = 1;
				}
				if (count == 3)
				{
					if (line[i] >= '0' && line[i] <= '9')
						number += line[i];
				}
				if (count == 4)
				{

					rec->_peopleCount = std::stoi(number);
					break;
				}



			}
			//std::cout << rec->_name << "\t" << rec->_sex << "\t" << rec->_peopleCount << std::endl;
			records.push_back(rec);
			*/



		char sym;
		std::string word;
		int count = 0;										//> счётчик ";"
		bool isFirstLine = true;							//> переменная для организации пропуска первой строки
		Record* rec = new Record;
		while (fin.get(sym))
		{
			if (isFirstLine)
				while (fin.get(sym).get() != '\n');
			isFirstLine = false;
			if (count == 4 && sym != '\n')
				continue;
			else if (count == 4 && sym == '\n')				//> переход на новую стрку в файле
			{
				rec->_peopleCount = std::stoi(word);
				records.push_back(rec);
				count = 0;
				rec = new Record;
				continue;
			}
			if (sym == ';')
			{
				count++;
				continue;

			}
			else if (count == 1)
			{
				rec->_name += sym;
				continue;
			}
			else if (count == 2)
			{
				
				word += sym;
				
				if (word == "Ж")
					rec->_sex = 0;
				else if (word == "М")
					rec->_sex = 1;
				word = "";
				continue;
			}
			else if (count == 3)
			{
				if (sym >= '0' && sym <= '9')
					word += sym;
			}

		}
		if (records.back() != nullptr && records.back() != rec) //> защита от утечки памяти. Если последняя строчка не будет записана в вектор records
			delete rec;
	}


	fin.close();
	return std::move(records);
}

/*!
функция подсчитывающая количество людей в векторе records
\param[in] records вектор содержащий указатели на структуры Record
\param[out] количество людей в records
*/
int NumbersOfPeople(const std::vector<Record*>& records)
{
	int result = 0;
	for (const auto& el : records)
	{
		result += el->_peopleCount;
	}
	return result;
}

/*!
функция возвращающая запись с максимальным количеством носителей имён
\param[in] records вектор содержащий указатели на структуры Record
\param[out] структуру Record
*/
Record MaxNumberName(const std::vector<Record*>& records)
{
	int max = 0;
	int max_count = 0;
	Record record;
	for (size_t i = 1; i < records.size(); i++)
	{
		if (max < records.at(i)->_peopleCount)
		{
			max = records.at(i)->_peopleCount;
			record = *(records.at(i));
			max_count = i;
		}
	}
	return record;
}

Record MinNumbersName(const std::vector<Record*>& records)
{
	int min = records.at(0)->_peopleCount;
	int count = 1;
	Record record;
	for (size_t i = 1; i < records.size(); i++)
	{
		if (min > records.at(i)->_peopleCount)
		{
			min = records.at(i)->_peopleCount;
			record = *(records.at(i));
			count = i;
		}
	}
	return record;
}

// сделать удаление имен у которых количество людей равно 0
void DeleteNamesWithZeroNameBearers(std::vector<Record*>& records)					// удаление элементов вектора в которых число носителей (_peopleCount) имени равно 0
{
	for (size_t i = 0; i < records.size(); )
	{
		if (records.at(i)->_peopleCount == 0)
		{
			delete records.at(i);
			records.erase(records.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

size_t NumberOfNamesWithZeroNameBearer(const std::vector<Record*>& records)			// Количество имен у которых нет носителя
{
	size_t number = 0;
	for (size_t i = 0; i < records.size(); i++)
	{
		if (records.at(i)->_peopleCount == 0)
		{
			number++;
		}
	}
	return number;
}

// найти имя у которого минимальное количество людей

void FreeingUpMemory(std::vector<Record*>& records)
{
	for (size_t i = 1; i < records.size(); i++)
	{
		delete records.at(i);
	}
}

void RecordsAnalize(std::vector<Record*>& records, std::string filename)
{
	

	std::cout << "Total number of people in the database = " << NumbersOfPeople(records) << std::endl;
	Record max_Record = MaxNumberName(records);					//> элемент вектора с наибольшим полем PeopleCount_

	std::cout << "Element with maximal PepleCount = " << max_Record._peopleCount << " is " << max_Record._name << std::endl;
	std::cout << "Total number of people int the database with zero name bearer -> " << NumberOfNamesWithZeroNameBearer(records) << std::endl;
	DeleteNamesWithZeroNameBearers(records);
	std::cout << "Total number of people int the database with zero name bearer -> " << NumberOfNamesWithZeroNameBearer(records) << std::endl;

	Record min_Record = MinNumbersName(records);

	std::cout << "Element with minimal PepleCount = " << min_Record._peopleCount << " is " << min_Record._name << std::endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	std::vector<Record*> records;
	std::string filename = "russian_names_ANSI.csv";
	records = ReadRusFile(filename);
	RecordsAnalize(records, filename);

	FreeingUpMemory(records);

	records.clear();
	
	filename = "foreign_names_ANSI.csv";

	records = ReadForeignFile(filename);
	
	RecordsAnalize(records, filename);

	FreeingUpMemory(records);

	records.clear();
	return 0;
}


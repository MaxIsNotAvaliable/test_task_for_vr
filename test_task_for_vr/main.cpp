#include "sorts.h"
#include "autotests.h"

/*
Цель:
Создать простое консольное приложение C++, которое сортирует 
один и тот же массив объектов с помощью нескольких потоков и 
соответствует следующим требованиям:
•	В разных потоках разные алгоритмы сортировки
•	После каждой сортировки выводится результат в консоль
•	Реализованы автотесты с разными тестовыми данными, демонстрирующими 
    работу приложения при различных граничных условиях задачи
*/

#pragma region ARRAY_UTILS

std::vector<int> GenerateVector(size_t size)
{
	std::vector<int> vec(size);
	for (size_t i = 0; i < size; i++)
	{
		vec[i] = rand();
	}
	return vec;
}

void PrintArray(const std::vector<int>& vec, bool shortPrint)
{
	bool useSkip = false;
	int skipId = 0;
	if (vec.size() > 20 && shortPrint)
	{
		useSkip = true;
		skipId = vec.size() - 10;
	}
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (useSkip && i > 10 && i < skipId)
		{
			printf("...\n");
			i = skipId;
			continue;
		}
		printf("[%i]\t%i\n", (int)i, vec[i]);
	}
}

#pragma endregion // ARRAY_UTILS

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	std::vector<int> vec = GenerateVector(100);

	auto start = std::chrono::high_resolution_clock::now();

	// Сортировка массива
	sorts::MultiThreadSort(vec);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	// Вывод массива
	PrintArray(vec, false);
	printf("%lld msec\n", duration.count());

	// Запуск тестов
	autotests::RunTests();

	return 0;
}
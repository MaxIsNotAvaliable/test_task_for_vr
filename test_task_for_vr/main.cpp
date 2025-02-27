#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>

/*
Цель:
Создать простое консольное приложение C++, которое сортирует один и тот же массив объектов с помощью нескольких потоков и соответствует следующим требованиям:
•	В разных потоках разные алгоритмы сортировки
•	После каждой сортировки выводится результат в консоль
•	Реализованы автотесты с разными тестовыми данными, демонстрирующими работу приложения при различных граничных условиях задачи
*/

#pragma region SORT_ALGORITHMS

void BubbleSort(std::vector<int>& vec, size_t begin, size_t end)
{
	if (begin >= vec.size() || end > vec.size() || begin >= end) 
	{
		return; 
	}

	bool swapped;
	for (size_t i = begin; i < end - 1; ++i)
	{
		swapped = false;
		for (size_t j = begin; j < end - i - 1; ++j)
		{
			if (vec[j] > vec[j + 1]) {
				std::swap(vec[j], vec[j + 1]);
				swapped = true;
			}
		}
		if (!swapped) {
			break; 
		}
	}
}

void CountingSort(std::vector<int>& vec, size_t begin, size_t end)
{
	if (begin >= vec.size() || end > vec.size() || begin >= end) 
	{
		return;
	}

	int max_elem = *std::max_element(vec.begin() + begin, vec.begin() + end);
	int min_elem = *std::min_element(vec.begin() + begin, vec.begin() + end);

	size_t range_size = end - begin;

	std::vector<size_t> count_vec(max_elem - min_elem + 1, 0);

	for (size_t i = begin; i < end; ++i) 
	{
		count_vec[vec[i] - min_elem]++;
	}

	for (size_t i = 1; i < count_vec.size(); ++i) 
	{
		count_vec[i] += count_vec[i - 1];
	}

	std::vector<int> result(range_size);

	for (int i = range_size - 1; i >= 0; --i) 
	{
		result[count_vec[vec[begin + i] - min_elem] - 1] = vec[begin + i];
		count_vec[vec[begin + i] - min_elem]--;
	}

	for (size_t i = 0; i < range_size; ++i) 
	{
		vec[begin + i] = result[i];
	}
}

void heapify(std::vector<int>& vec, int32_t n, int32_t i, size_t begin)
{
	int32_t largest = i;
	int32_t left = 2 * i + 1;
	int32_t right = 2 * i + 2;

	if (left < n && vec[begin + left] > vec[begin + largest])
	{
		largest = left;
	}

	if (right < n && vec[begin + right] > vec[begin + largest])
	{
		largest = right;
	}

	if (largest != i)
	{
		std::swap(vec[begin + i], vec[begin + largest]);
		heapify(vec, n, largest, begin);
	}
};

void HeapSort(std::vector<int>& vec, size_t begin, size_t end)
{
	if (begin >= vec.size() || end > vec.size() || begin >= end) 
	{
		return;
	}

	int32_t n = end - begin;

	for (int32_t i = n / 2 - 1; i >= 0; --i)
	{
		heapify(vec, n, i, begin);
	}

	for (int32_t i = n - 1; i > 0; --i)
	{
		std::swap(vec[begin], vec[begin + i]);
		heapify(vec, i, 0, begin);
	}
}

void MergeRegions(std::vector<int>& vec, size_t left, size_t mid, size_t right)
{
	std::vector<int> temp(right - left);
	size_t i = left, j = mid, k = 0;

	while (i < mid && j < right)
	{
		if (vec[i] <= vec[j]) {
			temp[k++] = vec[i++];
		}
		else {
			temp[k++] = vec[j++];
		}
	}

	while (i < mid)
	{
		temp[k++] = vec[i++];
	}

	while (j < right)
	{
		temp[k++] = vec[j++];
	}

	for (size_t p = 0; p < temp.size(); ++p)
	{
		vec[left + p] = temp[p];
	}
}

void MultiThreadSort(std::vector<int>& vec)
{
	if (vec.size() <= 1)
	{
		return;
	}

	std::vector<std::function<void(std::vector<int>&, size_t, size_t)>> sortFuncList = {
		BubbleSort,
		CountingSort,
		HeapSort
	};

	struct Region_t
	{
		size_t begin, end;
		Region_t(size_t begin, size_t end) : begin(begin), end(end) {}
	};

	size_t chunkSize = vec.size() / sortFuncList.size();
	std::vector<Region_t> regions;
	for (size_t i = 0; i < sortFuncList.size() - 1; i++)
	{
		regions.emplace_back(i * chunkSize, (i + 1) * chunkSize);
	}
	regions.emplace_back((sortFuncList.size() - 1) * chunkSize, vec.size());

	if (regions.size() != sortFuncList.size())
	{
		throw std::length_error("Sizes are not match!");
	}

	std::vector<std::thread> threads;
	for (size_t i = 0; i < regions.size(); i++)
	{
		threads.emplace_back(sortFuncList[i], std::ref(vec), regions[i].begin, regions[i].end);
	}

	for (size_t i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

	for (size_t i = 0; i < regions.size() - 1; i++)
	{
		MergeRegions(vec, 0, regions[i].end, regions[i + 1].end);
	}
}

#pragma endregion // SORT_ALGORITHMS

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

void PrintArray(const std::vector<int>& vec)
{
	bool useSkip = false;
	int skipId = 0;
	if (vec.size() > 20)
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

#pragma region AUTOTESTS

bool IsSorted(const std::vector<int>& vec) {
	for (size_t i = 1; i < vec.size(); ++i) {
		if (vec[i - 1] > vec[i]) return false;
	}
	return true;
}

void RunTests() 
{
	std::vector<std::vector<int>> testCases = {
		{},
		{1},
		{9, 8, 7, 6, 5, 4, 3, 2, 1},
		{1, 2, 3, 4, 5},
		{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5} 
	};

	std::vector<std::string> testNames = {
		"Пустой массив",
		"Один элемент",
		"Обратный порядок",
		"Отсортированный массив",
		"Случайные числа"
	};

	bool failed = false;

	for (size_t i = 0; i < testCases.size(); i++)
	{
		std::vector<int> vec = testCases[i];
		MultiThreadSort(vec);
		if (!IsSorted(vec))
		{
			failed |= true;
			printf("Тест провален - \"%s\"!\n", testNames[i].c_str());
		}
	}

	if (!failed)
	{
		printf("Тесты пройдены успешно!\n");
	}
}

#pragma endregion // AUTOTESTS

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	std::vector<int> vec = GenerateVector(100);

	auto start = std::chrono::high_resolution_clock::now();

	MultiThreadSort(vec);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	PrintArray(vec);
	printf("%lld msec\n", duration.count());

	RunTests();

	return 0;
}
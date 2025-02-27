#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>

/*
÷ель:
—оздать простое консольное приложение C++, которое сортирует один и тот же массив объектов с помощью нескольких потоков и соответствует следующим требовани€м:
Х	¬ разных потоках разные алгоритмы сортировки
Х	ѕосле каждой сортировки выводитс€ результат в консоль
Х	–еализованы автотесты с разными тестовыми данными, демонстрирующими работу приложени€ при различных граничных услови€х задачи
*/

#pragma region SORT_ALGORITHMS
template<typename T>
void BubbleSort(std::vector<T>& vec)
{
	bool swapped;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		swapped = false;
		for (size_t j = 0; j < vec.size() - i - 1; ++j)
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

template <typename T>
void CountingSort(std::vector<T>& vec) {
	size_t size = vec.size();
	T max_elem = *max_element(vec.begin(), vec.end());

	std::vector<size_t> count_vec(max_elem + 1, 0);

	for (size_t i = 0; i < size; i++)
	{
		count_vec[vec[i]]++;
	}

	for (size_t i = 1; i < count_vec.size(); i++)
	{
		count_vec[i] += count_vec[i - 1];
	}

	std::vector<T> result(size);
	for (int i = size - 1; i >= 0; i--)
	{
		result[count_vec[vec[i]] - 1] = vec[i];
		count_vec[vec[i]]--;
	}
	vec = result;
}

template <typename T>
void heapify(std::vector<T>& vec, int32_t n, int32_t i) {
	int32_t largest = i;
	int32_t left = 2 * i + 1;
	int32_t right = 2 * i + 2;

	if (left < n && vec[left] > vec[largest])
	{
		largest = left;
	}

	if (right < n && vec[right] > vec[largest])
	{
		largest = right;
	}

	if (largest != i)
	{
		std::swap(vec[i], vec[largest]);

		heapify(vec, n, largest);
	}
};

template <typename T>
void HeapSort(std::vector<T>& vec)
{
	int32_t n = vec.size();
	for (int32_t i = n / 2 - 1; i >= 0; --i)
	{
		heapify(vec, n, i);
	}
	for (int32_t i = n - 1; i > 0; --i)
	{
		std::swap(vec[0], vec[i]);
		heapify(vec, i, 0);
	}
}

#pragma endregion // SORT_ALGORITHMS

std::vector<int> GenerateVector(size_t size)
{
	std::vector<int> vec(size);
	for (size_t i = 0; i < size; i++)
	{
		vec[i] = rand() % size;
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

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	std::vector<int> arr = GenerateVector(1000);

	std::vector<int> bubleArr = std::vector<int>(arr);
	std::vector<int> countingArr = std::vector<int>(arr);
	std::vector<int> heapArr = std::vector<int>(arr);
	
	auto start = std::chrono::high_resolution_clock::now();

	std::thread bubleTh = std::thread(BubbleSort<int>, std::ref(bubleArr));
	std::thread countingTh = std::thread(CountingSort<int>, std::ref(countingArr));
	std::thread heapTh = std::thread(HeapSort<int>, std::ref(heapArr));

	bubleTh.join();
	countingTh.join();
	heapTh.join();

	//BubbleSort(bubleArr);
	//CountingSort(countingArr);
	//HeapSort(heapArr);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	PrintArray(bubleArr);
	printf("%lld msec\n", duration.count());

	return 0;
}
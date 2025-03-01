#include "sorts.h"
#include "autotests.h"

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

	// Ñîðòèðîâêà ìàññèâà
	sorts::MultiThreadSort(vec);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	// Âûâîä ìàññèâà
	PrintArray(vec, false);
	printf("%lld msec\n", duration.count());

	// Çàïóñê òåñòîâ
	autotests::RunTests();

	return 0;
}
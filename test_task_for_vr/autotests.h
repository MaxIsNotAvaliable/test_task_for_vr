#pragma once

#ifndef AUTOTESTS_H
#define AUTOTESTS_H

#include "sorts.h"

namespace autotests
{
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
			sorts::MultiThreadSort(vec);
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
}

#endif
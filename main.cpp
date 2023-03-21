﻿#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <array>
#include <algorithm>
#include <numeric>
#include <future>
#include <random>
#include "SecondaryFunction.h"

/*
Необходимо реализовать алгоритм сортировки выбором.
Сортировка выбором состоит в том, что в неупорядоченном списке
находится наименьший элемент. Выбранный в исходном списке минимальный
элемент записывается на i-е место исходного списка (i=1,2,…,п), а
элемент с i-го места – на место выбранного. При этом очевидно, что уже
упорядоченные элементы (а они будут расположены начиная с первого места)
исключаются из дальнейшей сортировки, поэтому длина списка, участвующего
в каждом последующем просмотре должна быть на один элемент меньше предыдущего.

Поиск минимального элемента реализовать в виде асинхронной задачи.
Результат должен возвращаться в основной поток через связку std::promise-std::future.
*/



int main(int argc, char** argv)
{
	printHeader(L"Сортировка выбором");

	
	std::wcout << "\n";

	return 0;
}
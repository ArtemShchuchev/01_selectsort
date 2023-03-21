#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
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

// заполняет массив случайными значениями от 0 до 100
void fillArr(std::vector<int>& arr)
{
	std::srand(std::time(nullptr));
	for (auto& data : arr) data = std::rand() % 100;
}
// перегружаю оператор << для wcout (чтобы выводить std::vector<int>)
std::wostream& operator<< (std::wostream& out, const std::vector<int>& vect)
{
	for (const auto& val : vect) out << val << " ";
	return out;
}
// сортировка выбором
// поиск минимального элеманта
void findminidx(const std::vector<int>& arr, size_t index, std::promise<size_t> prom)
{
	size_t end = arr.size();
	size_t smallestIndex{ index };	// индекс хранящий наименьшее значение
	// Ищем индекс содержащий меньший элемент в оставшейся части массива
	for ( ; index < end; ++index)
	{
		if (arr[index] < arr[smallestIndex]) smallestIndex = index;
	}
	prom.set_value(smallestIndex);
}
// основная программа сортировки
void selectionSort(std::vector<int>& arr)
{
	// Пройдемся по каждому элементу массива (кроме последнего,
	// который уже будет отсортирован к тому моменту, когда мы туда доберемся)
	size_t length = arr.size() - 1;
	for (size_t startIndex{ 0 }; startIndex < length; ++startIndex)
	{
		// не совсем понятно в чем тут заключается ассинхронность?
		// Но сортировка работает...
		std::promise<size_t> prm;
		std::future<size_t> smallIdx = prm.get_future();
		auto res = std::async(findminidx, arr, startIndex, std::move(prm));
		smallIdx.wait(); // эта строка не очень нужна, т.к. он все равно ждет в .get()?

		// smallIdx.get() указывает на самый маленький элемент в оставшемся массиве
		// меняем местами наш начальный элемент самым маленьким элементом
		// (это сортирует его в нужное место)
		std::swap(arr[startIndex], arr[smallIdx.get()]);
	}
}

int main(int argc, char** argv)
{
	printHeader(L"Сортировка выбором");

	const size_t arrsize = 20;
	std::vector<int> arr(arrsize);

	fillArr(arr);
	std::wcout << L"Массив до сортировки: " << arr << "\n";
	selectionSort(arr);
	std::wcout << L"Массив после сортировки: " << arr << "\n";
	
	std::wcout << "\n";

	return 0;
}

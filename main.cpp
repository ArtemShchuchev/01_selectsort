#include <future>
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
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
using vecIt_t = std::vector<int>::iterator;
void findminidx(const std::vector<int>& arr, vecIt_t itr, std::promise<vecIt_t> prom)
{
	auto smallestIt{ itr };	// итератор хранящий наименьшее значение
	auto end = arr.end();
	// Ищем итератор содержащий меньший элемент в оставшейся части массива
	for (; itr < end; ++itr) if (*itr < *smallestIt) smallestIt = itr;

	prom.set_value(smallestIt);
}
// основная программа сортировки
void selectionSort(std::vector<int>& arr)
{
	// Пройдемся по каждому элементу массива (кроме последнего,
	// который уже будет отсортирован к тому моменту, когда мы туда доберемся)
	auto last = arr.end() - 1;
	for (auto it = arr.begin(); it < last; ++it)
	{
		// не совсем понятно в чем тут заключается ассинхронность?
		// Но сортировка работает...
		std::promise<vecIt_t> prm;
		std::future<vecIt_t> smallIt = prm.get_future();
		auto res = std::async(findminidx, std::ref(arr), it, std::move(prm));
		smallIt.wait(); // эта строка не очень нужна, т.к. он все равно ждет в .get()?

		// smallIt.get() указывает на самый маленький элемент в оставшемся массиве
		// меняем местами наш начальный элемент самым маленьким элементом
		// (это сортирует его в нужное место)
		std::swap(*it, *smallIt.get());
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

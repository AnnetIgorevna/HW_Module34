#include <iostream>
#include <string>

template <typename T>
concept ComplexConcept =
!std::has_virtual_destructor<T>::value && // проверка отсутстви€ у типа виртуального деструктора
	requires(T v) {
		{ v.hash() } -> std::convertible_to<long>; // наличие у типа метода, который возвращает тип, конвертируемый в long
		{ v.toString() } -> std::same_as<std::string>; // наличие у типа метода, который возвращает std::string
};

void PrintType(ComplexConcept auto& t) {
	std::cout << "Type is correct.\n";
	std::cout << "toString method - Hash is " << t.toString() << "\n";
	std::cout << "hash method - Hash is " << t.hash() << "\n";
}

// класс, удовлетвор€ющий услови€м концепта
class CorrectTestClass
{
public:
	CorrectTestClass(const std::string& hash) : _hash(hash) {}; // конструктор тестового класса
	~CorrectTestClass() {}; // деструктор Ќ≈виртуальный
	size_t hash() { return std::hash<std::string> {} (_hash); } // метод, который возвращает тип, конвертируемый в long
	std::string toString() const { return _hash; } // метод, возвращающий std::string

private:
	std::string _hash;
};

// класс, Ќ≈удовлетвор€ющий услови€м концепта
class IncorrectTestClass
{
public:
	IncorrectTestClass(const std::string& hash) : _hash(hash) {}; // конструктор тестового класса
	virtual ~IncorrectTestClass() {}; // деструктор виртуальный
	size_t hash() { return std::hash<std::string> {} (_hash); } // метод, который возвращает тип, конвертируемый в long
	int toInt(int a, int b) {
		return a + b;
	}
	// нет метода, возвращающего std::string
private:
	std::string _hash;
};


int main()
{
	CorrectTestClass a{ "Hello" };
	IncorrectTestClass b{ "password1234" };

	PrintType(a); // все скомпилировалось
	PrintType(b); // ошибка компил€ции

	return 0;
}
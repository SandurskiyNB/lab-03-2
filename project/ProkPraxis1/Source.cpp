#include<iostream>
#include<vector>
#include<string>
using namespace std;

// Функция заполнения массива вводимыми числами
vector<double> input_numbers(int numbers_count) {
	vector<double> numbers(numbers_count);
	for (int i = 0; i < numbers_count; i++) {
		cin >> numbers[i];
	}
	return numbers;
}

// Функция поиска минимума и максимума среди чисел
void find_minmax(vector<double> numbers, const int numbers_count, double& min, double& max) {
	min = numbers[0];
	max = numbers[0];
	for (int i = 1; i < numbers_count; i++)
		if (numbers[i] < min)
			min = numbers[i];
		else
			if (numbers[i] > max)
				max = numbers[i];
}

// Функция вычисления частот корзин
vector<int> make_histogram(vector<double> numbers, const int numbers_count, const int sections_count) {

	// Поиск максимального и минимального чисел
	double min_value, max_value;
	find_minmax(numbers, numbers_count, min_value, max_value);

	// Вычисление промежуточного интервала между корзинами и подготовка массива для вычисления кол-ва чисел в корзинах
	double separate_interval = (max_value - min_value) / sections_count;
	vector<int> freak_sections(sections_count);
	for (int i = 0; i < sections_count; i++)
		freak_sections[i] = 0;

	// Подсчёт кол-ва чисел в корзинах
	for (int i = 0; i < numbers_count; i++) { // проход по всем числам
		double left_bord = min_value; // задание начальной левой границы интервала
		double right_bord = min_value + separate_interval; // задание начальной правой границы интервала
		bool odds_max_value = true; // переменная, фиксирующая, может ли число быть максимальным на данном этапе выполнения
		for (int j = 0; j < sections_count; j++) { // проход по корзинам
			if (numbers[i] >= left_bord && numbers[i] < right_bord) { // если число попадает в корзину
				freak_sections[j]++; // повышаем частоту корзины
				odds_max_value = false; // число точно не максимальное, раз оно попало в корзину данной проверкой
				break; // выход из цикла прохода по корзинам
			}
			left_bord += separate_interval; // повышение левой границы интервала
			right_bord += separate_interval; // повышение правой границы интервала
		}
		if (odds_max_value) // если число не попало ни в одну корзину ранее, следовательно, оно является максимальным
			freak_sections[sections_count - 1]++; // повышение частоты крайней корзины
	}
	return freak_sections; // возврат массива частот корзин
}

// Функция отображения гистрограммы
void show_histogram_text(vector<int> freak_sections, const int sections_count) {

	const double FREAK_LIMIT = 70;
	const int MAX_MINUS_COUNT = 78;
	const int SYMB_COUNT = 5;
	const int END_INDENT_COUNT = 3;

	// Поиск масильманой частоты
	int max_freak = freak_sections[0];
	for (int i = 1; i < sections_count; i++)
		if (freak_sections[i] > max_freak)
			max_freak = freak_sections[i];

	// Выяснение, нужно ли масштабирование
	bool scaling;
	if (max_freak > FREAK_LIMIT)
		scaling = true;
	else
		scaling = false;

	// Печать верхней линии рамки
	cout << "\n+";
	int minus_count;
	if (scaling)
		minus_count = MAX_MINUS_COUNT;
	else
		minus_count = max_freak + SYMB_COUNT + END_INDENT_COUNT;
	for (int i = 0; i < minus_count; i++)
		cout << "-";
	cout << "+\n";

	// Печать результатов
	for (int i = 0; i < sections_count; i++) {

		cout << "|";

		// Печать частот корзины
		if (freak_sections[i] < 10)
			cout << "   ";
		else
			if (freak_sections[i] < 100)
				cout << "  ";
			else
				cout << " ";
		cout << freak_sections[i] << "|";

		// Печать звёздочек
		int stars_count;
		if (scaling)
			stars_count = freak_sections[i] / (max_freak / FREAK_LIMIT);
		else
			stars_count = freak_sections[i];
		for (int j = 0; j < stars_count; j++)
			cout << "*";

		// Печать пробелов после звёздочек
		int spaces_count = minus_count - stars_count - SYMB_COUNT;
		for (int i = 0; i < spaces_count; i++)
			cout << " ";

		cout << "|\n";
	}

	// Печать нижней линии рамки
	cout << "+";
	for (int i = 0; i < minus_count; i++)
		cout << "-";
	cout << "+\n";
}

void svg_begin(double width, double height) {
	cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
	cout << "<svg ";
	cout << "width='" << width << "' ";
	cout << "height='" << height << "' ";
	cout << "viewBox='0 0 " << width << " " << height << "' ";
	cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
	cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text) {
	cout << "<text x='" << left << "' ";
	cout << "y='" << baseline << "'";
	cout << ">" << text << "</text>\n";
}

void svg_rect(double x, double y, double width, double height, string stroke, string fill) {
	cout << "<rect ";
	cout << "x='" << x << "' ";
	cout << "y='" << y << "' ";
	cout << "width='" << width << "' ";
	cout << "height='" << height << "' ";
	cout << "stroke='" << stroke << "' ";
	cout << "fill=' " << fill << "'/>\n";
}

void show_histogram_svg(const vector<int>& freak_sections, const int section_count) {

	const auto IMAGE_WIDTH = 400;
	const auto IMAGE_HEIGHT = 300;
	const auto TEXT_LEFT = 20;
	const auto TEXT_BASELINE = 20;
	const auto TEXT_WIDTH = 50;
	const auto BIN_HEIGHT = 30;
	const auto BLOCK_WIDTH = 10;
	const double FREAK_LIMIT = 70;

	// Поиск максимальной частоты среди частот всех корзин
	int max_freak = freak_sections[0];
	for (int i = 1; i < section_count; i++)
		if (freak_sections[i] > max_freak)
			max_freak = freak_sections[i];
	
	// Проверка, требуется ли выполнять масштабирование
	bool scaling;
	if (max_freak > FREAK_LIMIT)
		scaling = true;
	else
		scaling = false;

	double shift = 0;
	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
	for (int i = 0; i < section_count; i++) {
		double section_width;
		if (scaling)
			section_width = BLOCK_WIDTH * freak_sections[i] / (max_freak / FREAK_LIMIT);
		else
			section_width = BLOCK_WIDTH * freak_sections[i];
		svg_text(TEXT_LEFT, TEXT_BASELINE + shift, to_string(freak_sections[i]));
		svg_rect(TEXT_WIDTH, shift, section_width, BIN_HEIGHT, "blue", "green");
		shift += BIN_HEIGHT;
	}
	svg_end();
}


int main() {

	setlocale(LC_ALL, "Russian");

	// Ввод количества чисел
	int numbers_count;
	//cout << "Введите кол-во чисел: ";
	cin >> numbers_count;

	// Заполнение массива числами
	//cout << "Введите числа:\n";
	const auto numbers = input_numbers(numbers_count);

	// Ввод количества корзин и создание массива частот на каждом интервале
	double sections_count;
	//cout << "Введите кол-во корзин: ";
	cin >> sections_count;
		
	vector<int> freak_sections = make_histogram(numbers, numbers_count, sections_count);

	show_histogram_svg(freak_sections, sections_count);

}
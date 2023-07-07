#include<iostream>
#include<vector>
#include<string>
#include "histogram.h"
using namespace std;

// ������� ���������� ������� ��������� �������
vector<double> input_numbers(int numbers_count) {
	vector<double> numbers(numbers_count);
	for (int i = 0; i < numbers_count; i++) {
		cin >> numbers[i];
	}
	return numbers;
}


// ������� ���������� ������ ������
vector<int> make_histogram(vector<double> numbers, const int numbers_count, const int sections_count) {

	// ����� ������������� � ������������ �����
	double min_value, max_value;
	find_minmax(numbers, numbers_count, min_value, max_value);

	// ���������� �������������� ��������� ����� ��������� � ���������� ������� ��� ���������� ���-�� ����� � ��������
	double separate_interval = (max_value - min_value) / sections_count;
	vector<int> freak_sections(sections_count);
	for (int i = 0; i < sections_count; i++)
		freak_sections[i] = 0;

	// ������� ���-�� ����� � ��������
	for (int i = 0; i < numbers_count; i++) { // ������ �� ���� ������
		double left_bord = min_value; // ������� ��������� ����� ������� ���������
		double right_bord = min_value + separate_interval; // ������� ��������� ������ ������� ���������
		bool odds_max_value = true; // ����������, �����������, ����� �� ����� ���� ������������ �� ������ ����� ����������
		for (int j = 0; j < sections_count; j++) { // ������ �� ��������
			if (numbers[i] >= left_bord && numbers[i] < right_bord) { // ���� ����� �������� � �������
				freak_sections[j]++; // �������� ������� �������
				odds_max_value = false; // ����� ����� �� ������������, ��� ��� ������ � ������� ������ ���������
				break; // ����� �� ����� ������� �� ��������
			}
			left_bord += separate_interval; // ��������� ����� ������� ���������
			right_bord += separate_interval; // ��������� ������ ������� ���������
		}
		if (odds_max_value) // ���� ����� �� ������ �� � ���� ������� �����, �������������, ��� �������� ������������
			freak_sections[sections_count - 1]++; // ��������� ������� ������� �������
	}
	return freak_sections; // ������� ������� ������ ������
}

// ������� ����������� ������������
void show_histogram_text(vector<int> freak_sections, const int sections_count) {

	const double FREAK_LIMIT = 70;
	const int MAX_MINUS_COUNT = 78;
	const int SYMB_COUNT = 5;
	const int END_INDENT_COUNT = 3;

	// ����� ����������� �������
	int max_freak = freak_sections[0];
	for (int i = 1; i < sections_count; i++)
		if (freak_sections[i] > max_freak)
			max_freak = freak_sections[i];

	// ���������, ����� �� ���������������
	bool scaling;
	if (max_freak > FREAK_LIMIT)
		scaling = true;
	else
		scaling = false;

	// ������ ������� ����� �����
	cout << "\n+";
	int minus_count;
	if (scaling)
		minus_count = MAX_MINUS_COUNT;
	else
		minus_count = max_freak + SYMB_COUNT + END_INDENT_COUNT;
	for (int i = 0; i < minus_count; i++)
		cout << "-";
	cout << "+\n";

	// ������ �����������
	for (int i = 0; i < sections_count; i++) {

		cout << "|";

		// ������ ������ �������
		if (freak_sections[i] < 10)
			cout << "   ";
		else
			if (freak_sections[i] < 100)
				cout << "  ";
			else
				cout << " ";
		cout << freak_sections[i] << "|";

		// ������ ��������
		int stars_count;
		if (scaling)
			stars_count = freak_sections[i] / (max_freak / FREAK_LIMIT);
		else
			stars_count = freak_sections[i];
		for (int j = 0; j < stars_count; j++)
			cout << "*";

		// ������ �������� ����� ��������
		int spaces_count = minus_count - stars_count - SYMB_COUNT;
		for (int i = 0; i < spaces_count; i++)
			cout << " ";

		cout << "|\n";
	}

	// ������ ������ ����� �����
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
	cout << "fill='" << fill << "'/>\n";
}

void show_histogram_svg(const vector<int>& freak_sections, const int section_count) {

	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 300;
	const int TEXT_LEFT = 20;
	const int TEXT_BASELINE = 20;
	const int BLOCK_WIDTH = 50;
	const int BLOCK_HEIGHT = 30;
	const int BLOCK_LEN = 10;
	const double FREAK_LIMIT = 30;
	const int OFFSET_FRAME = 15;

	// ����� ������������ ������� ����� ������ ���� ������
	int max_freak = freak_sections[0];
	for (int i = 1; i < section_count; i++)
		if (freak_sections[i] > max_freak)
			max_freak = freak_sections[i];
	
	// ��������, ��������� �� ��������� ���������������
	bool scaling;
	if (max_freak > FREAK_LIMIT)
		scaling = true;
	else
		scaling = false;

	double shift = 20;
	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
	for (int i = 0; i < section_count; i++) {
		double section_width;
		if (scaling)
			section_width = BLOCK_LEN * freak_sections[i] / (max_freak / FREAK_LIMIT);
		else
			section_width = BLOCK_LEN * freak_sections[i];
		svg_text(TEXT_LEFT, TEXT_BASELINE + shift, to_string(freak_sections[i]));
		svg_rect(BLOCK_WIDTH, shift, section_width, BLOCK_HEIGHT, "blue", "green");
		shift += BLOCK_HEIGHT;
	}
	int rightX;
	if (scaling)
		rightX = 365;
	else
		rightX = BLOCK_WIDTH + max_freak * BLOCK_LEN + OFFSET_FRAME;
	int downY = 5 + OFFSET_FRAME + BLOCK_HEIGHT * section_count + OFFSET_FRAME;

	cout << "<line x1='5' y1='5' x2 ='" << rightX << "' y2='5' stroke='red' stroke-width='1' stroke-dasharray='5 5'/>";
	cout << "<line x1='" << rightX << "' y1='5' x2 ='" << rightX << "' y2='" << downY << "' stroke='red' stroke-width='1' stroke-dasharray='5 5'/>";
	cout << "<line x1='" << rightX << "' y1='" << downY << "' x2 ='5' y2='" << downY << "' stroke='red' stroke-width='1' stroke-dasharray='5 5'/>";
	cout << "<line x1='5' y1='" << downY << "' x2 ='5' y2='5' stroke='red' stroke-width='1' stroke-dasharray='5 5'/>";
	svg_end();
}


int main() {

	setlocale(LC_ALL, "Russian");

	// ���� ���������� �����
	int numbers_count;
	//cout << "������� ���-�� �����: ";
	cin >> numbers_count;

	// ���������� ������� �������
	//cout << "������� �����:\n";
	const auto numbers = input_numbers(numbers_count);

	// ���� ���������� ������ � �������� ������� ������ �� ������ ���������
	double sections_count;
	//cout << "������� ���-�� ������: ";
	cin >> sections_count;
		
	vector<int> freak_sections = make_histogram(numbers, numbers_count, sections_count);

	show_histogram_svg(freak_sections, sections_count);

}
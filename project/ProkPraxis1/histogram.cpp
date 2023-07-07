#include "histogram.h"
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
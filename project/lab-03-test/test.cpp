#include "histogram.h"
#include <cassert>
void test__find_minmax() {
	double min = 0;
	double max = 0;
	find_minmax({}, 0,  min, max);
	assert(min == 0);
	assert(max == 0);
}
int
main() {
	test__find_minmax();

}
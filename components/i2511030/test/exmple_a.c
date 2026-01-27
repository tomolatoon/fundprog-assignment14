#include "i2511030/component.h"

int main()
{
	double time = 0.5;

	HLayer layer1 = i2511030_sample_layer1_generator(time);

	layer_save_p3(layer1, "test_output_layer1.ppm");

	return 0;
}


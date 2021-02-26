#include <iostream>
#include "../include/funcs.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: ./pulse <imgpath>" << std::endl;
		return -1;
	}

	float a; float b;

	while (1) {
		std::cout << "Enter two numbers constrast[1.0,3.0], brightness[0,100]" << std::endl;
		std::cin >> a; std::cin >> b;
		adjust_contrast_and_brightness(argv[1], a, b);
	}
	
	return 0;
}

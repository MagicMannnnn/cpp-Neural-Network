#include "network.h"
#include <iostream>

#include <vector>

int main() {

	//pass by reference in layers or will it break???
	
	Network network;
	network.addLayer("Dense", 2, 3);
	network.addLayer("Tanh");
	network.addLayer("Dense", 3, 1);
	network.addLayer("Relu");

	//network.load(NETWORKS_PATH"XNOR");

	std::vector < std::vector<double>> X = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	std::vector < std::vector<double>> Y = { {1}, {0}, {0}, {1} };


	for (int i = 0; i < 10; i++) {
		

		for (std::vector<double>& x : X) {

			std::cout << network.forwardPass(x)[0] << std::endl;
		}
		std::cout << std::endl;

		network.train(1000, 0.1, X, Y, false);
	}

	for (std::vector<double>& x : X) {

		std::cout << network.forwardPass(x)[0] << std::endl;
	}
	std::cout << std::endl;

	

	network.save(NETWORKS_PATH"XNOR");
	

	return 0;
}
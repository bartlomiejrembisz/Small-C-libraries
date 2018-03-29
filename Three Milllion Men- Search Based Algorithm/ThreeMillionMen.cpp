#include <string>
#include <iostream>
#include <vector>
#include "three-million-men-algorithm.h"
#include <map>
#include <unordered_map>



int main()
{
	std::string filePath = "1K.txt";


	std::cout << R"(Choose the data structure: 
1. Map
2. Unordered_Map
)" << std::endl;

	int choice;
	std::cin >> choice;

	switch (choice)
	{
	case 1:
		Extract<std::map, std::string, std::string>(filePath);
		break;
	case 2:
		Extract<std::unordered_map, std::string, std::string>(filePath);
		break;
	}
	
	int test;
	std::cin >> test;

	return 0;
}

#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

//Load the file
template<typename TContainer>
TContainer loadData(const std::string &fileName)
{
	TContainer tempContainer;

	std::ifstream myFile;
	myFile.open(fileName);
	std::string line;

	//Read file content
	while (std::getline(myFile, line))
	{
		//Spliting the lines to create a pair.
		size_t commaPos = line.find_first_of(',');

		std::string eastern = line.substr(0, commaPos);
		std::string western = line.substr(commaPos + 1, line.size());

		tempContainer.emplace(eastern, western); //Emplace instead of insert to avoid copying to increase performance
	}

	return tempContainer;
}

//Function responsible for initialising containers as well as extracting the names out of the dataset and parsing them
template<template<typename...> class Container, typename Key, typename Value>
void Extract(const std::string &filePath)
{
	using std::chrono::steady_clock;
	using std::chrono::milliseconds;
	using std::chrono::duration_cast;

	std::cout << "Using container: " << typeid(Container).name() << std::endl;

	const short numIter = 3;
	long long totalTime = 0;
	std::string file = "50.txt";
		
	std::cout << "File: " << file << std::endl;
	std::cout << "Time 1 (ms):\tTime 2 (ms):\tTime 3 (ms):\tAverage (ms):" << std::endl;
	for (int i = 0; i < numIter; ++i)
	{

		Container<Key, Value> dataSet = loadData<Container<Key, Value>>(filePath);
		std::list<Value> resultSequence;

		steady_clock::time_point startTime = steady_clock::now();

		std::string startingPoint = dataSet.begin()->first;
		resultSequence.emplace_back(startingPoint);

		ExtractPeople<Container, Key, Value>(resultSequence, dataSet);

		steady_clock::time_point finishTime = steady_clock::now();

		long long timeTaken = duration_cast<milliseconds>(finishTime - startTime).count();
		std::cout << timeTaken << "\t\t";
		totalTime += timeTaken;
		saveToFile<Value>(resultSequence, file);
	}

	std::cout << totalTime / numIter << std::endl << std::endl;

}

//Function to copy and reverse the container and then extract the names in the most optimal way.
template<template<typename...> class Container, typename Key, typename Value>
void ExtractPeople(std::list<std::string> &resultSequence, Container<Key, Value> &dataSet)
{
	Container<Value, Key> reverseDataSet;
	for (Container<Key, Value>::iterator i = dataSet.begin(); i != dataSet.end(); ++i)
		reverseDataSet.insert(std::make_pair(i->second, i->first));


	Container<Key, Value>::iterator it = dataSet.begin();
	while (it != dataSet.end())
	{
		std::string westernPerson = it->second;
		resultSequence.emplace_back(westernPerson);
		it = dataSet.find(westernPerson);
	}

	Container<Value, Key>::iterator reverseIt = reverseDataSet.find(*(resultSequence.begin()));
	while (reverseIt != reverseDataSet.end())
	{
		std::string easternPerson = reverseIt->second;
		resultSequence.emplace_front(easternPerson);
		reverseIt = reverseDataSet.find(easternPerson);
	}
}

//Function responsible for saving the output into the file.
template<typename Value>
bool saveToFile(const std::list<Value> &_list, const std::string &fileName)
{
	std::ofstream newFile;
	newFile.open("output_" + fileName);
	for (auto it = _list.begin(); it != _list.end(); ++it)
		newFile << *it << "\n";
	newFile.close();
	return true;
}



#include <iostream>		//couts
#include <vector>		//data storage
#include "node.cpp"		//nodes
// #include "block.cpp"	//blocks for createBlock
#include <fstream>		//file generation
#include <iomanip>	    //io manipulation - for inputs
#include <sstream>		//for importing FAT (iss >> x)
#include <stdlib.h>		//rand
#include <map>			//map multi map
#include "Blockchain/blockchain.hpp"
// #include "Blockchain/blocks.hpp"

#define RESET 	"\033[0m"
#define YELLOW 	"\033[33m"
#define RED 	"\033[31m"
#define CYAN 	"\033[36m"

//nice functions
void printNice(std::string print) {	//print out UI for user
	std::cout << YELLOW << "*****" << print << "*****" << RESET << std::endl;
}
void populateNameVecWithRandom(std::vector<std::string>& fileNames, std::vector<std::string>& content, std::vector<std::string>& inputFileNames) {	//populate sample inputs for testing / avoid wasting time.
	fileNames.clear();
	fileNames.push_back("one");
	fileNames.push_back("two");
	fileNames.push_back("three");
	fileNames.push_back("four");
	fileNames.push_back("five");
	fileNames.push_back("six");
	fileNames.push_back("seven");
	fileNames.push_back("eight");
	fileNames.push_back("nine");
	fileNames.push_back("ten");
	fileNames.push_back("eleven");
	fileNames.push_back("twelve");

	content.clear();
	content.push_back("We're no strangers to love");
	content.push_back("You know the rules and so do I");
	content.push_back("A full commitment's what I'm thinking of");
	content.push_back("You wouldn't get this from any other guy");
	content.push_back("I just wanna tell you how I'm feeling");
	content.push_back("Gotta make you understand");
	content.push_back("Never gonna give you up");
	content.push_back("Never gonna let you down");
	content.push_back("Never gonna run around and desert you");
	content.push_back("Never gonna make you cry");
	content.push_back("Never gonna say goodbye");
	content.push_back("Never gonna tell a lie and hurt you");

	inputFileNames.clear();
	inputFileNames.push_back("train monks.txt");
	inputFileNames.push_back("test monks.txt");

}
template <class T> void swap(T& a, T& b) {	//Simple template swap
	T temp = a;
	a = b;
	b = temp;
}
template <class T> void shuffleVector(std::vector<T>& vecToShuffle) {	//shuffle a vector's contents for file scrambling
	int random;
	for (int i = 0; i < vecToShuffle.size(); i++) {
		random = rand() % vecToShuffle.size();	//between 0 and size()-1
		swap(vecToShuffle[i], vecToShuffle[random]);
	}
}
void split(std::vector<std::string>& vecToPopulate, std::string stringToSplit, int numPieces) {	//Splits stringToSplit into numPieces and puts them into vecToPopulate (splits a string into x files)
	int length = stringToSplit.length();
	if (length == 0) {
		printNice("Nothing to Split");
		return;
	}

	int splitSize = length / numPieces;
	splitSize++;
	std::string temp;

	for (int i = 0; i < length; i++) {		//split
		temp += stringToSplit[i];
		if (i % splitSize == 0 && i != 0) {
			vecToPopulate.push_back(temp);
			temp = "";
		}
	}

	if (vecToPopulate.size() != numPieces) {	//get last string if not perfect division
		vecToPopulate.push_back(temp);
	}
}

//importing, exporting, and block generation

void importFat(std::vector<Node>& nodes, std::string fileName) {	//not working	//error string filename was declared under as well.
	printNice("Importing File Allocation Table from file");
	std::ifstream myFile;
	std::string line, name;
	int nodeNumber, blockNumber, counter = 0;

	//node number, filename block filename block filename block
	myFile.open(fileName);
	if (myFile) {
		while (getline(myFile, line)) {
			std::istringstream iss(line);
			iss >> nodeNumber;

			Node temp(nodeNumber);
			nodes.push_back(temp);
			while (iss >> blockNumber >> name) {
				Block b(name);
				nodes[counter].getBlocks().push_back(b);
			}
			counter++;
		}
		myFile.close();
	}
	else {
		printNice("error opening file");
		return;
	}
}
void exportFat(std::vector<Node>& nodes) {	//export contents in the following way: (node number) (file name) (block number) into a single file
	printNice("Exporting File Allocation Table as backup.txt");
	std::ofstream myFile;
	myFile.open("backup.txt");

	//similar to the print function, just outputs.
	for (int i = 0; i < nodes.size(); i++) {	//prints node number, file name, then block number
		myFile << nodes[i].getNodeNumber() << " ";
		for (int j = 0; j < nodes[i].getBlocks().size(); j++) {
			myFile << nodes[i].getBlocks()[j].getFileName() << " " << j << " "; //<< nodes[i].getBlocks()[j].getData() << " ";
		}
		myFile << std::endl;
	}
	myFile.close();
}
std::string importFile(const std::string& fileName) {	//takes in a file name and returns the contents in a string
	std::string output, data;
	std::ifstream myFile(fileName.c_str());

	//getline(myFile, output, std::string::traits_type::to_char_type(std::traits_type::eof()));
	while (std::getline(myFile, data)) {
		output += data;
		output.push_back(" ");
	}

	return output;
}
void exportSearchedFile(std::vector<Node>& nodes, std::multimap<std::string, std::pair<int, int>>& table, std::string userInput, int numFiles) {	//Doing rn
	//getting searched file into a string
	std::multimap<std::string, std::pair<int, int>>::iterator it = table.find(userInput);
	std::string s;
	int nodeNumber, blockNumber;

	//output file variables
	const std::string path = "storage/";
	std::ofstream temp(path + userInput);

	//populate
	for (int i = 0; i < numFiles; i++) {
		nodeNumber = it->second.first;
		blockNumber = it->second.second;
		//std::cout << "node number: " << nodeNumber << " block number: " << blockNumber << " data: " << nodes[nodeNumber].getBlocks()[blockNumber].getData() << std::endl;
		s += nodes[nodeNumber].getBlocks()[blockNumber].getData();
		it++;
	}

	temp << s;
	temp.close();
}

void exportBlock(std::string fileName, std::string content) {	//exports each individual block for Networking portion
	const std::string path = "storage/";
	std::ofstream temp(path + fileName);
	temp << content;
	temp.close();
}
//populate Fat
void createBlocks(std::vector<Node>& nodes, std::multimap<std::string, std::pair<int, int>>& table, std::vector<std::string>& splitStrings, std::string fileName) {	//Creates blocks and populates them with the content and file name
		std::vector<int> temp;
		int counter = 0;
		for (int i = 0; i < splitStrings.size(); i++) {
			temp.push_back(i);
			counter++;
		}
		shuffleVector(temp);

		for (int i = 0; i < splitStrings.size(); i++) {
			//block generation and fat population
			table.insert(std::make_pair(fileName, std::make_pair(temp[i], nodes[temp[i]].getBlocks().size()))); //fileName, pair(nodeNumber, blockNumber)
			//Block b(fileName);
			Block b(fileName, splitStrings[i]);
			nodes[temp[i]].pushBackBlock(b);

			//file generation (for emmanuel)
			std::string newFileName = fileName + "_" + std::to_string(temp[i]) + "_" + std::to_string(nodes[temp[i]].getBlocks().size() - 1);   //filename_nodenumber_blocknumber
			exportBlock(newFileName, splitStrings[i]);	//filename, content
		}

		splitStrings.clear();
	}

	void deleteBlock(std::vector<Node> & nodes, std::multimap<std::string, std::pair<int, int>> & table, int userInput) {	//in progress, create destructors
		nodes[userInput].getBlocks().clear();
		for (int i = 0; i < nodes.size(); i++) {
			//if(nodes[i].getBlocks());
		}
	}
	//print
	void printFatContent(std::vector<Node> & nodes) {	//prints proper fat
		if (fatIsSetUp) {
			printNice("Showing File Allocation Table");
			for (int i = 0; i < nodes.size(); i++) {	//goes through each node
				std::cout << "Node number " << nodes[i].getNodeNumber() << std::endl;;
				for (int j = 0; j < nodes[i].getBlocks().size(); j++) {	//goes through each block of that node.
					std::cout << std::setw(5) << " " << "File name: " << std::left << std::setw(10) << nodes[i].getBlocks()[j].getFileName() << std::endl; //<< " content: " << std::left << std::setw(20) << nodes[i].getBlocks()[j].getData() << std::endl;
				}
			}
		}
		else {
			printNice("Please set up the File Allocation Table first");
		}
	}
	void printMap(std::multimap<std::string, std::pair<int, int>> & table) {	//prints table
		if (!table.empty()) {
			printNice("Showing file table");
			for (auto i = table.begin(); i != table.end(); i++) {	//prints whole map
				std::cout << "File name: " << std::left << std::setw(20) << i->first << " is located in Node " << std::setw(5) << i->second.first << " block " << std::setw(5) << i->second.second << std::endl;
			}
		}
		else {
			printNice("Please set up the File Allocation Table first");
		}
	}
	void printSearchedFile(std::vector<Node> & nodes, std::multimap<std::string, std::pair<int, int>> & table, std::string userInput, int numFiles) {	//searches for a file name and puts it in the correct order
		std::multimap<std::string, std::pair<int, int>>::iterator it = table.find(userInput);
		std::string s;
		int nodeNumber, blockNumber;
		for (int i = 0; i < numFiles; i++) {
			nodeNumber = it->second.first;
			blockNumber = it->second.second;
			//std::cout << "node number: " << nodeNumber << " block number: " << blockNumber << " data: " << nodes[nodeNumber].getBlocks()[blockNumber].getData() << std::endl;
			s += nodes[nodeNumber].getBlocks()[blockNumber].getData();
			it++;
		}
		std::cout << "The combined file " << userInput << " is: " << s << std::endl;
	}
	void printEverything(std::vector<Node> & nodes) {	//prints FAT AND the contents.
		if (fatIsSetUp) {
			printNice("Showing File Allocation Table");
			for (int i = 0; i < nodes.size(); i++) {	//goes through each node
				std::cout << "Node number " << nodes[i].getNodeNumber() << std::endl;;
				for (int j = 0; j < nodes[i].getBlocks().size(); j++) {	//goes through each block of that node.
					std::cout << std::setw(5) << " " << "File name: " << std::left << std::setw(10) << nodes[i].getBlocks()[j].getFileName() << " content: " << std::left << std::setw(20) << nodes[i].getBlocks()[j].getData() << std::endl;
				}
			}
		}
		else {
			printNice("Please set up the File Allocation Table first");
		}
	}

	int main(int argc, char* argv[]) {
		int userChoice, numFiles;
		std::string userInput, fileName, filename;
		bool fatIsSetUp = false;
		std::vector<std::string> splitStrings;
		std::vector<Node> nodes;
		std::multimap<std::string, std::pair<int, int>> table;

		//filenames (inputs to avoid cin / cout)
		int nameIterator = 0;	//iterates every use for case 2
		int nameIterator2 = 0;	//iterates every use for case 11
		std::vector<std::string> fileNames, fileContent, inputFileNames;
		populateNameVecWithRandom(fileNames, fileContent, inputFileNames);


		// blockchain: @params: difficulty of PoW algorithm. Higher the more difficulty.
		Blockchain bchain(2);
		// declare for when we import blockchain:
		// @params: (difficulty, any number(not used))
		Blockchain importedChain(2, -1); // currently empty until we import a Blockchain from a file

		while (true)
		{
			std::cout << CYAN << "1: Set up FAT, 2: Create Block, 3: Print FAT, 4: Print File Names, 5: Print Everything, 6: Delete Block 7: Clear FAT, 8: Import FAT, 9: Export FAT, 10: Search File, 11: Create Block from File" << RESET << std::endl;
			std::cout << "Your Input: ";
			std::cin >> userChoice;

			switch (userChoice)
			{
			case 1:	//Set up FAT: set up fat once: makes the nodes and keeps that amount of nodes until deleted.
				if (fatIsSetUp) {
					printNice("The File Allocation Table is already set up");
				}
				else {
					printNice("Setting up File Allocation Table");
					/* std::cout << "Enter amount of files: " << std::endl;
					std::cin >> numFiles;*/
					numFiles = 3;

					for (int i = 0; i < numFiles; i++) {
						Node temp(nodes.size());
						nodes.push_back(temp);
					}
					fatIsSetUp = true;
				}
				break;
			case 2:	//Create Block: any additional file will be split and added randomly to different nodes that was already set up.
				if (fatIsSetUp) {	//only adds these blocks if there are nodes to hold them.
					printNice("Creating a file");
					/*std::cout << "Enter what to put into the file" << std::endl;
					std::cin >> userInput;
					std::cout << "Enter name for the file" << std::endl;
					std::cin >> fileName*/

					userInput = fileContent[nameIterator];
					fileName = fileNames[nameIterator];

					split(splitStrings, userInput, numFiles);
					createBlocks(nodes, table, splitStrings, fileName, bchain);
					nameIterator++;
				}
				else {
					printNice("Please set up the File Allocation Table first");
				}
				break;
			case 3:	//Print FAT
				printFatContent(nodes);
				break;
			case 4:	//Print File Names
				printMap(table);
				break;
			case 5:	//Print Everything - for Jay's blockchain code
				printEverything(nodes);
				break;
			case 6:	//Delete block
				printNice("Enter the name of the file you want to delete");
				std::cin >> userChoice;
				deleteBlock(nodes, table, userChoice);
				break;
			case 7:	//Clear FAT
				printNice("Clearing File Allocation Table");
				nodes.clear();
				table.clear();
				fatIsSetUp = false;
				nameIterator = 0;
				nameIterator2 = 0;
				break;
			case 8:	//Import FAT	//not working 
				//std::cout << "Enter file name for import (add extension if applies)" << std::endl;
				//std::cin >> userInput
				userInput = "backup.txt";

				if (fatIsSetUp) {
					printNice("You can't import, FAT is already set up");
				}
				else {
					importFat(nodes, userInput);
					fatIsSetUp = true;
				}
				break;
			case 9:	//Export FAT to a single file
				exportFat(nodes);
				break;
			case 10:	//Search for a file name and display it in the correct order
				if (fatIsSetUp) {
					while (true) {
						printNice("Enter the name of the file you want to search for, and q to stop");
						std::cin >> userInput;
						if (userInput != "q") {
							printSearchedFile(nodes, table, userInput, numFiles);
						}
						else {
							printNice("Quitting command");
							break;
						}
					}
				}
				else {
					printNice("Please set up the File Allocation Table first");
				}
				break;
			case 11:	//Create Block from file
				if (fatIsSetUp) {
					/*std::cout << "Enter the name and extension of the file " << std::endl;
					std::cin >> userInput;*/

					printNice("Creating a Block from the file");

					userInput = importFile(inputFileNames[nameIterator2]);

					split(splitStrings, userInput, numFiles);
					exportSearchedFile(nodes, table, userInput, numFiles)
						nameIterator++;
				}
				else {
					print Nice("Please set up the File Allocation Table first");
				}
			default:
				std::cout << RED << "*****You didn't enter a proper command*****" << RESET << std::endl;
				break;
			}
		}
		return 0;
	}

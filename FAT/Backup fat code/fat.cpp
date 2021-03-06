#ifndef FAT_H
#define FAT_H

#include <vector>	//data storage

class Node;	//forward declaration
class Fat{
	public:
		Fat();
		Fat(std::vector<Node> nodes);
		//getters and setters
		std::vector<Node>& getNodes();
		void setNodes(std::vector<Node> nodes);
	private:
		std::vector<Node> nodes;
		std::string fileName;
};	

Fat::Fat(){
	//this->nodes = new std::vector<Node>;
	this->fileName = "name";
}

Fat::Fat(std::vector<Node> nodes){
	this->nodes = nodes;
}

//getters and setters
std::vector<Node>& Fat::getNodes() {	
	return nodes;
}

void Fat::setNodes(std::vector<Node> nodes) { //pass in reference to make it faster, const so it doesnt get modified.
	this->nodes = nodes;
}
#endif 

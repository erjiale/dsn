// dsn - decentralized storage network


#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

// libraries for client-side, including some above
#include <arpa/inet.h>

// Classes
#include "Requester.hpp"
#include "Listener.hpp"

void menu() {
  std::cout << "Menu:\n";
  std::cout << "\ttype \"fetch\" to collect all distributed blocks and store local file copy\n";
   

  std::cout << "\n> ";
}

void run_server(const int server_port) {
  Listener listener(server_port);
  listener.run();
}

void run_client(const Request & request) {
  Requester requester;
  requester.run(request);
}

int main() {
  // Get port, and all the nodes
  std::ofstream nodes_list;
  nodes_list.open("./nodes.txt");

  std::cout << "[?] What is the port of this node?: ";
  int server_port = 0;
  std::cin >> server_port;


  std::vector<std::string> addresses;
  std::vector<int> ports;

  addresses.reserve(4);
  ports.reserve(4);
  
  int node_number = 0;
  std::string address = "";
  int port = 0;

  // Add this node as first node
  nodes_list << "0 127.0.0.1 " << server_port << std::endl;
  addresses.push_back("127.0.0.1");
  ports.push_back(server_port);

  while (true) {
    std::cout << "[?] Enter address for node in network, enter 0 to end: "; 
    std::cin >> address;

    if (address == "0")
      break;

    addresses.push_back(address);

    std::cout << "[?] Enter port for current node: ";
    std::cin >> port;
    ports.push_back(port);

    nodes_list << ++node_number << " " << address << " " << port << std::endl;
  }

  nodes_list.close();

  // Send all nodes a copy of the FAT
  std::cout << "[?] Distribute FAT?[y/n]: ";
  std::string response = "";
  std::cin >> response;

  if (response == "y") {  
    // Check if FAT file exists
    std::ifstream fat_file("./backup.txt");
    std::ifstream nodes_file("./nodes.txt");
    
    if (!fat_file.is_open()) {
      std::cout << "[Error] FAT table file missing. Exiting..." << std::endl;
      return 0;
    }
    
    if (!nodes_file.is_open()) {
      std::cout << "[Error] nodes.txt missing. Exiting..." << std::endl;
    }

    std::vector<std::string> fat;
    fat.reserve(10);

    std::string line = "";
    // Load FAT into memory
    while(std::getline(fat_file, line))
      fat.push_back(line);



    std::vector<std::string> nodes;
    nodes.reserve(5);

    // Load nodes list into memory
    line = "";
    while(std::getline(nodes_file, line))
      nodes.push_back(line);

    // Usecase 3 specific
    Request send_fat_to_node2;
    Request send_fat_to_node3;
  
    send_fat_to_node2.type = "fat_distrib";
    send_fat_to_node3.type = "fat_distrib";
  
    send_fat_to_node2.target_address = addresses[1];
    send_fat_to_node3.target_address = addresses[2];
  
    send_fat_to_node2.target_port = ports[1];
    send_fat_to_node3.target_port = ports[2];
   
    send_fat_to_node2.fat_copy = fat;   
    send_fat_to_node3.fat_copy = fat;   
  
    std::thread fat_distrib_thread_1(run_client, send_fat_to_node2); 
    std::thread fat_distrib_thread_2(run_client, send_fat_to_node3); 
  
    fat_distrib_thread_1.join();
    fat_distrib_thread_2.join();
    
    // Usecase 4 specific
    Request send_nodes_to_node2;
    Request send_nodes_to_node3;
  
    send_nodes_to_node2.type = "nodes_distrib";
    send_nodes_to_node3.type = "nodes_distrib";
  
    send_nodes_to_node2.target_address = addresses[1];
    send_nodes_to_node3.target_address = addresses[2];
  
    send_nodes_to_node2.target_port = ports[1];
    send_nodes_to_node3.target_port = ports[2];
   
    send_nodes_to_node2.nodes_copy = nodes;   
    send_nodes_to_node3.nodes_copy = nodes;   
  
    std::thread nodes_distrib_thread_1(run_client, send_nodes_to_node2); 
    std::thread nodes_distrib_thread_2(run_client, send_nodes_to_node3); 
  
    nodes_distrib_thread_1.join();
    nodes_distrib_thread_2.join();
        
    
  }
  
  // Immediately start listening for requests
  std::thread listening_thread(run_server, server_port);

  // User interaction loop
  std::string user_input = "";
  menu();
  std::cin >> user_input;  

  while (user_input != "q") {
    if (user_input == "fetch") {
      std::cout << "[Main] Preparing file fetch...";
      std::string filename = "";
      std::cout << "[?] What is the name of the file?: ";
      std::cin >> filename;
      std::cout << "[Main] File \"" << filename << "\"...";
      
      // List blocks, list locations (ip and port)  
      
      
    } else {
      std::cout << "[ERROR] Unknown command: \"" << user_input << "\"...\n"; 
    }
    
    menu();
    std::cin >> user_input;
  
  }




  listening_thread.join();
  return 0;
}


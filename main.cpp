/*
* The core middleware component: Acts as the interface between users and the encrypted file system. 
* It interprets commands and performs actions such as file and directory operations within an encrypted environment.
*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "helpers/json.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cerr << "\nKeyfile for authentication missing or incorrectly provided.\n" 
                << "Usage: ./fileserver keyfile_name\n" << std::endl;
        return 1;
    }

    return 0;
}

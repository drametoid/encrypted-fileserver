#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <filesystem>

#include "encryption/encryption.h"
#include "file_operations/user_features.h"
#include "user_management/user_management.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  std::string filesystemPath = fs::current_path();

  if(fs::exists("filesystem")) {
    if(argc != 2) {
      std::cout << "Invalid keyfile\n" << std::endl;
      return 1;
    } 
    else {
      std::string keyFileName = argv[1];
      std::string userName = getTypeOfUser(keyFileName);
      UserType userType;
      if(userName == "admin")
          userType = admin;
      else
          userType = user;

      userFeatures(userName, userType, readEncKeyFromMetadata(userName, ""), filesystemPath);
    }
  } 
  else {
    if(!createDirectory("public_keys") ||
      !createDirectory("private_keys") ||
      !createDirectory("metadata") ||
      !createDirectory("shared_files") ||
      !createDirectory("filesystem")) {
        return 1;
    }

    std::string metadataPath = "metadata/metadata.json";
    std::ofstream metadataFile(metadataPath);
    if (metadataFile) {
        metadataFile << "{\"test\":\"123\"}";
        metadataFile.close();
    } 
    else {
        std::cerr << "Error creating metadata.json\n" << std::endl;
        return 1;
    }

    std::string userName = "admin";
    addUser(userName, filesystemPath, true);
    userFeatures(userName, UserType::admin, readEncKeyFromMetadata(userName, ""), filesystemPath);
  }
}

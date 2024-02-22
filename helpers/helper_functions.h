#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "../encryption/encryption.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <openssl/rand.h>
#include <regex>
#include <sstream>
#include <string>
#include <unistd.h>

#include "../encryption/randomizer_function.h"

using namespace std;
namespace fs = std::filesystem;

/// Normalize a path by removing trailing slashes and collapsing consecutive slashes to a single slash
/// \param path The path to normalize
string normalizePath(string path) {
    int n = path.length();
    int i = n - 1;
    while (i > 0 && path[i] == '/') {
        i--;
    }
    path.erase(i + 1, n - i - 1);
    if (path.length() > 1) {
        string::iterator it = unique(path.begin(), path.end(), [](char currentChar, char nextChar) {
            return currentChar == nextChar && currentChar == '/';
        });
        path.erase(it, path.end());
    }
    return path;
}

vector<uint8_t> readEncKeyFromMetadata(string userName, string directory) {
    // Determine the metadata file path
    const std::string metadataFilePath = !directory.empty() ? directory : "metadata/";
    std::ifstream metadataFile(metadataFilePath + userName + "_key", std::ios::in | std::ios::binary);

    if (!metadataFile) {
        std::cerr << "Failed to read key from metadata for " << userName << std::endl;
        return {}; // Return an empty vector if the file failed to open
    }

    std::vector<uint8_t> encryptionKey(KEY_SIZE);
    metadataFile.read(reinterpret_cast<char*>(encryptionKey.data()), encryptionKey.size());

    return encryptionKey;
}

bool isValidFilename(const string &filename) {
    // Regex pattern to validate the filename structure
    std::regex validFilenamePattern(
            "^[a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?(\\.(?!$)[a-zA-Z0-9_-]+)+$"
            "|^([a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?)$"
    );

    // Maximum allowed length for the filename
    const int MaxFilenameLength = 255;

    // Check if the filename matches the pattern and does not exceed the maximum length
    return std::regex_match(filename, validFilenamePattern) && (filename.length() < MaxFilenameLength);

}

bool checkIfPersonalDirectory(string username, string pwd, string filesystemPath) {
    // Generate randomized directories for the user and their personal space
    std::string userDirectory = FilenameRandomizer::GetRandomizedName("/filesystem/" + username, filesystemPath);
    std::string personalDirectory = FilenameRandomizer::GetRandomizedName("/filesystem/" + userDirectory + "/personal", filesystemPath);
    std::string authorizedWritePath = "/filesystem/" + userDirectory + "/" + personalDirectory;

    // Validate the pwd against the authorized write path
    if (pwd.length() >= authorizedWritePath.length() &&
        pwd.substr(0, authorizedWritePath.length()) == authorizedWritePath) {
        return true;
    }

    return false;
}

string getUsernameFromPath(string path) {
    const std::string filesystemPrefix = "/filesystem/";
    if (path.size() <= filesystemPrefix.size()) return "";

    // Remove the filesystem prefix from the start of the path
    std::string withoutPrefix = path.substr(filesystemPrefix.size());

    // Find the next slash after the username
    size_t slashIndex = withoutPrefix.find('/');
    if (slashIndex != std::string::npos) {
        // If found, truncate the string to only include the username
        return withoutPrefix.substr(0, slashIndex);
    }

    // If no further slash is found, the remaining string is the username
    return withoutPrefix;
}

void createInitFsForUser(string username, string path) {
    mode_t old_umask = umask(0); // to ensure the following modes get set
    mode_t mode = 0700;

    string encrypted_username = FilenameRandomizer::EncryptFilename("/filesystem/" + username, path);
    string u_folder = path + "/filesystem/" + encrypted_username;
    if (mkdir(u_folder.c_str(), mode) != 0) {
        cerr << "Error creating root folder for " << username << endl;
    } else {
        string encrypted_p_folder = FilenameRandomizer::EncryptFilename("/filesystem/" + encrypted_username + "/personal", path);
        u_folder = path + "/filesystem/" + encrypted_username + "/" + encrypted_p_folder;
        if (mkdir(u_folder.c_str(), mode) != 0) {
            cerr << "Error creating personal folder for " << username << endl;
        }
        string encrypted_s_folder = FilenameRandomizer::EncryptFilename("/filesystem/" + encrypted_username + "/shared", path);
        u_folder = path + "/filesystem/" + encrypted_username + "/" + encrypted_s_folder;
        if (mkdir(u_folder.c_str(), mode) != 0) {
            cerr << "Error creating shared folder for " << username << endl;
        }
    }
    umask(old_umask); // Restore the original umask value
}



#endif
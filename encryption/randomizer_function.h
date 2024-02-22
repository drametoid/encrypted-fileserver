#ifndef FILESERVER_RANDOMIZER_FUNCTION_H
#define FILESERVER_RANDOMIZER_FUNCTION_H

#include "../helpers/json.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <stdexcept> // for std::runtime_error
#include <filesystem> // for std::filesystem

namespace fs = std::filesystem;
using json = nlohmann::json;

class FilenameRandomizer {
public:
    static std::string Randomize(int length);
    static json ReadMetadata(const std::string& path_to_metadata);
    static std::string GetFilename(const std::string& randomized_name, const std::string& path_to_metadata);
    static std::string GetRandomizedName(const std::string& filename, const std::string& path_to_metadata);
    static std::string GetRandomizedFilePath(const std::string& filepath, const std::string& path_to_metadata);
    static std::string GetPlaintextFilePath(const std::string& randomized_filepath, const std::string& path_to_metadata);
    static std::string EncryptFilename(const std::string& filename, const std::string& path_to_metadata);
    static std::string DecryptFilename(const std::string& randomized_name, const std::string& path_to_metadata);

private:
    static std::string GenerateRandomString(int length);
};

#endif // FILESERVER_RANDOMIZER_FUNCTION_H
#include <string>
#include <iostream>
#include <zip.h>
#include <Windows.h>
#include <fstream>
#include <filesystem>

bool CreateDirectoryIfNotExist(std::string path) {
    if (CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
        return true;
    }
    
    return false;
}


std::string unzip(const char* zipFilename, const char* extractPath) {
    int err = 0;
    zip* archive = zip_open(zipFilename, ZIP_RDONLY, &err);
    if (!archive) {
        std::cerr << "Failed to open zip file" << std::endl;
        return "";
    }

    // Ensure the output directory exists
    std::filesystem::path outputDir(extractPath);
    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directories(outputDir);
    }

    std::string first_file = zip_get_name(archive, 0, ZIP_FL_ENC_UTF_8);

    zip_int64_t numFiles = zip_get_num_entries(archive, ZIP_FL_UNCHANGED);
    for (zip_int64_t i = 0; i < numFiles; ++i) {
        const char* name = zip_get_name(archive, i, ZIP_FL_ENC_UTF_8);
        if (!name) {
            std::cerr << "Failed to get file name" << std::endl;
            continue;
        }

        zip_file* file = zip_fopen_index(archive, i, ZIP_FL_UNCHANGED);
        if (!file) {
            std::cerr << "Failed to open file in zip archive" << std::endl;
            continue;
        }

        std::filesystem::path outputPath = outputDir / name;

        // Check if the current entry is a directory
        if (name[strlen(name) - 1] == '/') {
            std::filesystem::create_directories(outputPath);
            zip_fclose(file);
            continue;
        }

        // Create directories if needed
        std::filesystem::create_directories(outputPath.parent_path());

        std::ofstream outFile(outputPath.c_str(), std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            zip_fclose(file);
            continue;
        }

        char buffer[4096];
        zip_int64_t bytesRead;
        while ((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0) {
            outFile.write(buffer, bytesRead);
        }

        zip_fclose(file);
        outFile.close();
    }

    zip_close(archive);

    return first_file;
}
#pragma once
#include <fstream>
#include "Utils.hpp"

namespace DistinctWords
{
    class FileProcessor
    {
    public:
        FileProcessor(std::string fileName);
        std::ifstream &getFile();
        FileProcessingType getFileProcessingType();

    private:
        void setFileProcessingMethod(std::string firstLineFromFile);
        bool didFileOpen(std::string fileName);
        void chooseFileProcessingMethod(std::string fileName);

        std::ifstream file;
        FileProcessingType fileProcessingType{FileProcessingType::FILENOTOPEN};
    };
}
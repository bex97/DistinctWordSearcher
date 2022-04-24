#include "FileProcessor.hpp"
#include <regex>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>

namespace DistinctWords
{
    FileProcessor::FileProcessor(std::string fileName)
    {
        chooseFileProcessingMethod(fileName);
    }

    bool FileProcessor::didFileOpen(std::string fileName)
    {
        file.open(fileName);
        if (!file.is_open())
        {
            std::cerr << "File open failed!" << std::endl;
            return false;
        }
        return true;
    }

    void FileProcessor::chooseFileProcessingMethod(std::string fileName)
    {
        if (!didFileOpen(fileName))
            return;

        std::string firstLineFromFile;
        std::getline(file, firstLineFromFile);
        setFileProcessingMethod(firstLineFromFile);
        file.seekg(0);
    }

    void FileProcessor::setFileProcessingMethod(std::string firstLineFromFile)
    {
        if (firstLineFromFile.length() == firstLineFromFile.max_size() or file.eof())
        {
            fileProcessingType = FileProcessingType::PROCESSFILEBYWORDS;
        }
        else
        {
            fileProcessingType = FileProcessingType::PROCESSFILEBYLINES;
        }
    }

    FileProcessingType FileProcessor::getFileProcessingType()
    {
        return fileProcessingType;
    }

    std::ifstream &FileProcessor::getFile()
    {
        if (fileProcessingType == FileProcessingType::FILENOTOPEN)
            std::cout << "WARNING!!! Return value may be empty";
        return file;
    }
}
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
        openFile(fileName);
    }

    void FileProcessor::openFile(std::string fileName)
    {
        file.emplace();
        file->open(fileName);
        if (!file->is_open())
        {
            std::cerr << "File open failed!" << std::endl;
            file.reset();
        }
    }

    std::optional<std::ifstream>& FileProcessor::getFile()
    {
        return file;
    }

    void FileProcessor::closeFile()
    {
        if (file && file->is_open())
        {
            file->close();
            file.reset();
        }
    }
}
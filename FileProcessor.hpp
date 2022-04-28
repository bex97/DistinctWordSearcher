#pragma once
#include <fstream>
#include <optional>

namespace DistinctWords
{
    class FileProcessor
    {
    public:
        FileProcessor(std::string fileName);
        std::optional<std::ifstream> &getFile();
        void closeFile();

    private:
        void openFile(std::string fileName);

        std::optional<std::ifstream> file;
    };
}
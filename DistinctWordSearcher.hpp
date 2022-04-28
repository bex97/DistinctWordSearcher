#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <map>
#include <regex>
#include "FileProcessor.hpp"

namespace DistinctWords
{
    class DistinctWordSearcher
    {
    public:
        DistinctWordSearcher(
            std::shared_ptr<std::mutex> _mutexForFileAccess,
            std::shared_ptr<std::mutex> _mutexForMapAccess,
            std::shared_ptr<std::map<std::string, uint16_t>> _distinctWords);
        void run(std::shared_ptr<FileProcessor> fileProcessor);

    private:
        std::vector<std::string> splitWords(std::string stringFromFile);
        void addDistinctWords(std::string newLineFromFile);
        void processFile(std::ifstream &textFile);
        void pushWordsToMap(std::vector<std::string> &vecOfWords);
        void pushWordToMap(std::string word);
        void processFoundWord(std::string &stringFromFile, uint16_t positionOfSpace, std::vector<std::string> &vecOfWords);

        std::shared_ptr<std::mutex> mutexForFileAccess;
        std::shared_ptr<std::mutex> mutexForMapAccess;
        std::shared_ptr<std::map<std::string, uint16_t>> distinctWords;
    };
}
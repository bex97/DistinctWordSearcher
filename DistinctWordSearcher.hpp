#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <map>
#include "FileProcessor.hpp"
#include "Utils.hpp"

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
        void addDistinctWord(std::string newWord);
        void processFileByLines(std::ifstream &textFile);
        void processFileByWords(std::ifstream &textFile);
        void pushWordsFromVectorToMap(std::vector<std::string> vecOfWords);
        void pushNewWordToMap(std::string);

        std::shared_ptr<std::mutex> mutexForFileAccess;
        std::shared_ptr<std::mutex> mutexForMapAccess;
        std::shared_ptr<std::map<std::string, uint16_t>> distinctWords;
    };
}
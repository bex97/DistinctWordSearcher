#include "DistinctWordSearcher.hpp"
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>

namespace DistinctWords
{
    DistinctWordSearcher::DistinctWordSearcher(
        std::shared_ptr<std::mutex> _mutexForFileAccess,
        std::shared_ptr<std::mutex> _mutexForMapAccess,
        std::shared_ptr<std::map<std::string, uint16_t>> _distinctWords) : mutexForFileAccess{_mutexForFileAccess},
                                                                           mutexForMapAccess{_mutexForMapAccess},
                                                                           distinctWords{_distinctWords} {};

    std::vector<std::string> DistinctWordSearcher::splitWords(std::string stringFromFile)
    {
        std::vector<std::string> vecOfWords;
        std::size_t positionOfSpaceChar;
        while ((positionOfSpaceChar = stringFromFile.find_first_of("\t ")) != std::string::npos)
        {
            processFoundWord(stringFromFile, positionOfSpaceChar, vecOfWords);
        }
        vecOfWords.push_back(stringFromFile);
        return vecOfWords;
    }

    void DistinctWordSearcher::processFoundWord(std::string &stringFromFile, uint16_t positionOfSpaceChar, std::vector<std::string> &vecOfWords)
    {
        std::string word = stringFromFile.substr(0, positionOfSpaceChar);
        stringFromFile.erase(0, positionOfSpaceChar + 1);
        vecOfWords.push_back(word);
    }

    void DistinctWordSearcher::addDistinctWords(std::string newLineFromFile)
    {
        std::vector<std::string> wordsFromFile = splitWords(newLineFromFile);
        pushWordsToMap(wordsFromFile);
    }

    void DistinctWordSearcher::pushWordsToMap(std::vector<std::string> &vecOfWords)
    {
        for (auto word : vecOfWords)
        {
            if (!word.empty())
                pushWordToMap(word);
        }
    }

    void DistinctWordSearcher::pushWordToMap(std::string newWord)
    {
        std::unique_lock ul(*mutexForMapAccess);
        if (!distinctWords->count(newWord))
        {
            (*distinctWords)[newWord] = 1;
        }
    }

    void DistinctWordSearcher::processFile(std::ifstream &textFile)
    {
        while (true)
        {
            std::string stringFromFile;

            std::unique_lock ul(*mutexForFileAccess);
            if (textFile.eof())
                break;
            std::getline(textFile, stringFromFile);
            ul.unlock();
            addDistinctWords(stringFromFile);
        }
    }

    void DistinctWordSearcher::run(std::shared_ptr<FileProcessor> fileProcessor)
    {
        std::unique_lock ul(*mutexForFileAccess);
        auto &file = fileProcessor->getFile();
        if (file.has_value())
        {
            ul.unlock();
            processFile(file.value());
            ul.lock();
            fileProcessor->closeFile();
        }
    }
}
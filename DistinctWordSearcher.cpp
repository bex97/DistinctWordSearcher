#include "DistinctWordSearcher.hpp"
#include <regex>
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
        std::smatch wordMatch;
        std::regex regex("[a-zA-Z]+\\s+");
        while (std::regex_search(stringFromFile, wordMatch, regex))
        {
            std::string word = wordMatch.str();
            word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
            vecOfWords.push_back(word);
            stringFromFile = wordMatch.suffix().str();
        }
        if (stringFromFile != "")
            vecOfWords.push_back(stringFromFile);
        return vecOfWords;
    }

    void DistinctWordSearcher::addDistinctWords(std::string newLineFromFile)
    {
        std::vector<std::string> wordsFromFile = splitWords(newLineFromFile);
        pushWordsFromVectorToMap(wordsFromFile);
    }

    void DistinctWordSearcher::pushWordsFromVectorToMap(std::vector<std::string> vecOfWords)
    {
        std::unique_lock ul(*mutexForMapAccess);
        ul.unlock();
        for (auto word : vecOfWords)
        {
            ul.lock();
            if (!distinctWords->count(word))
            {
                (*distinctWords)[word] = 1;
            }
            ul.unlock();
        }
    }

    void DistinctWordSearcher::pushNewWordToMap(std::string newWord)
    {
        std::unique_lock ul(*mutexForMapAccess);
        if (!distinctWords->count(newWord))
        {
            (*distinctWords)[newWord] = 1;
        }
    }

    void DistinctWordSearcher::addDistinctWord(std::string newString)
    {
        if (newString.find('\n') != std::string::npos)
        {
            auto vecOfWords = splitWords(newString);
            pushWordsFromVectorToMap(vecOfWords);
        }
        else
        {
            pushNewWordToMap(newString);
        }
    }

    void DistinctWordSearcher::processFileByLines(std::ifstream &textFile)
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

    void DistinctWordSearcher::processFileByWords(std::ifstream &textFile)
    {
        while (true)
        {
            std::string stringFromFile;
            std::unique_lock ul(*mutexForFileAccess);
            if (textFile.eof())
                break;
            std::getline(textFile, stringFromFile, ' ');
            ul.unlock();
            addDistinctWord(stringFromFile);
        }
    }

    void DistinctWordSearcher::run(std::shared_ptr<FileProcessor> fileProcessor)
    {
        auto fileProcessingType = fileProcessor->getFileProcessingType();
        auto &file = fileProcessor->getFile();

        switch (fileProcessingType)
        {
        case FileProcessingType::PROCESSFILEBYLINES:
        {
            processFileByLines(file);
            break;
        }
        case FileProcessingType::PROCESSFILEBYWORDS:
        {
            processFileByWords(file);
            break;
        }
        default:
        {
            std::cerr << "File will not be processed. Please check file status";
            break;
        }
        }
    }
}
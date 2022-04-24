#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <functional>
#include "DistinctWordSearcher.hpp"

int main(int argc, char **argv)
{
    auto mutexForFileAccess = std::make_shared<std::mutex>();
    auto mutexForMapAccess = std::make_shared<std::mutex>();
    auto distinctWords = std::make_shared<std::map<std::string, uint16_t>>();
    const auto nthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threadsArray;

    if (argc < 2)
    {
        std::cerr << "There is no file name to read from. Start application adding file name after program name." << std::endl;
        return 1;
    }

    auto fileProcessor = std::make_shared<DistinctWords::FileProcessor>(argv[1]);
    for (int i = 0; i < nthreads; i++)
    {
        auto dws = std::make_shared<DistinctWords::DistinctWordSearcher>(mutexForFileAccess, mutexForMapAccess, distinctWords);
        threadsArray.push_back(std::thread(&DistinctWords::DistinctWordSearcher::run, dws, fileProcessor));
    }

    for (auto &thread : threadsArray)
    {
        thread.join();
    }

    std::cout << distinctWords->size() << std::endl;

    // for(auto word : *distinctWords)
    // {
    //     std::cout<<word.first<<std::endl;
    // }

    return 0;
}
#include <vector>
#include <algorithm>
#include <string>

std::vector<std::string> buildTabs(int numThreads){
    std::vector<std::string> spacesForThreads(numThreads);

    spacesForThreads[0] = "";

    for (int i = 1; i < numThreads; i++) {
        spacesForThreads[i] = spacesForThreads[i - 1] + "\t\t\t\t";
    }

    return spacesForThreads;
}
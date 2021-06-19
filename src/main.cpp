#include <bits/stdc++.h>
#include <sys/stat.h>
#include <cstdio>


/**
 * Format of temporal file: run_(index).txt
 * where index is number from 1
 */
#define RUN_FILE_PREFIX "run_"
#define FILE_EXTENTION ".txt"

using namespace std;


/**
 * MinHeap Node
 *  1. line: one string line in file
 *  2. index: index of sorted run file that contain this line
 */
struct HeapNode {
    string line;
    int index;

    HeapNode(string _line, int _index) {
        line = _line;
        index = _index;
    }
};

/**
 * Customized heap node greater function to create Min Heap 
 * using STL priority queue data structure
 */
struct HeapNodeGreater : public binary_function<HeapNode*, HeapNode*, bool> {
    bool operator() (HeapNode node1, HeapNode node2) const {
        return (node1.line.compare(node2.line)) > 0;
    }
};

/**
 * Helper function to get maximum of RAM usage for a sorted run 
 * that except a mount of memory for stack, caculation
 *      
 */
uint64_t getMaxBufferSize(const uint64_t &RAMSize) {
    return 8 * RAMSize / 10;
}


/**
 * Sorting first and then write each in-memory sorted run into temporal file
 */
void writeSortedRunToFile(vector<string> &run, const string &runFileName) {
    ofstream outFile(runFileName);

    if (run.empty()) {
        return;
    }

    sort(run.begin(), run.end());
    int i = 0;
    for (i = 0; i < run.size() - 1; ++i) {
        outFile << run[i] << endl;
    }
    outFile << run[i];

    outFile.close();
}

/**
 * Devide large input file into small sorted run that fit maximum of buffer size 
 * and return number of sorted run files 
 */
int createSortedRuns(const string &inputFilePath, const uint64_t &maxBufferSize) {
    ifstream inFile(inputFilePath);

    uint64_t bufferSize = 0;
    int numRuns = 0;
    string line;
    vector<string> stringRun;

    while (getline(inFile, line)) {
        if (bufferSize + line.size() >= maxBufferSize) {
            ++numRuns;
            writeSortedRunToFile(stringRun, RUN_FILE_PREFIX + to_string(numRuns) + FILE_EXTENTION);

            bufferSize = 0;
            stringRun.clear();
            stringRun.shrink_to_fit();
        }

        stringRun.push_back(line);
        bufferSize += line.size();
        // plus one for termination character 
        bufferSize += 1;
    }

    // process remain lines in last run
    if (!stringRun.empty()) {
        ++numRuns;
        writeSortedRunToFile(stringRun, RUN_FILE_PREFIX + to_string(numRuns) + FILE_EXTENTION);
    }

    inFile.close();

    return numRuns;
}

/**
 * Merge all sorted run files into one sorted output file
 */
void mergeFiles(const uint64_t &numSortedRuns, const string &outputFilePath) {
    vector<ifstream> sortedRunFiles(numSortedRuns + 1);
    priority_queue<HeapNode, vector<HeapNode>, HeapNodeGreater> minHeap;
    ofstream outFile(outputFilePath);

    for (int i = 1; i <= numSortedRuns; ++i) {
        sortedRunFiles[i].open(RUN_FILE_PREFIX + to_string(i) + FILE_EXTENTION);
        string firstLine;
        getline(sortedRunFiles[i], firstLine);
        minHeap.push(HeapNode(firstLine, i));
    }

    int count = numSortedRuns;
    while (count > 0) {
        outFile << minHeap.top().line << endl;
        int id = minHeap.top().index;
        minHeap.pop();

        if (sortedRunFiles[id].eof()) {
            --count;
            sortedRunFiles[id].close();
        } else {
            string nextLine;
            getline(sortedRunFiles[id], nextLine);
            minHeap.push(HeapNode(nextLine, id));
        }
    }

    outFile.close();
}

/**
 * Clear temporal run files after sorting done
 */
void clearRunFiles(int numSortedRuns) {
    for (int i = 1; i <= numSortedRuns; ++i) {
        string fileName = RUN_FILE_PREFIX + to_string(i) + FILE_EXTENTION;
        remove(fileName.c_str());
    }
}

/**
 * External Sort:
 * Pass 1: Create sorted runs
 * Pass 2: Merge sorted runs into output file 
 */
void externalSort(const string &inputFilePath, const string &outputFilePath, const uint64_t &RAMsize) {
    uint64_t maxBufferSize = getMaxBufferSize(RAMsize);
    int numSortedRuns = createSortedRuns(inputFilePath, maxBufferSize);
    cout << "Pass 1 done!" << endl;
    
    mergeFiles(numSortedRuns, outputFilePath);
    cout << "Pass 2 done!" << endl;
    
    clearRunFiles(numSortedRuns);
}

/**
 * Main function
 * argc = 3
 * argv[0] : input file path
 * argv[1] : output file path (after sort)
 * argv[2] : limit of RAM
 */
int main(int argc, char **argv) {
    if (argc < 4) {
        cout << "Wrong input format!" << endl;
        return 0;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];
    uint64_t RAMsize = stoull(argv[3], nullptr, 0);

    time_t start, finish;
    time(&start);

    // Invoke external sort function
    externalSort(inputFilePath, outputFilePath, RAMsize);

    time(&finish);
    cout << "Sorting done in " << difftime(finish, start) << " seconds \n" ;
    return 0;
}

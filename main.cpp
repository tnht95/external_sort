#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;
const clock_t begin_time = clock();

struct node {
    string sentence;
    int index;

    node() {};

    node(string a, int b) : sentence(move(a)), index(b) {};

    bool operator<(const node &other) const {
        return (sentence.compare(other.sentence) > 0);
    }
};

void writeToDisk(const string &outputFileName, const vector<string> &data) {
    ofstream outputStream;
    outputStream.open(outputFileName);

    if (!outputStream) {
        cout << "Can't open output file " << outputFileName << '\n';
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < data.size() - 1; ++i) {
        outputStream << data[i] << '\n';
    }
    outputStream << data[data.size() - 1];
    outputStream.close();
}


int inputHandle(const string &inputFileName, long long availableMemory) {
    ifstream inputStream;
    inputStream.open(inputFileName);

    if (!inputStream) {
        cout << "Can't open input file\n";
        exit(EXIT_FAILURE);
    }

    inputStream.seekg(0, inputStream.end);
    long long inputSize = inputStream.tellg();
    inputStream.seekg(0, inputStream.beg);

    vector<string> data;
    cout << "File " << inputFileName << " with size " << inputSize << " bytes is being read\n";

    int outputFilesCount{0};
    long long currentUsedMemory{0};
    while (!inputStream.eof()) {
        string sentence;
        getline(inputStream, sentence);
        if (sentence.size() + currentUsedMemory < availableMemory) {
            data.push_back(sentence);
            currentUsedMemory += sentence.size();
        } else {
            sort(data.begin(), data.end());
            ++outputFilesCount;
            writeToDisk("Output_file_" + to_string(outputFilesCount), data);
            data.clear();
            currentUsedMemory = sentence.size();
            data.push_back(sentence);
        }
    }

    //handle leftover data if exist
    if (!data.empty()) {
        sort(data.begin(), data.end());
        ++outputFilesCount;

        writeToDisk("Output_file_" + to_string(outputFilesCount), data);
    }

    cout << "Input reading is done!\n";
    return outputFilesCount;
}

void deleteRedundantOutputFiles(int totalFiles) {
    cout << "-------------------------------------------------------\n";
    cout << "Removing output files!" << endl;
    for (int i = 1; i <= totalFiles; ++i) {
        string file{"Output_file_" + to_string(i)};
        cout << "Removing Output_file_" << i << '\n';
        remove(file.c_str());
    }
    cout << "-------------------------------------------------------\n\n\n";
}


void kWayMerge(int totalFiles, const string &outputFileName, long long availableMemory) {
    cout << "Merging " << totalFiles << " into " << outputFileName << '\n';
    ifstream inputStream[totalFiles + 1];
    for (int i = 1; i <= totalFiles; ++i) {
        inputStream[i].open("Output_file_" + to_string(i));
        if (!inputStream[i]) {
            cout << "Can't open file Output_file_" << i << '\n';
        }
    }

    ofstream outputStream;
    outputStream.open(outputFileName);

    if (!outputStream) {
        cout << "Can't open output file " << outputFileName << '\n';
    }

    priority_queue<node, vector<node> > minHeap;
    for (int i = 1; i <= totalFiles; ++i) {
        string sentence;
        getline(inputStream[i], sentence);
        minHeap.push(node{sentence, i});
    }

    //numbers of input files and a output file
    long long sizeOfEachPart{availableMemory / (totalFiles + 1)};
    long long curUsedMemory{0};
    vector<string> data;
    node cur;
    while (!minHeap.empty()) {
        cur = minHeap.top();
        minHeap.pop();
        if (curUsedMemory + cur.sentence.size() >= sizeOfEachPart) {
            for (int i = 0; i < data.size(); ++i) {
                outputStream << data[i] << '\n';
            }
            outputStream.flush();
            data.clear();
            curUsedMemory = cur.sentence.size();
        }
        if (!inputStream[cur.index].eof()) {
            string sentence;
            getline(inputStream[cur.index], sentence);
            minHeap.push(node{sentence, cur.index});
        }
        data.push_back(cur.sentence);
    }

    if (!data.empty()) {
        for (int i = 0; i < data.size(); ++i) {
            outputStream << data[i] << '\n';
        }
    }

    for (int i = 1; i <= totalFiles; ++i) {
        inputStream[i].close();
    }

    outputStream.close();
    deleteRedundantOutputFiles(totalFiles);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "Please provide input file name, output file name and memory\n";
        return 0;
    }

    string inputFileName{argv[1]};
    string outputFileName{argv[2]};
    //memory in bytes
    long long availableMemory{strtoll(argv[3], nullptr, 10)};

    int totalOutputFiles{inputHandle(inputFileName, availableMemory)};

    kWayMerge(totalOutputFiles, outputFileName, availableMemory);
    cout << "DONE!\n";
    cout << "Executing time:  " << float(clock() - begin_time) / CLOCKS_PER_SEC * 1000 << " msec." << endl;
    return 0;
}
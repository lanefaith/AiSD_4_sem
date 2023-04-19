#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include <chrono>

using namespace std;

//AH

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
};

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Creating a frequency table for characters in a string
unordered_map<char, int> buildFrequencyTable(string& inputString) {
    unordered_map<char, int> freqTable;
    for (char ch : inputString) {
        freqTable[ch]++;
    }
    return freqTable;
}

// Creating a Huffman tree based on a frequency table
Node* buildHuffmanTree(unordered_map<char, int>& freqTable) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& pair : freqTable) {
        pq.push(new Node{ pair.first, pair.second, nullptr, nullptr });
    }
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node{ '\0', left->freq + right->freq, left, right };
        pq.push(parent);
    }
    return pq.top();
}

// Creating a code table for characters based on a Huffman tree
void buildCodeTableHelper(Node* root, string& code, unordered_map<char, string>& codeTable) {
    if (root == nullptr) return;
    if (root->ch != '\0') {
        codeTable[root->ch] = code;
    }
    code.push_back('0');
    buildCodeTableHelper(root->left, code, codeTable);
    code.pop_back();
    code.push_back('1');
    buildCodeTableHelper(root->right, code, codeTable);
    code.pop_back();
}

unordered_map<char, string> buildCodeTable(Node* root) {
    unordered_map<char, string> codeTable;
    string code;
    buildCodeTableHelper(root, code, codeTable);
    return codeTable;
}

// Encoding a string based on a table of character codes
string encodeString(string& inputString, unordered_map<char, string>& codeTable) {
    string encodedString;
    for (char ch : inputString) {
        encodedString += codeTable[ch];
    }
    return encodedString;
}

// A function for printing a dictionary to the console.
void printCodeTable(unordered_map<char, string>& codeTable) {
    cout << "Code table:" << endl;
    for (auto& pair : codeTable) {
        cout << "'" << pair.first << "': " << pair.second << endl;
    }
}

//RLE

string compress_RLE(string data) {
    string compressed_data;
    int count = 1;
    for (int i = 1; i <= data.length(); i++) {
        if (data[i] != data[i - 1]) {
            compressed_data += to_string(count);
            compressed_data += data[i - 1];
            count = 1;
        }
        else {
            count++;
        }
    }
    return compressed_data;
}

//LZ78

vector<pair<int, char>> lz78_compress(string input) {
    map<string, int> dictionary;
    vector<pair<int, char>> output;
    string current = "";
    int code = 0;

    for (char c : input) {
        string next = current + c;
        if (dictionary.count(next)) {
            current = next;
        }
        else {
            output.push_back(make_pair(dictionary[current], c));
            dictionary[next] = ++code;
            current = "";
        }
    }

    if (current != "") {
        output.push_back(make_pair(dictionary[current], ' '));
    }

    return output;
}

//BWT

string BWT(const string& input) {
    string output;

    // Creating a vector of all possible cyclic shifts of a string
    vector<string> rotations;
    for (int i = 0; i < input.length(); i++) {
        rotations.push_back(input.substr(i) + input.substr(0, i));
    }

    // Sorting the vector of cyclic shifts
    sort(rotations.begin(), rotations.end());

    // Obtaining the last characters of each cyclic shift
    for (int i = 0; i < rotations.size(); i++) {
        output += rotations[i].back();
    }

    return output;
}


//MTF
vector<int> mtf_encode(const string& s) {
    vector<int> v(1000);
    for (int i = 0; i < 256; i++) {
        v[i] = i;
    }
    vector<int> res;
    for (char c : s) {
        int idx = find(v.begin(), v.end(), c) - v.begin();
        res.push_back(idx);
        v.erase(v.begin() + idx);
        v.insert(v.begin(), c);
    }
    return res;
}

// AC

// A character structure that contains its frequency and probability.
struct Symbol {
    char character;
    int frequency;
    double probability;
};

// A function for counting the frequency of characters
map<char, int> get_frequency(string data) {
    map<char, int> freq;
    for (int i = 0; i < data.length(); i++) {
        freq[data[i]]++;
    }
    return freq;
}

// A function for obtaining the probability of each character
vector<Symbol> get_probability(map<char, int> freq, int length) {
    vector<Symbol> symbols;
    for (auto f : freq) {
        Symbol s;
        s.character = f.first;
        s.frequency = f.second;
        s.probability = (double)f.second / length;
        symbols.push_back(s);
    }
    return symbols;
}

// A function for arithmetic encoding
string arithmetic_encoding(string data) {
    vector<Symbol> symbols = get_probability(get_frequency(data), data.length());
    double start = 0, end = 1, range;
    for (int i = 0; i < data.length(); i++) {
        for (auto s : symbols) {
            if (s.character == data[i]) {
                range = end - start;
                end = start + range * (s.probability + s.frequency - 1);
                start = start + range * s.probability;
                break;
            }
        }
    }
    return to_string(start);
}


//PPM
const int MAX_ORDER = 4; // Maximum order of the model
const int CONTEXT_SIZE = MAX_ORDER + 1; 

map<string, int> charCounts; // Character frequencies
map<string, int> contextCounts[CONTEXT_SIZE]; // Context frequencies

void updateContext(vector<string>& context, string nextChar)
{
    context.erase(context.begin()); // Removing the first character from the context
    context.push_back(nextChar); // Adding a new character to the context
}

void updateCounts(vector<string>& context, string nextChar)
{
    charCounts[nextChar]++; // Increasing the character counter.

    for (int i = 0; i < CONTEXT_SIZE; i++) {
        if (i == context.size()) {
            break;
        }

        string contextStr;
        for (int j = 0; j < i; j++) {
            contextStr += context[context.size() - i + j];
        }
        contextStr += nextChar;

        contextCounts[i][contextStr]++; // Increasing the context counter.
    }
}

void compress(ofstream& fout)
{
    fstream fin("input.txt");
    vector<string> context(MAX_ORDER, "");
    string output = "";
    string line;

    // Считываем строки из файла, пока не достигнем конца файла
    while (getline(fin, line)) {
        string output = "";
        // Используем итератор для чтения строки посимвольно
        for (char it : line) {
            string nextChar = "";
            nextChar += it;
            int order = MAX_ORDER;
            while (order >= 0) {
                string contextStr;
                for (int j = 0; j < order; j++) {
                    contextStr += context[context.size() - order + j];
                }
                contextStr += nextChar;

                if (contextCounts[order][contextStr] > 0) {
                    // A matching context has been found
                    output += to_string(contextCounts[order][contextStr]);
                    output += contextStr.substr(order, 1);
                    updateContext(context, nextChar);
                    updateCounts(context, nextChar);
                    break;
                }
                else {
                    order--;
                }
            }

            if (order < 0) {
                // No matching context found
                output += "0";
                output += nextChar;
                updateContext(context, nextChar);
                updateCounts(context, nextChar);
            }
        }
        fout << output << " ";
    }
    fin.close();
}

int main() {
    int n = -1;
    while (n != 0)
    {
        cout << "Choose an algorithm:\n1.Huffman algorithm\n2.RLE\n3.LZ78\n4.BWT\n5.MTF\n6.AC\n7.PPM\nTo exit the program, press 0.\n";
        cin >> n;
        if (n == 1) //AH
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ifstream inputFilename("input.txt");
            ofstream outputEncodedFilename("output_ah.txt");
            string inputString;
            // Reading a string from a file
            if (inputFilename.is_open()) {
                while (getline(inputFilename, inputString)) {
                    // Creating a frequency table for characters in a string
                    unordered_map<char, int> freqTable = buildFrequencyTable(inputString);

                    // Creating a Huffman tree based on a frequency table
                    Node* root = buildHuffmanTree(freqTable);

                    // Creating a code table for characters based on a Huffman tree.
                    unordered_map<char, string> codeTable = buildCodeTable(root);

                    // Printing the code table to the console
                    //printCodeTable(codeTable);

                    // Encoding a string based on a table of character codes
                    string encodedString = encodeString(inputString, codeTable);

                    // Writing the encoded string to a file with characters
                    outputEncodedFilename << encodedString << " ";
                    delete root;

                }
                inputFilename.close();
            }
            outputEncodedFilename.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 2) //RLE
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ifstream file("input.txt");
            ofstream output_file("output_RLE.txt");
            if (file.is_open()) {
                string data;
                if (file.is_open()) {
                    while (getline(file, data)) {
                        if (output_file.is_open()) {
                            // Data compression.
                            string compressed_data = compress_RLE(data);
                            output_file << compressed_data;
                        }
                        else {
                            cout << "Unable to open output file" << endl;
                        }
                    }
                    file.close();
                }
                output_file.close();
            }
            else {
                cout << "Unable to open input file" << endl;
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 3) //LZ78
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ifstream inFile("input.txt");
            ofstream outFile("output_lz78.txt");
            if (!inFile) {
                cerr << "Error: cannot open input file" << endl;
                return 1;
            }

            // Read the original string from a file.
            string input;
            if (inFile.is_open()) {
                while (getline(inFile, input)) {
                    vector<pair<int, char>> compressed = lz78_compress(input);
                    // Compress the source string and write it to the file compressed.txt.
                    if (!outFile) {
                        cerr << "Error: cannot open output file" << endl;
                        return 1;
                    }

                    for (auto p : compressed) {
                        outFile << p.first << "," << p.second << " ";
                    }
                }
                inFile.close();
            }
            outFile.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 4) //BWT
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ifstream fin("input.txt");
            ofstream fout("output_bwt.txt");
            string input;
            if (fin.is_open()) {
                while (getline(fin, input)) {
                    string bwt = BWT(input);
                    fout << bwt << " ";
                }
                fin.close();
            }
            fout.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 5) //MTF
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ifstream fin("input.txt");
            ofstream fout("output_mtf.txt");
            string input;
            if (fin.is_open()) {
                while (getline(fin, input)) {
                    vector<int> encoded = mtf_encode(input);
                    for (int x : encoded) {
                        fout << x << " ";
                    }
                }
                fin.close();
            }
            fout.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 6) //AC
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            fstream fin("input.txt");
            ofstream fout("output_ac.txt");
            string input;
            if (fin.is_open()) {
                while (getline(fin, input)) {
                    string encoded_ac = arithmetic_encoding(input);
                    fout << encoded_ac << " ";
                }
                fin.close();
            }
            fout.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (n == 7) //PPM
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            ofstream fout("output_ppm.txt");
            compress(fout);
            fout.close();
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
    }
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <regex>
#include <cctype>

struct ProcessedFile {
    std::vector<std::string> lines;
    std::mutex mtx;
};

bool splitFlightCode(const std::string& code, std::string& airline, std::string& number) {
    if (code.size() > 7) return false;

    std::smatch matches;
    std::regex pattern(R"(^([A-Za-z]{1,2}[A-Za-z0-9]?)(?:\s*)(\d*)$)");
    
    if (std::regex_match(code, matches, pattern)) {
        airline = matches[1].str();
        number = matches[2].str();
        
        number.erase(0, number.find_first_not_of('0'));
        if (number.empty() && !matches[2].str().empty()) number = "0";
        
        std::transform(airline.begin(), airline.end(), airline.begin(), ::toupper);
        return true;
    }
    
    number = code;
    number.erase(0, number.find_first_not_of('0'));
    if (number.empty() && !code.empty()) number = "0";
    
    airline = "";
    return true;
}

void processFile(const std::string& inputFile, ProcessedFile& result) {
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error opening file: " << inputFile << std::endl;
        return;
    }

    std::vector<std::string> localLines;
    std::string line;

    while (std::getline(inFile, line)) {
        std::string airline, number;
        if (splitFlightCode(line, airline, number)) {
            std::string normalized;
            if (!airline.empty()) {
                normalized += airline;
                if (!number.empty()) {
                    normalized += " ";
                }
            }
            normalized += number;

            if (std::find(localLines.begin(), localLines.end(), normalized) == localLines.end()) {
                localLines.push_back(normalized);
            }
        }
    }
    inFile.close();

    std::lock_guard<std::mutex> lock(result.mtx);
    for (const auto& line : localLines) {
        if (std::find(result.lines.begin(), result.lines.end(), line) == result.lines.end()) {
            result.lines.push_back(line);
        }
    }
}

void saveToFile(const std::vector<std::string>& lines, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    for (const auto& line : lines) {
        outFile << line << "\n";
    }
    outFile.close();
}

int main() {
    ProcessedFile result1, result2;

    std::thread t1(processFile, "1_in.txt", std::ref(result1));
    std::thread t2(processFile, "2_in_txt", std::ref(result2));

    t1.join();
    t2.join();

    saveToFile(result1.lines, "1_out.txt");
    saveToFile(result2.lines, "2_out.txt");

    std::cout << "Deduplication completed. Results saved to 1_out.txt and 2_out.txt" << std::endl;
    return 0;
}

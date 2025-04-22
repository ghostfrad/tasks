#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <cctype>

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

bool compareFlightCodes(const std::string& code1, const std::string& code2) {
    std::string airline1, number1;
    std::string airline2, number2;

    if (!splitFlightCode(code1, airline1, number1)) return false;
    if (!splitFlightCode(code2, airline2, number2)) return false;

    if (number1.size() < 1 || number1.size() > 5 || 
        number2.size() < 1 || number2.size() > 5) {
        return false;
    }

    return airline1 == airline2 && number1 == number2;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <flight_code1> <flight_code2>" << std::endl;
        return 1;
    }

    bool result = compareFlightCodes(argv[1], argv[2]);
    std::cout << (result ? "equal" : "not equal") << std::endl;

    return 0;
}

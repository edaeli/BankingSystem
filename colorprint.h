#ifndef COLORPRINT_H
#define COLORPRINT_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

class Painter {
    std::ostream& out;
    std::vector<std::string> green_words;
    std::vector<std::string> red_words;

public:
    Painter(std::ostream& os, std::vector<std::string> g, std::vector<std::string> r) 
        : out(os), green_words(g), red_words(r) {}

    void print(std::string text) {
	for (const auto& word : green_words) {
            std::regex reg("\\b" + word + "\\b");
            text = std::regex_replace(text, reg, "\033[32m" + word + "\033[0m");
        }
        
        for (const auto& word : red_words) {
            std::regex reg("\\b" + word + "\\b");
            text = std::regex_replace(text, reg, "\033[31m" + word + "\033[0m");
        }
        
        out << text;
    }
};

#endif

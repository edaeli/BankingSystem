#ifndef COLORPRINT_H
#define COLORPRINT_H

#include <iostream>
#include <string>
#include <vector>

class Painter {
    std::ostream& out;
    std::vector<std::string> green_words;
    std::vector<std::string> red_words;

public:
    Painter(std::ostream& os, std::vector<std::string> g, std::vector<std::string> r) 
        : out(os), green_words(g), red_words(r) {}

    void print(std::string text) {
        for (const auto& word : green_words) {
            size_t pos = 0;
            while ((pos = text.find(word, pos)) != std::string::npos) {
                text.replace(pos, word.length(), "\033[32m" + word + "\033[0m");
                pos += 13; 
            }
        }
        for (const auto& word : red_words) {
            size_t pos = 0;
            while ((pos = text.find(word, pos)) != std::string::npos) {
                text.replace(pos, word.length(), "\033[31m" + word + "\033[0m");
                pos += 13;
            }
        }
        out << text;
    }
};

#endif

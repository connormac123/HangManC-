#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include <set>
#include <unordered_map>



class Game {
public:
    virtual void play() = 0;
};

class Hangman : public Game {
private:
    std::string word;
    int lives;
    std::set<char> guessedLetters;
    std::string hangmanBody;

    std::unordered_map<int, std::string> hangmanBodies = {
            {1, " _______\n |       |\n |       \n_|______________\n"},
            {2, " _______\n |       |\n |       O\n_|______________\n"},
            {3, " _______\n |       |\n |      /O\n_|______________\n"},
            {4, " _______\n |       |\n |      /|O\n_|______________\n"},
            {5, " _______\n |       |\n |      /|\\O\n_|______________\n"},
            {6, " _______\n |       |\n |      /|\\O\n |       |\n_|______________\n"}
    };

public:
    Hangman(std::string word) : word(std::move(word)), lives(6), hangmanBody("") {}

    void updateHangmanBody(int position) {
        hangmanBody = hangmanBodies[position];
    }

    void play() override {
        std::cout << "Welcome to Hangman!" << std::endl;
        std::cout << "You have " << lives << " lives." << std::endl;

        int position = 0;
        while (lives > 0) {
            std::cout << "Guess a letter: ";
            char letter;
            std::cin >> letter;

            if (guessedLetters.find(letter)!= guessedLetters.end()) {
                std::cout << "You already guessed this letter!" << std::endl;
                continue;
            }

            guessedLetters.insert(letter);

            bool correct = false;
            for (char c : word) {
                if (c == letter) {
                    correct = true;
                }
            }

            if (!correct) {
                lives--;
                position++;
                updateHangmanBody(position);
                std::cout << "Incorrect! You have " << lives << " lives left." << std::endl;
                std::cout << "Hangman body: " << hangmanBody << std::endl;
            } else {
                std::cout << "Correct!" << std::endl;
            }

            // Display the current state of the hangman
            std::cout << ":";
            for (char c : word) {
                if (guessedLetters.find(c)!= guessedLetters.end()) {
                    std::cout << c << " ";
                } else {
                    std::cout << "_ ";
                }
            }
            std::cout << std::endl;

            // Display all the guessed letters
            std::cout << "Guessed letters: ";
            for (char c : guessedLetters) {
                std::cout << c << " ";
            }
            std::cout << std::endl;

            bool won = true;
            for (char c : word) {
                if (guessedLetters.find(c) == guessedLetters.end()) {
                    won = false;
                    break;
                }
            }

            if (won) {
                std::cout << "Congratulations, you won! The word was " << word << "." << std::endl;
                return;
            }
        }

        std::cout << "Game over! The word was " << word << "." << std::endl;
    }
};

std::string getRandomWord(const std::vector<std::string>& words) {
    if (words.empty()) {
        throw std::runtime_error("Error: cannot get random word from empty vector.");
    }
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, words.size() - 1);
    return words[dist(mt)];
}

int main() {
    std::string filePath = R"(C:\Users\conno\CLionProjects\HangManGame\words.txt)";
    std::vector<std::string> words = loadWords(filePath);
    if (words.empty()) {
        std::cerr << "Error: unable to load words from file." << std::endl;
        return 1;
    }
    std::string randomWord = getRandomWord(words); // Corrected here
    Hangman game(randomWord);
    game.play();
    return 0;
}


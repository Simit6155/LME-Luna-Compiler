

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <cctype>

enum class TokenType {
    _exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens;
    std::string buf;
    size_t i = 0;

    while (i < str.length()) {
        char c = str.at(i);

        if (std::isalpha(c)) {
            buf.push_back(c);
            i++;

            while (i < str.length() && std::isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
        }

        if (buf == "cik") {
            tokens.push_back({TokenType::_exit, std::nullopt});
            buf.clear();
            continue;
        } else if (!buf.empty()) {
            std::cerr << "ERROR! Unknown keyword: " << buf << std::endl;
            exit(EXIT_FAILURE);
        } else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;

            while (i < str.length() && std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }

            tokens.push_back({TokenType::int_lit, buf});
            buf.clear();
            continue;
        } else if (c == ';') {
            tokens.push_back({TokenType::semi, std::nullopt});
        } else if (std::isspace(c)) {
            i++;
            continue;
        } else {
            std::cerr << "ERROR! Unknown character: " << c << std::endl;
            exit(EXIT_FAILURE);
        }

        i++;
    }

    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;

    output << "global _start\n";
    output << "_start:\n";

    for (size_t i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);

        if (token.type == TokenType::_exit) {

            // FIX: We need to check i + 2 because we access i + 2 below.
            if (i + 2 < tokens.size() &&
                tokens.at(i + 1).type == TokenType::int_lit &&
                tokens.at(i + 2).type == TokenType::semi) {

                output << "    mov rax, 60\n";
                output << "    mov rdi, "
                       << tokens.at(i + 1).value.value() << "\n";
                output << "    syscall\n";
            }
        }
    }

    return output.str();
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct usage is... " << std::endl;
        std::cerr << "Luna <input.lme>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;

    {
        std::stringstream contents_stream;

        std::fstream input(argv[1], std::ios::in);

        // Check if the file actually opened
        if (!input) {
            std::cerr << "Could not open input file: "
                      << argv[1] << std::endl;
            return EXIT_FAILURE;
        }

        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::vector<Token> tokens = tokenize(contents);

    {
        std::fstream file("out.asm", std::ios::out);

        if (!file) {
            std::cerr << "Could not create out.asm" << std::endl;
            return EXIT_FAILURE;
        }

        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}

/*
variable	degişken
function	fonksiyon
if	eğer
else	degilse
while	iken
for	icin
true	dogru
false	yanlis
string literal	metin
char literal	karakter
operator	islemci
print / output	yazdir
return getir
 */




#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <cctype>

enum class TokenType {
    _return,
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

        if (buf == "getir") {
            tokens.push_back({TokenType::_return, std::nullopt});
            buf.clear();
            continue;
        } else if (!buf.empty()) {
            std::cerr << "Yanlislik yaptin! " << std::endl;
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
            std::cerr << "Yanlislik yaptin! " << std::endl;
            exit(EXIT_FAILURE);
        }
        i++;
    }

    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 1 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Yanlis kullanis. Dogru kullanis ..." << std::endl;
        std::cerr << "luna <input.lme>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::vector<Token> tokens = tokenize(contents);

    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    tokenize(contents);
    return EXIT_SUCCESS;
}

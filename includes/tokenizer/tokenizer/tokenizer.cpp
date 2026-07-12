#include "tokenizer.h"

#include <optional>
#include <stdexcept>
#include <utility>

namespace {

struct Lexeme {
    string text;
    int type;
};

int expected_commas(const string& function_name) {
    return function_name == "min" || function_name == "max" ? 1 : 0;
}

bool valid_sequence(const vector<Lexeme>& lexemes) {
    if (lexemes.empty()) return false;

    bool expect_operand = true;
    optional<int> pending_function;
    vector<pair<int, int>> frames;

    for (std::size_t index = 0; index < lexemes.size(); ++index) {
        const Lexeme& lexeme = lexemes[index];

        if (lexeme.type == FUNCTION) {
            if (!expect_operand || pending_function.has_value()) return false;
            if (index + 1 >= lexemes.size() || lexemes[index + 1].type != LPAREN) {
                return false;
            }
            pending_function = expected_commas(lexeme.text);
            continue;
        }

        if (lexeme.type == LPAREN) {
            if (!expect_operand) return false;
            frames.push_back({pending_function.value_or(-1), 0});
            pending_function.reset();
            expect_operand = true;
            continue;
        }

        if (lexeme.type == INTEGER || lexeme.type == VAR) {
            if (!expect_operand || pending_function.has_value()) return false;
            expect_operand = false;
            continue;
        }

        if (lexeme.type == OPERATOR) {
            if (lexeme.text == "~") {
                if (!expect_operand) return false;
                continue;
            }
            if (expect_operand) return false;
            expect_operand = true;
            continue;
        }

        if (lexeme.type == TOKEN) {
            if (expect_operand || frames.empty() || frames.back().first < 0) {
                return false;
            }
            frames.back().second++;
            expect_operand = true;
            continue;
        }

        if (lexeme.type == RPAREN) {
            if (expect_operand || frames.empty()) return false;
            const auto frame = frames.back();
            frames.pop_back();
            if (frame.first >= 0 && frame.first != frame.second) return false;
            expect_operand = false;
            continue;
        }

        return false;
    }

    return !expect_operand && frames.empty() && !pending_function.has_value();
}

} // namespace

void Tokenizer::tokenize(const string& input) {
    _input = input;
    _tokens.clear();
    _types.clear();
    _output.clear();
    _owned_tokens.clear();
    _types = _tokenize(input, _tokens);
}

Tokenizer::Tokenizer(const string& input) {
    tokenize(input);
}

void Tokenizer::set_input(const string& input) {
    tokenize(input);
}

Queue<Token*> Tokenizer::infix() {
    if (!_output.empty()) return _output;

    vector<Lexeme> lexemes;
    while (!_tokens.empty() && !_types.empty()) {
        lexemes.push_back({_tokens.pop(), _types.pop()});
    }
    if (!_tokens.empty() || !_types.empty() || !valid_sequence(lexemes)) {
        _tokens.clear();
        _types.clear();
        return {};
    }

    const auto own = [this](unique_ptr<Token> token) {
        Token* pointer = token.get();
        _owned_tokens.push_back(std::move(token));
        _output.push(pointer);
    };

    for (const auto& lexeme : lexemes) {
        if (lexeme.type == INTEGER) {
            std::size_t consumed = 0;
            const double value = stod(lexeme.text, &consumed);
            if (consumed != lexeme.text.size()) {
                throw invalid_argument("Number contains trailing characters");
            }
            own(make_unique<Integer>(value));
        } else if (lexeme.type == VAR) {
            own(make_unique<Variable>());
        } else if (lexeme.type == FUNCTION) {
            own(make_unique<Function>(lexeme.text));
        } else if (lexeme.type == LPAREN) {
            own(make_unique<LeftParen>());
        } else if (lexeme.type == RPAREN) {
            own(make_unique<RightParen>());
        } else if (lexeme.type == OPERATOR) {
            own(make_unique<Operator>(lexeme.text));
        } else if (lexeme.type == TOKEN) {
            own(make_unique<Comma>());
        }
    }

    return _output;
}

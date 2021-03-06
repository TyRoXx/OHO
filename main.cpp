#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

namespace oho {
struct interpreter {
  std::map<std::string, std::string> variables;

  std::string next_token(char const *&begin, char const *const end) {
    while ((begin != end) &&
           (std::isspace(*begin) || (*begin == '(') || (*begin == ')'))) {
      ++begin;
    }
    if (begin == end) {
      return "";
    }
    if (std::isalnum(*begin) || (*begin == '~')) {
      char const *const begin_of_id = begin;
      ++begin;
      while (begin != end && (std::isalnum(*begin) || (*begin == '~'))) {
        ++begin;
      }
      return std::string(begin_of_id, begin);
    }
    if (*begin == '"') {
      char const *const begin_of_string = begin;
      ++begin;
      char const *const end_of_string = std::find(begin, end, '"');
      begin = end_of_string;
      if (begin != end) {
        ++begin;
      }
      return std::string(begin_of_string, end_of_string);
    }
    ++begin;
    if (begin != end && *begin == '=') {
      ++begin;
      return std::string(begin - 2, begin);
    }
    return std::string(begin - 1, begin);
  }

  std::string evaluate_atom(char const *&begin, char const *const end) {
    std::string const token = next_token(begin, end);
    if (token.empty()) {
      return token;
    }
    if (token[0] == '"') {
      return token.substr(1, token.size() - 1);
    }
    auto found = variables.find(token);
    if (found != variables.end()) {
      return found->second;
    }
    return token;
  }

  std::string equals(std::string const &left, std::string const &right) {
    if (left.size() == right.size()) {
      return "true";
    }
    return "false";
  }

  std::string add(std::string const &left, std::string const &right) {
    return std::to_string(std::stoi(left) + std::stoi(right));
  }

  std::string less(std::string const &left, std::string const &right) {
    if (left.size() < right.size()) {
      return "true";
    }
    return (left < right) ? "true" : "false";
  }

  std::string evaluate_expression(char const *&begin, char const *const end) {
    std::string left = evaluate_atom(begin, end);
    if (left == "!") {
      std::string operand = evaluate_expression(begin, end);
      if (operand == "true") {
        return "false";
      }
      return "true";
    }
    char const *const before_operator = begin;
    std::string potential_operator = next_token(begin, end);
    if (potential_operator == "==") {
      std::string right = evaluate_expression(begin, end);
      return equals(left, right);
    } else if (potential_operator == "+") {
      std::string right = evaluate_expression(begin, end);
      return add(left, right);
    } else if (potential_operator == "&") {
      std::string right = evaluate_expression(begin, end);
      return (left + right);
    } else if (potential_operator == "<") {
      std::string right = evaluate_expression(begin, end);
      return less(left, right);
    } else {
      begin = before_operator;
      return left;
    }
  }

  void run_while(char const *&begin, char const *const end) {
    char const *const condition_at = begin;
    for (;;) {
      std::string condition = evaluate_expression(begin, end);
      std::string colon = next_token(begin, end);
      if (condition != "true") {
        begin = std::find(begin, end, '\n');
        return;
      }
      run_line(begin, end);
      begin = condition_at;
    }
  }

  void run_line(char const *&begin, char const *end) {
    end = std::find(begin, end, '\n');
    for (;;) {
      std::string token = next_token(begin, end);
      std::transform(token.begin(), token.end(), token.begin(),
                     [](char c) { return std::tolower(c); });
      if (token == "") {
        return;
      }
      if (token == "print") {
        std::string message = evaluate_expression(begin, end);
        std::cout << message << std::endl;
      } else if (token == "if") {
        std::string condition = evaluate_expression(begin, end);
        std::string colon = next_token(begin, end);
        if (condition != "true") {
          begin = std::find(begin, end, '\n');
        }
      } else if (token == "while") {
        run_while(begin, end);
      } else {
        std::string const next = next_token(begin, end);
        if (next == "=") {
          variables[token] = evaluate_expression(begin, end);
        }
      }
    }
  }

  void run(char const *begin, char const *const end) {
    while (begin != end) {
      char const *const before = begin;
      run_line(begin, end);
      if (before == begin) {
        ++begin;
      }
    }
  }
};
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    return 1;
  }
  std::string source{std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>()};
  if (!file) {
    return 1;
  }
  oho::interpreter i;
  i.run(source.data(), source.data() + source.size());
}

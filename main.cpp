#include <fstream>
#include <iostream>
#include <algorithm>

namespace oho {
std::string next_token(char const *&begin, char const *const end) {
  while ((begin != end) && std::isspace(*begin)) {
    ++begin;
  }
  if (begin == end) {
    return "";
  }
  if (std::isalnum(*begin)) {
    char const *const begin_of_id = begin;
    ++begin;
    while (begin != end && std::isalnum(*begin)) {
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
  return std::string(begin - 1, begin);
}

std::string evaluate_expression(char const *&begin, char const *const end) {
  std::string const token = next_token(begin, end);
  if (token.empty()) {
    return token;
  }
  if (token[0] == '"') {
    return token.substr(1, token.size() - 1);
  }
  return token;
}

void run(char const *begin, char const *const end) {
  for (;;) {
    std::string const token = next_token(begin, end);

    if (token == "") {
      break;
    }
    if (token == "print") {
      std::string message = evaluate_expression(begin, end);
      std::cout << message << std::endl;
    }
  }
}
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
  oho::run(source.data(), source.data() + source.size());
}

#include <string>
#include <iostream>
#include "Thing.h"
#include "Explainer.h"
#include "grammar.h"

bool parse(const std::string &line)
{
    classify::state s;
    tao::pegtl::string_input<> input(line, "input");
    auto b = tao::pegtl::parse<classify::grammar::grammar,
                               classify::action,
                               tao::pegtl::normal>(input, s);
    if (b) {
        return s._again;
    } else {
        std::cout << "I don't understand you, try again." << std::endl;
        return true;
    }
}

int main()
{
    bool again = true;
    classify::help();
    do {
      std::string line;
      std::cout << "> ";
      getline(std::cin, line);
      again = parse(line);
    } while (again);
    return 0;
}

using namespace std;

#include <string>
#include <iostream>

int main(void) {
    string input;

    while (1) {
        std::cin >> input;
        if (input == "uci") {
            std::cout << "id name a\n";
            std::cout << "id author a\n";
            std::cout << "uciok\n";
        }
    }

    return 0;
}
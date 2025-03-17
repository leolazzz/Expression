#include "expression.hpp"
#include <iostream>
#include <string>
#include <cstring>

bool eval(int argc, char* argv[]){
    Expression<long double> x;
    std::string s = argv[2];
    try { 
        x = Expression<long double>(s);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::map <std::string, long double> context;
    for (int i = 3; i < argc; ++i) {
        std::string str = argv[i];
        bool fl = 0;
        for(int j = 0; str.size(); ++j){
            if(str[j] == '='){
                fl = 1;
                context[str.substr(0, j)] = std::stod(str.substr(j+1));
                break;
            }
        }
        if(!fl){
            std::cout << "Correct command: differentiator eval <expression>  <variable1>=<value1> <variable2>=<value2> ........\n";
            return 1;
        }
    }
    long double ans;
    try {
        ans = x.eval(context);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << ans << '\n';
    return 0;
}

bool diff(int argc, char* argv[]){
    if (argc != 5) {
        std::cout << "Correct command: differentiator diff <expression> by <variable>\n";
        return 1;
    }

    Expression<long double> x;
    std::string s = argv[2];
    try { 
        x = Expression<long double>(s);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::string q = argv[4];
    std::string ans;
    try {
        ans = x.diff(argv[4]);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << ans << '\n';
    return 0;
}

int main(int argc, char* argv[]){
    
    if (argc < 3 || (std::strcmp(argv[1], "eval") != 0 && std::strcmp(argv[1], "diff") != 0)) {
        std::cout << "Correct command(diff): differentiator diff <expression> by <variable>\n";
        std::cout << "Correct command(eval): differentiator eval <expression>  <variable1>=<value1> <variable2>=<value2> ........\n";
        std::cout<<"smth went wrong\n";
        return 1;
    } else if (std::strcmp(argv[1], "eval") == 0) {
        if (eval(argc, argv)) {
            std::cout<<"smth went wrong\n";
            return 1;
        }
    } else {
        if (diff(argc, argv)) {
            std::cout<<"smth went wrong\n";
            return 1;
        }
    }
    return 0;
}

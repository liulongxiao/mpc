//
// Created by llx on 2018/8/13.
//

#ifndef MPC_UTILS_H
#define MPC_UTILS_H


#include<string>
#include<vector>
#include<regex>
#include <sstream>
inline std::vector<std::string> split(const std::string& input, const std::string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}


template<typename ValueType>
std::string stringulate(ValueType v)
{
    std::ostringstream oss;
    oss << v;
    return oss.str();
}
#endif //MPC_UTILS_H

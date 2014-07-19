//
//  enum_parser.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_ENUM_PARSER_H_
#define BATCH_SAMPLING_ENUM_PARSER_H_

#include <string>
#include <map>
#include <iostream>  // NOLINT
#include "./common.h"

template <typename ENUM>
class EnumParser {
public:  // NOLINT
    EnumParser() {}
    std::string enum_to_string(ENUM e);
    ENUM string_to_enum(const std::string &s);
private:  // NOLINT
    std::map<std::string, ENUM> string_to_enum_;
    std::map<ENUM, std::string> enum_to_string_;
};

template <typename ENUM>
std::string EnumParser<ENUM>::enum_to_string(ENUM e) {
    typename std::map<ENUM, std::string>::const_iterator it =
        enum_to_string_.find(e);
    if (it == enum_to_string_.end()) {
        std::cerr << "Error parsing enum!" << std::endl;
        exit(1);
    }
    return it->second;
}

template <typename ENUM>
ENUM EnumParser<ENUM>::string_to_enum(const std::string &s) {
    typename std::map<std::string, ENUM>::const_iterator it =
    string_to_enum_.find(s);
    if (it == string_to_enum_.end()) {
        std::cerr << "Error parsing enum!" << std::endl;
        exit(1);
    }
    return it->second;
}

template<>
EnumParser<Policy>::EnumParser() {
    enum_to_string_[MIT] = "mit";
    enum_to_string_[BS] = "bs";
    enum_to_string_[BSWF] = "bswf";
    string_to_enum_["mit"] = MIT;
    string_to_enum_["bs"] = BS;
    string_to_enum_["bswf"] = BSWF;
}

#endif  // BATCH_SAMPLING_ENUM_PARSER_H_

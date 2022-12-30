//
// Created by lyutoev on 28.12.22.
//

#include "yaml_parser.h"

yaml_parser::yaml_parser(std::string filename)
{
    YAML::Node basenode = YAML::LoadFile(filename);
    std::cout << basenode["limit_job"] << std::endl;
}

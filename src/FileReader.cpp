
#include "FileReader.h"

#include <iostream>
#include <fstream>

std::string *read_file(std::string path) {
    // Open file
    std::ifstream ifs;
    ifs.open(path, std::ifstream::in);

    if (!ifs.good()) {
        std::cerr << "Error when opening file " << path << std::endl;
        return NULL;
    }

    // Read file content
    std::string *content = new std::string();
    {
        std::string line;
        while (std::getline(ifs, line)) {
            *content += line;
        }
    }

    ifs.close();
    return content;
}


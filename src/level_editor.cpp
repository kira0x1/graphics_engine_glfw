//
// Created by kira on 28/10/2023.
//

#include "level_editor.h"
#include <c++/iostream>
#include <c++/sstream>
#include <c++/string>
#include <c++/fstream>

LevelEditor::LevelEditor(const char *levelPath) {
    std::ifstream levelFile;

    levelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        levelFile.open(levelPath);
        std::stringstream levelStream;
        levelStream << levelFile.rdbuf();
        levelFile.close();
        levelCode = levelStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::LEVEL::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
}

std::string *LevelEditor::getLevelCode() {
    return &levelCode;
}
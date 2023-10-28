//
// Created by kira on 28/10/2023.
//

#ifndef KIRA_SOURCE_LEVEL_EDITOR_H
#define KIRA_SOURCE_LEVEL_EDITOR_H

#include <c++/string>

class LevelEditor {
public:
    LevelEditor(const char *levelPath);
    std::string *getLevelCode();
private:
    std::string levelCode;
};


#endif //KIRA_SOURCE_LEVEL_EDITOR_H

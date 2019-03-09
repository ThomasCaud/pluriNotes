#ifndef TIMELIBRARY_H
#define TIMELIBRARY_H
#include <ctime>
#include <iostream>
#include <string>

/**
 * @brief The TimeLibrary class permet de manipuler le type time_t
 */
class TimeLibrary{
public:
    static std::string getString(time_t, bool);
};

#endif // TIMELIBRARY_H

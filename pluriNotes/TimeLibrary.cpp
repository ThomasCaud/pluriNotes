#include "TimeLibrary.h"
#include <sstream>

/**
 * @brief TimeLibrary::getString
 * @param t (timestamp que l'on veut lire)
 * @param displayHours (boolean: afficher ou non les heures)
 * @return un string contenant un affichage visible par un humain
 */
std::string TimeLibrary::getString(time_t t, bool displayHours){
    time_t now = t;
    tm *ltm = localtime(&now);
    std::stringstream date;
    date << ltm->tm_mday
         << "/"
         << 1 + ltm->tm_mon
         << "/"
         << 1900 + ltm->tm_year;

    if(displayHours){
        date << " "
         << 1 + ltm->tm_hour
         << ":"
         << 1 + ltm->tm_min
         << ":"
         << 1 + ltm->tm_sec;
    }
    return date.str();
}

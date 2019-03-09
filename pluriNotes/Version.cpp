#include "Version.h"
#include <iostream>

/**
 * @brief Version::display Pour le developpement
 */
void Version::display()const{
    std::cout<<"Version "<<getNoVersion()<< " ";
    aNote->display();
}

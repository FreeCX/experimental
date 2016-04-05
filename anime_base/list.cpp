#include <iostream>
#include <clocale>
#include "database.hpp"

const char menu_info[] =
    "> menu:\n"
    " 1 -- ввести regex\n"
    " 2 -- вывести базу\n"
    " 3 -- найти по id\n"
    " 4 -- найти по имени\n"
    " 5 -- записать изменения\n"
    " 0 -- выход\n"
    "> select: ";

int main( int argc, char * argv[] ) {
    int status = 0, select;
    std::string buffer;
    anibase data;

    setlocale( LC_ALL, "" );
    if ( argc == 1 ) {
        std::cout << "usage: " << argv[0] << " <database> <regex>" << std::endl;
        exit( 0 );
    } else if ( data.read_database( argv[1] ) == false ) {
        std::cout << "Can't open " << argv[1] << " file!" << std::endl;
        exit( 0 );
    }
    if ( argc > 2 ) {
        data.run_regexp( argv[2] );
    } else {
        do {
            std::cout << menu_info;
            std::cin >> status;
            switch ( status ) {
                case 1:
                    std::cout << "> input regexp: ";
                    std::cin >> buffer;
                    std::cout << std::endl;
                    data.run_regexp( buffer );
                    std::cout << std::endl;
                    break;
                case 2:
                    data.print_database();
                    break;
                case 3:
                    std::cout << "> available id: 0 -- " << data.get_size() << ": ";
                    std::cin >> select;
                    data.print_element( select );
                    std::cout << std::endl;
                    break;
                case 4:
                    std::cout << "> input text: ";
                    std::cin >> buffer;
                    data.print_by_name( buffer );
                    std::cout << std::endl;
                    break;
                case 5:
                    data.write_database( argv[1] );
                    std::cout << "> changes saved!" << std::endl;
                    break;
                default:
                    break;
            }
        } while ( status > 0 );
    }
    return 0;
}

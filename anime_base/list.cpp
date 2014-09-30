#include <iostream>
#include "database.hpp"

const char filename[] = "anime-list";
const char menu_info[] =
    "> menu:\n"
    " 1 -- input regexp\n"
    " 2 -- print database\n"
    " 3 -- print by id\n"
    " 4 -- find by name\n"
    " 5 -- write database\n"
    " 0 -- exit\n"
    "> select: ";

int main( int argc, char * argv[] )
{
    int status = 0, select;
    std::string buffer;
    anibase data;

    data.read_database( filename );
    if ( argc > 1 ) {
        data.run_regexp( argv[1] );
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
                    data.write_database( filename );
                    std::cout << "> changes saved!" << std::endl;
                    break;
                default:
                    break;
            }
        } while ( status > 0 );
    }
    return 0;
}

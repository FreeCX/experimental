#include <iostream>
#include <string>
#include <vector>

std::string example_str = "special! text^= ?123;word @ number#2";
std::string example_del = "!^?;@# ";

std::vector<std::string> tokenize( std::string data, std::string delimeters )
{
    size_t start = 0, counter = 0, length;
    size_t i, del_length = delimeters.length() + 1;
    size_t d_length = data.length();
    std::vector<std::string> a;

    do {
        for ( i = 0; i < del_length; i++ ) {
            if ( data[counter] == delimeters[i] ) {
                length = counter - start;
                if ( length > 0 ) {
                    a.push_back( std::string( data, start, length ) );
                    start = counter + 1;
                    break;
                }
                start = counter + 1;
            }
        }
    } while ( counter++ <= d_length ); 
    return a;
}

int main( void )
{
    std::vector<std::string> a;

    a = tokenize( example_str, example_del );
    for ( size_t i = 0; i < a.size(); i++ ) {
        std::cout << "token: " << a[i] << std::endl;
    }
}

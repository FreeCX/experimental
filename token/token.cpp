#include <iostream>
#include <string>
#include <vector>

std::string example_str = "special! text^= ?123;word @ number#2";
std::string example_del = "!^?;@# ";

void tokenize( std::vector<std::string> & token, const std::string & data, const std::string & delimeters )
{
    size_t del_length = delimeters.length() + 1;
    size_t dat_length = data.length() + 1;
    size_t i, j, prev, next;

    prev = next = 0;
    for ( i = 0; i < dat_length; i++ ) {
        for ( j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev >= 1 ) {
                    token.push_back( std::string( data, prev, next - prev ) );
                    prev = next + 1;
                    break;
                }
                prev = next + 1;
            }
        }
        next++;
    }
}

int main( void )
{
    std::vector<std::string> token;

    tokenize( token, example_str, example_del );
    for ( size_t i = 0; i < token.size(); i++ ) {
        std::cout << "token: " << token[i] << std::endl;
    }
}

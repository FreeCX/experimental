#include <iostream>
#include <string>
#include <vector>

void tokenize( std::vector<std::string> & token, const std::string & data, const std::string & delimeters )
{
    size_t prev, next;

    prev = next = 0;
    for ( auto & i : data ) {
        for ( auto & j : delimeters ) {
            if ( i == j ) {
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
    // add last token
    if ( next - prev >= 1 ) {
        token.push_back( std::string( data, prev, next - prev ) );
    }
}

int main( void )
{
    std::string example_str = "special! text^= ?123;word @ number#2";
    std::string example_del = "!^?;@# ";
    std::vector<std::string> token;

    tokenize( token, example_str, example_del );
    for ( auto & i : token ) {
        std::cout << "token: " << i << std::endl;
    }
}

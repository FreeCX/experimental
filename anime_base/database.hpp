#ifndef __DATABASE_HPP__

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector< std::string > token_t;

struct anime_list_t {
    std::string name;
    short status;
    int progress_cur;
    int progress_max;
    int score;
};
struct print_format_t {
    size_t max_name = 0;
    size_t max_progress = 0;
    size_t max_score = 0;
    size_t id_size = 0;
    size_t max_status = 0;
};

class anibase {
public:
    ~anibase();
    void read_database( std::string filename );
    void write_database( std::string filename );
    void run_regexp( std::string regexp );
    void print_element( size_t id );
    void print_database( void );
    void print_by_name( std::string name );
    void get_id_by_name( std::string name, std::vector< size_t > & id );
    size_t get_size( void );
private:
    void tokenize( const std::string & data, const std::string & delimeters );
    int get_status_id( const std::string & status );
    const char * get_status_str( size_t i );
    void update_print_format( print_format_t & fmt, anime_list_t & a );
    void print_one( print_format_t & fmt, size_t id );
    std::vector< anime_list_t > database;
    print_format_t null_format, format;
    std::string file_name;
    token_t token;
};

#endif

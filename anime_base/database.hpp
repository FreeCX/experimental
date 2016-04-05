#ifndef __DATABASE_HPP__

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sys/ioctl.h>

typedef std::vector< std::string > token_t;

struct anime_list_t {
    std::string name;
    short status = 3; // plan
    int progress_cur = 0;
    int progress_max = 0;
    int score = 0;
    bool operator == ( const anime_list_t & p ) const {
        return ( name == p.name );
    }
    bool operator < ( const anime_list_t & p ) const {
        return ( name < p.name );
    }
};
struct print_format_t {
    int max_name = 0;
    int max_progress = 0;
    int max_score = 2;
    int id_size = 0;
    int max_status = 0;
};

enum {
    S_TITLE = 1,
    S_STATUS = 2,
    S_EPISODES = 3,
    S_WATCHED = 4,
    S_SCORE = 5
};

class anibase {
public:
    anibase();
    ~anibase();
    bool read_database( std::string filename );
    void read_xml( std::string xml );
    void merge_xml( std::string xml );
    void write_database( std::string filename );
    void run_regexp( std::string regexp );
    void print_element( size_t id );
    void print_database( void );
    void print_by_name( std::string name );
    void get_id_by_name( std::string name, std::vector< size_t > & id );
    size_t add_element( std::string name );
    size_t get_size( void );
private:
    void tokenize( const std::string & data, const std::string & delimeters );
    int get_status_id( const std::string & status );
    int get_short_status_id( const char status );
    const char * get_status_str( size_t i );
    void update_print_format( print_format_t & fmt, anime_list_t & a );
    void print_one( print_format_t & fmt, size_t id );
    void import_xml( std::vector< anime_list_t > & list, std::string name );
    void merge_database( std::vector< anime_list_t > & list );
    size_t format_switch( std::string buf );
    size_t status_switch( std::string status );
    std::vector< anime_list_t > database;
    print_format_t null_format, format;
    std::string file_name;
    token_t token;
    uint8_t term_size;
};

#endif

#include "database.hpp"

const char * status_list[] = {
    (char *) "wrong",
    (char *) "complete",
    (char *) "drop",
    (char *) "plan",
    (char *) "watch",
    nullptr
};
const char regexp_info[] =
    ">> regexp info:\n"
    " f         -- find by title\n"
    " d         -- delete elements {found by title}\n"
    " +         -- progress +1\n"
    " -         -- progress -1\n"
    " s{number} -- set score to {number}\n"
    " p{number} -- set progress to {number}\n"
    " s{letter} -- set status { c -- complete, d -- drop, p -- plan, w -- watch }\n"
    " n{new}    -- set name to {new}\n"
    " w         -- write to database\n"
    ">> example: f/D.Gray-man/+/-/s7/p23/sc/n/d.gray-man/w";

void anibase::read_database( std::string filename )
{
    std::string delimeters = " /";
    std::string buffer;
    std::ifstream read;
    anime_list_t tmp;

    file_name = filename;
    read.open( filename );
    while ( std::getline( read, buffer ) ) {
        tokenize( buffer, delimeters );
        tmp.name = token[0].substr( 1, token[0].length() - 2 );
        tmp.status = get_status_id( token[1] );
        tmp.progress_cur = std::stoi( token[3] );
        tmp.progress_max = std::stoi( token[4] );
        tmp.score = std::stoi( token[6] );
        update_print_format( format, tmp );
        database.push_back( tmp );
        token.clear();
    }
    format.max_status = 8; // by status_list
    format.id_size = std::ceil( std::log10( database.size() ) );
    read.close();
}

void anibase::write_database( std::string filename )
{
    std::ofstream write;

    write.open( filename );
    for ( auto & a : database ) {
        write << "\"" << a.name << "\" " << status_list[ a.status ]
              << " progress " << a.progress_cur << "/" << a.progress_max
              << " score " << a.score << std::endl;
    }
    write.close();
}

void anibase::run_regexp( std::string regexp )
{
    std::vector< size_t > id;
    token_t regexp_token;
    int update = 0;

    tokenize( regexp, "/" );
    for ( auto & t : token ) {
        if ( t[0] == 's' || t[0] == 'p' ) {
            regexp_token.push_back( t.substr( 0, 1 ) );
            regexp_token.push_back( t.substr( 1 ) );
        } else {
            regexp_token.push_back( t );
        }
    }
    token.clear();
    for ( size_t i = 0; i < regexp_token.size(); i++ ) {
        switch ( regexp_token[i][0] ) {
            case '+':
                for ( auto & a : id ) {
                    database[a].progress_cur++;
                }
                update++;
                break;
            case '-':
                for ( auto & a : id ) {
                    database[a].progress_cur--;
                }
                update++;
                break;
            case 'd':
                std::cout << "\e[0;31m>> deleted elements:\e[0m" << std::endl;
                for ( auto & a : id ) {
                    print_one( format, a );
                    database.erase( database.begin() + a );
                }
                update++;
                id.clear();
                break;
            case 'f':
                i++;
                id = get_id_by_name( regexp_token[i] );
                std::cout << "\e[0;37m>> found elements:\e[0m" << std::endl;
                for ( auto & a : id ) {
                    print_one( format, a );
                }
                break;
            case 'i':
                std::cout << regexp_info << std::endl;
                break;
            case 'n':
                i++;
                for ( auto & a : id ) {
                    database[a].name = regexp_token[i];
                }
                update++;
                break;
            case 'p':
                i++;
                for ( auto & a : id ) {
                    database[a].progress_cur = std::stoi( regexp_token[i] );
                }
                update++;
                break;
            case 's':
                i++;
                if ( isalpha( regexp_token[i][0] ) ) {
                    for ( auto & a : id ) {
                        switch ( regexp_token[i][0] ) {
                            case 'c':
                                database[a].status = get_status_id( "complete" );
                                database[a].progress_cur = database[a].progress_max;
                                break;
                            case 'd':
                                database[a].status = get_status_id( "drop" );
                                break;
                            case 'p':
                                database[a].status = get_status_id( "plan" );
                                break;
                            case 'w':
                                database[a].status = get_status_id( "watch" );
                                break;
                            default:
                                break;
                        }
                    }
                } else {
                    for ( auto & a : id ) {
                        database[a].score = std::stoi( regexp_token[i] );
                    }
                }
                update++;
                break;
            case 'w':
                write_database( file_name );
                break;
            default:
                break;
        }
    }
    if ( id.size() > 0 && update > 0 ) {
        std::cout << "\e[0;32m>> changed to:\e[0m" << std::endl;
        for ( auto & a : id ) {
            print_one( format, a );
        }
    }
}

anibase::~anibase()
{
    database.clear();
}

void anibase::tokenize( const std::string & data, const std::string & delimeters )
{
    size_t prev, next;
    short comma = 0;

    prev = next = 0;
    for ( auto & i : data ) {
        if ( i == '\"' ) {
            comma = !comma;
        }
        if ( comma == 0 ) {
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
        }
        next++;
    }
    // add last token
    if ( next - prev >= 1 ) {
        token.push_back( std::string( data, prev, next - prev ) );
    }
}

int anibase::get_status_id( const std::string & status )
{
    for ( size_t i = 0; status_list[i] != nullptr; i++ ) {
        if ( status_list[i] == status ) {
            return i;
        }
    }
    return 0;
}

const char * anibase::get_status_str( size_t i )
{
    return status_list[i];
}

void anibase::update_print_format( print_format_t & fmt, anime_list_t & a )
{
    size_t progress_m = std::ceil( std::log10( a.progress_max ) );
    size_t score = std::ceil( std::log10( a.score ) );

    if ( a.name.length() > fmt.max_name ) {
        fmt.max_name = a.name.length();
    }
    if ( progress_m > fmt.max_progress ) {
        fmt.max_progress = progress_m;
    }
    if ( score > fmt.max_score ) {
        fmt.max_score = score;
    }
}

void anibase::print_one( print_format_t & fmt, size_t id )
{
    anime_list_t & a = database[id];

    std::cout << " [id: " <<std::setw( fmt.id_size ) << id
              << "] title: '" << std::setw( fmt.max_name ) << a.name
              << "', status: " << std::setw( fmt.max_status ) << get_status_str( a.status )
              << ", progress: " << std::setw( fmt.max_progress ) << a.progress_cur
              << " / " << std::setw( fmt.max_progress ) << a.progress_max
              << ", score: " << std::setw( fmt.max_score ) << a.score << " / 10" << std::endl;
}

void anibase::print_element( size_t id )
{
    print_one( null_format, id );
}

void anibase::print_database( void )
{
    for ( size_t i = 0; i < database.size(); i++ ) {
        print_one( format, i );
    }
}

size_t anibase::get_size( void )
{
    return database.size() - 1;
}

void anibase::print_by_name( std::string name )
{
    for ( size_t i = 0; i < database.size(); i++ ) {
        if ( database[i].name.find( name ) != std::string::npos ) {
            print_one( format, i );
        }
    }
}

std::vector< size_t > anibase::get_id_by_name( std::string name )
{
    std::vector< size_t > id;

    for ( size_t i = 0; i < database.size(); i++ ) {
        if ( database[i].name.find( name ) != std::string::npos ) {
            id.push_back( i );
        }
    }
    return id;
}

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
    ">> regex info:\n"
    " +        -- номер серии +1\n"
    " -        -- номер серии -1\n"
    " a        -- добавить элемент\n"
    " d        -- удалить элементы {найденые элементы параметром f}\n"
    " f        -- поиск по названию\n"
    " i        -- распечатать эту информацию\n"
    " n        -- изменить имя на новое\n"
    " p{число} -- установить номер серии на {число}\n"
    " m{число} -- установить максимальный номер серии {0 в случае онгоинга}\n"
    " s{число} -- установить рейтинг {число}\n"
    " s{буква} -- установить статут { c -- complete, d -- drop, p -- plan, w -- watch }\n"
    " w        -- записать изменения в базу\n"
    ">> example: f/D.Gray-man/+/-/s7/p23/sc/n/d.gray-man/w";

bool anibase::read_database( std::string filename )
{
    std::string delimeters = " /";
    std::string buffer;
    std::ifstream read;
    anime_list_t tmp;

    file_name = filename;
    read.open( filename );
    if ( read.is_open() == false ) {
        return false;
    }
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
    return true;
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
    size_t update = 0, id_curr = 0, eid = 0;
    std::vector< size_t > id, changed;
    bool save_flag = false;
    token_t regexp_token;

    tokenize( regexp, "/" );
    for ( auto & t : token ) {
        if ( t[0] == 's' || t[0] == 'p' || t[0] == 'm' ) {
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
            case 'a':
                i++;
                eid = add_element( regexp_token[i] );
                id.push_back( eid );
                changed.push_back( eid );
#ifdef _WIN32
                std::cout << ">> append:";
#else
                std::cout << "\e[0;34m>> append:\e[0m";
#endif
                print_one( format, get_size() );
                update++;
                break;
            case 'd':
                for ( auto & a : id ) {
#ifdef _WIN32
                    std::cout << ">> delete:";
#else
                    std::cout << "\e[0;31m>> delete:\e[0m";
#endif
                    print_one( format, a );
                }
                for ( auto & a : id ) {
                    database.erase( database.begin() + a );
                }
                id.clear();
                break;
            case 'f':
                i++;
                get_id_by_name( regexp_token[i], id );
                for ( size_t a = id_curr; a < id.size(); a++ ) {
#ifdef _WIN32
                    std::cout << ">>  found:";
#else
                    std::cout << "\e[0;37m>>  found:\e[0m";
#endif
                    print_one( format, id[a] );
                }
                changed.reserve( id.size() + changed.size() );
                changed.insert( changed.end(), id.begin(), id.end() );
                std::sort( changed.begin(), changed.end() );
                changed.erase( std::unique( changed.begin(), changed.end() ), changed.end() );
                id_curr = id.size();
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
                id_curr = 0;
                update++;
                break;
            case 'm':
                i++;
                for ( auto & a : id ) {
                    database[a].progress_max = std::stoi( regexp_token[i] );
                }
                id_curr = 0;
                update++;
                break;
            case 's':
                i++;
                if ( isalpha( regexp_token[i][0] ) ) {
                    for ( auto & a : id ) {
                        switch ( regexp_token[i][0] ) {
                            case 'c':
                                database[a].status = get_status_id( "complete" );
                                database[a].progress_cur = database[id[a]].progress_max;
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
                save_flag = true;
                id.clear();
                break;
            default:
                break;
        }
    }
    if ( changed.size() > 0 && update > 0 ) {
        std::sort( changed.begin(), changed.end() );
        changed.erase( std::unique( changed.begin(), changed.end() ), changed.end() );
        for ( auto & a : changed ) {
#ifdef _WIN32
            std::cout << ">> change:";
#else
            std::cout << "\e[0;32m>> change:\e[0m";
#endif
            print_one( format, a );
        }
    }
    if ( save_flag == true ) {
#ifdef _WIN32
        std::cout << ">> change saved!" << std::endl;
#else
        std::cout << "\e[0;34m>> change saved!\e[0m" << std::endl;
#endif
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
    int progress_m = std::ceil( std::log10( a.progress_max ) );

    if ( (int) a.name.length() > fmt.max_name ) {
        fmt.max_name = a.name.length();
    }
    if ( progress_m > fmt.max_progress ) {
        fmt.max_progress = progress_m;
    }
}

void anibase::print_one( print_format_t & fmt, size_t id )
{
    anime_list_t & a = database[id];

    std::cout << " '" << std::setw( fmt.max_name ) << a.name
              << "', status: " << std::setw( fmt.max_status ) << get_status_str( a.status )
              << ", progress: " << std::setw( fmt.max_progress ) << a.progress_cur
              << " / " << std::setw( fmt.max_progress );
    if ( a.progress_max == 0 ) {
        std::cout << "?";
    } else {
        std::cout << a.progress_max;
    }
    std::cout << ", score: " << std::setw( fmt.max_score ) << a.score << " / 10"
              << ", id: " << std::setw( fmt.id_size ) << id << std::endl;
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

void anibase::get_id_by_name( std::string name, std::vector< size_t > & id )
{
    for ( size_t i = 0; i < database.size(); i++ ) {
        if ( database[i].name.find( name ) != std::string::npos ) {
            id.push_back( i );
        }
    }
}

size_t anibase::add_element( std::string name )
{
    anime_list_t tmp;

    tmp.name = name;
    database.push_back( tmp );
    return get_size();
}

#include "database.hpp"

const char * status_list[] = {
    (const char *) "wrong",
    (const char *) "complete",
    (const char *) "drop",
    (const char *) "plan",
    (const char *) "watch",
    (const char *) "hold",
    nullptr
};
const char regexp_info[] =
    ">> regex info:\n"
    " -        -- номер серии -1\n"
    " +        -- номер серии +1\n"
    " a        -- добавить элемент [ a/имя | a/\"имя\" ]\n"
    " d        -- удалить элементы { найденые элементы параметром f }\n"
    " f        -- поиск по названию [ f/\"имя или regex\" ]\n"
    " i        -- распечатать эту информацию\n"
    " l        -- вывести весь список\n"
    " m{число} -- установить максимальный номер серии { ? в случае онгоинга }\n"
    " n        -- изменить имя на новое [ n/имя | n/\"имя\" ]\n"
    " r        -- загрузить xml файл [ r/имя | r/\"путь-до-файла\" ]\n"
    " p{число} -- установить номер серии на { число }\n"
    " s{буква} -- установить статуc { c -- complete, d -- drop, p -- plan, w -- watch }\n"
    " s{число} -- установить рейтинг { число }\n"
    " x        -- объеденить базу и xml файл [ r/имя | r/\"путь-до-файла\" ]\n"
    " w        -- записать изменения в базу\n"
    ">> example: f/\"D.Gray-man\"/+/-/s7/p23/sc/n/d.gray-man/w";

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
        if ( token.size() == 7 ) {
            tmp.name = token[0].substr( 1, token[0].length() - 2 );
            tmp.status = get_status_id( token[1] );
            tmp.progress_cur = isdigit( token[3][0] ) ? std::stoi( token[3] ) : 0;
            tmp.progress_max = ( token[4][0] == '?' ) ? 0 : std::stoi( token[4] );
            tmp.score = isdigit( token[6][0] ) ? std::stoi( token[6] ) : 0;
            update_print_format( format, tmp );
            database.push_back( tmp );
        }
        token.clear();
    }
    format.max_status = 8; // by status_list
    format.id_size = std::ceil( std::log10( database.size() ) );
    read.close();
    return true;
}

void anibase::read_xml( std::string xml )
{
    database.clear();
    import_xml( database, xml );
    std::sort( database.begin(), database.end() );
    database.erase( std::unique( database.begin(), database.end() ), database.end() );
}

void anibase::merge_xml( std::string xml )
{
    std::vector< anime_list_t > list;

    import_xml( list, xml );
    merge_database( list );
    std::sort( database.begin(), database.end() );
    database.erase( std::unique( database.begin(), database.end() ), database.end() );
}

void anibase::write_database( std::string filename )
{
    std::ofstream write;

    write.open( filename );
    for ( auto & a : database ) {
        write << "\"" << a.name << "\" " << status_list[ a.status ]
              << " progress " << a.progress_cur << "/";
        if ( a.progress_max == 0 ) {
            write << "?";
        } else {
            write << a.progress_max;
        }
        write << " score " << a.score << std::endl;
    }
    write.close();
}

void insert_with_update( std::vector< size_t > & id, std::vector< size_t > & changed )
{
    changed.reserve( id.size() + changed.size() );
    changed.insert( changed.end(), id.begin(), id.end() );
    std::sort( changed.begin(), changed.end() );
    changed.erase( std::unique( changed.begin(), changed.end() ), changed.end() );
}

void anibase::run_regexp( std::string regexp )
{
    bool save_flag = false, delete_flag = false, xml_flag = false;
    size_t update = 0, id_curr = 0, eid = 0;
    std::vector< size_t > id, changed;
    token_t regexp_token;
    std::string tmp;

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
                    auto & p = database[a];
                    p.progress_cur++;
                    if ( ( p.progress_cur >= p.progress_max && p.progress_max != 0 ) ||
                         p.status == get_status_id( "complete" ) ) {
                        p.progress_cur = p.progress_max;
                        p.status = get_status_id( "complete" );
                    }
                }
                update++;
                break;
            case '-':
                for ( auto & a : id ) {
                    auto & p = database[a];
                    p.progress_cur--;
                    if ( p.progress_cur <= 0 ) {
                        p.progress_cur = 0;
                    }
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
                delete_flag = true;
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
                id_curr = id.size();
                break;
            case 'i':
                std::cout << regexp_info << std::endl;
                break;
            case 'n':
                i++;
                for ( auto & a : id ) {
                    auto & p = database[a];
                    if ( regexp_token[i][0] == '\"' ) {
                        p.name = regexp_token[i].substr( 1, regexp_token[i].length() - 2 );
                    } else {
                        p.name = regexp_token[i][0];
                    }
                }
                update++;
                break;
            case 'l':
                print_database();
                break;
            case 'p':
                i++;
                for ( auto & a : id ) {
                    auto & p = database[a];
                    p.progress_cur = std::stoi( regexp_token[i] );
                    if ( p.progress_cur >= p.progress_max && p.progress_max != 0 ) {
                        p.progress_cur = p.progress_max;
                        p.status = get_status_id( "complete" );
                    }
                }
                id_curr = 0;
                update++;
                break;
            case 'r':
                i++;
                xml_flag = true;
                if ( regexp_token[i][0] == '\"' ) {
                    tmp = regexp_token[i].substr( 1, regexp_token[i].length() - 2 );
                } else {
                    tmp = regexp_token[i];
                }
                read_xml( tmp );
#ifdef _WIN32
                std::cout << ">> xml loaded" << std::endl;
#else
                std::cout << "\e[0;37m>> xml loaded\e[0m" << std::endl;
#endif
                break;
            case 'm':
                i++;
                for ( auto & a : id ) {
                    auto & p = database[a];
                    p.progress_max = regexp_token[i][0] == '?' ? 0 : std::stoi( regexp_token[i] );
                    if ( p.status == get_status_id( "complete" ) ) {
                        p.progress_cur = p.progress_max;
                    }
                }
                id_curr = 0;
                update++;
                break;
            case 's':
                i++;
                for ( auto & a : id ) {
                    auto & p = database[a];
                    switch ( regexp_token[i][0] ) {
                        case 'c':
                            p.status = get_status_id( "complete" );
                            p.progress_cur = p.progress_max;
                            break;
                        case 'd':
                            p.status = get_status_id( "drop" );
                            break;
                        case 'p':
                            p.status = get_status_id( "plan" );
                            break;
                        case 'w':
                            p.status = get_status_id( "watch" );
                            break;
                        case '0'...'9':
                            p.score = std::stoi( regexp_token[i] );
                            break;
                        default:
                            break;
                    }
                }
                update++;
                break;
            case 'w':
                write_database( file_name );
                save_flag = true;
                insert_with_update( id, changed );
                id.clear();
                id_curr = 0;
                break;
            case 'x':
                i++;
                xml_flag = true;
                if ( regexp_token[i][0] == '\"' ) {
                    tmp = regexp_token[i].substr( 1, regexp_token[i].length() - 2 );
                } else {
                    tmp = regexp_token[i];
                }
                merge_xml( tmp );
#ifdef _WIN32
                std::cout << ">> xml merged" << std::endl;
#else
                std::cout << "\e[0;32m>> xml merged\e[0m" << std::endl;
#endif
                break;
            default:
                break;
        }
    }
    if ( update > 0 ) {
        insert_with_update( id, changed );
    } else if ( id.size() == 0 && delete_flag == false && xml_flag == false ){
#ifdef _WIN32
        std::cout << ">> record not found" << std::endl;
#else
        std::cout << "\e[0;37m>> record not found\e[0m" << std::endl;
#endif
    }
    if ( changed.size() > 0 ) {
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
        id.clear();
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
    std::regex regex_str;

    try {
        regex_str = ( name[0] == '\"' ) ? name.substr( 1, name.length() - 2 ) : name;
    } catch ( std::regex_error& e ){
        if ( e.code() == std::regex_constants::error_badrepeat ) {
            std::cerr << "[error]: Repeat was not preceded by a valid regular expression." << std::endl;
        } else {
            std::cerr << "[error]: Some other regex exception happened." << std::endl;
        }
        return;
    }
    for ( size_t i = 0; i < database.size(); i++ ) {
        if ( std::regex_match( database[i].name, regex_str ) ) {
            id.push_back( i );
        }
    }
}

size_t anibase::add_element( std::string name )
{
    anime_list_t tmp;

    tmp.name = ( name[0] == '\"' ) ? name.substr( 1, name.length() - 2 ) : name;
    database.push_back( tmp );
    return get_size();
}

size_t anibase::format_switch( std::string buf )
{
    const char * format[] = {
        (const char *) "series_title",
        (const char *) "my_status",
        (const char *) "series_episodes",
        (const char *) "my_watched_episodes",
        (const char *) "my_score",
        nullptr
    };

    for ( size_t i = 0; format[i] != nullptr; i++ ) {
        if ( format[i] == buf ) {
            return i + 1;
        }
    }
    return 0;
}

size_t anibase::status_switch( std::string status )
{
    const char * status_switch[] = {
        (const char *) "Completed",
        (const char *) "Dropped",
        (const char *) "Plan to Watch",
        (const char *) "Watching",
        (const char *) "On-Hold",
    };

    for ( size_t i = 0; status_switch[i] != nullptr; i++ ) {
        if ( status_switch[i] == status ) {
            return i+1;
        }
    }
    return 0;
}

void anibase::import_xml( std::vector< anime_list_t > & list, std::string name )
{
    std::string delimeters = "</>";
    std::ifstream read( name );
    bool anime_flag = false;
    std::string buffer;
    anime_list_t tmp;

    if ( read.is_open() == true ) {
        while ( std::getline( read, buffer ) ) {
            token.clear();
            tokenize( buffer, delimeters );
            if ( token[1] == "anime" ) {
                if ( anime_flag == true ) {
                    list.push_back( tmp );
                }
                anime_flag = !anime_flag;
                continue;
            }
            if ( anime_flag == true ) {
                switch ( format_switch( token[1] ) ) {
                    case S_TITLE:
                        tmp.name = token[2][0] != '!' ? token[2] : token[2].substr( 8, token[2].size() - 10 );
                        break;
                    case S_STATUS:
                        tmp.status = status_switch( token[2] );
                        break;
                    case S_EPISODES:
                        tmp.progress_max = std::stoi( token[2] );
                        break;
                    case S_WATCHED:
                        tmp.progress_cur = std::stoi( token[2] );
                        break;
                    case S_SCORE:
                        tmp.score = std::stoi( token[2] );
                        break;
                    default:
                        break;
                }
            }
        }
        read.close();
    }
}

void anibase::merge_database( std::vector< anime_list_t > & list )
{
    std::vector< anime_list_t > tmp;

    std::sort( database.begin(), database.end() );
    std::sort( list.begin(), list.end() );

    for ( auto & a : database ) {
        for ( auto & b : list ) {
            if ( a.name == b.name ) {
                if ( a.progress_cur < b.progress_cur ) {
                    a = b;
                    continue;
                }
            } else {
                tmp.push_back( b );
            }
        }
    }
    database.reserve( database.size() + tmp.size() );
    database.insert( database.end(), tmp.begin(), tmp.end() );
    std::sort( database.begin(), database.end() );
}

#include "font.h"

SDL_Window *window;
SDL_Renderer *render;
SDL_Event event;
short quit_flag = 0;
font_table_t *ft = NULL;

wchar_t example_text[] =
    L"+----------------------+\n"
    L"#    $ 1234567890 $    #\n"
    L"#  this english text!  #\n"
    L"#  это русский текст!  #\n"
    L"+----------------------+";

void send_error( int code )
{
    printf( "%s", SDL_GetError() );
    exit( code );
}

int font_load( SDL_Renderer * r, font_table_t ** t, const char * font ) {
    unsigned int text_size = 0, abc_size = 0;
    SDL_Texture *tex = NULL;
    font_table_t *a = NULL;
    wint_t current = 0;
    char * text_name;
    size_t load = 1;
    int id = 0;
    FILE * f;

    a = (font_table_t *) calloc( 1, sizeof( font_table_t ) );
    *t = a;
    f = fopen( font, "rb" );
    if ( f == NULL ) {
        return A_ERROR_OPEN_FILE;
    }
    fread( &( text_size ), sizeof(int), 1, f );
    fread( &( abc_size ), sizeof(int), 1, f );
    text_name = (char *) malloc( text_size * sizeof(char) );
    a->table = (int *) malloc( abc_size * sizeof(int) );
    fread( text_name, text_size, 1, f );
    fread( &( a->t_width ), sizeof(int), 1, f );
    fread( &( a->t_height ), sizeof(int), 1, f );
    printf( "%s [ %dx%d ]\n", text_name, a->t_width, a->t_height );
    tex = IMG_LoadTexture( r, text_name );
    free( text_name );
    a->font = tex;
    if ( tex == NULL ) {
        fclose( f );
        return A_ERROR_LOAD_TEXTURE;
    }
    SDL_QueryTexture( tex, NULL, NULL, &( a->f_width ), &( a->f_height ) );
    fseek( f, sizeof(int) * 4 + text_size + 1, SEEK_SET );
    do {
        load = fread( &current, 2, 1, f );
        if ( current != L'\n' && current < 0xFFFF && load != 0 ) {
            a->table[current] = id++;
            printf( "id: %02d [%lc:%d] : %u\n", a->table[current], current, current, (unsigned int) load );
        }
    } while ( load != 0 );
    fclose( f );
    return A_SUCCESS;
}

void font_destroy( font_table_t *t )
{
    SDL_DestroyTexture( t->font );
    free( t->table );
    free( t );
}

void font_color( font_table_t *t, Uint32 color )
{
    SDL_SetTextureColorMod( t->font, color >> 16, ( color >> 8 ) & 0xFF, color & 0xFF );
}

void font_draw( SDL_Renderer *r, font_table_t *t, const wchar_t *text, int x, int y )
{
    SDL_Rect wnd = { 0, 0, t->t_width, t->t_height };
    SDL_Rect pos = { 0, 0, t->t_width, t->t_height };
    int dy = 0, i = 0, id = 0, old_x = x;
    wint_t current;

    pos.x = x; pos.y = y;
    while ( ( current = text[i++] ) != '\0' ) {
        switch ( current ) {
            case '\n':
                pos.y += t->t_height;
                pos.x = old_x;
                continue;
            case '\t':
                pos.x += 2 * t->t_width;
                continue;
            // to upper
            case 'a'...'z':
            case L'а'...L'я':
                current -= 0x20;
                break;
        }
        id = t->table[current];
        while ( id * t->t_width >= t->f_width ) {
            id -= t->f_width / t->t_width;
            dy++;
        }
        wnd.x = id * t->t_width; wnd.y = dy * t->t_height;
        pos.x += t->t_width;
        SDL_RenderCopy( r, t->font, &wnd, &pos );
        dy = 0;
    }
}

void game_init( void )
{
    window = SDL_CreateWindow( "Font Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( window == NULL ) {
        send_error( EXIT_FAILURE );
    }
    render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( render == NULL ) {
        send_error( EXIT_FAILURE );
    }
    font_load( render, &ft, "font.cfg" );
    // set orange color
    font_color( ft, 0xFF7F27 );
}

void game_event( SDL_Event *event )
{
    SDL_PollEvent( event );
    switch ( event->type ) {
        case SDL_QUIT:
            quit_flag = 1;
            break;
        default:
            break;
    }
}

void game_render( void )
{
    // background color
    SDL_SetRenderDrawColor( render, 4, 32, 41, 255 );
    SDL_RenderClear( render );
    font_draw( render, ft, example_text, 52, 96 );
    SDL_RenderPresent( render );
}

void game_destroy( void )
{
    font_destroy( ft );
    SDL_DestroyRenderer( render );
    SDL_DestroyWindow( window );
    SDL_Quit();
}

int main( int argc, char *argv[] )
{
    setlocale( LC_CTYPE, "" );
    game_init();
    while ( quit_flag == 1 ) {
        game_event( &event );
        game_render();
    }
    game_destroy();
    return EXIT_SUCCESS;
}
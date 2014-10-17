#include "font.h"
#include "token.h"

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

int font_load( SDL_Renderer *r, font_table_t **t, const char *font )
{
    SDL_Texture *tex = NULL;
    font_table_t *a = NULL;
    char tex_name[64];
    char buffer[128];
    wint_t current = 0;
    size_t load = 1, i = 0;
    FILE *f;
    token_t *token;
    int id = 0;

    a = (font_table_t *) calloc( 1, sizeof( font_table_t ) );
    *t = a;
    f = fopen( font, "rb" );
    if ( f == NULL ) {
        return A_ERROR_OPEN_FILE;
    }
    do {
        load = fread( &buffer[i], 1, 1, f );
    } while ( buffer[i++] != '\0' );
    tokenize( &token, buffer, " " );
    strcpy( tex_name, token->name[0] );
    a->t_width = atoi( token->name[1] );
    a->t_height = atoi( token->name[2] );
    free_token( token );
    printf( "%s [ %dx%d ]\n", tex_name, a->t_width, a->t_height );
    tex = IMG_LoadTexture( r, tex_name );
    a->font = tex;
    if ( tex == NULL ) {
        fclose( f );
        return A_ERROR_LOAD_TEXTURE;
    }
    SDL_QueryTexture( tex, NULL, NULL, &( a->f_width ), &( a->f_height ) );
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
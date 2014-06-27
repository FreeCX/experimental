#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "../src/loader.h"

void system_init( int *argc, char *argv[] );
void system_opengl( void );
void system_render( void );
void system_redraw( int value );
void system_resize( const int width, const int height );
void system_destroy( void );

char def[] = "../img/image.bmp";
char *img_file;
image_t img;
int w_width = 332, w_height = 50;

void system_init( int *argc, char *argv[] )
{
	GLint sw, sh;
    size_t len;

    if ( *argc == 2 && ( len = strlen( argv[1] ) ) > 0 ) {
        img_file = (char *) malloc( len*sizeof(char) );
        strcpy( img_file, argv[1] );
    } else {
        img_file = def;
    }
    glutInit( argc, argv );
    glutInitWindowSize( w_width, w_height );
    glutInitWindowPosition( 0, 0 );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow( "libimage test" );
    glutReshapeFunc( system_resize );
    glutDisplayFunc( system_render );
    glutTimerFunc( 30, system_redraw, 0 );
    sw = glutGet( GLUT_SCREEN_WIDTH );
	sh = glutGet( GLUT_SCREEN_HEIGHT );
    glutPositionWindow( ( sw - w_width ) / 2, ( sh - w_height ) / 2 );
    system_opengl();
    glutMainLoop();
}

void system_destroy( void )
{
    img_close( &img );
    free( img_file );
}

void system_opengl( void )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
    img_debug( 1 );
    img_load( img_file, &img );
    glBindTexture( GL_TEXTURE_2D, 1 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, img.bpp, img.width, img.height, 0,
        img.c_format, GL_UNSIGNED_BYTE, img.data );
	free( img.data );
}

void system_render( void )
{
	float n2 = (float) img.height / w_height;
	float n1 = (float) img.width / w_width;

    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    glBegin( GL_QUADS );
        glTexCoord2f( 1.0f, 0.0f );
        glVertex2f( n1, -n2 );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex2f( -n1, -n2 );
        glTexCoord2f( 0.0f, 1.0f );
        glVertex2f( -n1, n2 );
        glTexCoord2f( 1.0f, 1.0f );
        glVertex2f( n1, n2 );
    glEnd();
    glutSwapBuffers();
}

void system_redraw( int value )
{
    system_render();
    glutTimerFunc( 30, system_redraw, 0 );
}

void system_resize( const int width, const int height )
{
    float a = width / (float) height;
    w_width = width; w_height = height;

    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glOrtho( 0.0f, width, height, 0.0f, 0.0f, 0.0f );
    gluPerspective( 0.0f, a, 0.0f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

int main( int argc, char *argv[] )
{
    system_init( &argc, argv );
    system_destroy();
    return EXIT_SUCCESS;
}
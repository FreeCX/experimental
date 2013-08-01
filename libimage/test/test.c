#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "../src/loader.h"

void system_init( int *argc, char *argv[] );
void system_opengl( void );
void system_render( void );
void system_resize( const int width, const int height );
void system_destroy( void );

char def[] = "../img/image.pcx";
char *img_file;
image_t img;
float w, h;

void system_init( int *argc, char *argv[] )
{
    size_t len;
    if ( *argc == 2 && ( len = strlen( argv[1] ) ) > 0 ) {
        img_file = malloc( len );
        strcpy( img_file, argv[1] );
    } else {
        img_file = def;
    }
    glutInit( argc, argv );
    glutInitWindowSize( 332, 50 );
    glutInitWindowPosition( 200, 50 );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow( "libimage test" );
    glutReshapeFunc( system_resize );
    glutDisplayFunc( system_render );
    system_opengl();
    glutMainLoop();
}

void system_destroy( void )
{
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
	float n2 = img.height / h;
	float n1 = img.width / w;

    usleep( (1.0 / 60.0) * 1000000 );
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

void system_resize( const int width, const int height )
{
    float a = width / (float) height;
    w = (float) width; h = (float) height;

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
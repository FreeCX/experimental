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

image_t img;

void system_init( int *argc, char *argv[] )
{
    glutInit( argc, argv );
    glutInitWindowSize( 700, 300 );
    glutInitWindowPosition( 200, 50 );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow( "libimage test" );
    glutReshapeFunc( system_resize );
    glutDisplayFunc( system_render );
    system_opengl();
    glutMainLoop();
}

void system_opengl( void )
{
    FILE *f;
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    img_load( "../img/image.pcx", &img );
}

void system_render( void )
{
    usleep( (1.0 / 60.0) * 1000000 );
    glClear( GL_COLOR_BUFFER_BIT );
    glRasterPos2f( -3.1f, 0.0f );
    glPixelZoom( 2.0f, 2.0 );
    glDrawPixels( img.width, img.height, GL_RGB, GL_UNSIGNED_BYTE, img.data );
    glutSwapBuffers();
}

void system_resize( const int width, const int height )
{
    const float n = 1.5f;
    const float a = (float) width / (float) height;

    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    if ( width <= height ) {
        glOrtho( -n, n, -n / a, n / a, n, -n );
    } else {
        glOrtho( -n * a, n * a, -n, n, n, -n );
    }
    gluPerspective( 0.0f, a, 0.0f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void system_destroy( void )
{
    free( img.data );
}

int main( int argc, char *argv[] )
{
    system_init( &argc, argv );
    system_destroy();
    return EXIT_SUCCESS;
}
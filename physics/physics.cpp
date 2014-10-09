#include <GL/glut.h>
#include <cmath>
#include "vector.hpp"
#include "simulator.hpp"

int w_width = 500;
int w_height = 500;
int win_id = 0;
GLfloat n = 60.0f;
GLfloat aspect;

simulator lab( 300, 12, 2.0f, 50.0f, 50.0f );

void program_init( void )
{
    GLint sw = glutGet( GLUT_SCREEN_WIDTH );
    GLint sh = glutGet( GLUT_SCREEN_HEIGHT );
    glutPositionWindow( ( sw - w_width ) / 2, ( sh - w_height) / 2 );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable( GL_LINE_SMOOTH );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    lab.setline( -55.0f, 55.0f, -55.0f, -55.0f );
    lab.setline( 55.0f, 55.0f, 55.0f, -55.0f );
    lab.setline( -55.0f, 55.0f, 55.0f, 55.0f );
    lab.setline( -55.0f, -55.0f, 55.0f, -55.0f );

    lab.setline( -30.0f, -30.0f, 30.0f, 30.0f );
}

void program_render( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glColor3f( 1.0f, 1.0f, 1.0f );
    lab.draw();
    glutSwapBuffers();
}

void program_redraw( int value )
{
    lab.step( 1.0f / 30.0f );
    program_render();
    glutTimerFunc( 30, program_redraw, 0 );
}

void program_resize( int width, int height )
{
    aspect = (float) width / height;
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    if ( width <= height ) {
        glOrtho( -n, n, -n/aspect, n/aspect, n, -n );
    } else {
        glOrtho( -n * aspect, n * aspect, -n, n, n, -n );
    }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
void program_keyboard( unsigned char key, int x, int y )
{
    static int fullscreen = 1;
    if ( key == 'q' ) {
        glutDestroyWindow( win_id );
    } else if ( key == 'f' ) {
        if ( fullscreen ) {
            glutFullScreen();
        } else {
            glutReshapeWindow( w_width, w_height );
        }
        fullscreen = !fullscreen;
    }
}

void program_free( void )
{
}

int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitWindowSize( w_width, w_height );
    glutInitWindowPosition( 0, 0 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    win_id = glutCreateWindow( "Physics" );
    glutReshapeFunc( program_resize );
    glutDisplayFunc( program_render );
    glutKeyboardFunc( program_keyboard );
    glutTimerFunc( 1, program_redraw, 0 );
    program_init();
    glutMainLoop();
    program_free();
    return EXIT_SUCCESS;
}

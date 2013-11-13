#include <GL/glut.h>
#include <cmath>

int w_width = 500;
int w_height = 500;
int win_id = 0;
GLfloat n = 60.0f;
GLfloat aspect;

static float * vertex = NULL;
static int segment_g = 16;

/* 3 dot illusion (gif) */
const GLfloat cv[] = {
    0.9f, 1.0f, 0.0f, 1.0f, 0.7f, 0.0f, 1.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 
    1.0f, 0.0f, 0.4f, 1.0f, 0.0f, 0.7f, 0.9f, 0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 
    0.1f, 0.0f, 1.0f, 0.0f, 0.4f, 1.0f, 0.0f, 0.6f, 1.0f, 0.0f, 1.0f, 1.0f, 
    0.0f, 1.0f, 0.6f, 0.0f, 1.0f, 0.4f, 0.1f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 
};

void spVertexInit( int segment )
{
    int j = 0;

    segment_g = segment;
    vertex = new float [2*(segment+2)];
    for ( float i = 0; i <= 360.0f; i += ( 360.0f / segment ) ) {
        vertex[j++] = cos( i * M_PI / 180.0f);
        vertex[j++] = sin( i * M_PI / 180.0f);
    }
    vertex[j++] = 1.0f;
    vertex[j++] = 0.0f;
}

void spDrawCircle3f( float x, float y, float r )
{
    glPushMatrix();
    glTranslatef( x, y, 0 );
    glScalef( r, r, 0 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, vertex );
    glDrawArrays( GL_POLYGON, 0, segment_g + 1 );
    glDisableClientState( GL_VERTEX_ARRAY );
    glPopMatrix();
}

void spFree( void )
{
    delete [] vertex;
}

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
    spVertexInit( 32 );
}

void program_render( void )
{
    const int divider = 8;
    const GLfloat A = 15.0f;
    static GLfloat dt = 0.0f;
    int k;

    dt = dt == M_PI ? 0 : dt;
    glClear( GL_COLOR_BUFFER_BIT );
    glAccum( GL_LOAD, 1.0f );
    for ( int i = 0; i < 1000; i++ ) {
        k = divider;
        glLoadIdentity();
        glColor3f( 0.8f, 0.6f, 0.0f );
        do {
            glColor3fv( &cv[3*(k-1)+0] );
            spDrawCircle3f( 0.0f, A*(sin(dt+k)+2), i / 3000.0f + 1.0f );
            glColor3fv( &cv[3*(k-1)+24] );
            spDrawCircle3f( 0.0f, A*(sin(dt+k)-2), i / 3000.0f + 1.0f );
            glRotatef( -22.5f, 0.0f, 0.0f, 1.0f );
        } while ( --k );
        dt += 0.0002f;
        glAccum( GL_ACCUM, 1.0f / i );
    }
    glAccum( GL_RETURN, 1.0f );
    glFlush();
    glutSwapBuffers();
}

void program_redraw( int value )
{
    program_render();
    glutTimerFunc( 1, program_redraw, 0 );
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
    glMatrixMode(GL_MODELVIEW);
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
    spFree();
}

int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitWindowSize( w_width, w_height );
    glutInitWindowPosition( 0, 0 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    win_id = glutCreateWindow( "Polygon" );
    glutReshapeFunc( program_resize );
    glutDisplayFunc( program_render );
    glutKeyboardFunc( program_keyboard );
    glutTimerFunc( 30, program_redraw, 0 );
    program_init();
    glutMainLoop();
    program_free();
    return EXIT_SUCCESS;
}
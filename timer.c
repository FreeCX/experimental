#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
   
typedef unsigned int uint32;

struct w_timer {
    uint32 usec;
    uint32 count;
    void (*func)(void);
    struct w_timer *next;
};
typedef struct w_timer w_timer_t;

void timer_init( void );
void timer_loop( int signo );
void timer_set( uint32 usec, void (*f)(void) );
void timer_destroy( void );

w_timer_t *p = NULL;

uint32 weTicks( void )
{
    struct timeval tv;
    gettimeofday( &tv, 0 );
    return ( tv.tv_sec * 1000 + tv.tv_usec / 1000 );
}

void timer_init( void )
{
    struct itimerval delay;

    signal( SIGALRM, timer_loop );
    delay.it_value.tv_sec = 0;
    delay.it_value.tv_usec = 1;
    delay.it_interval.tv_sec = 0;
    delay.it_interval.tv_usec = 1000;
    setitimer( ITIMER_REAL, &delay, NULL );
}

void timer_loop( int signo )
{
    static uint32 a, b;
    w_timer_t *t = p;

    a = b;
    b = weTicks();
    while ( t != NULL ) {
        if ( b - a > 1000 ) {
            t->count++;
        } else {
            t->count += b - a;
        }
        if ( t->count >= t->usec ) {
            t->count = 0;
            t->func();
        }
        t = t->next;
    }
}

void timer_set( uint32 usec, void (*func)(void) )
{
    w_timer_t *a = p, *c;

    c = (w_timer_t *) malloc( sizeof(w_timer_t) );
    c->usec = usec;
    c->func = func;
    c->next = p;
    p = c;
}

void timer_destroy( void )
{
    w_timer_t *a = p, *c;

    while ( a != NULL ) {
        c = a->next;
        free( a );
        a = c;
    }
}

void f1( void )
{
    printf( "hello " );
}

void f2( void )
{
    printf( "world!\n" );
}

int main( void )
{
    int count = 0;

    timer_init();
    timer_set( 1000, f1 );
    timer_set( 1001, f2 );
    while ( 1 ) {
        count++;
        usleep( 1000 );
    }
    timer_destroy();
}

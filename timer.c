#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
   
typedef unsigned int uint32;
typedef unsigned long int uint64;

struct w_timer {
    uint64 nsec;
    uint64 count;
    void (*func)(void);
    struct w_timer *next;
};
typedef struct w_timer w_timer_t;

void timer_init( void );
void timer_loop( int signo );
void timer_set( uint64 nsec, void (*f)(void) );
void timer_destroy( void );

w_timer_t *p = NULL;
short n = 0, m = 0;

uint64 weTicks( void )
{
    struct timespec tp;
    clock_gettime( CLOCK_REALTIME, &tp );
    return tp.tv_nsec;
}

void timer_init( void )
{
    struct itimerval delay;

    signal( SIGALRM, timer_loop );
    delay.it_value.tv_sec = 0;
    delay.it_value.tv_usec = 1;
    delay.it_interval.tv_sec = 0;
    delay.it_interval.tv_usec = 5;
    setitimer( ITIMER_REAL, &delay, NULL );
}

void timer_loop( int signo )
{
    static uint64 a = 0, b = 0;
    w_timer_t *t = p;

    a = b;
    b = weTicks();
    while ( t != NULL ) {
        if ( a == 0 ) {
            continue;
        } else {
            t->count += b - a;
        }
        if ( t->count >= t->nsec ) {
            t->func();
            t->count = 0;
        }
        t = t->next;
    }
}

void timer_set( uint64 nsec, void (*func)(void) )
{
    w_timer_t *a = p, *c;

    c = (w_timer_t *) malloc( sizeof(w_timer_t) );
    c->nsec = nsec;
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
    n++;
}

void f2( void )
{
    m++;
}

int main( void )
{
    int count = 0;

    timer_init();
    timer_set( 1, f2 );
    timer_set( 1, f1 );
    while ( count < 1 ) {
        count++;
    }
    timer_destroy();
    printf( "n = %u\nm = %u\n", n, m );
}

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
// #ifdef __WIN32__
// #include <windows.h>
// #endif
   
typedef unsigned int uint32;
typedef unsigned long int uint64;

struct w_timer {
    uint64 nsec;
    uint64 count;
    void (*func)(void);
    struct w_timer *next;
};
typedef struct w_timer w_timer_t;

void timer_init( uint32 usec );
void timer_loop( int signo );
void timer_set( uint64 nsec, void (*f)(void) );
void timer_destroy( void );

w_timer_t *p = NULL;
unsigned short n = 0, m = 0;

// #ifdef __WIN32__
// uint32 tp_usec = 5;
// HANDLE hTimer = NULL;
// HANDLE hThread = NULL;
// #endif

uint64 weTicks( void )
{
// #ifdef __WIN32__
//  uint64 freq, a;

//  QueryPerformanceFrequency( (LARGE_INTEGER *) &freq );
//  QueryPerformanceCounter( (LARGE_INTEGER *) &a );
//  return a / freq;
// #elif __linux__
    struct timespec tp;
    clock_gettime( CLOCK_REALTIME, &tp );
    return tp.tv_nsec;
// #endif
}

void timer_init( uint32 usec )
{
// #ifdef __WIN32__
//     DWORD id;

//     tp_usec = usec;
//     hTimer = CreateEvent( 0, 1, 0, 0 );
//     hThread = CreateThread( 0, 0, timer_loop, 0, 0, &id );
//     SetTimer( 0, 1, 5, (TIMERPROC) timer_loop );
// #elif __linux__
    struct itimerval delay;

    signal( SIGALRM, timer_loop );
    delay.it_value.tv_sec = 0;
    delay.it_value.tv_usec = 1;
    delay.it_interval.tv_sec = 0;
    delay.it_interval.tv_usec = usec;
    setitimer( ITIMER_REAL, &delay, NULL );
// #endif
}

void timer_loop( int signo )
{
    static uint64 a = 0, b = 0;
    w_timer_t *t = p;

// #ifdef __WIN32__
// _the_loop:
//  WaitForSingleObject( hTimer, tp_usec );
// #endif
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
// #ifdef __WIN32__
//     goto _the_loop;
// #endif
}

void timer_set( uint64 nsec, void (*func)(void) )
{
    w_timer_t *c;

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
// #ifdef __WIN32__
//     CloseHandle( hThread );
// #endif
}

void f1( void )
{
    printf( "n = %u\n", n++ );
}

void f2( void )
{
    printf( "m = %u\n", m++ );
}

int main( void )
{
    timer_init( 1000 );
    timer_set( 1E8, f2 );
    timer_set( 9E8, f1 );
    while ( 1 ) {
        usleep( 10000 );
    }
    timer_destroy();
    printf( "n = %u\nm = %u\n", n, m );
}

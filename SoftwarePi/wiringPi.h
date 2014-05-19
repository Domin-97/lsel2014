#ifndef WIRINGPI_H
#define WIRINGPI_H

int wiringPiSetup (void);
int wiringPiISR (int pin, int edgeType, void (*isr)(void));
void pinMode (int pin, int direction);
void digitalWrite (int pin, int value);

#define INT_EDGE_SETUP   0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING  2
#define INT_EDGE_BOTH    3

#define INPUT  0
#define OUTPUT 1

/* interrupt simulation */
void wiringPi_gen_interrupt (int pin);

#endif

/*
  Local variables:
    mode: c
    c-file-style: stroustrup
  End:
*/
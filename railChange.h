/*
 LSEL 2014

 @authors _____, J. Martin
 @date April 2014
 
 */

#ifndef railChange_H
#define railChange_H

#include <stdlib.h>
#include <native/mutex.h>
#include "interp.h"

typedef enum {
	LEFT, RIGHT
} direction_t;

typedef struct railChange_t {
	//int GPIOline;
	uint16_t i2c_address;
	
	direction_t direction;

	RT_MUTEX mutex;
	//pthread_mutex_t mutex;

} railChange_t;

//----------------------------
void railChange_setup(void);
int railChange_cmd(char* arg);

void railChange_init(railChange_t* this,  direction_t direction, uint16_t i2c_address);

railChange_t* railChange_new(direction_t direction, uint16_t i2c_address);

direction_t railChange_get_direction();

void railChange_set_direction(railChange_t* this, direction_t direction);

#endif

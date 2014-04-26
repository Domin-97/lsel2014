#include "Model/observer.h"
#include "Model/Sensors/sensorIR.h"
#include "model.h"
#include "tracker.h"
#include "train.h"


static observer_t tracker_observer;
// Arrays where model elements are stored
static struct ir_sensor_data_t {
		sensorIR_t* sensor;
		char sector;
} tracker_ir_sensors [MAXSENSORS];
 
static struct train_data_t {
		train_t* train;
		int IRsimbolicId;
} tracker_trains [MAXTRAINS];

static struct railway_data_t {
		railway_t* train;
		int platform;
} tracker_railway [MAXRAILWAY];
static int n_ir_sensors;
static int n_trains;
static int n_railway;
static event_t* event;
// Translate the ID ir sensors returns into the actual train pointer
train_t*
tracker_gen_train(int id){
     struct train_data_t* t;             
     for (t = tracker_trains; t->train; ++t) {
         if ( id == t-> IRsimbolicId )
         return t->train;
     }
     return NULL;
}  
void
timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}
                   
// Notify checks registered sensors and if some of them
// has an event and it's not a rebound updates the model.
static
void tracker_notify (observer_t* this, observable_t* foo)
{
	struct ir_sensor_data_t* p;
	struct telemetry_t* tel;
	struct timeval diff , now ,last;
	struct train_t* train;
	float speed;
	for (p = tracker_ir_sensors; p->sensor; ++p) {
	    event = sensorIR_get_event(p->sensor);
	    if (event->flag == 1) {
	    	//printf( " he cogido a %d en %d \n", event->passingTrain,p->sector);
	    	train = tracker_gen_train(event->passingTrain);
	    	train_set_current_sector (train , p->sector);
	    	tel = train_get_telemetry(train);
	    	last = tel-> timestamp;
	    	gettimeofday (&now, NULL);
	    	timeval_sub ( &diff , &now , &last );
	    	speed = LENGHTSECTOR / diff -> tv_usec ;
	    	train_set_current_speed(train , speed );
	    	//railway_register_train(tracker_gen_train(event->passingTrain), p->sector);
      	}
    }
}

void
tracker_init (void)
{
    // Struct with the names and associated sector of the sensors
	static struct ir_name_t {
			const char* name;
			char sector;
	} ir_names [] = {
			{ "IRsensor[0]", 0 },
			{ "IRsensor[1]", 1 },
			{ "IRsensor[2]", 2 },
			{ "IRsensor[3]", 3 },
			{ NULL, 0 }
	};
	// Struct with the names and the ID of each train in the IR sensors
	static struct train_name_t {
			const char* name;
			int IRsimbolicId;
	} train_names [] = {
			{ "Diesel", 4 },
			{ "Renfe", 3 },
			{ NULL, 0 }
	};
	static struct railway_name_t {
			const char* name;
			int platform;
	} railway_names [] = {
			{ "via[0]", 0 },
			{ NULL, 0 }
	};
	struct ir_name_t* s;
	struct train_name_t* t;
	struct railway_name_t* r;
	observer_init (&tracker_observer, tracker_notify);
	
	n_ir_sensors = 0;
	n_trains=0;
	n_railway=0;
	// those "for" takes the elements in the model, cast them to 
	// his type ( since all the elements in the model are observables)
	// and store them in the appropriate struct.
	for (s = ir_names; s->name; ++s) {
		observable_t* obs = model_get_observable (s->name);
		observable_register_observer (obs, &tracker_observer);
		tracker_ir_sensors[n_ir_sensors].sensor = (sensorIR_t*) obs;
		tracker_ir_sensors[n_ir_sensors].sector = s->sector;
		++n_ir_sensors;
	}
	
	for (t = train_names; t->name; ++t) {
		observable_t* obs = model_get_observable (t->name);
		observable_register_observer (obs, &tracker_observer);
		tracker_trains[n_trains].train = (train_t*) obs;
		tracker_trains[n_trains].IRsimbolicId = t->IRsimbolicId;
		++n_trains;
	}
	
	for( r = railway_names; t->name; ++r) {
		observable_t* obs = model_get_observable (r->name);
		observable_register_observer (obs, &tracker_observer);
		tracker_railway[n_railway].train = (railway_t*) obs;
		tracker_railway[n_railway].platform = r->platform;
		++n_railway;
	}
}


  


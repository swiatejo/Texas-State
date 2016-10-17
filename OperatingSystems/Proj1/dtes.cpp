nclude <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <queue>
using namespace std;

//event definitions
#define ARRIVAL 1
#define DEPART 2
#define TIME_SLICE 3


struct event{
        float time;
        int   type;
//      struct event *next;
        bool operator>(const event& rhs){
           return time > rhs.time;
        }
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <queue>
using namespace std;

//event definitions
#define ARRIVAL 1
#define DEPART 2
#define TIME_SLICE 3


struct event{
        float time;
        int   type;
//      struct event *next;
        bool operator>(const event& rhs){
           return time > rhs.time;
        }
};

//Prototypes for Function
void init();
int run_sim(int sim);
void generate_report();
int schedule_event(struct event*);
int process_arrival(struct event* eve);
int process_departure(struct event* eve);
int process_timeslice(struct event* eve);

//Global Variables
struct event* head;     //head of event queue
float sim_clock;                //simulator clock

//Function Definitions


//Function: init()
//initializes all variables, states, and end conditions
//schedule first events
void init()
{
  
  priority_queue<event*> e_queue;
  e_queue.push( event(3.00, 1) );
}

//Function: run_sim(int sim)
//runs the simulator based on type chosen
int run_sim(int sim)
{
return 0;
}

//Function:generate_report()
void generate_report()
{
}

//Function:schedule_event(struct event*)
//Schedule a new event
int schedule_event(struct event* )
{
return 0;
}

//Function: process_arrival(struct event* eve)
//Process an arrival event
int process_arrival(struct event* eve)
{
return 0;
}

//Function: process_departure(struct event* eve)
//Process a departure event
int process_departure(struct event* eve)
{
return 0;
}

//Function: process_timeslice(struct event* eve)
//Processes a time slice event
int process_timeslice(struct event* eve)
{
return 0;
}
 
//Function: urand()
//Returns a random number between 0 and 1
float urand()
{
   return ( (float) rand()/RAND_MAX );
}

//Function: genexp()
//returns a random number that follows an exponential distribution
float genexp(float lambda)
{
{
{
   float u,x;
   x = 0;
   while (x == 0)
     {
        u = urand();
        x = (-1/lambda)*log(u);
     }
   return x;
}

int main(int argc, char* argv[])
{
    
   float service_time;          //service time amount
   int arrival_rate,            //arrival rate amount
       quantum,                 //quantum amount
       sim_number;              //simulation number



      if (argc < 3)
      {
        cout << "(Simulator Choice (1-4)) (Arrival Time) (Service Time)  (Quantum(Round Robin Only))" << endl;
      }
      else 
      {
       istringstream(argv[1]) >> sim_number;
       istringstream(argv[2]) >> arrival_rate;
       istringstream(argv[3]) >> service_time;
       if (argc == 5)
         istringstream(argv[4]) >> quantum;
      }

      cout << urand() << "    " << genexp(service_time) << endl;



   //cin.ignore(999, '\n');
   cout << "Press Enter or Return when ready...";
   cin.get();
   return EXIT_SUCCESS;
}


#include "process.cpp"  // user defined process class
#include <vector>       // used for queueing
#include <algorithm>    // used for sort();
#include <sstream>
#include <queue>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////
// events
#define ARRIVAL 1
#define DEPARTURE 2
#define QUANTUM 3
//

//////////////////////////////////////////////////////     

/////////////////////////////////////////////////////
// function definition
void init();
int run_sim(int scheduler);
void generate_report(int scheduler);
void schedule_event();
float genexp(float lambda);
void FCFS();   // first come first serve
void SRTF();   // shortest remaing time first 
void HRRN();   // highest response ratio next
void RR();     // round robin

////////////////////////////////////////////////////////////////
//Global variables
bool cpuBusy;			//boolean for if cpu is busy
float turnaround;		//float for storing turnaround time of system
float utilization;		//float for storing utilization of cpu
float throughput;		//float for storing total throughput of system
float avg_proc_in_Q;		//float for storing average number of processes in ready Q
float simClock; 		//simulation clock
float avgServiceTime;
float lambda;			
float sumServiceTime;
float sumWait;
float sumArrivalRate;
int quantum;
int processCount = 0;
int endSimulation = 10000; // how many processes to finish before end sim
vector <process> eventQ;

////////////////////////////////////////////////////////////////
void init()
{
    // initialize all variables, states, and end conditions
    // schedule first events
    simClock = 0;
    sumServiceTime = 0;
    sumWait = 0;
    sumArrivalRate = 0;
    cpuBusy = false;

    for(int i = 0; i < 3; i++)
    {
        float randServiceTime = genexp(1/avgServiceTime);
        process temp(randServiceTime, 0, ++processCount, ARRIVAL);
        eventQ.push_back(temp);
    }
}
////////////////////////////////////////////////////////////////
void generate_report(int sched)
{
    float avgLambda = processCount/sumArrivalRate;

    // calculate statistics
    turnaround = ((sumServiceTime + sumWait)/endSimulation);
    throughput = endSimulation/simClock;
    utilization = (avgLambda) / (endSimulation/simClock);
    avg_proc_in_Q = ((turnaround*avgLambda) - ((avgLambda*(sumServiceTime/endSimulation))/avgLambda));

    //file output for each scheduler, creates new file for each one and appends data
    ofstream ofs;
    switch(sched){
    case 1:
    ofs.open ("fcfs.txt", std::ofstream::out | std::ofstream::app);
    ofs << lambda << "," << turnaround << "," << throughput << "," << utilization << "," << avg_proc_in_Q << std::endl;
    break;
    case 2:

    break;
    case 3:
    ofs.open ("hrrn.txt", std::ofstream::out | std::ofstream::app);
    ofs << lambda << "," << turnaround << "," << throughput << "," << utilization << "," << avg_proc_in_Q << std::endl;
    break;
    case 4:

    break;
    default:
    break;
    ofs.close();
    }
}
//////////////////////////////////////////////////////////////// 
//schedules an event 
void schedule_event()
{  

    // generate random service time and arrival time
    float randServiceTime = genexp(1/avgServiceTime);
    float randArrivalTime = genexp(lambda);

    sumArrivalRate += randArrivalTime;

    //set new event with serviceTime, arrivalTime, pid, processType
    process temp(randServiceTime, 
            simClock + randArrivalTime, 
            ++processCount, 
            ARRIVAL);
    eventQ.push_back(temp);
}
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
    return( (float) rand()/RAND_MAX );
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp(float lambda)
{
    float u,x;
    x = 0;
    while (x == 0)
    {
        u = urand();
        x = (-1/lambda)*log(u);
    }
    return(x);
}
/////////////////////////////////////////////////////////////
// call proper simulation based on scheduler parameter passed
int simulate(int scheduler)
{
    switch (scheduler)
    {
        case 1:
            FCFS();  // first come first serve
            break;
        case 2:
            SRTF();  // shortest remaining time first
            break;
        case 3:
            HRRN();  // highest response ratio next ((wait+service time)/service time)
            break;
        case 4:
            RR();    // round robin
            break;
        default:
            cout<<"improper scheduler input (1-4 requested)"<<endl
                << "(scheduler # 1-4) (lambda # 1-30) (service time - .06) (quantum - .01 or .2)" << endl;
            break;
    }
    return 0;
}

//////////////////////////////////////

void FCFS()    // first come first serve
{
    vector<process> readyQ;
    int totalProcess = 0;
    unsigned int eqSize = eventQ.size();
    while (totalProcess < endSimulation)
    {
        //event Q has already been loaded with some events, sorted based on earliest arrival time
        sort(eventQ.begin(), eventQ.end(), sorter3);


        switch (eventQ.front().get_type())
        {
            case ARRIVAL: //arrival event
                
                simClock = eventQ.front().get_arrival(); //update system clock
                schedule_event(); //schedule another evetn

                if (cpuBusy == false) // cpu not busy
                {
                    cpuBusy = true; //set cpu to busy
                    eventQ.front().set_type(DEPARTURE); //change event to departure event
                    eventQ.front().set_arrival(simClock + eventQ.front().get_serviceTime()); // set event time to clock time + service time of event
                }
                else // cpu is busy
                {
                    readyQ.push_back(eventQ.front());  //put event in ready Q
                    eventQ.erase(eventQ.begin()); // remove event from event Q
                }

                break;
            case DEPARTURE: //departure event
		
                simClock = eventQ.front().get_arrival(); // update system clock
                sumServiceTime += eventQ.front().get_serviceTime(); // running total of service times for statistics
                //sumServiceTime += eventQ.front().get_serviceTime(); // running total of service times for statistics
                
                if(readyQ.empty()) // ready Q empty
                {
                    cpuBusy = false; // cpu not busy 
                    eventQ.erase(eventQ.begin()); // remove departure event from event Q
                }
                else // ready Q has processes in it
                {
                    readyQ.front().been_seen(simClock - readyQ.front().get_arrival()); // set wait time
                    sumWait += readyQ.front().get_totalWait(); // add wait time to accumulator for statistics later
                
                    readyQ.front().set_type(DEPARTURE); //change event to departure
                    readyQ.front().set_arrival(simClock + readyQ.front().get_serviceTime()); // set time of event to clock + service time of event

                    eventQ.push_back(readyQ.front());  // put event back in event Q
                    readyQ.erase(readyQ.begin()); // remove event from ready Q
                }
                totalProcess++; //update total events completed
               
                break;

            default:
                // error
                break;
        }
    }
}

void SRTF()    // shortest remaing time first 
{
}

void HRRN()    // highest response ratio next
{
    vector<process> readyQ;
    int totalProcess = 0;
    unsigned int eqSize = eventQ.size();
    while (totalProcess < endSimulation)
    {
        //event Q already filled with some events that arrived at 0, sorting each time we add more events by earliest arrival time
        sort(eventQ.begin(), eventQ.end(), sorter3);


        switch (eventQ.front().get_type())
        {
            case ARRIVAL: //arrival event
                simClock = eventQ.front().get_arrival();  //update system clock 
                schedule_event(); // schedule another arrival event
                if (cpuBusy == false) // cpu not busy
                {
                    cpuBusy = true; // set cpu to busy
                    eventQ.front().set_type(DEPARTURE);	//change event at front of event Q to departure event
                    eventQ.front().set_arrival(simClock + eventQ.front().get_serviceTime()); //set the new time of the event to clock + service time of event
                }
                else //cpu is busy
                {
                    readyQ.push_back(eventQ.front()); // add event at front of event Q to ready Q 
                    eventQ.erase(eventQ.begin()); // remove event from front of event Q
                }

                break;
            case DEPARTURE: //departure event
               
                simClock = eventQ.front().get_arrival(); //update system clock
                sumServiceTime += eventQ.front().get_serviceTime(); // running total of service times for statistics

                if(readyQ.empty())
                {
                    cpuBusy = false;
                    eventQ.erase(eventQ.begin());
                }
                else
                {

                    float priorityHRRN;  //variable to hold HRRN

                    //loop through the entire ready Q and calculate the HRRN for each process
                    for(unsigned int i = 0; i < readyQ.size(); i++)
                    {
                      readyQ[i].set_wait(simClock - readyQ[i].get_arrival());
                      priorityHRRN = ((readyQ[i].get_totalWait() + readyQ[i].get_serviceTime())/(readyQ[i].get_serviceTime()));
                      readyQ[i].set_priority(priorityHRRN);

                    }
                    //sort readyQ based on calculated HRRN for each process in ready Q
                    sort(readyQ.begin(), readyQ.end(), sorter2);

                    sumWait += readyQ.front().get_totalWait();
                    eventQ.erase(eventQ.begin());

                    readyQ.front().set_type(DEPARTURE);
                    readyQ.front().set_arrival(simClock + readyQ.front().get_serviceTime());

                    eventQ.push_back(readyQ.front());
                    readyQ.erase(readyQ.begin());
                }
                totalProcess++;
              

                break;

            default:
                // error
                break;
        }
    }
}

void RR()      // round robin
{
}

////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    // parsing arguments 
    if (argc < 3)
    {
        cout<<"At least 3 arguements seperated by spaces needed:"<<endl;
        cout<<"1st arguement: (1-4) to pick scheduler"<<endl;
        cout<<"2nd arguement: (Average Arrival Time)"<<endl;
        cout<<"3rd arguement: (Average Service Time)"<<endl;
        cout<<"4th arguement: (Size of Quantum) for Round Robin"<<endl;
        cout<<"Example: 4 5 0.06 0.01"<<endl;
        return 0;
    }
    int scheduler;
    istringstream(argv[1]) >> scheduler;
    istringstream(argv[2]) >> lambda;
    istringstream(argv[3]) >> avgServiceTime;
    if (argc == 5)
    {
        istringstream(argv[4]) >> quantum;
    }


    init();
    simulate(scheduler); 
    generate_report(scheduler);
    return 0;
}

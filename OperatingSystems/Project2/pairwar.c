#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS  4

//Function declarations
void* dealer(void*);
void* player(void*);
void printHandLog(int);
void printDeckLog();
void printHandCon(int);
void printDeckCon();
void shuffle_deck();
void create_threads(int);

//global condition and mutex
pthread_mutex_t mutex;
pthread_cond_t cond;

//global varialbles
FILE *logfile;
void *ptrID;
int seed;
int deck[52];
int player1[2];
int player2[2];
int player3[2];
int thread_ids[4] = {0,1,2,3};
int startPlayer;
int topOfDeck = 0;
int roundWin = 0;
int t1=0, t2=1, t3=2, t4=3;
pthread_t threads[4];


//Fucntion to shuffle cards
void shuffle_deck()
{
        int n, i, temp;
        for (i = 0; i < 52; ++i){
           n = rand() % 52;
           temp = deck[i];
           deck[i] = deck[n];
           deck[n] = temp;
	}
}

//Function: dealer; for dealer thread
void* dealer(void* t){
	
	//lock the mutex
	pthread_mutex_lock(&mutex);

	//assign thread id
	int my_id = *((int*)t);

	fprintf(logfile, "DEALER: shuffle \n");
	//initialize deck to ordered 0-51
	int j;
	for (j =0; j<52; ++j){
		deck[j] = j;
	}

	//set rand() seed
	srand(seed);

	//shuffle the deck of cards
	shuffle_deck();

	//Deal first round of cards
	player1[0] = deck[0] % 13;
	player2[0] = deck[1] % 13;
	player3[0] = deck[2] % 13;

	topOfDeck = 3;
	startPlayer = 1;

	//unlock the mutex and broadcast cond so players can play
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond);

	//dealer waits for players to play
        int i;
	for (i=1; i<NUM_THREADS; i++){
           pthread_join(threads[i], NULL);
  	}

	//reset globals
	startPlayer = -1;
	topOfDeck = 0;
	roundWin = 0;

	//lock the mutex
	pthread_mutex_lock(&mutex);

	fprintf(logfile, "DEALER: shuffle \n");

	//Need to start new player threads
	create_threads(3);

	//Begin round 2
	//initialize deck to 1-51
	for (j =0; j<52; ++j){
		deck[j] = j;
	}

	//shuffle the deck of cards
	shuffle_deck();

	//Deal second round of cards
	player2[0] = deck[0] % 13;
	player3[0] = deck[1] % 13;
	player1[0] = deck[2] % 13;

	topOfDeck = 3;
	startPlayer = 2;

	//unlock the mutex and braodcast cond
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond);

	//dealer waits for players to play
	for (i=1; i<NUM_THREADS; i++){
           pthread_join(threads[i], NULL);
  	};

	//reset globals
	startPlayer = -1;
	topOfDeck = 0;
	roundWin = 0;

	//lock the mutex
	pthread_mutex_lock(&mutex);

	fprintf(logfile, "DEALER: shuffle \n");
	
	//create new threads 
	create_threads(3);

	//shuffle cards for round 3
	//initialize deck to 1-51
	for (j =0; j<52; ++j){
		deck[j] = j;
	}

	//shuffle the deck of cards
	shuffle_deck();

	//Deal third round of cards
	player3[0] = deck[0] % 13;
	player1[0] = deck[1] % 13;
	player2[0] = deck[2] % 13;

	topOfDeck = 3;
	startPlayer = 3;
	
	//unlock the mutex and broadcast cond
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond);

	//exit thread
	pthread_exit(NULL);
}

//function: player; for player threads
void* player(void* t){

	//record this threads id
	int my_id = *((int*)t);
	int n;

	while(roundWin == 0){

		//lock the mutex
		pthread_mutex_lock(&mutex);

		//have other threads of player wait for the startPlayer
		while (my_id != startPlayer)
			pthread_cond_wait(&cond, &mutex);

		//break out if round is won
		if (roundWin==1){break;}
		
		//check player id and play that hand
		if (my_id == 1){
			printHandLog(my_id);
			player1[1] = deck[topOfDeck] % 13;
			++topOfDeck;
			fprintf(logfile, "PLAYER 1: draws %i\n", player1[1]);
			fprintf(logfile, "PLAYER 1: hand %i ", player1[0]);
			fprintf(logfile, "%i\n", player1[1]);
			printHandCon(my_id);
			fflush(logfile);
			if (player1[0] == player1[1]){
				//output win
				fprintf(logfile, "PLAYER 1: wins \n");
				fprintf(logfile, "PLAYER %i: exits round \n", my_id);
				fprintf(stdout, "PLAYER 1: wins \n");
				printDeckCon();
				roundWin = 1;
				fflush(logfile);
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
				pthread_exit(NULL);
			}else{
				//random a card to discard
				//if it's card 0 swap [1] to [0]
				fprintf(stdout, "PLAYER 1: loses \n");
				printDeckCon();
				n = rand() % 2;
				if (n == 0){
					fprintf(logfile, "PLAYER 1: discards %i\n", player1[0]);
                                        player1[0] = player1[1];
					printHandLog(my_id);
					printDeckLog();
				}else{
					fprintf(logfile, "PLAYER 1: discards %i\n", player1[1]);
					printHandLog(my_id);
					printDeckLog();
				}
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
			}
		}
		else if (my_id == 2){
			printHandLog(my_id);
			player2[1] = deck[topOfDeck] % 13;
			++topOfDeck;
			fprintf(logfile, "PLAYER 2: draws %i\n", player2[1]);
			fprintf(logfile, "PLAYER 2: hand %i ", player2[0]);
			fprintf(logfile, "%i\n", player2[1]);
			printHandCon(my_id);
			if (player2[0] == player2[1]){
				//output win
				fprintf(logfile, "PLAYER 2: wins \n");
				fprintf(logfile, "PLAYER %i: exits round \n", my_id);
				fprintf(stdout, "PLAYER 2: wins \n");
				printDeckCon();
				roundWin = 1;
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
				pthread_exit(NULL);
			}else{
				//random a card to discard
				//if it's card 0 swap [1] to [0]
				fprintf(stdout, "PLAYER 2: loses \n");
				printDeckCon();
				n = rand() % 2;
				if (n == 0){
					fprintf(logfile, "PLAYER 2: discards %i\n", player2[0]);
                                        player2[0] = player2[1];
					printHandLog(my_id);
					printDeckLog();
				}else{
					fprintf(logfile, "PLAYER 2: discards %i\n", player2[1]);
					printHandLog(my_id);
					printDeckLog();
				}
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
			}
		}
		else if (my_id == 3){
			player3[1] = deck[topOfDeck] % 13;
			printHandLog(my_id);
			++topOfDeck;
			fprintf(logfile, "PLAYER 3: draws %i\n", player3[1]);
			fprintf(logfile, "PLAYER 3: hand %i ", player3[0]);
			fprintf(logfile, "%i\n", player3[1]);
			printHandCon(my_id);
			if (player3[0] == player3[1]){
				//output win
				fprintf(logfile, "PLAYER 3: wins \n");
				fprintf(logfile, "PLAYER %i: exits round \n", my_id);
				fprintf(stdout, "PLAYER 3: wins \n");
				printDeckCon();
				roundWin = 1;
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
				pthread_exit(NULL);
			}else{
				//random a card to discard
				//if it's card 0 swap [1] to [0]
				fprintf(stdout, "PLAYER 3: loses \n");
				printDeckCon();
				n = rand() % 2;
				if (n == 0){
					fprintf(logfile, "PLAYER 3: discards %i\n", player3[0]);
                                        player3[0] = player3[1];
					printHandLog(my_id);
					printDeckLog();
				}else{
					fprintf(logfile, "PLAYER 3: discards %i\n", player3[1]);
					printHandLog(my_id);
					printDeckLog();
				}
				startPlayer++;
    			if (startPlayer == 4)
        			startPlayer = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond);
			}
		}
	}

	//if a player won the round exit
	fprintf(logfile, "PLAYER %i: exits round \n", my_id);
	startPlayer++;
    if (startPlayer == 4)
        startPlayer = 1;
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond);
	pthread_exit(NULL);
}

//Fucntion to create threads for dealer and players
void create_threads(int thread)
{
	if (thread == 4)
	{
	   //create dealer thread
           ptrID = &t1;
           pthread_create(&threads[0], NULL, dealer, ptrID);

           //create player1 thread
           ptrID = &t2;
           pthread_create(&threads[1], NULL, player, ptrID);

           //create player2 thread
           ptrID = &t3;
           pthread_create(&threads[2], NULL, player, ptrID);

           //create player3 thread
           ptrID = &t4;
           pthread_create(&threads[3], NULL, player, ptrID);
         }
        else
	{
          //create player1 thread
          ptrID = &t2;
          pthread_create(&threads[1], NULL, player, ptrID);

          //create player2 thread
          ptrID = &t3;
          pthread_create(&threads[2], NULL, player, ptrID);

          //create player3 thread
          ptrID = &t4;
          pthread_create(&threads[3], NULL, player, ptrID);
	}
}
//Function to print out the players hand to log.txt
void printHandLog(int play_id)
{
	switch (play_id){
	case 1:
		fprintf(logfile, "PLAYER %i: hand ", play_id);
		fprintf(logfile, "%i", player1[0]); 
    	fprintf(logfile,"\n");
		break;
	case 2:
		fprintf(logfile, "PLAYER %i: hand ", play_id);
		fprintf(logfile, "%i", player2[0]); 
    	fprintf(logfile,"\n");
		break;
	case 3:
		fprintf(logfile, "PLAYER %i: hand ", play_id);
		fprintf(logfile, "%i", player3[0]); 
    	fprintf(logfile,"\n");
		break;
	}
}

//Function to print out the players hand to console
void printHandCon(int play_id)
{
	switch (play_id){
	case 1:
		fprintf(stdout, "PLAYER 1: hand %i ", player1[0]);
		fprintf(stdout, "%i", player1[1]); 
    	fprintf(stdout,"\n");
		break;
	case 2:
		fprintf(stdout, "PLAYER 2: hand %i ", player2[0]);
		fprintf(stdout, "%i", player2[1]); 
    	fprintf(stdout,"\n");
		break;
	case 3:
		fprintf(stdout, "PLAYER 3: hand %i ", player3[0]);
		fprintf(stdout, "%i", player3[1]); 
    	fprintf(stdout,"\n");
		break;
	}
}

//Function to print out the deck to log.txt
void printDeckLog()
{
	fprintf(logfile,"DECK: ");
	int x;
	for (x=topOfDeck; x<52; ++x){
       fprintf(logfile,"%i ", deck[x]);
    }
    fprintf(logfile,"\n");
}

//Function to print out the deck to console
void printDeckCon()
{
	fprintf(stdout,"DECK: ");
	int x;
	for (x=topOfDeck; x<52; ++x){
       fprintf(stdout,"%i ", deck[x]);
    }
    fprintf(stdout,"\n");
}


//Begining of MAIN()
int main(int argc, char *argv[]){
	
	//check for argument
	if (argc !=2){
		fprintf(stderr,"Failed to find argument for seed. \n");
		return(-1);
	}
	//Create Log file
        logfile = fopen("log.txt","a");

	//check file is good
        if (logfile == NULL){
		fprintf(stderr,"Failed to open Log File.\n");
		return(-1);
	}

	//print start of log file
        fprintf(logfile,"\n*************************************************************\nSTART NEW GAME!\n\n");

	//Initialize cond/mutex and IDs for threads
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	//assign seed argument
	seed = atoi(argv[1]);

	//create dealer thread
        create_threads(4);

	//Join threads back together
	int i;
	for (i=0; i<NUM_THREADS; i++){
           pthread_join(threads[i], NULL);
  	}

	//Garbage collection
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return(0);
}
//End of MAIN()

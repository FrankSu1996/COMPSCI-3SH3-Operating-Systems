#include <pthread.h>
#include <stdio.h>

//max number of philosophers
#define MAX_PHILOSOPHER 5
//max sleep time in seconds
#define MAX_SLEEP_TIME 3

// state of each philosopher either thinking, hungry, or eating
enum
{
    THINKING,
    HUNGRY,
    EATING
};
int state[MAX_PHILOSOPHER];

//arguments to pass to pthread_create
int philosopher_id[MAX_PHILOSOPHER];

//condition variables and mutex lock
pthread_cond_t condition_variables[MAX_PHILOSOPHER];
pthread_mutex_t mutex;

//array of thread
pthread_t thread_id[MAX_PHILOSOPHER];

//function to initialize synchronization data structures
void init()
{
    //initialize all philosophers to be in thinking state
    for (int i = 0; i < MAX_PHILOSOPHER; i++)
    {
        state[i] = THINKING;
        philosopher_id[i] = i;
        pthread_cond_init(&condition_variables[i], NULL);
    }

    //init mutex
    pthread_mutex_init(&mutex, NULL);
}

//function to simulate eating: will sleep for eating_time seconds
void eat(int eating_time, int philosopherNumber)
{
    printf("Philosopher %d is eating...\n", philosopherNumber);
    sleep(eating_time);
}

//function to simulate thinking: will sleep for thinking_time seconds
void think(int thinking_time, int philosopherNumber)
{
    printf("Philosopher %d is thinking...\n", philosopherNumber);
    sleep(thinking_time);
}

//start routine for each philosopher thread
void *philosopherStartRoutine(void *id)
{
    int *x = (int *)id;
    int philosopherNumber = *x;
    int sleep_time;

    srandom((unsigned)time(NULL));

    //run this loop forever
    while (1)
    {
        sleep_time = (int)((random() % MAX_SLEEP_TIME + 1));

        //each philospher starts off thinking
        think(sleep_time, philosopherNumber);

        //then picks up forks, eats, and puts forks back down
        pickup_forks(philosopherNumber);
        
        sleep_time = (int)((random() % MAX_SLEEP_TIME + 1));
        eat(sleep_time, philosopherNumber);
        return_forks(philosopherNumber);
        
    }
}

//function to return the id of the philosopher to the left
int left_neighbor_id(int n)
{
    if (n == 0)
    {
        return MAX_PHILOSOPHER - 1;
    }
    else
    {
        return n - 1;
    }
}

//function to return the id of the philosopher to the right
int right_neighbor_id(int n)
{
    if (n == MAX_PHILOSOPHER - 1)
    {
        return 0;
    }
    else
    {
        return n + 1;
    }
}

//function to check the states of philosophers
//if left and right neighbors aren't eating, then philosopher i will eat
void check_philosopher_state(int i)
{

    if (state[left_neighbor_id(i)] != EATING && state[right_neighbor_id(i)] != EATING && state[i] == HUNGRY)
    {
        //set state of phil i to eating, and signal condition variable
        state[i] = EATING;
        pthread_cond_signal(&condition_variables[i]);
    }
}

//function to pick up forks
void pickup_forks(int i)
{
    //first must lock mutex
    pthread_mutex_lock(&mutex);

    state[i] = HUNGRY;
    check_philosopher_state(i);

    while (state[i] != EATING)
    {
        pthread_cond_wait(&condition_variables[i], &mutex);
    }

    //unlock mutex
    pthread_mutex_unlock(&mutex);
}

//function to return forks
void return_forks(int i)
{
    pthread_mutex_lock(&mutex);

    state[i] = THINKING;
    check_philosopher_state(left_neighbor_id(i));
    check_philosopher_state(right_neighbor_id(i));

    pthread_mutex_unlock(&mutex);
}

int main()
{
    //call init function for setup
    init();

    //create philosopher threads
    for (int i = 0; i < MAX_PHILOSOPHER; i++)
    {
        pthread_create(&thread_id[i], 0, philosopherStartRoutine, (void *)&philosopher_id[i]);
    }

    //join theads
    for (int i = 0; i < MAX_PHILOSOPHER; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
}
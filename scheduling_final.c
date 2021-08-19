//Dhruv Shetty

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Process constructor
struct Process{
    int pid;
    int cpu_time;
    int io_time;
    int arrival_time;
    int time_remaining;
    int rr_runtime;
    int time_to_wait;
    int cycle_finished;
    int state;
    struct Process * next;
};

// Queue constructor
struct Queue{
    struct Process * front;
    struct Process * end; 
    int size;
};

// Function declarations
void fcfs (struct Process *p, int num_processes, char output[]);
void round_robin (struct Process *p, int num_processes, char output[]);
void shortest_first (struct Process *p, int num_processes, char output[]);

int main(int argc, char *argv[]) {

    // Read input text file and create processes based on inputs given
    FILE * fp = fopen(argv[1], "r");
    int i = 0;
    char parser[20];
    char file[255] = "";

    fscanf(fp, "%s", parser);
    strcat(file, parser);
    int num_processes = file[0]-48;

    for(i=0; i< (num_processes*4); i++){
        fscanf(fp, "%s", parser);
        strcat(file, parser);
    }
    fclose(fp);

    int index = 1;
    struct Process * p = malloc(num_processes * (sizeof *p));

    for(i = 0; i < num_processes; i++) {
        p[i].pid = (int)file[index]-48;
        index++;
        p[i].cpu_time = (int)file[index]-48;
        index++;
        p[i].io_time = (int)file[index]-48;
        index++;
        p[i].arrival_time = (int)file[index]-48;
        index++;
        p[i].time_remaining = p[i].cpu_time;
        p[i].time_to_wait = -1;
    }

    printf("\n");
    printf("\n");

    char arg = *argv[2];

    char output[40];
    strncpy(output, argv[1], strlen(argv[1])-4);
    output[strlen(argv[1])-4] = 0;

    // Determine which scheduling algorithm is used
    if(arg == '0'){
        strcat(output,"-0.txt");
        fcfs(p, num_processes, output);
    }
    else if(arg == '1'){
        strcat(output,"-1.txt");
        round_robin(p, num_processes, output);
    }
    else if(arg == '2'){
        strcat(output,"-2.txt");
        shortest_first(p, num_processes, output);
    }
    return 0;
}

// First-come-first-serve algorithm
void fcfs (struct Process *p, int num_processes, char output[]){
    // Output file creation
    FILE * f = fopen(output, "w");

    // Creates queue to store processes
    struct Queue * list;
    list = malloc(sizeof(struct Queue));
    list->front = NULL;
    list->end = NULL;
    list->size = 0;

    int cycle_number = 0;
    int loop = 1;
    double total_cpu_runtime = 0;

    // Main while loop. Ends when all processes are finished
    while(loop){
        for(int i = 0; i < num_processes; i++){
            // Checks if process is ready to be added to the queue
            if(p[i].arrival_time == cycle_number || p[i].time_to_wait == cycle_number){ 
                p[i].state = 1;

                p[i].next=0;
                if(list->front == 0)
                    list->front = &p[i];
                else
                    list->end->next = &p[i];
                list->end = &p[i];
                list->size++;                    
            }
        }
        
        // Runs process
        if(list->size > 0){
            // Checks if process state must turn into I/O (blocked)
            if((list->front->time_remaining == (list->front->cpu_time/2))){
                list->front->state = 3;
                list->front->time_to_wait = list->front->io_time + cycle_number;
                
                // Removes first process
                list->front = list->front->next;
                list->size--;
                
                // New process is running
                if(list->size > 0){
                    list->front->state = 2;
                    list->front->time_remaining--;
                }
            }
            // Checks if process is terminated
            else if(list->front->time_remaining == 0){
                list->front->state = 4;
                list->front->cycle_finished = cycle_number;

                // Remove process from queue
                list->front = list->front->next;
                list->size--;

            // New process is running
                if(list->size > 0){
                    list->front->state = 2;
                    list->front->time_remaining--;
                }
            }
            else{
            // Front process runs for a cycle
                list->front->state = 2;
                list->front->time_remaining--;
            }  
        }
        total_cpu_runtime++;
        int count = 0;
        // Checks if at least one process has time remaining (while loop runs again)
        for(int i = 0; i < num_processes; i++) {
            if(p[i].time_remaining != 0){
                loop = 1;
                break;
            }
            count++;
        }
        
        // Write cycle to output file
        fprintf(f, "%d ",cycle_number);
        printf("%d ",cycle_number);
        for(int i = 0; i < num_processes; i++) {
            if(p[i].state == 1){
                fprintf(f, "%d:%s ", p[i].pid, "ready");
                printf("%d:%s ", p[i].pid, "ready");
            }
            if(p[i].state == 2){
                fprintf(f, "%d:%s ", p[i].pid, "running");
                printf("%d:%s ", p[i].pid, "running");
            }
            if(p[i].state == 3){
                fprintf(f, "%d:%s ", p[i].pid, "blocked");
                printf("%d:%s ", p[i].pid, "blocked");
            }
        }
        fprintf(f, "\n");
        printf("\n");
        cycle_number++;

        if(count == num_processes)
            break;
    }
    printf("\n");

    // Write statistics to output file
    fprintf(f, "Finishing time: %d\n", cycle_number-1);
    fprintf(f, "CPU Utilization: %.3f\n", total_cpu_runtime/(double)cycle_number);
    for(int j = 0; j < num_processes; j++)
    fprintf(f, "Turnaround process %d: %d\n", j, p[j].cycle_finished-p[j].arrival_time);

    fclose(f);
}

// Round robin algorithm
void round_robin (struct Process *p, int num_processes, char output[]){
    // Output file creation
    FILE * f = fopen(output, "w");

    // Creates queue to store processes
    struct Queue *list;
    list = malloc(sizeof(struct Queue));
    list->front = NULL;
    list->end = NULL;
    list->size = 0;

    int cycle_number = 0;
    int loop = 1;
    double total_cpu_runtime = 0;

    // Main while loop. Ends when all processes are finished
    while(loop){
        for(int i = 0; i < num_processes; i++){
            // Checks if process is ready to be added to the queue
            if(p[i].arrival_time == cycle_number || p[i].time_to_wait == cycle_number){ 
                p[i].state = 1;

                p[i].next=0;
                if(list->front == 0)
                    list->front = &p[i];
                else
                    list->end->next = &p[i];
                list->end = &p[i];
                list->size++;                    
            }
        }
        
        // Runs process
        if(list->size > 0){
            // Checks if process state must turn into I/O (blocked)
            if((list->front->time_remaining == (list->front->cpu_time/2) && list->front->time_to_wait < 0)){
                list->front->state = 3;
                list->front->rr_runtime = 0;
                list->front->time_to_wait = list->front->io_time + cycle_number;
                
                // Removes first process
                list->front = list->front->next;
                list->size--;
                
                // New process is running
                if(list->size > 0){
                    list->front->state = 2;
                    list->front->time_remaining--;
                    list->front->rr_runtime++;
                }
            }
            // Checks if process is terminated
            else if(list->front->time_remaining == 0){
                list->front->state = 4;
                list->front->cycle_finished = cycle_number;

                // Remove process from queue
                list->front = list->front->next;
                list->size--;

            // New process is running
                if(list->size > 0){
                    list->front->state = 2;
                    list->front->time_remaining--;
                    list->front->rr_runtime++;
                }
            }
            // If front runtime hits 2 (quantum), switch to next process
            else if(list->front->rr_runtime == 2){
                list->front->state = 1;
                list->front->rr_runtime = 0;
                
                // Removes front process from queue, moved to end of queue
                struct Process * temp;
                temp = list->front;
                list->front = list->front->next;
                temp->next = 0;
                if(list->front == 0)
                    list->front = temp;
                else
                    list->end->next = temp;
                list->end = temp;
                
                if(list->size > 0){
                    list->front->state = 2;
                    list->front->rr_runtime++;
                    list->front->time_remaining--;
                }
            }
            else{
            // Front process runs for one cycle
                list->front->state = 2;
                list->front->time_remaining--;
                list->front->rr_runtime++;
            }  
        }
        total_cpu_runtime++;
        int count = 0;
        for(int i = 0; i < num_processes; i++)
        {
            // Checks if at least one process has time remaining (while loop runs again)
            if(p[i].time_remaining != 0){
                loop = 1;
                break;
            }
            count++;
        }

        // Write cycle to output file
        fprintf(f, "%d ",cycle_number);
        printf("%d ",cycle_number);
        for(int i = 0; i < num_processes; i++)
        {
            if(p[i].state == 1){
                fprintf(f, "%d:%s ", p[i].pid, "ready");
                printf("%d:%s ", p[i].pid, "ready");
            }
            if(p[i].state == 2){
                fprintf(f, "%d:%s ", p[i].pid, "running");
                printf("%d:%s ", p[i].pid, "running");
            }
            if(p[i].state == 3){
                fprintf(f, "%d:%s ", p[i].pid, "blocked");
                printf("%d:%s ", p[i].pid, "blocked");
            }
        }
        fprintf(f, "\n");
        printf("\n");
        cycle_number++;

        if(count == num_processes)
            break;
    }
    printf("\n");
    
    // Write statistics to output file
    fprintf(f, "Finishing time: %d\n", cycle_number-1);
    fprintf(f, "CPU Utilization: %.3f\n", total_cpu_runtime/(double)cycle_number);
    for(int j = 0; j < num_processes; j++)
    fprintf(f, "Turnaround process %d: %d\n", j, p[j].cycle_finished-p[j].arrival_time);

    fclose(f);
}

// Shortest job first algorithm
void shortest_first (struct Process *p, int num_processes, char output[]){
    // Output file creation
    FILE * f = fopen(output, "w");

    // Creates queue to store processes
    struct Queue *list;
    list = malloc(sizeof(struct Queue));
    list->front = NULL;
    list->end = NULL;
    list->size = 0;

    int cycle_number = 0;
    int loop = 1;
    double total_cpu_runtime = 0;

    // Main while loop. Ends when all processes are finished
    while(loop){
        for(int i = 0; i < num_processes; i++){
            if(p[i].arrival_time == cycle_number || p[i].time_to_wait == cycle_number){ 
                p[i].state = 1;
            }
        }

        // Identifies process which shortest time remaining
        int min_process = -1;
        int min_time = 999999;
        for(int i = 0; i < num_processes; i++){
            if(p[i].state >= 1 && p[i].state <= 2 && p[i].time_remaining < min_time){
                min_time = p[i].time_remaining;
                min_process = i;
            }
        }

        // If there is a shortest process, then run that process
        if(min_process > -1){
            p[min_process].state = 2;
            // If the shortest process must be placed in I/O (blocked), then find the next shortest process
            if(p[min_process].time_remaining == (p[min_process].runtime - p[min_process].runtime/2) && p[min_process].time_to_wait == -1){
                p[min_process].state = 3;
                p[min_process].time_to_wait = cycle_number + p[min_process].iotime;
                min_process = -1;
                min_time = 999999;
                for(int i = 0; i < num_processes; i++){
                    if(p[i].state >= 1 && p[i].state <= 2 && p[i].time_remaining < min_time){
                        min_time = p[i].time_remaining;
                        min_process = i;
                    }
                }
                // If there is a next shortest process, then run that process
                if(min_process > -1){
                    p[min_process].state = 2;
                    p[min_process].time_remaining--;
                    total_cpu_runtime++;
                }
            }
            // Terminate process if no time remains and find nexst shortest process to run
            else if(p[min_process].time_remaining == 0){
                p[min_process].state = 4;
                p[min_process].finish = cycle_number;
                min_process = -1;
                min_time = 999999;
                for(int i = 0; i < num_processes; i++){
                    if(p[i].state > 0 && p[i].state < 3 && p[i].time_remaining < min_time){
                        min_time = p[i].time_remaining;
                        min_process = i;
                    }
                }
                // Run next shortest process
                if(min_process > -1){
                    p[min_process].state = 2;
                    p[min_process].time_remaining--;
                    total_cpu_runtime++;
                }
            }
            //Run shortest process
            else{
                p[min_process].time_remaining--;
                total_cpu_runtime++;
            }
        }

        total_cpu_runtime++;
        int count = 0;
        // Checks if at least one process has time remaining (while loop runs again)
        for(int i = 0; i < num_processes; i++) {
            if(p[i].time_remaining != 0){
                loop = 1;
                break;
            }
            count++;
        }
        
        // Write cycle to output file
        fprintf(f, "%d ",cycle_number);
        printf("%d ",cycle_number);
        for(int i = 0; i < num_processes; i++)
        {
            if(p[i].state == 1){
                fprintf(f, "%d:%s ", p[i].pid, "ready");
                printf("%d:%s ", p[i].pid, "ready");
            }
            if(p[i].state == 2){
                fprintf(f, "%d:%s ", p[i].pid, "running");
                printf("%d:%s ", p[i].pid, "running");
            }
            if(p[i].state == 3){
                fprintf(f, "%d:%s ", p[i].pid, "blocked");
                printf("%d:%s ", p[i].pid, "blocked");
            }
        }
        fprintf(f, "\n");
        printf("\n");
        cycle_number++;

        if(count == num_processes)
            break;
    }

    printf("\n");
    
    // Write statistics to output file
    fprintf(f, "Finishing time: %d\n", cycle_number-1);
    fprintf(f, "CPU Utilization: %.3f\n", total_cpu_runtime/(double)cycle_number);
    for(int j = 0; j < num_processes; j++)
    fprintf(f, "Turnaround process %d: %d\n", j, p[j].cycle_finished-p[j].arrival_time);

    fclose(f);
}
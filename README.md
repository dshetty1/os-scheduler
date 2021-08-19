# os-scheduler
### Description: 
This is a basic operating system scheduler that implements 3 different algorithms:  <br>
- 0 -- First-Come-First-Serve (non-preemptive)
  * Queue of ready processes
  * Newly arriving processes are added to the end of the queue.
  * When a process is blocked (due to I/O) and then becomes ready, it is added to the end of the queue.
  * If two processes happen to be ready at the same time, preference is given to the one with lower ID.

- 1 -- Round Robin (quantum of 2)
  * Another process is scheduled if one of the following occurs:
    * Current running process terminates
    * Current running process is blocked on I/O
    * Current running process ran for 2 cycles
  * When a process goes from running to ready, it moves to the back of the queue.
  * If two processes happen to be ready at the same time, preference is given to the one with lower ID.

- 2 -- Shortest remaining job first (preemptive)
  * At each cycle, calculate the remaining CPU time for all ready/running processes and run the one with the shortest remaining time.
  * If several processes have the same remaining CPU time, preference is given to the process with lower ID.


### Input File:<br>
The input file is a text file of the format: <br>
| Number of Processes | |  |  |
| --- | --- | --- | --- |
| Process ID | CPU Time | I/O Time | Arrival Time |


ex. <br>
3 <br>
0 2 2 0 <br>
1 2 1 5 <br>
2 2 1 3

### Output: <br>
Timing snapshot (at every line show):
- Cycle time
- State of each process (running, ready, or blocked) 
  * ex. 1: blocked  (i.e. process 1 is in blocked state)<br>

Statistics: <br>
- Finishing time (i.e. last cycle)
- CPU utilization (# of cycles CPU was doing work / total number of cycles)
- Turnaround time fo each process (i.e. cycle this process finished – cycle it started + 1)


ex. <br>
0 0:running<br>
1 0:blocked<br>
2 0:blocked<br>
3 0:running 2:ready<br>
4 2:running<br>
5 1:running 2:blocked <br>
6 1:blocked 2:running<br>
7 1:running<br>

Finishing time: 7<br>
CPU utilization: 0.625<br>
Turnaround process 0: 4<br>
Turnaround process 1: 3<br>
Turnaround process 2: 4

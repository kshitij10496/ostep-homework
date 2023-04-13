# Homework Solutions

1. Run `process-run.py` with the following flags: `-l 5:100,5:100`.
   What should the CPU utilization be (e.g., the percent of time the CPU is in use?)
   Why do you know this? Use the -c and -p flags to see if you were right.

The CPU utilization should ideally be **100%** because neither process is ever blocked by I/O.
Thus, the OS should always use the CPU to run instructions from either process (depending on the scheduling algorithm).

Output

```sh
$ python3 process-run.py -l 5:100,5:100 -c -p
Time PID: 0 PID: 1 CPU IOs
1 RUN:cpu READY 1
 2 RUN:cpu READY 1
 3 RUN:cpu READY 1
 4 RUN:cpu READY 1
 5 RUN:cpu READY 1
 6 DONE RUN:cpu 1
 7 DONE RUN:cpu 1
 8 DONE RUN:cpu 1
 9 DONE RUN:cpu 1
 10 DONE RUN:cpu 1

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy 0 (0.00%)
```

2. Now run with these flags: `./process-run.py -l 4:100,1:0`.
   These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done.
   How long does it take to complete both processes? Use -c and -p to find out if you were right.

The CPU should be used for atleast 6 units of time (exactly 4 to run Process 0, atleast 2 to handle IO Process 1).
The IO operation should take the default IO_LENGTH units of time (which is 5).
Hence, the total time should be 11 time units.

```sh
$ python3 process-run.py -l 4:100,1:0 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

3. Switch the order of the processes: `-l 1:0,4:100`.
   What happens now?
   Does switching the order matter? Why? (As always, use -c and -p to see if you were right)

Yes, the order matters. Depending on the Scheduler, the idle time can be reduced.
In this particular case, while Process 0 is BLOCKED on IO, the OS can RUN the Process 1.
Total time should now reduce to 7 time units.

```sh
$ python3 process-run.py -l 1:0,4:100 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

4. We’ll now explore some of the other flags. One important flag is `-S`, which determines how the system reacts when a process issues an I/O. With the flag set to `SWITCH_ON_END`, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (`-l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?

No, not quite. This is also identical to case 2.

```sh
$ python3 process-run.py -l 1:0,4:100 -S SWITCH_ON_END -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED         READY                           1
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7*   RUN:io_done         READY             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
 11           DONE       RUN:cpu             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

5. Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (`-l 1:0,4:100 -c -S SWITCH_ON_IO`).
   What happens now? Use -c and -p to confirm that you are right.

Same as case 3.

6. One other important behavior is what to do when an I/O completes.
   With `-I IO_RUN_LATER`, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running.
   What happens when you run this combination of processes? (Run `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`)
   Are system resources being effectively utilized?

With this scheduler config, the Process 0 will stay in READY state while the other CPU instructions are finished on other processes.
Once every other process is DONE, the OS RUNs Process 0, only to be BLOCKED again by IO.
Thus, this setup is not using the resources efficiently.

```sh
$ python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p

Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1
  2        BLOCKED       RUN:cpu         READY         READY             1             1
  3        BLOCKED       RUN:cpu         READY         READY             1             1
  4        BLOCKED       RUN:cpu         READY         READY             1             1
  5        BLOCKED       RUN:cpu         READY         READY             1             1
  6        BLOCKED       RUN:cpu         READY         READY             1             1
  7*         READY          DONE       RUN:cpu         READY             1
  8          READY          DONE       RUN:cpu         READY             1
  9          READY          DONE       RUN:cpu         READY             1
 10          READY          DONE       RUN:cpu         READY             1
 11          READY          DONE       RUN:cpu         READY             1
 12          READY          DONE          DONE       RUN:cpu             1
 13          READY          DONE          DONE       RUN:cpu             1
 14          READY          DONE          DONE       RUN:cpu             1
 15          READY          DONE          DONE       RUN:cpu             1
 16          READY          DONE          DONE       RUN:cpu             1
 17    RUN:io_done          DONE          DONE          DONE             1
 18         RUN:io          DONE          DONE          DONE             1
 19        BLOCKED          DONE          DONE          DONE                           1
 20        BLOCKED          DONE          DONE          DONE                           1
 21        BLOCKED          DONE          DONE          DONE                           1
 22        BLOCKED          DONE          DONE          DONE                           1
 23        BLOCKED          DONE          DONE          DONE                           1
 24*   RUN:io_done          DONE          DONE          DONE             1
 25         RUN:io          DONE          DONE          DONE             1
 26        BLOCKED          DONE          DONE          DONE                           1
 27        BLOCKED          DONE          DONE          DONE                           1
 28        BLOCKED          DONE          DONE          DONE                           1
 29        BLOCKED          DONE          DONE          DONE                           1
 30        BLOCKED          DONE          DONE          DONE                           1
 31*   RUN:io_done          DONE          DONE          DONE             1

Stats: Total Time 31
Stats: CPU Busy 21 (67.74%)
Stats: IO Busy  15 (48.39%)
```

7. Now run the same processes,but with `-I IO_RUN_IMMEDIATE` set, which immediately runs the process that issued the I/O.
   How does this behavior differ? Why might running a process that just completed an I/O again be a good idea?

This should use the resource optimally.
Running a process that just completed an I/O might be good idea for I/O heavy programs.
Because if we expect a process to be blocked by IO again, it might be efficient to RUN it as eagerly as possible.

8. Now run with some randomly generated processes: `-s 1 -l 3:50,3:50` or `-s 2 -l 3:50,3:50` or `-s 3 -l 3:50,3:50`.
   See if you can predict how the trace will turn out.
   What happens when you use the flag -I IO RUN IMMEDIATE vs. -I IO RUN LATER?
   What happens when you use -S SWITCH ON IO vs. -S SWITCH ON END?

I don't think it's possible to predice the instruction set of each process.
However, it's very curious to study how the behaviour of the scheduler closely affects the efficiency (in terms of both time and resource utilization).

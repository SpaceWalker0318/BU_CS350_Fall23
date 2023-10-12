# EVAL problem 1

## a) Estimate CPU spd on SLEEP method

```shell
# ./clock_script
# Bash
for ((i=1; i <= 10; i++)) do
		# change method to s/b accordingly
    ./build/clock $i 0 s
done
```



**Results**

![Screen Shot 2023-09-21 at 3.03.39 PM](/Users/jialuli/Desktop/Screen Shot 2023-09-21 at 3.03.39 PM.png)



**Ans:**

- Min: 2592.08
- Max: 2592.49
- Avg: 2592.24
- Std: 0.151




## b) Estimate CPU spd on BUSY_WAIT method

**Results:**

![image-20230921151210577](/Users/jialuli/Library/Application Support/typora-user-images/image-20230921151210577.png)

**Ans:**

- Min: 2592.00
- Max: 2592.00
- Avg: 2592.00
- Std: 0.00



## c)

* The BUSY_WAIT method is more closer to the actual CPU speed of my processor and yielded more accurate results

* **CPU spec: 2.6GHZ (2600MHZ)**

  ![image-20230921180105998](/Users/jialuli/Library/Application Support/typora-user-images/image-20230921180105998.png)

* The Sleep method is slightly closer to my CPU spec



# EVAL problem 2

## a) Throughput

```
R0:13036.503263,0.041792,13036.503366,13036.545159
R1:13036.686721,0.133472,13036.686907,13036.820380
R2:13036.839639,0.018340,13036.839792,13036.858132
R3:13037.082349,0.121834,13037.082486,13037.204320
R4:13037.123259,0.067280,13037.204371,13037.271653
R5:13037.155829,0.082600,13037.271683,13037.354284
R6:13037.220770,0.060026,13037.354333,13037.414360
R7:13037.266175,0.206605,13037.414410,13037.621016
R8:13037.570343,0.105279,13037.621096,13037.726376
R9:13037.671364,0.077822,13037.726408,13037.804230
R10:13037.686696,0.023186,13037.804249,13037.827436
R11:13037.688358,0.135878,13037.827446,13037.963325
R12:13037.703134,0.042700,13037.963370,13038.006100
R13:13037.720192,0.009599,13038.006138,13038.015738
R14:13037.734117,0.020519,13038.015769,13038.036289
R15:13038.417635,0.152254,13038.417806,13038.570061
R16:13038.489656,0.029251,13038.570115,13038.599367
R17:13038.584518,0.061911,13038.599397,13038.661309
R18:13038.686035,0.300301,13038.686191,13038.986493
R19:13038.754138,0.122966,13038.986538,13039.109505
R20:13038.788780,0.122441,13039.109555,13039.231998
R21:13038.863718,0.185106,13039.232035,13039.417142
R22:13038.914875,0.036214,13039.417221,13039.453436
R23:13038.948236,0.209469,13039.453474,13039.662943
R24:13039.113172,0.248530,13039.662981,13039.911512
R25:13039.120425,0.007498,13039.911559,13039.919058
R26:13039.195089,0.090695,13039.919077,13040.009773
R27:13039.216464,0.035756,13040.009808,13040.045565
R28:13039.437413,0.001685,13040.045608,13040.047293
R29:13039.444072,0.005431,13040.047314,13040.052745
R30:13039.505280,0.293993,13040.052770,13040.346764
R31:13039.532526,0.158605,13040.346810,13040.505416
R32:13039.765032,0.064667,13040.505463,13040.570131
R33:13039.796117,0.119012,13040.570209,13040.689248
R34:13039.843162,0.091815,13040.689285,13040.781101
R35:13039.915086,0.003339,13040.781150,13040.784490
R36:13039.990960,0.223818,13040.784511,13041.008330
R37:13040.048544,0.106364,13041.008377,13041.114742
R38:13040.315658,0.111787,13041.114793,13041.226582
R39:13040.349146,0.036419,13041.226618,13041.263037
R40:13040.451330,0.015124,13041.263080,13041.278205
```



* Start timestamp: 13036.503263

  End timestamp: 13042.278205

  Time difference: 5.7749s

  **Throughput** : 41req/5.7749s = 7.1req/s



## b) Utilization rate

```
R0:13036.503263,0.041792,13036.503366,13036.545159
R1:13036.686721,0.133472,13036.686907,13036.820380
R2:13036.839639,0.018340,13036.839792,13036.858132
R3:13037.082349,0.121834,13037.082486,13037.204320
R4:13037.123259,0.067280,13037.204371,13037.271653
R5:13037.155829,0.082600,13037.271683,13037.354284
R6:13037.220770,0.060026,13037.354333,13037.414360
R7:13037.266175,0.206605,13037.414410,13037.621016
R8:13037.570343,0.105279,13037.621096,13037.726376
R9:13037.671364,0.077822,13037.726408,13037.804230
R10:13037.686696,0.023186,13037.804249,13037.827436
R11:13037.688358,0.135878,13037.827446,13037.963325
R12:13037.703134,0.042700,13037.963370,13038.006100
R13:13037.720192,0.009599,13038.006138,13038.015738
R14:13037.734117,0.020519,13038.015769,13038.036289
R15:13038.417635,0.152254,13038.417806,13038.570061
R16:13038.489656,0.029251,13038.570115,13038.599367
R17:13038.584518,0.061911,13038.599397,13038.661309
R18:13038.686035,0.300301,13038.686191,13038.986493
R19:13038.754138,0.122966,13038.986538,13039.109505
```

* Total time to process the request (sum up the request length): 1.813615
* Total time elapsed (Completion t of R19 - receipt t of R0) = 2.606139
* **Utilization** = 1.81/2.60 = 70%



## c) Actual CPU usage

```
Command terminated by signal 13
        Command being timed: "./server 2222"
        User time (seconds): 8.05
        System time (seconds): 0.00
        Percent of CPU this job got: 90%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:08.86
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 1280
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 1
        Minor (reclaiming a frame) page faults: 62
        Voluntary context switches: 12
        Involuntary context switches: 193
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

**Percent of CPU usage:** 90%

Different. Because number of requests considered is too low



## d) Run server in batch

**server-script (taking a long time to run 500 requests so set to 30)**

```shell
# Bash
for ((i=1; i <= 12; i++)) do
    /usr/bin/time -v ./server 2222 > /dev/null & ./client -a $i -s 12 -n 30 2222 > /dev/null
done
```

 

```
# i=1        
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 9%

# i=2
        Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 18%

# i=3
        Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 27%
        ...
# i=4
        Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 36%
# i=5
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 45%
# i=6
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 54%
# i=7
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 61%
# i=8
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 67%
# i=9
        Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 67%
# i=10
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 72%
# i=11
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 77%
# i=12
Command being timed: "./server 2222"
        User time (seconds): 2.75
        System time (seconds): 0.00
        Percent of CPU this job got: 82%
```

![image-20230921205753228](/Users/jialuli/Library/Application Support/typora-user-images/image-20230921205753228.png)

**Conclusion: **The value of -a and the cpu usage positively correlated



## e)


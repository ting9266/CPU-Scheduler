# CPU-Scheduler

## 開發平台
Windows 11 

## 開發環境
Visual Studio Code
Win32 MinGW 6.3.0

## 題目說明
模擬CPU的排程，包含FCFS、RR、SRTF、PPRR，並輸出每一個時間片段使用CPU 的process 以及每個process在不同排程法的Waiting Time 以及Turnaround Time。

## 輸入說明
第1列的第1個數字為⽅法, ⽅法 1~6
1. FCFS
2. RR
3. SRTF
4. PPRR
5. HRRN
6. ALL(都要執⾏)

第1列的第2個數字為time slice，範圍不定。

第2列的欄位名稱為ID、CPU Burst 、Arrival Time、Priority。

第3列開始每⾏依序為
[Process ID] [CPU Burst] [Arrival time] [Priority]

![image](https://user-images.githubusercontent.com/95240041/194377336-7a99b8e5-a146-47a1-9bfe-6067a0669419.png)


## 輸出說明
第1⾏先輸出⽅法名稱，接著輸出該⽅法對應的⽢特圖，如果⽅法名稱為All，則依序輸出FCFS、RR、SRTF、PPRR、HRRN的⽢特圖。

第2個區段顯⽰各個process id在各個⽅法(可能數個)的Waiting Time。

第3個區段顯⽰各個process id在各個⽅法(可能數個)的Turnaround Time。

![image](https://user-images.githubusercontent.com/95240041/194378161-2556b47a-4218-4516-a134-b5bcad4d627f.png)


## 不同排程法比較
平均等待時間:
  INPUT1中(timeslice = 1 ) : SRTF < HRRN < FCFS < PPRR < RR
           平均等待時間  8.07    11.6   14.3    14.7   18.4

  INPUT2中(timeslice = 3 ) : SRTF < RR < HRRN < FCFS < PPRR
           平均等待時間   3    6.4    8.2     8.4    9.4

  INPUT3中(timeslice = 10 ) : SRTF = HRRN = FCFS < RR < PPRR
           平均等待時間   6.7     6.7     6.7   11.7   12.5
           
工作往返時間:
INPUT1:(黃色為此Data各排程中工作往返時間最小者)
![image](https://user-images.githubusercontent.com/95240041/194378829-edd6d42e-e186-477e-ac51-f3223a75da5c.png)


INPUT2:(黃色為此Data各排程中工作往返時間最小者)
![image](https://user-images.githubusercontent.com/95240041/194378690-0861b507-7b5d-4176-bf1a-52dc114535a8.png)


INPUT3:(黃色為此Data各排程中工作往返時間最小者)
![image](https://user-images.githubusercontent.com/95240041/194378774-3eded733-55bb-4408-8d3c-2cba3cb6b970.png)


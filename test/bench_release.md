# Bench

## aliyun

### ab -n 10000000 -c 1000 -k http://127.0.0.1:8888/

#### ./test_epoll_server > /dev/null  ab -n 10000000 -c 1000 -k http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   151.215 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    66130.94 [#/sec] (mean)
Time per request:       15.122 [ms] (mean)
Time per request:       0.015 [ms] (mean, across all concurrent requests)
Transfer rate:          6845.59 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.3      0      33
Processing:     2   15   3.3     14     103
Waiting:        1   15   3.3     14     103
Total:          2   15   3.3     14     103

Percentage of the requests served within a certain time (ms)
  50%     14
  66%     15
  75%     15
  80%     15
  90%     18
  95%     21
  98%     26
  99%     29
 100%    103 (longest request)

#### ./my_reactor_test 127.0.0.1 8888 > /dev/null   ab -n 10000000 -c 1000 -k http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   208.094 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    48055.22 [#/sec] (mean)
Time per request:       20.809 [ms] (mean)
Time per request:       0.021 [ms] (mean, across all concurrent requests)
Transfer rate:          4974.47 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.2      0      27
Processing:     8   21   4.2     20      75
Waiting:        0   21   4.2     20      75
Total:          8   21   4.2     20      75

Percentage of the requests served within a certain time (ms)
  50%     20
  66%     20
  75%     21
  80%     21
  90%     27
  95%     29
  98%     35
  99%     38
 100%     75 (longest request)

### ./simple_co_server > /dev/null  ab -n 10000000 -c 1000 -k -r http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   317.699 seconds
Complete requests:      10000000
Failed requests:        2700
   (Connect: 0, Receive: 1350, Length: 0, Exceptions: 1350)
Non-2xx responses:      9998650
Keep-Alive requests:    9998650
Total transferred:      1059856900 bytes
HTML transferred:       0 bytes
Requests per second:    31476.30 [#/sec] (mean)
Time per request:       31.770 [ms] (mean)
Time per request:       0.032 [ms] (mean, across all concurrent requests)
Transfer rate:          3257.85 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0  92.2      0   65674
Processing:     1   28 1523.5     10  131222
Waiting:        0   10   2.5     10     230
Total:          1   28 1526.3     10  131222

Percentage of the requests served within a certain time (ms)
  50%     10
  66%     11
  75%     11
  80%     11
  90%     12
  95%     14
  98%     19
  99%     22
 100%  131222 (longest request)

### ./echo_server > /dev/null (asyncio) ab -n 10000000 -c 1000 -k -r http://127.0.0.1:8888/



### ./echo_server > /dev/null (muduo) ab -n 10000000 -c 1000 -k http://127.0.0.1:2007/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            2007

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   137.458 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    72749.59 [#/sec] (mean)
Time per request:       13.746 [ms] (mean)
Time per request:       0.014 [ms] (mean, across all concurrent requests)
Transfer rate:          7530.72 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.2      0      28
Processing:     3   14   2.6     13      65
Waiting:        0   14   2.6     13      65
Total:          3   14   2.7     13      65

Percentage of the requests served within a certain time (ms)
  50%     13
  66%     13
  75%     13
  80%     14
  90%     17
  95%     18
  98%     22
  99%     25
 100%     65 (longest request)

### ./async_echo_server  > /dev/null (simple coroutine) ab -n 10000000 -c 1000 -k http://127.0.0.1:9980/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            9980

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   290.147 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    34465.33 [#/sec] (mean)
Time per request:       29.015 [ms] (mean)
Time per request:       0.029 [ms] (mean, across all concurrent requests)
Transfer rate:          3567.70 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.3      0      40
Processing:     2   29   6.2     28     124
Waiting:        0   29   6.2     28     124
Total:          2   29   6.2     28     124

Percentage of the requests served within a certain time (ms)
  50%     28
  66%     30
  75%     31
  80%     31
  90%     36
  95%     42
  98%     47
  99%     51
 100%    124 (longest request)


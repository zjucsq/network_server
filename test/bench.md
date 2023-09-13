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
Time taken for tests:   148.226 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    67464.75 [#/sec] (mean)
Time per request:       14.823 [ms] (mean)
Time per request:       0.015 [ms] (mean, across all concurrent requests)
Transfer rate:          6983.66 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   1.9      0    1032
Processing:     2   15   3.2     14    3436
Waiting:        0   15   3.2     14    3436
Total:          2   15   4.4     14    4468

Percentage of the requests served within a certain time (ms)
  50%     14
  66%     14
  75%     15
  80%     15
  90%     18
  95%     18
  98%     22
  99%     25
 100%   4468 (longest request)

#### ./test_epoll_server  ab -n 10000000 -c 1000 -k http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   575.021 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    17390.66 [#/sec] (mean)
Time per request:       57.502 [ms] (mean)
Time per request:       0.058 [ms] (mean, across all concurrent requests)
Transfer rate:          1800.20 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.4      0    1005
Processing:     4   57  20.4     54     413
Waiting:        0   57  20.4     54     413
Total:          4   57  20.5     54    1120

Percentage of the requests served within a certain time (ms)
  50%     54
  66%     59
  75%     64
  80%     67
  90%     75
  95%     85
  98%     98
  99%    109
 100%   1120 (longest request)

### ./simple_co_server > /dev/null

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   434.495 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    23015.22 [#/sec] (mean)
Time per request:       43.450 [ms] (mean)
Time per request:       0.043 [ms] (mean, across all concurrent requests)
Transfer rate:          2382.44 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.3      0      41
Processing:     1   43   9.2     47     116
Waiting:        0   43   9.2     47     116
Total:          1   43   9.2     47     116

Percentage of the requests served within a certain time (ms)
  50%     47
  66%     48
  75%     49
  80%     50
  90%     53
  95%     55
  98%     62
  99%     69
 100%    116 (longest request)

### ./simple_co_server > /dev/null  ab -n 10000000 -c 1000 -k -r http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   329.869 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    30315.07 [#/sec] (mean)
Time per request:       32.987 [ms] (mean)
Time per request:       0.033 [ms] (mean, across all concurrent requests)
Transfer rate:          3138.08 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   1.3      0    1032
Processing:    12   33   3.8     32     341
Waiting:        0   33   3.8     32     340
Total:         13   33   4.1     32    1372

Percentage of the requests served within a certain time (ms)
  50%     32
  66%     33
  75%     33
  80%     34
  90%     37
  95%     40
  98%     44
  99%     50
 100%   1372 (longest request)

### ./echo_server > /dev/null (asyncio) ab -n 10000000 -c 1000 -k -r http://127.0.0.1:8888/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   394.234 seconds
Complete requests:      10000000
Failed requests:        1150
   (Connect: 0, Receive: 575, Length: 0, Exceptions: 575)
Non-2xx responses:      9999425
Keep-Alive requests:    9999425
Total transferred:      1059939050 bytes
HTML transferred:       0 bytes
Requests per second:    25365.63 [#/sec] (mean)
Time per request:       39.423 [ms] (mean)
Time per request:       0.039 [ms] (mean, across all concurrent requests)
Transfer rate:          2625.59 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0  11.8      0    1041
Processing:     3   39 1064.1     32  209925
Waiting:        0   32 198.7     32  105657
Total:          3   39 1072.2     32  210943

Percentage of the requests served within a certain time (ms)
  50%     32
  66%     34
  75%     37
  80%     39
  90%     41
  95%     43
  98%     48
  99%     50
 100%  210943 (longest request)

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

### ./async_echo_server  > /dev/null (muduo) ab -n 10000000 -c 1000 -k http://127.0.0.1:9980/

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            9980

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   340.662 seconds
Complete requests:      10000000
Failed requests:        0
Non-2xx responses:      10000000
Keep-Alive requests:    10000000
Total transferred:      1060000000 bytes
HTML transferred:       0 bytes
Requests per second:    29354.61 [#/sec] (mean)
Time per request:       34.066 [ms] (mean)
Time per request:       0.034 [ms] (mean, across all concurrent requests)
Transfer rate:          3038.66 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.2      0      25
Processing:     5   34  36.2     32    8464
Waiting:        1   34  36.2     32    8464
Total:          5   34  36.3     32    8475

Percentage of the requests served within a certain time (ms)
  50%     32
  66%     33
  75%     34
  80%     35
  90%     42
  95%     44
  98%     54
  99%     60
 100%   8475 (longest request)
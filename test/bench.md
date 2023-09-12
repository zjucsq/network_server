# Bench

## aliyun

### ab -n 10000000 -c 1000 -k http://127.0.0.1:8888/

### ./test_epoll_server > /dev/null

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

### ./test_epoll_server

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
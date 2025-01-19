Parsing completed successfully.

Metric family item:
Name: http_requests_total
Type: counter
Help: The total number of HTTP requests.
Metric:
Label name: method, Label value: post
Label name: code, Label value: 200
Value: 1027.000000
Metric timestamp: 1395066363000
Metric:
Label name: method, Label value: post
Label name: code, Label value: 400
Value: 3.000000
Metric timestamp: 1395066363000

Comment item:
Escaping in label values:

Metric family item:
Name: msdos_file_access_time_seconds
Type value: untyped
Help: (null)
Metric:
Label name: path, Label value: C:\DIR\FILE.TXT
Label name: error, Label value: Cannot find file:
"FILE.TXT"
Value: 1458255915.000000

Comment item:
Minimalistic line:

Metric family item:
Name: metric_without_timestamp_and_labels
Type value: untyped
Help: (null)
Metric:
Value: 12.470000

Comment item:
A weird metric from before the epoch:

Metric family item:
Name: something_weird
Type value: untyped
Help: (null)
Metric:
Label name: problem, Label value: division by zero
Value: inf
Metric timestamp: -3982045

Comment item:
Corrupted histogram:

Metric family item:
Name: http_request_duration_seconds
Type: histogram
Help: A histogram of the request duration.
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: 0.05
Value: 24054.000000
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: 0.1
Value: 33444.000000
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: 0.2
Value: 100392.000000
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: 0.5
Value: 129389.000000
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: 1
Value: 133988.000000
Metric:
Label name: m_suff, Label value: bucket
Label name: le, Label value: +Inf
Value: 144320.100000
Metric:
Label name: m_suff, Label value: sum
Value: 53423.000000
Metric:
Label name: m_suff, Label value: count
Value: 144320.000000


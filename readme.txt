1.	Xử lý được các yêu cầu: GET/HEAD (30 points)
•	GET request từ một web browser (FF or Chrome) (10 points)
Test case:
Mở được các trang URLs như: 
http://example.com/ , http://students.iitk.ac.in/gymkhana/, http://phthiraptera.info/ 
Kết quả mong đợi: Hiển thị thành công các trang web page
•	GET request từ curl (5 points)
Test case:
$ curl -v --proxy localhost:8888 http://www.hcmus.edu.vn
Kết quả mong đợi: 200 (OK) HTTP chứa nội dung web page
•	HEAD request từ curl (5 points)
Test case:
$ curl -v --proxy localhost:8888 -X HEAD http://www.hcmus.edu.vn
Kết quả mong đợi:
	200 (OK) HTTP
2.	Filltering URLs (15 points)
Để sử dựng tính năng này, proxy sẽ được cấu hình để filter domain “fit.hcmus.edu”  and chạy trên port 8888
$ ./<MSSV> 8888 fit.hcmus.edu
•	Suffix filtering (5 points)
Test case:
$ curl -v --proxy localhost:8888 http://www.fit.hcmus.edu/~lqvu/ (không có .vn => thiếu suffix)
Kết quả mong đợi:
403 (Forbidden) HTTP response. The proxy không được gửi bất kỳ request nào tới web server.
•	Prefix Filtering (5 points)
Test case:
$ curl -v --proxy localhost:8888 http://fit.hcmus.edu.vn/~lqvu/ (không có www => thiếu prefix)
Kết quả mong đợi:
403 (Forbidden) HTTP reponse. 
•	Non-filtering case (5 points)
Test case:
$ curl -v –proxy localhost:8888 http://www.fit.hcmus.edu.vn/~lqvu/ (có đầy đủ cả suffix và prefix)
Kết quả mong đợi:
301 (Moved Permanently) HTTP response, kết quả trả về từ web server. Proxy không được chặn request, và chuyển request tới web server.
3.	Gửi request đồng thời (10 points)
Kiểm tra Proxy có thể handle nhiều request đồng thời hay không. Và kiểm tra Proxy có tạo ra những zoombie processes (http://stackoverflow.com/questions/16944886/how-to-kill-zombie-process)

•	Handle 4 request đồng thời (5 points)
Test case:
$./conctest.sh (file trên Moodle)
(Script mở 4 ternimal và mỗi terminal sẽ gửi một HTTP request đến http://tuoitre.vn)
Kết quả mong đợi:
 	200 (OK) HTTP reponse chứa nội dung của trang web
•	Zoombie process (5 points)
Sau khi chạy conctest.sh script. chạy command sau để test
Test case:
$ ps aux | grep defunct
Kết quả mong đợi:
Không có zoombie (defunct) process được hiển thị
4.	Signal handling (15 points)
Ref: http://www.yolinux.com/TUTORIALS/C++Signals.html 
•	SIGUSR1 (5 points)
Test case:
$ pkill a.out --signal 10
Kết quả mong đợi:
Proxy sẽ báo cáo lại thống kê. Ví dụ:
Received SIGUSR1...reporting status:
-- Processed 6 requests successfully
-- Filtering: fit.hcmus.edu
-- Filtered 2 requests
-- Encountered 0 requests in error
•	SIGUSR2 (5 points)
Test case:
$ pkill a.out --signal 12
Kết quả mong đợi:
Proxy shutdown
•	SIGINT (5 points)
Test case:
Nhấn Ctrl-C trong terminal mà proxy đang chạy
Kết quả mong đợi:
Proxy bỏ qua signal và tiếp tục chạy
5.	Memory leak (10 points)
Dùng Valgrind tool để kiểm tra Memory leak (ref: https://en.wikipedia.org/wiki/Memory_leak)
Test case:
- Proxy side
$ valgrind --leak-check=full --show-reachable=yes ./<MSSV> 8888
- Client side
$ curl -v --proxy localhost:8888 http://hcmus.edu.vn/
Kết quả mong đợi:
Nếu không có memory leak trong proxy, giá trị X và Y bên dưới của Valgrind sẽ là 0
LEAK SUMMARY:
definitely lost: X bytes in Y blocks
indirectly lost: 0 bytes in 0 blocks
possibly lost: 0 bytes in 0 blocks
still reachable: 0 bytes in 0 blocks
suppressed: 0 bytes in 0 blocks

6.	Không hỗ trỡ những request ngoài GET và HEAD (10 points)
Test case:
$ curl -v --proxy localhost:8888 -X OPTIONS http://hcmus.edu.vn/
Kết quả mong đợi:
 405 (Method Not Allowed) or 501 (Not Implemented) HTTP response. Proxy không gửi request đến web server
7.	Non-HTTP request (10 points)
Proxy không được kill khi nhận những request không hợp lệ
Test case:
$ telnet localhost 8888
<type ’abcdefg’ and enter>
Kết quả mong đợi:
Proxy vẫn hoạt động bình thường và phục vụ những request HTTP khác

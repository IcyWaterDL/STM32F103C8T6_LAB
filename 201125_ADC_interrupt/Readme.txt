1. Ngắt timer nháy led B2
Lỗi: -xung đột hàm Core_PrintInfo() và các hàm config gpio, timer
	Gọi hàm Core_PrintInfo() sau các hàm config
- config gpio: chọn các mode khác out_pp, in_floating thì k chạy đc ngắt timer.
nếu k có hàm chọn pin cho port

 


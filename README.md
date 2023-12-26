# Smarthome-IOT102

## Giới Thiệu Dự Án

Đây là dự án của môn IOT102 trong khuôn khổ của lớp SE1711 kì FA23. Dự án tập trung vào ứng dụng Internet of Things (IoT) trong việc điều khiển nhà thông minh thông qua một cloud server.

## Mô Hình Dự Án

Dự án sử dụng nhiều thiết bị và cảm biến IoT để điều khiển các chức năng trong nhà thông minh. Dưới đây là mô tả chi tiết về các chức năng chính:

### 1. Khóa Cửa Vân Tay

- Sử dụng cảm biến vân tay để mở khóa cửa tự động.
- Thông báo danh tính của người dùng thông qua ứng dụng di động.
- Cho phép mở khóa cửa từ xa thông qua ứng dụng.

### 2. Cảm Biến Nước Mưa

- Sử dụng cảm biến nước mưa để phát hiện lượng mưa.
- Tự động kéo quần áo khi trời mưa hoặc dừng dây phơi.

### 3. Phát Hiện Xâm Nhập

- Sử dụng cảm biến chuyển động hoặc cảm biến cửa để phát hiện sự xâm nhập trái phép.
- Kích hoạt chuông cửa và gửi thông báo đến ứng dụng di động.

### 4. Báo Cháy

- Sử dụng cảm biến khói để phát hiện khói (dấu hiệu cháy).
- Tự động gửi thông báo tới ứng dụng di động.
- Kích hoạt hệ thống báo động và mở cửa khi bật đèn để đảm bảo an toàn.

## Các Thành Phần Cần Thiết

Dưới đây là danh sách các thành phần cần thiết để triển khai dự án:

- Cảm biến vân tay (Mô-đun cảm biến vân tay Adafruit)
- Servo Motors (doorServo để mở cửa, laundryServo để quản lý chế độ phơi đồ)
- Màn hình LCD (LiquidCrystal)
- Buzzer hoặc Loa (buzzerPin) để phát ra âm thanh
- Các nút (btnLaundryPin cho chế độ giặt, btnEnrollPin để đăng ký dấu vân tay, btnOpenPin để mở cửa)
- Mô-đun Wi-Fi (ESP8266) để kết nối với mạng Wi-Fi
- Cảm biến khí (gasPin) để phát hiện khí
- Motion Sensor (motionPin) để phát hiện chuyển động
- Cảm biến mưa (waterPin) để phát hiện mưa

## Link Mua Sản Phẩm

- [Cảm biến mưa](https://www.thegioiic.com/mach-cam-bien-mua-roi)
- [Servo Motors](https://www.thegioiic.com/sg90-dong-co-servo-360-do-1-8-kg-cm-4-8-6-v)
- [Cảm biến chuyển động](https://www.thegioiic.com/hc-sr501-cam-bien-chuyen-dong-pir)
- [Cảm biến khí](https://www.thegioiic.com/mq-2-mach-cam-bien-khi-gas-lpg-propane-hydrogen)

## Đánh Giá Chi Tiết

Dự án nhà thông minh sử dụng Internet of Things (IoT) của chúng tôi đã mang lại những kết quả tích cực và hiệu quả trong việc tự động hóa các chức năng trong ngôi nhà. Dưới đây là một số đánh giá chi tiết về các khía cạnh của dự án:

### 1. Khóa Cửa Vân Tay
   - **Ưu Điểm:**
     - Tích hợp cảm biến vân tay giúp mở khóa cửa một cách an toàn và thuận tiện.
     - Thông báo danh tính qua ứng dụng di động giúp kiểm soát và xác nhận người dùng.

   - **Khuyết Điểm:**
     - Đôi khi có thể gặp khó khăn trong việc đọc vân tay, đặc biệt là khi tay ẩm hoặc khô.

### 2. Cảm Biến Nước Mưa
   - **Ưu Điểm:**
     - Phát hiện mưa tự động và thực hiện các hành động phù hợp như kéo quần áo hoặc ngừng phơi đồ.

   - **Khuyết Điểm:**
     - Đôi khi có thể phản ứng chậm đối với mưa nhẹ.

### 3. Phát Hiện Xâm Nhập
   - **Ưu Điểm:**
     - Sử dụng cảm biến chuyển động và cảm biến cửa để bảo vệ ngôi nhà khỏi xâm nhập trái phép.
     - Thông báo ngay lập tức đến người dùng thông qua ứng dụng di động.

   - **Khuyết Điểm:**
     - Đôi khi có thể phát hiện giả mạo từ các yếu tố như ánh sáng môi trường.

### 4. Báo Cháy
   - **Ưu Điểm:**
     - Sử dụng cảm biến khói để phát hiện nguy cơ cháy và kích hoạt các biện pháp an toàn.
     - Thông báo kịp thời qua ứng dụng di động.

   - **Khuyết Điểm:**
     - Đôi khi có thể phản ứng chậm đối với các khói nhẹ.

### Đối với Việc Tối Ưu Hóa Kết Nối
   - **Ưu Điểm:**
     - Việc đặt các cảm biến ở ESP8266 và các output ở Uno giúp tối ưu hóa kết nối và tăng hiệu suất.

   - **Khuyết Điểm:**
     - Cần thực hiện kiểm thử và điều chỉnh để giải quyết vấn đề trong quá trình phơi đồ.

### Lưu Ý Khi Sử Dụng Thiết Bị
   - **Cảm Biến Vân Tay:**
     - Hạn chế nguồn cấp ở 3.3V để tránh chập.

   - **Cảm Biến Khí:**
     - Kiểm tra đèn ngay sau khi cắm vào và điều chỉnh độ nhạy nếu cần.

## Tổng Kết

Dự án nhà thông minh đã đạt được một số thành công đáng kể, nhưng vẫn cần một số điều chỉnh nhỏ để tối ưu hóa hiệu suất. Đánh giá chi tiết này sẽ giúp chúng tôi thực hiện các cải tiến để cung cấp một trải nghiệm người dùng tốt nhất cho hệ thống nhà thông minh IoT này.

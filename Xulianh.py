import cv2
import numpy as np
import serial
import threading

# Khởi tạo camera
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# Cài đặt cổng nối tiếp
ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

# Cờ xử lý và biến đếm
start_processing = False
ca_count = 0

# Hàm để đọc dữ liệu từ cổng nối tiếp
def read_serial():
    global start_processing
    while True:
        try:
            data_rec = ser.readline().decode('utf-8', errors='ignore').strip()
            print(data_rec)
            if data_rec == "Start Processing":
                start_processing = True
            elif data_rec == "NoThing" or data_rec == "Done":
                start_processing = False
        except serial.SerialException as e:
            print("Serial Exception:", e)
            break

# Khởi tạo luồng đọc dữ liệu từ cổng nối tiếp
thread = threading.Thread(target=read_serial, daemon=True)
thread.start()

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    cv2.imshow("Camera", frame)
    
    if start_processing:
        img_crop = frame[165:(165+250), 165:(165+300)]
        hsv_img = cv2.cvtColor(img_crop, cv2.COLOR_BGR2HSV)
        cv2.imshow("Cropped Image", hsv_img)
        
        red_lower1 = np.array([0, 100, 100])
        red_upper1 = np.array([10, 255, 255])
        red_lower2 = np.array([160, 100, 100])
        red_upper2 = np.array([180, 255, 255])
        green_lower = np.array([36, 50, 50])
        green_upper = np.array([86, 255, 255])
        yellow_lower = np.array([22, 100, 100])
        yellow_upper = np.array([38, 255, 255])
            
        # Giả định hsv_img đã được chuyển đổi từ hình ảnh BGR sang HSV

        # Tạo mask cho màu đỏ
        red_mask1 = cv2.inRange(hsv_img, red_lower1, red_upper1)
        red_mask2 = cv2.inRange(hsv_img, red_lower2, red_upper2)
        red_mask = cv2.bitwise_or(red_mask1, red_mask2)

        # Tạo mask cho màu xanh lá
        green_mask = cv2.inRange(hsv_img, green_lower, green_upper)

        # Tạo mask cho màu vàng
        yellow_mask = cv2.inRange(hsv_img, yellow_lower, yellow_upper)

        # Đếm số pixel mỗi màu
        red_pixels = cv2.countNonZero(red_mask)
        green_pixels = cv2.countNonZero(green_mask)
        yellow_pixels = cv2.countNonZero(yellow_mask)

        # Tính tỷ lệ phần trăm cho mỗi màu
        total_pixels = red_pixels + green_pixels + yellow_pixels + 1  # Cộng thêm 1 để tránh chia cho 0
        R = (red_pixels / total_pixels) * 100
        G = (green_pixels / total_pixels) * 100
        Y = (yellow_pixels / total_pixels) * 100

        # Phân loại và đếm số lượng cà chua
        if R > G and R > Y and R > 85:  # Ngưỡng cho màu đỏ
            ca_count += 1
            print(f"Cà chua đỏ. Tổng số: {ca_count}")
            ser.write(('RED\n').encode())
        elif G > R and G > Y and G > 50:  # Ngưỡng cho màu xanh
            ca_count += 1
            print(f"Cà chua xanh. Tổng số: {ca_count}")
            ser.write(('GRE\n').encode())
        elif Y > R and Y > G and Y > 30:  # Ngưỡng cho màu vàng
            ca_count += 1
            print(f"Cà chua vàng. Tổng số: {ca_count}")
            ser.write(('YEL\n').encode())
        else:
            ca_count += 1
            print(f"Cà chua cam. Tổng số: {ca_count}")
            ser.write(('ORAN\n').encode())

        start_processing = False
    
    # Kiểm tra nếu người dùng muốn thoát
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Dọn dẹp sau khi thoát
cap.release()
cv2.destroyAllWindows()
ser.close()
thread.join()

# In số lượng cà chua đã được phân loại
print("Tổng số cà chua đã được phân loại:", ca_count)

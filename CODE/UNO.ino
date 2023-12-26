#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Khai báo hằng
const int buzzerPin = A0;     // Chân A0 cho buzzer
const int btnLaundryPin = A1; // Chân 4 cho nút "Laundry"
const int btnEnrollPin = 7;   // Chân 7 cho nút "Enroll"
const int btnOpenPin = 6;     // Chân 6 cho nút "Open"
const int doorServoPin = 4;      // Chân 4 cho servo
const int laundryServoPin = 5;   // Chân 5 cho servo
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;

// Khai báo biến
String rc = "";
int incorrectAttempts = 0;
int set_time = 8;
uint8_t id;
bool LaundryMode = false;

// Khai báo đối tượng
SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo doorServo;
Servo laundryServo;

void setup()
{
    Serial.begin(9600);
    finger.begin(57600);

    Wire.begin(8);                // tạo kết nối i2c với địa chỉ 8
    Wire.onReceive(receiveEvent); // nhận tín hiệu từ master

    pinMode(btnEnrollPin, INPUT_PULLUP); // Sử dụng chân btnEnrollPin cho nút enroll
    pinMode(btnOpenPin, INPUT_PULLUP);   // Sử dụng chân btnOpenPin cho nút open
    pinMode(doorServoPin, OUTPUT);          // Khai báo chân doorServoPin là chân điều khiển servo mở cửa
    pinMode(buzzerPin, OUTPUT);          // Khai báo chân buzzerPin là chân điều khiển buzzer
    pinMode(btnLaundryPin, INPUT_PULLUP);   // Sử dụng chân btnTestPin cho nút test
    pinMode(laundryServoPin, OUTPUT);       // Khai báo chân laundryServoPin là chân điều khiển servo mở cửa

    lcd.begin(16, 2); // Khai báo kích thước màn hình LCD
    lcd.setCursor(0, 0);
    lcd.print("Security System");
    lcd.setCursor(0, 1);
    lcd.print("by Finger Print");
    delay(1000); // Đợi một chút
    lcd.clear();
    lcd.print("Finding Module");
    lcd.setCursor(0, 1);
    delay(1000);
    if (finger.verifyPassword())
    {
        Serial.println("Found fingerprint sensor!");
        lcd.clear();
        lcd.print("Found Module ");
    }
    else
    {
        Serial.println("Did not find fingerprint sensor");
        lcd.clear();
        lcd.print("module not Found");
        lcd.setCursor(0, 1);
        lcd.print("Check Connections");
    }

    lcd.clear();
}
void loop()
{
    lcd.clear();
    lcd.print("Place Finger [ ]");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    int result = getFingerprintIDez();

    if (result > 0)
    {
        lcd.clear();
        lcd.print("Access Granted");
        delay(1000);
        tgDoor();
    }
    checkKeys();
    Serial.println(rc);
    delay(100);
}

void Enroll()
{
    int count = 0;
    lcd.clear();
    lcd.print("Enroll Finger    ");
    lcd.setCursor(0, 1);
    lcd.print("Location:");
    while (1)
    {
        lcd.setCursor(9, 1);
        lcd.print(count);
        lcd.print("   ");

        if (digitalRead(btnOpenPin) == 0)
        {
            count++;
            if (count > 128)
                count = 0;
            delay(300);
        }

        else if (digitalRead(btnEnrollPin) == 0)
        {
            id = count;
            getFingerprintEnroll();
            return;
        }
    }
}

uint8_t getFingerprintEnroll()
{
    int p = -1;
    lcd.clear();
    lcd.print("finger ID:");
    lcd.print(id);
    lcd.setCursor(0, 1);
    lcd.print("Place Finger");
    delay(2000);
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("Image taken");
            lcd.clear();
            lcd.print("Image taken");
            break;
        case FINGERPRINT_NOFINGER:
            Serial.println("No Finger");
            lcd.clear();
            lcd.print("No Finger");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            lcd.clear();
            lcd.print("Comm Error");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            lcd.clear();
            lcd.print("Imaging Error");
            break;
        default:
            Serial.println("Unknown error");
            lcd.clear();
            lcd.print("Unknown Error");
            break;
        }
    }
    // OK success!
    p = finger.image2Tz(1);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        lcd.clear();
        lcd.print("Image converted");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        lcd.clear();
        lcd.print("Image too messy");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.print("Comm Error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.print("Feature Not Found");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.print("Feature Not Found");
        return p;
    default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.print("Unknown Error");
        return p;
    }

    Serial.println("Remove finger");
    lcd.clear();
    lcd.print("Remove Finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
    }
    Serial.print("ID ");
    Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
    lcd.clear();
    lcd.print("Place Finger");
    lcd.setCursor(0, 1);
    lcd.print("   Again");
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("Image taken");
            break;
        case FINGERPRINT_NOFINGER:
            Serial.print(".");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
        default:
            Serial.println("Unknown error");
            return;
        }
    }
    // OK success!
    p = finger.image2Tz(2);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");
    Serial.println(id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Prints matched!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
        Serial.println("Fingerprints did not match");
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }

    Serial.print("ID ");
    Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Stored!");
        lcd.clear();
        lcd.print("Stored!");
        delay(2000);
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        Serial.println("Could not store in that location");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        Serial.println("Error writing to flash");
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }
}

int getFingerprintIDez()
{
    uint8_t p = finger.getImage();

    if (p != FINGERPRINT_OK)
        return -1;
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return -1;
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
        lcd.clear();
        lcd.print("Finger Not Found");
        lcd.setCursor(0, 1);
        lcd.print("Try Later ");
        delay(2000);
        return -1;
    }
    // found a match!
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    return finger.fingerID;
}

void checkKeys()
{
    if (digitalRead(btnEnrollPin) == 0)
    {
        lcd.clear();
        lcd.print("Please Wait");
        delay(1000);
        while (digitalRead(btnEnrollPin) == 0)
            ;
        Enroll();
    }
    else if (digitalRead(btnOpenPin) == 0)
    {
        lcd.clear();
        lcd.print("Please Wait");
        delay(1000);
        while (digitalRead(btnOpenPin) == 0)
            ;
        tgDoor();
    }
    else if (digitalRead(btnLaundryPin) == 0)
    {
        // Kiểm tra nút Laundry
        if (LaundryMode)
        {
            LaundryMode = false; // Nếu là true, đặt thành false
        }
        else
        {
            LaundryMode = true; // Nếu là false, đặt thành true
        }
        laundryControl(); // Gọi hàm laundryControl để cập nhật trạng thái
    }
    else if (rc == "OpenDoor")
    {
        rc = "";
        Serial.println("Please Wait");
        delay(1000);
        tgDoor();
    }
    if (rc == "Alarm")
    {
        alarm();
        rc = "";
    }
    if(rc=="Rain"){
        if(LaundryMode==true){
            LaundryMode=false;
            laundryControl();
        }
        rc="";
    }
}

void laundryControl()
{
    if (LaundryMode)
    {
        // Nếu laundryMode là true, quay servo 45 độ
        laundryServo.attach(laundryServoPin);
        laundryServo.write(45);
    }
    else
    {
        // Nếu laundryMode là false, quay servo -45 độ (ngược chiều kim đồng hồ)
        laundryServo.attach(laundryServoPin);
        laundryServo.write(-45);
    }
    delay(1000);
    laundryServo.detach(); // Tắt servo sau khi hoàn thành
}

void tgDoor()
{
    lcd.clear();
    lcd.print("Please Wait... ");
    lcd.setCursor(0, 1);
    lcd.print(" Door is Opening");
    delay(1000);

    doorServo.attach(doorServoPin); // Gắn servo vào chân doorServo
    doorServo.write(90);         // Mở cửa (quay servo 90 độ)

    delay(500); // Chờ một khoảng thời gian

    lcd.clear();
    lcd.print("    Welcome   ");
    lcd.setCursor(0, 1);
    lcd.print("  Door is Open ");

    for (int i = 0; i < set_time; i++)
    {
        delay(1000);
    }

    lcd.clear();
    lcd.print("Please Wait... ");
    lcd.setCursor(0, 1);
    lcd.print(" Door is Closing");
    delay(1000);

    doorServo.write(-90); // Đóng cửa (quay servo -90 độ)

    delay(500); // Chờ một khoảng thời gian

    doorServo.detach(); // Tắt servo

    delay(100);
    lcd.clear();
}

void alarm()
{
    Serial.println("Alarm");
    tone(buzzerPin, 1000);
    delay(5000); // Buzzer will sound for 10 seconds
    noTone(buzzerPin);
}

void receiveEvent(int howMany)
{

    while (0 < Wire.available())
    {
        char c = Wire.read(); /* Nhận byte như một ký tự */
        rc += c;              /* Nối ký tự vào chuỗi str */
    }
}

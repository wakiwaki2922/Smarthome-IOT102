#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6pywv26g9"
#define BLYNK_TEMPLATE_NAME "Smarthome"
#define BLYNK_AUTH_TOKEN "kavmAUcpsBJ92dRbWciP2-3vAQWbeWZb"

#include <BlynkMultiClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

const char *ssid = "FPTU_Student";
const char *pass = "12345678";

BlynkTimer timer; // Create a Blynk timer object

static WiFiClient blynkWiFiClient;
// Khai báo hằng
const int btnThiefPin = D3;
const int gasPin = D4;
const int motionPin = D5;
const int waterPin = D6;

// Khai báo biến
bool openDoorFlag = false;   // Biến để kiểm tra cần mở cửa hay không
bool antiThiefMode = false;  // Biến để kiểm tra chế độ Anti Thief
String gasStatus = "Normal"; // Ban đầu, trạng thái là "Normal"

void connectWiFi()
{

    Serial.print("Connecting to ");

    Serial.println(ssid);

    if (pass && strlen(pass))
    {

        WiFi.begin((char *)ssid, (char *)pass);
    }
    else
    {

        WiFi.begin((char *)ssid);
    }

    while (WiFi.status() != WL_CONNECTED)
    {

        delay(500);

        Serial.print(".");
    }

    Serial.println();

    Serial.print("Connected with IP: ");

    Serial.println(WiFi.localIP());
}

void setup()
{

    Serial.begin(9600);

    connectWiFi();

    Blynk.addClient("WiFi", blynkWiFiClient, 80); // Add Blynk client with name "WiFi"

    Blynk.config(BLYNK_AUTH_TOKEN);
    Wire.begin(D1, D2);

    // Lập kế hoạch cho tác vụ kiểm tra cửa sau mỗi 5 giây
    timer.setInterval(5000L, checkDoorStatus);
    // Lập kế hoạch cho tác vụ kiểm tra khí gas sau mỗi 500ms (0.5 giây)
    timer.setInterval(500L, checkGasSensor);
    // Lập kế hoạch cho tác vụ kiểm tra chế độ Anti Thief sau mỗi 1 giây
    timer.setInterval(1000L, checkMotion);
    // Lập kế hoạch cho tác vụ kiểm tra cảm biến nước mưa sau mỗi 1 giây
    timer.setInterval(1000L, checkRainSensor);

    Blynk.virtualWrite(V1, "Normal");
    pinMode(waterPin, INPUT);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {

        connectWiFi();

        return;
    }

    Blynk.run(); // Run Blynk library
    timer.run(); // Run Blynk timer
    // Code bắt đầu từ đây
    checkKeys();
}

void checkDoorStatus()
{
    if (openDoorFlag)
    {
        Serial.println("OpenDoor");
        Wire.beginTransmission(8); // Begin with device address 8
        Wire.write("OpenDoor");
        Wire.endTransmission(); // Stop transmitting
        openDoorFlag = false;
        Blynk.virtualWrite(V0, 0);
    }
}
void checkGasSensor()
{
    int gasValue = digitalRead(gasPin); // Đọc giá trị từ cảm biến khí gas

    if (gasValue == HIGH) // Cảm biến phát hiện khí gas
    {
        gasStatus = "Normal";
    }
    else
    {
        gasStatus = "Gas leak detected!";
        // Gửi chuỗi "Alarm" đến thiết bị I2C có địa chỉ 8
        Wire.beginTransmission(8); // Bắt đầu với địa chỉ thiết bị 8
        Wire.write("Alarm");       // Gửi chuỗi "Alarm"
        Wire.endTransmission();    // Dừng việc truyền dữ liệu
    }

    Blynk.virtualWrite(V3, gasStatus); // Gửi trạng thái khí gas vào chân V3 của Blynk
}
void checkMotion()
{
    if (antiThiefMode && digitalRead(motionPin) == HIGH)
    {
        Blynk.virtualWrite(V1, "Someone is in your house!");
        Wire.beginTransmission(8);
        Wire.write("Alarm");
        Wire.endTransmission();
        timer.setTimeout(5000L, []()
                         { Blynk.virtualWrite(V1, "Normal"); });
    }
}
void checkRainSensor()
{
    int rainValue = digitalRead(waterPin); // Đọc giá trị từ cảm biến nước mưa

    if (rainValue == LOW) // Nếu giá trị từ cảm biến là 0 (nghĩa là có mưa)
    {
        Blynk.virtualWrite(V4, "Rain");

        Wire.beginTransmission(8); // Bắt đầu gửi dữ liệu đến thiết bị I2C có địa chỉ 8
        Wire.write("Rain");        // Gửi chuỗi "Rain" qua I2C
        Wire.endTransmission();    // Dừng việc truyền dữ liệu
    }
    else
    {
        Blynk.virtualWrite(V4, "Clear");
    }
}

void checkKeys()
{

    if (digitalRead(btnThiefPin) == LOW)
    {
        if (antiThiefMode == true)
        {
            Serial.println("AntiThiefMode : OFF(Button)");
            antiThiefMode = false;
            Blynk.virtualWrite(V2, 0); // Gửi giá trị 0 đến widget trên Blynk
        }
        else
        {
            Serial.println("AntiThiefMode : ON(Button)");
            antiThiefMode = true;
            Blynk.virtualWrite(V2, 1); // Gửi giá trị 1 đến widget trên Blynk
        }
        delay(500); // Đợi 0.5 giây để tránh đọc nút liên tục khi nó đang được nhấn
    }
}

BLYNK_WRITE(V2)
{
    int bn = param.asInt();
    if (bn == 1)
    {
        antiThiefMode = true;
    }
    else
    {
        antiThiefMode = false;
    }
}
BLYNK_WRITE(V0)
{
    int bn = param.asInt();

    if (bn == 1)
    {
        openDoorFlag = true;
    }
}

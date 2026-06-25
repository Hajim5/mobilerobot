#include <Arduino.h>
#include <MecanumCar_v2.h>
#include <IRremote.h>
#include <Servo.h>

mecanumCar mecanumCar(3, 2);
Servo myservo;
// =====================
// PINS
// =====================
#define SensorLeft    A0
#define SensorMiddle  A1
#define SensorRight   A2
#define servo_pin 9

int RECV_PIN = A3;

#define TRIG_PIN 12
#define ECHO_PIN 13

#define COLOR_S2 6
#define COLOR_S3 7
#define COLOR_OUT 8

// =====================
// NEW COUUNTER & TURN VARIABLES
// =====================
bool missionRunning = false;
// USER ADJUSTABLE JUNCTION ALIGNMENT TIMES (CHANGE THESE)

// =====================
// MOTOR SPEED
// =====================
extern uint8_t speed_Upper_L;
extern uint8_t speed_Lower_L;
extern uint8_t speed_Upper_R;
extern uint8_t speed_Lower_R;
const unsigned long OK_BUTTON = 0xFF02FD;

IRrecv irrecv(RECV_PIN);
decode_results results;

void runMission();
void moveForwardIntersections(int target);
void turnLeft90();
void turnRight90();
void strafeLeftMarkers(int target);
void strafeRightMarkers(int target);
void adjustment();

void setup()
{
    Serial.begin(9600);

    mecanumCar.Init();

    pinMode(SensorLeft, INPUT);
    pinMode(SensorMiddle, INPUT);
    pinMode(SensorRight, INPUT);

    irrecv.enableIRIn();

    speed_Upper_L = 40;
    speed_Lower_L = 40;
    speed_Upper_R = 40;
    speed_Lower_R = 40;

    Serial.println("Ready");
}

void loop()
{
    if (irrecv.decode(&results))
    {
        if (results.value == OK_BUTTON && !missionRunning)
        {
            missionRunning = true;
            runMission();
            missionRunning = false;
        }
        irrecv.resume();
    }
}

void runMission()
{
    moveForwardIntersections(3);
    adjustment();
    turnLeft90();
    adjustment();
    moveForwardIntersections(2);
    adjustment();
    turnRight90();
    adjustment();
    moveForwardIntersections(3);
    adjustment();
   
    //strafeLeftMarkers(2);
    moveForwardIntersections(3);
    mecanumCar.Stop();
    Serial.println("MISSION COMPLETE");
}

void moveForwardIntersections(int target)
{
    int counter = 0;
    bool onJunction = false;

    while(counter < target)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        if(left && middle && right)
        {
            if(!onJunction)
            {
                counter++;

                Serial.print("Intersection: ");
                Serial.println(counter);

                onJunction = true;
            }

            mecanumCar.Advance();
        }
        else
        {
            onJunction = false;

            if(!left && middle && !right)
                mecanumCar.Advance();

            else if(left && !middle && !right)
                mecanumCar.Turn_Left();

            else if(!left && !middle && right)
                mecanumCar.Turn_Right();

            else if(left && middle && !right)
                mecanumCar.Turn_Left();

            else if(!left && middle && right)
                mecanumCar.Turn_Right();
            else if(left && !middle && right)
                mecanumCar.Advance();
            else if(!left && !middle && !right)
                mecanumCar.Advance();
        }
    }

    mecanumCar.Stop();
    delay(300);
}

void turnLeft90()
{
    unsigned long startTime = millis();

    while(millis() - startTime < 300)
    {
        mecanumCar.Turn_Left();
    }

    while(true)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.Turn_Left();

        if(left || middle && !right)
        {
            break;
        }
    }

    mecanumCar.Stop();
    delay(300);
}

void turnRight90()
{
    speed_Upper_L = 60;
    speed_Lower_L = 60;
    speed_Upper_R = 60;
    speed_Lower_R = 60;

    unsigned long startTime = millis();

    while(millis() - startTime < 300)
    {
        mecanumCar.Turn_Right();
    }

    while(true)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.Turn_Right();

        if(!left && middle || right)
        {
            break;
        }
    }

    mecanumCar.Stop();
    delay(300);

    speed_Upper_L = 40;
    speed_Lower_L = 40;
    speed_Upper_R = 40;
    speed_Lower_R = 40;
}

void strafeLeftMarkers(int target)
{
    int counter = 0;
    bool onMarker = false;

    while(counter < target)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.L_Move();

        // Detect center line crossing
        if(left && !middle && !right)
        {
            if(!onMarker)
            {
                counter++;

                Serial.print("Strafe Left Count: ");
                Serial.println(counter);

                onMarker = true;

                if(counter >= target)
                    break;
            }
        }
        else
        {
            onMarker = false;
        }
    }

    mecanumCar.Stop();
    delay(300);
}

void strafeRightMarkers(int target)
{
    int counter = 0;
    bool onMarker = false;

    while(counter < target)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.R_Move();

        if(!left && middle && !right)
        {
            if(!onMarker)
            {
                counter++;

                Serial.print("Strafe Right Count: ");
                Serial.println(counter);

                onMarker = true;

                if(counter >= target)
                    break;
            }
        }
        else
        {
            onMarker = false;
        }
    }

    mecanumCar.Stop();
    delay(300);
}

void adjustment()
{
    while(true)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        if(!left && middle && !right)
        {
            break;
        }

        if(left)
            mecanumCar.Turn_Left();

        if(right)
            mecanumCar.Turn_Right();
    }

    mecanumCar.Stop();
}

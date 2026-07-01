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

#define S0 4
#define S1 5
#define S2 7
#define S3 6
#define SENSOR_OUT 8
#define servo_pin 9

int RECV_PIN = A3;

#define TRIG_PIN 12
#define ECHO_PIN 13


#define TURN_BLIND_MS        300
#define TURN_SPEED           60
#define FOLLOW_SPEED         40

// Choose which sensor should detect the new line
// Usually the center sensor
#define STOP_SENSOR SensorMiddle

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
const unsigned long NO0_BUTTON = 0xFF30CF;

IRrecv irrecv(RECV_PIN);
decode_results results;

int redFrequency;
int greenFrequency;
int blueFrequency;

int requestedColorState = 0;

void runMission1();
void runMission2();
void moveForwardIntersections(int target);
void strafeLeftMarkers(int target);
void strafeRightMarkers(int target);
void adjustment();
void strafeRightMarkers_2();
void turnRight90_V2();
bool grab();
void strafeLeftMarkers_o(int target);
void turnRight90_V2_o();
void returnHomeFromLocation1();
void returnHomeFromLocation2();
void returnHomeFromLocation3();

void setup()
{
    Serial.begin(9600);

    mecanumCar.Init();

    pinMode(SensorLeft, INPUT);
    pinMode(SensorMiddle, INPUT);
    pinMode(SensorRight, INPUT);

    myservo.attach(servo_pin);
    myservo.write(7); // Initialize open

    pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
    pinMode(SENSOR_OUT, INPUT);
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // TCS3200 Frequency scaling = 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

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
        switch (results.value)
        {
            case OK_BUTTON:
                if (!missionRunning)
                {
                    missionRunning = true;
                    runMission1();
                    missionRunning = false;
                }
                break;

            case NO0_BUTTON:
                if (!missionRunning)
                {
                    missionRunning = true;
                    runMission2();
                    missionRunning = false;
                }
                break;

            default:
                break;
        }

        irrecv.resume();
    }
}
void runMission1()
{
    //void pickUpObject();
    grab();
    mecanumCar.Stop();
    myservo.write(7);
    Serial.println("MISSION COMPLETE");
}

void runMission2()
{
    //void pickUpObject();
    moveForwardIntersections(3);
    delay(1000);
    strafeLeftMarkers(2);
    delay(1000);
    moveForwardIntersections(3);
    delay(1000);
    if (grab())
    {
    Serial.println("Found at Location 1");
    returnHomeFromLocation1();
    return;
    }
    turnRight90_V2();
    delay(1000);
    moveForwardIntersections(2);
    delay(1000);
    turnRight90_V2();    
    delay(1000);
    turnRight90_V2();
    delay(1000);
    turnRight90_V2();
    delay(1000);
    if (grab())
    {
    Serial.println("Found at Location 2");
    returnHomeFromLocation2();
    return;
    }
    turnRight90_V2();
    delay(1000);
    moveForwardIntersections(2);
    delay(1000);
    turnRight90_V2();    
    delay(1000);
    turnRight90_V2();
    delay(1000);
    turnRight90_V2();
    delay(1000);
    if (grab())
    {
    Serial.println("Found at Location 3");
    returnHomeFromLocation3();
    return;
    }
    Serial.println("Target object not found.");

}

long getDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    if (duration == 0) return 999;                  // Return large number if no echo
    
    long distance = duration * 0.034 / 2;           // Convert time to cm
    return distance;
}


void moveForwardIntersections(int target)
{
    int counter = 0;
    bool onJunction = false;
    speed_Upper_L = 50;
    speed_Lower_L = 50;
    speed_Upper_R = 40;
    speed_Lower_R = 40;

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
    //speed_Upper_L = 60;
    //speed_Lower_L = 60;
    //speed_Upper_R = 40;
    //speed_Lower_R = 40;
    speed_Upper_L = 40;
    speed_Lower_L = 40;
    speed_Upper_R = 20;
    speed_Lower_R = 20;
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
    //speed_Upper_L = 55;
    //speed_Lower_L = 55;
    //speed_Upper_R = 55;
    //speed_Lower_R = 55;
    speed_Upper_L = 55;
    speed_Lower_L = 55;
    speed_Upper_R = 55;
    speed_Lower_R = 55;

    unsigned long startTime = millis();

    //original 300
    while(millis() - startTime < 30)
    {
        mecanumCar.Turn_Right();
    }

    while(true)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.Turn_Right();

        if(!left && !middle && right)
        {
            break;
        }
    }

    mecanumCar.Stop();
}

void strafeLeftMarkers(int target)
{
    int counter = 0;
    bool onMarker = false;
    //speed_Upper_L = 57;
    //speed_Lower_L = 50;
    //speed_Upper_R = 60;
    //speed_Lower_R = 60;
    speed_Upper_L = 47;
    speed_Lower_L = 40;
    speed_Upper_R = 50;
    speed_Lower_R = 50;


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
    //speed_Upper_L = 45;
    //speed_Lower_L = 45;
    //speed_Upper_R = 60;
    //speed_Lower_R = 50;
    speed_Upper_L = 55;
    speed_Lower_L = 60;
    speed_Upper_R = 55;
    speed_Lower_R = 45;

    while(counter < target)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.R_Move();

        if(!left && !middle && right)
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
    speed_Upper_L = 60;
    speed_Lower_L = 60;
    speed_Upper_R = 60;
    speed_Lower_R = 60;

    while (true)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        // Perfectly centered
        if (!left && middle && !right)
        {
            mecanumCar.Stop();
            break;
        }

        // No sensor sees the line -> rotate right until a line is found
        else if (!left && !middle && !right)
        {
            mecanumCar.Turn_Right();
        }

        // Line is on the left
        else if (left)
        {
            mecanumCar.Turn_Left();
        }

        // Line is on the right
        else if (right)
        {
            mecanumCar.Turn_Right();
        }
    }

    mecanumCar.Stop();
}

void turnRight90_V2()
{
    //dulu 60
    speed_Upper_L = 60;
    speed_Lower_L = 60;
    speed_Upper_R = 60;
    speed_Lower_R = 60;

    unsigned long start = millis();

    while (millis() - start < TURN_BLIND_MS)
        mecanumCar.Turn_Right();

    start = millis();
    //150 dulu
    while (millis() - start < 150)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.Turn_Right();

        if (!left && !middle && right)
            break;
    }

    mecanumCar.Stop();
    delay(100);
}

void strafeRightMarkers_2(int target)
{
    int counter = 0;
    bool onMarker = false;

    const int START_SPEED = 20;
    const int MAX_SPEED   = 60;
    const int MIN_SPEED   = 30;

    int currentSpeed = START_SPEED;

    // Start strafing
    speed_Upper_L = currentSpeed;
    speed_Lower_L = currentSpeed;
    speed_Upper_R = currentSpeed / 2;
    speed_Lower_R = currentSpeed / 2;

    mecanumCar.R_Move();

    unsigned long lastRamp = millis();
    unsigned long detectTime = 0;

    while(counter < target)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        //----------------------------------------------------
        // Smooth acceleration / deceleration
        //----------------------------------------------------

        int targetSpeed;

        if(counter < target-1)
            targetSpeed = MAX_SPEED;
        else
            targetSpeed = MIN_SPEED;

        if(millis() - lastRamp > 15)
        {
            lastRamp = millis();

            if(currentSpeed < targetSpeed)
                currentSpeed++;

            else if(currentSpeed > targetSpeed)
                currentSpeed--;

            speed_Upper_L = currentSpeed;
            speed_Lower_L = currentSpeed;

            speed_Upper_R = currentSpeed / 2;
            speed_Lower_R = currentSpeed / 2;
        }

        //----------------------------------------------------
        // Line correction while strafing
        //----------------------------------------------------

        if(left && !middle)
        {
            speed_Upper_L = currentSpeed-5;
            speed_Lower_L = currentSpeed-5;

            speed_Upper_R = currentSpeed/2+5;
            speed_Lower_R = currentSpeed/2+5;
        }

        else if(right && !middle)
        {
            speed_Upper_L = currentSpeed+5;
            speed_Lower_L = currentSpeed+5;

            speed_Upper_R = currentSpeed/2-5;
            speed_Lower_R = currentSpeed/2-5;
        }

        else
        {
            speed_Upper_L = currentSpeed;
            speed_Lower_L = currentSpeed;

            speed_Upper_R = currentSpeed/2;
            speed_Lower_R = currentSpeed/2;
        }

        //----------------------------------------------------
        // Marker detection
        //----------------------------------------------------

        if(!left && middle && !right)
        {
            if(!onMarker)
            {
                if(detectTime==0)
                    detectTime=millis();

                if(millis()-detectTime>20)
                {
                    counter++;

                    Serial.print("Marker : ");
                    Serial.println(counter);

                    onMarker=true;
                    detectTime=0;
                }
            }
        }
        else
        {
            onMarker=false;
            detectTime=0;
        }

        mecanumCar.R_Move();
    }

    //----------------------------------------------------
    // Smooth Stop
    //----------------------------------------------------

    while(currentSpeed>0)
    {
        currentSpeed--;

        speed_Upper_L = currentSpeed;
        speed_Lower_L = currentSpeed;

        speed_Upper_R = currentSpeed/2;
        speed_Lower_R = currentSpeed/2;

        mecanumCar.R_Move();

        delay(10);
    }

    mecanumCar.Stop();

    Serial.println("================================");
    Serial.println("RIGHT STRAFE COMPLETE");
    Serial.println("================================");

    delay(300);
}


void turnRight90_V2_o()
{
//dulu 60
    speed_Upper_L = 67;
    speed_Lower_L = 67;
    speed_Upper_R = 67;
    speed_Lower_R = 67;

    unsigned long start = millis();

    while (millis() - start < TURN_BLIND_MS)
        mecanumCar.Turn_Right();

    start = millis();
    //150 dulu
    while (millis() - start < 200)
    {
        bool left   = digitalRead(SensorLeft);
        bool middle = digitalRead(SensorMiddle);
        bool right  = digitalRead(SensorRight);

        mecanumCar.Turn_Right();

        if (!left && !middle && right)
            break;
    }

    mecanumCar.Stop();
    delay(100);
}

bool grab()
{
    Serial.println("Entered");

    // Open gripper
    myservo.write(7);
    delay(300);

    while (true)
    {
        // Move forward
        speed_Upper_L = 40;
        speed_Lower_L = 40;
        speed_Upper_R = 30;
        speed_Lower_R = 30;

        mecanumCar.Advance();

        // ---------- Ultrasonic ----------
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);

        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);

        digitalWrite(TRIG_PIN, LOW);

        long duration = pulseIn(ECHO_PIN, HIGH, 30000);

        if (duration == 0)
            continue;

        long distance = duration * 0.034 / 2;

        Serial.print("Distance: ");
        Serial.println(distance);

        // Keep moving until close enough
        if (distance > 4)
        {
            delay(50);
            continue;
        }

        // Object reached
        mecanumCar.Stop();
        Serial.println("Object detected.");

        // ---------- Read Color ----------
        digitalWrite(S2, LOW);
        digitalWrite(S3, LOW);
        redFrequency = pulseIn(SENSOR_OUT, LOW);

        digitalWrite(S2, HIGH);
        digitalWrite(S3, HIGH);
        greenFrequency = pulseIn(SENSOR_OUT, LOW);

        digitalWrite(S2, LOW);
        digitalWrite(S3, HIGH);
        blueFrequency = pulseIn(SENSOR_OUT, LOW);

        Serial.print("R: ");
        Serial.print(redFrequency);
        Serial.print(" G: ");
        Serial.print(greenFrequency);
        Serial.print(" B: ");
        Serial.println(blueFrequency);

        // ---------- Check Red ----------
        if (redFrequency < 1000)
        {
            Serial.println("Red object detected.");

            myservo.write(60);      // Close gripper
            delay(1000);

            Serial.println("Object grabbed.");
            return true;                  // Mission complete
        }
        else
        {
        Serial.println("Wrong color.");
        return false;
        }
    }
}

void strafeLeftMarkers_o(int target)
{
    int counter = 0;
    bool onMarker = false;
    speed_Upper_L = 52;
    speed_Lower_L = 45;
    speed_Upper_R = 55;
    speed_Lower_R = 55;

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

void returnHomeFromLocation1(){
    turnRight90_V2_o();
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    moveForwardIntersections(3);
    delay(1000);
    strafeLeftMarkers_o(2);
    delay(1000);
    moveForwardIntersections(4);
    delay(1000);
    mecanumCar.Stop();
    myservo.write(7);
}

void returnHomeFromLocation2(){
    turnRight90_V2_o();
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    moveForwardIntersections(7);
    delay(1000);
    mecanumCar.Stop();
    myservo.write(7);

}
void returnHomeFromLocation3(){
    turnRight90_V2_o();
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    moveForwardIntersections(3);
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    moveForwardIntersections(2);
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    turnRight90_V2_o();
    delay(1000);
    moveForwardIntersections(4);
    delay(1000);
    mecanumCar.Stop();
    myservo.write(7);
}

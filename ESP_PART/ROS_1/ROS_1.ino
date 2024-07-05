/*
 * This intend to connect to a Wifi Access Point
 * and a rosserial socket server.
 * You can launch the rosserial socket server with
 * roslaunch rosserial_server socket.launch
 * The default port is 11411
 *
 */
//LIBRARIES USED FOR THE CONNECTION
#include <WiFi.h>
#include <ros.h>
//Type of message that are going to be publised or recived
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Empty.h>

//Flag used to do the control each sampling time given by the timer
int PID_flag=0;

//Timer variable 
hw_timer_t *My_timer = NULL;    //timer

//PINS for the line sensors
const int L_line = 34;
const int R_line = 35;
//Variables used in the interruptions of the line sensors.
short int FlagA;
short int FlagB;

//PINS for encoder sensors
const int L_encoder = 32;
const int R_encoder = 33;
//Variables used in the interruptions of the encoders sensors.
int count_l=0; //count how many times the encoder read a hole 
int count_r=0;
short int c1; //maintain the final count and it is given to the PID control
short int c2;

//INTERRUPTION CALLBACKS
//LINE SENSOR INTERRUPTION CALLBACK
void IRAM_ATTR isr1(){
  if (digitalRead(L_line)==LOW)
  FlagA=1;
  else 
  FlagA=0;
}
void IRAM_ATTR isr2(){
  if (digitalRead(R_line)==LOW)
  FlagB=1;
  else 
  FlagB=0;
}
//ENCODER SENSOR INTERUPTION CALLBACK
void IRAM_ATTR isr3(){
  count_l+=1;
}
void IRAM_ATTR isr4(){
  count_r+=1;
}
//TIMER INTERRUPTION CALLBACK
void IRAM_ATTR timer1(){
  c1 = count_l; //TOTAL NUMBER OF HOLES READ BY THE ENCODER
  c2 = count_r;
  count_l=0; // RESTART THE COUNT AGAIN 
  count_r=0;
  PID_flag=1; // TELL THAT THE CONTROL MUST WORK 
}

// INITIAL CONDITION FOR START VARIABLE
int START=0;

// ROS AND ESP CONNECTION CONFIGURATION
const char* ssid     = "ssid";
const char* password = "pass";
// Set the rosserial socket server IP address
IPAddress server(172,20,10,3);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

//NODES
ros::NodeHandle node;
// PUBLISHER NODE 
std_msgs::Int16MultiArray str_msg; 
ros::Publisher sensors("sensor", &str_msg);   //"sensor" name of the topic and "str_msg" type of mess  


// SUSCRIBER NODE
//Calback 
void messageCb( const std_msgs::Empty& toggle_msg){ //calback name "messageCb"
     digitalWrite(2, HIGH-digitalRead(2));   // blink the led
     START=!START;
   }
ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );// SUSCRIBE TOPIC "toggle_led" 


void setup()
{
// put your setup code here, to run once:

//INTERRUPTIONS
//INTERRUPTION ASSOCIATED TO LINE_SENSORS
pinMode(L_line, INPUT_PULLUP);
attachInterrupt(L_line, isr1, CHANGE);
pinMode(R_line, INPUT_PULLUP);
attachInterrupt(R_line, isr2, CHANGE);
//INTERRUPTION ASSOCIATED TO ENCODERS_SENSORS  
pinMode(L_encoder, INPUT_PULLUP);
attachInterrupt(L_encoder, isr3, RISING);
pinMode(R_encoder, INPUT_PULLUP);
attachInterrupt(R_encoder, isr4, RISING);


//To turn on a LED for the start;
pinMode(2,OUTPUT);

//INTERRUPTION ASSOCIATED TO THE TIMER 
My_timer = timerBegin(3,80,true);
timerAttachInterrupt(My_timer, &timer1, true);
timerAlarmWrite(My_timer,100000, true);
timerAlarmEnable(My_timer);
  

//CONNECTION 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  // Set the connection to rosserial socket server
  node.getHardware()->setConnection(server, serverPort);
  //Start the nodes
  node.initNode();
  node.advertise(sensors);
  node.subscribe(sub);
}

void loop()
{
  
  if (node.connected()) { //if the connection was succesfull continue
    if (START){// if the start was sended by the suscriber node
    if (PID_flag){ // Each Ts=0.1 given by the timer send sensor info to be processed by the control node
    short int value[4]= {c1,c2,FlagA,FlagB}; //value is the data transmited
    str_msg.data = value; 
    str_msg.data_length=4; 
    sensors.publish( &str_msg );
    PID_flag=0;
    }
 }
 } 
  node.spinOnce(); //will call all the callbacks
}

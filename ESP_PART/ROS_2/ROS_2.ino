//LIBRARIES USED FOR THE CONNECTION
#include <WiFi.h>
#include <ros.h>
//Type of message that are going to be recived
#include <std_msgs/Int16MultiArray.h>

//VARIABLE THAT WILL RECIEBE THE DATA OF PID
short int uu[1];

// ROS AND ESP CONNECTION CONFIGURATION
const char* ssid     = "ssid";
const char* password = "passwrod";
// Set the rosserial socket server IP address
IPAddress server(172,20,10,3);
// Set the rosserial socket server port
const uint16_t serverPort = 11412;

//NODES
ros::NodeHandle node2;
// SUSCRIBER NODE
//Calback 
void messageCb( const std_msgs::Int16MultiArray& arr){ //calback name "messageCb"
  for (int i=0; i<arr.data_length;i++){
   uu[i]=arr.data[i];
  }
 }
ros::Subscriber<std_msgs::Int16MultiArray> sub2("pid", &messageCb ); //subscibe to topic pid

//pwm
#define LEDC_CHANNEL_2     2
#define LEDC_CHANNEL_3     3

// use 8 bit precission for  timer
#define LEDC_TIMER_8_BIT  8
// use 1000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     1000


int LED_PIN2=13;     
int LED_PIN3=12;


void setup() {
  // put your setup code here, to run once:

//PWM

ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
ledcAttachPin(LED_PIN2, LEDC_CHANNEL_2);
ledcAttachPin(LED_PIN3, LEDC_CHANNEL_3);
ledcWrite(LEDC_CHANNEL_2, 0);
ledcWrite(LEDC_CHANNEL_3, 0);

pinMode(32,INPUT);  //Read pin 2 of the first esp, start/stop
 
//CONNECTION 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  // Set the connection to rosserial socket server
  node2.getHardware()->setConnection(server, serverPort);
  //Start the nodes
  node2.initNode();
  node2.subscribe(sub2);
  //
  }

void loop() {

 if (node2.connected()) {
  uu1=uu[1];
  uu2=uu[2]);

if (FlagA == 0 && FlagB ==0){

  ledcWrite(LEDC_CHANNEL_2, uu1);
  ledcWrite(LEDC_CHANNEL_3, uu2);
}

if (FlagA == 0 && FlagB ==1){
  ledcWrite(LEDC_CHANNEL_2, 0);
  ledcWrite(LEDC_CHANNEL_3, uu2);
}

if (FlagA == 1 && FlagB ==0){
  ledcWrite(LEDC_CHANNEL_2, uu1);
  ledcWrite(LEDC_CHANNEL_3, 0);
}
 if (FlagA ==1 && FlagB ==1){
  ledcWrite(LEDC_CHANNEL_2, 0);
  ledcWrite(LEDC_CHANNEL_3, 0);
}
      
  } else{
ledcWrite(LEDC_CHANNEL_2, 0);
ledcWrite(LEDC_CHANNEL_3, 0);
  }
  node.spinOnce();

}

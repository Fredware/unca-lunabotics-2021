// Basic demo for accelerometer/gyro readings from Adafruit ISM330DHCX

#include <Adafruit_ISM330DHCX.h>

#include <ros.h>
#include <std_msgs/Float32.h>
//#include <sensor_msgs/Imu.h>

#define BAUD 115200
#define PUB_INTERVAL 1// [millis]

Adafruit_ISM330DHCX ism330dhcx;

ros::NodeHandle nh;

std_msgs::Float32 msg;
//sensor_msgs::Imu msg;
ros::Publisher imu_pub("my_imu", &msg);

void setup(void) {
/*****************************************SERIAL**********************************************/  
//  Serial.begin(115200);
//  Serial.println("Adafruit ISM330DHCX test!");

/*****************************************SENSOR**********************************************/  
  ism330dhcx.begin_I2C();

  ism330dhcx.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  ism330dhcx.setGyroRange(LSM6DS_GYRO_RANGE_125_DPS);
  
  ism330dhcx.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
  ism330dhcx.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);
  
  ism330dhcx.configInt1(false, false, true); // accelerometer DRDY on INT1
  ism330dhcx.configInt2(false, true, false); // gyro DRDY on INT2

/*****************************************NODE**********************************************/  
  nh.getHardware()->setBaud(BAUD);
  nh.initNode();
  nh.advertise(imu_pub);
}

long pub_timer;

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  if( millis() > pub_timer){
   ism330dhcx.getEvent(&accel, &gyro, &temp); 

   msg.data = accel.acceleration.x;
   imu_pub.publish(&msg);
   msg.data = accel.acceleration.y;
   imu_pub.publish(&msg);
   msg.data = accel.acceleration.z;

   imu_pub.publish(&msg);   
   pub_timer = millis() + PUB_INTERVAL;
  }

  nh.spinOnce();


/*****************************************SERIAL**********************************************/  
//  Serial.print(accel.acceleration.x);
//  Serial.print(","); Serial.print(accel.acceleration.y);
//  Serial.print(","); Serial.print(accel.acceleration.z);
//  Serial.print(",");
//
//  Serial.print(gyro.gyro.x);
//  Serial.print(","); Serial.print(gyro.gyro.y);
//  Serial.print(","); Serial.print(gyro.gyro.z);
//  Serial.println();
//  delay(1000);
}

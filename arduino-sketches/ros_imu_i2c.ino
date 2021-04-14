/**
 * Author: 
 *  Fredi R. Mino
 * Date: 
 *  Apr 14, 2021
 * 
 * Description:
 *  This program communicates with an ISM330DLC sensor using I2C. It reads 6 values for linear
 *  acceleration and angular velocity. Then, it formats these values so that they can be sent
 *  to a ROS topic using the Arduino rosserial library.
*/

#include <Adafruit_ISM330DHCX.h>

#include <ros.h>
#include <sensor_msgs/Imu.h>

#define BAUD 115200
#define PUB_INTERVAL 1// [millis]

Adafruit_ISM330DHCX ism330dhcx;

ros::NodeHandle nh;

sensor_msgs::Imu msg;
ros::Publisher imu_pub("my_imu", &msg);

void setup(void) {
/*****************************************SENSOR*********************************************/
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

unsigned long pub_timer;

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  if( millis() > pub_timer){
   ism330dhcx.getEvent(&accel, &gyro, &temp); 

   msg.header.stamp = nh.now();
   
   msg.angular_velocity.x = gyro.gyro.x;
   msg.angular_velocity.y = gyro.gyro.y;
   msg.angular_velocity.z = gyro.gyro.z;

   msg.linear_acceleration.x = accel.acceleration.x;
   msg.linear_acceleration.y = accel.acceleration.y;
   msg.linear_acceleration.z = accel.acceleration.z;

   imu_pub.publish(&msg);   
   pub_timer = millis() + PUB_INTERVAL;
  }
  
  nh.spinOnce();
}

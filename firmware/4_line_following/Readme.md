# TABLE OF CONTENTS
* [PID controller](#PID-Controller)
    * [Theory](#Theory)
    * [Error Calculation](#Error-Calculation)
    * [Use of PID](#Use-of-PID)
    * [Use of Wifi module](#Use-of-wifi-module)
    * [Algorithm](#Algorithm)
* [Description Of Functions Used](#Description-of-functions-used)

# PID Contoller

## Theory
Line Following is one of the most important aspects of robotics.
A Line Following Robot is an autonomous robot which is able to follow a line (usually black or white) that is drawn on a surface consisting of a contrasting color. It is designed to move automatically and follow the line.

![2-PID-controller-Arduino](https://user-images.githubusercontent.com/103832825/208858059-9e96e7c5-80dc-4473-beba-a7648f62a8f7.png)

The robot uses arrays of sensors (LSAs) to identify the line, thus assisting the robot to stay on the track. 

### Error Calculation
* First, we multiply the values of the LSA with certain weights. Here we have used 3,1,0,-1, and -3
* 3 for the leftmost sensor, -3 for the rightmost
* The more negative the resulting weighted sum, the more the robot is to the left of the line, and vice versa.

### Use of PID

PID stands for **P**roportional-**I**ntegral-**D**erivative
It is one kind of device used to control different process variables like pressure, flow, temperature, and speed in industrial applications. In this controller, a control loop feedback device is used to regulate all the process variables.This type of control is used to drive a system in the direction of an objective location otherwise level.
> * [Follow this link for detailed explanation of PID](https://www.youtube.com/playlist?list=PLn8PRpmsu08pQBgjxYFXSsODEF3Jqmm-y)
> * [This link explains PID with respect to line folllowing](https://youtu.be/4Y7zG48uHRo)


### Use of Wifi Module
Although simple in concept, the mathematics underpinning PID control is complex and acheiving optimal performance entails selecting process-specific values for a range of interacting parameters.
The process of finding these values is referred to as “tuning.” When a PID controller is tuned optimally, the device minimizes deviation from the set point, and responds to disturbances or set point changes quickly but with minimal overshoot.

For easy tuning we use wifi module that dynamically changes the `kp,ki,kd` values
 1. User connects to the server using Wi-Fi
 2. Server sends the HTML file back to the user
 3. Server receives the kp,ki,kd values for tuning
 4. Server makes appropriate changes to the bot
 5. Back to step 3

![wifi](https://user-images.githubusercontent.com/103832825/208859651-6cbe3176-214c-4d42-9b09-46669f722ae4.png)

## ALGORITHM
The process to implement a line following robot can be summarized in these basic steps :
Sensors detect deviation from line
> 1. Find error
> 2. Find error correction
> 3. Correct the error
> 4. Follow the line

![chart](https://user-images.githubusercontent.com/103832825/208859859-47704e92-be6e-4ce5-baf3-9f2a8d45414f.png)

# Description of the functions
```c
void calculate_error()
```
**Description**: Uses sensor readings to calculate the error.
> * error is calculated by multiplying weights to the sensor reading and taking a weighted sum.
> * wieghted sum is divided by sum to calculate position wrt to line
> * if all sensors were black, reached a dead end, we decide the direction according to previous error and assign error of 2.5
```c
void calculate_correction()
```
**Description**: Uses error calculated by the error function to calculate the correction. 
> ```c    correction = read_pid_const().kp*error + read_pid_const().ki*cumulative_error + read_pid_const().kd*difference ```

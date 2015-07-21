 stereo_guided_copter

 The goal of this code is to accomplish stereo vision on a quadcopter for obstacle avoidance.
 There are three software components needed to accomplish this objective:
 First:    Capturing an image using dual webcams and processing them to create a dense disparity map of absolute distance from the camera.
 Second:   To process the disparity map and use it to make navigational decisions based on the obstacle and its relative position to the goal.
 Third:    To carry out the decision by sending it as a PPM signal to a flight controller board, which allows us to abstract the copter and dispenses with the need for a real-time system running our code.

 Only the first and third components have been addressed this far.

 Stereo vision was addresed using python and opencv3, this code is contained in the camTest.py file.  It waits until it detects two webcams connected to the system and opens them and continually creates and displays sparse disparity maps to the screen
 Signals were sent over PPM by sending them serially to an arduino, which then converted them to a PPM signal and sent them directly to the flight controller.
 The file serialTest.ino is for the arduino, the SerialCom.py works on a raspberry pi, and the serialComPc is the corresponding Windows PC version.
 The serial software utilizes a very simple communication protocol with minimal error-checking and has only been partially tested, but the underlying framework performs well.

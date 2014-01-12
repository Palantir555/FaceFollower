FaceFollower
============

This Qt application captures images from a webcam, detects a face using OpenCV, and sends information through a serial port about how to move the webcam in order to keep the face in the middle of the image.

I have added code for an Arduino, which will receive the information from the application and move the 2 servomotors on which the webcam is mounted.

![Motors+webcam structure](http://i.imgur.com/L6diwf9.jpg?1)


Note: This is a first version, with the serial port and camera selection hardcoded, as well as other limitations. I expect to keep working on it as soon as I have some spare time.

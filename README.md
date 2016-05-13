# AVCteam Numoro Uno
AVC stuff. 
##Checklist for Week One
- [x] Hardware: Plan the robot, and create a prototype of how the robot will work most efficently
- [ ] Software: working out what the values the sensor are going to give
- [x] Software: How to send code to the pi
- [x] Software: Constructing code for the robot to go forwards
- [x] Simran: Write a progress report for week one

##Week One Review
- We planned the robot, and we were able to make a prototype of the robot and see if it was efficent enough *(this is a base for the robot so that it is in a functional state to test code with)*.
- Since we did not get how to work out what the sensor values are going to give, the software team will try completing this      task next week.
- We connected up the pi, and were able to send code to it
- Everyone has signed the AVC plan and submitted it.
- We all attended the labs/meetings held

##Checklist for Week Two
- [x] Hardware: Choose the design for the robot
- [x] Software: Fix the bug (get_pixel() returning a row out of range error, *row and col values needed to be swapped*)
- [x] Software: Working out what the values the sensor are going to give
- [ ] Software: Try get the robot to follow the line for quadrant one
- [x] Simran: Write a progress report for week two
- [x] Simran: Plan a structure for the progress report

##Week Two Review
- The hardware have decided on the design/theme of the robot *(focus on being short in length for maze corners)*
    Side affect of short design is robot is tall making it unstable in abrupt turns, due to this may change to small wheels to reduce height
- We were able to fix the bug we encountered in the lab by swapping row and col values in the get_Pixel()
- We were not able to get the robot to follow the line for quadrant one (the robot kept spinning in a circle, this is happened   because we had an error in the code), we will complete this in week three.
- Everyone attended the lab this week

##Checklist for Week Three
- [ ] Hardware: Making something to support the IR sensors 
- [x] Hardware: Change the position of the camera
- [x] Hardware: Change the wheels to smaller 
- [x] Software: Fix error in the code to make the robot follow the line for quadrant one
- [x] Simran: Weekly Report
- [x] All: Discuss the progress report, so we all write similar things
- [x] Software: Sending code to sever to open gate

##Week Three Review
- Everyone attended the Lab session
- We decided to record the testing we are doing in this weeks reviews, as our understanding and code is at a stage where the robot will begin to function as intened. This also allows the team to access it and use it in their report.
- Hardware: We are putting the camera on an angle again so we can see the line and what is on either side of it, this is to solve an issue of having the camera facing straigth down and located low resulting in it being able to see a very little area including the line but not much either side making controlling movement difficult/impossible.
- Software (Testing): More code fixing and adapting through tests. Testing showed us that the robot was moving to fast as it kept losing the line. We also found that that it could cope with one side of the white line and not the other. This was because it is over adjusting too much.
-Software (Testing): We fixed the robot spinning in circles by making one of the motor speeds negative because before we had both motor speeds at positive *issue arose while making code changes*. Making one negative and the other positive makes the wheels both turn the same way.
- Software (Testing): We also had an error that the robot was adjusting to the wrong side of the line *turning the opposite way to that expected*, we fixed this by changing the value of the proportional value (from negative to positive and positive to negative) *Wheels had been turning the wrong way*
- Software (testing): Halved the kp (proportional constant) constant which made the robot follow the line much better.
- We fixed the noticed errors and managed to get the robot through quadrant one
- Software: Gate sucessfully opened. Need to implement code to make it wait at gate for it to open
- 

##Checklist for week four
- [ ] Software: Tune line following code. Make it more smoother
- [ ] Software: Handle junctions on line.
- [ ] Hardware: Plan mount for IR sensors
- [ ] Software: Get sensor value from IR sensors
- [ ] Hardware: Finalise plan for robot's look

# robot
Robot Movement IO 


SCENARIO

Convert to indsutry level, 
-Buat cam last task

Combine part:
----------------------------------------------------
technical part
buisness part 
----------------------------------------------------


Task Group 5 (Data Titan)

Group 5: The Outdoor Campus Delivery Rover

Mission Brief: A rugged robot that delivers food across
a university campus, dealing with outdoor elements
and crosswalks.
Key Behaviors for the FSM:

Start: Idle/Docked at the student union.
Trigger: App order received, transitions to GPS
Navigation.

Crosswalk Protocol: When it reaches a road
(detected by map coordinates), it transitions to
Waiting for Walk Signal. It uses a camera to look for
the green pedestrian light before transitioning back
to GPS Navigation.

Weather Failsafe: If its moisture sensor detects
heavy rain, it transitions to Seeking Shelter (driving
to the nearest covered waypoint) and waits there
until the rain stops.

Delivery: Arrives at the dorm and enters a "Waiting
for PIN" state. Once the student inputs the correct
PIN via the app, the rover opens its hatch and
signals the dorm's automated portico arm. The arm
safely extracts the food package and hands it to
the student. Once the hatch area is clear, the rover
seals itself and goes home

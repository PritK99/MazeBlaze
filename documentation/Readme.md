# MazeBlaze-v2

# TABLE OF CONTENTS
* [PID controller](#PID-Controller)
    * [Theory](#Theory)
    * [Error Calculation](#Error-Calculation)
    * [Use of PID](#Use-of-PID)
    * [Use of Wifi module](#Use-of-wifi-module)
    * [Algorithm](#Algorithm)
    * [Description Of Functions Used](#Description-of-functions-used)
* [Left Follow Rule](#Left-follow-rule)
    * [Theory](#Theory)
    * [Error Calculation](#Error-Calculation)
    * [Use of PID](#Use-of-PID)
    * [Use of Wifi module](#Use-of-wifi-module)
    * [Algorithm](#Algorithm)
    * [Description Of Functions Used](#Description-of-functions-used)

# <b><u>PID Contoller</b></u>

## <b><u>Theory</b></u>
Line Following is one of the most important aspects of robotics.
A Line Following Robot is an autonomous robot which is able to follow a line (usually black or white) that is drawn on a surface consisting of a contrasting color. It is designed to move automatically and follow the line.

![2-PID-controller-Arduino](https://user-images.githubusercontent.com/103832825/208858059-9e96e7c5-80dc-4473-beba-a7648f62a8f7.png)

The robot uses arrays of sensors (LSAs) to identify the line, thus assisting the robot to stay on the track. 

## <b><u>Error Calculation</b></u>
* First, we multiply the values of the LSA with certain weights. Here we have used 3,1,0,-1, and -3
* 3 for the leftmost sensor, -3 for the rightmost
* The more negative the resulting weighted sum, the more the robot is to the left of the line, and vice versa.

## <b><u>Use of PID</b></u>

PID stands for **P**roportional-**I**ntegral-**D**erivative
It is one kind of device used to control different process variables like pressure, flow, temperature, and speed in industrial applications. In this controller, a control loop feedback device is used to regulate all the process variables.This type of control is used to drive a system in the direction of an objective location otherwise level.
> * [Follow this link for detailed explanation of PID](https://www.youtube.com/playlist?list=PLn8PRpmsu08pQBgjxYFXSsODEF3Jqmm-y)
> * [This link explains PID with respect to line folllowing](https://youtu.be/4Y7zG48uHRo)


## <b><u>Use of Wifi Module</b></u>
Although simple in concept, the mathematics underpinning PID control is complex and acheiving optimal performance entails selecting process-specific values for a range of interacting parameters.
The process of finding these values is referred to as “tuning.” When a PID controller is tuned optimally, the device minimizes deviation from the set point, and responds to disturbances or set point changes quickly but with minimal overshoot.

For easy tuning we use wifi module that dynamically changes the `kp,ki,kd` values
 1. User connects to the server using Wi-Fi
 2. Server sends the HTML file back to the user
 3. Server receives the kp,ki,kd values for tuning
 4. Server makes appropriate changes to the bot
 5. Back to step 3

![wifi](https://user-images.githubusercontent.com/103832825/208859651-6cbe3176-214c-4d42-9b09-46669f722ae4.png)

## <b><u>Algorithm</b></u>
The process to implement a line following robot can be summarized in these basic steps :
Sensors detect deviation from line
> 1. Find error
> 2. Find error correction
> 3. Correct the error
> 4. Follow the line

![chart](https://user-images.githubusercontent.com/103832825/208859859-47704e92-be6e-4ce5-baf3-9f2a8d45414f.png)

## <b><u>Description of the functions</b></u>
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
> ```correction = read_pid_const().kp*error + read_pid_const().ki*cumulative_error + read_pid_const().kd*difference ```

# <b><u>Left Follow Rule</b></u>

## <b><u>Theory</b></u>

The “left-hand rule” or “left-follow-algorithm” is a Maze solving technique in which you place your left hand on the wall to your left, and keep it touching the wall as you move through the Maze. In other words, you continually take left turns. A parallel rule is the “right-hand rule” which is the same but you instead always take right turns.

These wall following Maze solving techniques will always solve any 2D Maze, as long as the section of walls at the start is connected to the section of walls at the finish. This means wall following (although simple and easy to apply) won’t work to solve every possible Maze. In scenarios where wall following doesn’t work, you’ll go in a circle around the section of wall you’re connected with and return to where you started.

In simple words, the "Left Hand Rule" approach is to make your way through the maze, while choosing how to turn at intersections as follows:

```
1) Always turn left if you can
2) If you cannot turn left, go straight
3) If you cannot turn left, or go straight, turn right
4) If you cannot turn left, go straight, or turn right, turn around because you must be at a dead end
```

## <b><u>Implementation</b></u>

For the implementation of left follow, we need to record every turn taken by the bot. Then we need to recognize the redundant paths from the given turns.

We assign numbers to each direction and then store these numbers in an array called <b>dry_run[ ]</b>.

```
North - 1
East - 2
South - 3
West - 4
```

We always consider that the bot initially is always facing the North direction. Thus other directions are established as per the North direction.
Once established, these directions are fixed with respect to world frame, and the bot keeps track of all these directions whenever it takes turns.

Our program is going to keep track of the serial numbers (direction index) of the above assumptions only. When ever our robot encounters a node or turns, it has to make an entry of one of these direction index into the direction array.
## <b><u>1) Condition for left</u></b>

Whenever the bot takes a <b>left</b>, it simply has to <b>subtract 1 from the previous reading</b> in array to get current reading.

For eg. Assume the bot is moving in East i.e. 2 and takes a left, in this case it gives 1 which implies bot is now facing North.

<b>Note</b> : Due to the circular definition from 1 to 4, subtracting 1 from 1, gives 4 i.e. West is left to North.

## <b><u>2) Condition for Straight</u></b>

Whenever the bot goes <b>straight</b>, the current reading is same as previous reading</b> in array.

For eg. Assume the bot is moving in East i.e. 2 and takes goes straight, the current reading will be 2 which implies bot ccontinues to move in East

## <b><u>3) Condition for Right</u></b>

Whenever the bot takes a <b>Right</b>, it simply has to <b>add 1 from the previous reading</b> in array to get current reading.

For eg. Assume the bot is moving in East i.e. 2 and takes a right, in this case it gives 3 which implies bot is now facing South.

<b>Note</b> : Due to the circular definition from 1 to 4, adding 1 to 4, gives 1 i.e. North is right to West.

## <b><u>4) Condition for Dead-End</u></b>

Whenever the bot takes a <b>left</b>, it simply has to <b>subtract 2 from the previous reading</b> in array to get current reading.

For eg. Assume the bot is moving in East i.e. 2 and finds a dead end, in this case it gives 3 which implies bot is now facing west after truning.

<b>Note</b> : Due to the circular definition from 1 to 4, subtracting 2 from 1, gives 3 i.e. bot will go south if it hits a dead end at north

Thus, the <b>General rule</b> :

```
1) For left : dry_run[i] = dry_run[i-1] - 1
2) For Straight : dry_run[i] = dry_run[i-1]
3) For Right : dry_run[i] = dry_run[i-1] + 1
4) For Dead End : dry_run[i] = dry_run[i-1] - 2

for all i > 0 and dry_run[0] = 1 (The diection which bot faces initially, is always North)
```

## <b><u>Example</b></u>

![image](https://user-images.githubusercontent.com/103832825/209423014-8fc88d07-ed65-4b5d-8687-d810516f8c0e.png)

Thus, for the given maze, Assuming starting position as North i.e. 1, The array dry_run[ ] will store the following,

```
dry_run[ ] = {1, 4, 1, 2, 1, 4, 1, 1, 3, 2, 1, 3, 2, 1, 2, 4, 3, 2, 3, 3, 4, 3, 2, 1, 2, 1, 4, 1, 1, 3, 2}
```

Here, the redundant paths are highlighted in bold

dry_run[ ] = {1, 4, 1, 2, 1, 4, 1, <b>1, 3</b>, 2, <b>1, 3</b>, 2, <b>1, 2, 4, 3</b>, 2, 3, 3, 4, 3, 2, 1, 2, 1, 4, 1, <b>1, 3</b>, 2}

Thus, when we call simply_path(), it removes all these redundancies and stores the shortest path in an array called final_run[ ].

```
final_run[ ] = {1, 4, 1, 2, 1, 4, 1, 2, 3, 2, 3, 3, 4, 3, 2, 1, 2, 1, 4, 1, 2}
```

Thus, we make use of simplify_path() function in our code :

![image](https://user-images.githubusercontent.com/103832825/209433300-b896ce58-6e02-4112-8874-0370b74ad662.png)

# <b><u>Problems with current algorithm</b></u>

The <b>Left-Follow algorithm</b> helps in reducing the <b>redundancies</b> from the path. However, the problem with Left-Follow rule is that the entire maze does <b>not</b> get <b>mapped</b>, and thus the path so obtained in final_run[ ] is <b>not</b> the <b>most optimum path</b>.

However,The problem with Left-Follow rule is that the entire maze does not get mapped, and thus the path so obtained in final_run[ ] is <b>not</b> the <b>most optimum path</b>.

For the general algorithm, we need a method to <b>map the whole maze</b>, which is required if the maze is complex.

Thus there are a several other alternatives, which help in mapping the maze completely and calculate shortest path. During our project, we studied several algorithms to traverse maze and find the shortest path.

# <u><b>Other Algorithms</b></u>

A maze can be represented as a graph G ( V, E ) , where V refers to the number of nodes and E refers to the number of edges.
Thus we can modify and apply several graph algorithms over a given maze to solve it.

## <b><u>Depth First Search (DFS)</b></u>

The proposed maze mapping system is based on coordinate system and after mapping the whole maze as a graph in standard 'Adjacency-List representation' or 'Adjacency-Matrix representation' method, shortest path and shortest time path can be extracted using Dijkstra's algorithm or floodfill algorithm.

While traversing, the first thing that we need to do is we need to keep track of which vertices we've already found. And for this, we're going to associate a boolean variable to each vertex visited(v) which basically tells us have we visited it yet.

The next thing that we're going to need to do is we need to is to keep track of the list of vertices that still have edges hanging off of them that might connect this to something new or in other words, are unexplored. 

The final thing is we need to discover which order to discover, to follow new edges in. And for this we are going to use what is known as the Depth First order. What this means is we're just going to start our initial vertex and we're just going to start following a chain of edges. 

We're just going to follow some really long path forward until one of two things happens. 

* One thing is we could stumble across a vertex that we have already seen before. In which case there's no reason to have followed that edge and we'll just back up to where we were before.

    This is the case where a <b>loop</b> exists in maze and our bot returns to the same vertex again.
* The second thing that could happen though is that we hit a dead end. And we actually hit a <b>dead end</b> and can't go any further forward, then we actually back up. And then once we back up though, we don't just back all the way to the beginning.

    We just back up once step and then try going forwards again from that new vertex that we found.

Okay, so that's the basic idea. How do we implement this? Well part of the beauty about this is that we have a very simple recursive algorithm. So Explore(v), the first thing you do is you set the visited marker of v to be true. We say we have visited it. Next, for each neighbor w of v, for each w that's adjacent to v, if w has not already been visited, we recursively explore w.

This goes on and on till we hit one of the two conditons mentioned above. These two conditions are the base conditions for recursive calls in our case.
 
<a href = "https://www.simplilearn.com/tutorials/data-structure-tutorial/dfs-algorithm">Click here</a> to learn more about DFS.

Apart from exploring all the nodes, we need to store the cost of edges too. In order to find the coordinates of the turning points and junctions, linear distance between the points are needed, for which wheel encoder was used. Considering a tolerance of 3-5 %, encoders gives us almost exact linear distance between two points from the reading of wheel encoder of the robot moving in a linear path.

We interfaced Encoders in our bot using interrupt service routine (ISR) to give proper distance between two nodes.This is the <a href = "https://github.com/PritK99/MazeBlaze-v2.1/tree/main/firmware/3_encoders" >code</a> for interfacing ISR in Esp32 and obtaining readings from encoders.

## <b><u>Djikstra's Algorithm</b></u>

## <b><u>FloodFill Algorithm </b></u>

Flood fill algorithm itself is analogous to flooding a maze with Water.Water continues to flow to flood the whole maze. Path traversed by the first water drops until it reaches the start location is the shortest path to reach that goal.

How it works flood fill algorithm is to start giving value to each cell in the maze. The process of scoring was done by observing the position of the existing walls of the maze. The first water-filled cells are the cells of interest and these cells are given a value of 0. The water then flows into the surrounding area which is not blocked by the wall. The next cell that has been filled with water will be assigned a value of 1, then this value will continue to grow to the next cell to the entire cell occupied by water maze. The robot cannot move diagonally and the robots have learned some of the positions of the existing wall.

 The values of these cells represents the <b>manhattan distance</b> of each cell to the destination cell. If the robot is in a cell that is worth 2, the robot is located as far as 2 cells from the cells of interest, which is the end of maze. 
 
 Assume cells that are at the bottom left of the initial cell, then searched the cell, which has a smaller value than the value of the cell that is being occupied. The path is the shortest path is formed which can be reached from the initial cell leading to the destination cell With the flood fill algorithm, each time the robot reaches a new cell, the robot needs to update the mapping of the dead ends, refill each cell with the new values, determine neighboring cells which have the smallest value, and continue moving towards neighboring cells which have the smallest value.

<a href = "https://www.youtube.com/watch?v=GoJ-K7gniFI">Click here</a> for detailed explaination of floodfill.

<a href = "https://www.youtube.com/watch?v=Zwh-QNlsurI">Click here</a> for an example of maze solved by floodfill. 

<a href = "https://www.youtube.com/watch?v=7Q_RRomuXYk">Click here</a> for reference to pseudo-code and implementation of floodfill algorithm.

Implementing these algorithms in MazeBlaze-v2.1 is one of our future aims.

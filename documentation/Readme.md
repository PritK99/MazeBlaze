# MazeBlaze-v2
## <b><u>Left Follow Rule</b></u>

### <b><u>Theory</b></u> 

The “left-hand rule” or “left-follow-algorithm” is a Maze solving technique in which you place your left hand on the wall to your left, and keep it touching the wall as you move through the Maze. In other words, you continually take left turns. A parallel rule is the “right-hand rule” which is the same but you instead always take right turns.

These wall following Maze solving techniques will always solve any 2D Maze, as long as the section of walls at the start is connected to the section of walls at the finish. This means wall following (although simple and easy to apply) won’t work to solve every possible Maze. In scenarios where wall following doesn’t work, you’ll go in a circle around the section of wall you’re connected with and return to where you started.
 
In simple words, the "Left Hand Rule" approach is to make your way through the maze, while choosing how to turn at intersections as follows:

```
1) Always turn left if you can
2) If you cannot turn left, go straight
3) If you cannot turn left, or go straight, turn right
4) If you cannot turn left, go straight, or turn right, turn around because you must be at a dead end
```

### <b><u>Implementation</b></u>
 
For the implementation of left follow, we need to record every turn taken by the bot. Then we need to recognize the redundant paths from the given turns.

We assign numbers to each direction and then store these numbers in an array called <b>dry_run[ ]</b>.

```
North - 1
East - 2 
South - 3
West - 4
```

We always consider that the bot initially is always facing the North direction. Thus other directions are established as per the North direction. 
Once established, these directions are fixed with respect to world frame, and the bot keeps track of all these directions whenever it takes turns

### <b><u>1) Condition for left</u></b>

Whenever the bot takes a <b>left</b>, it simply has to <b>subtract 1 from the previous reading</b> in array to get current reading.

For eg. Assume the bot is moving in East i.e. 2 and takes a left, in this case it gives 1 which implies bot is now facing North.

<b>Note</b> : Due to the circular definition from 1 to 4, subtracting 1 from 1, gives 4 i.e. West is left to North.

### <b><u>2) Condition for Straight</u></b>

Whenever the bot goes <b>straight</b>, the current reading is same as previous reading</b> in array.

For eg. Assume the bot is moving in East i.e. 2 and takes goes straight, the current reading will be 2 which implies bot ccontinues to move in East
### <b><u>3) Condition for Right</u></b>

Whenever the bot takes a <b>Right</b>, it simply has to <b>add 1 from the previous reading</b> in array to get current reading.

For eg. Assume the bot is moving in East i.e. 2 and takes a right, in this case it gives 3 which implies bot is now facing South.

<b>Note</b> : Due to the circular definition from 1 to 4, adding 1 to 4, gives 1 i.e. North is right to West.

### <b><u>4) Condition for Dead-End</u></b>

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

### <b><u>Example</b></u>

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

```
UNDER REVIEW 
PLEASE IGNORE FOR NOW

A maze can be represented as a graph G ( V, E ) , where V refers to the number of nodes and E refers to the number of edges. 
Thus we can modify and apply several graph algorithms over a given maze to solve it.

We studied several algorithms to traverse maze and find the shortest path. 

In order to traverse the maze, we can use DFS or BFS. Once we get the knowledge of all the vertices, edges and cost related to each edge, we can implement algorithms like dijkstra's algorithm to get the shortest path. We can also modify Flood fill algorithm and use it to solve the maze.

One of the approaches which we decided to work on was to implement DFS to traverse the maze and then use dijkstra's algorithm to find the shortest path.

We referred few research papers and github repository to implement the algorithm. We even interfaced encoders, which will allow us to to obtain cost of an edge i.e. the distance between two vertices.

However, due to the shortage of time, we decided to use left-follow rule to solve the maze.

```

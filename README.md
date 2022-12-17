# MazeBlaze-v2.1

## Table of Contents

- [Project](#MazeBlaze-v2)
  - [Table of Contents](#table-of-contents)
  - [About The Project](#about-the-project)
  - [Demo](#demo)
  - [File Structure](#file-structure)
  - [Future Work](#future-work)
  - [Contributors](#contributors)
  - [Acknowledgements and Resources](#acknowledgements-and-references)
  - [License](#license)
  
## About

In this project, we will be working on Improvising MazeBlaze bot and Implement a maze solving algorithm on the MazeBlaze bot. The Main aim is to design an algorithm , which will enable us to map the entire maze and hence give us the shortest path. Further , we have improvised the design by understanding the current electronics and t 

The end goal of our project is to form a line-following, Maze-solving bot , which can find the shortest path in the given maze.

## Domains Explored
Embedeed C, Graph algorithms, electronics, Basics of PCB design etc.

## Project Workflow 
- Learn to design PCB and understand the design of PCB used in MazeBlaze bot
- Understand and improvise electronic components used in MazeBlaze bot
- Testing Line-sensor-array, motor drivers and interfacing encoders.
- Implementing left follow rule 
- Implementing path plannig


## File Structure
```
👨‍💻MazeBlaze-v2.1
 ┣ 📂assets                            // Installation.md & all reference gifs, images
 ┣ 📂firmware                          // Code files 
   ┣ 📂1_lsa                           // Contains code for getting readings from LSA 
   ┃ ┣ 📂main                         
   ┃ ┃ ┗ 📄main.c 
   ┃ ┃ ┣ 📄CMakeList.txt
   ┃ ┣ 📄CMakeList.txt
   ┣ 📂3_encoders                      // Contains code for interfacing encoders 
   ┣ 📂4_line_following                // Contains code for line following
   ┣ 📂5_left_follow                   // Contains code for left-follow-rule
   ┣ 📂6_path_planning                 // Contains code for path planning
  ┣ 📂Components                       // Contains all the header and source files used in project
     ┣ 📂include                       
     ┣ 📂src 
     ┣ 📂esp-wifi-logger
     ┣ 📄CMakeList.txt
     
``` 
## Future Work

- [ ] Implementing better algorithms for Maze solving such as DFS, BFS, Djikstra's algorithm etc. on the bot
- [ ] Improve on the design and model of mazzblaze
- [ ] Achive higher speeds by increasing the efficiency of bot

## Contributors
* [Prit Kanadiya](https://github.com/PritK99)
* [Raj Gupta](https://github.com/RajGupta17)


## Acknowledgements and References
* [SRA VJTI](http://sra.vjti.info/) Eklavya 2022  
* [MazeBlaze-v1](https://github.com/ChinmayLonkar/MAZEBLAZE.git) Github repository of MazeBlaze-v1
 
## License
[MIT License](https://opensource.org/licenses/MIT)



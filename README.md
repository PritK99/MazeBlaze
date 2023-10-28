# MazeBlaze

## Table of Contents

- [Project](#MazeBlaze-v2)
  - [Table of Contents](#table-of-contents)
  - [About The Project](#about-the-project)
  - [Demo](#demo)
  - [File Structure](#file-structure)
  - [Getting started](#Getting-Started)
  - [Future Work](#future-work)
  - [Contributors](#contributors)
  - [Acknowledgements and Resources](#acknowledgements-and-references)
  - [License](#license)
  
## About

In this project, we will create a Line-following, Maze-solving bot which can find the shortest path in the given maze.

  ### Domains Explored
Artifical Intelligence, Control Systems, Embedded C, Graph algorithms, Electronics, Basics of PCB design

  ### Project Workflow 
- Learn to design PCB and understand the design of PCB used in MazeBlaze bot
- Understand and improvise electronic components used in MazeBlaze bot
- Testing Line-sensor-array, motor drivers and interfacing encoders.
- Implementing left follow rule 
- Implementing path planing

## Demo

![ezgif com-gif-maker](https://user-images.githubusercontent.com/103832825/208280190-4e15a275-d6ff-4abc-b3d7-ef7825c5c433.gif)


## File Structure
```
👨‍💻MazeBlaze-v2.1
 ┣ 📂assets                            // Contains all the reference gifs, images
 ┣ 📂documentation
 ┣ 📂firmware                          // Code files 
   ┣ 📂1_lsa                           // Contains code for getting readings from LSA 
   ┃ ┣ 📂main                         
   ┃ ┃ ┗ 📄main.c 
   ┃ ┃ ┣ 📄CMakeList.txt
   ┃ ┣ 📄CMakeList.txt
   ┣ 📂3_encoders                      // Contains code for interfacing encoders 
   ┣ 📂4_line_following                // Contains code for line following
   ┣ 📂5_left_follow                   // Contains code for left-follow-rule and path planning
   ┣ 📂6_boot 
   ┣ 📂Components                       // Contains all the header and source files used in project
     ┣ 📂include                       
     ┣ 📂src 
     ┣ 📂esp-wifi-logger
     ┣ 📄CMakeList.txt
     
``` 
## Getting Started

### Prerequisites
To download and use this code, the minimum requirements are:

* [ESP_IDF](https://github.com/espressif/esp-idf)
* Windows 7 or later (64-bit), Ubuntu 20.04 or later
* [Microsoft VS Code](https://code.visualstudio.com/download) or any other IDE 

### Installation

Clone the project by typing the following command in your Terminal/CommandPrompt

```
git clone https://github.com/PritK99/MazeBlaze-v2.1.git 
```
Navigate to the MazeBlaze-v2.1 folder

```
cd MazeBlaze-v2.1
```

### Usage

Once the requirements are satisfied, you can easily download the project and use it on your machine.
After following the above steps , use the following commands to:

To activate the IDF

```
get_idf
```

To build the code 

```
idf.py build
```

To flash the code

```
idf.py -p (PORT) flash monitor
```
## Future Work

- [ ] Implementing better algorithms for Maze solving such as DFS, BFS, Djikstra's algorithm etc. on the bot
- [ ] Improve on the design and model of MazeBlaze-v2.1
- [ ] Achive higher speeds by increasing the efficiency of bot

## Contributors

* [Raj Gupta](https://github.com/RajGupta17)
* [Prit Kanadiya](https://github.com/PritK99)

## Acknowledgements and References
* [SRA VJTI](https://sravjti.in/) Eklavya 2022  
* Special thanks to our mentors [Chinmay Lonkar](https://github.com/ChinmayLonkar), [Marck Koothoor](https://github.com/marck3131), [Mateen Shah](https://github.com/Jamm02) and all the seniors at SRA, VJTI for their constant support and guidance throughout the project.
* [MazeBlaze-v1](https://github.com/ChinmayLonkar/MAZEBLAZE.git) Github repository of MazeBlaze-v1
* [Encoders](https://github.com/DavidAntliff/esp32-rotary-encoder-example.git) reference to interface interrupt service routine in Esp-32.
 
## License
[MIT License](https://opensource.org/licenses/MIT)



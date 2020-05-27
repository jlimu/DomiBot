<img src="https://i.pinimg.com/originals/9d/4f/c7/9d4fc74f015e5db77b2a05e6ce915d9b.jpg" align="right" width="300" alt="header pic"/>

# DomiBot

Robot project for the RLP subject in UAB Computer Engineering


# Table of Contents
   * [What is this?](#what-is-this)
   * [Software architecture](#software-architecture)
   * [IMPORTANT](#important)
   * [Requirements](#requirements)
   * [Instructions](#hinstructions)
   * [License](#license)
   * [Citing](#citing)
   * [Authors](#authors)

# What is this?

This is a robot project for the RLP Project in UAB Computer Engineering.

It consists of a robotic arm that plays domino against a human.

# Software architecture

We identified 4 software modules for our robot. Our software architecture is based on a photograph taken by a camera that tells us the status of the domino board and the tiles that the robot has to play with.

Computer vision

Based on the photograph of the domino board made by the camera, this module is responsible for analyzing the board using computer vision.
The aim of this analysis is to identify all the tiles that make up the board and those that the robot has to play, and find those positions where the robot can make possible plays along with the numbers it needs to make those plays. These positions will be next to the tiles at the ends of the board and those tiles that have the same number on both sides, so the latter will have two possible positions for the same play.

Decision played

This module receives, from the computer vision module, the information of the cards of the board and the robot and the positions of the board where the possible plays along with their number can be made. With this information, this module must implement an algorithm that returns the best possible play.
The robot will value more those plays that have been assigned a number that is very repeated in their tiles, and will take it even more into account if the number on the other side appears very frequently on the board and in its tiles, which will mean that the user is less likely to have it.
If the decision of the analysis is to place a tile, we will go directly to the mapping, instead we will have to steal a tile.

Steal tab

In addition to possible plays, the robot will analyze if it has no plays available. If this is the case, we will call the module "steal a file". Consequently he will steal a token and will have to photograph the board again and its consequent computer vision to, again, analyze whether we have a play or not.
To steal a token we will also have to resort to the movement of the arm, which will move by mapping and choose a random token which it will place in its play area.

Mapping

This module consists of converting the positions of the photograph of the board of the possible plays, that we obtain in the module of vision by computer, in the real positions of the physical board that we will need later to indicate the point where the robotic arm must move .
Knowing the play we will make, the mapping converts the parameters that have been passed to us, which we need to move the arm and the positions of the appropriate servos to put the tile in the indicated place.

Servo position control

This module receives the play selected by the robot in the module "decision played" and the position of the physical board to perform the play, obtained from the module "mapping". The aim of this module is to perform a reverse kinematics so that, from the position of the physical board of the play selected by the robot, obtain the necessary angles to rotate the servos and that the arm reaches the desired position of the board. You will also need to rotate the piece according to how you place it on the board.

# IMPORTANT

In this project, we had the goal of making it physical but due to COVID-19, we only do this virtual. This project contains: simulation of the robot, visual computing and a telnet interface to play against the robot.

# Requirements

- Ubuntu 16.04

- ROS Kinetic

- Gazebo 7

All of this were used in a website to code in ROS online without even installing any of these:

https://rds.theconstructsim.com/

# Instructions

por hacer

# License 

UAB

# Citing

If you use this project's code for your academic work, we encourage you to cite [our project](https://github.com/jlimu/DomiBot) 

If you use this project's code in industry, we'd love to hear from you as well; feel free to reach out to the developers directly.

# Authors

- [Sergi Pérez](https://github.com/Sergipemu/)

- [Eloy Bardají](https://github.com/eloybp/)

- [Albert Company](https://github.com/albertcom23/)

- [Limuel Pinto](https://github.com/jlimu/)

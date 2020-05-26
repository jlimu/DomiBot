<img src="https://www.google.com/search?q=robot+drawing&source=lnms&tbm=isch&sa=X&ved=2ahUKEwiMmqDRn9LpAhUL1xoKHU5pC0UQ_AUoAXoECA0QAw&biw=1366&bih=657#imgrc=gniXH2rbyPC2TM" align="right" width="300" alt="header pic"/>

# DomiBot

Robot project for the RLP subject in UAB Computer Engineering


# Table of Contents
   * [What is this?](#what-is-this)
   * [Requirements](#requirements)
   * [Documentation](#documentation)
   * [How to use](#how-to-use)
   * [Localization](#localization)
      * [Extended Kalman Filter localization](#extended-kalman-filter-localization)
      * [Particle filter localization](#particle-filter-localization)
      * [Histogram filter localization](#histogram-filter-localization)
   * [Mapping](#mapping)
      * [Gaussian grid map](#gaussian-grid-map)
      * [Ray casting grid map](#ray-casting-grid-map)
      * [Lidar to grid map](#lidar-to-grid-map)
      * [k-means object clustering](#k-means-object-clustering)
      * [Rectangle fitting](#rectangle-fitting)
   * [SLAM](#slam)
      * [Iterative Closest Point (ICP) Matching](#iterative-closest-point-icp-matching)
      * [FastSLAM 1.0](#fastslam-10)
   * [Path Planning](#path-planning)
      * [Dynamic Window Approach](#dynamic-window-approach)
      * [Grid based search](#grid-based-search)
         * [Dijkstra algorithm](#dijkstra-algorithm)
         * [A* algorithm](#a-algorithm)
         * [Potential Field algorithm](#potential-field-algorithm)
         * [Grid based coverage path planning](#grid-based-coverage-path-planning)
      * [State Lattice Planning](#state-lattice-planning)
         * [Biased polar sampling](#biased-polar-sampling)
         * [Lane sampling](#lane-sampling)
      * [Probabilistic Road-Map (PRM) planning](#probabilistic-road-map-prm-planning)
      * [Rapidly-Exploring Random Trees (RRT)](#rapidly-exploring-random-trees-rrt)
         * [RRT*](#rrt)
         * [RRT* with reeds-shepp path](#rrt-with-reeds-shepp-path)
         * [LQR-RRT*](#lqr-rrt)
      * [Quintic polynomials planning](#quintic-polynomials-planning)
      * [Reeds Shepp planning](#reeds-shepp-planning)
      * [LQR based path planning](#lqr-based-path-planning)
      * [Optimal Trajectory in a Frenet Frame](#optimal-trajectory-in-a-frenet-frame)
   * [Path Tracking](#path-tracking)
      * [move to a pose control](#move-to-a-pose-control)
      * [Stanley control](#stanley-control)
      * [Rear wheel feedback control](#rear-wheel-feedback-control)
      * [Linear–quadratic regulator (LQR) speed and steering control](#linearquadratic-regulator-lqr-speed-and-steering-control)
      * [Model predictive speed and steering control](#model-predictive-speed-and-steering-control)
      * [Nonlinear Model predictive control with C-GMRES](#nonlinear-model-predictive-control-with-c-gmres)
   * [Arm Navigation](#arm-navigation)
      * [N joint arm to point control](#n-joint-arm-to-point-control)
      * [Arm navigation with obstacle avoidance](#arm-navigation-with-obstacle-avoidance)
   * [Aerial Navigation](#aerial-navigation)
      * [drone 3d trajectory following](#drone-3d-trajectory-following)
      * [rocket powered landing](#rocket-powered-landing)
   * [Bipedal](#bipedal)
      * [bipedal planner with inverted pendulum](#bipedal-planner-with-inverted-pendulum)
   * [License](#license)
   * [Use-case](#use-case)
   * [Contribution](#contribution)
   * [Citing](#citing)
   * [Support](#support)
   * [Authors](#authors)

# What is this?

This is a robot project for the RLP Project in UAB Computer Engineering.

It consists of a robotic arm that plays domino against a human.

# Requirements

- Ubuntu 16.04

- ROS Kinetic

- Gazebo 7

All of this were used in a website to code in ROS online without even installing any of these:

https://rds.theconstructsim.com/

# Documentation

This README only shows some examples of this project. 

If you are interested in other examples or mathematical backgrounds of each algorithm, 

You can check the full documentation online: [https://pythonrobotics.readthedocs.io/](https://pythonrobotics.readthedocs.io/)

All animation gifs are stored here: [AtsushiSakai/PythonRoboticsGifs: Animation gifs of PythonRobotics](https://github.com/AtsushiSakai/PythonRoboticsGifs)

# How to use

1. Go to https://rds.theconstructsim.com/ and login to an account. If you don't have an account then create one.

2. Open the project.

3. Launch the simulation with the x file.

4. Open a shell and type the commands x for the simulation.

# License 

UAB

# Use-case

If this project helps your robotics project, please let me know with creating an issue.

Your robot's video, which is using PythonRobotics, is very welcome!!

This is a list of other user's comment and references:[users\_comments](https://github.com/AtsushiSakai/PythonRobotics/blob/master/users_comments.md)

# Citing

If you use this project's code for your academic work, we encourage you to cite [our papers](https://arxiv.org/abs/1808.10703) 

If you use this project's code in industry, we'd love to hear from you as well; feel free to reach out to the developers directly.

# Authors

- [Sergi Pérez](https://github.com/Sergipemu/)

- [Eloy Bardaji](https://github.com/eloybp/)

- [Albert Company](https://github.com/albertcom23/)

- [Limuel Pinto](https://github.com/jlimu/)

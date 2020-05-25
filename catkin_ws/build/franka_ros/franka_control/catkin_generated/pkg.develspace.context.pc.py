# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "/home/user/catkin_ws/devel/include;/home/user/catkin_ws/src/franka_ros/franka_control/include;/opt/ros/kinetic/include/libfranka".split(';') if "/home/user/catkin_ws/devel/include;/home/user/catkin_ws/src/franka_ros/franka_control/include;/opt/ros/kinetic/include/libfranka" != "" else []
PROJECT_CATKIN_DEPENDS = "actionlib;controller_interface;franka_hw;franka_msgs;geometry_msgs;message_runtime;pluginlib;realtime_tools;roscpp;sensor_msgs;tf2_msgs".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lfranka_state_controller;-lfranka_control_services;/opt/ros/kinetic/lib/libfranka.so.0.7.1".split(';') if "-lfranka_state_controller;-lfranka_control_services;/opt/ros/kinetic/lib/libfranka.so.0.7.1" != "" else []
PROJECT_NAME = "franka_control"
PROJECT_SPACE_DIR = "/home/user/catkin_ws/devel"
PROJECT_VERSION = "0.7.0"

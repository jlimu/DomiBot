cd datasets_ws/joint_states/
rosbag record /joint_states
cd notebook_ws/
ls
tar xvfp notebook_ws_Live_Class_74.tar 
cd simulation_ws/src/
git clone https://github.com/erdalpekel/panda_simulation.git
cd ..
cd catkin_ws/src/
git clone https://github.com/erdalpekel/panda_moveit_config.git
git clone --branch simulation https://github.com/erdalpekel/frank
git clone --branch simulation https://github.com/erdalpekel/franka_ros.git
cd ..
catkin_make
source devel/setup.bash
cd ..
cd simulation_ws/
catkin_make
source devel/setup.bash
rviz

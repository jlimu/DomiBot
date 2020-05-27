#! /usr/bin/env python
import sys
import rospy
import moveit_commander
import geometry_msgs.msg



moveit_commander.roscpp_initialize(sys.argv)
rospy.init_node('move_group_python_interface_tutorial', anonymous = True)
robot = moveit_commander.RobotCommander()
scene = moveit_commander.PlanningSceneInterface()

group_name = "panda_arm"
arm_group = moveit_commander.MoveGroupCommander(group_name)
#arm_group = moveit_commander.MoveGroupCommander([panda_arm])

#-------------------------------------------------------------
## Things to better debug the code
# We can get the name of the reference frame for this robot:
planning_frame = arm_group.get_planning_frame()
print "============ Reference frame: %s" % planning_frame

# We can also print the name of the end-effector link for this group:
eef_link = arm_group.get_end_effector_link()
print "============ End effector: %s" % eef_link

# We can get a list of all the groups in the robot:
group_names = robot.get_group_names()
print "============ Robot Groups:", robot.get_group_names()

# Sometimes for debugging it is useful to print the entire state of the
# robot:
print "============ Printing robot state"
print robot.get_current_state()
print ""

#Get the names of the named robot states available as targets, 
#both either remembered states or default states from srdf.
print "============ Printing arm_group names targets"
print arm_group.get_named_targets()
print ""
#-----------------------------------------------------------------------

hand_group = moveit_commander.MoveGroupCommander("hand")

# Put the arm in the start position
#target_name='start'
arm_group.set_named_target('ready')
plan1 = arm_group.go()

rospy.sleep(2)

arm_group.set_named_target('f1c')
plan1 = arm_group.go()

hand_group.set_named_target("open")
plan2 = hand_group.go()

rospy.sleep(2)

print "close?"
hand_group.set_named_target("close")
plan2 = hand_group.go()

rospy.sleep(2)

arm_group.set_named_target('f1d')
plan1 = arm_group.go()

rospy.sleep(2)

hand_group.set_named_target("open")
plan2 = hand_group.go()

rospy.sleep(2)



arm_group.set_named_target('f2c')
plan1 = arm_group.go()

hand_group.set_named_target("open")
plan2 = hand_group.go()

rospy.sleep(2)

hand_group.set_named_target("close")
plan2 = hand_group.go()

rospy.sleep(2)

arm_group.set_named_target('f2d')
plan1 = arm_group.go()

rospy.sleep(2)

hand_group.set_named_target("open")
plan2 = hand_group.go()

rospy.sleep(2)

print "============ Printing hand_group names targets"
print hand_group.get_named_targets()
print ""
#-----------------------------------------------------------------------

moveit_commander.roscpp_shutdown()

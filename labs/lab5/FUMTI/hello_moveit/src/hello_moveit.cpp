#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <vector>


using namespace std;

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");
  
  // Next step goes here
  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "robot");

  // Set a target Pose


  for (int i = 0; i < 1; i++) {
    auto target_pose = []{
      geometry_msgs::msg::Pose msg;
      msg.orientation.w = 0.0;
      msg.position.x = -0.2;
      msg.position.y = 0.1;
      msg.position.z = 1.2;
      return msg;
    }();

    auto ahh = move_group_interface.getCurrentPose().pose;

    std::cout << ahh.position.x;

    // if (i == 0) {
    //   target_pose.position.x=0.4;
    // } else if (i==1) {
    //   target_pose.position.x = -0.4;
    // } else if (i==2) {
    //   target_pose.position.z = 0.4;
    //   target_pose.position.x = -0.4;
    // } else {
    //   target_pose.position.z = 0.4;
    //   target_pose.position.x = 0.4;
    // }

    move_group_interface.setPoseTarget(target_pose);

    // Create a plan to that target pose
    auto const [success, plan] = [&move_group_interface]{
      moveit::planning_interface::MoveGroupInterface::Plan msg;
      auto const ok = static_cast<bool>(move_group_interface.plan(msg));
      return std::make_pair(ok, msg);
    }();

    // Execute the plan
    if(success) {
      move_group_interface.execute(plan);
    } else {
      RCLCPP_ERROR(logger, "Planing failed!");
    }
  }
  
  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
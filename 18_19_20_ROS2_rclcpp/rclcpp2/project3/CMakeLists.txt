cmake_minimum_required(VERSION 3.5)
project(rclcpp-2-project3)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

#build
add_executable(rclcpp-2-project3 src/rclcpp-2-project3.cpp)
ament_target_dependencies(rclcpp-2-project3 rclcpp geometry_msgs)

# Install
install(TARGETS rclcpp-2-project3 DESTINATION lib/${PROJECT_NAME})

ament_package()
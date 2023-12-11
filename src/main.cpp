#include <rclcpp/executors.hpp>
#include <rclcpp/rclcpp.hpp>
#include <urg_node2/urg_node2.hpp>

auto main(int argc, char **argv) -> int {


    rclcpp::init(argc, argv);
    auto node = std::make_shared<urg_node2::UrgNode2>();
    auto async_operation = node->StartAsync();
    rclcpp::spin_until_future_complete(node, async_operation);
    return 0;
}

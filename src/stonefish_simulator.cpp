/*
    This file is a part of stonefish_ros2.

    stonefish_ros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    stonefish_ros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  stonefish_simulator.cpp
//  stonefish_ros2
//
//  Created by Patryk Cieslak on 02/10/23.
//  Copyright (c) 2023-2024 Patryk Cieslak. All rights reserved.
//

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"  // For the Empty service type
#include <Stonefish/utils/SystemUtil.hpp>
#include "stonefish_ros2/ROS2SimulationManager.h"
#include "stonefish_ros2/ROS2GraphicalSimulationApp.h"
#include <fstream>
#include <cstdlib>  // For std::rand() and std::srand()
#include <ctime>    // For std::time()

using namespace std::chrono_literals;



btMatrixXd generateRandomMatrix(int rows, int cols) {
    btMatrixXd matrix(rows, cols);
    
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Fill the matrix with random values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix.setElem(i, j, static_cast<double>(std::rand()) / RAND_MAX);  // Random value between 0 and 1
        }
    }
    
    return matrix;
}

class StonefishNode : public rclcpp::Node
{
public:
    StonefishNode(const std::string& scenarioPath,
                  const std::string& dataPath, 
                  const sf::RenderSettings& s, 
                  const sf::HelperSettings& h,
                  sf::Scalar rate) 
        : Node("stonefish_simulator")
    {   
        sf::ROS2SimulationManager* manager = new sf::ROS2SimulationManager(rate, scenarioPath, std::shared_ptr<rclcpp::Node>(this));
        app_ = std::shared_ptr<sf::ROS2GraphicalSimulationApp>(new sf::ROS2GraphicalSimulationApp("Stonefish Simulator", 
                                                                                             dataPath, s, h, "100", manager));
        app_->Startup();
        tickTimer_ = this->create_wall_timer(10ms, std::bind(&sf::ROS2GraphicalSimulationApp::Tick, app_));
        
        // Initialize the service
        service_ = this->create_service<std_srvs::srv::Empty>(
            "generate_images", 
            std::bind(&StonefishNode::generateImages, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
        );
    }
    
    std::shared_ptr<sf::ROS2GraphicalSimulationApp> getApp() const {
        return app_;
    }

private:
    void generateImages(const std::shared_ptr<rmw_request_id_t> request_header, 
                        const std::shared_ptr<std_srvs::srv::Empty::Request> request,
                        const std::shared_ptr<std_srvs::srv::Empty::Response> response)
    {
        // Prepare the transformation matrix
        btMatrixXd transformation = generateRandomMatrix(4, 4);
        std::cout << "GENERATING IMAGE " << std::endl;
        // Generate the FLS image from the pose using the modified transformation
        getApp()->generateFLSImageFromPose(transformation);    
}


    std::shared_ptr<sf::ROS2GraphicalSimulationApp> app_;
    rclcpp::TimerBase::SharedPtr tickTimer_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv, rclcpp::InitOptions(), rclcpp::SignalHandlerOptions::None);
    
    // Check number of command line arguments
    if (argc < 7)
    {
        std::cout << "Not enough command-line arguments provided!" << std::endl;
        exit(-1);
    }

    // Parse the arguments
    std::string dataPath = std::string(argv[1]) + "/";
    std::string scenarioPath(argv[2]);
    sf::Scalar rate = atof(argv[3]);

    sf::RenderSettings s;
    s.windowW = atoi(argv[4]);
    s.windowH = atoi(argv[5]);

    std::string quality(argv[6]);
    if (quality == "low")
    {
        s.shadows = sf::RenderQuality::LOW;
        s.ao = sf::RenderQuality::DISABLED;
        s.atmosphere = sf::RenderQuality::LOW;
        s.ocean = sf::RenderQuality::LOW;
        s.aa = sf::RenderQuality::LOW;
        s.ssr = sf::RenderQuality::DISABLED;
    }
    else if (quality == "high")
    {
        s.shadows = sf::RenderQuality::HIGH;
        s.ao = sf::RenderQuality::HIGH;
        s.atmosphere = sf::RenderQuality::HIGH;
        s.ocean = sf::RenderQuality::HIGH;
        s.aa = sf::RenderQuality::HIGH;
        s.ssr = sf::RenderQuality::HIGH;
    }
    else // "medium"
    {
        s.shadows = sf::RenderQuality::MEDIUM;
        s.ao = sf::RenderQuality::MEDIUM;
        s.atmosphere = sf::RenderQuality::MEDIUM;
        s.ocean = sf::RenderQuality::MEDIUM;
        s.aa = sf::RenderQuality::MEDIUM;
        s.ssr = sf::RenderQuality::MEDIUM;
    }

    // Initialize GUI settings
    sf::HelperSettings h;
    h.showFluidDynamics = false;
    h.showCoordSys = false;
    h.showBulletDebugInfo = false;
    h.showSensors = false;
    h.showActuators = false;
    h.showForces = false;

    // Start simulation
    std::shared_ptr<StonefishNode> node(new StonefishNode(scenarioPath, dataPath, s, h, rate));
    rclcpp::spin(node);
    
    return 0;
}


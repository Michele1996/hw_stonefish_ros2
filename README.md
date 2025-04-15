### stonefish_ros package for ROS2

This package delivers a ROS2 interface for the _Stonefish_ library. It also includes a standard simulator node, which loads the simulation scenario from a scenario description file (XML). The included parser extends the standard functionality of the _Stonefish_ library to enable search for files, resolution of parameters as well as a complete message interface. 

### Installation

1. Install pybind11 via: 
```bash
apt install python3-pybind11
```
2. Clone the *stonefish_ros2* package to your workspace.
3. Compile the workspace.

### Launching

To run the standard simulator node you have to include the 'stonefish_simulator.launch.py' file in your own launch file, overriding the default arguments.
Please refer to the documentation for details.

## ArduPilot-Stonefish-Ros2
Please refer to https://github.com/bvibhav/stonefish_bluerov2

### Credits
If you find this software useful in your research, please cite:

*Patryk Cieślak, "Stonefish: An Advanced Open-Source Simulation Tool Designed for Marine Robotics, With a ROS Interface", In Proceedings of MTS/IEEE OCEANS 2019, June 2019, Marseille, France* 

*Michele Grimaldi, "Stonefish: Supporting Machine Learning Research in Marine Robotics", In Proceedings of ICRA 2025, May 2025, USA*
```
@inproceedings{stonefish,
   author = {Cie{\'s}lak, Patryk},
   booktitle = {OCEANS 2019 - Marseille},
   title = {{Stonefish: An Advanced Open-Source Simulation Tool Designed for Marine Robotics, With a ROS Interface}},
   month = jun,
   year = {2019},
   doi={10.1109/OCEANSE.2019.8867434}}

@misc{grimaldi2025stonefishsupportingmachinelearning,
      title={Stonefish: Supporting Machine Learning Research in Marine Robotics}, 
      author={Michele Grimaldi and Patryk Cieslak and Eduardo Ochoa and Vibhav Bharti and Hayat Rajani and Ignacio Carlucho and Maria Koskinopoulou and Yvan R. Petillot and Nuno Gracias},
      year={2025},
      eprint={2502.11887},
      archivePrefix={arXiv},
      primaryClass={cs.RO},
      url={https://arxiv.org/abs/2502.11887}, 
}
```


### License
This is free software, published under the General Public License v3.0.

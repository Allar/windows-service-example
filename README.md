# windows-service-example

Example on how to use the windows-service CMake library https://github.com/Allar/windows-service

# VS Code

This repo has example launch configurations for the various things you can do with a service, i.e. Install, Uninstall, Attach, Stop, Start, Delete, and Create.

It also has a launch configuration called Run that runs the example with the command-line arg "run", which causes the example app to run its business logic without being ran as a service. This allows you to just hit F5 in VS Code and have it run your code not as a service but still maintain Windows Service functionality.
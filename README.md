# Container clone

Overview
--------
The program creates a container in Linux machine and runs the given program inside the created container.
The program does not validates arguments.


Usage
-----
* Create executable:
    `make`

* Server Usage:
    `./container <new_hostname> <new_filesystem_directory> <num_processes> <path_to_program_to_run_within_container> <args_for_arguments>`
    
new_hostname - any given name.
new_filesystem_directory is an image of a linux file system, an example of one can be found under "fs".
num_rocesses is the maximum mumber of processes to run in the container.
path_to_program_to_run_within_container is an executable file to run inside the container
args_for_arguments - zero or morre arguments to send run with the executable file.

Credits
-------
This project was made as part of my Computer Science studies in the Hebrew University.
It was given as an exercise in course 67808 ( Operating Systems ) which I took in 2022.

# Build and run instructions
Open a terminal here
$ cd Source
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./PageRank 100nodes_graph.txt web-stanford.txt 0.85 0.0000001 10

Launch the PageRank program with the given launch command (or modify the parameters if you wish), it will run both main.c and main_omp.c in order, with output in the console.

# Program arguments
argv[0] = Executable name [default]
argv[1] = Small graph name
argv[2] = Large graph name
argv[3] = Damping
argv[4] = Convergence threshold
argv[5] = Number of top webpages to show

# Example launch commands
./PageRank 100nodes_graph.txt web-stanford.txt 0.85 0.0000001 10
## Testing command
./PageRank simple-webgraph.txt simple-webgraph.txt 1 0.0000001 10

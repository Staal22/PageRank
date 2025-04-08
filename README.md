Reads a graph from file and sorts it using a simple implementation of the PageRank algorithm. Includes a serial C implementation and a parallelized OpenMP version.
# Build and run instructions
```
cd Source
mkdir build
cd build
cmake ..
cmake --build .
./PageRank 100nodes_graph.txt web-stanford.txt 0.85 0.0000001 10
```

Modify the parameters if you wish. The program will run both main.c and main_omp.c in order, with output in the console.

# Program arguments
```
argv[0] = Executable name [default]  
argv[1] = Small graph name  
argv[2] = Large graph name  
argv[3] = Damping  
argv[4] = Convergence threshold  
argv[5] = Top n pages to print
```

# Example launch commands
`./PageRank 100nodes_graph.txt web-stanford.txt 0.85 0.0000001 10`  
`./PageRank simple-webgraph.txt simple-webgraph.txt 1 0.0000001 10`

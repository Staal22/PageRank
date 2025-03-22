### Instructions
Launch the PageRank program with the given launch command (or modify the parameters if you wish), it will run both main.c and main_omp.c in order, with detailed output in the console showing when what is happening, according to the task specifications.

### Program arguments
argv[0] = Executable name [default]
argv[1] = Small graph name
argv[2] = Larg graph name
argv[3] = Damping
argv[4] = Epsilon/Convergence threshold
argv[5] = Number of top webpages to show

### Example launch command
./PageRank 100nodes_graph.txt web-stanford.txt 0.85 0.0000001 10
## Compile
    use make to compile
    "linux & gcc 4.7.2" preferred

## Execute
    Example:
        ./imm_discrete -dataset nethept/ -k 50 -model IC -epsilon 0.1
        ./imm_continuous -dataset nethept/ -k 50 -model CONT -epsilon 0.1 -T 0.01

    Arguments:
        -model:
            the diffusion model uses
            IC or LT or CONT

        -dataset:
            path to the dataset directory

        -epsilon:
            a double value for epsilon

        -k:
            number of selected nodes
		-T:
        	time threshold, only in continous model


## Dataset Format
    Three files are necessary inside path_to_dataset folder

## Format for attribute
    Location:
        path_to_dataset/attribute.txt
    Format:
        This file should have exactly two lines
        n=number of nodes
        m=number of edges

### Format for IC model
    Location:
        path_to_dataset/graph_ic.inf
    Format:
        Each line has three numbers
        node1 node2 propogation_probability_from_node1_to_node2
    Comments:
        It is always a directed graph
        node number should range in [0 to n-1] (inclusive)

### Format for LT model
    Location:
        path_to_dataset/graph_lt.inf
    Format is same as IC model

### Format for CONT model
    Location:
        path_to_dataset/graph_lt.inf
    Format:
        Each line has Four numbers
        node1 node2 param1 param2
	For the details of two parameters, refer to http://www.cc.gatech.edu/~ndu8/DuSonZhaMan-NIPS-2013.html





Compile:
    using make to compile
    "gcc 4.7.2" preferred

Execute:
    Example:
        ./tim -model LT -dataset nethept/ -epsilon 0.1 -k 50

    Arguments:
        -model:
            the diffusion model uses
            IC or LT

        -dataset:
            path to the dataset directory

        -epsilon:
            a double value for epsilon

        -k:
            number of selected nodes



Dataset Format:
    Three files are necessary inside path_to_dataset folder

Format for attribute:
    Location:
        path_to_dataset/attribute.txt
    Format:
        This file should have exactly two lines
        n=number of nodes
        m=number of edges

Format for ic model:
    Location:
        path_to_dataset/graph_ic.inf
    Format:
        Each line has three numbers
        node1 node2 propogation_probability_from_node1_to_node2
    Comments:
        It is always a directed graph
        node number should range in [0 to n-1] (inclusive)

Format for lt model
    Location:
        path_to_dataset/graph_lt.inf
    Format is same as IC model



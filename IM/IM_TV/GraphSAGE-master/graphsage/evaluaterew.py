import networkx as nx

# changes the graph
def evaluate(graph, selected_nodes):
	quality = 0
	for node in selected_nodes:
		nodes_covered = graph.neighbors(node)
		quality += len(nodes_covered)
		print(" node  ", node ," gain ", len(nodes_covered))

		for nd in nodes_covered:
			graph.remove_node(nd)
	return quality
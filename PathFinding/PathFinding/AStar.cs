using System.Collections.Generic;

namespace PathFinding
{
    class AStar
    {
        private List<Node> _open;
        private List<Node> _closed;

        /// <summary>
        /// Finds a path from [start] to [end] using the AStar algorithm.
        /// </summary>
        /// <param name="start">The node to start at.</param>
        /// <param name="end">The node to end at.</param>
        /// <param name="map">The map.</param>
        /// <returns>The path as a list of nodes.</returns>
        public List<Node> FindPath(Node start, Node end, Map map)
        {
            // Create lists
            var path = new List<Node>();
            _open = new List<Node>();
            _closed = new List<Node>();

            // Create start node
            var tempStart = start;
            tempStart.CameFrom = null;
            tempStart.GScore = 0;
            tempStart.FScore = tempStart.GScore + tempStart.DistanceTo(end);
            AddToOpenList(tempStart);

            // While open is not empty...
            while (_open.Count > 0)
            {
                var current = _open[0];

                // If we found the goal ...
                if (current.Equals(end))
                {
                    // ... Create the path.
                    path =  ReconstructPath(current);
                    break;
                }

                _open.Remove(current);
                AddToClosedList(current);

                // For all neighbour nodes
                foreach (var neighbour in map.NeighbourNodes(current))
                {
                    if (neighbour.Closed) continue;

                    // Calculate scores.
                    var tentativeGScore = current.GScore + neighbour.Cost;
                    var tentativeFScore = tentativeGScore + neighbour.DistanceTo(end);

                    // If the neighbour is in the closed list and the tentativeFScore is higher
                    // than the neighbour's, skip it.
                    if (_closed.Contains(neighbour)
                        && tentativeFScore >= _closed[_closed.IndexOf(neighbour)].FScore)
                    {
                        continue;   
                    }

                    // If the neighbour is not in the open list, add it.
                    if (!_open.Contains(neighbour)
                        && map.WithinMap(neighbour))
                    {
                        neighbour.CameFrom = current;
                        neighbour.GScore = tentativeGScore;
                        neighbour.FScore = tentativeFScore;

                        AddToOpenList(neighbour);
                        continue;
                    }

                    // If the neighbour is in the open list, modify it.
                    if (_open.Contains(neighbour)
                        && map.WithinMap(neighbour))
                    {
                        var indexOfNeighbour = _open.IndexOf(neighbour);

                        if (tentativeFScore < _open[indexOfNeighbour].FScore)
                        {
                            _open[indexOfNeighbour].CameFrom = current;
                            _open[indexOfNeighbour].GScore = tentativeGScore;
                            _open[indexOfNeighbour].FScore = tentativeFScore;
                        }
                    }
                }
            }

            return path;
        }

        /// <summary>
        /// Reconstructs the path that ends at node [node].
        /// </summary>
        /// <param name="node">The end node.</param>
        /// <returns>A list that holds the nodes for the entire path.</returns>
        private List<Node> ReconstructPath(Node node)
        {
            var path = new List<Node>();

            ReconstructPath(path, node);
            path.Reverse();

            return path;
        }

        /// <summary>
        /// Recursive method for constructing the path.
        /// </summary>
        /// <param name="nodes">The list of nodes for the path.</param>
        /// <param name="node">The node to add.</param>
        /// <returns>The list of nodes for the path.</returns>
        private List<Node> ReconstructPath(List<Node> nodes, Node node)
        {
            if (node == null) return nodes;

            nodes.Add(node);

            return ReconstructPath(nodes, node.CameFrom);
        }

        /// <summary>
        /// Adds a node to the open list, ensuring that the list is in ascending order.
        /// </summary>
        /// <param name="node">The node to add.</param>
        private void AddToOpenList(Node node)
        {
            for (var i = 0; i < _open.Count; i++)
            {
                if (node.FScore < _open[i].FScore)
                {
                    _open.Insert(i, node);
                    return;
                }
            }

            _open.Add(node);
        }

        /// <summary>
        /// Adds a node to the closed list, ensuring that the list is in ascending order.
        /// </summary>
        /// <param name="node">The node to add.</param>
        private void AddToClosedList(Node node)
        {
            for (var i = 0; i < _closed.Count; i++)
            {
                if (node.FScore < _closed[i].FScore)
                {
                    _closed.Insert(i, node);
                    return;
                }
            }

            _closed.Add(node);
        }
    }
}

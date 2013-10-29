using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Map
    {
        private Node[,] _map;

        /// <summary>
        /// Gets the height of the map.
        /// </summary>
        public int Height { get { return _map.GetLength(0); } }

        /// <summary>
        /// Gets the width of the map.
        /// </summary>
        public int Width { get { return _map.GetLength(1); } }

        /// <summary>
        /// Gets or sets the value of the coordinates given.
        /// </summary>
        /// <param name="x">The x-coordinate.</param>
        /// <param name="y">The y-coordinate.</param>
        /// <returns>???</returns>
        public Node this[int x, int y]
        {
            get { return _map[x, y]; }
            set { _map[x, y] = value; }
        }

        public Map(int height, int width)
        {
            _map = new Node[height, width];

            for (var i = 0; i < height; i++)
            {
                for (var k = 0; k < width; k++)
                {
                    _map[i, k] = new Node(i, k);
                }
            }
        }

        /// <summary>
        /// Checks if a given node is within the bounds of the map.
        /// </summary>
        /// <param name="node">The node.</param>
        /// <returns>True if the location is within the bounds; false if it is not.</returns>
        public bool WithinMap(Node node)
        {
            return WithinMap(node.X, node.Y);
        }

        /// <summary>
        /// Checks if a given location is within the bounds of the map.
        /// </summary>
        /// <param name="x">The x-coordinate.</param>
        /// <param name="y">The y-coordinate.</param>
        /// <returns>True if the location is within the bounds; false if it is not.</returns>
        public bool WithinMap(int x, int y)
        {
            if (x < 0 || x > Height - 1) return false;
            if (y < 0 || y > Width - 1) return false;

            return true;
        }

        /// <summary>
        /// Gets the nodes that are neighbours to the given node.
        /// </summary>
        /// <param name="node">The node to find the neighbours for.</param>
        /// <returns>A list containing the neighbours.</returns>
        public List<Node> NeighbourNodes(Node node)
        {
            var neighbors = new List<Node>();

            if (WithinMap(node.X - 1, node.Y)) neighbors.Add(_map[node.X - 1, node.Y]);
            if (WithinMap(node.X + 1, node.Y)) neighbors.Add(_map[node.X + 1, node.Y]);
            if (WithinMap(node.X, node.Y - 1)) neighbors.Add(_map[node.X, node.Y - 1]);
            if (WithinMap(node.X, node.Y + 1)) neighbors.Add(_map[node.X, node.Y + 1]);
            if (WithinMap(node.X - 1, node.Y - 1)) neighbors.Add(_map[node.X - 1, node.Y - 1]);
            if (WithinMap(node.X + 1, node.Y - 1)) neighbors.Add(_map[node.X + 1, node.Y - 1]);
            if (WithinMap(node.X - 1, node.Y + 1)) neighbors.Add(_map[node.X - 1, node.Y + 1]);
            if (WithinMap(node.X + 1, node.Y + 1)) neighbors.Add(_map[node.X + 1, node.Y + 1]);

            return neighbors;
        }

        public void Draw(Node agentA, Node agentB, List<Node> agentAPath, List<Node> agentBPath)
        {
            Node tempNode = new Node(0, 0);

            Console.Clear();

            for (int x = 0; x < Height; x++)
            {
                Console.Write("+");
                for (int y = 0; y < Width; y++)
                {
                    Console.Write("-+");
                }
                Console.WriteLine();

                Console.Write("|");
                for (int y = 0; y < Width; y++)
                {
                    tempNode.X = x;
                    tempNode.Y = y;

                    if (tempNode.Equals(agentA)) Console.Write("A");
                    else if (tempNode.Equals(agentB)) Console.Write("B");
                    else if (agentAPath.Contains(tempNode)) Console.Write("a");
                    else if (agentBPath.Contains(tempNode)) Console.Write("b");
                    else Console.Write(" ");
                    Console.Write("|");
                }
                Console.WriteLine();
            }

            Console.Write("+");
            for (int y = 0; y < Width; y++)
            {
                Console.Write("-+");
            }
            Console.WriteLine();
            Console.Write("Press <enter> to continue to next step.");
        }
    }
}

using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Map
    {
        /// <summary>
        /// The map.
        /// </summary>
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

        /// <summary>
        /// Initializes the map with the given height and width.
        /// </summary>
        /// <param name="height"></param>
        /// <param name="width"></param>
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
        /// Closes a position on the map.
        /// </summary>
        /// <param name="x">The x-coordinate of the position.</param>
        /// <param name="y">The y-coordinate of the position.</param>
        public void Close(int x, int y)
        {
            if (!WithinMap(x, y)) return;

            _map[x, y].Closed = true;
        }

        /// <summary>
        /// Opens a position on the map.
        /// </summary>
        /// <param name="x">The x-coordinate of the position.</param>
        /// <param name="y">The y-coordinate of the position.</param>
        public void Open(int x, int y)
        {
            if (!WithinMap(x, y)) return;

            _map[x, y].Closed = false;
        }

        /// <summary>
        /// Draws the map to the console.
        /// </summary>
        /// <param name="agentA">The location of agent A.</param>
        /// <param name="agentB">The location of agent B.</param>
        public void Draw(Agent agentA, Agent agentB)
        {
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
                    var tempNode = new Node(x, y);

                    if (_map[x, y].Closed) Console.Write("X");
                    else if (x == agentA.X && y == agentA.Y) Console.Write("A");
                    else if (x == agentB.X && y == agentB.Y) Console.Write("B");
                    else if (agentA.Path.Contains(tempNode)) Console.Write("a");
                    else if (agentB.Path.Contains(tempNode)) Console.Write("b");
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
    }
}

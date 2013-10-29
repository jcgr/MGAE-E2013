using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Node
    {
        public bool Closed { get; private set; }

        public int X { get; set; }
        public int Y { get; set; }

        /// <summary>
        /// Gets or sets the g score.
        /// </summary>
        public float GScore { get; set; }

        /// <summary>
        /// Gets or sets the f score.
        /// </summary>
        public float FScore { get; set; }

        /// <summary>
        /// Gets the cost of moving to the node.
        /// </summary>
        public float Cost { get; private set; }

        /// <summary>
        /// Gets or sets the vectorNode this obkect came from.
        /// </summary>
        public Node CameFrom { get; set; }

        private static readonly IEqualityComparer<Node> XYComparerInstance = new XYEqualityComparer();

        public Node(int x, int y)
        {
            X = x;
            Y = y;
            Cost = 1;
            Closed = false;
        }

        /// <summary>
        /// Calculates the distance to the given node.
        /// </summary>
        /// <param name="target">The node to calculate the distance to.</param>
        /// <returns>The distance.</returns>
        public float DistanceTo(Node target)
        {
            return (float)Math.Sqrt(Math.Pow(target.X - X, 2) + Math.Pow(target.Y - Y, 2));
        }

        /// <summary>
        /// Equality generated with Resharper.
        /// </summary>
        private sealed class XYEqualityComparer : IEqualityComparer<Node>
        {
            public bool Equals(Node x, Node y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.X == y.X && x.Y == y.Y;
            }

            public int GetHashCode(Node obj)
            {
                unchecked
                {
                    return (obj.X * 397) ^ obj.Y;
                }
            }
        }

        public static IEqualityComparer<Node> XYComparer
        {
            get { return XYComparerInstance; }
        }
    }
}

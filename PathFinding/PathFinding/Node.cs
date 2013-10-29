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

        protected bool Equals(Node other)
        {
            return X == other.X && Y == other.Y;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((Node) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (X * 397) ^ Y;
            }
        }
    }
}

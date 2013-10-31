using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Node
    {
        /// <summary>
        /// Gets or sets the x-coordinate of the node.
        /// </summary>
        public int X { get; private set; }
        /// <summary>
        /// Gets or sets the y-coordinate of the node.
        /// </summary>
        public int Y { get; private set; }

        /// <summary>
        /// Gets a value that indicates if the node is closed or not.
        /// </summary>
        public bool Closed { get; set; }

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

        /// <summary>
        /// Initializes the node.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
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
        /// Checks if another node is equal to this node.
        /// </summary>
        /// <param name="other">The node to compare with.</param>
        /// <returns>True if the X and Y values of both nodes are the same; False if they are not.</returns>
        protected bool Equals(Node other)
        {
            return X == other.X && Y == other.Y;
        }

        /// <summary>
        /// Checks if another object is equal to this node.
        /// </summary>
        /// <param name="obj">The object to compare with.</param>
        /// <returns>True if they are equal to each other; False if they are not.</returns>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((Node) obj);
        }

        /// <summary>
        /// Generates a hashcode for the node.
        /// </summary>
        /// <returns>The hashcode.</returns>
        public override int GetHashCode()
        {
            unchecked
            {
                return (X * 397) ^ Y;
            }
        }
    }
}

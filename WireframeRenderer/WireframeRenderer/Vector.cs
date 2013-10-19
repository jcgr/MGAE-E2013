using System;

namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents a 3D vector.
    /// </summary>
    class Vector
    {
        /// <summary>
        /// Creates a new instance of Vector.
        /// </summary>
        /// <param name="x">The x-coordinate of the vector.</param>
        /// <param name="y">The y-coordinate of the vector.</param>
        /// <param name="z">The z-coordinate of the vector.</param>
        public Vector(double x, double y, double z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// Gets or sets the X value.
        /// </summary>
        public double X { get; set; }

        /// <summary>
        /// Gets or sets the Y value.
        /// </summary>
        public double Y { get; set; }

        /// <summary>
        /// Gets or sets the Z value.
        /// </summary>
        public double Z { get; set; }

        /// <summary>
        /// Calculates the length of a 3D vector.
        /// </summary>
        /// <param name="vector">The vector to calculate the length of.</param>
        /// <returns>The length of the vector.</returns>
        public static double VectorLength(Vector vector)
        {
            return Math.Sqrt(Math.Pow(vector.X, 2)
                            + Math.Pow(vector.Y, 2)
                            + Math.Pow(vector.Z, 2));
        }

        /// <summary>
        /// Normalizes a 3D vector.
        /// </summary>
        /// <param name="vector">The vector to normalize.</param>
        /// <param name="length">The length to normalize by.</param>
        /// <returns>The normalized vector.</returns>
        public static Vector NormalizeVector(Vector vector, double length)
        {
            var normalizedX = vector.X / length;
            var normalizedY = vector.Y / length;
            var normalizedZ = vector.Z / length;

            return new Vector(normalizedX, normalizedY, normalizedZ);
        }

        /// <summary>
        /// Calculates the crossproduct of two vectors.
        /// </summary>
        /// <param name="vector1">The first vector.</param>
        /// <param name="vector2">The second vector.</param>
        /// <returns>The crossproduct.</returns>
        public static Vector CrossProduct(Vector vector1, Vector vector2)
        {
            var newX = (vector1.Y * vector2.Z) - (vector1.Z * vector2.Y);
            var newY = (vector1.Z * vector2.X) - (vector1.X * vector2.Z);
            var newZ = (vector1.X * vector2.Y) - (vector1.Y * vector2.X);

            return new Vector(newX, newY, newZ);
        }
    }
}

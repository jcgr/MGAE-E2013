using System;

namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents a 3D vector.
    /// </summary>
    class Vector
    {
        #region Properties
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
        #endregion

        #region Constructors
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
        #endregion

        #region Public Methods
        /// <summary>
        /// Calculates the length of the vector.
        /// </summary>
        /// <returns>The length of the vector.</returns>
        public double Length()
        {
            return Math.Sqrt(Math.Pow(X, 2) + Math.Pow(Y, 2) + Math.Pow(Z, 2));
        }

        /// <summary>
        /// Normalizes the vector using its own length.
        /// </summary>
        /// <returns>The normalized vector.</returns>
        public Vector Normalize()
        {
            return new Vector(X / Length(), Y / Length(), Z / Length());
        }
        #endregion

        #region Static Methods
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
        #endregion
    }
}

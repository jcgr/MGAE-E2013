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
    }
}

using System.Drawing;

namespace WireframeRenderer
{
    /// <summary>
    /// Represents a vertex in 3D space.
    /// Can also substitute as a vector (just leave the W coordinate as 0).
    /// </summary>
    class Vertex
    {
        /// <summary>
        /// Creates a new instance of Vertex.
        /// </summary>
        public Vertex()
        {
        }

        /// <summary>
        /// Creates a new instance of Vertex.
        /// For use when the dummy value (z) should be left as 0.
        /// </summary>
        /// <param name="x">The x-coordinate of the vertex.</param>
        /// <param name="y">The y-coordinate of the vertex.</param>
        /// <param name="z">The z-coordinate of the vertex.</param>
        public Vertex(double x, double y, double z)
            : this(x, y, z, 1)
        {
        }

        /// <summary>
        /// Creates a new instance of Vertex.
        /// </summary>
        /// <param name="x">The x-coordinate of the vertex.</param>
        /// <param name="y">The y-coordinate of the vertex.</param>
        /// <param name="z">The z-coordinate of the vertex.</param>
        /// <param name="w">The w-coordinate of the vertex.</param>
        public Vertex(double x, double y, double z, double w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
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
        /// Gets or sets the W (dummy) value.
        /// </summary>
        public double W { get; set; }

        /// <summary>
        /// Gets or sets the screen-coordinate of the vertex.
        /// </summary>
        public Point ScreenCoordinate { get; set; }
    }
}

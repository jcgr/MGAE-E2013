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

        /// <summary>
        /// Updates the vertex' screen point with the aid of the camera.
        /// </summary>
        /// <param name="camera">The camera.</param>
        public void UpdateScreenPoint(Camera camera)
        {
            // apply the view and perspective transforms
            var vertexViewSpace = MatrixToVertex(Matrix.NaiveMultiplication(camera.AllTransforms, VertexToMatrix(this)));

            // normalize by dividing through by the homogeneous coordinate w
            vertexViewSpace.X = vertexViewSpace.X / vertexViewSpace.W;
            vertexViewSpace.Y = vertexViewSpace.Y / vertexViewSpace.W;
            vertexViewSpace.Z = vertexViewSpace.Z / vertexViewSpace.W;

            // now map [-1, 1] into the screen coordinates (0, width) and (0, height)
            // where (0,0) is the top-left corner of the screen
            ScreenCoordinate = new Point
            {
                X = (int)(vertexViewSpace.X * (camera.Width / 2) + (camera.Width / 2)),
                Y = (int)(-vertexViewSpace.Y * (camera.Height / 2) + (camera.Height / 2))
            };
        }

        #region Static Methods

        /// <summary>
        /// Converts the first column of a matrix into a vertex.
        /// </summary>
        /// <param name="m">The matrix to convert.</param>
        /// <returns>The resulting vertex.</returns>
        public static Vertex MatrixToVertex(Matrix m)
        {
            return new Vertex(m[0, 0], m[1, 0], m[2, 0], m[3, 0]);
        }

        /// <summary>
        /// Converts a vertex into a 1-width matrix.
        /// </summary>
        /// <param name="v">The vertex to convert.</param>
        /// <returns>The resulting matrix.</returns>
        public static Matrix VertexToMatrix(Vertex v)
        {
            var result = new Matrix(4, 1);

            result[0, 0] = v.X;
            result[1, 0] = v.Y;
            result[2, 0] = v.Z;
            result[3, 0] = v.W;

            return result;
        }
        #endregion 
    }
}

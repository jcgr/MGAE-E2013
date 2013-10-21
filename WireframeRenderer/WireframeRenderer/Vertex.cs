using System.Drawing;

namespace WireframeRenderer
{
    /// <summary>
    /// Represents a vertex in 3D space.
    /// </summary>
    class Vertex
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

        /// <summary>
        /// Gets or sets the W (dummy) value.
        /// </summary>
        public double W { get; set; }

        /// <summary>
        /// Gets or sets the screen-coordinate of the vertex.
        /// </summary>
        public Point ScreenCoordinate { get; set; }
        #endregion

        #region Constructors
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
        #endregion

        #region Public Methods
        /// <summary>
        /// Updates the vertex' screen point with the aid of the camera.
        /// </summary>
        /// <param name="camera">The camera.</param>
        public void UpdateScreenPoint(Camera camera)
        {
            // Apply transformantions.
            var vertexViewSpace = MatrixToVertex(Matrix.NaiveMultiplication(camera.AllTransforms, VertexToMatrix(this)));

            // Normalize each coordinate.
            vertexViewSpace.X = vertexViewSpace.X / vertexViewSpace.W;
            vertexViewSpace.Y = vertexViewSpace.Y / vertexViewSpace.W;
            vertexViewSpace.Z = vertexViewSpace.Z / vertexViewSpace.W;

            // Map each coordinate to the right place on the screen.
            ScreenCoordinate = new Point
            {
                X = (int)(vertexViewSpace.X * (camera.Width / 2) + (camera.Width / 2)),
                Y = (int)(-vertexViewSpace.Y * (camera.Height / 2) + (camera.Height / 2))
            };
        }
        #endregion

        #region Static Methods
        /// <summary>
        /// Converts the first column of a matrix into a vertex.
        /// </summary>
        /// <param name="matrix">The matrix to convert.</param>
        /// <returns>A vertex version of the first column of the matrix.</returns>
        public static Vertex MatrixToVertex(Matrix matrix)
        {
            return new Vertex(matrix[0, 0], matrix[1, 0], matrix[2, 0], matrix[3, 0]);
        }

        /// <summary>
        /// Converts a vertex into a 4-hight, 1-width matrix.
        /// </summary>
        /// <param name="vertex">The vertex to convert</param>
        /// <returns>A Matrix[4, 1] version of the vertex.</returns>
        public static Matrix VertexToMatrix(Vertex vertex)
        {
            var matrix = new Matrix(4, 1);

            matrix[0, 0] = vertex.X;
            matrix[1, 0] = vertex.Y;
            matrix[2, 0] = vertex.Z;
            matrix[3, 0] = vertex.W;

            return matrix;
        }
        #endregion 
    }
}

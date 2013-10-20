namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents a triangle of vertices in 3D spaces.
    /// </summary>
    class Triangle
    {
        /// <summary>
        /// Creates a new instance of Triangle.
        /// </summary>
        public Triangle()
        {
        }

        /// <summary>
        /// Creates a new instance of Triangle.
        /// </summary>
        /// <param name="v1">The first vertex of the triangle.</param>
        /// <param name="v2">The second vertex of the triangle.</param>
        /// <param name="v3">The third vertex of the triangle.</param>
        public Triangle(Vertex v1, Vertex v2, Vertex v3)
        {
            A = v1;
            B = v2;
            C = v3;
        }

        /// <summary>
        /// Gets or sets the A vertex.
        /// </summary>
        public Vertex A { get; set; }

        /// <summary>
        /// Gets or sets the B vertex.
        /// </summary>
        public Vertex B { get; set; }

        /// <summary>
        /// Gets or sets the C vertex.
        /// </summary>
        public Vertex C { get; set; }

        /// <summary>
        /// Updates the screen points of the triangle's vertices.
        /// </summary>
        /// <param name="camera">The camera.</param>
        public void UpdateVerticesScreenPoint(Camera camera)
        {
            A.UpdateScreenPoint(camera);
            B.UpdateScreenPoint(camera);
            C.UpdateScreenPoint(camera);
        }
    }
}

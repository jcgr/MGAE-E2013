namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents a Matrix.
    /// Source: http://dev.bratched.com/en/fun-with-matrix-multiplication-and-unsafe-code/
    /// </summary>
    class Matrix
    {
        private readonly double[,] _matrix;

        /// <summary>
        /// Creates a new matrix with the given height and width.
        /// </summary>
        /// <param name="height">The height of the matrix.</param>
        /// <param name="width">The width of the matrix.</param>
        public Matrix(int height, int width)
        {
            _matrix = new double[height, width];
        }

        /// <summary>
        /// Gets the height of the matrix.
        /// </summary>
        public int Height { get { return _matrix.GetLength(0); } }
        /// <summary>
        /// Gets the width of the matrix.
        /// </summary>
        public int Width { get { return _matrix.GetLength(1); } }

        /// <summary>
        /// Gets or sets the value of the coordinates given.
        /// </summary>
        /// <param name="x">The x-coordinate.</param>
        /// <param name="y">The y-coordinate.</param>
        /// <returns>???</returns>
        public double this[int x, int y]
        {
            get { return _matrix[x, y]; }
            set { _matrix[x, y] = value; }
        }
    }
}

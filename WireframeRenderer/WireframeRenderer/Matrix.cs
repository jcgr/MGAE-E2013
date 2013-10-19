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

        /// <summary>
        /// Multiplies two matrices.
        /// Source: http://dev.bratched.com/en/fun-with-matrix-multiplication-and-unsafe-code/
        /// </summary>
        /// <param name="m1">The first matrix</param>
        /// <param name="m2">The second matrix</param>
        /// <returns>The resulting matrix of the multiplication</returns>
        public static Matrix NaiveMultiplication(Matrix m1, Matrix m2)
        {
            var resultMatrix = new Matrix(m1.Height, m2.Width);

            // Handles the multiplication.
            for (var i = 0; i < resultMatrix.Height; i++)
            {
                for (var j = 0; j < resultMatrix.Width; j++)
                {
                    resultMatrix[i, j] = 0;
                    for (var k = 0; k < m1.Width; k++)
                    {
                        resultMatrix[i, j] += m1[i, k] * m2[k, j];
                    }
                }
            }

            return resultMatrix;
        }
    }
}

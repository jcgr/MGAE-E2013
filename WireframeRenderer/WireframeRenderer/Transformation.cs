using System;

namespace WireframeRenderer
{
    /// <summary>
    /// A class for handling math related to matrices.
    /// </summary>
    class Transformation
    {
        /// <summary>
        /// Clones the given matrix.
        /// </summary>
        /// <param name="m">The matrix to clone.</param>
        /// <returns>A clone of the given matrix.</returns>
        public static Matrix Clone(Matrix m)
        {
            var newMatrix = new Matrix(m.Height, m.Width);

            // Copies each value into the new matrix.
            for (var i = 0; i < newMatrix.Height; i++)
            {
                for (var j = 0; j < newMatrix.Width; j++)
                {
                    newMatrix[i, j] = m[i, j];
                }
            }

            return newMatrix;
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

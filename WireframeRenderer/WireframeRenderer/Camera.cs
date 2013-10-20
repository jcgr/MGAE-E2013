using System;

namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents the camera of the renderer.
    /// </summary>
    class Camera
    {

        /// <summary>
        /// Gets or sets the position of the camera.
        /// </summary>
        private Vector Position { get; set; }
        /// <summary>
        /// Gets or sets the place the camera is looking at.
        /// </summary>
        private Vector LookPoint { get; set; }
        /// <summary>
        /// Gets or sets the up direction of the camera.
        /// </summary>
        private Vector Up { get; set; }

        /// <summary>
        /// Gets or sets the far value of the camera.
        /// </summary>
        private double Far { get; set; }
        /// <summary>
        /// Gets or sets the near value of the camera.
        /// </summary>
        private double Near { get; set; }
        /// <summary>
        /// Gets or sets the width of the camera.
        /// </summary>
        public double Width { get; private set; }
        /// <summary>
        /// Gets or sets the height of the camera.
        /// </summary>
        public double Height { get; private set; }
        /// <summary>
        /// Gets or sets the field of view of the camera.
        /// </summary>
        private double FieldOfView { get; set; }
        /// <summary>
        /// Gets or sets the aspect ratio of the camera.
        /// </summary>
        private double AspectRatio { get; set; }

        /// <summary>
        /// Gets all the camera transformations multiplied with each other.
        /// </summary>
        public Matrix AllTransforms { get; private set; }

        /// <summary>
        /// Creates a new instance of Camera.
        /// </summary>
        public Camera()
        {
            //Position = new Vector(100, 100, 300);
            Position = new Vector(0, 0, 0);
            LookPoint = new Vector(100, 100, 100);
            Up = new Vector(0, 1, 0);

            Far = -2000;
            Near = -300;

            //Width = 1280;
            //Height = 720;

            FieldOfView = 75d;
            AspectRatio = 16d / 9d;

            var fovradians = Math.PI / 180 * (FieldOfView);

            Width = (-2d) * Near * Math.Tan(fovradians / 2.0);
            Height = Width / AspectRatio;
        }

        #region Transforms
        /// <summary>
        /// Moves the camera depending on what button was clicked.
        /// </summary>
        /// <param name="key">The key on the keyboard that was clicked.</param>
        public void Move(string key)
        {
            if (key.Equals("a") || key.Equals("A"))
            {
                Position.X -= 10;
            }
            if (key.Equals("d") || key.Equals("D"))
            {
                Position.X += 10;
            }
            if (key.Equals("w") || key.Equals("W"))
            {
                Position.Y += 10;
            }
            if (key.Equals("s") || key.Equals("S"))
            {
                Position.Y -= 10;
            }
        }

        /// <summary>
        /// Calculates the camera location transformation (3.1.1)
        /// </summary>
        /// <returns>The resulting matrix.</returns>
        public Matrix LocationTransform()
        {
            var cameraLocationTransform = new Matrix(4, 4);

            cameraLocationTransform[0, 0] = 1;
            cameraLocationTransform[1, 1] = 1;
            cameraLocationTransform[2, 2] = 1;
            cameraLocationTransform[3, 3] = 1;

            cameraLocationTransform[0, 3] = -Position.X;
            cameraLocationTransform[1, 3] = -Position.Y;
            cameraLocationTransform[2, 3] = -Position.Z;

            return cameraLocationTransform;
        }

        /// <summary>
        /// Calculates the camera look transformation (3.1.2)
        /// </summary>
        /// <returns>The resulting matrix.</returns>
        public Matrix LookTransform()
        {
            // Direction
            var directionX = LookPoint.X - Position.X;
            var directionY = LookPoint.Y - Position.Y;
            var directionZ = LookPoint.Z - Position.Z;
            var direction = new Vector(directionX, directionY, directionZ);

            // Normalise for z-axis
            var normalVector = Vector.NormalizeVector(direction, Vector.VectorLength(direction));

            // x-axis
            var crossProduct = Vector.CrossProduct(Up, normalVector);
            var uVector = Vector.NormalizeVector(crossProduct, Vector.VectorLength(crossProduct));

            // y-axis
            var vVector = Vector.CrossProduct(normalVector, uVector);

            var transformMatrix = new Matrix(4, 4);

            transformMatrix[0, 0] = uVector.X;
            transformMatrix[0, 1] = uVector.Y;
            transformMatrix[0, 2] = uVector.Z;

            transformMatrix[1, 0] = vVector.X;
            transformMatrix[1, 1] = vVector.Y;
            transformMatrix[1, 2] = vVector.Z;

            transformMatrix[2, 0] = normalVector.X;
            transformMatrix[2, 1] = normalVector.Y;
            transformMatrix[2, 2] = normalVector.Z;

            transformMatrix[3, 3] = 1;

            return transformMatrix;
        }

        /// <summary>
        /// Calculates the projection matrix (3.2).
        /// </summary>
        /// <returns>The resulting matrix.</returns>
        public Matrix PerspectiveTransform()
        {
            var projectionMatrix = new Matrix(4, 4);

            projectionMatrix[0, 0] = (2d * Near) / Width;
            projectionMatrix[1, 1] = (2d * Near) / Height;
            projectionMatrix[2, 2] = (-(Far + Near)) / (Far - Near);
            projectionMatrix[2, 3] = (-2d * Far * Near) / (Far - Near);
            projectionMatrix[3, 2] = -1d;

            return projectionMatrix;
        }


        /// <summary>
        /// Multiplies perspective transform, look transform and location transforms.
        /// </summary>
        /// <returns>The resulting matrix.</returns>
        public Matrix CalculateTransforms()
        {
            var m = Matrix.NaiveMultiplication(PerspectiveTransform(), LookTransform());
            m = Matrix.NaiveMultiplication(m, LocationTransform());

            AllTransforms = m;

            return m;
        }
        #endregion
    }
}

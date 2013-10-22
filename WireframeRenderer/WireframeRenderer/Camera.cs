namespace WireframeRenderer
{
    /// <summary>
    /// A class that represents the camera of the renderer.
    /// </summary>
    class Camera
    {
        #region Properties
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
        #endregion

        #region Constructors
        /// <summary>
        /// Creates a new instance of Camera.
        /// </summary>
        public Camera()
        {
            Position = new Vector(200, 100, 0);
            LookPoint = new Vector(200, 50, 200);
            Up = new Vector(0, 1, 0);

            Far = -3000;
            Near = -300;

            Width = 1280;
            Height = 720;

            FieldOfView = 90d;
            AspectRatio = Width / Height;

            //var radians = Math.PI / 180 * (FieldOfView);

            //Width = (-2d) * Near * Math.Tan(radians / 2.0);
            //Height = Width / AspectRatio;

            CalculateTransforms();
        }
        #endregion

        #region Movement
        /// <summary>
        /// Moves the camera depending on what button was clicked.
        /// </summary>
        /// <param name="key">The key on the keyboard that was clicked.</param>
        public void Move(string key)
        {
            switch (key)
            {
                // Movement
                case "a":
                    ChangePosition(-10, 0, 0);
                    break;

                case "d":
                    ChangePosition(10, 0, 0);
                    break;

                case "s":
                    ChangePosition(0, -10, 0);
                    break;

                case "w":
                    ChangePosition(0, 10, 0);
                    break;

                case "q":
                    ChangePosition(0, 0, -10);
                    break;

                case "e":
                    ChangePosition(0, 0, 10);
                    break;

                // Rotation
                case "j":
                    ChangeRotation(-10, 0, 0);
                    break;

                case "l":
                    ChangeRotation(10, 0, 0);
                    break;

                case "k":
                    ChangeRotation(0, -10, 0);
                    break;

                case "i":
                    ChangeRotation(0, 10, 0);
                    break;

                case "u":
                    ChangeRotation(0, 0, -10);
                    break;

                case "o":
                    ChangeRotation(0, 0, 10);
                    break;
            }
        }

        /// <summary>
        /// Changes the position of the camera.
        /// </summary>
        /// <param name="x">The change to the x-coordinate of the camera.</param>
        /// <param name="y">The change to the y-coordinate of the camera.</param>
        /// <param name="z">The change to the z-coordinate of the camera.</param>
        private void ChangePosition(int x, int y, int z)
        {
            Position.X += x;
            Position.Y += y;
            Position.Z += z;

            ChangeRotation(x, y, z);
        }

        /// <summary>
        /// Changes the rotation of the camera.
        /// </summary>
        /// <param name="x">The change to the x-coordinate of the look point.</param>
        /// <param name="y">The change to the y-coordinate of the look point.</param>
        /// <param name="z">The change to the z-coordinate of the look point.</param>
        private void ChangeRotation(int x, int y, int z)
        {
            LookPoint.X += x;
            LookPoint.Y += y;
            LookPoint.Z += z;
        }
        #endregion

        #region Transforms
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

            // Normalize for z-axis
            var normalVector = direction.Normalize();

            // The two vectors
            var uVector = Vector.CrossProduct(Up, normalVector).Normalize();
            var vVector = Vector.CrossProduct(normalVector, uVector).Normalize();

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
        public void CalculateTransforms()
        {
            var allTransformMatrix = Matrix.NaiveMultiplication(PerspectiveTransform(), LookTransform());
            allTransformMatrix = Matrix.NaiveMultiplication(allTransformMatrix, LocationTransform());

            AllTransforms = allTransformMatrix;
        }
        #endregion
    }
}

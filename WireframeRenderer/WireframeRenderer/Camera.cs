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
        public Vector Position { get; set; }
        /// <summary>
        /// Gets or sets the place the camera is looking at.
        /// </summary>
        public Vector LookPoint { get; set; }
        /// <summary>
        /// Gets or sets the up direction of the camera.
        /// </summary>
        public Vector Up { get; set; }

        /// <summary>
        /// Gets or sets the far value of the camera.
        /// </summary>
        public double Far { get; set; }
        /// <summary>
        /// Gets or sets the near value of the camera.
        /// </summary>
        public double Near { get; set; }
        /// <summary>
        /// Gets or sets the width of the camera.
        /// </summary>
        public double Width { get; set; }
        /// <summary>
        /// Gets or sets the height of the camera.
        /// </summary>
        public double Height { get; set; }
        /// <summary>
        /// Gets or sets the field of view of the camera.
        /// </summary>
        public double FieldOfView { get; set; }
        /// <summary>
        /// Gets or sets the aspect ratio of the camera.
        /// </summary>
        public double AspectRatio { get; set; }

        /// <summary>
        /// Creates a new instance of Camera.
        /// </summary>
        public Camera()
        {
            //Position = new Vector(100, 100, 300);
            Position = new Vector(0, 0, 0);
            LookPoint = new Vector(200, 200, 200);
            Up = new Vector(0, 1, 0);

            Far = -500;
            Near = -100;

            Width = 1280;
            Height = 720;

            FieldOfView = 75;

            AspectRatio = Width / Height;
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

            cameraLocationTransform[3, 0] = -Position.X;
            cameraLocationTransform[3, 1] = -Position.Y;
            cameraLocationTransform[3, 2] = -Position.Z;

            return cameraLocationTransform;
        }

        /// <summary>
        /// Calculates the camera look transformation (3.1.2)
        /// </summary>
        /// <returns>The resulting matrix.</returns>
        public Matrix LookTransform()
        {
            var cameraLocation = Position;
            var cameraLookPoint = LookPoint;

            // Direction
            var directionX = cameraLookPoint.X - cameraLocation.X;
            var directionY = cameraLookPoint.Y - cameraLocation.Y;
            var directionZ = cameraLookPoint.Z - cameraLocation.Z;
            var direction = new Vector(directionX, directionY, directionZ);

            // Normalise for z-axis
            var normalVector = Transformation.NormalizeVector(direction, Transformation.VectorLength(direction));

            // x-axis
            var crossProduct = Transformation.CrossProduct(Up, normalVector);
            var xVector = Transformation.NormalizeVector(crossProduct, Transformation.VectorLength(crossProduct));

            // y-axis
            var yVector = Transformation.CrossProduct(normalVector, xVector);

            var transformMatrix = new Matrix(4, 4);

            //transformMatrix[0, 0] = xVector.X;
            //transformMatrix[1, 0] = xVector.Y;
            //transformMatrix[2, 0] = xVector.Z;

            //transformMatrix[0, 1] = yVector.X;
            //transformMatrix[1, 1] = yVector.Y;
            //transformMatrix[2, 1] = yVector.Z;

            //transformMatrix[0, 2] = normalVector.X;
            //transformMatrix[1, 2] = normalVector.Y;
            //transformMatrix[2, 2] = normalVector.Z;

            transformMatrix[0, 0] = xVector.X;
            transformMatrix[0, 1] = xVector.Y;
            transformMatrix[0, 2] = xVector.Z;

            transformMatrix[1, 0] = yVector.X;
            transformMatrix[1, 1] = yVector.Y;
            transformMatrix[1, 2] = yVector.Z;

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
        public Matrix ProjectionMatrix()
        {
            var projectionMatrix = new Matrix(4, 4);

            projectionMatrix[0, 0] = (2 * Near) / Width;
            projectionMatrix[1, 1] = (2 * Near) / Height;
            projectionMatrix[2, 2] = (-(Far + Near)) / (Far - Near);
            projectionMatrix[2, 3] = (-2 * Far * Near) / (Far - Near);
            projectionMatrix[3, 2] = -1;

            return projectionMatrix;
        }
    }
}

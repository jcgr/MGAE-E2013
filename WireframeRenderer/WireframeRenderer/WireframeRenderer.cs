using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Windows.Forms;

namespace WireframeRenderer
{
    public partial class WireframeRenderer : Form
    {
        private Camera camera;
        private Graphics graphicsObj;
        private Pen myPen;
        private List<Triangle> pyramidTriangles = new List<Triangle>();

        /// <summary>
        /// Initializes the WireframeRenderer.
        /// </summary>
        public WireframeRenderer()
        {
            InitializeComponent();
            camera = new Camera();

            Width = (int)camera.Width;
            Height = (int)camera.Height;

            graphicsObj = CreateGraphics();

            myPen = new Pen(Color.Black, 2);

            LoadPyramid();

            KeyPreview = true;
            KeyPress += WireframeRenderer_KeyPress;

        }

        /// <summary>
        /// Paints the screen.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The args.</param>
        private void WireframeRenderer_Paint(object sender, PaintEventArgs e)
        {
            foreach (var triangle in pyramidTriangles)
            {
                //triangle.A.ScreenCoordinate = new Point((int)triangle.A.X, (int)triangle.A.Y);
                //triangle.B.ScreenCoordinate = new Point((int)triangle.B.X, (int)triangle.B.Y);
                //triangle.C.ScreenCoordinate = new Point((int)triangle.C.X, (int)triangle.C.Y);

                triangle.A = HandleVertex(triangle.A);
                triangle.B = HandleVertex(triangle.B);
                triangle.C = HandleVertex(triangle.C);

                DrawTriangle(triangle);
            }
        }

        /// <summary>
        /// Handles keyboard input.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The args.</param>
        private void WireframeRenderer_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Escape makes the program stop
            if (e.KeyChar == 27)
            {
                Application.Exit();
            }

            camera.Move(e.KeyChar.ToString(CultureInfo.InvariantCulture));

            Invalidate();
        }

        /// <summary>
        /// Draws the triangle to the screen.
        /// </summary>
        /// <param name="t">The triangle to draw.</param>
        private void DrawTriangle(Triangle t)
        {
            graphicsObj.DrawLine(myPen, t.A.ScreenCoordinate, t.B.ScreenCoordinate);
            graphicsObj.DrawLine(myPen, t.A.ScreenCoordinate, t.C.ScreenCoordinate);
            graphicsObj.DrawLine(myPen, t.B.ScreenCoordinate, t.C.ScreenCoordinate);
        }

        /// <summary>
        /// Changes a vertex' screencoordinates in relation to the camera (4)
        /// </summary>
        /// <param name="vertex">The vertex to transform.</param>
        private Vertex HandleVertex(Vertex vertex)
        {
            // Creating a matrix from the vertex for calculations
            var vertexMatrix = new Matrix(4, 1);
            vertexMatrix[0, 0] = vertex.X;
            vertexMatrix[1, 0] = vertex.Y;
            vertexMatrix[2, 0] = vertex.Z;
            vertexMatrix[3, 0] = vertex.W;

            // Multipling the camera stuff with the vertex
            var viewSpaceMatrix = Matrix.NaiveMultiplication(camera.PerspectiveTransform(), camera.LookTransform());
            viewSpaceMatrix = Matrix.NaiveMultiplication(viewSpaceMatrix, camera.LocationTransform());
            viewSpaceMatrix = Matrix.NaiveMultiplication(viewSpaceMatrix, vertexMatrix);

            // Back to vertex 
            var vertexViewSpace = new Vertex(viewSpaceMatrix[0, 0], viewSpaceMatrix[1, 0], viewSpaceMatrix[2, 0], viewSpaceMatrix[3, 0]);

            vertexViewSpace.X = vertexViewSpace.X / vertexViewSpace.W;
            vertexViewSpace.Y = vertexViewSpace.Y / vertexViewSpace.W;
            vertexViewSpace.Z = vertexViewSpace.Z / vertexViewSpace.W;

            if (vertexViewSpace.X < -1 || vertexViewSpace.X > 1 ||
                vertexViewSpace.Y < -1 || vertexViewSpace.Y > 1 ||
                vertexViewSpace.Z < -1 || vertexViewSpace.Z > 1)
            {
                return vertex;
            }

            // Create the screen point for the vertex
            var newScreenPoint = new Point
            {
                X = (int) (vertexViewSpace.X * (camera.Width / 2) + (camera.Width / 2)),
                Y = (int) (-vertexViewSpace.Y * (camera.Height / 2) + (camera.Height / 2))
            };
            vertex.ScreenCoordinate = newScreenPoint;

            return vertex;
        }

        /// <summary>
        /// Loads a pyramid for drawing.
        /// </summary>
        void LoadPyramid()
        {
            const int low = 50;
            const int mid = 100;
            const int top = 150;

            var v1 = new Vertex(low, top, -low);
            var v2 = new Vertex(top, top, -low);
            var v3 = new Vertex(mid, top, -top);
            var v4 = new Vertex(mid, low, -mid);

            var t1 = new Triangle(v1, v2, v3);
            var t2 = new Triangle(v1, v2, v4);
            var t3 = new Triangle(v1, v3, v4);
            var t4 = new Triangle(v2, v3, v4);

            pyramidTriangles.Add(t1);
            pyramidTriangles.Add(t2);
            pyramidTriangles.Add(t3);
            pyramidTriangles.Add(t4);
        }
    }
}

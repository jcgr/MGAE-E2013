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
            camera.CalculateTransforms();

            Width = 1280;
            Height = 720;
            //Width = (int)camera.Width;
            //Height = (int)camera.Height;

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
            camera.CalculateTransforms();

            foreach (var triangle in pyramidTriangles)
            {
                triangle.UpdateVerticesScreenPoint(camera);
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
        /// Loads a pyramid for drawing.
        /// </summary>
        private void LoadPyramid()
        {
            //pyramidTriangles = Pyramid();
            const int low = 50;
            const int mid = 100;
            const int top = 150;

            var v1 = new Vertex(low, 0, low);
            var v2 = new Vertex(top, 0, low);
            var v3 = new Vertex(mid, 0, top);
            var v4 = new Vertex(mid, mid, mid);

            var t1 = new Triangle(v1, v2, v3);
            var t2 = new Triangle(v1, v2, v4);
            var t3 = new Triangle(v1, v3, v4);
            var t4 = new Triangle(v2, v3, v4);

            pyramidTriangles.Add(t1);
            pyramidTriangles.Add(t2);
            pyramidTriangles.Add(t3);
            pyramidTriangles.Add(t4);
        }

        /// <summary>
        /// Loads a standard pyramid shape.
        /// </summary>
        /// <returns>A list of triangles making up a pyramid.</returns>
        private List<Triangle> Pyramid()
        {
            var tempTriangles = new List<Triangle>();

            const int near = 50;
            const int mid = 100;
            const int far = 150;
            const int height = 75;

            var v1 = new Vertex(near, 0, near);
            var v2 = new Vertex(far, 0, near);
            var v3 = new Vertex(near, 0, far);
            var v4 = new Vertex(far, 0, far);
            var v5 = new Vertex(mid, height, mid);

            var t1 = new Triangle(v1, v2, v5);
            var t2 = new Triangle(v1, v3, v5);
            var t3 = new Triangle(v2, v4, v5);
            var t4 = new Triangle(v3, v4, v5);

            tempTriangles.Add(t1);
            tempTriangles.Add(t2);
            tempTriangles.Add(t3);
            tempTriangles.Add(t4);

            return tempTriangles;
        }
    }
}

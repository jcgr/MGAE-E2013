using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Windows.Forms;

namespace WireframeRenderer
{
    public partial class WireframeRenderer : Form
    {
        private readonly Camera _camera = new Camera();
        private readonly Graphics _graphicsObj;
        private readonly Pen _myPen = new Pen(Color.Black, 2);
        private readonly List<Triangle> _pyramidTriangles = new List<Triangle>();

        /// <summary>
        /// Initializes the WireframeRenderer.
        /// </summary>
        public WireframeRenderer()
        {
            InitializeComponent();

            Width = (int)_camera.Width;
            Height = (int)_camera.Height;
            //Width = 1280;
            //Height = 720;

            _graphicsObj = CreateGraphics();

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
            foreach (var triangle in _pyramidTriangles)
            {
                triangle.UpdateVerticesScreenPoint(_camera);
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
            // Escape makes the program stop.
            if (e.KeyChar == 27)
            {
                Application.Exit();
            }

            _camera.Move(e.KeyChar.ToString(CultureInfo.InvariantCulture));

            Invalidate();
        }

        /// <summary>
        /// Draws the triangle to the screen.
        /// </summary>
        /// <param name="t">The triangle to draw.</param>
        private void DrawTriangle(Triangle t)
        {
            _graphicsObj.DrawLine(_myPen, t.A.ScreenCoordinate, t.B.ScreenCoordinate);
            _graphicsObj.DrawLine(_myPen, t.A.ScreenCoordinate, t.C.ScreenCoordinate);
            _graphicsObj.DrawLine(_myPen, t.B.ScreenCoordinate, t.C.ScreenCoordinate);
        }

        /// <summary>
        /// Loads a pyramid for drawing.
        /// </summary>
        private void LoadPyramid()
        {
            const int low = 100;
            const int medium = 200;
            const int heigh = 300;

            var v1 = new Vertex(low, 0, low);
            var v2 = new Vertex(heigh, 0, low);
            var v3 = new Vertex(low, 0, heigh);
            var v4 = new Vertex(heigh, 0, heigh);
            var v5 = new Vertex(medium, medium, medium);

            var t1 = new Triangle(v1, v2, v5);
            var t2 = new Triangle(v1, v3, v5);
            var t3 = new Triangle(v2, v4, v5);
            var t4 = new Triangle(v3, v4, v5);

            _pyramidTriangles.Add(t1);
            _pyramidTriangles.Add(t2);
            _pyramidTriangles.Add(t3);
            _pyramidTriangles.Add(t4);
        }
    }
}

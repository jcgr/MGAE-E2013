namespace WireframeRenderer
{
    partial class WireframeRenderer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // WireframeRenderer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(508, 393);
            this.Name = "WireframeRenderer";
            this.Text = "Form1";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.WireframeRenderer_Paint);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.WireframeRenderer_KeyPress);
            this.ResumeLayout(false);

        }

        #endregion
    }
}


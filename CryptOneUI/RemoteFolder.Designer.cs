namespace CryptOneService
{
    partial class RemoteFolder
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
            this.label1 = new System.Windows.Forms.Label();
            this.folderList = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.closeButton = new System.Windows.Forms.Button();
            this.getButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(34, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(231, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Available encrypted folders:";
            // 
            // folderList
            // 
            this.folderList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.folderList.FullRowSelect = true;
            this.folderList.Location = new System.Drawing.Point(34, 77);
            this.folderList.Name = "folderList";
            this.folderList.Size = new System.Drawing.Size(631, 198);
            this.folderList.TabIndex = 1;
            this.folderList.UseCompatibleStateImageBehavior = false;
            this.folderList.View = System.Windows.Forms.View.Details;
            this.folderList.SelectedIndexChanged += new System.EventHandler(this.folderList_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 200;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Size";
            this.columnHeader2.Width = 100;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Last modified";
            this.columnHeader3.Width = 200;
            // 
            // closeButton
            // 
            this.closeButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.closeButton.Location = new System.Drawing.Point(553, 312);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(112, 34);
            this.closeButton.TabIndex = 2;
            this.closeButton.Text = "Close";
            this.closeButton.UseVisualStyleBackColor = true;
            // 
            // getButton
            // 
            this.getButton.Location = new System.Drawing.Point(34, 312);
            this.getButton.Name = "getButton";
            this.getButton.Size = new System.Drawing.Size(112, 34);
            this.getButton.TabIndex = 4;
            this.getButton.Text = "Pull";
            this.getButton.UseVisualStyleBackColor = true;
            this.getButton.Click += new System.EventHandler(this.getButton_Click);
            // 
            // RemoteFolder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(693, 372);
            this.Controls.Add(this.getButton);
            this.Controls.Add(this.closeButton);
            this.Controls.Add(this.folderList);
            this.Controls.Add(this.label1);
            this.Name = "RemoteFolder";
            this.Text = "CryptOne";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Label label1;
        private ListView folderList;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private Button closeButton;
        private Button getButton;
        private ColumnHeader columnHeader3;
    }
}
﻿namespace CryptOneService
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem(new string[] {
            "",
            "0",
            "Google Cloud",
            "~\\Google Drive"}, -1);
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.keyFolderEdit = new System.Windows.Forms.TextBox();
            this.setKeyFolderRadioButton = new System.Windows.Forms.RadioButton();
            this.autoDetectKeyFolderRadioButton = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.button6 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.foldersList = new System.Windows.Forms.ListView();
            this.folderIdColumn = new System.Windows.Forms.ColumnHeader();
            this.folderNameColumn = new System.Windows.Forms.ColumnHeader();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.removeCloudButton = new System.Windows.Forms.Button();
            this.addCloudButton = new System.Windows.Forms.Button();
            this.cloudsList = new System.Windows.Forms.ListView();
            this.cloudId = new System.Windows.Forms.ColumnHeader();
            this.localFolderColumn = new System.Windows.Forms.ColumnHeader();
            this.cloudFolderDescriptionColumn = new System.Windows.Forms.ColumnHeader();
            this.fullPathColumn = new System.Windows.Forms.ColumnHeader();
            this.button2 = new System.Windows.Forms.Button();
            this.applyButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(28, 22);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(956, 342);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.keyFolderEdit);
            this.tabPage1.Controls.Add(this.setKeyFolderRadioButton);
            this.tabPage1.Controls.Add(this.autoDetectKeyFolderRadioButton);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 34);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(948, 304);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Key storage";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // keyFolderEdit
            // 
            this.keyFolderEdit.Location = new System.Drawing.Point(117, 141);
            this.keyFolderEdit.Name = "keyFolderEdit";
            this.keyFolderEdit.Size = new System.Drawing.Size(150, 31);
            this.keyFolderEdit.TabIndex = 3;
            this.keyFolderEdit.Text = "D:\\";
            // 
            // setKeyFolderRadioButton
            // 
            this.setKeyFolderRadioButton.AutoSize = true;
            this.setKeyFolderRadioButton.Location = new System.Drawing.Point(117, 89);
            this.setKeyFolderRadioButton.Name = "setKeyFolderRadioButton";
            this.setKeyFolderRadioButton.Size = new System.Drawing.Size(146, 29);
            this.setKeyFolderRadioButton.TabIndex = 2;
            this.setKeyFolderRadioButton.TabStop = true;
            this.setKeyFolderRadioButton.Text = "Specify folder";
            this.setKeyFolderRadioButton.UseVisualStyleBackColor = true;
            // 
            // autoDetectKeyFolderRadioButton
            // 
            this.autoDetectKeyFolderRadioButton.AutoSize = true;
            this.autoDetectKeyFolderRadioButton.Location = new System.Drawing.Point(117, 41);
            this.autoDetectKeyFolderRadioButton.Name = "autoDetectKeyFolderRadioButton";
            this.autoDetectKeyFolderRadioButton.Size = new System.Drawing.Size(263, 29);
            this.autoDetectKeyFolderRadioButton.TabIndex = 1;
            this.autoDetectKeyFolderRadioButton.TabStop = true;
            this.autoDetectKeyFolderRadioButton.Text = "Auto-detect external storage";
            this.autoDetectKeyFolderRadioButton.UseVisualStyleBackColor = true;
            this.autoDetectKeyFolderRadioButton.CheckedChanged += new System.EventHandler(this.autoDetectKeyFolderRadioButton_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(108, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Key location";
            // 
            // tabPage2
            // 
            this.tabPage2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tabPage2.Controls.Add(this.button6);
            this.tabPage2.Controls.Add(this.button5);
            this.tabPage2.Controls.Add(this.foldersList);
            this.tabPage2.Location = new System.Drawing.Point(4, 34);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(948, 304);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Folders";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(159, 251);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(112, 34);
            this.button6.TabIndex = 2;
            this.button6.Text = "Remove";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(6, 252);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(112, 34);
            this.button5.TabIndex = 1;
            this.button5.Text = "Add";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // foldersList
            // 
            this.foldersList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.folderIdColumn,
            this.folderNameColumn});
            this.foldersList.FullRowSelect = true;
            this.foldersList.GridLines = true;
            this.foldersList.Location = new System.Drawing.Point(6, 35);
            this.foldersList.Name = "foldersList";
            this.foldersList.Size = new System.Drawing.Size(893, 199);
            this.foldersList.TabIndex = 0;
            this.foldersList.UseCompatibleStateImageBehavior = false;
            this.foldersList.View = System.Windows.Forms.View.Details;
            // 
            // folderIdColumn
            // 
            this.folderIdColumn.Text = "Index";
            // 
            // folderNameColumn
            // 
            this.folderNameColumn.Text = "Path";
            this.folderNameColumn.Width = 800;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.removeCloudButton);
            this.tabPage3.Controls.Add(this.addCloudButton);
            this.tabPage3.Controls.Add(this.cloudsList);
            this.tabPage3.Location = new System.Drawing.Point(4, 34);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(948, 304);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Clouds";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // removeCloudButton
            // 
            this.removeCloudButton.Location = new System.Drawing.Point(159, 251);
            this.removeCloudButton.Name = "removeCloudButton";
            this.removeCloudButton.Size = new System.Drawing.Size(112, 34);
            this.removeCloudButton.TabIndex = 2;
            this.removeCloudButton.Text = "Remove";
            this.removeCloudButton.UseVisualStyleBackColor = true;
            this.removeCloudButton.Click += new System.EventHandler(this.removeCloudButton_Click);
            // 
            // addCloudButton
            // 
            this.addCloudButton.Location = new System.Drawing.Point(6, 252);
            this.addCloudButton.Name = "addCloudButton";
            this.addCloudButton.Size = new System.Drawing.Size(112, 34);
            this.addCloudButton.TabIndex = 1;
            this.addCloudButton.Text = "Add";
            this.addCloudButton.UseVisualStyleBackColor = true;
            this.addCloudButton.Click += new System.EventHandler(this.addCloudButton_Click);
            // 
            // cloudsList
            // 
            this.cloudsList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.cloudId,
            this.localFolderColumn,
            this.cloudFolderDescriptionColumn,
            this.fullPathColumn});
            this.cloudsList.FullRowSelect = true;
            this.cloudsList.GridLines = true;
            this.cloudsList.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem2});
            this.cloudsList.Location = new System.Drawing.Point(6, 35);
            this.cloudsList.Name = "cloudsList";
            this.cloudsList.Size = new System.Drawing.Size(936, 197);
            this.cloudsList.TabIndex = 0;
            this.cloudsList.UseCompatibleStateImageBehavior = false;
            this.cloudsList.View = System.Windows.Forms.View.Details;
            this.cloudsList.DoubleClick += new System.EventHandler(this.cloudsList_DoubleClick);
            // 
            // cloudId
            // 
            this.cloudId.Text = "ID";
            this.cloudId.Width = 50;
            // 
            // localFolderColumn
            // 
            this.localFolderColumn.Text = "Folder";
            this.localFolderColumn.Width = 200;
            // 
            // cloudFolderDescriptionColumn
            // 
            this.cloudFolderDescriptionColumn.Text = "Description";
            this.cloudFolderDescriptionColumn.Width = 200;
            // 
            // fullPathColumn
            // 
            this.fullPathColumn.Text = "Full path";
            this.fullPathColumn.Width = 400;
            // 
            // button2
            // 
            this.button2.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button2.Location = new System.Drawing.Point(872, 370);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(112, 34);
            this.button2.TabIndex = 2;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // applyButton
            // 
            this.applyButton.Location = new System.Drawing.Point(583, 370);
            this.applyButton.Name = "applyButton";
            this.applyButton.Size = new System.Drawing.Size(112, 34);
            this.applyButton.TabIndex = 3;
            this.applyButton.Text = "Apply";
            this.applyButton.UseVisualStyleBackColor = true;
            this.applyButton.Click += new System.EventHandler(this.applyButton_Click);
            // 
            // okButton
            // 
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(739, 370);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(112, 34);
            this.okButton.TabIndex = 4;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(994, 435);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.applyButton);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "CryptOne Configuration";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.Form1_Shown);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private TabControl tabControl1;
        private TabPage tabPage1;
        private TextBox keyFolderEdit;
        private RadioButton setKeyFolderRadioButton;
        private RadioButton autoDetectKeyFolderRadioButton;
        private Label label1;
        private TabPage tabPage2;
        private Button button2;
        private TabPage tabPage3;
        private Button removeCloudButton;
        private Button addCloudButton;
        private ListView cloudsList;
        private ColumnHeader cloudId;
        private ColumnHeader localFolderColumn;
        private Button button6;
        private Button button5;
        private ListView foldersList;
        private ColumnHeader folderIdColumn;
        private ColumnHeader folderNameColumn;
        private ColumnHeader cloudFolderDescriptionColumn;
        private ColumnHeader fullPathColumn;
        private Button applyButton;
        private Button okButton;
    }
}
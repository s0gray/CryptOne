namespace CryptOneService
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
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem(new string[] {
            "",
            "0",
            "Google Cloud",
            "~\\Google Drive"}, -1);
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.useButton = new System.Windows.Forms.Button();
            this.initButton = new System.Windows.Forms.Button();
            this.removableList = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.label3 = new System.Windows.Forms.Label();
            this.keyStatusLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.keyFolderEdit = new System.Windows.Forms.TextBox();
            this.setKeyFolderRadioButton = new System.Windows.Forms.RadioButton();
            this.autoDetectKeyFolderRadioButton = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.refreshButton = new System.Windows.Forms.Button();
            this.pushButton = new System.Windows.Forms.Button();
            this.removeMonitoredFolderButton = new System.Windows.Forms.Button();
            this.addMonitoredFolderButton = new System.Windows.Forms.Button();
            this.foldersList = new System.Windows.Forms.ListView();
            this.folderIdColumn = new System.Windows.Forms.ColumnHeader();
            this.folderNameColumn = new System.Windows.Forms.ColumnHeader();
            this.cloudFileColumn = new System.Windows.Forms.ColumnHeader();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.removeCloudButton = new System.Windows.Forms.Button();
            this.addCloudButton = new System.Windows.Forms.Button();
            this.cloudsList = new System.Windows.Forms.ListView();
            this.cloudId = new System.Windows.Forms.ColumnHeader();
            this.localFolderColumn = new System.Windows.Forms.ColumnHeader();
            this.cloudFolderDescriptionColumn = new System.Windows.Forms.ColumnHeader();
            this.fullPathColumn = new System.Windows.Forms.ColumnHeader();
            this.tabConfigPage = new System.Windows.Forms.TabPage();
            this.browseTempFolderButton = new System.Windows.Forms.Button();
            this.tempFolderEdit = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.applyButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.cloudStorageEdit = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabConfigPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabConfigPage);
            this.tabControl1.Location = new System.Drawing.Point(28, 22);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1001, 342);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.useButton);
            this.tabPage1.Controls.Add(this.initButton);
            this.tabPage1.Controls.Add(this.removableList);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.keyStatusLabel);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.keyFolderEdit);
            this.tabPage1.Controls.Add(this.setKeyFolderRadioButton);
            this.tabPage1.Controls.Add(this.autoDetectKeyFolderRadioButton);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 34);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(993, 304);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Key storage";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // useButton
            // 
            this.useButton.Location = new System.Drawing.Point(622, 225);
            this.useButton.Name = "useButton";
            this.useButton.Size = new System.Drawing.Size(112, 34);
            this.useButton.TabIndex = 9;
            this.useButton.Text = "Use";
            this.useButton.UseVisualStyleBackColor = true;
            this.useButton.Click += new System.EventHandler(this.useButton_Click);
            // 
            // initButton
            // 
            this.initButton.Location = new System.Drawing.Point(622, 185);
            this.initButton.Name = "initButton";
            this.initButton.Size = new System.Drawing.Size(112, 34);
            this.initButton.TabIndex = 8;
            this.initButton.Text = "Initialize storage";
            this.initButton.UseVisualStyleBackColor = true;
            this.initButton.Click += new System.EventHandler(this.initButton_Click);
            // 
            // removableList
            // 
            this.removableList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.removableList.FullRowSelect = true;
            this.removableList.Location = new System.Drawing.Point(191, 185);
            this.removableList.Name = "removableList";
            this.removableList.Size = new System.Drawing.Size(425, 92);
            this.removableList.TabIndex = 7;
            this.removableList.UseCompatibleStateImageBehavior = false;
            this.removableList.View = System.Windows.Forms.View.Details;
            this.removableList.SelectedIndexChanged += new System.EventHandler(this.removableList_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Drive";
            this.columnHeader1.Width = 100;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Key";
            this.columnHeader2.Width = 100;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 185);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(156, 25);
            this.label3.TabIndex = 6;
            this.label3.Text = "Removable drives:";
            // 
            // keyStatusLabel
            // 
            this.keyStatusLabel.AutoSize = true;
            this.keyStatusLabel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.keyStatusLabel.Location = new System.Drawing.Point(185, 134);
            this.keyStatusLabel.Name = "keyStatusLabel";
            this.keyStatusLabel.Size = new System.Drawing.Size(166, 25);
            this.keyStatusLabel.TabIndex = 5;
            this.keyStatusLabel.Text = "Key file not found";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(43, 133);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 25);
            this.label2.TabIndex = 4;
            this.label2.Text = "Current status:";
            // 
            // keyFolderEdit
            // 
            this.keyFolderEdit.Location = new System.Drawing.Point(386, 40);
            this.keyFolderEdit.Name = "keyFolderEdit";
            this.keyFolderEdit.Size = new System.Drawing.Size(235, 31);
            this.keyFolderEdit.TabIndex = 3;
            this.keyFolderEdit.Text = "D:\\";
            // 
            // setKeyFolderRadioButton
            // 
            this.setKeyFolderRadioButton.AutoSize = true;
            this.setKeyFolderRadioButton.Location = new System.Drawing.Point(186, 40);
            this.setKeyFolderRadioButton.Name = "setKeyFolderRadioButton";
            this.setKeyFolderRadioButton.Size = new System.Drawing.Size(146, 29);
            this.setKeyFolderRadioButton.TabIndex = 2;
            this.setKeyFolderRadioButton.TabStop = true;
            this.setKeyFolderRadioButton.Text = "Specify folder";
            this.setKeyFolderRadioButton.UseVisualStyleBackColor = true;
            this.setKeyFolderRadioButton.CheckedChanged += new System.EventHandler(this.setKeyFolderRadioButton_CheckedChanged);
            // 
            // autoDetectKeyFolderRadioButton
            // 
            this.autoDetectKeyFolderRadioButton.AutoSize = true;
            this.autoDetectKeyFolderRadioButton.Location = new System.Drawing.Point(186, 86);
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
            this.label1.Location = new System.Drawing.Point(49, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(108, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Key location";
            // 
            // tabPage2
            // 
            this.tabPage2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tabPage2.Controls.Add(this.refreshButton);
            this.tabPage2.Controls.Add(this.pushButton);
            this.tabPage2.Controls.Add(this.removeMonitoredFolderButton);
            this.tabPage2.Controls.Add(this.addMonitoredFolderButton);
            this.tabPage2.Controls.Add(this.foldersList);
            this.tabPage2.Location = new System.Drawing.Point(4, 34);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(993, 304);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Folders";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // refreshButton
            // 
            this.refreshButton.Location = new System.Drawing.Point(718, 252);
            this.refreshButton.Name = "refreshButton";
            this.refreshButton.Size = new System.Drawing.Size(112, 34);
            this.refreshButton.TabIndex = 4;
            this.refreshButton.Text = "Refresh";
            this.refreshButton.UseVisualStyleBackColor = true;
            this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
            // 
            // pushButton
            // 
            this.pushButton.Location = new System.Drawing.Point(864, 252);
            this.pushButton.Name = "pushButton";
            this.pushButton.Size = new System.Drawing.Size(112, 34);
            this.pushButton.TabIndex = 3;
            this.pushButton.Text = "Push";
            this.pushButton.UseVisualStyleBackColor = true;
            this.pushButton.Click += new System.EventHandler(this.pushButton_Click);
            // 
            // removeMonitoredFolderButton
            // 
            this.removeMonitoredFolderButton.Location = new System.Drawing.Point(159, 251);
            this.removeMonitoredFolderButton.Name = "removeMonitoredFolderButton";
            this.removeMonitoredFolderButton.Size = new System.Drawing.Size(112, 34);
            this.removeMonitoredFolderButton.TabIndex = 2;
            this.removeMonitoredFolderButton.Text = "Remove";
            this.removeMonitoredFolderButton.UseVisualStyleBackColor = true;
            this.removeMonitoredFolderButton.Click += new System.EventHandler(this.removeMonitoredFolderButton_Click);
            // 
            // addMonitoredFolderButton
            // 
            this.addMonitoredFolderButton.Location = new System.Drawing.Point(6, 252);
            this.addMonitoredFolderButton.Name = "addMonitoredFolderButton";
            this.addMonitoredFolderButton.Size = new System.Drawing.Size(112, 34);
            this.addMonitoredFolderButton.TabIndex = 1;
            this.addMonitoredFolderButton.Text = "Add";
            this.addMonitoredFolderButton.UseVisualStyleBackColor = true;
            this.addMonitoredFolderButton.Click += new System.EventHandler(this.addMonitoredFolderButton_Click);
            // 
            // foldersList
            // 
            this.foldersList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.folderIdColumn,
            this.folderNameColumn,
            this.cloudFileColumn});
            this.foldersList.FullRowSelect = true;
            this.foldersList.GridLines = true;
            this.foldersList.Location = new System.Drawing.Point(6, 35);
            this.foldersList.Name = "foldersList";
            this.foldersList.Size = new System.Drawing.Size(970, 199);
            this.foldersList.TabIndex = 0;
            this.foldersList.UseCompatibleStateImageBehavior = false;
            this.foldersList.View = System.Windows.Forms.View.Details;
            this.foldersList.SelectedIndexChanged += new System.EventHandler(this.foldersList_SelectedIndexChanged);
            // 
            // folderIdColumn
            // 
            this.folderIdColumn.Text = "Index";
            this.folderIdColumn.Width = 100;
            // 
            // folderNameColumn
            // 
            this.folderNameColumn.Text = "Path";
            this.folderNameColumn.Width = 275;
            // 
            // cloudFileColumn
            // 
            this.cloudFileColumn.Text = "Cloud filename";
            this.cloudFileColumn.Width = 150;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.removeCloudButton);
            this.tabPage3.Controls.Add(this.addCloudButton);
            this.tabPage3.Controls.Add(this.cloudsList);
            this.tabPage3.Location = new System.Drawing.Point(4, 34);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(993, 304);
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
            listViewItem1});
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
            // tabConfigPage
            // 
            this.tabConfigPage.Controls.Add(this.cloudStorageEdit);
            this.tabConfigPage.Controls.Add(this.label5);
            this.tabConfigPage.Controls.Add(this.browseTempFolderButton);
            this.tabConfigPage.Controls.Add(this.tempFolderEdit);
            this.tabConfigPage.Controls.Add(this.label4);
            this.tabConfigPage.Location = new System.Drawing.Point(4, 34);
            this.tabConfigPage.Name = "tabConfigPage";
            this.tabConfigPage.Padding = new System.Windows.Forms.Padding(3);
            this.tabConfigPage.Size = new System.Drawing.Size(993, 304);
            this.tabConfigPage.TabIndex = 3;
            this.tabConfigPage.Text = "Config";
            this.tabConfigPage.UseVisualStyleBackColor = true;
            // 
            // browseTempFolderButton
            // 
            this.browseTempFolderButton.Location = new System.Drawing.Point(745, 45);
            this.browseTempFolderButton.Name = "browseTempFolderButton";
            this.browseTempFolderButton.Size = new System.Drawing.Size(112, 34);
            this.browseTempFolderButton.TabIndex = 2;
            this.browseTempFolderButton.Text = "Browse";
            this.browseTempFolderButton.UseVisualStyleBackColor = true;
            this.browseTempFolderButton.Click += new System.EventHandler(this.browseTempFolderButton_Click);
            // 
            // tempFolderEdit
            // 
            this.tempFolderEdit.Location = new System.Drawing.Point(260, 48);
            this.tempFolderEdit.Name = "tempFolderEdit";
            this.tempFolderEdit.Size = new System.Drawing.Size(453, 31);
            this.tempFolderEdit.TabIndex = 1;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 47);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(152, 25);
            this.label4.TabIndex = 0;
            this.label4.Text = "Temporary folder:";
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
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(20, 118);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(180, 25);
            this.label5.TabIndex = 3;
            this.label5.Text = "Cloud storage folder:";
            // 
            // cloudStorageEdit
            // 
            this.cloudStorageEdit.Enabled = false;
            this.cloudStorageEdit.Location = new System.Drawing.Point(260, 113);
            this.cloudStorageEdit.Name = "cloudStorageEdit";
            this.cloudStorageEdit.Size = new System.Drawing.Size(453, 31);
            this.cloudStorageEdit.TabIndex = 4;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1041, 424);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.applyButton);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "CryptOne Configuration";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.Form1_Shown);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabConfigPage.ResumeLayout(false);
            this.tabConfigPage.PerformLayout();
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
        private Button removeMonitoredFolderButton;
        private Button addMonitoredFolderButton;
        private ListView foldersList;
        private ColumnHeader folderIdColumn;
        private ColumnHeader folderNameColumn;
        private ColumnHeader cloudFolderDescriptionColumn;
        private ColumnHeader fullPathColumn;
        private Button applyButton;
        private Button okButton;
        private Label keyStatusLabel;
        private Label label2;
        private ColumnHeader cloudFileColumn;
        private Label label3;
        private ListView removableList;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private Button useButton;
        private Button initButton;
        private Button pushButton;
        private TabPage tabConfigPage;
        private TextBox tempFolderEdit;
        private Label label4;
        private Button browseTempFolderButton;
        private Button refreshButton;
        private TextBox cloudStorageEdit;
        private Label label5;
    }
}
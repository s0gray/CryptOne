using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CryptOneService
{
    public partial class EditCloudFolder : Form
    {
        public EditCloudFolder()
        {
            InitializeComponent();
        }

        public string folder = "";
        public string description = "";
        private void okButton_Click(object sender, EventArgs e)
        {
            this.folder = this.folderEdit.Text;
            this.description = this.cloudDescription.Text;
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1 = new FolderBrowserDialog();
            folderBrowserDialog1.SelectedPath = this.folderEdit.Text;

            DialogResult res = folderBrowserDialog1.ShowDialog();
            if(res == DialogResult.OK)
            {                
                this.folderEdit.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void EditCloudFolder_Load(object sender, EventArgs e)
        {
            this.cloudDescription.Text = this.description;
            this.folderEdit.Text = folder;
        }
    }
}

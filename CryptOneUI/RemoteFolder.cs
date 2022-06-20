using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CryptOneService
{
    public partial class RemoteFolder : Form
    {
        private CloudFolder cloudFolder;

        public RemoteFolder(CloudFolder cloudFolder)
        {
            InitializeComponent();

            this.cloudFolder = cloudFolder;
            Log.Line("RemoteFolder " + cloudFolder.fullPath);

            updateRemoteFolderList();

            this.getButton.Enabled = false;
        }

        private void updateRemoteFolderList()
        {
            string folder = cloudFolder.fullPath + "\\" + Form1.cloudStorageFolder;
            Log.Line("cloud-folder: " + folder);

            if (!Directory.Exists(folder)) return;

            string [] files = Directory.GetFiles(folder, "*.tgz.enc");

            folderList.Items.Clear();

            if(files!=null && files.Length>0)
            {
                for(int i=0; i<files.Length;i++)
                {
                    //var attr = File.GetAttributes(files[i]);
                    long size = new System.IO.FileInfo(files[i]).Length;
                    Log.Line("size = " + size);

                    DateTime dt = File.GetLastWriteTime(files[i]);

                    string[] arr = new string[3];
                    arr[0] = Path.GetFileName(files[i]);
                    arr[1] = "" +size;
                    arr[2] = dt.ToString();
                    folderList.Items.Add( new ListViewItem(arr) );
                }
            }

        }

        private void folderList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if( folderList.SelectedItems.Count==1)
            {
                getButton.Enabled = true;
            } else
            {
                getButton.Enabled = false;
            }
        }

        private void getButton_Click(object sender, EventArgs e)
        {

        }
    }
}

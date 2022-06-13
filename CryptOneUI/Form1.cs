using System.Diagnostics;
using System.IO;


namespace CryptOneService
{
    public partial class Form1 : Form
    {
        static string homeDir = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
        public const string SECTION = "config";
        public const string KEY_FOLDER = "keyFolder";
        public const string INI_FILENAME = "CryptOne.ini";
        public const string CLOUDFOLDER_KEY = "cloudFolder";
        public const string CLOUDDESCRIPTION_KEY = "cloudDescription";

        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void removeCloudButton_Click(object sender, EventArgs e)
        {
            if (cloudsList.SelectedItems.Count > 0)
            {
                for(int i=0; i < cloudsList.SelectedItems.Count; i++)
                {
                    ListViewItem item = cloudsList.SelectedItems[i];
                    Debug.WriteLine("deleting cloud #" + item.SubItems[0].Text);
                    cloudsList.Items.Remove(item);
                    applyButton.Enabled = true;

                }
            }
            else
            {
                Debug.WriteLine("Nothing selected");
            }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            IniFile ini = new IniFile();
            applyButton.Enabled = false;

            var keyFolder = ini.Read(KEY_FOLDER);
            if(keyFolder==null || keyFolder.Length==0)
            {
                Debug.WriteLine("Auto keyFolder");
                autoDetectKeyFolderRadioButton.Select();
                keyFolderEdit.Enabled = false;
            } else
            {
                Debug.WriteLine("keyFolder = [" + keyFolder + "]");

                setKeyFolderRadioButton.Select();
                keyFolderEdit.Enabled = true;
                keyFolderEdit.Text = keyFolder;

            }
            string currentFolder = Directory.GetCurrentDirectory();

            string[] arr = new string[2];
            arr[0] = "0";
            arr[1] = currentFolder;
            ListViewItem item = new ListViewItem(arr);
            foldersList.Items.Add(item);

            int index = 0;
            cloudsList.Items.Clear();
            while (true)
            {
                string key = CLOUDFOLDER_KEY + index;
                string descriptionKey = CLOUDDESCRIPTION_KEY + index;

                var cloudFolder = ini.Read(key);
                var cloudFolderDescription = ini.Read(descriptionKey);

                if (cloudFolder != null && cloudFolder.Length > 0)
                {
                    Debug.WriteLine("cloudFolder [" + cloudFolder + "] = [" + cloudFolder + "]");

                    string[] arr1 = new string[4];
                    arr1[0] = "" + index;
                    arr1[1] = cloudFolder;
                    arr1[2] = cloudFolderDescription;
                    arr1[3] = decodeFolder(cloudFolder);
                    ListViewItem item1 = new ListViewItem(arr1);
                    cloudsList.Items.Add(item1);

                }
                else
                {
                    break;
                }
                index++;
            }

        }

        private static string decodeFolder(string folder)
        {
            
            return folder.Replace("~", homeDir);
        }

        private void autoDetectKeyFolderRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (autoDetectKeyFolderRadioButton.Checked)
            {
                keyFolderEdit.Enabled = false;
            }
            else
            {
                keyFolderEdit.Enabled = true;

            }
        }

        void saveChanges()
        {
            Debug.WriteLine("save changes to ini file");
            File.Delete(INI_FILENAME);
            IniFile ini = new IniFile(INI_FILENAME);            
            for (int i = 0; i < cloudsList.Items.Count; i++)
            {
                string key = CLOUDFOLDER_KEY + i;                
                ini.Write(key, cloudsList.Items[i].SubItems[1].Text, SECTION);

                string descriptionKey = CLOUDDESCRIPTION_KEY + i;
                ini.Write(descriptionKey, cloudsList.Items[i].SubItems[2].Text, SECTION);

            }

            if (this.setKeyFolderRadioButton.Checked)
            {
                ini.Write(KEY_FOLDER, this.keyFolderEdit.Text, SECTION);
            }

            
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            // save
            saveChanges();
            applyButton.Enabled = false;
        }

        private void addCloudButton_Click(object sender, EventArgs e)
        {
            EditCloudFolder dlg = new EditCloudFolder();
            if( dlg.ShowDialog() == DialogResult.OK)
            {
                string[] arr1 = new string[4];
                arr1[0] = "" + cloudsList.Items.Count;
                arr1[1] = encodeFolder(dlg.folder);
                arr1[2] = dlg.description;
                arr1[3] = dlg.folder;
                ListViewItem item1 = new ListViewItem(arr1);
                cloudsList.Items.Add(item1);

                applyButton.Enabled = true;
            }
        }

        private static string encodeFolder(string folder)
        {
            return folder.Replace(homeDir, "~");
        }

        private void cloudsList_DoubleClick(object sender, EventArgs e)
        {
            if (cloudsList.SelectedItems.Count == 0)
            {
                Debug.WriteLine("Nothing selected");
                return;
            }

            if (cloudsList.SelectedItems.Count > 1)
            {
                Debug.WriteLine("Select only one row to edit");
                return;
            }
      
            ListViewItem item = cloudsList.SelectedItems[0];
            Debug.WriteLine("edit cloud #" + item.SubItems[0].Text);

            // edit
            EditCloudFolder dlg = new EditCloudFolder();
            dlg.folder = item.SubItems[3].Text;
            dlg.description = item.SubItems[2].Text;  

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                // update
                item.SubItems[3].Text = dlg.folder;
                item.SubItems[2].Text = dlg.description;
                item.SubItems[1].Text = encodeFolder(dlg.folder);

                // todo detect changes
                applyButton.Enabled = true;
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (applyButton.Enabled)
            {
                saveChanges();
            }
            this.Close();
        }
    }
}
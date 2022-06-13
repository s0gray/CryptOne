using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Management;


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
        public const string KEY_FILENAME = "key0001.ekey";

        public Form1()
        {
            InitializeComponent();

            Thread thread1 = new Thread(backgroundWorker1_DoWork);
            thread1.Start();
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

            updateKeyStatus();

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

        string getKeyStatusText()
        {
            Debug.WriteLine("getKeyStatusText");

            string keyPath = "";
            if (keyFolderEdit.Enabled)
            {
                keyPath = keyFolderEdit.Text;
            }
            else
            {
                keyPath = Tools.getKeyFolder();
            }
            Debug.WriteLine("keyPath = " + keyPath);

            string keyFileName = keyPath + KEY_FILENAME;
            Debug.WriteLine("keyFileName = " + keyFileName);

            if (keyPath != null && keyPath.Length > 0 && File.Exists(keyFileName))
            {
                return "Key file found at [" + keyFileName + "]";
            }
            else
            {
                string msg = "Key file not found";
                if (keyPath.Length > 0)
                {
                    msg += " at [" + keyFileName + "]";
                }
                return msg;
            }
        }

        void updateKeyStatus()
        {
            string msg = getKeyStatusText();
            keyStatusLabel.Text = msg;           
        }
        void updateKeyStatusFromOtherThread()
        {
            string msg = getKeyStatusText();

            keyStatusLabel.Invoke((MethodInvoker) delegate {
                // Running on the UI thread
                keyStatusLabel.Text = msg;
            });
        }


        private  void DeviceInsertedEvent(object sender, EventArrivedEventArgs e)
        {
           Debug.WriteLine("DeviceInsertedEvent");
            /*ManagementBaseObject instance = (ManagementBaseObject)e.NewEvent["TargetInstance"];
             foreach (var property in instance.Properties)
             {
                // Debug.WriteLine(property.Name + " = " + property.Value);
             }*/
            updateKeyStatusFromOtherThread();

        }

        private  void DeviceRemovedEvent(object sender, EventArrivedEventArgs e)
        {
           Debug.WriteLine("DeviceRemovedEvent");

            /*ManagementBaseObject instance = (ManagementBaseObject)e.NewEvent["TargetInstance"];
            foreach (var property in instance.Properties)
            {
              //  Debug.WriteLine(property.Name + " = " + property.Value);
            }*/
            updateKeyStatusFromOtherThread();
        }
        private void backgroundWorker1_DoWork() //object sender, DoWorkEventArgs e)
        {
              WqlEventQuery insertQuery = new WqlEventQuery("SELECT * FROM __InstanceCreationEvent WITHIN 2 WHERE TargetInstance ISA 'Win32_USBHub'");

              ManagementEventWatcher insertWatcher = new ManagementEventWatcher(insertQuery);
              insertWatcher.EventArrived += new EventArrivedEventHandler(DeviceInsertedEvent);
              insertWatcher.Start();

              WqlEventQuery removeQuery = new WqlEventQuery("SELECT * FROM __InstanceDeletionEvent WITHIN 2 WHERE TargetInstance ISA 'Win32_USBHub'");
              ManagementEventWatcher removeWatcher = new ManagementEventWatcher(removeQuery);
              removeWatcher.EventArrived += new EventArrivedEventHandler(DeviceRemovedEvent);
              removeWatcher.Start();

              // Do something while waiting for events
              System.Threading.Thread.Sleep(3000);

       /*     var watcher = new ManagementEventWatcher();
            var query = new WqlEventQuery("SELECT * FROM Win32_DeviceChangeEvent WHERE EventType = 2");
            watcher.EventArrived += new EventArrivedEventHandler(watcher_EventArrived);
            watcher.Query = query;
            watcher.Start();*/
        }

        private void watcher_EventArrived(object sender, EventArrivedEventArgs e)
        {
            ManagementBaseObject instance = (ManagementBaseObject)e.NewEvent["TargetInstance"];
            foreach (var property in instance.Properties)
            {
                Debug.WriteLine(property.Name + " = " + property.Value);
            }
        }
    }
}
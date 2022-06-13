using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Management;


namespace CryptOneService
{
    public partial class Form1 : Form
    {
        public static string homeDir = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
        public const string SECTION = "config";
        public const string KEY_FOLDER = "keyFolder";
        public const string INI_FILENAME = "CryptOne.ini";
        public const string MONITOREDFOLDER_KEY = "monitoredFolder";

        public const string CLOUDFOLDER_KEY = "cloudFolder";
        public const string CLOUDDESCRIPTION_KEY = "cloudDescription";
        public const string KEY_FILENAME = "key0001.ekey";

        MonitoredFoldersContainer monitoredFoldersContainer;
        CloudFolderContainer cloudFolderContainer;
        CryptoOne cryptoOne = new CryptoOne();
        public Form1()
        {
            InitializeComponent();
            monitoredFoldersContainer = new MonitoredFoldersContainer(this);
            cloudFolderContainer = new CloudFolderContainer();

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
                int deletedCount = 0;
                for (int i = 0; i < cloudsList.SelectedItems.Count; i++)
                {
                    ListViewItem item = cloudsList.SelectedItems[i];
                    Debug.WriteLine("deleting cloud folder #" + item.SubItems[0].Text);

                    int index = int.Parse(item.SubItems[0].Text);
                    cloudFolderContainer.remove(index - deletedCount);

                    deletedCount++;
                    applyButton.Enabled = true;
                }
                cloudFolderContainer.show( cloudsList );
            }
            else
            {
                Debug.WriteLine("Nothing selected");
            }
        }

        public string getFolderStatus(MonitoredFolder monitoredFolder)
        {
            int value = 0;

            for(int index = 0; index < cloudFolderContainer.getCount(); index++)
            {
                bool present = this.isArchivePresentOnCloud(monitoredFolder, index);
                if (present)
                {
                    value |= (1 << index);
                }
            }

            return Convert.ToString(value, 2);
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            IniFile ini = new IniFile();
            applyButton.Enabled = false;

            // START INIT KeyFolder TAB
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
            // END INIT KeyFolder TAB

            // START INIT folders TAB
            monitoredFoldersContainer.load(ini);
            monitoredFoldersContainer.show(foldersList);
            // END INIT folders TAB

            // START INIT cloudFolder TAB
            cloudFolderContainer.load(ini);
            cloudFolderContainer.show(cloudsList);
            // END INIT cloudFolder TAB

            startMainProcessing();
        }

        private void startMainProcessing()
        {
            for (int index = 0; index < monitoredFoldersContainer.getCount(); index++)
            {
                for (int cloudIndex = 0; cloudIndex < cloudFolderContainer.getCount(); cloudIndex++)
                {
                    bool present = isArchivePresentOnCloud(index, cloudIndex);
                    if (!present)
                    {
                        Debug.WriteLine("Folder ["+index+"] not present on cloud ["+cloudIndex+"] - will upload");

                        cryptoOne.push(monitoredFoldersContainer.get(index), cloudFolderContainer.get(cloudIndex));
                    }
                }
            }
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
            monitoredFoldersContainer.save(ini);
            cloudFolderContainer.save(ini);

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
                cloudFolderContainer.add(encodeFolder(dlg.folder), dlg.description, dlg.folder);

                cloudFolderContainer.show(cloudsList);
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
        private void backgroundWorker1_DoWork() 
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
        }


        private void addMonitoredFolderButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                monitoredFoldersContainer.add(folderBrowserDialog1.SelectedPath);
                monitoredFoldersContainer.show(foldersList);

                applyButton.Enabled = true;
            }
        }

        private void removeMonitoredFolderButton_Click(object sender, EventArgs e)
        {
            if (foldersList.SelectedItems.Count > 0)
            {
                int deletedCount = 0;
                for (int i = 0; i < foldersList.SelectedItems.Count; i++)
                {
                    ListViewItem item = foldersList.SelectedItems[i];
                    Debug.WriteLine("deleting monitored folder #" + item.SubItems[0].Text);
                    // foldersList.Items.Remove(item);

                    int index = int.Parse(item.SubItems[0].Text);
                    monitoredFoldersContainer.remove(index - deletedCount);

                    deletedCount++;
                    applyButton.Enabled = true;
                }
                monitoredFoldersContainer.show(foldersList);
            }
            else
            {
                Debug.WriteLine("Nothing selected");
            }
        }
        public bool isArchivePresentOnCloud(int folderIndex, int cloudIndex)
        {
            if (folderIndex < 0 || folderIndex >= monitoredFoldersContainer.getCount())
            {
                return false;
            }
            MonitoredFolder folder = monitoredFoldersContainer.get(folderIndex);
            return isArchivePresentOnCloud(folder, cloudIndex);
        }

        public bool isArchivePresentOnCloud(MonitoredFolder folder, int cloudIndex)
        {
            if (cloudIndex < 0 || cloudIndex >= cloudFolderContainer.getCount())
            {
                return false;
            }

            string filename = folder.getCloudFileName();
            string cloudFileName = cloudFolderContainer.get(cloudIndex).fullPath + filename;
            return File.Exists(cloudFileName);
        }
    }
}
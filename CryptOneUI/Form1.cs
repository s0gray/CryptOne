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
        public const string TEMP_DIR_KEY = "tempDir";
        public const string ROOT_DIR_KEY = "rootDir";

        public const string KEY_FILENAME = "key0001.ekey";
        public const string STATUS_PRESENT = "Present";
        public const string STATUS_ABSENT = "Absent";

        public static string cloudStorageFolder = "CryptOne";
        public static string AppName = "CryptOne";

        MonitoredFoldersContainer monitoredFoldersContainer;
        CloudFolderContainer cloudFolderContainer;
        KeyStorageContainer keyStorageContainer;

        CryptoOne cryptoOne = new CryptoOne();
        public static string appDataFolder =  Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData) + "\\" + Form1.AppName;
        static string IniFileName = Form1.appDataFolder + "\\" + AppName + ".ini";

        public Form1()
        {
            InitializeComponent();
            monitoredFoldersContainer = new MonitoredFoldersContainer(this);
            cloudFolderContainer = new CloudFolderContainer();
            keyStorageContainer = new KeyStorageContainer();

            Thread thread1 = new Thread(backgroundWorker1_DoWork);
            thread1.Start();

            Log.setTextBox(this.logBox);

        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        /// <summary>
        /// Initiliaze Form1
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Shown(object sender, EventArgs e)
        {
            IniFile ini = new IniFile(IniFileName);
            this.applyButton.Enabled = false;
            pushButton.Enabled = false;
            this.useButton.Enabled = false;
            this.initButton.Enabled = false;
            this.getButton.Enabled = false;
            this.browseKeyLocationButton.Enabled = false;  

            string tmpDir = ini.Read(TEMP_DIR_KEY);
            if(tmpDir != null && tmpDir.Length>0)
            {
                CryptoOne.tempFolder = tmpDir;
            } else
            {
                CryptoOne.tempFolder = Path.GetTempPath();
            }

           /* string rootDir = ini.Read(ROOT_DIR_KEY);
            if (rootDir != null && rootDir.Length > 0)
            {
                Form1.localFolderRoot = rootDir;
            }
            else
            {
                Form1.localFolderRoot = Form1.appDataFolder + "\\Clouds\\";
            }*/
            Tools.checkAndCreateIniFiles();

            this.tempFolderEdit.Text = CryptoOne.tempFolder;
            this.cloudStorageEdit.Text = Form1.cloudStorageFolder;
//            this.localFolderRootEdit.Text = Form1.localFolderRoot;

            // START INIT KeyFolder TAB
            keyStorageContainer.load(ini);
            showKeyTab();
            updateKeyStatus();
            // END INIT KeyFolder TAB

            // START INIT cloudFolder TAB
            cloudFolderContainer.load(ini);
            cloudFolderContainer.show(cloudsList);

            addCloudFolderColumns();
            cloudFolderContainer.initCloudStorages();
            // END INIT cloudFolder TAB

            // START INIT folders TAB
            monitoredFoldersContainer.load(ini);
            monitoredFoldersContainer.show(foldersList);
            // END INIT folders TAB

            updateRemovableList();
            startMainProcessing();
        }

        private void addCloudFolderColumns()
        {
            int nClouds = cloudFolderContainer.getCount();

            for(int i = 0; i < nClouds; i++)
            {
                CloudFolder cloudFolder = cloudFolderContainer.get(i);
                foldersList.Columns.Add(cloudFolder.description, 140, HorizontalAlignment.Left);
            }
        }

        private void updateRemovableListFromOtherThread()
        {
            string[] removable = Tools.getRemovableDrives();
            
            this.removableList.Invoke((MethodInvoker)delegate {
                this.removableList.Items.Clear();
            });

            for (int i = 0; i < removable.Length; i++)
            {
                string[] arr2 = new string[2];
                arr2[0] = removable[i];
                if (Tools.isKeyFileExistOnRemovableDrive(removable[i]))
                    arr2[1] = STATUS_PRESENT;
                else
                    arr2[1] = STATUS_ABSENT;

                this.removableList.Invoke((MethodInvoker)delegate {
                    this.removableList.Items.Add(new ListViewItem(arr2));
                });
            }
        }
        private void updateRemovableList()
        {
            string[] removable = Tools.getRemovableDrives();
            this.removableList.Items.Clear();
            for(int i=0; i< removable.Length; i++)
            {
                string[] arr2 = new string[2];
                arr2[0] = removable[i];
                if (Tools.isKeyFileExistOnRemovableDrive(removable[i]))
                    arr2[1] = "Present";
                else
                    arr2[1] = "Absent";
                this.removableList.Items.Add(new ListViewItem(arr2));
            }
        }
        private void showKeyTab()
        {
            if(keyStorageContainer.autodetectKeyStorage)
            {
                //Debug.WriteLine("Auto keyFolder");
                autoDetectKeyFolderRadioButton.Select();
                keyFolderEdit.Enabled = false;
            } else
            {
//                Debug.WriteLine("keyFolder = [" + keyStorageContainer.getKeyFolder() + "]");

                setKeyFolderRadioButton.Select();
                keyFolderEdit.Enabled = true;
                keyFolderEdit.Text = keyStorageContainer.getKeyFolder();
            }
        }

        private void startMainProcessing()
        {
            updateFolderStatus();
          /*  string keyFolder = Tools.getKeyFolder();
            if (keyFolder == null || keyFolder.Length == 0)
            {
                Log.Line("keyfolder not found");
                return;
            }
            Log.Line("keyFolder = " + keyFolder);

            for (int index = 0; index < monitoredFoldersContainer.getCount(); index++)
            {
                for (int cloudIndex = 0; cloudIndex < cloudFolderContainer.getCount(); cloudIndex++)
                {
                    bool present = isArchivePresentOnCloud(index, cloudIndex);
                    if (!present)
                    {
                        Log.Line("Folder ["+index+"] not present on cloud ["+cloudIndex+"] - will  upload");

                        cryptoOne.push(monitoredFoldersContainer.get(index), cloudFolderContainer.get(cloudIndex), keyFolder + Form1.KEY_FILENAME);
                    } else
                    {
                        Log.Line("Folder [" + index + "] present on cloud [" + cloudIndex + "] OK");

                    }
                }
            }*/
        }

            
        private void updateFolderStatus()
        {
            Log.Line("monitoredFoldersContainer : "+ monitoredFoldersContainer.getCount()+", sz = "+ foldersList.Items.Count);
            for (int index = 0; index < monitoredFoldersContainer.getCount(); index++)
            {
                MonitoredFolder fld = monitoredFoldersContainer.get(index);
                for (int cloudIndex = 0; cloudIndex < cloudFolderContainer.getCount(); cloudIndex++)
                {
                    bool present = isArchivePresentOnCloud(index, cloudIndex);
                    string msg = "";
                    if (!present)
                    {
                        Log.Line("Folder [" + index + "] not present on cloud [" + cloudIndex + "] - will  upload");
                        msg = "NO";
                    }
                    else
                    {
                        msg = "OK" ;
                        Log.Line("Folder [" + index + "] present on cloud [" + cloudIndex + "] OK");

                    }

                    if (!fld.changed) 
                            msg += "+"; else msg += "-";

                    if (index < foldersList.Items.Count) {
                        ListViewItem item = foldersList.Items[index];

                        // Log.Line( "subitems: "  + item.SubItems.Count);

                        if((3 + cloudIndex) < item.SubItems.Count)
                            item.SubItems[3 + cloudIndex].Text = msg;

                    }  else
                    {
                        Log.Line("~ index is "+index+", count is " + foldersList.Items.Count);
                    }

                } // for cloudIndex
            } // for index
        }
        private void autoDetectKeyFolderRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            onKeyStorageRadiobuttonClicked();

        }

        void saveChanges()
        {
            Log.Line("save changes to ini file");
            File.Delete(INI_FILENAME);

            IniFile ini = new IniFile(IniFileName);
            monitoredFoldersContainer.save(ini);
            cloudFolderContainer.save(ini);
            ini.Write(TEMP_DIR_KEY, CryptoOne.tempFolder);
            //ini.Write(ROOT_DIR_KEY, Form1.localFolderRoot);

            if (this.setKeyFolderRadioButton.Checked)
            {
                keyStorageContainer.setKeyFolder(this.keyFolderEdit.Text);
            }     
            keyStorageContainer.save(ini);
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
                onDataChanged();
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
                Log.Line("Nothing selected");
                return;
            }

            if (cloudsList.SelectedItems.Count > 1)
            {
                Log.Line("Select only one row to edit");
                return;
            }
      
            ListViewItem item = cloudsList.SelectedItems[0];
            Log.Line("edit cloud #" + item.SubItems[0].Text);

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
                onDataChanged();
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
            string keyPath = "";
            if (keyFolderEdit.Enabled)
            {
                keyPath = keyFolderEdit.Text;
            }
            else
            {   // auto-detect
                keyPath = Tools.getKeyFolder();
                if(keyPath==null || keyPath.Length == 0)
                {
                    return "Key file not found";
                }
            }

            string keyFileName = keyPath + KEY_FILENAME;
            // Debug.WriteLine("keyFileName = " + keyFileName);

            if (keyPath != null && keyPath.Length > 0 && File.Exists(keyFileName))
            {
                return "Key file found at [" + keyFileName + "]";
            }
                      
            string msg = "Key file not found";
            if (keyPath!=null && keyPath.Length > 0)
            {
                msg += " at [" + keyFileName + "]";
            }
            return msg;            
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

            updateRemovableListFromOtherThread();
        }

        private  void DeviceInsertedEvent(object sender, EventArrivedEventArgs e)
        {
           Log.Line("DeviceInsertedEvent");
            /*ManagementBaseObject instance = (ManagementBaseObject)e.NewEvent["TargetInstance"];
             foreach (var property in instance.Properties)
             {
                // Debug.WriteLine(property.Name + " = " + property.Value);
             }*/
            updateKeyStatusFromOtherThread();
        }

        private  void DeviceRemovedEvent(object sender, EventArrivedEventArgs e)
        {
           Log.Line("DeviceRemovedEvent");

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

                onDataChanged();
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
                    onDataChanged();
                }
                monitoredFoldersContainer.show(foldersList);
            }
            else
            {
                Log.Line("Nothing selected");
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
            if (folder == null)
                return false;

            if (cloudIndex < 0 || cloudIndex >= cloudFolderContainer.getCount())
            {
                return false;
            }

            string filename = folder.getCloudFileName();
            string cloudFileName = cloudFolderContainer.get(cloudIndex).fullPath +
                Form1.cloudStorageFolder + "\\" + filename;
            return File.Exists(cloudFileName);
        }

        private void setKeyFolderRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            onKeyStorageRadiobuttonClicked();
        }

        void onKeyStorageRadiobuttonClicked()
        {
            if (autoDetectKeyFolderRadioButton.Checked)
            {
                keyFolderEdit.Enabled = false;
                keyStorageContainer.setAutodetectKeyStorage(true, "");
                this.browseKeyLocationButton.Enabled = false;
            }
            else
            {
                keyStorageContainer.setAutodetectKeyStorage(false, keyFolderEdit.Text);
                keyFolderEdit.Enabled = true;
                this.browseKeyLocationButton.Enabled = true;
            }
            onDataChanged();
        }

        private void removableList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.removableList.SelectedItems.Count==1)
            {
                ListViewItem selected = this.removableList.SelectedItems[0];
                string drive = selected.SubItems[0].Text;
                string status = selected.SubItems[1].Text;
                Log.Line("[" + drive +"] "+status);

                if(STATUS_PRESENT.Equals(status))
                {
                    initButton.Enabled = false;
                    useButton.Enabled = true;
                }
                else
                {
                    initButton.Enabled = true;
                    useButton.Enabled = false;
                }
            }
        }

        private void initButton_Click(object sender, EventArgs e)
        {
            PassForm form = new PassForm();
            DialogResult result = form.ShowDialog();
            if(result == DialogResult.OK)
            {
                // generate and store key 
                ListViewItem selected = this.removableList.SelectedItems[0];
                string drive = selected.SubItems[0].Text;
                cryptoOne.generateEncryptedKeyFile(drive, form.password);
                // update UI
                updateRemovableListFromOtherThread();
            }
        }

        private void useButton_Click(object sender, EventArgs e)
        {

        }

        private void foldersList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(foldersList.SelectedItems.Count==1)
            {
                this.pushButton.Enabled = true;
            } else
            {
                this.pushButton.Enabled = false;
            }
        }

        private void pushButton_Click(object sender, EventArgs e)
        {
            string keyFolder = Tools.getKeyFolder();
            if (keyFolder == null || keyFolder.Length == 0)
            {
                Log.Line("keyfolder not found");
                return;
            }
            Log.Line("keyFolder = " + keyFolder);

            ListViewItem item = foldersList.SelectedItems[0];
            Log.Line("push " + item.SubItems[1].Text);

            for (int cloudIndex = 0; cloudIndex < cloudFolderContainer.getCount(); cloudIndex++)
            {
                cryptoOne.push(monitoredFoldersContainer.get(item.Index), cloudFolderContainer.get(cloudIndex), keyFolder + Form1.KEY_FILENAME);
            }
        }
        private void removeCloudButton_Click(object sender, EventArgs e)
        {
            if (cloudsList.SelectedItems.Count > 0)
            {
                int deletedCount = 0;
                for (int i = 0; i < cloudsList.SelectedItems.Count; i++)
                {
                    ListViewItem item = cloudsList.SelectedItems[i];
                    Log.Line("deleting cloud folder #" + item.SubItems[0].Text);

                    int index = int.Parse(item.SubItems[0].Text);
                    cloudFolderContainer.remove(index - deletedCount);

                    deletedCount++;
                    onDataChanged();
                }
                cloudFolderContainer.show(cloudsList);
            }
            else
            {
                Log.Line("Nothing selected");
            }
        }

        private void browseTempFolderButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                tempFolderEdit.Text = folderBrowserDialog1.SelectedPath;
                CryptoOne.tempFolder = tempFolderEdit.Text;
                onDataChanged();
            }
        }
        private void refreshButton_Click(object sender, EventArgs e)
        {
            updateFolderStatus();
        }

        private void getButton_Click(object sender, EventArgs e)
        {
            string cloudIndex = cloudsList.SelectedItems[0].SubItems[0].Text;

            string keyFolder = Tools.getKeyFolder();
            if (keyFolder == null || keyFolder.Length == 0)
            {
                Log.Line("keyfolder not found");
                return;
            }
            //Log.Line("keyFolder = " + keyFolder);

            RemoteFolder form = new RemoteFolder(
                this.cloudFolderContainer.get(int.Parse(cloudIndex)), 
                cryptoOne, 
                keyFolder + Form1.KEY_FILENAME,
                monitoredFoldersContainer);
            form.ShowDialog();
        }

        private void cloudsList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if( cloudsList.SelectedItems.Count==1)
            {
                getButton.Enabled = true;
            } else
            {
                getButton.Enabled = false;

            }
        }

        private void helpButton_Click(object sender, EventArgs e)
        {
            //helpProvider1.
        }

        public void onDataChanged()
        {
            Log.Line("onDataChanged");
            this.applyButton.Enabled = true;
        }

        private void browseKeyLocationButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                keyFolderEdit.Text = folderBrowserDialog1.SelectedPath;
                onDataChanged();
            }
        }

        private void logBox_TextChanged(object sender, EventArgs e)
        {
            logBox.SelectionStart = logBox.Text.Length;
            logBox.ScrollToCaret();
        }

        private void clearLogButton_Click(object sender, EventArgs e)
        {
            this.logBox.Text = "";
        }

        /* private void browseRootFolderButton_Click(object sender, EventArgs e)
         {
             FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
             DialogResult res = folderBrowserDialog1.ShowDialog();
             if (res == DialogResult.OK)
             {
                 localFolderRootEdit.Text = folderBrowserDialog1.SelectedPath;
                 Form1.localFolderRoot = localFolderRootEdit.Text;

                 onDataChanged();
             }
         }*/
    }
}
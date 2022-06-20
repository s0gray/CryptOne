using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace CryptOneService
{
    public class MonitoredFolder
    {
        public string path = "";

        FileSystemWatcher fileSystemWatcher;
        Form1 mainForm;

        public MonitoredFolder(string p, Form1 form)
        {
            this.path = p;
            this.mainForm = form;

            this.fileSystemWatcher = new FileSystemWatcher(p);
            this.fileSystemWatcher.EnableRaisingEvents = true;
            this.fileSystemWatcher.NotifyFilter = System.IO.NotifyFilters.LastWrite; //LastAccess;
            this.fileSystemWatcher.SynchronizingObject = mainForm;
            this.fileSystemWatcher.Changed += new System.IO.FileSystemEventHandler(this.fileSystemWatcher_Changed);
        }

        internal string getArchiveFileName()
        {
            string lastFolderName = Path.GetFileName(path);// Path.GetDirectoryName(path));

            return lastFolderName;
        }

        private void fileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
        {
            Log.Line("File changed in folder ["+this.path+"]");
        }

        // in which file this folder will be stored in cloud
        public string getCloudFileName()
        {      
            return getArchiveFileName() + ".tgz.enc"; 
        }

    }
}

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

        private void fileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
        {
            Debug.WriteLine("File changed in folder ["+this.path+"]");
        }

        // in which file this folder will be stored in cloud
        public string getCloudFileName()
        {
            string hashHex = Crypto.hashSHA256hex(path);

            return hashHex + ".tar.gz.enc";  // = tar.gz.enc
        }

    }
}

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class MonitoredFoldersContainer
    {
        List<MonitoredFolder> monitoredFolders = new List<MonitoredFolder>();
        Form1 mainForm;
        public MonitoredFoldersContainer(Form1 form)
        {
            this.mainForm = form;
        }

        /**
         * @return number of loaded monitored folders
         */
        public int load(IniFile ini)
        {
            int index = 0;
            monitoredFolders.Clear();
            while (true)
            {
                string key = Form1.MONITOREDFOLDER_KEY + index;
                var monitoredFolder = ini.Read(key);

                if (monitoredFolder != null && monitoredFolder.Length > 0)
                {
                    Debug.WriteLine("monitoredFolder [" + index + "] = [" + monitoredFolder + "]");

                    monitoredFolders.Add(new MonitoredFolder(monitoredFolder, mainForm));
                }
                else
                {
                    break;
                }
                index++;
            } // while (true)
            return index;
        }

        public void show(ListView foldersList)
        {
            foldersList.Items.Clear();

            for(int i=0;i<monitoredFolders.Count;i++)
            {
                string[] arr2 = new string[4];
                arr2[0] = "" + i;
                arr2[1] = monitoredFolders[i].path;
                arr2[2] = mainForm.getFolderStatus(monitoredFolders[i]);
                arr2[3] = monitoredFolders[i].getCloudFileName();
                foldersList.Items.Add(new ListViewItem(arr2));
            }
        }

        public void add(string selectedPath)
        {
            monitoredFolders.Add(new MonitoredFolder(selectedPath, mainForm));
        }

        public void remove(int index)
        {
            if (index >= 0 && index < monitoredFolders.Count)
            {
                monitoredFolders.RemoveAt(index);
            }
            else
            {
                Debug.WriteLine("Index out of bounds: " + index);
            }
        }

        public void save(IniFile ini)
        {
            for(int i = 0; i < monitoredFolders.Count; i++)
            {
                string key = Form1.MONITOREDFOLDER_KEY + i;
                ini.Write(key, monitoredFolders[i].path, Form1.SECTION);
            }
        }

        public int getCount()
        {
            return this.monitoredFolders.Count;
        }

        public MonitoredFolder get(int folderIndex)
        {
            if (folderIndex < 0 || folderIndex >= getCount())
                return null;

            return monitoredFolders[folderIndex];
        }
    }
}

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
                    Log.Line("monitoredFolder [" + index + "] = [" + monitoredFolder + "]");

                    if (Directory.Exists(monitoredFolder))
                    {
                        monitoredFolders.Add(new MonitoredFolder(monitoredFolder, mainForm));
                    } else
                    {
                        Log.Line("Folder ["+ monitoredFolder + "] not exists");
                    }
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
                foldersList.Items.Add(makeFolderListViewItem(i, foldersList.Columns.Count));
            }
        }

        public void updateUI()
        {
            show(mainForm.foldersList);
            mainForm.applyButton.Enabled = true;
        }

        ListViewItem makeFolderListViewItem(int i, int columns)
        {
            string[] arr2 = new string[columns];
            arr2[0] = "" + i;
            arr2[1] = monitoredFolders[i].path;
            arr2[2] = monitoredFolders[i].getCloudFileName();

            for(int j=3; j < columns; j++)
            {
                arr2[j] = "";
            }
            return new ListViewItem(arr2);
        }

        public void add(string selectedPath)
        {
            Log.Line("monitoredFolders add: ["+ selectedPath + "]");
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
                Log.Line("Index out of bounds: " + index);
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

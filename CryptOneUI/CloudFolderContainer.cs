using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class CloudFolderContainer
    {
        List<CloudFolder> cloudFolders = new List<CloudFolder>();

        public CloudFolderContainer()
        {

        }

        public void load(IniFile ini)
        {
            int index = 0;
            cloudFolders.Clear();
            while (true)
            {
                string key = Form1.CLOUDFOLDER_KEY + index;
                string descriptionKey = Form1.CLOUDDESCRIPTION_KEY + index;

                var cloudFolder = ini.Read(key);
                var cloudFolderDescription = ini.Read(descriptionKey);

                if (cloudFolder != null && cloudFolder.Length > 0)
                {
                    Log.Line("cloudFolder [" + index + "] = [" + cloudFolder + "]");

                    cloudFolders.Add(new CloudFolder(index, cloudFolder, cloudFolderDescription, Tools.decodeFolder(cloudFolder)));
                }
                else
                {
                    break;
                }
                index++;
            } // while (true)
        }

        internal void show(ListView view)
        {
            view.Items.Clear();

            for (int i = 0; i < cloudFolders.Count; i++)
            {
                string[] arr1 = new string[4];
                arr1[0] = "" + cloudFolders[i].index;
                arr1[1] = cloudFolders[i].folder;
                arr1[2] = cloudFolders[i].description;
                arr1[3] = cloudFolders[i].fullPath;

                view.Items.Add(new ListViewItem(arr1));
            }
        }

        public void remove(int index)
        {
            if (index >= 0 && index < cloudFolders.Count)
            {
                cloudFolders.RemoveAt(index);
            }
            else
            {
                Log.Line("Index out of bounds: " + index);
            }
        }

        public void save(IniFile ini)
        {
            for (int i = 0; i < cloudFolders.Count; i++)
            {
                string key = Form1.CLOUDFOLDER_KEY + i;
                ini.Write(key, cloudFolders[i].folder, Form1.SECTION);

                string descriptionKey = Form1.CLOUDDESCRIPTION_KEY + i;
                ini.Write(descriptionKey, cloudFolders[i].description, Form1.SECTION);
            }
        }

        public void add(string folder, string description, string fullPath)
        {
            this.cloudFolders.Add(new CloudFolder(folder, description, fullPath, this.cloudFolders.Count));
        }

        public int getCount()
        {
            return this.cloudFolders.Count;
        }

        public CloudFolder get(int cloudIndex)
        {
            return cloudFolders[cloudIndex];
        }

        // create folders 'CryptOne' on clouds if not present
        public void initCloudStorages()
        {
            for(int i = 0; i < cloudFolders.Count; i++)
            {
                CloudFolder cloudFolder = cloudFolders[i];
                cloudFolder.initStorage();
            }
        }
    }
}

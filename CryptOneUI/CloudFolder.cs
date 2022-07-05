using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class CloudFolder
    {
        public string folder;
        public string description;
        public string fullPath;
        public int index;


        public CloudFolder(string folder, string description, string fullPath, int index)
        {
            this.folder = folder;
            this.description = description;

            if (!fullPath.EndsWith('\\'))
                fullPath += "\\";
            this.fullPath = fullPath;
            this.index = index;
        }

        public CloudFolder(int index, string cloudFolder, string cloudFolderDescription, string fullPath)
        {
            this.index = index;
            this.folder = cloudFolder;
            this.description = cloudFolderDescription;
            if (!fullPath.EndsWith('\\'))
                fullPath += "\\";
            this.fullPath = fullPath;

        }

        /**
         * return 'true' if OK
         * */
        public bool initStorage()
        {
            string cloudStorageFolder = fullPath + Form1.cloudStorageFolder;
            if ( !Directory.Exists(cloudStorageFolder) )
            {
                Log.Line("Creating cloud storage folder [" + cloudStorageFolder + "]");
                try
                {
                    Directory.CreateDirectory(cloudStorageFolder);
                } catch (System.IO.DirectoryNotFoundException e) {
                    Log.Line("Error during CreateDirectory: "+ e.Message);
                    return false;
                }
            }
            return true;
        }
    }
}

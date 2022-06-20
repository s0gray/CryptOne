using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSharpCode.SharpZipLib.GZip;
using ICSharpCode.SharpZipLib.Tar;
using ICSharpCode.SharpZipLib.Zip;

using System.IO;
using System.Diagnostics;

namespace CryptOneService
{
    public class Tools
    {
        public static string[] getAvailableDrives()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();
            string[] result = new string[drives.Length];
            for(int i=0; i<drives.Length;i++)
            {
                result[i] = drives[i].Name;
            }

            return result;
        }

        public static string[] getRemovableDrives()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();
            List<string> result = new List<string>();
            for (int i = 0; i < drives.Length; i++)
            {
                if (drives[i].DriveType == DriveType.Removable)
                   result.Add( drives[i].Name );
            }

            return result.ToArray();
        }

        public static string decodeFolder(string folder)
        {
            return folder.Replace("~", Form1.homeDir);
        }

        public static string getKeyFolder()
        {
            string[] removable = Tools.getRemovableDrives();
            if(removable== null || removable.Length == 0)
            {
                return "";
            }

            for(int i=0; i< removable.Length; i++)
            {
                if (isKeyFileExistOnRemovableDrive(removable[i]))
                    return removable[i];
            }
            return "";
        }

        public static bool isKeyFileExistOnRemovableDrive(string drive)
        {
             string keyFileName = drive + Form1.KEY_FILENAME;
             return File.Exists(keyFileName);
        }

        public static void WriteFile(byte[] data, string filename)
        {
            File.WriteAllBytes(filename, data);
        }

        public static string CreateTGZ(string sourceDirectory, string tgzFileName, string targetDirectory, bool deleteSourceDirectoryUponCompletion = false)
        {
            if (!tgzFileName.EndsWith(".tgz"))
            {
                tgzFileName = tgzFileName + ".tgz";
            }
            using (var outStream = File.Create(Path.Combine(targetDirectory, tgzFileName)))
            using (var gzoStream = new GZipOutputStream(outStream))
            {
                var tarArchive = TarArchive.CreateOutputTarArchive(gzoStream, Encoding.UTF8);

                // Note that the RootPath is currently case sensitive and must be forward slashes e.g. "c:/temp"
                // and must not end with a slash, otherwise cuts off first char of filename
                tarArchive.RootPath = sourceDirectory.Replace('\\', '/');
                if (tarArchive.RootPath.EndsWith("/"))
                {
                    tarArchive.RootPath = tarArchive.RootPath.Remove(tarArchive.RootPath.Length - 1);
                }
                AddDirectoryFilesToTGZ(tarArchive, sourceDirectory);

                if (deleteSourceDirectoryUponCompletion)
                {
                    File.Delete(sourceDirectory);
                }
                var tgzPath = (tarArchive.RootPath + ".tgz").Replace('/', '\\');

                tarArchive.Close();
                return targetDirectory + Path.GetFileName( tgzPath );
            }
        }

        private static void AddDirectoryFilesToTGZ(TarArchive tarArchive, string sourceDirectory)
        {
            AddDirectoryFilesToTGZ(tarArchive, sourceDirectory, string.Empty);
        }

        private static void AddDirectoryFilesToTGZ(TarArchive tarArchive, string sourceDirectory, string currentDirectory)
        {
        //    Debug.WriteLine("AddDirectoryFilesToTGZ src="+ sourceDirectory+" cur="+ currentDirectory);
            var pathToCurrentDirectory = Path.Combine(sourceDirectory, currentDirectory);

            // Write each file to the tgz.
            var filePaths = Directory.GetFiles(pathToCurrentDirectory);
            foreach (string filePath in filePaths)
            {
                var tarEntry = TarEntry.CreateEntryFromFile(filePath);

                // Name sets where the file is written. Write it in the same spot it exists in the source directory
                tarEntry.Name = filePath.Replace(sourceDirectory, "");

                // If the Name starts with '\' then an extra folder (with a blank name) will be created, we don't want that.
                if (tarEntry.Name.StartsWith('\\'))
                {
                    tarEntry.Name = tarEntry.Name.Substring(1);
                }
                tarArchive.WriteEntry(tarEntry, true);
            }

            // Write directories to tgz
            var directories = Directory.GetDirectories(pathToCurrentDirectory);
            foreach (string directory in directories)
            {
                AddDirectoryFilesToTGZ(tarArchive, sourceDirectory, directory);
            }
        }

        public static byte[] concat(byte[] x, byte[] y)
        {
            var z = new byte[x.Length + y.Length];
            x.CopyTo(z, 0);
            y.CopyTo(z, x.Length);
            return z;
        }

        public static string ShowDialog(string text, string caption)
        {
            Form prompt = new Form()
            {
                Width = 500,
                Height = 150,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                Text = caption,
                StartPosition = FormStartPosition.CenterScreen
            };
            Label textLabel = new Label() { Left = 50, Top = 20, Text = text };
            TextBox textBox = new TextBox() { Left = 50, Top = 50, Width = 400 };
            Button confirmation = new Button() { Text = "Ok", Left = 350, Width = 100, Top = 70, DialogResult = DialogResult.OK };
            confirmation.Click += (sender, e) => { prompt.Close(); };
            prompt.Controls.Add(textBox);
            prompt.Controls.Add(confirmation);
            prompt.Controls.Add(textLabel);
            prompt.AcceptButton = confirmation;

            return prompt.ShowDialog() == DialogResult.OK ? textBox.Text : "";
        }

        public static string AskPassword()
        {
            PassForm2 form = new PassForm2();
            DialogResult result = form.ShowDialog();
            if (result == DialogResult.OK)
            {
                return form.password;
            }
            return "";
        }

        // copy file and overwrite if needed
        public static void copyFile(string from, string to)
        {
            Log.Line("COPY ["+from+"] -> ["+to+"]");
            if(File.Exists(to))
            {
                File.Delete(to);
            }
            File.Copy(from, to);

        }
    }
}

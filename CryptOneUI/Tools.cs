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
using System.IO.Compression;

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

            for(int i=0; i < removable.Length; i++)
            {
                if (isKeyFileExistOnRemovableDrive(removable[i]))
                    return removable[i];
            }
            return "";
        }

        public static void checkAndCreateIniFiles()
        {
            if(!Directory.Exists(Form1.appDataFolder))
            {
                Directory.CreateDirectory(Form1.appDataFolder);
                Log.Line("Creating app folder " + Form1.appDataFolder);
            }

            if (!Directory.Exists(Form1.localFolderRoot))
            {
                Directory.CreateDirectory(Form1.localFolderRoot);
                Log.Line("Creating local root " + Form1.localFolderRoot);
            }      
        }

        public static bool isKeyFileExistOnRemovableDrive(string drive)
        {
             string keyFileName = drive + Form1.KEY_FILENAME;
             return File.Exists(keyFileName);
        }

        public static void createInfoFile(string filename, Dictionary<string, string> info)
        {
            Log.Line("Creating info file at "+filename);

            if(File.Exists(filename))
            {
                File.Delete(filename);
            }

            IniFile file = new IniFile(filename);
            foreach (KeyValuePair<string, string> entry in info)
            {
                file.Write(entry.Key, entry.Value);
            }
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

        public static void UncompressTGZ(string filename)
        {
            string outputDir = Path.GetDirectoryName(filename);
            ExtractTarGz(filename, outputDir);

         /*   Stream inStream = new MemoryStream(byteArray);
            Stream gzipStream = new GZipInputStream(inStream);

            TarArchive tarArchive = TarArchive.CreateInputTarArchive(gzipStream);
            tarArchive.ExtractContents(@".");
            tarArchive.Close();

            gzipStream.Close();
            inStream.Close();*/

            /*     Stream inStream = new MemoryStream(byteArray);
                 Stream gzipStream = new GZipInputStream(inStream);

                 using (var tarInputStream = new TarInputStream(gzipStream))
                 {
                     TarEntry entry;
                     while ((entry = tarInputStream.GetNextEntry()) != null)
                     {
                         var fileName = entry.File;
                         using (var fileContents = new MemoryStream())
                         {
                             tarInputStream.CopyEntryContents(fileContents);

                             // use entry, fileName or fileContents here
                         }
                     }
                 }*/
        }

        public static void ExtractTarGz(string filename, string outputDir)
        {
            using (var stream = File.OpenRead(filename))
                ExtractTarGz(stream, outputDir);
        }

        public static void ExtractTarGz(Stream stream, string outputDir)
        {
            // A GZipStream is not seekable, so copy it first to a MemoryStream
            using (var gzip = new GZipStream(stream, CompressionMode.Decompress))
            {
                const int chunk = 4096;
                using (var memStr = new MemoryStream())
                {
                    int read;
                    var buffer = new byte[chunk];
                    do
                    {
                        read = gzip.Read(buffer, 0, chunk);
                        memStr.Write(buffer, 0, read);
                    } while (read == chunk);

                    memStr.Seek(0, SeekOrigin.Begin);
                    ExtractTarGz(memStr, outputDir);
                }
            }
        }

        public static void ExtractTar(string filename, string outputDir)
        {
            using (var stream = File.OpenRead(filename))
                ExtractTarGz(stream, outputDir);
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

        // do not copy if target hash is same as source
        public static void smartCopyFile(string src, string target)
        {
            string srcHash = Tools.calculateFileHash(src);
            smartCopyFile(src, srcHash, target);
        }
        public static void smartCopyFile(string src, string srcHash, string target)
        {
            Log.Line("smartCopyFile "+src+" -> "+target);
            string dstHash = Tools.calculateFileHash(target);

            if (srcHash != null && dstHash != null && srcHash.Equals(dstHash))
            {
                Log.Line("target file is same as source - do not copy");
                return;
            }
            copyFile(src, target);
        }
        public static string calculateFolderHash(string path)
        {
            string []files = getFilesRecursively(path);
            if (files == null || files.Length == 0)
                return "";

            string result = "";
            for(int i = 0; i<files.Length; i++)
            {
                string hash = Tools.calculateFileHash(files[i]);
                result += hash;
            }

//            Log.Line("folder hash material " + result);
            byte[] sha256 = Crypto.hashSHA256(result);

            return Tools.bytesToHex(sha256);
        }

        // return hex string
        public static string calculateFileHash(string path)
        {
            return Tools.bytesToHex(calculateFileHashBytes(path));
        }

        public static byte[] calculateFileHashBytes(string path)
        {
            byte[] content = null;
            if (!File.Exists(path))
            {
                if (Directory.Exists(path))
                {   // for empty folders take full name as content
                    content = Encoding.ASCII.GetBytes(Path.GetFullPath(path));
                }
            }
            else
            {
                content = File.ReadAllBytes(path);
            }

            if (content == null)
                return null;

            return Crypto.hashSHA256bytes(content);
        }

        public static string[] getFilesRecursively(string path)
        {
            List<string> allFiles = new List<string>();
            string[] files = Directory.GetFiles(path);
            string[] dirs = Directory.GetDirectories(path);

            if (dirs == null || dirs.Length == 0)
                return files;

            allFiles.AddRange(files);

            for(int i = 0; i < dirs.Length; i++)
            {
                allFiles.Add(dirs[i]);
                allFiles.AddRange ( getFilesRecursively(dirs[i]) );
            }
            return allFiles.ToArray();
        }

        public static string bytesToHex(byte[] data)
        {
            if (data == null || data.Length == 0) return "";
            return BitConverter.ToString(data).Replace("-", string.Empty);
        }

        // tested
        public static bool decompressTgzWithShell(string file, string outputFolder)
        {
            if(!File.Exists(file))
            {
                Log.Line("Decompress: File ["+file+"] not exist");
                return false;
            }
            long size = new FileInfo(file).Length;
            long startTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            execShell("tar xf " +file+" -C "+outputFolder);
            long endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();

            long deltaMs = (endTime - startTime);
            double speed = 0;
            if(deltaMs > 0)
            {
                speed = size / deltaMs;
            }

            Log.Line("Decompressed TGZ: [" + file + "] for " + deltaMs / 1000 
                + "s "+ speed + " bytes/ms");
            return true;
        }

        
        public static bool compressTgzWithShell(string compressedFile, string folderFullPath)
        {
            if (folderFullPath==null || !Directory.Exists(folderFullPath))
            {
                Log.Line("Compress: Folder [" + folderFullPath + "] not exist");
                return false;
            }

            string basePath = Directory.GetParent(folderFullPath).FullName;
            string folder = Path.GetFileName(folderFullPath);

            long startTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
//            execShell("tar -czf " + compressedFile + " " + folder + " -C " + basePath);
            execShell("tar -czf " + compressedFile + " " + folder, basePath);

            long endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();

            long deltaMs = (endTime - startTime);

            Log.Line("Compressed TGZ: [" + folder + "] to ["+ compressedFile + "] for " + deltaMs / 1000
                + "s ");
            return true;
        }

        public static void execShell(string cmd, string workingDirectory = null)
        {
            Log.Line("execShell ["+cmd+"]");

            Process process = new Process();
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.FileName = "cmd.exe";
            startInfo.Arguments = "/C \"" + cmd + "\"";
            if (workingDirectory != null && workingDirectory.Length > 0)
            {
                startInfo.WorkingDirectory = workingDirectory;
            }
            process.StartInfo = startInfo;
            process.Start();
        }
    }
}

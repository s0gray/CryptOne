using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class KeyStorage
    {
        public string keyFolder = "";

        public KeyStorage()
        {

        }

        public KeyStorage(string keyFolder)
        {
            this.keyFolder = keyFolder;
        }
    }
}

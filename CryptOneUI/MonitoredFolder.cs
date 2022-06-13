using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class MonitoredFolder
    {
        public string path = "";

        public MonitoredFolder()
        {
        }

        public MonitoredFolder(string p)
        {
            this.path = p;
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CryptOneService
{
    public partial class PassForm2 : Form
    {
        public string password = "";
        public PassForm2()
        {
            InitializeComponent();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.password = passEdit.Text;
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {

        }
    }
}

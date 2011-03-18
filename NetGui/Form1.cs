using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NetDll;

namespace NetGui
{
    public partial class Form1 : Form
    {
        private CapsToGWBasicAscii dll;

        private Icon capsIcon;
        private Boolean gwKeysOn;
        private Icon gwIcon;

        public Form1()
        {
            InitializeComponent();
            this.Visible = false;

            dll = new CapsToGWBasicAscii();
            gwKeysOn = false;
            
            capsIcon = new Icon(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("NetGui.NormalCapsNotify.ico"));
            gwIcon = new Icon(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("NetGui.gwNotify.ico"));

            notifyIcon.Icon = capsIcon;
            //notifyIcon.Icon = gwIcon;
            notifyIcon.Visible = true;
        }       

        private void toggleKeys_Click(object sender, EventArgs e)
        {
            if (!gwKeysOn)
            {
                dll.Install();
            }
            else
            {
                dll.Uninstall();
            }
            gwKeysOn = !gwKeysOn;
            notifyIcon.Icon = gwKeysOn ? gwIcon : capsIcon;
            toggleKeys.Text = gwKeysOn ? "Disable" : "Enable";
        }

        private void exitApp_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}

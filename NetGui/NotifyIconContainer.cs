using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;
using NetDll;

namespace NetGui
{

class NotifyIconContainer 
{
    private Icon capsIcon;
    private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
    private GwKeysDll gwKeys;
    private System.Windows.Forms.ToolStripMenuItem exitApp;
    
    private Icon gwIcon;
    private System.Windows.Forms.NotifyIcon notifyIcon;
    private System.Windows.Forms.ToolStripMenuItem toggleKeys;
    private bool quit;
    
    public NotifyIconContainer()
    {
        this.contextMenuStrip = new ContextMenuStrip();
        this.exitApp = new ToolStripMenuItem();
        this.toggleKeys = new ToolStripMenuItem();
        this.notifyIcon = new NotifyIcon();

        this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toggleKeys,
            this.exitApp});
        this.contextMenuStrip.Name = "contextMenuStrip1";
        this.contextMenuStrip.Size = new System.Drawing.Size(153, 70);

        this.toggleKeys.Name = "toggleKeys";
        this.toggleKeys.Size = new System.Drawing.Size(152, 22);
        this.toggleKeys.Text = "Enable";
        this.toggleKeys.Click += new System.EventHandler(this.toggleKeys_Click);

        this.exitApp.Name = "exitApp";
        this.exitApp.Size = new System.Drawing.Size(152, 22);
        this.exitApp.Text = "Exit";
        this.exitApp.Click += new System.EventHandler(this.exitApp_Click);

        this.notifyIcon.ContextMenuStrip = this.contextMenuStrip;
        this.notifyIcon.Text = "GwKeys";
        this.notifyIcon.Visible = true;

        gwKeys = new GwKeysDll();
        
        capsIcon = new Icon(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("NetGui.NormalCapsNotify.ico"));
        gwIcon = new Icon(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("NetGui.gwNotify.ico"));

        notifyIcon.Icon = capsIcon;
        //notifyIcon.Icon = gwIcon;
        notifyIcon.Visible = true;

        quit = false;
    }

    public void Destroy()
    {
        gwKeys.On = false;
        notifyIcon.Visible = false;        
    }

    public void Init()
    {
        gwKeys.On = true;
        Refresh();
    }

    public void Refresh()
    {
        notifyIcon.Icon = gwKeys.On ? gwIcon : capsIcon;
        toggleKeys.Text = gwKeys.On ? "Disable" : "Enable";
    }

    private void toggleKeys_Click(object sender, EventArgs e)
    {
        gwKeys.toggle();
        Refresh();
    }

    private void exitApp_Click(object sender, EventArgs e)
    {
        quit = true;
    }

    public bool Quit
    {
        get
        {
            return quit;
        }
    }
}

}

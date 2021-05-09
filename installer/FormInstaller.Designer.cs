
namespace installer
{
    partial class FormInstaller
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBoxApply4GBPatch = new System.Windows.Forms.CheckBox();
            this.checkBoxModKitRegistryFix = new System.Windows.Forms.CheckBox();
            this.textBoxGamePath = new System.Windows.Forms.TextBox();
            this.buttonBrowsePath = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonInstall = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.linkLabelDiscord = new System.Windows.Forms.LinkLabel();
            this.linkLabelReddit = new System.Windows.Forms.LinkLabel();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.checkBoxApply4GBPatch);
            this.groupBox1.Controls.Add(this.checkBoxModKitRegistryFix);
            this.groupBox1.Location = new System.Drawing.Point(12, 101);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(530, 139);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Options";
            // 
            // label2
            // 
            this.label2.Enabled = false;
            this.label2.Location = new System.Drawing.Point(22, 99);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(456, 35);
            this.label2.TabIndex = 4;
            this.label2.Text = "Sets a registry key to allow the ModKit and WicEd to be installed properly. This " +
    "is required if you use the Steam or GOG versions of the game.";
            // 
            // label1
            // 
            this.label1.Enabled = false;
            this.label1.Location = new System.Drawing.Point(22, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(456, 34);
            this.label1.TabIndex = 3;
            this.label1.Text = "Allows the game executable to utilize 4GB of virtual memory. This is restricted t" +
    "o 2GB by default. Enable this if your game crashes because of out-of-memory cond" +
    "itions.";
            // 
            // checkBoxApply4GBPatch
            // 
            this.checkBoxApply4GBPatch.AutoSize = true;
            this.checkBoxApply4GBPatch.Checked = true;
            this.checkBoxApply4GBPatch.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxApply4GBPatch.Location = new System.Drawing.Point(6, 22);
            this.checkBoxApply4GBPatch.Name = "checkBoxApply4GBPatch";
            this.checkBoxApply4GBPatch.Size = new System.Drawing.Size(173, 19);
            this.checkBoxApply4GBPatch.TabIndex = 2;
            this.checkBoxApply4GBPatch.Text = "Apply 4GB executable patch";
            this.checkBoxApply4GBPatch.UseVisualStyleBackColor = true;
            // 
            // checkBoxModKitRegistryFix
            // 
            this.checkBoxModKitRegistryFix.AutoSize = true;
            this.checkBoxModKitRegistryFix.Location = new System.Drawing.Point(6, 79);
            this.checkBoxModKitRegistryFix.Name = "checkBoxModKitRegistryFix";
            this.checkBoxModKitRegistryFix.Size = new System.Drawing.Size(171, 19);
            this.checkBoxModKitRegistryFix.TabIndex = 0;
            this.checkBoxModKitRegistryFix.Text = "Apply WicEd registry key fix";
            this.checkBoxModKitRegistryFix.UseVisualStyleBackColor = true;
            // 
            // textBoxGamePath
            // 
            this.textBoxGamePath.Location = new System.Drawing.Point(6, 22);
            this.textBoxGamePath.Name = "textBoxGamePath";
            this.textBoxGamePath.Size = new System.Drawing.Size(518, 23);
            this.textBoxGamePath.TabIndex = 1;
            // 
            // buttonBrowsePath
            // 
            this.buttonBrowsePath.Location = new System.Drawing.Point(6, 51);
            this.buttonBrowsePath.Name = "buttonBrowsePath";
            this.buttonBrowsePath.Size = new System.Drawing.Size(75, 23);
            this.buttonBrowsePath.TabIndex = 2;
            this.buttonBrowsePath.Text = "Browse";
            this.buttonBrowsePath.UseVisualStyleBackColor = true;
            this.buttonBrowsePath.Click += new System.EventHandler(this.buttonBrowsePath_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.textBoxGamePath);
            this.groupBox2.Controls.Add(this.buttonBrowsePath);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(530, 83);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Game Installation Path";
            // 
            // buttonInstall
            // 
            this.buttonInstall.Location = new System.Drawing.Point(12, 248);
            this.buttonInstall.Name = "buttonInstall";
            this.buttonInstall.Size = new System.Drawing.Size(75, 23);
            this.buttonInstall.TabIndex = 4;
            this.buttonInstall.Text = "Install";
            this.buttonInstall.UseVisualStyleBackColor = true;
            this.buttonInstall.Click += new System.EventHandler(this.buttonInstall_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(93, 248);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 5;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // linkLabelDiscord
            // 
            this.linkLabelDiscord.AutoSize = true;
            this.linkLabelDiscord.Location = new System.Drawing.Point(448, 256);
            this.linkLabelDiscord.Name = "linkLabelDiscord";
            this.linkLabelDiscord.Size = new System.Drawing.Size(47, 15);
            this.linkLabelDiscord.TabIndex = 6;
            this.linkLabelDiscord.TabStop = true;
            this.linkLabelDiscord.Text = "Discord";
            this.linkLabelDiscord.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelDiscord_LinkClicked);
            // 
            // linkLabelReddit
            // 
            this.linkLabelReddit.AutoSize = true;
            this.linkLabelReddit.Location = new System.Drawing.Point(501, 256);
            this.linkLabelReddit.Name = "linkLabelReddit";
            this.linkLabelReddit.Size = new System.Drawing.Size(41, 15);
            this.linkLabelReddit.TabIndex = 7;
            this.linkLabelReddit.TabStop = true;
            this.linkLabelReddit.Text = "Reddit";
            this.linkLabelReddit.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelReddit_LinkClicked);
            // 
            // FormInstaller
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(554, 283);
            this.Controls.Add(this.linkLabelReddit);
            this.Controls.Add(this.linkLabelDiscord);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonInstall);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormInstaller";
            this.Text = "World in Conflict Multiplayer Fix Installer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormInstaller_FormClosing);
            this.Load += new System.EventHandler(this.FormInstaller_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxApply4GBPatch;
        private System.Windows.Forms.CheckBox checkBoxModKitRegistryFix;
        private System.Windows.Forms.TextBox textBoxGamePath;
        private System.Windows.Forms.Button buttonBrowsePath;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonInstall;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.LinkLabel linkLabelDiscord;
        private System.Windows.Forms.LinkLabel linkLabelReddit;
    }
}


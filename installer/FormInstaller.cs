using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace installer
{
    public partial class FormInstaller : Form
    {
        public FormInstaller()
        {
            InitializeComponent();
        }

        private void FormInstaller_Load(object sender, EventArgs e)
        {
            string gamePath = TryGuessGamePath();

            if (!string.IsNullOrEmpty(gamePath))
                textBoxGamePath.Text = gamePath;
            else
                textBoxGamePath.Text = "Unable to locate game files. Please click Browse and select 'wic.exe'.";

            ActiveControl = buttonCancel;
        }

        private void FormInstaller_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to cancel?", "Cancel", MessageBoxButtons.YesNo) == DialogResult.No)
                e.Cancel = true;
        }

        private void buttonInstall_Click(object sender, EventArgs e)
        {
            string gameRootPath = textBoxGamePath.Text;
            string executablePath = Path.Combine(gameRootPath, "wic.exe");

            if (!gameRootPath.EndsWith('\\'))
                gameRootPath += '\\';

            if (!ValidateGameInstallPath(gameRootPath))
            {
                MessageBox.Show("An invalid game installation path was specified. Please check your spelling and that you have permissions to access the folder.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Check the installed version
            var versionInfo = FileVersionInfo.GetVersionInfo(executablePath);

            if (versionInfo.FileVersion != "1.0.1.1 (b35)")
            {
                MessageBox.Show($"Invalid game version detected. World in Conflict must be updated to version 1.0.1.1. Current version is {versionInfo.FileVersion}.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Install the multiplayer patch
            try
            {
                WriteResourceToFile("installer.DLLPackage.dbghelp.dll", Path.Combine(gameRootPath, "dbghelp.dll"));
                WriteResourceToFile("installer.DLLPackage.dbghelp_old.dll", Path.Combine(gameRootPath, "dbghelp_old.dll"));
                WriteResourceToFile("installer.DLLPackage.wic_cl_hook.dll", Path.Combine(gameRootPath, "wic_cl_hook.dll"));
                WriteResourceToFile("installer.DLLPackage.wic_ds_hook.dll", Path.Combine(gameRootPath, "wic_ds_hook.dll"));
                WriteResourceToFile("installer.DLLPackage.wic_bt_hook.dll", Path.Combine(gameRootPath, "wic_bt_hook.dll"));
                WriteResourceToFile("installer.DLLPackage.wic_ed_hook.dll", Path.Combine(gameRootPath, "wic_ed_hook.dll"));
            }
            catch (Exception)
            {
                MessageBox.Show("Failed to write DLLs to the game directory. Access is denied or files are currently in use.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Apply 4GB patch
            if (checkBoxApply4GBPatch.Checked)
            {
                try
                {
                    var data = File.ReadAllBytes(executablePath);

                    // Set the Large Address Aware (LAA) flag in the PE header
                    if (data[0x126] == 0x3)
                    {
                        data[0x126] = 0x23;
                        File.WriteAllBytes(executablePath, data);
                    }
                    else if (data[0x126] != 0x23)
                    {
                        MessageBox.Show("Unable to apply the 4GB patch to the WIC executable. Unknown flags are present.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
                catch (Exception)
                {
                    MessageBox.Show("Unable to apply the 4GB patch to the WIC executable. Access is denied.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            // Install registry key required by WicEd
            if (checkBoxModKitRegistryFix.Checked)
            {
                try
                {
                    var key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\WOW6432Node\Massive Entertainment AB\World in Conflict", true);

                    if (key == null)
                        key = Registry.LocalMachine.CreateSubKey(@"SOFTWARE\WOW6432Node\Massive Entertainment AB\World in Conflict", true);

                    key.SetValue("InstallPath", gameRootPath, RegistryValueKind.String);
                    key.SetValue("Version", "1.0.1.1", RegistryValueKind.String);
                    key.Close();
                }
                catch (System.Security.SecurityException)
                {
                    MessageBox.Show("Failed to write WicEd registry keys. Access is denied.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            // Completed
            MessageBox.Show("Done!", "", MessageBoxButtons.OK);

            FormClosing -= FormInstaller_FormClosing;
            Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void buttonBrowsePath_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog()
            {
                Filter = "wic.exe|wic.exe",
            };

            if (fileDialog.ShowDialog() == DialogResult.OK)
                textBoxGamePath.Text = Path.GetDirectoryName(fileDialog.FileName);
        }

        private void linkLabelDiscord_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("explorer", "https://discord.com/invite/eKDmvqz");
        }

        private void linkLabelReddit_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("explorer", "https://www.reddit.com/r/Massgate/");
        }

        private static string TryGuessGamePath()
        {
            // Try checking the registry first (Steam/GOG/physical disk)
            try
            {
                using (var key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 21760", false))
                {
                    if (key != null)
                    {
                        string fullPath = key.GetValue("InstallLocation") as string;

                        if (ValidateGameInstallPath(fullPath))
                            return fullPath;
                    }
                }

                using (var key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\WOW6432Node\Massive Entertainment AB\World in Conflict", false))
                {
                    if (key != null)
                    {
                        string fullPath = key.GetValue("InstallPath") as string;

                        if (ValidateGameInstallPath(fullPath))
                            return fullPath;
                    }
                }
            }
            catch (System.Security.SecurityException)
            {
                // User doesn't have permission
            }

            // No registry keys. Assume it's a hardcoded install path for now. Drive doesn't matter.
            var potentialDirs = new string[]
            {
                @"Program Files (x86)\Sierra Entertainment\World in Conflict",
                @"Program Files (x86)\Ubisoft\World in Conflict",
                @"Program Files (x86)\Steam\steamapps\common\World in Conflict",
                @"Program Files (x86)\World in Conflict - Complete Edition",
                @"Program Files\Sierra Entertainment\World in Conflict",
                @"Program Files\Ubisoft\World in Conflict",
                @"Program Files\Steam\steamapps\common\World in Conflict",
                @"Program Files\World in Conflict - Complete Edition",
            };

            foreach (string drive in Directory.GetLogicalDrives())
            {
                foreach (string dir in potentialDirs)
                {
                    string fullPath = Path.Combine(drive, dir);

                    if (ValidateGameInstallPath(fullPath))
                        return fullPath;
                }
            }

            return null;
        }

        private static bool ValidateGameInstallPath(string folderPath)
        {
            // Check if the directory exists and check if it contains wic.exe.
            if (!Directory.Exists(folderPath))
                return false;

            if (!File.Exists(Path.Combine(folderPath, "wic.exe")) || !File.Exists(Path.Combine(folderPath, "wic1.sdf")))
                return false;

            return true;
        }

        private static void WriteResourceToFile(string resourceName, string fileName)
        {
            using (var resource = Assembly.GetExecutingAssembly().GetManifestResourceStream(resourceName))
            using (var file = new FileStream(fileName, FileMode.Create, FileAccess.Write))
                resource.CopyTo(file);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Microsoft.Win32;
using System.IO;
using Interop;

namespace RLTools
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
        public static MainWindow Root;

		public MainWindow()
		{
			this.InitializeComponent();

			// Insert code required on object creation below this point.
            AddControl.ClickEvent += OnAddControlClick;

            Root = this;
		}

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void OnAddControlClick(object sender, RoutedEventArgs e)
        {
            try
            {
                NewFile();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void MiniButton_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = System.Windows.WindowState.Minimized;
        }

        private void MaxiButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.WindowState == System.Windows.WindowState.Maximized)
            {
                this.WindowState = System.Windows.WindowState.Normal;
                MaxiButton.ToolTip = "最大化";
                ResetWrapPanelLayout();
            }
            else if (this.WindowState == System.Windows.WindowState.Normal)
            {
                this.WindowState = System.Windows.WindowState.Maximized;
                MaxiButton.ToolTip = "还原";
                ResetWrapPanelLayout();
            }
        }

        private void Window_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            MaxiButton_Click(sender, e);
        }

        private void OnUnitControlStateChange(object sender, EventArgs e)
        {
            UnitControl uc = sender as UnitControl;
            if (uc == null)
                return;

            foreach (object obj in MainWrapPanel.Children)
            {
                if (obj is UserControl)
                {
                    UserControl ucTmp = obj as UserControl;
                    if (uc.Maxi)
                        ucTmp.Visibility = Visibility.Collapsed;
                    else
                        ucTmp.Visibility = Visibility.Visible;
                }
            }

            uc.Visibility = Visibility.Visible;
            if (uc.Maxi)
            {
                uc.Width = MainWrapPanel.ActualWidth - 40;
                uc.Height = MainWrapPanel.ActualHeight - 8;
                PanelScroll.VerticalScrollBarVisibility = ScrollBarVisibility.Disabled;
            }
            else
            {
                uc.Width = UnitControl.DefaultWidth;
                uc.Height = UnitControl.DefaultHeight;
                PanelScroll.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
            }
        }

        private void ResetWrapPanelLayout()
        {
            foreach (object obj in MainWrapPanel.Children)
            {
                if (obj is UnitControl)
                {
                    UnitControl uc = obj as UnitControl;
                    uc.ResetLayout();
                }
            }
        }

        private Config _Config;
        public Config Config
        {
            get { return _Config; }
            set { _Config = value; }
        }

        private Interoperation _Interop;
        public Interoperation Interop
        {
            get { return _Interop; }
            set { _Interop = value; }
        }

        private void OpenFileButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.InitialDirectory = Config.OpenFileDirection;
                ofd.Filter = "tfs Files (.tfs)|*.tfs|All Files (*.*)|*.*";
                if (ofd.ShowDialog(this) == true)
                {
                    Config.OpenFileDirection = System.IO.Path.GetDirectoryName(ofd.FileName);
                    foreach (string strFileName in ofd.FileNames)
                    {
                        OpenFile(strFileName);
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Config.Save();
        }

        public bool OpenFile(string strFile, bool bSilence = false)
        {
            if (!File.Exists(strFile))
            {
                if (!bSilence)
                {
                    MessageBox.Show(strFile + " not existed!", "OpenFileError.", MessageBoxButton.OK
                       , MessageBoxImage.Error);
                }
                return false;
            }

            foreach (object obj in MainWrapPanel.Children)
            {
                if (obj is UnitControl)
                {
                    UnitControl uc = obj as UnitControl;
                    if (uc.IsEqual(strFile))
                    {
                        SelectUnit(uc);
                        return false;
                    }
                }
            }

            TinyFS tfs = _Interop.GetTinyFS(strFile);
            if (!tfs.m_bReady)
            {
                if (!bSilence)
                {
                    MessageBox.Show(strFile + " 文件损坏!", "OpenFileError.", MessageBoxButton.OK
                       , MessageBoxImage.Error);
                }
                return false;
            }

            UnitControl ucNew = new UnitControl();
            ucNew.FileName = strFile;
            ucNew.Margin = new System.Windows.Thickness(24,4,0,0);
            ucNew.ControlStateChangeEvent += OnUnitControlStateChange;
            ucNew.GotFocus += new RoutedEventHandler(OnUnitControlGotFocus);
            ucNew.ControlCloseEvent += OnUnitControlClose;
            ucNew.ControlSavedEvent += OnUnitControlSaved;
            ucNew.AttachToTinyFS(tfs);
            MainWrapPanel.Children.Add(ucNew);
            ucNew.Focus();

            Config.AddOpenListItem(strFile);
            return true;
        }

        public bool NewFile(bool bSilence = false)
        {
            TinyFS tfs = _Interop.GetTinyFS(null);
            tfs.New();
            if (!tfs.m_bReady)
            {
                MessageBox.Show("无法创建新文件!", "CreateFileError.", MessageBoxButton.OK
                    , MessageBoxImage.Error);
                return false;
            }

            UnitControl ucNew = new UnitControl();
            ucNew.Margin = new System.Windows.Thickness(24, 4, 0, 0);
            ucNew.ControlStateChangeEvent += OnUnitControlStateChange;
            ucNew.GotFocus += new RoutedEventHandler(OnUnitControlGotFocus);
            ucNew.ControlCloseEvent += OnUnitControlClose;
            ucNew.ControlSavedEvent += OnUnitControlSaved;
            ucNew.AttachToTinyFS(tfs);
            ucNew.Modify = true;
            MainWrapPanel.Children.Add(ucNew);
            ucNew.Focus();
            return true;
        }

        void OnUnitControlSaved(object sender, EventArgs e)
        {
            UnitControl uc = sender as UnitControl;
            if (uc == null)
                return;

            Config.AddOpenListItem(uc.FileName);
        }

        void OnUnitControlClose(object sender, EventArgs e)
        {
            UnitControl uc = sender as UnitControl;
            if (uc == null)
                return;

            uc.Maxi = false;
            MainWrapPanel.Children.Remove(uc);
            Config.RemoveOpenListItem(uc.FileName);
        }

        void OnUnitControlGotFocus(object sender, RoutedEventArgs e)
        {
            try
            {
                UnitControl uc = sender as UnitControl;
                AddressTextBox.Text = uc.ToString();

                foreach (object obj in MainWrapPanel.Children)
                {
                    if (obj is UnitControl)
                    {
                        UnitControl ucTmp = obj as UnitControl;
                        ucTmp.BorderBlurRadius = UnitControl.NormalBlurRadis;
                    }
                }

                uc.BorderBlurRadius = UnitControl.FocusBlurRadius;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void SelectUnit(UnitControl uc)
        {
            uc.Focus();
            uc.Maxi = true;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < Config.LastOpenedFiles.Count; )
            {
                bool ret = OpenFile(Config.LastOpenedFiles[i], true);
                if (ret)
                    ++i;
                else
                    Config.LastOpenedFiles.RemoveAt(i);
            }
        }

        private void AddressTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string strFile = AddressTextBox.Text;
                if (!System.IO.Path.IsPathRooted(strFile))
                    strFile = System.IO.Path.Combine(Directory.GetCurrentDirectory(), strFile);
                OpenFile(strFile);
            }
        }
	}
}
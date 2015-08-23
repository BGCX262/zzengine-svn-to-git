using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using Interop;

namespace TinyLogQuery
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            _Interop = new Interoperation();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void MaxiButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.WindowState == System.Windows.WindowState.Maximized)
            {
                this.WindowState = System.Windows.WindowState.Normal;
                MaxiButton.ToolTip = "最大化";
            }
            else if (this.WindowState == System.Windows.WindowState.Normal)
            {
                this.WindowState = System.Windows.WindowState.Maximized;
                MaxiButton.ToolTip = "还原";
            }
        }

        private void MiniButton_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = System.Windows.WindowState.Minimized;
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void OpenButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Filter = "tlog Files (.tlog)|*.tlog|All Files (*.*)|*.*";

                if (ofd.ShowDialog(this) == true)
                {
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

        private Interoperation _Interop;

        private void OpenFile(string strFileName)
        {
            try
            {
                CTinyLog cTL = _Interop.LoadFromFile(strFileName);
                if (cTL == null)
                    return;

                TabItem tab = new TabItem();
                TabItemHeader tabHeader = new TabItemHeader();
                TinyLogUnit tlUnit = new TinyLogUnit();

                tabHeader.Text = cTL.GetName();
                tabHeader.Tag = tab;
                tabHeader.CloseButtonClickEvent += OnCloseButtonClick;

                tlUnit.BindingItem = cTL;

                tab.Header = tabHeader;
                tab.Content = tlUnit;

                SysTabControl.Items.Add(tab);
                SysTabControl.SelectedItem = tab;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void OnCloseButtonClick(object sender, EventArgs e)
        {
            try
            {
                if (sender is TabItemHeader)
                {
                    TabItemHeader header = sender as TabItemHeader;
                    TabItem tab = header.Tag as TabItem;
                    TinyLogUnit tlUnit = tab.Content as TinyLogUnit;
                    CTinyLog cTL = tlUnit.BindingItem;

                    cTL.Release();

                    SysTabControl.Items.Remove(tab);
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}

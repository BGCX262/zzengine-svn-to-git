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
using System.Windows.Shapes;

namespace RLTools
{
    /// <summary>
    /// Interaction logic for TableDialog.xaml
    /// </summary>
    public partial class TableDialog : Window
    {
        public TableDialog()
        {
            InitializeComponent();

            Result = MessageBoxResult.None;
        }

        public string Name;
        public MessageBoxResult Result;

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            Result = MessageBoxResult.Cancel;
            this.Close();
        }

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            if (NameTextBox.Text == null || NameTextBox.Text.Trim() == String.Empty)
            {
                MessageBox.Show("数据表名不能为空。");
                return;
            }

            Name = NameTextBox.Text;
            Result = MessageBoxResult.OK;
            this.Close();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            NameTextBox.Focus();
        }
    }
}

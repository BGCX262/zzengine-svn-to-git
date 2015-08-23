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
using Interop;

namespace TinyLogQuery
{
    /// <summary>
    /// Interaction logic for TinyLogUnit.xaml
    /// </summary>
    public partial class TinyLogUnit : UserControl
    {
        public TinyLogUnit()
        {
            InitializeComponent();
        }

        private CTinyLog _BindingItem;
        public CTinyLog BindingItem
        {
            get { return _BindingItem; }
            set
            {
                _BindingItem = value;
                UpdateDisplayData();
            }
        }

        private void UpdateDisplayData()
        {
            string str = _BindingItem.GetAllText();
            LogTextBox.Text = str;
        }
    }
}

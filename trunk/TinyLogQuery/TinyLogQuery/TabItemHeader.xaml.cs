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

namespace TinyLogQuery
{
    /// <summary>
    /// Interaction logic for TabItemHeader.xaml
    /// </summary>
    public partial class TabItemHeader : UserControl
    {
        public TabItemHeader()
        {
            InitializeComponent();
        }

        private string _Text;
        public string Text
        {
            get { return _Text; }
            set
            {
                _Text = value;
                HeaderTextBlock.Text = _Text;
            }
        }

        public delegate void CloseButtonClickHandler(object sender, EventArgs e);
        public event CloseButtonClickHandler CloseButtonClickEvent;

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            if (CloseButtonClickEvent != null)
                CloseButtonClickEvent(this, e);
        }


    }
}

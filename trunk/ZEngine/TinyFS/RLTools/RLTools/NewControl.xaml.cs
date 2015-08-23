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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RLTools
{
	/// <summary>
	/// Interaction logic for NewControl.xaml
	/// </summary>
	public partial class NewControl : UserControl
	{
		public NewControl()
		{
			this.InitializeComponent();
		}

        public delegate void ClickHandler(object sender, RoutedEventArgs e);
        public event ClickHandler ClickEvent;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (ClickEvent != null)
                ClickEvent(this, e);
        }
	}
}
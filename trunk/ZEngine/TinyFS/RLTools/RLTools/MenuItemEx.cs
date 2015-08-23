using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Controls;
using System.Windows.Media;

namespace RLTools
{
    public class MenuItemEx : MenuItem
    {
        private TextBlock _Text;
        private Image _Iamge;

        public MenuItemEx()
        {
            StackPanel stackPanel = new StackPanel();
            stackPanel.Orientation = Orientation.Horizontal;
            stackPanel.VerticalAlignment = VerticalAlignment.Center;
            this.Header = stackPanel;

            _Iamge = new Image();
            _Iamge.VerticalAlignment = VerticalAlignment.Center;
            _Iamge.Margin = new Thickness(0, 0, 2, 0);
            _Iamge.Width = 20;
            _Iamge.Height = 20;
            stackPanel.Children.Add(_Iamge);

            _Text = new TextBlock();
            _Text.VerticalAlignment = VerticalAlignment.Center;
            stackPanel.Children.Add(_Text);
        }

        public string Text
        {
            set { _Text.Text = value; }
            get { return _Text.Text; }
        }

        public ImageSource ImageSource
        {
            set { _Iamge.Source = value; }
        }

        public TreeViewItemContextMenu Owner;
    }

    public class TreeViewItemContextMenu : ContextMenu
    {
        public MenuItemEx AddItem;
        public MenuItemEx DeleteItem;
        public MenuItemEx PropertyItem;

        public string Name;
        public ImageTreeViewItem BindingItem;
        public ImageTreeViewItem RootItem;
        public string OwnerName;

        public TreeViewItemContextMenu()
        {
            AddItem = new MenuItemEx();
            DeleteItem = new MenuItemEx();
            PropertyItem = new MenuItemEx();

            AddItem.Text = "新建数据表";
            DeleteItem.Text = "删除";
            PropertyItem.Text = "属性";

            AddItem.Owner = this;
            DeleteItem.Owner = this;
            PropertyItem.Owner = this;

            AddItem.ImageSource = new BitmapImage(new Uri("Resource/Image/add.png", UriKind.Relative));
            DeleteItem.ImageSource = new BitmapImage(new Uri("Resource/Image/delete.png", UriKind.Relative));
            //PropertyItem.ImageSource = new BitmapImage(new Uri("Resource/Image/delete.png", UriKind.Relative));

            this.Items.Add(AddItem);
            this.Items.Add(DeleteItem);
            this.Items.Add(new Separator());
            this.Items.Add(PropertyItem);
        }
    }
}

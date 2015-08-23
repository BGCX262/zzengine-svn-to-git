using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace RLTools
{
    public class ImageTreeViewItem : TreeViewItem
    {
        private TextBlock text;
        private Image image;
        private ImageSource srcSelected;
        private ImageSource srcUnselected;

        public string TableName;
        public static ImageTreeViewItem RootItem;

        public ImageTreeViewItem()
        {
            StackPanel stackPanel = new StackPanel();
            stackPanel.Orientation = Orientation.Horizontal;
            stackPanel.VerticalAlignment = VerticalAlignment.Center;
            this.Header = stackPanel;

            image = new Image();
            image.VerticalAlignment = VerticalAlignment.Center;
            image.Margin = new Thickness(0, 0, 2, 0);
            image.Width = 20;
            image.Height = 20;
            stackPanel.Children.Add(image);

            text = new TextBlock();
            text.VerticalAlignment = VerticalAlignment.Center;
            stackPanel.Children.Add(text);

            TableName = null;
        }

        public string Text
        {
            set { text.Text = value; }
            get { return text.Text; }
        }

        public ImageSource SelectedImage
        {
            set
            {
                srcSelected = value;
                if (this.IsSelected)
                {
                    image.Source = srcSelected;
                }
            }
            get { return srcSelected; }
        }

        public ImageSource UnselectedImage
        {
            set
            {
                srcUnselected = value;
                if (!this.IsSelected)
                {
                    image.Source = srcUnselected;
                }
            }
            get { return srcUnselected; }
        }

        protected override void OnSelected(RoutedEventArgs e)
        {
            base.OnSelected(e);
            image.Source = srcSelected;
        }

        protected override void OnUnselected(RoutedEventArgs e)
        {
            base.OnUnselected(e);
            image.Source = srcUnselected;
        }
    }

    class ButtonTreeViewItem : ImageTreeViewItem
    {
        private TextBlock text;
        private Image image;

        public ButtonTreeViewItem()
        {
            Button btn = new Button();
            StackPanel stackPanel = new StackPanel();
            stackPanel.Orientation = Orientation.Horizontal;
            btn.Content = stackPanel;
            this.Header = btn;

            image = new Image();
            image.VerticalAlignment = VerticalAlignment.Center;
            image.Margin = new Thickness(0, 0, 2, 0);
            image.Width = 16;
            image.Height = 16;
            stackPanel.Children.Add(image);

            text = new TextBlock();
            text.VerticalAlignment = VerticalAlignment.Center;
            stackPanel.Children.Add(text);
        }

        public string Text
        {
            set { text.Text = value; }
            get { return text.Text; }
        }

        public ImageSource ImageSource
        {
            set
            {
                image.Source = value;
            }
            get { return image.Source; }
        }
    }
}

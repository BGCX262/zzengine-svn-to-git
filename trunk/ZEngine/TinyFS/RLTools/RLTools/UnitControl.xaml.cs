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
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Data;
using Microsoft.Win32;
using Interop;
using System.Xml;

namespace RLTools
{
    /// <summary>
    /// Interaction logic for UnitControl.xaml
    /// </summary>
    public partial class UnitControl : UserControl
    {
        public static readonly int DefaultWidth = 160;
        public static readonly int DefaultHeight = 176;

        public UnitControl()
        {
            InitializeComponent();

            FileName = "Unknown! Not Saved.";
            BorderBlurRadius = NormalBlurRadis;
            Modify = false;
        }

        private bool _Modify;
        public bool Modify
        {
            get { return _Modify; }
            set
            {
                if (_Modify != value)
                {
                    _Modify = value;
                    ModifyImage.Visibility = _Modify ? Visibility.Visible : Visibility.Hidden;
                }
            }
        }

        private void CanExecuteAddCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = SysTreeView.SelectedItem != null;
        }

        private void ExecuteAddCommand(object sender, ExecutedRoutedEventArgs e)
        {
            try
            {
                ImageTreeViewItem item = SysTreeView.SelectedItem as ImageTreeViewItem;
                if (item == null)
                    return;
                TableData tab = item.Tag as TableData;
                if (tab == null)
                    return;

                KeyValueDialog kvDialog = new KeyValueDialog();
                kvDialog.BindingT = _BindingT;
                kvDialog.Table = tab;
                kvDialog.DialogType = KeyValueDialogType.Add;
                kvDialog.ShowDialog();
                if (kvDialog.Result == MessageBoxResult.OK)
                {
                    SetValue(item.TableName, kvDialog.Key, KeyValueData.GetType(kvDialog.Type), kvDialog.Data);
                    Modify = true;

                    for (int i = 0; i < tab.Item.Count; ++i)
                    {
                        KeyValueData kvd = tab.Item[i];
                        DataRow dr = tab.DataSource.Rows[i];
                        if (kvd.Key == kvDialog.Key)
                        {
                            kvd.Type = kvDialog.Type;
                            kvd.Data = kvDialog.Data;
                            dr["Type"] = kvDialog.Type;
                            dr["Data"] = kvDialog.Data;
                            return;
                        }
                    }

                    tab.Item.Add(new KeyValueData(kvDialog.Key, kvDialog.Type, kvDialog.Data));
                    tab.DataSource.Rows.Add(kvDialog.Key, kvDialog.Type, kvDialog.Data);
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void CanExecuteDeleteCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            if (SysDataGrid != null)
                e.CanExecute = SysDataGrid.SelectedIndex != -1;
            else
                e.CanExecute = false;
        }

        private void ExecuteDeleteCommand(object sender, ExecutedRoutedEventArgs e)
        {
            if (SysDataGrid.SelectedIndex == -1 || !(SysDataGrid.Tag is TableData))
                return;

            try
            {
                TableData td = SysDataGrid.Tag as TableData;
                KeyValueData kvd = td.Item[SysDataGrid.SelectedIndex];
                string msg = String.Format("Are you sure to Delete Key-Value({0} = {1})?",
                    kvd.Key, kvd.Data);
                if (MessageBox.Show(msg, "Delete Item", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                {
                    int ret = BindingT.SetNull(td.Name, kvd.Key);
                    if (ret == 0)
                    {
                        MessageBox.Show("Delete Error!");
                        return;
                    }
                    td.Item.RemoveAt(SysDataGrid.SelectedIndex);
                    td.DataSource.Rows.RemoveAt(SysDataGrid.SelectedIndex);
                    
                    Modify = true;
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void CanExecuteModifyCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            if (SysDataGrid != null)
                e.CanExecute = SysDataGrid.SelectedIndex != -1;
            else
                e.CanExecute = false;
        }

        private void ExecuteModifyCommand(object sender, ExecutedRoutedEventArgs e)
        {
            try
            {
                ImageTreeViewItem item = SysTreeView.SelectedItem as ImageTreeViewItem;
                if (item == null)
                    return;
                TableData tab = item.Tag as TableData;
                if (tab == null)
                    return;

                KeyValueDialog kvDialog = new KeyValueDialog();
                kvDialog.BindingT = _BindingT;
                kvDialog.Table = tab;
                kvDialog.DialogType = KeyValueDialogType.Modify;
                kvDialog.Key = tab.Item[SysDataGrid.SelectedIndex].Key;
                kvDialog.Type = tab.Item[SysDataGrid.SelectedIndex].Type;
                kvDialog.Data = tab.Item[SysDataGrid.SelectedIndex].Data;
                kvDialog.ShowDialog();
                if (kvDialog.Result == MessageBoxResult.OK)
                {
                    SetValue(item.TableName, kvDialog.Key, KeyValueData.GetType(kvDialog.Type), kvDialog.Data);
                    Modify = true;

                    KeyValueData kvd = tab.Item[SysDataGrid.SelectedIndex];
                    DataRow dr = tab.DataSource.Rows[SysDataGrid.SelectedIndex];
                    kvd.Type = kvDialog.Type;
                    kvd.Data = kvDialog.Data;
                    dr["Type"] = kvDialog.Type;
                    dr["Data"] = kvDialog.Data;
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void CanExecuteCloseCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void ExecuteCloseCommand(object sender, ExecutedRoutedEventArgs e)
        {
            if (Modify)
            {
                string msg = String.Format("{0}修改未保存，是否关闭?", FileName);
                if (MessageBox.Show(msg, "Warning", MessageBoxButton.YesNo) == MessageBoxResult.No)
                    return;
            }

            if (ControlCloseEvent != null)
                ControlCloseEvent(this, null);
        }

        private void CanExecuteSaveCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = Modify == true;
        }

        private void ExecuteSaveCommand(object sender, ExecutedRoutedEventArgs e)
        {
            if (System.IO.File.Exists(FileName))
            {
                Save(FileName);
            }
            else
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.Filter = "tfs Files (.tfs)|*.tfs|All Files (*.*)|*.*";
                if (sfd.ShowDialog(MainWindow.Root) == true)
                {
                    FileName = sfd.FileName;
                    Save(sfd.FileName);
                    ImageTreeViewItem.RootItem.Text = System.IO.Path.GetFileName(FileName);
                }
            }
        }

        private void CanExecuteExportXmlCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = Modify != true;
        }

        private void ExecuteExportXmlCommand(object sender, ExecutedRoutedEventArgs e)
        {
            try
            {
                string file = FileName + ".xml";
                XmlTextWriter xmlWriter = new XmlTextWriter(file, Encoding.UTF8);
                xmlWriter.Formatting = Formatting.Indented;
                xmlWriter.Indentation = 4;
                xmlWriter.WriteStartDocument();
                xmlWriter.WriteStartElement(_BindingT.GetName());
                ExportXmlNode(xmlWriter, null);
                xmlWriter.WriteEndElement();
                xmlWriter.WriteEndDocument();
                xmlWriter.Flush();
                xmlWriter.Close();

                InfoTextBlock.Text = "创建" + file;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void ExportXmlNode(XmlTextWriter writer, string node)
        {
            List<string> keys = _BindingT.GetTableKeys(node);
            foreach (string key in keys)
            {
                writer.WriteStartElement(key);
                int value = _BindingT.GetValue(node, key);
                if (_BindingT.IsTable(value))
                {
                    ExportXmlNode(writer, key);
                }
                else if (_BindingT.IsBool(value))
                {
                    bool v = _BindingT.ToBool(value);
                    writer.WriteAttributeString("Type", "bool");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsByte(value))
                {
                    byte v = _BindingT.ToByte(value);
                    writer.WriteAttributeString("Type", "byte");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsDouble(value))
                {
                    double v = _BindingT.ToDouble(value);
                    writer.WriteAttributeString("Type", "double");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsFloat(value))
                {
                    double v = _BindingT.ToFloat(value);
                    writer.WriteAttributeString("Type", "float");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsInt32(value))
                {
                    int v = _BindingT.ToInt32(value);
                    writer.WriteAttributeString("Type", "int");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsInt64(value))
                {
                    long v = _BindingT.ToInt64(value);
                    writer.WriteAttributeString("Type", "int64");
                    writer.WriteAttributeString("Value", v.ToString());
                }
                else if (_BindingT.IsString(value))
                {
                    string v = _BindingT.ToString(value);
                    writer.WriteAttributeString("Type", "string");
                    writer.WriteAttributeString("Value", v);
                }
                writer.WriteEndElement();
            }
        }

        private void CanExecuteImportXmlCommand(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = false;
        }

        private void ExecuteImportXmlCommand(object sender, ExecutedRoutedEventArgs e)
        {
        }

        private void Save(string file)
        {
            bool ret = _BindingT.Save(file);
            if (ret)
            {
                InfoTextBlock.Text = file + " Saved.";
                Modify = false;
                if (ControlSavedEvent != null)
                    ControlSavedEvent(this, null);
            }
            else
                InfoTextBlock.Text = file + " Save Failed!";
        }

        private bool _Maxi = false;
        public bool Maxi
        {
            get { return _Maxi; }
            set 
            {
                if (_Maxi == value)
                    return;

                _Maxi = value;
                if (_Maxi)
                    MaxiButton.ToolTip = "还原";
                else
                    MaxiButton.ToolTip = "扩展到整个窗口";

                if (ControlStateChangeEvent != null)
                    ControlStateChangeEvent(this, null);
            }
        }

        public delegate void ControlStateChangeHandler(object sender, EventArgs e);
        public event ControlStateChangeHandler ControlStateChangeEvent;

        public delegate void ControlCloseHandler(object sender, EventArgs e);
        public event ControlCloseHandler ControlCloseEvent;

        public delegate void ControlSavedHandler(object sender, EventArgs e);
        public event ControlSavedHandler ControlSavedEvent;

        private void MaxiButton_Click(object sender, RoutedEventArgs e)
        {
            Maxi = !Maxi;
        }

        public void ResetLayout()
        {
            if (Maxi)
            {
                Maxi = false;
                if (ControlStateChangeEvent != null)
                    ControlStateChangeEvent(this, null);
            }

            BorderEffect.BlurRadius = NormalBlurRadis;
        }

        private void UserControl_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            this.Focus();
            e.Handled = true;
        }

        private string _FileName;
        public string FileName
        {
            get { return _FileName; }
            set 
            {
                _FileName = value;
                //this.ToolTip = _FileName;
            }
        }
        public bool IsEqual(string strFrom)
        {
            return FileName.CompareTo(strFrom) == 0;
        }

        public override string ToString()
        {
            return FileName;
        }

        private void UserControl_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.Focus();
        }

        public static double FocusBlurRadius = 50.0;
        public static double NormalBlurRadis = 5.0;
        private double _BorderBlurRadius = NormalBlurRadis;
        public double BorderBlurRadius
        {
            get { return _BorderBlurRadius; }
            set
            {
                if (_BorderBlurRadius != value)
                {
                    DoubleAnimation dAnimation = new DoubleAnimation();
                    dAnimation.From = _BorderBlurRadius;
                    dAnimation.To = value;
                    dAnimation.Duration = new Duration(TimeSpan.Parse("0:0:0.5"));
                    BorderEffect.BeginAnimation(DropShadowEffect.BlurRadiusProperty, dAnimation);

                    _BorderBlurRadius = value;
                }
            }
        }

        private TinyFS _BindingT;
        public TinyFS BindingT
        {
            get { return _BindingT; }
            set { _BindingT = value; }
        }
        public void AttachToTinyFS(TinyFS tfs)
        {
            BindingT = tfs;

            ImageTreeViewItem rootItem = new ImageTreeViewItem();
            rootItem.SelectedImage = new BitmapImage(new Uri("Resource/Image/letter1.png", UriKind.Relative));
            rootItem.UnselectedImage = new BitmapImage(new Uri("Resource/Image/letter.png", UriKind.Relative));
            rootItem.Text = System.IO.Path.GetFileName(FileName);
            rootItem.Selected += TreeViewItem_Selected;
            rootItem.IsExpanded = true;
            SysTreeView.Items.Add(rootItem);
            ImageTreeViewItem.RootItem = rootItem;

            TreeViewItemContextMenu cm = new TreeViewItemContextMenu();
            cm.DeleteItem.IsEnabled = false;
            cm.BindingItem = rootItem;
            cm.AddItem.Click += new RoutedEventHandler(AddItem_Click);
            cm.DeleteItem.Click += new RoutedEventHandler(DeleteItem_Click);
            rootItem.ContextMenu = cm;

            List<String> items = tfs.GetTableKeys(null);
            DisplayTreeView(rootItem, null, items);
        }

        void DeleteItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItemEx mi = sender as MenuItemEx;
            if (mi == null)
                return;

            string msg = String.Format("确实要删除{0}？", mi.Owner.Name);
            if (MessageBox.Show(msg, "Warning", MessageBoxButton.YesNo) == MessageBoxResult.No)
                return;

            _BindingT.SetNull(mi.Owner.OwnerName, mi.Owner.Name);
            Modify = true;
            mi.Owner.RootItem.Items.Remove(mi.Owner.BindingItem);
        }

        void AddItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItemEx mi = sender as MenuItemEx;
            if (mi == null)
                return;

            TableDialog td = new TableDialog();
            td.ShowDialog();

            if (td.Result != MessageBoxResult.OK)
                return;

            string name = td.Name;
            _BindingT.SetTable(mi.Owner.Name, name);
            Modify = true;

            List<KeyValueData> kvList = new List<KeyValueData>();
            ImageTreeViewItem node = new ImageTreeViewItem();
            node.SelectedImage = new BitmapImage(new Uri("Resource/Image/file1.png", UriKind.Relative));
            node.UnselectedImage = new BitmapImage(new Uri("Resource/Image/file.png", UriKind.Relative));
            node.Text = name;
            node.TableName = name;
            node.Selected += TreeViewItem_Selected;
            mi.Owner.BindingItem.Items.Add(node);

            TreeViewItemContextMenu cm = new TreeViewItemContextMenu();
            cm.Name = name;
            cm.OwnerName = mi.Owner.Name;
            cm.BindingItem = node;
            cm.RootItem = mi.Owner.BindingItem;
            node.ContextMenu = cm;
            cm.AddItem.Click += new RoutedEventHandler(AddItem_Click);
            cm.DeleteItem.Click += new RoutedEventHandler(DeleteItem_Click);

            DataTable dt = new DataTable(name);
            dt.Columns.Add("Key", typeof(string));
            dt.Columns.Add("Type", typeof(string));
            dt.Columns.Add("Data", typeof(string));
            foreach (KeyValueData kvd in kvList)
            {
                dt.Rows.Add(kvd.Key, kvd.Type, kvd.Data);
            }

            TableData tab = new TableData();
            tab.Name = name;
            tab.DataSource = dt;
            tab.Item = kvList;
            node.Tag = tab;
        }

        private void DisplayTreeView(ImageTreeViewItem root, String name, List<String> items)
        {
            List<KeyValueData> kvList = new List<KeyValueData>();

            foreach (String key in items)
            {
                int value = BindingT.GetValue(name, key);
                if (BindingT.IsTable(value))
                {
                    ImageTreeViewItem node = new ImageTreeViewItem();
                    node.SelectedImage = new BitmapImage(new Uri("Resource/Image/file1.png", UriKind.Relative));
                    node.UnselectedImage = new BitmapImage(new Uri("Resource/Image/file.png", UriKind.Relative));
                    node.Text = key;
                    node.TableName = key;
                    node.Selected += TreeViewItem_Selected;
                    root.Items.Add(node);

                    TreeViewItemContextMenu cm = new TreeViewItemContextMenu();
                    cm.Name = key;
                    cm.OwnerName = name;
                    cm.BindingItem = node;
                    cm.RootItem = root;
                    node.ContextMenu = cm;
                    cm.AddItem.Click += new RoutedEventHandler(AddItem_Click);
                    cm.DeleteItem.Click += new RoutedEventHandler(DeleteItem_Click);

                    List<String> subItems = BindingT.GetTableKeys(key);
                    DisplayTreeView(node, key, subItems);
                    continue;
                }

                KeyValueData kvData = new KeyValueData();
                if (BindingT.IsBool(value))
                {
                    bool bv = BindingT.ToBool(value);
                    kvData.AttachToTfsValue(key, ValueType.Bool, bv.ToString());
                }
                else if (BindingT.IsByte(value))
                {
                    byte bv = BindingT.ToByte(value);
                    kvData.AttachToTfsValue(key, ValueType.Byte, bv.ToString());
                }
                else if (BindingT.IsDouble(value))
                {
                    double dv = BindingT.ToDouble(value);
                    kvData.AttachToTfsValue(key, ValueType.Double, dv.ToString());
                }
                else if (BindingT.IsFloat(value))
                {
                    float fv = BindingT.ToFloat(value);
                    kvData.AttachToTfsValue(key, ValueType.Float, fv.ToString());
                }
                else if (BindingT.IsInt32(value))
                {
                    int i32v = BindingT.ToInt32(value);
                    kvData.AttachToTfsValue(key, ValueType.Int32, i32v.ToString());
                }
                else if (BindingT.IsInt64(value))
                {
                    Int64 i64v = BindingT.ToInt64(value);
                    kvData.AttachToTfsValue(key, ValueType.Int64, i64v.ToString());
                }
                else if (BindingT.IsString(value))
                {
                    string sv = BindingT.ToString(value);
                    kvData.AttachToTfsValue(key, ValueType.String, sv);
                }

                kvList.Add(kvData);
            }

            DataTable dt = new DataTable(name);
            dt.Columns.Add("Key", typeof(string));
            dt.Columns.Add("Type", typeof(string));
            dt.Columns.Add("Data", typeof(string));
            foreach (KeyValueData kvd in kvList)
            {
                dt.Rows.Add(kvd.Key, kvd.Type, kvd.Data);
            }

            TableData tab = new TableData();
            tab.Name = name;
            tab.DataSource = dt;
            tab.Item = kvList;
            root.Tag = tab;
        }

        private void TreeViewItem_Selected(object sender, RoutedEventArgs e)
        {
            ImageTreeViewItem item = sender as ImageTreeViewItem;
            if (item == null || !(item.Tag is TableData))
                return;
            e.Handled = true;
            TableData tab = item.Tag as TableData;
            SysDataGrid.DataContext = tab.DataSource;
            SysDataGrid.Tag = tab;
        }

        private void SetValue(string tab, string key, ValueType vtype, string value)
        {
            switch (vtype)
            {
                case ValueType.Bool:
                    _BindingT.SetBool(tab, key, bool.Parse(value));
                    break;
                case ValueType.Byte:
                    _BindingT.SetByte(tab, key, byte.Parse(value));
                    break;
                case ValueType.Double:
                    _BindingT.SetDouble(tab, key, double.Parse(value));
                    break;
                case ValueType.Float:
                    _BindingT.SetFloat(tab, key, float.Parse(value));
                    break;
                case ValueType.Int32:
                    _BindingT.SetInt32(tab, key, int.Parse(value));
                    break;
                case ValueType.Int64:
                    _BindingT.SetInt64(tab, key, long.Parse(value));
                    break;
                case ValueType.String:
                    _BindingT.SetString(tab, key, value);
                    break;
            }
        }
    }
}

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
using Interop;

namespace RLTools
{
    /// <summary>
    /// Interaction logic for KeyValueDialog.xaml
    /// </summary>

    public enum KeyValueDialogType
    {
        None,
        Add,
        Modify
    }

    public partial class KeyValueDialog : Window
    {
        public KeyValueDialog()
        {
            InitializeComponent();

            List<string> Types = KeyValueData.GetAllTypes();
            foreach (string t in Types)
            {
                ComboBoxItem cbi = new ComboBoxItem();
                cbi.Content = t;
                TypeComboBox.Items.Add(cbi);
            }
            TypeComboBox.SelectedIndex = 0;

            Result = MessageBoxResult.None;
        }

        private TinyFS _BindingT;
        public TinyFS BindingT { set { _BindingT = value; } }

        private TableData _Table;
        public TableData Table { set { _Table = value; } }

        public string Key
        {
            get { return KeyTextBox.Text; }
            set { KeyTextBox.Text = value; }
        }

        public string Type
        {
            get { return TypeComboBox.Text; }
            set
            {
                int index = (int)KeyValueData.GetType(value);
                TypeComboBox.SelectedIndex = index;
            }
        }

        public string Data
        {
            get { return DataTextBox.Text; }
            set { DataTextBox.Text = value; }
        }

        public MessageBoxResult Result;

        private KeyValueDialogType _DialogType;
        public KeyValueDialogType DialogType
        {
            get { return _DialogType; }
            set
            {
                _DialogType = value;
                switch (_DialogType)
                {
                    case KeyValueDialogType.Add:
                        KeyTextBox.IsEnabled = true;
                        this.Title = "Add Item";
                        break;
                    case KeyValueDialogType.Modify:
                        KeyTextBox.IsEnabled = false;
                        this.Title = "Modify Item";
                        break;
                }
            }
        }

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            if (!IsDataValidate())
                return;

            if (DialogType == KeyValueDialogType.Add)
            {
                foreach (KeyValueData kvd in _Table.Item)
                {
                    if (Key == kvd.Key)
                    {
                        string msg = String.Format("Key={0}已经存在，是否覆盖？", Key);
                        if (MessageBox.Show(msg, "Message", MessageBoxButton.YesNo)
                            != MessageBoxResult.Yes)
                            return;
                    }
                }
            }

            Result = MessageBoxResult.OK;
            this.Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            Result = MessageBoxResult.Cancel;
            this.Close();
        }

        private bool IsDataValidate()
        {
            if (KeyTextBox.Text == "" || KeyTextBox.Text == null)
            {
                MessageBox.Show("Key（名称）不能为空。");
                return false;
            }

            ValueType vtype = KeyValueData.GetType(TypeComboBox.Text);
            switch (vtype)
            {
                case ValueType.None:
                    MessageBox.Show("请选择一种有效的数据类型。");
                    return false;
                case ValueType.Bool:
                    try
                    {
                        bool.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("bool类型只能为True或者False。");
                        return false;
                    }
                    break;
                case ValueType.Byte:
                    try
                    {
                        byte.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("byte类型只能为8位整数。");
                        return false;
                    }
                    break;
                case ValueType.Double:
                    try
                    {
                        double.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("double类型只能为双精度数字。");
                        return false;
                    }
                    break;
                case ValueType.Float:
                    try
                    {
                        float.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("float类型只能为双精度数字。");
                        return false;
                    }
                    break;
                case ValueType.Int32:
                    try
                    {
                        int.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("int类型只能为32整数。");
                        return false;
                    }
                    break;
                case ValueType.Int64:
                    try
                    {
                        long.Parse(Data);
                    }
                    catch (System.Exception)
                    {
                        MessageBox.Show("int64类型只能为64整数。");
                        return false;
                    }
                    break;
            }
            return true;
        }
    }
}

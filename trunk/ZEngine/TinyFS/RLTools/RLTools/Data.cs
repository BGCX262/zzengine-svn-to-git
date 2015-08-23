using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using Interop;

namespace RLTools
{
    public enum ValueType
    {
        None,
        Bool,
        Byte,
        Int32,
        Float,
        Int64,
        Double,
        String
    }

    public class KeyValueData
    {
        public string Key;
        public string Type;
        public string Data;

        public KeyValueData()
        {
            Key = "";
            Type = GetTypeName(ValueType.None);
            Data = "Null";
        }

        public KeyValueData(string key, string type, string data)
        {
            Key = key;
            Type = type;
            Data = data;
        }

        public void AttachToTfsValue(string key, ValueType vtype, string value)
        {
            Key = key;
            Type = GetTypeName(vtype);
            Data = value;
        }

        public static List<string> TypeNames = new List<string>(new string[]{"Null"
                , "Boolean(8)"
                , "Byte(8)"
                , "Integer(32)"
                , "Float(32)"
                , "Integer(64)"
                , "Double(64)"
                , "String"});
        public static List<string> GetAllTypes() { return TypeNames; }
        public static string GetTypeName(ValueType vtype) { return TypeNames[(int)vtype]; }
        public static ValueType GetType(string name)
        {
            for (int i = 0; i < TypeNames.Count; ++i)
                if (name == TypeNames[i])
                    return (ValueType)i;
            return ValueType.None;
        }
    }

    public class TableData
    {
        public List<KeyValueData> Item;
        public DataTable DataSource;
        public string Name;
    }
}

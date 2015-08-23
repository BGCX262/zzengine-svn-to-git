using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using System.Windows;

namespace RLTools
{
    public enum ConfigState
    {
        None,
        NotExisted,
        FormatError,
        Ready
    }

    public class Config
    {
        public string OpenFileDirection;
        public string TinyFSFile;
        public List<string> LastOpenedFiles;
        public ConfigState State;

        public Config()
        {
            OpenFileDirection = "";
            TinyFSFile = "TinyFS.dll";
            LastOpenedFiles = new List<string>();
            State = ConfigState.None;
        }

        public string ConfigFile;
        public static Config LoadConfig(string strFile)
        {
            Config con = new Config();

            if (!Path.IsPathRooted(strFile))
                strFile = Path.Combine(Directory.GetCurrentDirectory(), strFile);
            con.ConfigFile = strFile;

            if (!File.Exists(strFile))
            {
                con.State = ConfigState.NotExisted;
                return con;
            }
            
            try
            {
                XmlDocument xmlDoc = new XmlDocument();
                xmlDoc.Load(strFile);

                XmlElement xmlEle = xmlDoc.SelectSingleNode("tfs-config/Environment") as XmlElement;
                con.OpenFileDirection = xmlEle.GetAttribute("Direction");

                xmlEle = xmlDoc.SelectSingleNode("tfs-config/OpenList") as XmlElement;
                foreach (XmlNode xmlNode in xmlEle.ChildNodes)
                {
                    XmlElement ele = xmlNode as XmlElement;
                    con.LastOpenedFiles.Add(ele.GetAttribute("Path"));
                }
            }
            catch (System.Exception)
            {
                con.State = ConfigState.FormatError;	
            }

            return con;
        }

        public void Save()
        {
            try
            {
                XmlTextWriter xmlWriter = new XmlTextWriter(ConfigFile, Encoding.UTF8);
                xmlWriter.Formatting = Formatting.Indented;
                xmlWriter.Indentation = 4;
                xmlWriter.WriteStartDocument();
                {
                    xmlWriter.WriteStartElement("tfs-config");
                    {
                        xmlWriter.WriteStartElement("Environment");
                        {
                            xmlWriter.WriteAttributeString("Direction", OpenFileDirection);
                        }
                        xmlWriter.WriteEndElement();

                        xmlWriter.WriteStartElement("OpenList");
                        {
                            foreach (string path in LastOpenedFiles)
                            {
                                xmlWriter.WriteStartElement("Element");
                                xmlWriter.WriteAttributeString("Path", path);
                                xmlWriter.WriteEndElement();
                            }
                        }
                        xmlWriter.WriteEndElement();
                    }
                    xmlWriter.WriteEndElement();
                }
                xmlWriter.WriteEndDocument();
                xmlWriter.Flush();
                xmlWriter.Close();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message, "Save Config File", MessageBoxButton.OK);
            }
        }

        public void AddOpenListItem(string strItem)
        {
            foreach (string item in LastOpenedFiles)
            {
                if (item.CompareTo(strItem) == 0)
                    return;
            }

            LastOpenedFiles.Add(strItem);
        }

        public void RemoveOpenListItem(string strItem)
        {
            LastOpenedFiles.Remove(strItem);
        }
    }
}

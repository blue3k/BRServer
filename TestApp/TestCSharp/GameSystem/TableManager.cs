using System;
using System.Reflection;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Text;
using Microsoft.Xna.Framework;

using BR;
using Cocos2D;

namespace BR.GameSystem
{
    public class TableManager : GameSystemComponent
    {

        public TableManager(BRGame game)
            : base(game)
        {
        }

        public override bool Initialize()
        {
            if (!base.Initialize())
                return false;

            return true;
        }

        public override void Start()
        {
        }

        public static T LoadXML<T>(string xmlFile) where T : class
        {
            return null;
        }

        public static void SaveXML<T>(string xmlFile, System.Object data) where T : class
        {
            try
            {
                // get tablename
                string tableName = data.GetType().Name;

                // get namespace name
                string namespaceName = string.Empty;
                foreach (var text in data.GetType().GetCustomAttributes(typeof(System.Xml.Serialization.XmlTypeAttribute), true))
                {
                    System.Xml.Serialization.XmlTypeAttribute attr = (System.Xml.Serialization.XmlTypeAttribute)text;
                    namespaceName = attr.Namespace;
                }

                // write data to memory stream!
                XmlSerializer serializer = new XmlSerializer(typeof(T));
                MemoryStream memoryStream = new MemoryStream();
                XmlTextWriter writer = new XmlTextWriter(memoryStream, Encoding.UTF8);
                serializer.Serialize(writer, data);

                // load xml document!
                memoryStream.Position = 0;
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.Load(memoryStream);


                // add schema location
                XmlAttribute att = null;
                if (namespaceName != null)
                {
                    att = xmlDocument.CreateAttribute("xsi", "schemaLocation", "http://www.w3.org/2001/XMLSchema-instance");
                    att.Value = namespaceName + " " + tableName + ".xsd";
                    xmlDocument.DocumentElement.SetAttributeNode(att);
                }
                else
                {
                    att = xmlDocument.CreateAttribute("xsi", "noNamespaceSchemaLocation", "http://www.w3.org/2001/XMLSchema-instance");
                    att.Value = "../" + tableName + ".xsd";
                    xmlDocument.DocumentElement.SetAttributeNode(att);
                }

                // write xml file
                Stream fileStream = new FileStream(xmlFile, FileMode.Create);
                xmlDocument.Save(fileStream);

                // close stream
                fileStream.Close();
                memoryStream.Close();

            }
            catch (Exception e)
            {
                BRDebug.Error(e.Message);
                BRDebug.Error(e.StackTrace);
            }
        }

        public static UInt64 MakeCompositeKey(UInt32 highKey, UInt32 lowKey)
        {
            UInt64 compositeKey = highKey;
            compositeKey = (compositeKey << 32) | lowKey;
            return compositeKey;
        }

    }
}

using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class UnrealTreeBuilder
    {
        protected override void DeclareEnum(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            OpenSection("enum", name);

            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = name + "_" + strEnum + ",";
                WriteStatement(statement);
            }
            CloseSection(); 
        }

        protected override void DeclareClass(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} structure defition", name);
            WriteStatement(statement);

            OpenSection("struct native", name);

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                // Support Choice!
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            DeclareVariable(arrayItemAttribute.Type, arrayItemAttribute.ElementName, true, ref _varNameSet);
                        }
                        
                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            DeclareVariable(elementAttribute.Type, elementAttribute.ElementName, true, ref _varNameSet);
                        }
                    }
                }
                else if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    DeclareVariable(prop, false, ref _varNameSet);
                }
                else
                {
                    DeclareVariable(prop, true, ref _varNameSet);
                }
            }
            CloseSection();
        }

        protected override void DeclareMemberData(Type typeInfo)
        {
            NewLine(1);

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                // Support Choice!
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            DeclareVariable(arrayItemAttribute.Type, arrayItemAttribute.ElementName, true, ref _varNameSet);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            DeclareVariable(elementAttribute.Type, elementAttribute.ElementName, true, ref _varNameSet);
                        }
                    }
                }
                else if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    DeclareVariable(prop, false, ref _varNameSet);
                }
                else
                {
                    DeclareVariable(prop, true, ref _varNameSet);
                }
            }
        }

        protected override void DeclareMemberFunction()
        {
            NewLine(1);
            string statement = "";

            WriteStatement("native function bool LoadTable( string strFileName );");

            NewLine(1);
            WriteStatement("function bool ClearTable()");
            OpenSection();
            Type rootTypeInfo = GetTypeInfo(ClassName);

            foreach (System.Reflection.PropertyInfo prop in rootTypeInfo.GetProperties())
            {
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            statement = string.Format("{0}s.Length = 0;", arrayItemAttribute.ElementName);
                            WriteStatement(statement);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            statement = string.Format("{0}s.Length = 0;", elementAttribute.ElementName);
                            WriteStatement(statement);
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    statement   = string.Format("{0}s.Length = 0;", prop.Name);
                    WriteStatement(statement);
                }
            }

            WriteStatement("return true;");
            CloseSection();
        }
    }
}
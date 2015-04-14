using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class CPPTreeBuilder
    {
        protected override void DeclareEnum(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} enum class defition", name);
            WriteStatement(statement);

            OpenSection("class", name);
            WriteStatement("public:", -1);
            OpenSection("enum", "Enum");

            // Declare enum
            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = strEnum + ",";
                WriteStatement(statement);
            }
            WriteStatement("INVALID_ENUM,");
            CloseSection();

            // Declare enum class method
            NewLine(1);
            WriteStatement("private:", -1);
            OpenSection("class", "EnumMap : public std::tr1::unordered_map<std::string, Enum>");
            WriteStatement("public:", -1);
            WriteStatement("EnumMap();");
            CloseSection();
            NewLine(1);
            WriteStatement("typedef EnumMap::iterator EnumMapItr;", 0);
            NewLine(1);
            WriteStatement("public:", -1);
            WriteStatement("Enum operator=(int i);", 0);
            WriteStatement("operator int () const;", 0);
            NewLine(1);
            WriteStatement("Enum operator=(const char* sz);", 0);
            WriteStatement("bool operator==(const char* sz);", 0);
            WriteStatement("operator const char* () const;", 0);
            NewLine(1);
            WriteStatement("static Enum AsEnum(const char* sz);", 0);
            WriteStatement("static Enum AsEnum(int e);", 0);
            WriteStatement("static const char* AsName(int e);", 0);

            // Declare enum class member
            NewLine(1);
            WriteStatement("private:", -1);
            WriteStatement("Enum m_Enum;", 0);
            WriteStatement("static EnumMap m_Map;", 0);

            CloseSection();
        }

        protected override void DeclareClass(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} class defition", name);
            WriteStatement(statement);

            OpenSection("class", name);
            WriteStatement("public:", -1);

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                // Root Class!
                bool bStatic = (name == ClassName);

                // Support Choice!
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            DeclareVariable(arrayItemAttribute.Type, arrayItemAttribute.ElementName, true, bStatic, ref _varNameSet);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            DeclareVariable(elementAttribute.Type, elementAttribute.ElementName, true, bStatic, ref _varNameSet);
                        }
                    }
                }
                else if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    DeclareVariable(prop, false, bStatic, ref _varNameSet);
                }
                else
                {
                    DeclareVariable(prop, true, bStatic, ref _varNameSet);
                }
            }
            
            // Declare Destructor!
            NewLine(1);
            statement = string.Format("~{0}()", name);
            WriteStatement(statement);
            OpenSection();

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                NewLine(1);
                // Support Choice!
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            ReleaseVariable(arrayItemAttribute.Type, arrayItemAttribute.ElementName, true, ref _varNameSet);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            ReleaseVariable(elementAttribute.Type, elementAttribute.ElementName, true, ref _varNameSet);
                        }
                    }
                }
                else if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    string propertyName = GetPropertyName(prop);

                    ReleaseVariable(prop.PropertyType, propertyName, ref _varNameSet);
                }
            }

            CloseSection();

            // This Class is a root class!
            if (name == ClassName)
            {
                // Build member function.
                DeclareMemberFunction();
            }

            CloseSection();
        }

        protected override void DeclareMemberData(Type typeInfo)
        {
            NewLine(1);

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    DeclareVariable(prop, false, true, ref _varNameSet);
                }
                else
                {
                    DeclareVariable(prop, true, true, ref _varNameSet);
                }
            }
        }


        protected override void DeclareMemberFunction()
        {
            NewLine(1);

            WriteStatement("// declare member function");
            WriteStatement("static HRESULT LoadTable( const char *strFileName );");
            WriteStatement("static HRESULT ClearTable();");
        }
    }
}

// 트리 빌더 룰!

// 코딩 룰
// 1. NewLine은 코드 시작하기 전에 붙임

// 타입 정의룰
// 1. 컴플렉스타입인 경우 접두사 CT를 붙임
// 2. 타입 접두사 enum은 E, 

// 변수 정의룰
// 1. 맴버 변수인 경우 접두사 m_ 을 추가한다. 
// 1. Attribute인 경우 접두사 m_ 뒤에 타입접두어을 붙인다. ex) m_n


using System;
using System.Collections.Generic;
using System.Linq;

using System.Reflection;

namespace TableBuilder
{
    internal class TreeInfo
    {
        // Element map for TreeInfo!
        private Dictionary<string, ElementInfo> _elementMap = new Dictionary<string, ElementInfo>();
        public Dictionary<string, ElementInfo>  ElementMap
        {
            get { return _elementMap; }
        }

        // Root Name!
        private string _rootName;
        public  string RootName
        {
            get { return _rootName; }
            set { _rootName = value; }
        }
    }


    internal abstract class TreeBuilder : Builder
    {
        private Dictionary<string, TreeInfo>  _schemaInfo         = new Dictionary<string, TreeInfo>();
        protected Dictionary<string, TreeInfo> SchemaInfo
        {
            get { return _schemaInfo; }
        }

        private Dictionary<string, ElementInfo> _elementStructure = new Dictionary<string, ElementInfo>();
        protected Dictionary<string, ElementInfo> ElementStructure
        {
            get { return _elementStructure; }
        }

        private Dictionary<string, string> _complexTypeNameMap     = new Dictionary<string, string>();
        protected Dictionary<string, string> ComplexTypeNameMap
        {
            get { return _complexTypeNameMap; }
        }

        private HashSet<string> _elementNameSet                     = new HashSet<string>();
        protected HashSet<string> ElementNameSet
        {
            get { return _elementNameSet; }
        }

        protected HashSet<string> _varNameSet                         = new HashSet<string>();

        public TreeBuilder(string strBasePath, string BaseParserName) :
            base(strBasePath, BaseParserName)
        {
        }

        protected override bool Initialize(string tableNameSpace, bool isCpp)
        {
            if (!base.Initialize(tableNameSpace,isCpp))
                return false;

            Type typeInfo = GetTypeInfo(ClassName);

            BuildElementNameSet(GetTypeInfo(ClassName), ClassName, ref _elementNameSet);

            BuildSchemaInfo();

            return true;
        }

        protected bool HasAttribute(ElementInfo elementInfo)
        {
            foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
            {
                if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                        HasIgnoreAttribute(prop))
                    continue;

                return true;
            }

            return false;
        }

        protected bool HasAttribute(string elementName)
        {
            Dictionary<string, ElementInfo>.KeyCollection keyCol =  ElementStructure.Keys;

            foreach (string key in keyCol)
            {
                if (key.EndsWith(elementName))
                {
                    foreach (System.Reflection.PropertyInfo prop in ElementStructure[key].TypeInfo.GetProperties())
                    {
                        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                        HasIgnoreAttribute(prop))
                            continue;

                        return true;
                    }
                }
            }

            return false;
        }

        protected void BuildAttributeHashSet(ref HashSet<string> hashSet)
        {
            IEnumerable<KeyValuePair<string, TreeInfo>> schemaInfo = SchemaInfo.Reverse();

            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;
                    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                    {
                        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                            HasIgnoreAttribute(prop))
                            continue;

                        hashSet.Add(prop.Name);
                    }
                }
            }
        }

        protected void BuildElementNameSet(Type typeInfo, string name, ref HashSet<string> hashSet)
        {
            // Pass except class, enum, array, string
            if (!typeInfo.IsClass || typeInfo == "".GetType())
                return;

            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (!prop.PropertyType.IsClass || prop.PropertyType == "".GetType())
                    continue;

                // Make a elementKey!
                string elementName = GetPropertyName(prop);

                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    // Support Choice!
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            BuildElementNameSet(arrayItemAttribute.Type, arrayItemAttribute.ElementName, ref hashSet);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            BuildElementNameSet(elementAttribute.Type, elementAttribute.ElementName, ref hashSet);
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    BuildElementNameSet(GetTypeInfo(typeName), elementName, ref hashSet);
                }
                else
                {
                    BuildElementNameSet(prop.PropertyType, elementName, ref hashSet);
                }

                foreach (object attribute in prop.GetCustomAttributes(true))
                {
                    System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                       = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                    if (arrayItemAttribute != null)
                    {
                        if (elementName == arrayItemAttribute.ElementName)
                        {
                            hashSet.Add(prop.Name);
                        }
                    }
                }
            }

            hashSet.Add(name);
        }

        // Build Element Info
        protected override void BuildElementMap(Type typeInfo, string parentTypeName, string parentElementName, bool isCpp, bool isComplexType, string complexElementName)
        {
            // Pass except class, enum, array, string
            if (!typeInfo.IsClass || typeInfo == "".GetType())
                return;

            string typeName;

            // Build element info per class's 
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                bool bIsComplex = !prop.PropertyType.Name.Contains(typeInfo.Name);

                if (prop.PropertyType.IsArray)
                {
                    typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    if (bIsComplex)
                        BuildElementMap(GetTypeInfo(typeName), typeInfo.Name, "", isCpp, true, prop.Name);
                    else
                        BuildElementMap(GetTypeInfo(typeName), typeInfo.Name, "", isCpp, false, "");
                }
                else
                {
                    if (bIsComplex)
                        BuildElementMap(prop.PropertyType, typeInfo.Name, "", isCpp, true, prop.Name);
                    else
                        BuildElementMap(prop.PropertyType, typeInfo.Name, "", isCpp, false, "");
                }
            }

            // Get element name
            string elementName;
            if (isComplexType)
                elementName = complexElementName;
            else
                elementName = typeInfo.Name.Substring(parentElementName.Length);

            // make a ElementInfo
            ElementInfo elementInfo = new ElementInfo(typeInfo, parentTypeName, typeInfo.Name, parentElementName, elementName);

            try
            {
                ElementMap.Add(typeInfo.Name, elementInfo);
            }
            catch (ArgumentException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return;
            }
        }

        protected void BuildRootName(Type typeInfo)
        {
            // Pass except class, enum, array, string
            if (!typeInfo.IsClass || typeInfo == "".GetType())
                return;

            string typeName = "";

            bool isRoot = false;

            // Check Root
            if (typeInfo.Name.Equals(ClassName))
            {
                isRoot      = true;
                typeName    = typeInfo.Name;
            }
            else
            {
                if (typeInfo.BaseType.Name == "Object")
                    typeName = typeInfo.Name;
                else
                {
                    typeName = typeInfo.BaseType.Name;

                    // Build Complex Type Name Map 
                    ComplexTypeNameMap.Add(typeInfo.BaseType.Name, typeInfo.Name);
                }

                isRoot = !typeName.Contains(ClassName);

                if (isRoot)
                {
                    foreach (KeyValuePair<string, TreeInfo> kvp in SchemaInfo)
                    {
                        TreeInfo treeInfo = kvp.Value;

                        if (typeName.Contains(treeInfo.RootName))
                        {
                            isRoot = false;
                            break;
                        }
                    }
                }
            }

            if (isRoot)
            {
                // AddRootInfo!
                TreeInfo newTreeInfo = new TreeInfo();
                newTreeInfo.RootName = typeName;

                try
                {
                    SchemaInfo.Add(typeName, newTreeInfo);
                }
                catch (ArgumentException e)
                {
                    Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                }
            }

            // Tree traverse!
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute 
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            BuildRootName(arrayItemAttribute.Type);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            BuildRootName(elementAttribute.Type);
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    try
                    {
                        typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                        BuildRootName(GetTypeInfo(typeName));
                    }
                    catch (SystemException e)
                    {
                        Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                        return;
                    }
                }
                else
                {
                    BuildRootName(prop.PropertyType);
                }
            }
        }

        protected void BuildElementInfo(Type typeInfo, string rootName, string parentTypeName, string parentElementName)
        {
            // Pass except class, enum, array, string
            if (!typeInfo.IsClass || typeInfo == "".GetType())
                return;

            // Build element Info
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            BuildElementInfo(arrayItemAttribute.Type, rootName, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length));
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                            BuildElementInfo(elementAttribute.Type, rootName, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length));
                    }

                }
                else if (prop.PropertyType.IsArray)
                {
                    try
                    {
                        string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                        BuildElementInfo(GetTypeInfo(typeName), rootName, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length));
                    }
                    catch (SystemException e)
                    {
                        Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                        return;
                    }
                }
                else
                {
                    BuildElementInfo(prop.PropertyType, rootName, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length));
                }
            }

            if (!typeInfo.Name.Contains(rootName))
                return;

            // make a ElementInfo
            string elementName = GetElementName(parentTypeName, typeInfo.Name);
            //string elementName = typeInfo.Name.Substring(parentTypeName.Length);


            ElementInfo elementInfo = new ElementInfo(typeInfo, parentTypeName, typeInfo.Name, parentElementName, elementName);

            try
            {
                SchemaInfo[rootName].ElementMap.Add(typeInfo.Name, elementInfo);
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return;
            }
        }

        protected void BuildElementStructure(Type typeInfo, string key)
        {
            // Pass except class, enum, array, string
            if (!typeInfo.IsClass || typeInfo == "".GetType())
                return;

            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (!prop.PropertyType.IsClass || prop.PropertyType == "".GetType())
                    continue;

                // Make a elementKey!
                bool bHasElementAttribute = false;
                string elementKey = key;

                foreach (object attribute in prop.GetCustomAttributes(true))
                {
                    System.Xml.Serialization.XmlElementAttribute elementAttribute
                        = attribute as System.Xml.Serialization.XmlElementAttribute;

                    if (elementAttribute != null)
                    {
                        elementKey += "/";
                        elementKey += elementAttribute.ElementName;
                        bHasElementAttribute = true;
                        
                        break;
                    }

                    System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                        = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                    if (arrayItemAttribute != null)
                    {
                        elementKey += "/";
                        elementKey += prop.Name;
                        elementKey += "/";                     
                        elementKey += arrayItemAttribute.ElementName;
   
                        bHasElementAttribute = true;
                        break;
                    }
                }

                if (!bHasElementAttribute)
                {
                    elementKey += "/";
                    elementKey += prop.Name;
                }

                // Support Switch
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            elementKey = key;
                            elementKey += "/";
                            elementKey += prop.Name;
                            elementKey += "/";
                            elementKey += arrayItemAttribute.ElementName;
                            BuildElementStructure(arrayItemAttribute.Type, elementKey);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            elementKey = key;
                            elementKey += "/";
                            elementKey += elementAttribute.ElementName;
                            BuildElementStructure(elementAttribute.Type, elementKey);
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    BuildElementStructure(GetTypeInfo(typeName), elementKey);
                }
                else
                {
                    BuildElementStructure(prop.PropertyType, elementKey);
                }
            }

            // Get ElementInfo
            ElementInfo elementInfo = GetElementInfoByTree(typeInfo.Name);
            if (elementInfo == null)
                return;

            // Add ElementInfo
            try
            {
                ElementStructure.Add(key, elementInfo);
            }
            catch (ArgumentException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return;
            }
        }

        protected void BuildSchemaInfo()
        {
            BuildRootName(GetTypeInfo(ClassName));

            foreach (KeyValuePair<string, TreeInfo> kvp in SchemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;
                BuildElementInfo(GetTypeInfo(treeInfo.RootName), treeInfo.RootName, "", "");
            }

            BuildElementStructure(GetTypeInfo(ClassName), ClassName);
        }

        protected ElementInfo GetElementInfoByTree(string key)
        {
            foreach (KeyValuePair<string, TreeInfo> kvp in SchemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                if (treeInfo.ElementMap.ContainsKey(key))
                    return treeInfo.ElementMap[key];
            }

            Console.WriteLine("{0}-Element Key:{1} don't exist in SchemaInfo!", ClassName, key);
            return null;
        }

        protected ElementInfo GetElementInfoByStructure(string key)
        {
            if (!ElementStructure.ContainsKey(key))
            {
                Console.WriteLine("{0}-Key:{1} don't exist in ElementStructure!", ClassName, key);
                return null;
            }

            return ElementStructure[key];
        }

        protected void GetElementPathList(string elementName, ref List<string> elementPathList)
        {
            Dictionary<string, ElementInfo>.KeyCollection keyCol = ElementStructure.Keys;

            foreach (string key in keyCol)
            {
                if (key.EndsWith(elementName))
                    elementPathList.Add(key);
            }
        }

        //protected bool IsArray(Type typeInfo, string elementName)
        //{
        //    foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
        //    {
        //        if (prop.PropertyType.Name.StartsWith("Object"))
        //        {
        //            foreach (object attribute in prop.GetCustomAttributes(true))
        //            {
        //                System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
        //                    = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

        //                if (arrayItemAttribute != null)
        //                {
        //                    if (IsArray(arrayItemAttribute.Type, elementName))
        //                        return true;
        //                }


        //                System.Xml.Serialization.XmlElementAttribute elementAttribute
        //                        = attribute as System.Xml.Serialization.XmlElementAttribute;

        //                if (elementAttribute != null)
        //                {
        //                    if (IsArray(elementAttribute.Type, elementName))
        //                        return true;
        //                }
        //            }

        //        }
        //        else if (prop.PropertyType.IsArray)
        //        {
        //            string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);

        //            if (typeName == elementName)
        //                return true;

        //            if (IsArray(GetTypeInfo(typeName), elementName))
        //                return true;
        //        }
        //        else
        //        {
        //            if (IsArray(prop.PropertyType, elementName))
        //                return true;
        //        }
        //    }

        //    return false;
        //}

        protected bool IsComplexRoot(string name)
        {
            if (SchemaInfo.ContainsKey(name))
                return true;

            return false;
        }

        protected bool IsComplexType(string typeName)
        {
            foreach (KeyValuePair<string, TreeInfo> kvp in SchemaInfo)
            {
                if (kvp.Key == ClassName)
                    continue;

                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeName == typeName)
                        return true;
                }
            }

            return false;
        }

        protected bool IsSwitchType(string parentTypeName, string elementName)
        {
            foreach (KeyValuePair<string, TreeInfo> kvp in SchemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeName != parentTypeName)
                        continue;

                    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                    {
                        if (!prop.PropertyType.Name.StartsWith("Object"))
                            continue;
                        
                        foreach (object attribute in prop.GetCustomAttributes(true))
                        {
                            System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                            if (arrayItemAttribute != null)
                            {
                                if (arrayItemAttribute.ElementName == elementName)
                                    return true;
                            }

                            System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                            if (elementAttribute != null)
                            {
                                if (elementAttribute.ElementName == elementName)
                                    return true;
                            }
                        }
                    }
                    
                }
            }

            return false;
        }

        protected string GetComplexTypeName(string typeName)
        {
            if (ComplexTypeNameMap.ContainsKey(typeName))
                return ComplexTypeNameMap[typeName];

            return typeName;
        }

        protected string MakeComplexVariable(string path)
        {
            string [] elementNames = path.Split(new Char[] { '/' });

            bool IsRootMember = (elementNames.Length == 2);
           
            string strVariable = "";

            if (!IsRootMember)
                strVariable = "m_p";

            for (int i = 0; i < elementNames.Length; ++i)
            {
                if (i != (elementNames.Length - 1))
                {
                    strVariable += elementNames[i];
                }
                else
                {
                    if (IsRootMember)
                        strVariable += "::m_";
                    else
                        strVariable += "->m_";

                    strVariable += elementNames[i];
                }
            }

            return strVariable;
        }

        protected string GetPropertyName(System.Reflection.PropertyInfo prop)
        {
            if (!prop.PropertyType.IsClass || prop.PropertyType == "".GetType())
                return prop.Name;

            string propertyName = "";
            bool bHasElementAttribute = false;

            foreach (object attribute in prop.GetCustomAttributes(true))
            {
                System.Xml.Serialization.XmlElementAttribute elementAttribute
                        = attribute as System.Xml.Serialization.XmlElementAttribute;

                if (elementAttribute != null)
                {
                    propertyName = elementAttribute.ElementName;
                    bHasElementAttribute = true;
                    break;
                }

                System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                       = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                if (arrayItemAttribute != null)
                {
                    propertyName = arrayItemAttribute.ElementName;
                    bHasElementAttribute = true;
                    break;
                }
            }

            if (!bHasElementAttribute)
                propertyName = prop.Name;

            return propertyName;
        }

        protected string GetElementName(string parentTypeName, string typeName)
        {
            string elementName;
            // make a ElementInfo
            if (typeName.Contains(parentTypeName))
            {
                elementName = typeName.Substring(parentTypeName.Length);

                if (ElementNameSet.Contains(elementName))
                    return elementName;

                while (elementName.Length > 0)
                {
                    elementName = elementName.Substring(0, elementName.Length - 1);
                    if (!ElementNameSet.Contains(elementName))
                        continue;

                    return elementName;
                }
            }
            else
            {
                while (parentTypeName.Length > 0)
                {
                    parentTypeName = parentTypeName.Substring(0, parentTypeName.Length - 1);
                    if (!typeName.Contains(parentTypeName))
                        continue;

                    elementName = typeName.Substring(parentTypeName.Length);

                    while (elementName.Length > 0)
                    {
                        elementName = elementName.Substring(0, elementName.Length - 1);
                        if (!ElementNameSet.Contains(elementName))
                            continue;

                        return elementName;
                    }
                }
            }

            Console.WriteLine("{0}-Invalid ParentTypeName:{1}, TypeName:{2}", ClassName, parentTypeName, typeName);
            return "";
        }

        protected string GetVarName(string name, ref HashSet<string> nameList)
        {
            string newName = name;

            if (!nameList.Contains(newName))
            {
                nameList.Add(newName);
                return newName;
            }

            for (int i = 1; ; ++i)
            {
                newName = name + i;
                if (!nameList.Contains(newName))
                {
                    nameList.Add(newName);
                    return newName;
                }
            } 
        }

        protected string GetVarName(string className, string varName, ref HashSet<string> nameList)
        {
            string newName = className + varName;

            if (!nameList.Contains(newName))
            {
                nameList.Add(newName);
                return varName;
            }

            for (int i = 1; ; ++i)
            {
                newName = className + varName + i;
                if (!nameList.Contains(newName))
                {
                    nameList.Add(newName);
                    return varName + i;
                }
            }
        }

        protected string GetBuiltInTypeName(PropertyInfo prop)
        {
            string typeName = "";
            bool bFindTypeName = false;

            foreach (object attribute in prop.GetCustomAttributes(true))
            {
                System.Xml.Serialization.XmlAttributeAttribute attributeAttribute
                        = attribute as System.Xml.Serialization.XmlAttributeAttribute;

                if (attributeAttribute != null && attributeAttribute.DataType != "")
                {
                    typeName = GetBuiltInType(attributeAttribute.DataType);
                    bFindTypeName = true;
                }
            }

            if (!bFindTypeName)
                typeName = GetBuiltInType(prop.PropertyType.Name);

            return typeName;
        }
    }
}

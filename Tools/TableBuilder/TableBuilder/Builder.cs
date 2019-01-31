////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Table builder base class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;


using System.Reflection;






namespace TableBuilder
{
    internal class ElementInfo
    {
        public ElementInfo(Type typeInfo, string parentTypeName,
                string typeName, string parentElementName, string elementName)
        {
            _typeInfo           = typeInfo;
            _parentTypeName     = parentTypeName;
            _typeName           = typeName;
            _parentElementName = parentElementName;
            _elementName        = elementName;
        }

        private Type _typeInfo;
        public Type TypeInfo
        {
            get { return _typeInfo; }
        }

        private string _parentTypeName;
        public string ParentTypeName
        {
            get { return _parentTypeName; }
        }

        private string _typeName;
        public string TypeName
        {
            get { return _typeName; }
        }

        private string _parentElementName;
        public string ParentElementName
        {
            get { return _parentElementName; }
        }

        private string _elementName;
        public string ElementName
        {
            get { return _elementName; }
        }

        public string OutputTypeName
        {
            get { return string.IsNullOrEmpty(ParentTypeName) ? TypeName : ParentTypeName + "::" + TypeName; }
        }
    }

    /// <summary>
    /// Builder class for building interface
    /// </summary>
    internal abstract class Builder
    {
        // Class map for  builders
        static private Dictionary<string, Type> _classMap;

        // Base name space
        static public string _tableNameSpace = "GameTbl";

        // class name to export
        private string _className;
        protected string ClassName
        {
            get { return _className; }
            set { _className = value; }
        }

        // parsing class name
        private string _baseParserName;
        protected string BaseParserName
        {
            get { return _baseParserName; }
            set { _baseParserName = value; }
        }

        private string _parserName;
        protected string ParserName
        {
            get { return _parserName; }
            set { _parserName = value; }
        }

        //// Exclude these when build 
        //protected Dictionary<string, Type> _excludeMap;

        // Out file base path
        private string _BasePath;
        public string BasePath
        {
            get { return _BasePath; }
            set { _BasePath = value; }
        }

        // Output file stream
        private FileStream _outFile;
        private StreamWriter _outStream;
        //////////////////////////////////////////////////////////////////////////
        // output stream management
        public StreamWriter OutStream
        {
            get { return _outStream; }
        }

        // Tab indent count
        private int _Indent;

        // Section stack
        private Stack<string> _sectionStack = new Stack<string>();

        // C# -> C++ or C# -> UC Type conversion map
        protected Dictionary<string, string> _typeConvMap = new Dictionary<string, string>();

        // Element map for builders
        protected Dictionary<string, ElementInfo> _elementMap = new Dictionary<string, ElementInfo>();
        protected Dictionary<string, ElementInfo>  ElementMap
        {
            get { return _elementMap; }
        }

        protected List<ElementInfo> _elementList = new List<ElementInfo>();

        protected Dictionary<string, string> _nameMap = new Dictionary<string, string>();

        // List to include At header/cpp
        protected List<string> _includesAtHeader = new List<string>();
        protected List<string> _includesAtCpp = new List<string>();


        protected virtual bool Initialize(string tableNameSpace, bool isCpp)
        {
            if (_classMap.Count == 0)
            {
                UpdateClassList(tableNameSpace);
            }

            Type typeInfo = GetTypeInfo(ClassName);
            // Validate root info
            if (typeInfo == null)
            {
                Console.WriteLine("Table: {0}: Root element name don't identity with ClassName({0}).", ClassName);
                return false;
            }

            // Build Type Info!
            BuildTypeConvMap();

            

            return true;
        }


        /// <summary>
        /// Method to populate a list with all the class
        /// in the namespace provided by the user
        /// </summary>
        /// <param name="nameSpace">The namespace the user wants searched</param>
        /// <returns></returns>
        static public void UpdateClassList(string nameSpace)
        {
            //create an Assembly and use its GetExecutingAssembly Method
            // http://msdn2.microsoft.com/en-us/library/system.reflection.assembly.getexecutingassembly.aspx
            Assembly asm = Assembly.GetExecutingAssembly();
            //create a list for the namespaces
            _classMap = new Dictionary<string, Type>();

            _tableNameSpace = nameSpace;


            //loop through all the "Types" in the Assembly using
            //the GetType method:
            // http://msdn2.microsoft.com/en-us/library/system.reflection.assembly.gettypes.aspx

            foreach (Type classType in asm.GetTypes())
            {
                if (classType.Namespace == nameSpace)
                {
                    _classMap.Add(classType.Name, classType);
                    //Console.WriteLine("Element Type:{0}", classType.Name);
                }
            }
        }


        //std::vector - List<T>
        //std::list - LinkedList<T>
        //std::map - SortedDictionary<Tkey, Tvalue>
        //std::set - SortedDictionary<Tkey, Tvalue> (with null values)
        //std::multimap - SortedDictionary<Tkey, List<Tvalue>>
        //std::multiset - SortedDictionary<Tkey, int> (with int keeping count of the number of Tkeys) 
        //std::unordered_map - Dictionary<Tkey, Tvalue>
        //std::unordered_set - Dictionary<Tkey, Tvalue> (with null values)
        //std::unordered_multimap - Dictionary<Tkey, List<Tvalue>>
        //std::unordered_multiset - Dictionary<Tkey, int> (with int keeping count of the number of Tkeys) 

        //System.Collections.Generic.

        static void DumpTypeInfo(string strIndent, Type InType)
        {


            //Console.Write(strIndent + "Class " + InType.Name);
            //Console.Write(", Type:" + InType.Name);
            //Console.Write(", Enum:" + InType.IsEnum);
            //Console.Write(", Array:" + InType.IsArray);
            //Console.WriteLine(", IsClass:" + InType.IsClass);
            if (InType.IsEnum)
            {
                //Console.WriteLine(strIndent + "Enums");
                //Console.Write(strIndent);
                //foreach (string strEnum in Enum.GetNames(InType))
                //    Console.WriteLine("        " + strEnum);
                //Console.WriteLine("");
            }
            else if (InType == "".GetType())
            {
                // Nothing any info with string type
            }
            else if (InType.IsClass)
            {
                if (InType.IsArray)
                {
                    // Do something when array
                }
            }

            strIndent += "    ";
            foreach (System.Reflection.PropertyInfo prop in InType.GetProperties())
            {
                Console.Write(strIndent + "Name:" + prop.Name);
                //Console.Write(", Type:" + prop.PropertyType.Name);
                //Console.Write(", Enum:" + prop.PropertyType.IsEnum);
                //Console.Write(", Array:" + prop.PropertyType.IsArray);
                //Console.WriteLine(", IsClass:" + prop.PropertyType.IsClass);
                if (prop.PropertyType.IsArray)
                {
                    string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    DumpTypeInfo(strIndent, _classMap[typeName]);
                }
                else
                    DumpTypeInfo(strIndent, prop.PropertyType);
            }

        }

        // Get type.
        public static Type GetTypeInfo(string typeName)
        {
            Type TypeInfo;

            try
            {
                TypeInfo = _classMap[typeName];
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}", e.Message, e.StackTrace);
                return null;
            }

            return TypeInfo;
        }


        // Build Type Name Info!
        protected void BuildNameMap(Type typeInfo, string ParentName, bool isCpp)
        {
            // Pass except class, enum, array
            if (typeInfo.IsArray == false &&
                typeInfo.IsClass == false &&
                typeInfo.IsEnum == false)
                return;

            if (typeInfo == "".GetType())
                return;

            string typeName = "";


            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (prop.PropertyType.IsArray)
                {
                    typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    BuildNameMap(GetTypeInfo(typeName), typeInfo.Name, isCpp);
                }
                else
                {
                    BuildNameMap(prop.PropertyType, typeInfo.Name, isCpp);
                }
            }

            // If it is a root, pass.
            if (typeInfo.Name == ClassName)
                return;

            if (isCpp)
            {
                if (typeInfo.Name.Contains(ParentName))
                    typeName = typeInfo.Name.Substring(ParentName.Length);
                else
                    typeName = typeInfo.Name;
            }
            else
                typeName = typeInfo.Name;

            if (typeInfo.IsEnum)
                typeName = "E" + typeName;

            try
            {
                _nameMap.Add(typeInfo.Name, typeName);
            }
            catch (ArgumentException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return;
            }
        }

        protected string GetTypeName(Type typeInfo)
        {
            string typeName;

            try
            {
                typeName = _nameMap[typeInfo.Name];
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return null;
            }


            return typeName;
        }


        protected string GetTypeName(string keyName)
        {
            string typeName;

            try
            {
                typeName = _nameMap[keyName];
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return null;
            }


            return typeName;
        }

        protected string GetBuiltInType(string typeName)
        {
            string BuiltInTypeName;

            try
            {
                BuiltInTypeName = _typeConvMap[typeName];
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return null;
            }

            return BuiltInTypeName;
        }

        // Build Element Info
        protected virtual void BuildElementMap(Type typeInfo, string parentTypeName, string parentElementName, bool isCpp, bool isComplexType, string complexElementName)
        {
            // Pass except class, enum, array
            if (!typeInfo.IsClass)
                return;

            // Pass string type
            if (typeInfo == "".GetType())
                return;

            string typeName = "";

            // Build element info per class's 
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                bool bIsComplex = !prop.PropertyType.Name.Contains(typeInfo.Name);
                

                if (prop.PropertyType.IsArray)
                {
                    typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);

                    
                    if (bIsComplex)
                        BuildElementMap(GetTypeInfo(typeName), typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length), isCpp, true, prop.Name);
                    else
                        BuildElementMap(GetTypeInfo(typeName), typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length), isCpp, false, "");
                }
                else
                {
                    if (bIsComplex)
                        BuildElementMap(prop.PropertyType, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length), isCpp, true, prop.Name);
                    else
                        BuildElementMap(prop.PropertyType, typeInfo.Name, typeInfo.Name.Substring(parentTypeName.Length), isCpp, false, "");
                }
            }

            // Get element name
            string elementName = "";
            if (isComplexType)
                elementName = complexElementName;
            else
                elementName = typeInfo.Name.Substring(parentTypeName.Length);

            // Get type name
            if (isCpp && !isComplexType)
                typeName = elementName;
            else
                typeName = typeInfo.Name;

            // make a ElementInfo
            ElementInfo elementInfo = new ElementInfo(typeInfo, parentTypeName, typeName, parentElementName, elementName);

            try
            {
                ElementMap.Add(typeName, elementInfo);
            }
            catch (ArgumentException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return;
            }
        }

        protected virtual ElementInfo GetElementInfo(string typeName)
        {
            ElementInfo elementInfo;

            try
            {
                elementInfo = ElementMap[typeName];
            }
            catch (KeyNotFoundException e)
            {
                Console.WriteLine("{0}-{1}-{2}", ClassName, e.Message, e.StackTrace);
                return null;
            }

            return elementInfo;
        }

        // Ignore XmlIgnoreAttribute 
        protected bool HasIgnoreAttribute(System.Reflection.PropertyInfo Property)
        {
            object[] Attributes = Property.GetCustomAttributes(typeof(System.Xml.Serialization.XmlIgnoreAttribute), true);

            if (Attributes.Length > 0)
                return true;

            return false;
        }

        protected void OpenOutFile(string OutputFileName, bool isCpp)
        {
            string strFilePath = GetOutputFilePath();

            if (Directory.Exists(strFilePath) == false)
                Directory.CreateDirectory(strFilePath);


            strFilePath += OutputFileName;

            _outFile = new FileStream(strFilePath, FileMode.Create, FileAccess.Write, FileShare.Read);
            if (isCpp)
                _outStream = new StreamWriter(_outFile, Encoding.UTF8);
            else
                _outStream = new StreamWriter(_outFile, Encoding.ASCII);

            IndentReset();
        }

        protected void CloseOutFile()
        {
            // Close all section
            CloseAllSection();
            NewLine(2);

            // close stream
            _outStream.Close();
            _outFile.Close();

            _outStream = null;
            _outFile = null;
        }


        ////////////////////////////////////////////////////////////////////////
        // Utility 
        protected bool IsArray(Type typeInfo, string elementName)
        {
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
                            if (IsArray(arrayItemAttribute.Type, elementName))
                                return true;
                        }


                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            if (IsArray(elementAttribute.Type, elementName))
                                return true;
                        }
                    }

                }
                else if (prop.PropertyType.IsArray)
                {
                    string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);

                    if (typeName == elementName)
                        return true;

                    if (IsArray(GetTypeInfo(typeName), elementName))
                        return true;
                }
                else
                {
                    if (IsArray(prop.PropertyType, elementName))
                        return true;
                }
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////
        // Indent management
        public void IndentInc(bool bSync /*= true*/ )
        {
            _Indent++;
            if (bSync)
                _outStream.Write(" ");
        }

        public void IndentDec()
        {
            _Indent--;
        }

        public void IndentReset()
        {
            _Indent = 0;
        }

        public void NewLine(int iNumNewLine /*= 1*/)
        {
            for (int iNew = 0; iNew < iNumNewLine; iNew++)
                _outStream.WriteLine();
        }

        public void MatchIndent(int iDiff /*= 0*/)
        {
            for (int iIndent = 0; iIndent < (_Indent + iDiff); iIndent++)
            {
                _outStream.Write("\t");
            }
        }



        //////////////////////////////////////////////////////////////
        // Section management
        public void OpenSection(string sectionType, string sectionName)
        {
            string strSection = sectionType + " " + sectionName;
            MatchIndent(0); _outStream.Write(strSection);
            NewLine(1);
            MatchIndent(0); _outStream.Write("{");
            NewLine(1);

            _sectionStack.Push("}; // " + strSection);

            IndentInc(true);
        }

        public void OpenSection()
        {
            MatchIndent(0); _outStream.Write("{");
            NewLine(1);

            _sectionStack.Push("}");

            IndentInc(true);
        }

        public void CloseSection()
        {
            string strSection = _sectionStack.Pop();

            IndentDec();

            MatchIndent(0); _outStream.Write(strSection);
            NewLine(1);
        }

        public void CloseAllSection()
        {
            while (_sectionStack.Count > 0)
                CloseSection();
        }


        //////////////////////////////////////////////////////////////
        // Statement management
        protected void WriteStatement(string statement)
        {
            MatchIndent(0);
            _outStream.WriteLine(statement);
        }

        protected void WriteStatement(string statement, params object[] args)
        {
            MatchIndent(0);
            _outStream.WriteLine(statement, args);
        }


        protected void WriteStatement(string statement, int diff)
        {
            MatchIndent(diff);
            _outStream.WriteLine(statement);
        }

        // constructor
        public Builder(string strBasePath, string baseParserName)
        {
            _BasePath = strBasePath;
            _baseParserName = baseParserName;
        }


        //////////////////////////////////////////////////////////////////////////
        //
        //  Virtual interfaces
        //
        protected abstract string GetOutputFilePath();
        protected abstract void BuildTypeConvMap();

        public abstract void Build(string tableNameSpace);

        // Build declaration
        protected abstract void DeclareEnum(string name, Type typeInfo);
        protected abstract void DeclareClass(string name, Type typeInfo);
        protected abstract void DefineMemberVariables(Type rootTypeInfo);
        protected abstract void DeclareMemberFunction();

        // Build definition
        protected virtual void DefineTableParser(Type rootTypeInfo) { }
        protected abstract void DefineEnumDefinition(string name, Type typeInfo);
        protected abstract void DefineMemberFunction();
    }
}

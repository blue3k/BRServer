using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BR.ToolLib
{
    public static class ToolEnvironment
    {
        static Dictionary<string, string> m_Settings = new Dictionary<string, string>();

        static string[] m_EnvironmentSettingNames = new string[]
        {
            "BASE_PATH",
            "DEBUG_TOOLS",
        };



        static ToolEnvironment()
        {
            // apply environmane
            foreach (var environmentName in m_EnvironmentSettingNames)
            {
                var settingName = Environment.GetEnvironmentVariable(environmentName);
                var environmentNameLwr = environmentName.ToLower();
                m_Settings.Add(environmentNameLwr, settingName);
            }

            string[] args = Environment.GetCommandLineArgs();
            for (int iArg = 1; iArg < args.Length; iArg++ )
            {
                string argument = args[iArg];
                bool isRemove = argument.StartsWith("-");
                if (isRemove || argument.StartsWith("+"))
                {
                    argument = argument.Substring(1);
                }
                else
                {
                    string value = "";
                    if (m_Settings.TryGetValue("", out value))
                        value += ",";
                    value += argument;
                    m_Settings["UNNAMED"] = value;
                }

                if (string.IsNullOrEmpty(argument))
                {
                    continue;
                }

                string argumentValue = "";
                int splitIndex = argument.IndexOf(':');
                if (splitIndex < 0)
                {
                    argumentValue = "true";
                }
                else
                {
                    argumentValue = argument.Substring(splitIndex + 1);
                    argument = argument.Substring(0, splitIndex);
                }

                argument = argument.ToLower();
                if (isRemove)
                {
                    m_Settings.Remove(argument);
                }
                else
                {
                    m_Settings.Add(argument, argumentValue);
                }
            }


        }

        public static string GetSettingString(string key, string defaultValue = "")
        {
            key = key.ToLower();
            string stringValue = defaultValue;
            m_Settings.TryGetValue(key, out stringValue);
            return stringValue;
        }

        public static T GetSetting<T>(string key, T defaultValue = default(T))
        {
            key = key.ToLower();
            string stringValue = "";
            if(m_Settings.TryGetValue(key, out stringValue))
            {

            }

            T resultValue = defaultValue;
            try
            {
                if (!string.IsNullOrEmpty(stringValue))
                    resultValue = (T)Convert.ChangeType(stringValue, typeof(T));
            }
            catch(Exception)
            {
                resultValue = defaultValue;
            }

            return resultValue;
        }



        public static void CheckDebugOption(string toolName)
        {
            toolName = toolName.ToLower();
            string debugTools = GetSettingString("DEBUG_TOOLS");
            if (!string.IsNullOrEmpty(debugTools))
            {
                foreach (var debugTool in debugTools.Split(','))
                {
                    if (debugTool.ToLower() == toolName)
                    {
                        System.Diagnostics.Debug.Assert(false);
                        return;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : 
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BR.ToolLib
{
    public static class ToolEnvironment
    {
        static Setting m_Settings = new Setting();


        static void ImportEnvironmentVariables()
        {
            var envVars = Environment.GetEnvironmentVariables();
            if (envVars.Contains("DEFINES"))
            {
                var defineVar = envVars["DEFINES"] as string;
                var defines = defineVar.Split(';');

                foreach (var define in defines)
                {
                    var keyValueSet = define.Split('=');
                    if (keyValueSet.Length == 0) continue;

                    m_Settings.SetValue("DEFINE", keyValueSet[0], keyValueSet.Length > 1 ? keyValueSet[1] : "");
                }
            }

            if (envVars.Contains("TARGET_PLATFORM"))
            {
                var defineVar = envVars["TARGET_PLATFORM"] as string;
                m_Settings.SetValue("TARGET_PLATFORM", defineVar);
            }

            if (envVars.Contains("BASE_PATH"))
            {
                var defineVar = envVars["BASE_PATH"] as string;
                m_Settings.SetValue("BASE_PATH", defineVar);
            }

            if (envVars.Contains("TOOL_DEBUG"))
            {
                var defineVar = envVars["TOOL_DEBUG"] as string;
                if (!string.IsNullOrEmpty(defineVar))
                {
                    var tool_debugs = defineVar.Split(';');
                    foreach (var tool_debug in tool_debugs)
                    {
                        m_Settings.SetValue("TOOL_DEBUG", tool_debug, "");
                    }
                }
            }
        }


        static void ImportCommandParameter()
        {
            string[] args = Environment.GetCommandLineArgs();
            m_Settings.ProcessParameter(args);
        }

        static ToolEnvironment()
        {
            ImportEnvironmentVariables();
            ImportCommandParameter();
        }

        public static string GetSettingString(string key, string defaultValue = "")
        {
            return m_Settings.GetValue<string>(key, defaultValue);
        }

        public static T GetSetting<T>(string key, T defaultValue = default(T))
        {
            return m_Settings.GetValue<T>(key, defaultValue);
        }

        static public SettingValueSet GetValueSet(string key)
        {
            return m_Settings.GetValueSet(key);
        }



        public static void CheckDebugOption(string toolName)
        {
            toolName = toolName.ToLower();
            string debugTools = GetSettingString("TOOL_DEBUG");
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

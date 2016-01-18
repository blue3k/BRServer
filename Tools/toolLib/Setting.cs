using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace BR.ToolLib
{
    public class Setting
    {
        Dictionary<string, SettingValue> m_SavedConfiguration;
        Dictionary<string, SettingValue> m_Configurations = new Dictionary<string, SettingValue>();

        public Setting()
        {
        }

        public void SaveConfig()
        {
            m_SavedConfiguration = m_Configurations;

            m_Configurations = new Dictionary<string, SettingValue>();
            foreach (var config in m_SavedConfiguration)
            {
                var clone = config.Value.Clone();
                m_Configurations.Add(config.Key, clone);
            }
        }

        public void RestoreConfig()
        {
            m_Configurations = m_SavedConfiguration;
        }

        public void ParseArgument(string argument)
        {
            if (string.IsNullOrEmpty(argument)) return;

            if (argument.StartsWith("-"))
            {
                argument = argument.Substring(1);
                string[] argSet = argument.Split(':');
                if (argSet.Length == 0) return;
                string key = argSet[0].ToLower();

                // remove value
                var value = GetValue(key);
                if (value is SettingValueString)
                {
                    RemoveValue(key);
                }
                else if (argSet.Length > 1)
                {
                    ((SettingValueSet)value).RemoveValue(argSet[1]);
                }
            }
            else if (argument.StartsWith("+"))
            {
                argument = argument.Substring(1);
                string[] argSet = argument.Split(':');
                if (argSet.Length == 0) return;
                string key = argSet[0].ToLower();

                var keyValue = GetValue(key);
                if (keyValue is SettingValueString)
                {
                    var strValue = keyValue as SettingValueString;
                    RemoveValue(strValue.Key);
                    SetValue(strValue.Key, strValue.Value, "");
                    keyValue = null;
                }

                if (keyValue == null)
                {
                    if (argSet.Length > 2)
                        SetValue(key, argSet[1], argSet[2]);
                    else
                        SetValue(key, argSet[1]);
                }
                else
                {
                    SettingValueSet keyValueSet = keyValue as SettingValueSet;
                    if (argSet.Length > 2)
                        keyValueSet.SetValue(argSet[1], argSet[2]);
                    else
                        keyValueSet.SetValue(argSet[1], "");
                }
            }
            else
            {
                // it's set, replace the value
                string[] argSet = argument.Split(':');
                if (argSet.Length == 0) return;
                string key = argSet[0].ToLower();
                string value = argSet.Length > 0 ? argSet[1] : "";

                var keyValue = GetValue(key);
                if(keyValue != null)
                {
                    if (keyValue is SettingValueString)
                    {
                        var strValue = keyValue as SettingValueString;
                        strValue.Value = value;
                    }
                    else
                    {
                        ToolDebug.Error("failed to replace setting {0} to {1}", key, value);
                    }
                }
                else
                {
                    SetValue(key, value);
                }
            }
        }

        public void ProcessParameter(string[] args)
        {
            for (int iArg = 1; iArg < args.Length; iArg++)
            {
                string argument = args[iArg];
                ParseArgument(argument);
            }
        }
        
        #region Enumerator implementation
        public class Enumerator : IEnumerator<SettingValue>
        {
            Dictionary<string, SettingValue> m_Map;
            Dictionary<string, SettingValue>.Enumerator m_Enumerator;

            public Enumerator(Dictionary<string, SettingValue> values)
            {
                m_Map = values;
                m_Enumerator = m_Map.GetEnumerator();
            }

            public void Dispose()
            {
            }

            public bool MoveNext() { return m_Enumerator.MoveNext(); }
            public void Reset() { m_Enumerator = m_Map.GetEnumerator(); }

            public SettingValue Current { get { return m_Enumerator.Current.Value; } }
            object IEnumerator.Current { get { return m_Enumerator.Current.Value; } }
        }

        //IEnumerator IEnumerable.GetEnumerator() { return new Enumerator(m_Configurations); }

        public IEnumerator<SettingValue> GetEnumerator() { return new Enumerator(m_Configurations); }

        #endregion Enumerator

        public void RemoveValue(string key)
        {
            //if (m_Configurations.ContainsKey(key))
            m_Configurations.Remove(key);
        }

        public SettingValue GetValue(string key)
        {
            SettingValue value;
            m_Configurations.TryGetValue(key.ToLower(), out value);
            return value;
        }

        public ReturnType GetValue<ReturnType>(string key, ReturnType defaultValue = default(ReturnType))
        {
            var valueString = GetValueString(key);
            if (string.IsNullOrEmpty(valueString)) return defaultValue;
            return (ReturnType)Convert.ChangeType((object)valueString, typeof(ReturnType));
        }

        public SettingValueSet GetValueSet(string key)
        {
            SettingValue value;
            key = key.ToLower();
            if (m_Configurations.TryGetValue(key.ToLower(), out value))
            {
                var valueSet = value as SettingValueSet;
                if (valueSet != null) return valueSet;

                // wrap the value as value set
                valueSet = new SettingValueSet(key.ToLower());
                valueSet.SetValue(value.Key, (value as SettingValueString).Value);
            }
            return null;
        }

        public string GetValueString(string key, string defaultValue = "")
        {
            SettingValue value;
            key = key.ToLower();
            if (!m_Configurations.TryGetValue(key.ToLower(), out value)) return defaultValue;
            var stringValue = value as SettingValueString;
            if (stringValue == null) return defaultValue;

            return stringValue.Value;
        }

        public int GetValueInt(string key, int defaultValue)
        {
            SettingValue value;
            key = key.ToLower();
            if (!m_Configurations.TryGetValue(key.ToLower(), out value)) return defaultValue;
            var stringValue = value as SettingValueString;
            if (stringValue == null) return defaultValue;

            int result;
            if (!int.TryParse(stringValue.Value, out result)) return defaultValue;

            return result;
        }

        public float GetValueFloat(string key, float defaultValue)
        {
            SettingValue value;
            key = key.ToLower();
            if (!m_Configurations.TryGetValue(key.ToLower(), out value)) return defaultValue;
            var stringValue = value as SettingValueString;
            if (stringValue == null) return defaultValue;

            float result;
            if (!float.TryParse(stringValue.Value, out result)) return defaultValue;

            return result;
        }

        // set value
        public void SetValue(string key, string value)
        {
            string keyLwr = key.ToLower();
            var keyValue = GetValue(keyLwr);
            if (keyValue == null)
            {
                m_Configurations.Add(keyLwr, new SettingValueString(keyLwr, value));
            }
            else if (keyValue is SettingValueSet)
            {
                var valueSet = (SettingValueSet)keyValue;
                valueSet.SetValue(value, value);
            }
            else if(keyValue is SettingValueString)
            {
                var valueString = (SettingValueString)keyValue;
                valueString.Value = value;
            }
            else
            {
                ToolDebug.Error("Key type not match for key:{0}, value:{1}", key, value);
            }
        }

        // Add value to a value set
        public void SetValue(string keySet, string key, string value)
        {
            string keySetLwr = keySet.ToLower();
            var keyValue = GetValue(keySetLwr);
            if (keyValue == null)
            {
                keyValue = new SettingValueSet(keySetLwr);
                m_Configurations.Add(keySetLwr, keyValue);
            }

            if (keyValue is SettingValueSet)
            {
                var valueSet = (SettingValueSet)keyValue;
                valueSet.SetValue(key, value);
            }
            else
            {
                ToolDebug.Error("Key type not match for key:{0}, value:{1}", key, value);
            }
        }

    }
}

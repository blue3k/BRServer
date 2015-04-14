using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace BR
{
    static class BRDebug
    {
        static public void Log( string message )
        {
            System.Diagnostics.Debug.Print(message);
        }

        static public void DebugLog( string message )
        {
            System.Diagnostics.Debug.Print(message);
        }

        static public void Warning(string message)
        {
            System.Diagnostics.Debug.Print("Warning: "+message);
        }

        static public void Error(string message)
        {
            System.Diagnostics.Debug.Print("Error: " + message);
        }

        static public void Assert(bool bCondition)
        {
            System.Diagnostics.Debug.Assert(bCondition);
        }

        static public void Assert(bool bCondition, string message)
        {
            System.Diagnostics.Debug.Assert(bCondition,message);
        }
    }
}

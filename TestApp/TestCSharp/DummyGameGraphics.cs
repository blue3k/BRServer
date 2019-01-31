using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace BR
{

}

namespace BR.GameGraphics
{
    static class Dummy
    {
    }
}

namespace MonoGame.Framework
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct | AttributeTargets.Enum | AttributeTargets.Constructor | AttributeTargets.Method | AttributeTargets.Property | AttributeTargets.Field | AttributeTargets.Event | AttributeTargets.Interface | AttributeTargets.Delegate)]
    public class PreserveAttribute : Attribute
    {
        public bool Conditional;

        public bool AllMembers;

    }
}

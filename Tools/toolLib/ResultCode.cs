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
    public struct ResultCode
    {
        #region Constants

        const int CodeBits = 16;
        const int CodeMask = (1 << CodeBits) - 1;
        const int CodeShift = 0;

        const int FacilityBits = 12;
        const int FacilityMask = (1 << FacilityBits) - 1;
        const int FacilityShift = (CodeBits + CodeShift);

        const int ReservedBits = 1;
        const int ReservedMask = (1 << ReservedBits) - 1;
        const int ReserveShift = (FacilityBits + FacilityShift);

        const int CustomBits = 1;
        const int CustomMask = (1 << CustomBits) - 1;
        const int CustomShift = (ReservedBits + ReserveShift);

        const int SeverityBits = 2;
        const int SeverityMask = (1 << SeverityBits) - 1;
        const int SeverityShift = (CustomBits + CustomShift);

        #endregion

        public enum SeverityType
        {
            Success,
            Informational,
            Warning,
            Error
        };

        public static string ServerityToDefineString(SeverityType serverity)
        {
            switch (serverity)
            {
                case SeverityType.Success: return "S";
                case SeverityType.Informational: return "I";
                case SeverityType.Warning: return "W";
                case SeverityType.Error:
                default:
                    return "E";
            }
        }

        public Int32 ID;

        public ResultCode(Int32 inID)
        {
            ID = inID;
        }

        public ResultCode(SeverityType severity, bool custom, int facility, int code)
        {
            ID = 0;
            Severity = severity;
            Custom = custom;
            Facility = facility;
            Code = code;
        }

        public SeverityType Severity
        {
            get { return (SeverityType)((ID >> SeverityShift) & SeverityMask); }
            set { ID = (ID & (~(SeverityMask << SeverityShift))) | (((int)value & SeverityMask) << SeverityShift); }
        }

        public bool Custom
        {
            get { return ((ID >> CustomShift) & CustomMask) != 0; }
            set { ID = (ID & (~(CustomMask << CustomShift))) | (((value?1:0) & CustomMask) << CustomShift); }
        }

        public int Facility
        {
            get { return (ID >> FacilityShift) & FacilityMask; }
            set { ID = (ID & (~(FacilityMask << FacilityShift))) | ((value & FacilityMask) << FacilityShift); }
        }

        public int Code
        {
            get { return (ID >> CodeShift) & CodeMask; }
            set { ID = (ID & (~(CodeMask << CodeShift))) | ((value & CodeMask) << CodeShift); }
        }
    }
}

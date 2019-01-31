////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : system types
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;


namespace BR
{

    public class Date
    {
        public const int ReferenceYear = 2010;
        static int[] MonthDays = new int [12] { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        public int Year;
        public int Month;
        public int Day;
        public int Hour;
        public int Min;
        public int Sec;

        public bool IsLeapYear
        {
            get { return IsThisLeapYear(Year); }
        }

        public Date()
        {
            Year = 0;
            Month = 0;
            Day = 0;
            Hour = 0;
            Min = 0;
            Sec = 0;
        }

        public Date(uint inTime)
        {
            SetTime(inTime);
        }

        public void SetTime( uint inTime )
        {
            // When the most significient bit is set, simple modulo operation will failed with signed type
            int time = (int)(inTime / 60);
            Sec = (int)(inTime - ((uint)time * 60));

            Min = time % 60;
            time /= 60;

            Hour = time % 24;
            time /= 24;

            int remainDays = time;
            // Calculate year from the reference year
            int year = ReferenceYear;
            for (; remainDays >= 365; year++)
            {
                if (IsThisLeapYear(year))
                {
                    if (remainDays < 366) break;
                    remainDays -= 366;
                }
                else
                {
                    remainDays -= 365;
                }
            }
            Year = year;

            // Calculate month
            int month = 0;
            for (; month < 12 && remainDays >= MonthDays[month]; month++)
            {
                remainDays -= MonthDays[month];
            }
            Month = month;
            BRDebug.Assert(month < 12);
            BRDebug.Assert(remainDays < MonthDays[month]);

            Day = remainDays;
        }


        static public bool IsThisLeapYear(int year)
        {
            return (year % 4) == 0
                && ((year % 100) != 0 || (year % 400) == 0);
        }
    };


    // converter between time32 to DateTime
    public struct TimeUTC
    {
        // Server Reference year is 2014
        const int UTCReferenceYear = 2014;

        static public DateTime ToDateTime(uint Time)
        {
            DateTime origin = new DateTime(UTCReferenceYear, 1, 1, 0, 0, 0, 0);
            return origin.AddSeconds(Time);
        }

        static public uint FromDateTime(DateTime date)
        {
            DateTime origin = new DateTime(UTCReferenceYear, 1, 1, 0, 0, 0, 0);
            TimeSpan diff = date.ToUniversalTime() - origin;
            uint Time = (uint)diff.TotalSeconds;
            return Time;
        }

        static public uint Now()
        {
            return TimeUTC.FromDateTime(DateTime.UtcNow);
        }
    };

}

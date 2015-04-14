using System;
using System.Collections.Generic;
using BR.Message.Game;
using BR.Message;
using BR;

namespace TestCSharp
{
    class ParsingTest : TestBase
    {
        public ParsingTest()
        {
            TestName = GetType().ToString();
        }

        public override bool RunTest()
        {
            const ulong TestPlayerID = 2387465293;
            const ulong TestTicket = 1234567890;
            MessageHeader header = new MessageHeader();
            header.MsgID = JoinGameCmd.MID;
            header.PeerID = 7532;
            header.Sequence = 10;
            byte[] buffer = new byte[1024];
            BinaryMemoryWriter binWriter = new BinaryMemoryWriter(buffer);
            binWriter.Length = 0;
            binWriter.Seek(0);
            JoinGameCmd.BuildMessageBuffer(binWriter, ref header, TestPlayerID, TestTicket, 0);

            BinaryMemoryReader binReader = new BinaryMemoryReader(buffer);
            binReader.Length = binWriter.Length;
            binReader.Seek(0);
            MessageHeader headerReaden = new MessageHeader();
            headerReaden.Read(binReader);
            JoinGameCmd msg = new JoinGameCmd(ref headerReaden);
            msg.ParseMsg(binReader);

            return true;
        }
    }
}

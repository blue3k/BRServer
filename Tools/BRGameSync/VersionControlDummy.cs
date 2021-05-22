using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using SF;

namespace BR
{
    public class VersionControlDummy : IVersionControl
    {

        public override IList<VersionFileInfo> GetFileList(string remotePath, ListOption listOption = ListOption.None) { return null; }

        public override bool FileExists(string remotePath) { return false; }

        // Mark for delete
        public override void MarkDelete(string remotePath) { }
        public override void MarkChanged(string remotePath) { }
        public override void MarkAdd(string remotePath) { }


        public override Result SetChangeDescription(string description) { return new Result(ResultCode.NOT_IMPLEMENTED); }
        public override Result SubmitChanges() { return new Result(ResultCode.NOT_IMPLEMENTED); }



    }

}



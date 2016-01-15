using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCSharpClient.Server.Transaction
{
    /// <summary>
    /// Transaction class
    /// </summary>
    public abstract class Transaction
    {
        public abstract IEnumerable<bool> ProcessTransaction();
    }
}

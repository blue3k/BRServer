using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BR.Server.Transaction
{

    /// <summary>
    /// Iterator wait
    /// </summary>
    public class WaitResult : IEnumerator<int>
    {
        int m_LastResult;
        IEnumerator<int> m_WaitingIterator;

        public WaitResult(IEnumerator<int> waitingIterator)
        {
            m_WaitingIterator = waitingIterator;
            m_LastResult = 0;
        }

        public int Current { get { return m_LastResult; } }
        object IEnumerator.Current { get { return Current; } }

        public bool MoveNext()
        {
            if (m_WaitingIterator == null)
                return false;

            var working = m_WaitingIterator.MoveNext();
            if(working)
                m_LastResult = m_WaitingIterator.Current;
            return working;
        }

        public void Reset()
        {
            if (m_WaitingIterator == null)
                return;
            m_WaitingIterator.Reset();
        }

        public void Dispose()
        {
            if (m_WaitingIterator == null)
                return;
            m_WaitingIterator.Dispose();
        }
    }


    /// <summary>
    /// Iterator wait until any iterator is finished
    /// </summary>
    public class WaitAny : IEnumerator<int>
    {
        int m_LastResult;
        IEnumerator<int>[] m_WaitingIterators;
        int[] m_IteratorResults;

        public WaitAny(params IEnumerator<int>[] waitingIterators)
        {
            m_WaitingIterators = waitingIterators;
            m_IteratorResults = new int[waitingIterators.Length];

            m_LastResult = 0;
        }


        public int Current { get { return m_LastResult; } }
        object IEnumerator.Current { get { return Current; } }

        public bool MoveNext()
        {
            if (m_WaitingIterators == null || m_WaitingIterators.Length == 0)
                return false;

            int lastResult = 0;
            bool allWorking = false;
            for (int iIter = 0; iIter < m_WaitingIterators.Length; iIter++)
            {
                var iter = m_WaitingIterators[iIter];
                var working = iter.MoveNext();
                //if (working)
                    m_IteratorResults[iIter] = iter.Current;
                if (lastResult == 0 && m_IteratorResults[iIter] != 0)
                    lastResult = m_IteratorResults[iIter];
                allWorking &= working;
            }

            m_LastResult = lastResult;
            return allWorking;
        }

        public void Reset()
        {
            if (m_WaitingIterators == null)
                return;
            for(int iIter = 0; iIter < m_WaitingIterators.Length; iIter++)
            {
                m_WaitingIterators[iIter].Reset();
                m_IteratorResults[iIter] = 0;
            }
        }

        public void Dispose()
        {
            if (m_WaitingIterators == null)
                return;

            foreach (var iter in m_WaitingIterators)
            {
                iter.Dispose();
            }

            m_WaitingIterators = null;
        }
    }


    /// <summary>
    /// Iterator wait until any iterator is finished
    /// </summary>
    public class WaitAll : IEnumerator<int>
    {
        int m_LastResult;
        IEnumerator<int>[] m_WaitingIterators;
        int[] m_IteratorResults;

        public WaitAll(params IEnumerator<int>[] waitingIterators)
        {
            m_WaitingIterators = waitingIterators;
            m_IteratorResults = new int[waitingIterators.Length];

            m_LastResult = 0;
        }


        public int Current { get { return m_LastResult; } }
        object IEnumerator.Current { get { return Current; } }

        public bool MoveNext()
        {
            if (m_WaitingIterators == null || m_WaitingIterators.Length == 0)
                return false;

            int lastResult = 0;
            bool allFinished = false;
            for (int iIter = 0; iIter < m_WaitingIterators.Length; iIter++)
            {
                var iter = m_WaitingIterators[iIter];
                var working = iter.MoveNext();
                //if (working)
                m_IteratorResults[iIter] = iter.Current;
                if (lastResult == 0 && m_IteratorResults[iIter] != 0)
                    lastResult = m_IteratorResults[iIter];
                allFinished &= !working;
            }

            m_LastResult = lastResult;
            return !allFinished;
        }

        public void Reset()
        {
            if (m_WaitingIterators == null)
                return;

            for (int iIter = 0; iIter < m_WaitingIterators.Length; iIter++)
            {
                m_WaitingIterators[iIter].Reset();
                m_IteratorResults[iIter] = 0;
            }
        }

        public void Dispose()
        {
            if (m_WaitingIterators == null)
                return;
            foreach (var iter in m_WaitingIterators)
            {
                iter.Dispose();
            }
            m_WaitingIterators = null;
        }
    }
}



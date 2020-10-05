////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2018 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Zookeeper session
//
////////////////////////////////////////////////////////////////////////////////


using Sce.Atf;
using Sce.Atf.Adaptation;
using Sce.Atf.Applications;
using Sce.Atf.Controls.PropertyEditing;
using SF;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace SFServerManager
{

    /// <summary>
    /// Zookeeper main connection section manager
    /// </summary>
    [Export(typeof(IInitializable))]
    [Export(typeof(ZookeeperSession))]
    [PartCreationPolicy(CreationPolicy.Shared)]
    public class ZookeeperSession : IInitializable
    {
        public SFZookeeper ZKInstance {  get { return m_zkInstance; } }
        public SFServerConfig ServerConfig { get { return m_ServerConfig; } }


        [ImportingConstructor]
        public ZookeeperSession()
        {
        }

        public void Initialize() 
        {
            m_zkInstance = new SFZookeeper();
            m_Timer = new Timer(OnTimer, this, 1000, 1000);
        }

        void OnTimer(object state)
        {
            if (m_Setting == null || string.IsNullOrEmpty(m_Setting.ConnectionString))
                return;

            if (!m_zkInstance.IsConnected())
            {
                if(m_zkInstance.GetState() != SFZookeeper.STATE_CONNECTING)
                    m_zkInstance.Connect(m_Setting.ConnectionString);
                return;
            }

            if (m_ServerConfig == null)
            {
                m_ServerConfig = new SFServerConfig();
                m_ServerConfig.LoadZookeeper(m_zkInstance, m_Setting.ConfigNodePath);
                RefreshServerList();
            }

            SFZookeeper.ZKEvent zkEvent;

            while(m_zkInstance.DequeueEvent(out zkEvent))
            {
                if(zkEvent.EventType == SFZookeeper.EVENT_CHILD)
                {
                    // It should be server list for now
                    RefreshServerList();
                }
            }
        }

        void RefreshServerList()
        {
            var serverListAdapter = m_ServerInstanceView.TreeView.As<ServerListAdapter>();
            if (serverListAdapter == null)
                return;

            m_ServerInstanceView.TreeControl.InvokeIfRequired(() =>
            {
                serverListAdapter.RefreshServerList(m_zkInstance, m_ServerConfig);
            });
        }


        SFServerConfig m_ServerConfig;
        SFZookeeper m_zkInstance;

        Timer m_Timer;

        [Import(AllowDefault = false)]
        private ServerConfigUploadView m_ServerInstanceView = null;

        [Import(AllowDefault =false)]
        ServerManagerSetting m_Setting = null;
    }


}
